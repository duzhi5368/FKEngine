/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Main
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../../include/CommonBase/CommonBaseInsideHead.h"
#include "../../include/Math/MathInsideHead.h"
#include "../../include/Control/ControlInsideHeader.h"
#include "../../include/PhysiceSystem/PhysicsSystemInsideHeader.h"
#include "../../include/RenderSystem/RenderSystemInsideHeader.h"
#include "../../include/SoundSystem/SoundSystemInsideHeader.h"
#include "../../include/UISystem/UISystemInsideHeader.h"
#include "../../include/ParticleSystem/ParticleSystemInsideHeader.h"
#include "../../include/Scene/SceneInsideHeader.h"
#include "../../include/RenderWrapper/RenderWrapperInsideHeader.h"
#include "../../include/CommonBase/AppEntry.h"
using namespace FK3DEngine;

#ifdef _DEBUG 
	#pragma comment( lib, "FK3DEngine_D.lib ")
	#pragma comment( lib, "JpegLib_D.lib" )
	#pragma comment( lib, "freetype_D.lib" )
#else
	#pragma comment( lib, "FK3DEngine.lib ")
	#pragma comment( lib, "JpegLib.lib" )
	#pragma comment( lib, "freetype.lib" )
#endif


//--------------------------------------------------------------------------------------------------------------
#include "resource.h"
#include "Player.h"
class CTestFrameListener;
//--------------------------------------------------------------------------------------------------------------
Root*					g_pEngRoot				= NULL;		// 引擎根级类
RenderWindow*			g_pRenderWindow			= NULL;		// 游戏窗口类
CTestFrameListener*		g_pTestFrameListener	= NULL;		// 帧监听器
SceneManager*			g_pSceneMgr				= NULL;		// 场景管理器
StaticSound*			g_pSound				= NULL;		// 背景音乐
//--------------------------------------------------------------------------------------------------------------
void PlaySound( LONG_PTR pParam )
{
	GuiCheckBox* pChk = (GuiCheckBox*)pParam;

	if( pChk->mbChecked )
		g_pSound->Play();
	else
		g_pSound->Stop();
}
//--------------------------------------------------------------------------------------------------------------
void SetSoundVolume( LONG_PTR pParam )
{
	GuiTrackBar* pTrackBar = (GuiTrackBar*)pParam;

	float fVolume = (float)pTrackBar->mValue / (float)pTrackBar->mRange;
	g_pSound->SetVolume( fVolume );
}
//--------------------------------------------------------------------------------------------------------------
class CTestFrameListener : public FrameListener
{
public:
	CTestFrameListener()
		: mpTextFPS		(NULL)
		, mpTextTrangle	(NULL)
		, mpCamera		(NULL)
		, mbControlView	(true)
		, mbControlPos	(true)
		, mbFreeLook	(true)
	{

	}
	~CTestFrameListener()
	{

	}
public:
	//帧起始事件
	void FrameStart( FrameListenerData* pFrameData )
	{
		//如果按退出键
		if( InputSystem::mpSingleton->mbKeyDown[DIK_ESCAPE] )
			g_pEngRoot->ExitLooping();

		//是否允许控制摄像机
		if( InputSystem::mpSingleton->mbKeyDown[DIK_CAPITAL] )
		{
			mbControlView = !mbControlView;
			mbControlPos = !mbControlPos;
		}

		//是否为自由观看模式
		if( InputSystem::mpSingleton->mbKeyDown[DIK_NUMLOCK] )
			mbFreeLook = !mbFreeLook;

		//更新玩家状态
		static Player::Input sInput;
		memset( &sInput, 0, sizeof(Player::Input) );

		if( mbControlPos )
		{
			sInput.bForward =		( InputSystem::mpSingleton->mbKeyState[DIK_UP]		|| InputSystem::mpSingleton->mbKeyState[DIK_W] );
			sInput.bBackward =		( InputSystem::mpSingleton->mbKeyState[DIK_DOWN]	|| InputSystem::mpSingleton->mbKeyState[DIK_S] );
			sInput.bShiftLeft =		( InputSystem::mpSingleton->mbKeyState[DIK_LEFT]	|| InputSystem::mpSingleton->mbKeyState[DIK_A] );
			sInput.bShiftRight =	( InputSystem::mpSingleton->mbKeyState[DIK_RIGHT]	|| InputSystem::mpSingleton->mbKeyState[DIK_D] );

			sInput.bJump =			( InputSystem::mpSingleton->mbKeyState[DIK_SPACE] );
		}

		if( mbControlView )
		{
			sInput.fViewPitch	= InputSystem::mpSingleton->mMouseRelY;
			sInput.fViewYaw		= InputSystem::mpSingleton->mMouseRelX;
		}

		sInput.fFrameTime = pFrameData->fTimeSinceLastFrame;

		mPlayer.UpdatePlayer( sInput, mbFreeLook ? Player::FreeLook : Player::Walk );

		//更新摄像机
		if( mpCamera != NULL )
			mPlayer.UpdateCamera( mpCamera );

		//更新帧速率和三角形数显示文字
		static wchar_t szFPS[32] = { NULL };
		NumConvert::FloatToUnicode( g_pEngRoot->mFPS, szFPS, 2 );

		static wchar_t szTri[32] = { NULL };
		NumConvert::IntToUnicode( RenderSystem::mNumTriangle, szTri );

		//mpTextFPS->SetText( szFPS );
		//mpTextTrangle->SetText( szTri );

		//处理帧事件
		ProcFrameEvent();
	}

	//帧结束时间
	void FrameEnd( FrameListenerData* pFrameData )
	{

	}

	//设置摄像机
	void SetCamera( Camera* pCamera )
	{
		mpCamera = pCamera;
	}
public:
	TextSurface*	mpTextFPS;		//帧速率显示文字
	TextSurface*	mpTextTrangle;	//三角形数显示文字

	Camera*			mpCamera;		//摄像机
	Player			mPlayer;		//玩家

	bool			mbControlView;	//是否可控制玩家视角
	bool			mbControlPos;	//是否可控制玩家位置
	bool			mbFreeLook;		//是否为自由观看模式
};
//--------------------------------------------------------------------------------------------------------------
//处理窗口消息
LRESULT WINAPI DemoProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	if( g_pEngRoot == NULL )
		return 0;

	// 第一种消息处理方式
	//switch( msg )
	//{
	//case WM_KEYDOWN:
	//	{
	//		Misc::ShowErrMsg( "有Windows按键消息！" );
	//		return 0;
	//	}
	//}

	return g_pEngRoot->WinProc( hWnd, msg, wParam, lParam );
}
//--------------------------------------------------------------------------------------------------------------
int WinMainEntry()
{
	BaseFile sTextureFile;			// 纹理包
	BaseFile sObjectFile;			// 物体对象包
	BaseFile sOverlayFile;			// 面板包
	BaseFile sUVAFile;				// UV动画包
	BaseFile sFontFile;				// 字体包
	BaseFile sShaderFile;			// Shader包
	FKTry
	{
		// 创建引擎
		g_pEngRoot = new Root;

		// 修改配置
		ConfigData sConfigData;
		ConfigManager::GetSingletonPtr()->GetDefaultConfig( &sConfigData );
		sConfigData.sWindowCfg.dwStyle	= WS_OVERLAPPEDWINDOW;
		ConfigManager::GetSingletonPtr()->AddConfig( &sConfigData );
		ConfigManager::GetSingletonPtr()->SetCurrenConfig( 0 );

		// 初始化引擎
		g_pEngRoot->Initialize();

		// 创建游戏窗口
		WindowInfo sWndInfo;
		sWndInfo.sCaption		= "FK3D引擎测试 ： Demo1";
		sWndInfo.bCenter		= true;
		sWndInfo.nLeft			= 0;
		sWndInfo.nTop			= 0;
		sWndInfo.nWidth			= 800;
		sWndInfo.nHeight		= 600;
		sWndInfo.dwStyle		= WS_OVERLAPPEDWINDOW | WS_POPUP | WS_SYSMENU;
		sWndInfo.fnWndProc		= DemoProc;
		sWndInfo.hIcon			= LoadIcon( GetModuleHandle( NULL ), MAKEINTRESOURCE(IDI_ICON1) );
		sWndInfo.hCursor		= NULL;//LoadCursor( NULL, IDC_ARROW );

		// 创建渲染窗口
		g_pRenderWindow	= g_pEngRoot->CreateGameWindow( &sWndInfo );

		if( g_pRenderWindow == NULL )
		{
			Misc::ShowErrMsg( "创建渲染窗口失败！" );
			SafeDelete( g_pEngRoot );
			return 0;
		}

		g_pEngRoot->CreateInputSystem();		//创建输入系统
		g_pEngRoot->CreateSoundSystem();		//创建声音系统
		g_pEngRoot->CreateRenderSystem();		//创建渲染系统
		g_pEngRoot->CreateParticleSystem();		//创建粒子系统

		// 设置纹理包
		TextureManager::GetSingletonPtr()->SetTexturePack( &sTextureFile );
		ObjectFileLoader::GetSingletonPtr()->SetObjectPackFile( &sObjectFile );
		OverlayManager::GetSingletonPtr()->SetOverlayPack( &sOverlayFile );
		UVAManager::GetSingletonPtr()->SetUVAPackFile( &sUVAFile );
		FontManager::GetSingletonPtr()->SetFontPack( &sFontFile );
		
		// 播放背景音乐
		BaseFile		mp3File;
		MP3Decoder		mp3Decoder;
		mp3Decoder.OpenSoundFile( &mp3File, "rc\\BackGroundMusic.mp3" );
	
		g_pSound = SoundSystem::mpSingleton->CreateStaticSound();
		g_pSound->LoadStaticSound( &mp3Decoder );
		g_pSound->Play();

		// 创建场景管理器
		g_pSceneMgr = g_pEngRoot->CreateSceneManager( SMT_Quadtree );
		// 创建场景根节点
		SceneNode* pRootNode = g_pSceneMgr->CreateSceneRootNode();

		// 创建地形
		BaseFile		mapFile;
		mapFile.Open( "rc\\Quadtree.map", OT_Read );
		((QuadSceneManager*)g_pSceneMgr)->LoadFromFile( &mapFile );
		mapFile.Close();

		// 创建天空球
		Texture* pSkyTex = TextureManager::mpSingleton->LoadFromFile( "rc\\Sphere.tga" );
		SkySphere* pSkySphere = g_pSceneMgr->CreateSkySphere();
		pSkySphere->SetSphereTexture( pSkyTex );
		pSkySphere->SetSurfaceRotation( 0.8f, Vector3( 0.0f, 1.0f, 0.0f ) );

		////载入顶点着色器
		//ShaderManager::GetSingletonPtr()->SetProgramPackFile( &sShaderFile );
		//ShaderManager::GetSingletonPtr()->CreateShaderFromFile( "rc\\Shader.scg" );

		//VertexShader* pVS = ShaderManager::GetSingletonPtr()->GetVertexShader( 0 );
		//pVS->SetTransformMatrixParam( 0 );
		//pVS->SetShdaerDiffuseParam( 1 );

		// 创建摄像机
		Camera* pCamera = g_pSceneMgr->CreateCamera();
		if( pCamera != NULL )
		{
			pCamera->SetViewPos( Vector3( 0.0f, 0.0f, -30.0f ) );
		}

		// 创建场景子节点
		SceneNode* pNode = (SceneNode*)pRootNode->CreateChild();
		// 创建灯光
		Light* pLight1 = g_pSceneMgr->CreateLight();
		// 绑定灯光于子节点上
		pNode->AttachLight( pLight1 );
		// 设置灯光属性
		pLight1->mLightType = LT_DIRECTIONAL;
		pLight1->mDiffuse = ColorValue( 1.0f, 0.91f, 0.83f, 0.76f );
		pLight1->mSpecular = ColorValue( 1.0f, 0.95f, 0.81f, 0.67f );
		pLight1->mAmbient = ColorValue( 1.0f, 0.52f, 0.458f, 0.38f );
		pLight1->mDirection = Vector3( -1.0f, -1.0f, 1.0f );
		// 激活灯光
		pLight1->ActiveLight();
		pLight1->EnableLight();

		// 创建帧监听器
		g_pTestFrameListener = new CTestFrameListener;

		g_pTestFrameListener->SetCamera( pCamera );
		// 设置帧监听器
		g_pEngRoot->SetFrameListener( (FrameListener*)g_pTestFrameListener );

		// 创建视口
		Viewport* pViewport = RenderSystem::mpSingleton->CreateViewport( 0, 0,
			RenderSystem::mRenderWidth, RenderSystem::mRenderHeight );
		pViewport->SetCamera( pCamera );

		//创建场景
		TriMeshList::Iterator tit = ObjectFileLoader::mpSingleton->mLoadedTriMeshList.Begin();
		TriMeshList::Iterator tend = ObjectFileLoader::mpSingleton->mLoadedTriMeshList.End();
		for(; tit!=tend; ++tit )
		{
			TriMesh* pMesh = *tit;
			pMesh->InitializeInstanceList( 100, 100 );
			RenderInstance* pInstance = pMesh->CreateInstance();

			if( ObjectFileLoader::mpSingleton->mLoadedSkeletonList.Size() != 0 )
			{
				Skeleton* pSkeleton = ObjectFileLoader::mpSingleton->mLoadedSkeletonList[0];

				pMesh->SetSkeleton( pSkeleton );

				AnimControl* pAnimControl = AnimControlManager::mpSingleton->CreateAnimControl();
				pAnimControl->SetAnimationInfo( pSkeleton->mNumKey, pSkeleton->mIntervalTime );
				pInstance->mpAnimControl = pAnimControl;
			}

			SceneNode* pSubNode = (SceneNode*)pNode->CreateChild();
			pSubNode->AttachRenderInstance( pInstance );
		}

		// 设置材质
		Material* pBumpMat = MaterialManager::mpSingleton->CreateMaterial( "JianDunMaterial" );
		pBumpMat->mAmbient = ColorValue::Black;
		pBumpMat->mDiffuse = ColorValue::Black;
		pBumpMat->mSpecular = ColorValue::Black;
		pBumpMat->mEmissive = ColorValue::Black;

		Texture* pBaseTex = TextureManager::mpSingleton->LoadFromFile( "rc\\jiandun.tga" );
		TextureState* pBaseTexState = pBumpMat->CreateTextureState( 0 );
		pBaseTexState->SetTexture( pBaseTex );

		TriMesh* pMesh = (TriMesh*)GeometryManager::mpSingleton->CreateGeometry( TriMesh::Creator );

		pMesh->InitializeInstanceList( 1, 1 );
		RenderInstance* pInst = pMesh->CreateInstance();

		pNode->AttachRenderInstance( pInst );

		// 创建粒子
		Material* pParticleMat = MaterialManager::GetSingletonPtr()->CreateMaterial( "rc\\IceBall.tga", Transparent );
		MissileParticle::Param sParticleParam;
		sParticleParam.pMaterial		= pParticleMat;
		sParticleParam.fLife			= 4.0f;
		sParticleParam.nBeginAlpha		= 0xFF;
		sParticleParam.nEndAlpha		= 0x0;
		sParticleParam.fBeginSize		= 0.8f;
		sParticleParam.fEndSize			= 3.0f;
		Particle* pParticle = FK3DEngine::ParticleSystem::GetSingletonPtr()->CreateParticle( MissileParticle::Creator, &sParticleParam );\

		// 创建粒子发射器
		MissileEmitter::Param sEmitterParam;
		sEmitterParam.pParticle			= pParticle;
		sEmitterParam.fEmitRate			= 0.3f;
		// 绑定粒子编辑器
		Emitter* pEmitter = FK3DEngine::ParticleSystem::GetSingletonPtr()->CreateEmitter( MissileEmitter::Creator, &sEmitterParam );
		pNode->AttachRenderInstance( pEmitter );

		// 创建大UI面板
		OverlayManager* pOverMgr = g_pEngRoot->mpOverlayManager;
		pOverMgr->SetRenderPlaneSize( RenderSystem::mRenderWidth, RenderSystem::mRenderHeight );

		JointSurface* pBK = pOverMgr->LoadJointSurfaceFromFile( 820.0f, 666.0f, 200.0f, 100.0f, ZOT_Bottom, NULL,
			"rc\\bkg.jpg", PF_A8R8G8B8, TFT_POINT );

		// 创建2D UV图片
		UV4 sUV = { 0.0f, 1.0f, 0.0f, 1.0f };
		// 读取DDS文件
		SpriteSet* pImgSpriteSet = pOverMgr->LoadSpriteSetFromFile( "rc\\water.dds", PF_DXT1, 10, 10 );
		pImgSpriteSet->CreateSprite( 0.0f, RenderSystem::mRenderHeight - 250.0f, 128.0f, 128.0f, ZOT_Top, NULL, &sUV );

		// 创建 UVA 动画
		UVAnimation* pCursorUVA = UVAManager::GetSingletonPtr()->CreateUVAnimation( "rc\\cursor.uva" );
		SpriteSet* pCursorSpriteSet = pOverMgr->LoadSpriteSetFromFile( "rc\\cursor.tga", PF_A8R8G8B8, 1, 1, TFT_POINT );
		// 创建光标
		GuiManager::GetSingletonPtr()->CreateCursor( 32, 32, 3, 3, pCursorSpriteSet, pCursorUVA );

		// 创建字体对象
		FontFace* pFont = FontManager::GetSingletonPtr()->CreateFontFace( "rc\\NGULIM.TTF", 32, 512, 32, 32);

		// 创建字体显示纹理
		OverlaySurface* pCursorOverlay = GuiManager::mpSingleton->mpCursorGui->GetZOrder( ZOT_Bottom );
		TextManager::GetSingletonPtr()->CreateTextSurface( ZOT_Top, pCursorOverlay, pFont, L"FK3DEngine_Demo", 10, 10, 0, 0, 0xFF7FFF7F, TFT_LINEAR );

		TextSurface* pAGPMemText = TextManager::GetSingletonPtr()->CreateTextSurface( ZOT_Top, pCursorOverlay,
			pFont, L"0", 100, 54, 600, 32, 0xffff0000 );

		// 获取可用显存
		UINT nAGPMem = RenderSystem::mpSingleton->GetAvailableTextureMem() >> 20;
		wchar_t szAGPMem[10] = { NULL };
		NumConvert::IntToUnicode( nAGPMem, szAGPMem );
		pAGPMemText->SetText( szAGPMem );

		// 创建对话框
		GuiLoca sDlgLoca = { 0, 0, RenderSystem::mRenderWidth, RenderSystem::mRenderHeight };
		GuiClient sDlgClient( 0, 0, RenderSystem::mRenderWidth, RenderSystem::mRenderHeight );
		GuiDialog* pDialog = GuiManager::GetSingletonPtr()->CreateDialog( ZOT_Top, NULL, &sDlgLoca, &sDlgClient );
		// 创建组合框
		FontFace* pComboFont = FontManager::GetSingletonPtr()->CreateFontFace( "rc\\NGULIM.TTF", 16, 512, 16, 16 );
		SpriteSet* pComboSpriteSet = pOverMgr->LoadSpriteSetFromFile( "rc\\ComboBox.tga", PF_A8R8G8B8, 1, 1, TFT_POINT );
		GuiComboBox::Param sParam1;
		sParam1.pSpriteSet		= pComboSpriteSet;
		sParam1.pFontFace		= pComboFont;
		sParam1.nItemNum		= 10;
		sParam1.nX				= 10;
		sParam1.nY				= 300;
		sParam1.nWidth			= 140;
		sParam1.nChoosenHeight	= 24;
		sParam1.nTextLeft		= 1;
		sParam1.nTextTop		= 1;
		sParam1.nBoarderHeight	= 3;
		sParam1.nItemHeight		= 18;
		sParam1.sChoosenClient.SetClient( 3, 3, 137, 21 );

		GuiComboBox* pCombo = pDialog->CreateComboBox( &sParam1 );
		pCombo->AddItem( L"Free", 0 );
		pCombo->AddItem( L"Knight", 1 );
		pCombo->AddItem( L"Katelinda", 2 );

		// 创建滑动条
		SpriteSet* pSliderSpriteSet = pOverMgr->LoadSpriteSetFromFile( "rc\\TrackBar.tga", PF_A8R8G8B8, 1, 1, TFT_POINT );

		GuiTrackBar::Param sParam2;
		sParam2.pSpriteSet		= pSliderSpriteSet;
		sParam2.nRange			= 100;
		sParam2.nValue			= 100;
		sParam2.nX				= 150;
		sParam2.nY				= 400;
		sParam2.nBkgY			= 7;
		sParam2.nBkgWidth		= 206;
		sParam2.nBkgHeight		= 14;
		sParam2.nProgLeft		= 12;
		sParam2.nProgTop		= 3;
		sParam2.nProgWidth		= 182;
		sParam2.nProgHeight		= 8;
		sParam2.nSliderY		= 0;
		sParam2.nSliderMinX		= 6;
		sParam2.nSliderMaxX		= 188;
		sParam2.nSliderWidth	= 12;
		sParam2.nSliderHeight	= 28;
		sParam2.fnEvent			= SetSoundVolume;	// 创建滑动条回调函数

		GuiTrackBar* pTrackBar = pDialog->CreateTrackBar( &sParam2 );

		// 创建复选框
		SpriteSet* pCheckSpriteSet = pOverMgr->LoadSpriteSetFromFile( "rc\\CheckBox.tga", PF_A8R8G8B8, 1, 1, TFT_POINT );
		
		GuiCheckBox::Param sParam3;
		sParam3.pSpriteSet = pCheckSpriteSet;
		sParam3.sClient.SetClient( 0.0f, 0.0f, 25, 25 );
		sParam3.sLoca.X = 190;
		sParam3.sLoca.Y = 450;
		sParam3.sLoca.Width = 25;
		sParam3.sLoca.Height = 25;
		sParam3.fnEvent = PlaySound;				// 创建复选框回调函数
		pDialog->CreateCheckBox( &sParam3 );

		// 显示光标
		g_pRenderWindow->ShowCursor( true );
		// 设置鼠标移动范围
		g_pEngRoot->ApplyMouseRectToScreen();

		// 设置主角坐标
		g_pTestFrameListener->mPlayer.SetPlayerPos( Vector3( 0.0f, 0.0f, -30.0f ) );
		//载入玩家模型及动画
		g_pTestFrameListener->mPlayer.LoadPlayerModel( "" );

		// 显示游戏窗口
		g_pRenderWindow->ShowGameWindow();

		// 设置渲染状态
		RenderSystem::GetSingletonPtr()->_SetAmbient( ColorValue::Yellow );

		//开始游戏主循环
		g_pEngRoot->StartLooping();

		// 删除摄像机
		g_pSceneMgr->DeleteCamera( pCamera );

		// 释放引擎
		g_pEngRoot->Shutdown();

	}
	FKCatch
	{
		SafeDelete( g_pTestFrameListener );
		SafeDelete( g_pEngRoot );
		ShowExceptionMsg;
	}
#ifndef _DEBUG
	{
		SafeDelete( g_pTestFrameListener );
		SafeDelete( g_pEngRoot );
		ShowUnknownExceptionMsg;
	}
#endif

	SafeDelete( g_pTestFrameListener );
	SafeDelete( g_pEngRoot );
	return 0;
}