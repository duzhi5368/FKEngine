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
*	文件说明：	引擎根级入口类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================
class ParticleSystem;
#include "../../include/UISystem/GuiManager.h"
//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//引擎根级入口类
	class FK_DLL Root : public Singleton< Root >
	{
	public:
		ConfigManager			mConfigManager;			//配置管理器

		ConfigData*				mpCurConfigData;		//当前配置数据指针

		InputSystem*			mpInputSystem;			//输入系统
		SoundSystem*			mpSoundSystem;			//声音系统
		RenderSystem*			mpRenderSystem;			//渲染系统
		BufferManager*			mpBufferManager;		//顶点缓存管理器
		ShaderManager*			mpShaderManager;		//顶点着色器管理器
		TextureManager*			mpTextureManager;		//纹理管理器
		OverlayManager*			mpOverlayManager;		//平面图像管理器
		ParticleSystem*			mpParticleSystem;		//粒子系统

		SceneManager*			mpSceneManager;			//场景管理器
		PhysicsSystem			mPhysicsSystem;			//物理系统

		GuiManager				mGuiManager;			//用户界面管理器
		FontManager				mFontManager;			//字体管理器
		TextManager				mTextManager;			//文字管理器
		MaterialManager			mMaterialManager;		//材质管理器
		UVAManager				mUVAManager;			// UV 动画管理器
		SkeletonManager			mSkeletonManager;		//骨骼管理类
		AnimControlManager		mAnimControlManager;	//动画控制器管理器
		GeometryManager			mGeometryManager;		//几何体管理器

		ObjectFileLoader		mObjectFileLoader;		//物体文件载入器

		RenderWindow*			mpRenderWindow;			//渲染窗口
		FrameListener*			mpFrameListener;		//帧监听器
		Timer					mFrameTimer;			//帧计时器
		Timer					mFPSTimer;				//帧速率统计计时器

		UINT					mRenderFrame;			//已渲染帧
		float					mFPS;					//帧速率
		float					mTimeSinceLastFrame;	//上一帧到当前帧的时间

	public:
		Root();
		~Root();

		//初始化引擎（pConfigFileData 为配置文件数据指针）
		void Initialize( DataChunk* pConfigFileData = NULL );

		//关闭引擎
		void Shutdown();

		//创建游戏窗口
		RenderWindow* CreateGameWindow( WindowInfo* pWindowInfo = NULL, WNDPROC fnWndProc = NULL );

		//使用已存在窗口创建游戏窗口
		RenderWindow* CreateGameWindow( HWND hWnd );

		//创建输入系统
		InputSystem* CreateInputSystem();

		//创建声音系统
		SoundSystem* CreateSoundSystem();

		//创建渲染系统
		RenderSystem* CreateRenderSystem();

		//创建粒子系统
		ParticleSystem* CreateParticleSystem( UINT nMaxParticle = 10000 );

		//创建场景管理器
		SceneManager* CreateSceneManager( SceneManagerType eType );

		//处理帧
		void ProcessFrame();

		//渲染帧
		void RenderFrame( const float fTimeSinceLastFrame );

		//开始引擎主循环
		void StartLooping();

		//退出游戏主循环
		void ExitLooping();

		//游戏窗口内部消息处理函数（必须在传入的窗口消息函数中调用）
		LRESULT WinProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

		//获取游戏窗口指针
		RenderWindow* GetGameWindow();

		//设置帧监听器
		void SetFrameListener( FrameListener* pFrameListener );

		//设置鼠标移动范围为全屏幕
		void ApplyMouseRectToScreen();

	protected:
		//统计帧速率
		void _GetFPS();
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
