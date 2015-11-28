/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	RenderSystem
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

#include "../RenderWrapper/RenderWindow.h"
#include "../RenderWrapper/Light.h"
#include "../RenderWrapper/Material.h"
#include "../RenderWrapper/ConfigDataFormat.h"
#include "Viewport.h"
#include "TextureState.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	class SceneManager;
	class Texture;
	class VertexBuffer;
	class IndexBuffer;
	class VertexDeclaration;
	//--------------------------------------------------------------------------------------------------------------
	//渲染系统类 （纯虚类）
	class FK_DLL RenderSystem : public Singleton< RenderSystem >
	{
	public:
		friend class SceneManager;

	public:
		RenderSystemType	mSystemType;			//渲染系统类型

		bool				mbRun;					//指示渲染系统是否正在运行
		RenderWindow*		mpRenderWindow;			//渲染窗口指针

		RenderSystemCfg*	mpConfig;				//渲染系统配置信息

		static bool			mbSoftVerProc;			//是否正在使用软件顶点处理方式
		static DWORD		mdwDepthMode;			//使用深度缓存模式
		static DWORD		mdwMaxBlendNum;			//硬件支持顶点混合最大矩阵混合数量
		static DWORD		mdwMaxBlendIndex;		//硬件支持顶点混合最大矩阵索引数量

		static UINT			mnAdpter;				//适配器序号
		static UINT			mRenderWidth;			//渲染场景宽
		static UINT			mRenderHeight;			//渲染场景高

		static PixelFormat	mDisplayFormat;			//显示格式
		static UINT			mDisplayColorDepth;		//显示格式色深字节数
		static UINT			mDisplayAlphaBits;		//显示格式 Alpha 值位数

		static DWORD		mMaxTextureNum;			//最大纹理数量
		static DWORD		mMaxStreamNum;			//最大顶点流数量

		static UINT			mNumTriangle;			//上一帧渲染三角形数

		Viewport*			mpViewport;				//当前激活的视口

		bool				mbUseSwapSrcRect;		//是否使用渲染页面交换来源矩形
		bool				mbUseSwapDestRect;		//是否使用渲染页面交换目标矩形
		RECT				mSwapSrcRect;			//渲染页面交换来源矩形
		RECT				mSwapDestRect;			//渲染页面交换目标矩形
		HWND				mhSwapDestWindow;		//渲染页面交换目标窗口句柄

	protected:
		ViewportList		mViewportList;			//视口列表

		ULONGStack			mFreeLightIndex;		//可用灯光索引值

		Texture*			mppLastTexture[8];		//纹理状态数组
		VertexBuffer*		mppLastVB[8];			//顶点流状态数组
		IndexBuffer*		mpLastIB;				//索引流状态
		VertexDeclaration*	mpLastVD;				//顶点声明状态

		RenderState			mRenderState;			//当前渲染状态
		TextureState		mpTextureState[8];		//当前纹理状态

	public:
		RenderSystem( const RenderSystemType eType );
		virtual ~RenderSystem();

		//创建渲染系统
		virtual bool Initialize( RenderWindow* pRenderWindow, RenderSystemCfg* pRenderSystemCfg ) = 0;

		//关闭渲染系统
		virtual void Shutdown() = 0;

		//获取可用显存估计值
		virtual UINT GetAvailableTextureMem() = 0;

		//创建视口
		Viewport* CreateViewport( DWORD dwLeft, DWORD dwTop, DWORD dwWidth, DWORD dwHeight,
			float fMinZ = 0.0f, float fMaxZ = 1.0f );

		//删除视口
		void DeleteViewport( Viewport* pViewport );

		//删除所有视口
		void DeleteAllViewport();

		//返回视口数量
		UINT GetNumViewport();

		//获取当前激活视口
		Viewport* GetActiveViewport();

		//设置渲染页面交换参数
		void SetSwapParam( RECT* pSrcRect = NULL, RECT* pDestRect = NULL, HWND hDestWindow = NULL );

		//设置视口
		virtual void FKFastcall _SetViewport( Viewport* pViewport ) = 0;

		//设置观察矩阵
		virtual void FKFastcall _SetViewMatrix( const Matrix4* pViewMatrix ) = 0;

		//设置投影矩阵
		virtual void FKFastcall _SetProjMatrix( const Matrix4* pProjMatrix ) = 0;

		//获取观察矩阵
		virtual void FKFastcall _GetViewMatrix( Matrix4* pViewMatrix ) = 0;

		//获取投影矩阵
		virtual void FKFastcall _GetProjMatrix( Matrix4* pProjMatrix ) = 0;

		//获取渲染矩阵
		virtual void FKFastcall _GetWorldMatrix( Matrix4* pWorldMatrix, int nIndex = 0 ) = 0;

		//设置渲染矩阵
		virtual void FKFastcall _SetWorldMatrix( const Matrix4* pWorldMatrix, int nIndex = 0 ) = 0;

		//设置材质
		virtual void FKFastcall _SetMaterial( const Material* pMaterial ) = 0;

		//设置纹理
		virtual void FKFastcall _SetTexture( const Texture* pTexture, const DWORD dwStage ) = 0;

		//设置顶点声明
		virtual void FKFastcall _SetVertexDeclaration( const VertexDeclaration* pVertexDeclaration ) = 0;

		//设置顶点流
		virtual void FKFastcall _SetVertexStream( const VertexBuffer* pVertexBuf, const DWORD dwStream ) = 0;

		//设置索引流
		virtual void FKFastcall _SetIndexStream( const IndexBuffer* pIndexBuf ) = 0;

		//渲染顶点（索引顶点）
		virtual void FKFastcall _RenderVertex( PrimitiveType ePrimType, int nVerOffset, UINT nMinIndex,
			UINT nNumVer, UINT nIndexOffset, UINT nPrimCount ) = 0;

		//渲染顶点
		virtual void FKFastcall _RenderVertex( PrimitiveType ePrimType, UINT nVerOffset, UINT nPrimCount ) = 0;

		//准备设备
		virtual void FKFastcall _PrepareDevice() = 0;

		//开始渲染
		virtual void FKFastcall _BeginRender() = 0;

		//结束渲染
		virtual void FKFastcall _EndRender() = 0;

		//交换渲染页面
		virtual void FKFastcall _SwapRenderBuffer() = 0;

		//获取渲染状态
		virtual void FKFastcall _GetRenderState( RenderState* pRenderState ) = 0;

		//设置渲染状态
		virtual void FKFastcall _SetRenderState( const RenderState* pRenderState ) = 0;

		//获取纹理状态
		virtual void FKFastcall _GetTextureState( TextureState* pTextureState, UINT nStage ) = 0;

		//设置纹理状态
		virtual void FKFastcall _SetTextureState( const TextureState* pTextureState, UINT nStage ) = 0;

		//关闭纹理状态
		virtual void FKFastcall _DisableTextureState( UINT nStage ) = 0;

		//设置光照状态
		virtual void FKFastcall _SetLightingState( bool bEnable ) = 0;

		//设置高光状态
		virtual void FKFastcall _SetSpecularState( bool bEnable ) = 0;

		//设置雾化状态
		virtual void FKFastcall _SetFogState( bool bEnable ) = 0;

		//设置深度测试状态
		virtual void FKFastcall _SetDepthTestState( bool bEnable ) = 0;

		//设置深度写入状态
		virtual void FKFastcall _SetDepthWriteState( bool bEnable ) = 0;

		//设置 Alpha 混合状态
		virtual void FKFastcall _SetAlphaState( bool bEnable ) = 0;

		//设置 Alpha 测试状态
		virtual void FKFastcall _SetAlphaTestState( bool bEnable ) = 0;

		//设置纹理过滤类型
		virtual void FKFastcall _SetTextureFilter( SamplerType eSamplerType, TextureFilterType eFilterType, UINT nStage = 0 ) = 0;

		//设置纹理寻址类型
		virtual void FKFastcall _SetTextureAddress( SamplerType eSamplerType, TextureAddressType eAddressType, UINT nStage = 0 ) = 0;

		//激活灯光
		virtual bool FKFastcall _ActiveLight( Light* pLight, const bool bActive = true ) = 0;

		//设置灯光状态
		virtual void FKFastcall _SetLightState( const Light* pLight ) = 0;

		//设置环境光
		virtual void FKFastcall _SetAmbient( const ColorValue& color ) = 0;

		//设置材质颜色来源
		virtual void FKFastcall _SetMaterialSource( const MaterialSource& refMatSor ) = 0;

		//设置索引混合状态
		virtual void FKFastcall _SetIndexVertexBlend( const UINT nNumBlendMatrix ) = 0;

		//设置顶点处理方式
		virtual void FKFastcall _SetSoftwareVertexProcess( bool bSoftProc ) = 0;
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
