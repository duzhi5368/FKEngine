/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	D3D9RenderSystem
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	D3D9 渲染系统类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================
struct IDirect3D9;
struct IDirect3DDevice9;
//--------------------------------------------------------------------------------------------------------------

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//D3D9 渲染系统类
	class FK_DLL D3D9RenderSystem : public RenderSystem
	{
	public:
		static IDirect3D9*				mpD3D9;				// D3D9 主对象
		static D3DCAPS9					mD3D9HardCaps;		// D3D9 硬件支持性能信息
		static D3DCAPS9					mD3D9CurCaps;		// D3D9 当前模式性能信息
		static IDirect3DDevice9*		mpD3D9Device;		// D3D9 设备

		static D3DDEVTYPE				mDeviceType;		// D3D9 设备类型
		static DWORD					mD3DBehavior;		// D3D9 设备顶点处理方式

		static D3DPRESENT_PARAMETERS	mD3Dpp;				// D3D9 设备信息

	public:
		D3D9RenderSystem();
		~D3D9RenderSystem();

		//创建渲染系统
		bool Initialize( RenderWindow* pRenderWindow, RenderSystemCfg* pRenderSystemCfg );

		//关闭渲染系统
		void Shutdown();

		//获取可用显存估计值
		UINT GetAvailableTextureMem();

	public:
		//检测显示模式是否可用
		PixelFormat _CheckDisplayMode( UINT nAdapter, UINT nWidth, UINT nHeight, PixelFormat ePF, int nRefreshRate );

		//设置 D3D9 设备信息
		void _SetD3DPresentParam();

		//释放设备资源
		void _InvalidateDeviceObjects();

		//重载设备资源
		void _RestoreDeviceObjects();

	public:
		//设置视口
		void FKFastcall _SetViewport( Viewport* pViewport );

		//设置观察矩阵
		void FKFastcall _SetViewMatrix( const Matrix4* pViewMatrix );

		//设置投影矩阵
		void FKFastcall _SetProjMatrix( const Matrix4* pProjMatrix );

		//设置渲染矩阵
		void FKFastcall _SetWorldMatrix( const Matrix4* pWorldMatrix, int nIndex = 0 );

		//获取观察矩阵
		void FKFastcall _GetViewMatrix( Matrix4* pViewMatrix );

		//获取投影矩阵
		void FKFastcall _GetProjMatrix( Matrix4* pProjMatrix );

		//获取渲染矩阵
		void FKFastcall _GetWorldMatrix( Matrix4* pWorldMatrix, int nIndex = 0 );

		//设置材质
		void FKFastcall _SetMaterial( const Material* pMaterial );

		//设置纹理
		void FKFastcall _SetTexture( const Texture* pTexture, const DWORD dwStage );

		//设置顶点声明
		void FKFastcall _SetVertexDeclaration( const VertexDeclaration* pVertexDeclaration );

		//设置顶点流
		void FKFastcall _SetVertexStream( const VertexBuffer* pVertexBuf, const DWORD dwStream );

		//设置索引流
		void FKFastcall _SetIndexStream( const IndexBuffer* pIndexBuf );

		//渲染顶点（索引顶点）
		void FKFastcall _RenderVertex( PrimitiveType ePrimType, int nVerOffset, UINT nMinIndex,
			UINT nNumVer, UINT nIndexOffset, UINT nPrimCount );

		//渲染顶点
		void FKFastcall _RenderVertex( PrimitiveType ePrimType, UINT nVerOffset, UINT nPrimCount );

		//准备设备
		void FKFastcall _PrepareDevice();

		//开始渲染
		void FKFastcall _BeginRender();

		//结束渲染
		void FKFastcall _EndRender();

		//交换渲染页面
		void FKFastcall _SwapRenderBuffer();

		//获取渲染状态
		void FKFastcall _GetRenderState( RenderState* pRenderState );

		//设置渲染状态
		void FKFastcall _SetRenderState( const RenderState* pRenderState );

		//获取纹理状态
		void FKFastcall _GetTextureState( TextureState* pTextureState, UINT nStage );

		//设置纹理状态
		void FKFastcall _SetTextureState( const TextureState* pTextureState, UINT nStage );

		//关闭纹理状态
		void FKFastcall _DisableTextureState( UINT nStage );

		//设置光照状态
		void FKFastcall _SetLightingState( bool bEnable );

		//设置高光状态
		void FKFastcall _SetSpecularState( bool bEnable );

		//设置雾化状态
		void FKFastcall _SetFogState( bool bEnable );

		//设置深度测试状态
		void FKFastcall _SetDepthTestState( bool bEnable );

		//设置深度写入状态
		void FKFastcall _SetDepthWriteState( bool bEnable );

		//设置 Alpha 混合状态
		void FKFastcall _SetAlphaState( bool bEnable );

		//设置 Alpha 测试状态
		void FKFastcall _SetAlphaTestState( bool bEnable );

		//设置纹理过滤类型
		void FKFastcall _SetTextureFilter( SamplerType eSamplerType, TextureFilterType eFilterType, UINT nStage = 0 );

		//设置纹理寻址类型
		void FKFastcall _SetTextureAddress( SamplerType eSamplerType, TextureAddressType eAddressType, UINT nStage = 0 );

		//激活灯光
		bool FKFastcall _ActiveLight( Light* pLight, const bool bActive = true );

		//设置灯光状态
		void FKFastcall _SetLightState( const Light* pLight );

		//设置环境光
		void FKFastcall _SetAmbient( const ColorValue& color );

		//设置材质颜色来源
		void FKFastcall _SetMaterialSource( const MaterialSource& refMatSor );

		//设置索引混合状态
		void FKFastcall _SetIndexVertexBlend( const UINT nNumBlendMatrix );

		//设置顶点处理方式
		void FKFastcall _SetSoftwareVertexProcess( bool bSoftProc );
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
