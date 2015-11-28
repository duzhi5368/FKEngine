/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Root
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../../include/RenderWrapper/RenderWrapperInsideHeader.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	Root::Root()
		: mpCurConfigData		(NULL)
		, mpRenderWindow		(NULL)
		, mpInputSystem			(NULL)
		, mpSoundSystem			(NULL)
		, mpRenderSystem		(NULL)
		, mpBufferManager		(NULL)
		, mpShaderManager		(NULL)
		, mpTextureManager		(NULL)
		, mpOverlayManager		(NULL)
		, mpParticleSystem		(NULL)
		, mpSceneManager		(NULL)
		, mpFrameListener		(NULL)
		, mRenderFrame			(0)
		, mFPS					(0.0f)
		, mTimeSinceLastFrame	(0.0f)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	Root::~Root()
	{
		Shutdown();
	}
	//--------------------------------------------------------------------------------------------------------------
	//初始化引擎
	void Root::Initialize( DataChunk* pConfigFileData )
	{
		//获取 CPU 信息
		const CPUInfo* pCPUInfo = ProcessorInfo::GetCPUInfo();

		//初始化数学库
		InitializeMathLibrary( pCPUInfo );

		//如果传入配置文件数据指针
		if( pConfigFileData != NULL )
		{
			mConfigManager.LoadConfigFileData( pConfigFileData );
		}
		else
		{
			mConfigManager.AddConfig( &ConfigManager::mDefConfigData );
		}

		//获取当前配置数据指针
		mConfigManager.SetCurrenConfig( 0 );
		mpCurConfigData = mConfigManager.GetCurrenConfig();
	}
	//--------------------------------------------------------------------------------------------------------------
	//关闭引擎
	void Root::Shutdown()
	{
		SafeDelete( mpSceneManager );
		SafeDelete( mpParticleSystem );

		mGuiManager.DeleteAllDialog();
		mGuiManager.DeleteCursor();
		mFontManager.DeleteAllFont();
		mTextManager.DeleteAllTextSurface();
		mSkeletonManager.DeleteAllSkeleton();
		mAnimControlManager.DeleteAllAnimControl();
		mGeometryManager.DeleteAllGeometry();
		mMaterialManager.DeleteAllMaterial();

		mPhysicsSystem.DeleteAllRigidBody();
		mPhysicsSystem.DeleteAllBoundVolume();

		SafeDelete( mpOverlayManager );
		SafeDelete( mpTextureManager );
		SafeDelete( mpShaderManager );
		SafeDelete( mpBufferManager );

		SafeDelete( mpRenderSystem );
		SafeDelete( mpSoundSystem );
		SafeDelete( mpInputSystem );

		SafeDelete( mpRenderWindow );
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建游戏窗口
	RenderWindow* Root::CreateGameWindow( WindowInfo* pWindowInfo, WNDPROC fnWndProc )
	{
		mpRenderWindow = new RenderWindow;

		//如果存在窗口创建参数
		if( pWindowInfo != NULL )
		{
			mpRenderWindow->Create( pWindowInfo );
			return mpRenderWindow;
		}

		//否则使用配置信息创建窗口
		WindowInfo WndInfo;
		WndInfo.sCaption	= mpCurConfigData->sWindowCfg.szCaption;
		WndInfo.bCenter		= mpCurConfigData->sWindowCfg.bCenter;
		WndInfo.nLeft		= mpCurConfigData->sWindowCfg.nLeft;
		WndInfo.nTop		= mpCurConfigData->sWindowCfg.nTop;
		WndInfo.nWidth		= mpCurConfigData->sWindowCfg.nWidth;
		WndInfo.nHeight		= mpCurConfigData->sWindowCfg.nHeight;
		WndInfo.dwStyle		= mpCurConfigData->sWindowCfg.dwStyle;
		WndInfo.fnWndProc	= fnWndProc;
		WndInfo.hIcon		= LoadIcon( NULL, IDI_WINLOGO );
		WndInfo.hCursor		= LoadCursor( NULL, IDC_ARROW );

		mpRenderWindow->Create( &WndInfo );

		return mpRenderWindow;
	}
	//--------------------------------------------------------------------------------------------------------------
	//使用已存在窗口创建游戏窗口
	RenderWindow* Root::CreateGameWindow( HWND hWnd )
	{
		if( hWnd == NULL )
			return NULL;

		mpRenderWindow = new RenderWindow;
		mpRenderWindow->CreateByExist( hWnd );
		return mpRenderWindow;
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建输入系统
	InputSystem* Root::CreateInputSystem()
	{
		//根据输入系统配置类型创建输入系统
		switch ( mpCurConfigData->sInputSystemCfg.eInputSystemType )
		{
		case IST_DirectInput8:
			{
				mpInputSystem = new DI8InputSystem;
				break;
			}
		default:
			Except( Exception::ERR_INTERNAL_ERROR, "无法识别的输入系统类型。" );
		}

		//初始化输入系统
		mpInputSystem->Initialize();

		//初始化键盘和鼠标
		mpInputSystem->InitializeKeyboard( mpRenderWindow->mhWnd, mpCurConfigData->sInputSystemCfg.bExclusiveKeyboard );
		mpInputSystem->InitializeMouse( mpRenderWindow->mhWnd, mpCurConfigData->sInputSystemCfg.bExclusiveMouse );

		return mpInputSystem;
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建声音系统
	SoundSystem* Root::CreateSoundSystem()
	{
		//根据输入系统配置类型创建输入系统
		switch ( mpCurConfigData->sSoundSystemCfg.eSoundSystemType )
		{
		case SST_DirectSound8:
			{
				mpSoundSystem = new DS8SoundSystem;
				break;
			}
		default:
			Except( Exception::ERR_INTERNAL_ERROR, "无法识别的声音系统类型。" );
		}

		mpSoundSystem->Initialize( &mpCurConfigData->sSoundSystemCfg, mpRenderWindow );

		return mpSoundSystem;
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建渲染系统
	RenderSystem* Root::CreateRenderSystem()
	{
		//根据渲染系统配置类型创建渲染系统
		switch ( mpCurConfigData->sRenderSystemCfg.eRenderSystemType )
		{
		case RST_Direct3D9:
			{
				mpRenderSystem = new D3D9RenderSystem;
				if( !mpRenderSystem->Initialize( mpRenderWindow, &mpCurConfigData->sRenderSystemCfg ) )
					return NULL;

				mpBufferManager		= new D3D9BufferManager;
				mpShaderManager		= new D3D9ShaderManager;
				mpTextureManager	= new D3D9TextureManager;
				break;
			}
		case RST_OpenGL:
			{
				break;
			}
		default:
			Except( Exception::ERR_INTERNAL_ERROR, "无法识别的渲染系统类型。" );
		}

		//初始化各种管理器
		mpOverlayManager = new OverlayManager;

		mpBufferManager->Initialize();
		mpOverlayManager->Initialize();

		mMaterialManager.Initialize( 100 );
		mGeometryManager.Initialize( 200, 200 );

		return mpRenderSystem;
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建粒子系统
	ParticleSystem* Root::CreateParticleSystem( UINT nMaxParticle )
	{
		mpParticleSystem = new ParticleSystem( nMaxParticle );
		return mpParticleSystem;
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建场景管理器
	SceneManager* Root::CreateSceneManager( SceneManagerType eType )
	{
		//根据场景管理器类型创建场景管理器
		switch (eType)
		{
		case SMT_Basic:
			{
				mpSceneManager = new SceneManager;
				break;
			}
		case SMT_BSP:
			{
				mpSceneManager = new BSPSceneManager;
				break;
			}
		case SMT_Quadtree:
			{
				mpSceneManager = new QuadSceneManager;
				break;
			}
		case SMT_Octree:
			{
				mpSceneManager = new OctSceneManager;
				break;
			}
		default:
			Except( Exception::ERR_INVALIDPARAMS, "无法识别的场景管理器类型。" );
		}

		return mpSceneManager;
	}
	//--------------------------------------------------------------------------------------------------------------
	//处理帧
	void Root::ProcessFrame()
	{
		//获取输入信息
		mpInputSystem->UpdateKeyboard();
		mpInputSystem->UpdateMouse();


		//重新开始计时
		float fTimeSinceLastFrame = mFrameTimer.GetTimeDouble();
		mFrameTimer.Start();

		//处理帧起始事件
		static FrameListenerData sFrameData;
		sFrameData.fTimeSinceLastFrame = fTimeSinceLastFrame;
		mpFrameListener->FrameStart( &sFrameData );

		//处理用户界面输入
		mGuiManager.ProcGuiInput( mpInputSystem, fTimeSinceLastFrame );

		//渲染帧
		RenderFrame( fTimeSinceLastFrame );

		//处理帧结束事件
		mpFrameListener->FrameEnd( &sFrameData );


		//统计帧速率
		_GetFPS();
	}
	//--------------------------------------------------------------------------------------------------------------
	//渲染帧
	void Root::RenderFrame( const float fTimeSinceLastFrame )
	{
		mTimeSinceLastFrame = fTimeSinceLastFrame;

		RenderSystem::mNumTriangle = 0;

		//更新动画帧延时
		mAnimControlManager.UpdateFrameTime( fTimeSinceLastFrame );
		mpOverlayManager->_UpdateFrameTime( fTimeSinceLastFrame );

		//渲染场景
		mpSceneManager->RenderScene( fTimeSinceLastFrame );
	}
	//--------------------------------------------------------------------------------------------------------------
	//开始引擎主循环
	void Root::StartLooping()
	{
		//开始进行帧计时
		mFrameTimer.Start();
		mFPSTimer.Start();

		//进入游戏主循环

		//如果不是退出消息
		while( mpRenderWindow->mMsg.message != WM_QUIT )
		{
			//如果有消息
			if( PeekMessage( &mpRenderWindow->mMsg, NULL, 0, 0, PM_REMOVE ) )
			{
				//处理消息
				TranslateMessage( &mpRenderWindow->mMsg );
				DispatchMessage( &mpRenderWindow->mMsg );
			}
			//如果没有消息
			else
			{
				//如果窗口处于激活状态
				if( mpRenderWindow->mbActive )
				{
					//处理游戏帧
					ProcessFrame();
				}
				//如果窗口被销毁
				else if( mpRenderWindow->mbDestroy )
				{
					return;
				}
				//无消息且不运行时则等待消息（将控制权交给操作系统）
				else
				{
					WaitMessage();
				}
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//退出游戏主循环
	void Root::ExitLooping()
	{
		PostQuitMessage( 0 );
	}
	//--------------------------------------------------------------------------------------------------------------
	//游戏窗口内部消息处理函数（必须在传入的窗口消息函数中调用）
	LRESULT Root::WinProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		return mpRenderWindow->WinProc( hWnd, msg, wParam, lParam );
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取游戏窗口指针
	RenderWindow* Root::GetGameWindow()
	{
		return mpRenderWindow;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置帧监听器
	void Root::SetFrameListener( FrameListener* pFrameListener )
	{
		mpFrameListener = pFrameListener;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置鼠标移动范围为全屏幕
	void Root::ApplyMouseRectToScreen()
	{
		MouseRect sMouseRect;
		sMouseRect.Left		= 0;
		sMouseRect.Right	= mpRenderSystem->mRenderWidth;
		sMouseRect.Top		= 0;
		sMouseRect.Bottom	= mpRenderSystem->mRenderHeight;
		sMouseRect.WheelMin	= -180;
		sMouseRect.WheelMax	= 180;

		mpInputSystem->SetMouseRect( &sMouseRect );
	}
	//--------------------------------------------------------------------------------------------------------------
	//统计帧速率
	void Root::_GetFPS()
	{
		//增加已渲染帧记录值
		++mRenderFrame;

		//获取从上一次统计帧速率到现在的延时
		double DelayTime = mFPSTimer.GetTimeDouble();

		//每隔 0.25 秒钟检测一次（注意：由于时钟误差，检测频率越高，误差越大。）
		if( DelayTime > 0.25 )
		{
			//将渲染的帧数除以渲染时间得出帧/秒
			mFPS = (float)( (double)mRenderFrame / DelayTime );

			mFPSTimer.Start();	//重新开始计时
			mRenderFrame = 0;	//重置帧记数
		}
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
