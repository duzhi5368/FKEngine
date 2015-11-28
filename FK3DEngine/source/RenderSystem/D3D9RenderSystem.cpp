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
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../../include/RenderSystem/RenderSystemInsideHeader.h"
#include "../../../Depend/dx9/d3dx9.h"
#include "../../../Depend/dx9/dxerr9.h"
#include "../../include/CommonBase/MemoryMarco.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	IDirect3D9*				D3D9RenderSystem::mpD3D9			= NULL;				// D3D9 主对象
	D3DCAPS9				D3D9RenderSystem::mD3D9CurCaps;							// D3D9 硬件支持性能信息
	D3DCAPS9				D3D9RenderSystem::mD3D9HardCaps;						// D3D9 当前模式性能信息
	IDirect3DDevice9*		D3D9RenderSystem::mpD3D9Device		= NULL;				// D3D9 设备
	D3DDEVTYPE				D3D9RenderSystem::mDeviceType		= D3DDEVTYPE_HAL;	// D3D9 设备类型
	DWORD					D3D9RenderSystem::mD3DBehavior		= 0;				// D3D9 设备顶点处理方式
	D3DPRESENT_PARAMETERS	D3D9RenderSystem::mD3Dpp			= { NULL };			// D3D9 设备信息
	//--------------------------------------------------------------------------------------------------------------
	D3D9RenderSystem::D3D9RenderSystem()
		: RenderSystem	( RST_Direct3D9 )
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	D3D9RenderSystem::~D3D9RenderSystem()
	{
		Shutdown();
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建渲染系统
	bool D3D9RenderSystem::Initialize( RenderWindow* pRenderWindow, RenderSystemCfg* pRenderSystemCfg )
	{
		HRESULT result = E_FAIL;

		//存储渲染系统配置信息指针
		mpConfig = pRenderSystemCfg;

		//存储渲染窗口指针
		mpRenderWindow = pRenderWindow;

		//存储使用的适配器序号
		mnAdpter = mpConfig->sD3D9Cfg.nAdapter;

		//存储渲染场景尺寸
		mRenderWidth	= mpConfig->sD3D9Cfg.nWidth;
		mRenderHeight	= mpConfig->sD3D9Cfg.nHeight;

		//建立主D3D对象
		mpD3D9 = Direct3DCreate9( D3D_SDK_VERSION );
		if( mpD3D9 == NULL )
			Except( Exception::ERR_RENDER_API_ERROR, "创建 D3D9 主对象失败。" );

		//设置设备信息
		_SetD3DPresentParam();

		//获取 D3D9 硬件支持性能信息
		result = mpD3D9->GetDeviceCaps( mpConfig->sD3D9Cfg.nAdapter, D3DDEVTYPE_HAL, &mD3D9HardCaps );
		if( FAILED( result ) )
			Except( Exception::ERR_RENDER_API_ERROR, (String)"无法获取显卡性能信息。"
			"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );


		//判断是否支持 W 深度缓存
		mdwDepthMode = ( mD3D9HardCaps.RasterCaps & D3DPRASTERCAPS_WBUFFER ) ? D3DZB_USEW : D3DZB_TRUE;

		//检查是否必须使用软件顶点处理
		bool bMustUseSoftVertexProc = false;

		//判断是否支持指定的着色器
		DWORD dwVerVS = D3DVS_VERSION( mpConfig->sD3D9Cfg.nVS_MajorVer, mpConfig->sD3D9Cfg.nVS_MinorVer );
		DWORD dwVerPS = D3DPS_VERSION( mpConfig->sD3D9Cfg.nPS_MajorVer, mpConfig->sD3D9Cfg.nPS_MinorVer );
		if( dwVerVS > mD3D9HardCaps.VertexShaderVersion || dwVerPS > mD3D9HardCaps.PixelShaderVersion )
			bMustUseSoftVertexProc = true;

		//硬件支持顶点混合最大设置矩阵数量和最大矩阵索引值数量
		mdwMaxBlendNum = mD3D9HardCaps.MaxVertexBlendMatrices;
		mdwMaxBlendIndex = mD3D9HardCaps.MaxVertexBlendMatrixIndex;
		if( mdwMaxBlendIndex < 32 || mdwMaxBlendNum < 4 )
		{
			bMustUseSoftVertexProc = true;
			mbSoftVerProc = true;
		}

		mD3DBehavior = bMustUseSoftVertexProc ? D3DCREATE_MIXED_VERTEXPROCESSING : D3DCREATE_HARDWARE_VERTEXPROCESSING;


		//如果要求编译为支持 NVPerfHUD 的性能调试版本
#ifdef _NVPerfHUD

		//要启用 NVPerfHUD 的性能调试功能，请确认您是在 NVPerfHUD 环境下运行的本程序
		//并且当前使用的显卡型号为 NVIDIA Geforce3 及其后续系列，同时安装了 ForceWare 71.80 及以上版本的驱动程序。

		//创建支持 NVPerfHUD 的 Direct3D 设备
		mpConfig->sD3D9Cfg.nAdapter = mpD3D9->GetAdapterCount() - 1;
		mDeviceType = D3DDEVTYPE_REF;

		result = mpD3D9->CreateDevice( mpConfig->sD3D9Cfg.nAdapter, mDeviceType, mpRenderWindow->mhWnd, 
			mD3DBehavior, &mD3Dpp, &mpD3D9Device );
		if( FAILED( result ) )
			Except( Exception::ERR_RENDER_API_ERROR, (String)"创建支持 NVIDIA NVPerfHUD 性能调试器"
			"的 Direct3D9 显示设备失败。\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );

#else

		//创建 Direct3D 设备
		mDeviceType = D3DDEVTYPE_HAL;
		result = mpD3D9->CreateDevice( mpConfig->sD3D9Cfg.nAdapter, mDeviceType, mpRenderWindow->mhWnd, 
			mD3DBehavior, &mD3Dpp, &mpD3D9Device );
		if( FAILED( result ) )
		{
			//如果创建失败则尝试使用软件顶点处理
			mD3DBehavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
			result = mpD3D9->CreateDevice( mpConfig->sD3D9Cfg.nAdapter, mDeviceType, mpRenderWindow->mhWnd, 
				mD3DBehavior, &mD3Dpp, &mpD3D9Device );
			if( FAILED( result ) )
			{
				//如果创建失败则尝试使用软件 3D 加速
				mDeviceType = D3DDEVTYPE_REF;
				result = mpD3D9->CreateDevice( mpConfig->sD3D9Cfg.nAdapter, mDeviceType, mpRenderWindow->mhWnd, 
					mD3DBehavior, &mD3Dpp, &mpD3D9Device );
				if( FAILED( result ) )
				{
					Except( Exception::ERR_RENDER_API_ERROR,
						(String)"创建 Direct3D9 显示设备失败，这可能是由于您的硬件不支持某些功能或是软件问题造成的。"
						"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
				}
				else
				{
					if( IDCANCEL == Misc::ShowMsg( "由于您的硬件不支持某些指定的特性，程序将使用软件方式模拟，"
						"但这一般会非常慢。\n点击确定继续执行程序，点击取消退出程序。",
						"注意", MB_OKCANCEL | MB_ICONINFORMATION ) )
						return false;
				}
			}
			else
			{
				Misc::ShowMsg( "由于您的设备不支持硬件顶点处理，程序将使用软件顶点处理。",
					"注意", MB_OK | MB_ICONINFORMATION );
			}
		}

#endif

		//获取 D3D9 当前模式性能信息
		result = mpD3D9->GetDeviceCaps( mpConfig->sD3D9Cfg.nAdapter, mDeviceType, &mD3D9CurCaps );
		if( FAILED( result ) )
			Except( Exception::ERR_RENDER_API_ERROR, (String)"无法获取显卡性能信息。"
			"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );

		//存储当前显示格式
		mDisplayFormat		= pRenderSystemCfg->sD3D9Cfg.eColorFormat;
		mDisplayColorDepth	= Misc::GetColorDepth( mDisplayFormat );
		mDisplayAlphaBits	= Misc::GetColorAlphaBits( mDisplayFormat );

		//将所有可用灯光索引值存入堆栈
		DWORD nNumFreeLight = mD3D9CurCaps.MaxActiveLights;
		for( DWORD index=0; index<nNumFreeLight; ++index )
			*mFreeLightIndex.Push() = index;

		//存储最大可使用的纹理和顶点流数量
		mMaxTextureNum	= ( mD3D9CurCaps.MaxSimultaneousTextures <= 8 ) ? mD3D9CurCaps.MaxSimultaneousTextures : 8;
		mMaxStreamNum	= ( mD3D9CurCaps.MaxStreams <= 8 ) ? mD3D9CurCaps.MaxStreams : 8;

		//获取顶点处理方式
		mbSoftVerProc = ( mpD3D9Device->GetSoftwareVertexProcessing() == TRUE );

		//清空页面缓存
		mpD3D9Device->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0, 1.0f, 0 );

		//获取初始渲染状态
		_GetRenderState( &mRenderState );

		//获取初始纹理状态
		for( UINT i=0; i<8; ++i )
			_GetTextureState( &mpTextureState[i], i );

		return true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//关闭渲染系统
	void D3D9RenderSystem::Shutdown()
	{
		SafeRelease( mpD3D9Device );
		SafeRelease( mpD3D9 );
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取可用显存估计值
	UINT D3D9RenderSystem::GetAvailableTextureMem()
	{
		return mpD3D9Device->GetAvailableTextureMem();
	}
	//--------------------------------------------------------------------------------------------------------------
	//检测显示模式是否可用
	PixelFormat D3D9RenderSystem::_CheckDisplayMode( UINT nAdapter, UINT nWidth, UINT nHeight, PixelFormat ePF,
		int nRefreshRate )
	{
		D3DDISPLAYMODE D3Ddm;

		//获取要检测的颜色格式
		D3DFORMAT d3dfmt = D3D9TypeGet::GetPixelFormat( ePF );

		//检测适配器序号是否正确
		UINT nAdapterCount = mpD3D9->GetAdapterCount();

		//检测颜色格式
		if( nAdapter >= 0 && nAdapter <= nAdapterCount )
		{
			//循环检测每一个显示模式
			UINT nModeCount = mpD3D9->GetAdapterModeCount( nAdapter, d3dfmt );
			for( UINT i=0; i<nModeCount; ++i )
			{
				//获取显示模式
				mpD3D9->EnumAdapterModes( nAdapter, d3dfmt, i, &D3Ddm );

				//如果为全屏模式则检测宽度、高度是否符合要求
				if( mpConfig->bWindowed || ( D3Ddm.Width == nWidth && D3Ddm.Height == nHeight ) )
				{
					//检测刷新率是否符合要求
					if( D3Ddm.RefreshRate == nRefreshRate )
					{
						return ePF;
					}
				}
			}
		}

		return PF_UNKNOWN;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置 D3D9 设备信息
	void D3D9RenderSystem::_SetD3DPresentParam()
	{
		//函数返回的执行结果
		HRESULT result = E_FAIL;

		//初始化设备信息变量
		memset( &mD3Dpp, 0, sizeof(mD3Dpp) );

		//检测并获取指定的像素格式
		PixelFormat DisplayPF = PF_UNKNOWN;
		DisplayPF = _CheckDisplayMode( mnAdpter, mRenderWidth, mRenderHeight,
			mpConfig->sD3D9Cfg.eColorFormat, mpConfig->sD3D9Cfg.nRefreshRate );
		if( DisplayPF == PF_UNKNOWN )
			Except( Exception::ERR_INTERNAL_ERROR, "显卡不支持配置文件中指定的显示模式。" );

		//设置 D3D 显示模式
		mD3Dpp.BackBufferFormat = D3D9TypeGet::GetPixelFormat( DisplayPF );

#ifdef _DEBUG
		mD3Dpp.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;
		mD3Dpp.SwapEffect					= D3DSWAPEFFECT_COPY;					//设置页面刷新方式复制
		mD3Dpp.Windowed						= true;
#else
		//如果为窗口模式
		if( mpConfig->bWindowed )
		{
			mD3Dpp.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;
			mD3Dpp.SwapEffect					= D3DSWAPEFFECT_COPY;				//设置页面刷新方式复制
			mD3Dpp.Windowed						= true;
			mD3Dpp.BackBufferCount				= 1;								//复制刷新方式缓存页面数量必须为 1
		}
		//如果为全屏模式
		else
		{
			mD3Dpp.FullScreen_RefreshRateInHz	= mpConfig->sD3D9Cfg.nRefreshRate;	//设置全屏刷新率为指定刷新率
			mD3Dpp.SwapEffect					= D3DSWAPEFFECT_DISCARD;			//设置页面刷新方式丢弃旧页面
			mD3Dpp.Windowed						= false;
			mD3Dpp.BackBufferCount				= mpConfig->sD3D9Cfg.nNumBackBuffer;
		}
#endif

		//设定设备信息
		mD3Dpp.BackBufferWidth				= mRenderWidth;							//设定缓存页面（场景）宽度
		mD3Dpp.BackBufferHeight				= mRenderHeight;						//设定缓存页面（场景）高度
		mD3Dpp.hDeviceWindow				= mpRenderWindow->mhWnd;				//设定窗口句柄

		//判断是否开启垂直同步
		mD3Dpp.PresentationInterval = ( mpConfig->sD3D9Cfg.bVsync ) ?
										D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;

		mD3Dpp.Flags						= D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;	//设置深度缓存模式
		mD3Dpp.EnableAutoDepthStencil		= TRUE;									//打开深度缓存
		mD3Dpp.AutoDepthStencilFormat		= D3DFMT_D16;							//设定深度缓存深度为16位

		//设置多重取样模式
		if( mpConfig->sD3D9Cfg.nMultiSampleType == 0 )
		{
			//如果未应用多重取样则设置为无
			mD3Dpp.MultiSampleType		= D3DMULTISAMPLE_NONE;
			mD3Dpp.MultiSampleQuality	= 0;
		}
		else
		{
			//如果应用了多重取样则首先检测硬件是否支持设定模式
			DWORD Quality = 0;
			BOOL bWindowed = ( mpConfig->bWindowed ) ? TRUE : FALSE;
			result = mpD3D9->CheckDeviceMultiSampleType( mpConfig->sD3D9Cfg.nAdapter, D3DDEVTYPE_HAL,
				mD3Dpp.BackBufferFormat, bWindowed,
				(D3DMULTISAMPLE_TYPE)mpConfig->sD3D9Cfg.nMultiSampleType, &Quality );
			if( result == D3D_OK )
			{
				result = mpD3D9->CheckDeviceMultiSampleType( mpConfig->sD3D9Cfg.nAdapter, D3DDEVTYPE_HAL,
					mD3Dpp.AutoDepthStencilFormat, bWindowed,
					(D3DMULTISAMPLE_TYPE)mpConfig->sD3D9Cfg.nMultiSampleType, &Quality );
			}

			//如果硬件支持设定模式则应用它
			if( result == D3D_OK && mpConfig->sD3D9Cfg.dwMultiSampleQuality < Quality )
			{
				mD3Dpp.MultiSampleType		= (D3DMULTISAMPLE_TYPE)mpConfig->sD3D9Cfg.nMultiSampleType;
				mD3Dpp.MultiSampleQuality	= mpConfig->sD3D9Cfg.dwMultiSampleQuality;
			}
			else
			{
				Except( Exception::ERR_INTERNAL_ERROR, "显卡不支持配置文件中指定的多重取样（全屏反锯齿）模式。" );
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//释放设备资源
	void D3D9RenderSystem::_InvalidateDeviceObjects()
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	//重载设备资源
	void D3D9RenderSystem::_RestoreDeviceObjects()
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置视口
	void FKFastcall D3D9RenderSystem::_SetViewport( Viewport* pViewport )
	{
		mpViewport = pViewport;
		HRESULT result = mpD3D9Device->SetViewport( (D3DVIEWPORT9*)pViewport );
		if( FAILED( result ) )
			Except( Exception::ERR_RENDER_API_ERROR, (String)"设置 D3D9 视口失败。"
			"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置观察矩阵
	void FKFastcall D3D9RenderSystem::_SetViewMatrix( const Matrix4* pViewMatrix )
	{
		HRESULT result = mpD3D9Device->SetTransform( D3DTS_VIEW, (D3DXMATRIX*)pViewMatrix );
		if( FAILED( result ) )
			Except( Exception::ERR_RENDER_API_ERROR, (String)"设置 D3D9 观察矩阵失败。"
			"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置投影矩阵
	void FKFastcall D3D9RenderSystem::_SetProjMatrix( const Matrix4* pProjMatrix )
	{
		HRESULT result = mpD3D9Device->SetTransform( D3DTS_PROJECTION, (D3DXMATRIX*)pProjMatrix );
		if( FAILED( result ) )
			Except( Exception::ERR_RENDER_API_ERROR, (String)"设置 D3D9 投影矩阵失败。"
			"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置渲染矩阵
	void FKFastcall D3D9RenderSystem::_SetWorldMatrix( const Matrix4* pWorldMatrix, int nIndex )
	{
		HRESULT result = mpD3D9Device->SetTransform( D3DTS_WORLDMATRIX( nIndex ), (D3DXMATRIX*)pWorldMatrix );
		if( FAILED( result ) )
			Except( Exception::ERR_RENDER_API_ERROR, (String)"设置 D3D9 世界矩阵失败。"
			"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取观察矩阵
	void FKFastcall D3D9RenderSystem::_GetViewMatrix( Matrix4* pViewMatrix )
	{
		HRESULT result = mpD3D9Device->GetTransform( D3DTS_VIEW, (D3DXMATRIX*)pViewMatrix );
		if( FAILED( result ) )
			Except( Exception::ERR_RENDER_API_ERROR, (String)"获取 D3D9 观察矩阵失败。"
			"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取投影矩阵
	void FKFastcall D3D9RenderSystem::_GetProjMatrix( Matrix4* pProjMatrix )
	{
		HRESULT result = mpD3D9Device->GetTransform( D3DTS_PROJECTION, (D3DXMATRIX*)pProjMatrix );
		if( FAILED( result ) )
			Except( Exception::ERR_RENDER_API_ERROR, (String)"获取 D3D9 投影矩阵失败。"
			"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取渲染矩阵
	void FKFastcall D3D9RenderSystem::_GetWorldMatrix( Matrix4* pWorldMatrix, int nIndex )
	{
		HRESULT result = mpD3D9Device->GetTransform( D3DTS_WORLDMATRIX( nIndex ), (D3DXMATRIX*)pWorldMatrix );
		if( FAILED( result ) )
			Except( Exception::ERR_RENDER_API_ERROR, (String)"获取 D3D9 世界矩阵失败。"
			"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置材质
	void FKFastcall D3D9RenderSystem::_SetMaterial( const Material* pMaterial )
	{
		//设置材质
		HRESULT result = mpD3D9Device->SetMaterial( (D3DMATERIAL9*)pMaterial );
		if( FAILED( result ) )
			Except( Exception::ERR_RENDER_API_ERROR, (String)"设置 D3D9 材质失败。"
			"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置纹理
	void FKFastcall D3D9RenderSystem::_SetTexture( const Texture* pTexture, const DWORD dwStage )
	{
		if( dwStage >= mMaxTextureNum )
			Except( Exception::ERR_INVALIDPARAMS, "指定设置的纹理数量超过了支持的最大限。" );

		//如果该纹理和最后设置的纹理相同则返回
		if( mppLastTexture[dwStage] == pTexture )
			return;

		IDirect3DBaseTexture9* pD3D9Texture = ( pTexture != NULL ) ?
			( reinterpret_cast< const D3D9Texture* >( pTexture )->mBaseTexture.mpD3D9Texture ) : NULL;

		HRESULT result = mpD3D9Device->SetTexture( dwStage, pD3D9Texture );
		if( FAILED( result ) )
			Except( Exception::ERR_RENDER_API_ERROR, (String)"设置 D3D9 纹理失败。"
			"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );

		mppLastTexture[dwStage] = (Texture*)pTexture;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置顶点声明
	void FKFastcall D3D9RenderSystem::_SetVertexDeclaration( const VertexDeclaration* pVertexDeclaration )
	{
		//如果该顶点声明和最后设置的顶点声明相同则返回
		if( mpLastVD == pVertexDeclaration )
			return;

		const D3D9VertexDeclaration* pVD = static_cast< const D3D9VertexDeclaration* >( pVertexDeclaration );
		HRESULT result = mpD3D9Device->SetVertexDeclaration( pVD->mpD3D9VertexDeclaration );
		if( FAILED( result ) )
			Except( Exception::ERR_RENDER_API_ERROR, (String)"设置 D3D9 顶点声明失败。"
			"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );

		mpLastVD = (VertexDeclaration*)pVertexDeclaration;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置顶点流
	void FKFastcall D3D9RenderSystem::_SetVertexStream( const VertexBuffer* pVertexBuf, const DWORD dwStream )
	{
		if( dwStream >= mMaxStreamNum )
			Except( Exception::ERR_INVALIDPARAMS, "指定设置的顶点流数量超过了支持的最大限。" );

		//如果该顶点流和最后设置的顶点流相同则返回
		if( mppLastVB[dwStream] == pVertexBuf )
			return;

		const D3D9VertexBuffer* pVB = static_cast< const D3D9VertexBuffer* >( pVertexBuf );
		HRESULT result = mpD3D9Device->SetStreamSource( dwStream, pVB->mpD3D9VertexBuffer, 0, pVB->mVerSize );
		if( FAILED( result ) )
			Except( Exception::ERR_RENDER_API_ERROR, (String)"设置 D3D9 顶点流失败。"
			"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );

		mppLastVB[dwStream] = (VertexBuffer*)pVertexBuf;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置索引流
	void FKFastcall D3D9RenderSystem::_SetIndexStream( const IndexBuffer* pIndexBuf )
	{
		//如果该索引流和最后设置的索引流相同则返回
		if( mpLastIB == pIndexBuf )
			return;

		HRESULT result = mpD3D9Device->SetIndices( static_cast< const D3D9IndexBuffer* >( pIndexBuf )->mpD3D9IndexBuffer );
		if( FAILED( result ) )
			Except( Exception::ERR_RENDER_API_ERROR, (String)"设置 D3D9 索引流失败。"
			"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );

		mpLastIB = (IndexBuffer*)pIndexBuf;
	}
	//--------------------------------------------------------------------------------------------------------------
	//渲染顶点（索引顶点）
	void FKFastcall D3D9RenderSystem::_RenderVertex( PrimitiveType ePrimType, int nVerOffset, UINT nMinIndex,
		UINT nNumVer, UINT nIndexOffset, UINT nPrimCount )
	{
		//--------------------------------------------------------------------------------------
		// 索引顶点绘制说明
		//
		//	1. 实际绘制使用顶点（以顶点缓存起始位置为基准） = 索引缓存中索引值 + 顶点数据偏移量
		//	2. 绘制顶点数量 = 实际绘制使用的第一个顶点 到 实际绘制使用的最后一个顶点
		//	3. 当前绘制所使用的索引缓存中的所有索引值都不允许小于使用最小索引值
		//	4. 索引数据偏移量的值不会影响实际的顶点索引值
		//
		//--------------------------------------------------------------------------------------

		D3DPRIMITIVETYPE d3dpt = D3D9TypeGet::GetPrimitiveType( ePrimType );
		HRESULT result = mpD3D9Device->DrawIndexedPrimitive(
			d3dpt,			//绘制图元类型
			nVerOffset,		//顶点数据偏移量
			nMinIndex,		//使用最小索引值
			nNumVer,		//绘制顶点数量
			nIndexOffset,	//索引数据偏移量
			nPrimCount );	//绘制图元数量

		if( FAILED( result ) )
			Except( Exception::ERR_RENDER_API_ERROR, (String)"绘制索引顶点流失败。"
			"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );

		//增加渲染图元计数
		mNumTriangle += nPrimCount;
	}
	//--------------------------------------------------------------------------------------------------------------
	//渲染顶点
	void FKFastcall D3D9RenderSystem::_RenderVertex( PrimitiveType ePrimType, UINT nVerOffset, UINT nPrimCount )
	{
		HRESULT result = mpD3D9Device->DrawPrimitive( D3D9TypeGet::GetPrimitiveType( ePrimType ), nVerOffset, nPrimCount );
		if( FAILED( result ) )
			Except( Exception::ERR_RENDER_API_ERROR, (String)"绘制顶点流失败。"
			"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );

		//增加渲染图元计数
		mNumTriangle += nPrimCount;
	}
	//--------------------------------------------------------------------------------------------------------------
	//准备设备
	void FKFastcall D3D9RenderSystem::_PrepareDevice()
	{
		//如果设备丢失，则重置设备。
		HRESULT result = mpD3D9Device->TestCooperativeLevel();
		if( result != D3D_OK )
		{
			//等待设备状态为 D3DERR_DEVICENOTRESET 才可以重置
			while( result != D3DERR_DEVICENOTRESET )
				result = mpD3D9Device->TestCooperativeLevel();

			//释放设备资源
			_InvalidateDeviceObjects();

			//重置设备
			result = mpD3D9Device->Reset( &mD3Dpp );
			if( FAILED( result ) )
				Except( Exception::ERR_RENDER_API_ERROR, (String)"设备丢失后重置设备失败。"
				"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );

			//重载设备资源
			_RestoreDeviceObjects();
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//开始渲染
	void FKFastcall D3D9RenderSystem::_BeginRender()
	{
		//清空上一帧的渲染图像
		DWORD ClearFlag = D3DCLEAR_ZBUFFER;
		if( mpViewport->mbClearPerFrame )
			ClearFlag |= D3DCLEAR_TARGET;

		HRESULT result = mpD3D9Device->Clear( 0, NULL, ClearFlag, mpViewport->mdwClearColor, 1.0f, 0 );
		if( FAILED( result ) )
			Except( Exception::ERR_RENDER_API_ERROR, (String)"使用 D3D9 设备清空上一帧的渲染图像失败。"
			"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
		//开始渲染
		result = mpD3D9Device->BeginScene();
		if( FAILED( result ) )
			Except( Exception::ERR_RENDER_API_ERROR, (String)"设置 D3D9 设备开始场景渲染失败。"
			"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
	}
	//--------------------------------------------------------------------------------------------------------------
	//结束渲染
	void FKFastcall D3D9RenderSystem::_EndRender()
	{
		//结束渲染
		HRESULT result = mpD3D9Device->EndScene();
		if( FAILED( result ) )
			Except( Exception::ERR_RENDER_API_ERROR, (String)"设置 D3D9 设备结束场景渲染失败。"
			"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
	}
	//--------------------------------------------------------------------------------------------------------------
	//交换渲染页面
	void FKFastcall D3D9RenderSystem::_SwapRenderBuffer()
	{
		//将缓存页面和主页面进行换页，使我们在缓存页面中渲染的东东显示在（主页面）屏幕上。
		HRESULT result = mpD3D9Device->Present(
			mbUseSwapSrcRect ? &mSwapSrcRect : NULL,
			mbUseSwapDestRect ? &mSwapDestRect : NULL,
			mhSwapDestWindow,
			NULL );

		if( FAILED( result ) )
			Except( Exception::ERR_RENDER_API_ERROR, (String)"D3D9 设备换页失败。"
			"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取渲染状态
	void FKFastcall D3D9RenderSystem::_GetRenderState( RenderState* pRenderState )
	{
		DWORD dwValue = 0;

		mpD3D9Device->GetRenderState( D3DRS_FILLMODE, &dwValue );
		pRenderState->mFillMode = D3D9TypeConvert::GetFillMode( dwValue );

		mpD3D9Device->GetRenderState( D3DRS_SHADEMODE, &dwValue );
		pRenderState->mShadeMode = D3D9TypeConvert::GetShadeMode( dwValue );

		mpD3D9Device->GetRenderState( D3DRS_CULLMODE, &dwValue );
		pRenderState->mCullMode = D3D9TypeConvert::GetCullMode( dwValue );

		mpD3D9Device->GetRenderState( D3DRS_LIGHTING, &dwValue );
		pRenderState->mbLighting = ( dwValue == TRUE ) ? true : false;

		mpD3D9Device->GetRenderState( D3DRS_SPECULARENABLE, &dwValue );
		pRenderState->mbSpecular = ( dwValue == TRUE ) ? true : false;

		mpD3D9Device->GetRenderState( D3DRS_FOGENABLE, &dwValue );
		pRenderState->mbFogEnable = ( dwValue == TRUE ) ? true : false;

		mpD3D9Device->GetRenderState( D3DRS_FOGTABLEMODE, &dwValue );
		pRenderState->mFogMode = D3D9TypeConvert::GetFogMode( dwValue );

		mpD3D9Device->GetRenderState( D3DRS_FOGCOLOR, &dwValue );
		pRenderState->mFogColor = dwValue;

		mpD3D9Device->GetRenderState( D3DRS_FOGSTART, &dwValue );
		pRenderState->mFogStart = *(float*)&dwValue;

		mpD3D9Device->GetRenderState( D3DRS_FOGEND, &dwValue );
		pRenderState->mFogEnd = *(float*)&dwValue;

		mpD3D9Device->GetRenderState( D3DRS_FOGDENSITY, &dwValue );
		pRenderState->mFogDensity = *(float*)&dwValue;

		mpD3D9Device->GetRenderState( D3DRS_ZENABLE, &dwValue );
		pRenderState->mbDepthTestEnable = ( dwValue != D3DZB_FALSE ) ? true : false;

		mpD3D9Device->GetRenderState( D3DRS_ZWRITEENABLE, &dwValue );
		pRenderState->mbDepthWriteEnable = ( dwValue == TRUE ) ? true : false;

		mpD3D9Device->GetRenderState( D3DRS_ALPHABLENDENABLE, &dwValue );
		pRenderState->mbAlphaEnable = ( dwValue == TRUE ) ? true : false;

		mpD3D9Device->GetRenderState( D3DRS_ALPHATESTENABLE, &dwValue );
		pRenderState->mbAlphaTestEnable = ( dwValue == TRUE ) ? true : false;

		mpD3D9Device->GetRenderState( D3DRS_ZFUNC, &dwValue );
		pRenderState->mDepthCompareFun = D3D9TypeConvert::GetCompareFun( dwValue );

		mpD3D9Device->GetRenderState( D3DRS_ALPHAFUNC, &dwValue );
		pRenderState->mAlphaCompareFun = D3D9TypeConvert::GetCompareFun( dwValue );

		mpD3D9Device->GetRenderState( D3DRS_ALPHAREF, &dwValue );
		pRenderState->mdwAlphaRefValue = dwValue;

		mpD3D9Device->GetRenderState( D3DRS_SRCBLEND, &dwValue );
		pRenderState->mSrcBlendFactor = D3D9TypeConvert::GetBlendFactor( dwValue );

		mpD3D9Device->GetRenderState( D3DRS_DESTBLEND, &dwValue );
		pRenderState->mDestBlendFactor = D3D9TypeConvert::GetBlendFactor( dwValue );

		mpD3D9Device->GetRenderState( D3DRS_SRCBLENDALPHA, &dwValue );
		pRenderState->mSrcAlphaBlendFactor = D3D9TypeConvert::GetBlendFactor( dwValue );

		mpD3D9Device->GetRenderState( D3DRS_DESTBLENDALPHA, &dwValue );
		pRenderState->mDestAlphaBlendFactor = D3D9TypeConvert::GetBlendFactor( dwValue );
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置渲染状态
	void FKFastcall D3D9RenderSystem::_SetRenderState( const RenderState* pRenderState )
	{
		HRESULT result;

		if( mRenderState.mFillMode != pRenderState->mFillMode )
		{
			result = mpD3D9Device->SetRenderState( D3DRS_FILLMODE, D3D9TypeGet::GetFillMode( pRenderState->mFillMode ) );
			if( FAILED( result ) )
				Except( Exception::ERR_RENDER_API_ERROR, (String)"设置填充模式错误。"
				"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
			mRenderState.mFillMode = pRenderState->mFillMode;
		}

		if( mRenderState.mShadeMode != pRenderState->mShadeMode )
		{
			result = mpD3D9Device->SetRenderState( D3DRS_SHADEMODE, D3D9TypeGet::GetShadeMode( pRenderState->mShadeMode ) );
			if( FAILED( result ) )
				Except( Exception::ERR_RENDER_API_ERROR, (String)"设置阴影模式错误。"
				"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
			mRenderState.mShadeMode = pRenderState->mShadeMode;
		}

		if( mRenderState.mCullMode != pRenderState->mCullMode )
		{
			result = mpD3D9Device->SetRenderState( D3DRS_CULLMODE, D3D9TypeGet::GetCullMode( pRenderState->mCullMode ) );
			if( FAILED( result ) )
				Except( Exception::ERR_RENDER_API_ERROR, (String)"设置拣选模式错误。"
				"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
			mRenderState.mCullMode = pRenderState->mCullMode;
		}

		if( mRenderState.mbLighting != pRenderState->mbLighting )
		{
			result = mpD3D9Device->SetRenderState( D3DRS_LIGHTING, pRenderState->mbLighting ? TRUE : FALSE );
			if( FAILED( result ) )
				Except( Exception::ERR_RENDER_API_ERROR, (String)"设置光照状态错误。"
				"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
			mRenderState.mbLighting = pRenderState->mbLighting;
		}

		if( mRenderState.mbSpecular != pRenderState->mbSpecular )
		{
			result = mpD3D9Device->SetRenderState( D3DRS_SPECULARENABLE, pRenderState->mbSpecular ? TRUE : FALSE );
			if( FAILED( result ) )
				Except( Exception::ERR_RENDER_API_ERROR, (String)"设置高光状态错误。"
				"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
			mRenderState.mbSpecular = pRenderState->mbSpecular;
		}

		if( mRenderState.mbFogEnable != pRenderState->mbFogEnable )
		{
			result = mpD3D9Device->SetRenderState( D3DRS_FOGENABLE, pRenderState->mbFogEnable ? TRUE : FALSE );
			if( FAILED( result ) )
				Except( Exception::ERR_RENDER_API_ERROR, (String)"设置雾化状态错误。"
				"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
			mRenderState.mbFogEnable = pRenderState->mbFogEnable;
		}

		if( mRenderState.mFogMode != pRenderState->mFogMode )
		{
			result = mpD3D9Device->SetRenderState( D3DRS_FOGTABLEMODE, D3D9TypeGet::GetFogMode( pRenderState->mFogMode ) );
			if( FAILED( result ) )
				Except( Exception::ERR_RENDER_API_ERROR, (String)"设置雾化模式错误。"
				"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
			mRenderState.mFogMode = pRenderState->mFogMode;
		}

		if( mRenderState.mFogColor != pRenderState->mFogColor )
		{
			result = mpD3D9Device->SetRenderState( D3DRS_FOGCOLOR, pRenderState->mFogColor );
			if( FAILED( result ) )
				Except( Exception::ERR_RENDER_API_ERROR, (String)"设置雾化颜色错误。"
				"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
			mRenderState.mFogColor = pRenderState->mFogColor;
		}

		if( mRenderState.mFogStart != pRenderState->mFogStart )
		{
			result = mpD3D9Device->SetRenderState( D3DRS_FOGSTART, *(DWORD*)&pRenderState->mFogStart );
			if( FAILED( result ) )
				Except( Exception::ERR_RENDER_API_ERROR, (String)"设置雾化开始位置错误。"
				"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
			mRenderState.mFogStart = pRenderState->mFogStart;
		}

		if( mRenderState.mFogEnd != pRenderState->mFogEnd )
		{
			result = mpD3D9Device->SetRenderState( D3DRS_FOGEND, *(DWORD*)&pRenderState->mFogEnd );
			if( FAILED( result ) )
				Except( Exception::ERR_RENDER_API_ERROR, (String)"设置雾化结束位置错误。"
				"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
			mRenderState.mFogEnd = pRenderState->mFogEnd;
		}

		if( mRenderState.mFogDensity != pRenderState->mFogDensity )
		{
			result = mpD3D9Device->SetRenderState( D3DRS_FOGDENSITY, *(DWORD*)&pRenderState->mFogDensity );
			if( FAILED( result ) )
				Except( Exception::ERR_RENDER_API_ERROR, (String)"设置雾化浓度错误。"
				"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
			mRenderState.mFogDensity = pRenderState->mFogDensity;
		}

		if( mRenderState.mbDepthTestEnable != pRenderState->mbDepthTestEnable )
		{
			result = mpD3D9Device->SetRenderState( D3DRS_ZENABLE, pRenderState->mbDepthTestEnable ? mdwDepthMode : D3DZB_FALSE );
			if( FAILED( result ) )
				Except( Exception::ERR_RENDER_API_ERROR, (String)"设置深度测试状态错误。"
				"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
			mRenderState.mbDepthTestEnable = pRenderState->mbDepthTestEnable;
		}

		if( mRenderState.mbDepthWriteEnable != pRenderState->mbDepthWriteEnable )
		{
			result = mpD3D9Device->SetRenderState( D3DRS_ZWRITEENABLE, pRenderState->mbDepthWriteEnable ? TRUE : FALSE );
			if( FAILED( result ) )
				Except( Exception::ERR_RENDER_API_ERROR, (String)"设置深度写入状态错误。"
				"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
			mRenderState.mbDepthWriteEnable = pRenderState->mbDepthWriteEnable;
		}

		if( mRenderState.mbAlphaEnable != pRenderState->mbAlphaEnable )
		{
			result = mpD3D9Device->SetRenderState( D3DRS_ALPHABLENDENABLE, pRenderState->mbAlphaEnable ? TRUE : FALSE );
			if( FAILED( result ) )
				Except( Exception::ERR_RENDER_API_ERROR, (String)"设置 Alpha 混合状态错误。"
				"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
			mRenderState.mbAlphaEnable = pRenderState->mbAlphaEnable;
		}

		if( mRenderState.mbAlphaTestEnable != pRenderState->mbAlphaTestEnable )
		{
			result = mpD3D9Device->SetRenderState( D3DRS_ALPHATESTENABLE, pRenderState->mbAlphaTestEnable ? TRUE : FALSE );
			if( FAILED( result ) )
				Except( Exception::ERR_RENDER_API_ERROR, (String)"设置 Alpha 测试状态错误。"
				"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
			mRenderState.mbAlphaTestEnable = pRenderState->mbAlphaTestEnable;
		}

		if( mRenderState.mDepthCompareFun != pRenderState->mDepthCompareFun )
		{
			result = mpD3D9Device->SetRenderState( D3DRS_ZFUNC, D3D9TypeGet::GetCompareFun( pRenderState->mDepthCompareFun ) );
			if( FAILED( result ) )
				Except( Exception::ERR_RENDER_API_ERROR, (String)"设置深度比较函数错误。"
				"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
			mRenderState.mDepthCompareFun = pRenderState->mDepthCompareFun;
		}

		if( mRenderState.mAlphaCompareFun != pRenderState->mAlphaCompareFun )
		{
			result = mpD3D9Device->SetRenderState( D3DRS_ALPHAFUNC, D3D9TypeGet::GetCompareFun( pRenderState->mAlphaCompareFun ) );
			if( FAILED( result ) )
				Except( Exception::ERR_RENDER_API_ERROR, (String)"设置 Alpha 比较函数错误。"
				"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
			mRenderState.mAlphaCompareFun = pRenderState->mAlphaCompareFun;
		}

		if( mRenderState.mdwAlphaRefValue != pRenderState->mdwAlphaRefValue )
		{
			result = mpD3D9Device->SetRenderState( D3DRS_ALPHAREF, pRenderState->mdwAlphaRefValue );
			if( FAILED( result ) )
				Except( Exception::ERR_RENDER_API_ERROR, (String)"设置 Alpha 测试参照值错误。"
				"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
			mRenderState.mdwAlphaRefValue = pRenderState->mdwAlphaRefValue;
		}

		if( mRenderState.mSrcBlendFactor != pRenderState->mSrcBlendFactor )
		{
			result = mpD3D9Device->SetRenderState( D3DRS_SRCBLEND, D3D9TypeGet::GetBlendFactor( pRenderState->mSrcBlendFactor ) );
			if( FAILED( result ) )
				Except( Exception::ERR_RENDER_API_ERROR, (String)"设置来源颜色混合类型错误。"
				"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
			mRenderState.mSrcBlendFactor = pRenderState->mSrcBlendFactor;
		}

		if( mRenderState.mDestBlendFactor != pRenderState->mDestBlendFactor )
		{
			result = mpD3D9Device->SetRenderState( D3DRS_DESTBLEND, D3D9TypeGet::GetBlendFactor( pRenderState->mDestBlendFactor ) );
			if( FAILED( result ) )
				Except( Exception::ERR_RENDER_API_ERROR, (String)"设置目标颜色混合类型错误。"
				"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
			mRenderState.mDestBlendFactor = pRenderState->mDestBlendFactor;
		}

		if( mRenderState.mSrcAlphaBlendFactor != pRenderState->mSrcAlphaBlendFactor )
		{
			result = mpD3D9Device->SetRenderState( D3DRS_SRCBLENDALPHA, D3D9TypeGet::GetBlendFactor( pRenderState->mSrcAlphaBlendFactor ) );
			if( FAILED( result ) )
				Except( Exception::ERR_RENDER_API_ERROR, (String)"设置来源 Alpha 值混合类型错误。"
				"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
			mRenderState.mSrcAlphaBlendFactor = pRenderState->mSrcAlphaBlendFactor;
		}

		if( mRenderState.mDestAlphaBlendFactor != pRenderState->mDestAlphaBlendFactor )
		{
			result = mpD3D9Device->SetRenderState( D3DRS_DESTBLENDALPHA, D3D9TypeGet::GetBlendFactor( pRenderState->mDestAlphaBlendFactor ) );
			if( FAILED( result ) )
				Except( Exception::ERR_RENDER_API_ERROR, (String)"设置目标 Alpha 值混合类型错误。"
				"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
			mRenderState.mDestAlphaBlendFactor = pRenderState->mDestAlphaBlendFactor;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取纹理状态
	void FKFastcall D3D9RenderSystem::_GetTextureState( TextureState* pTextureState, UINT nStage )
	{
		DWORD dwValue;

		mpD3D9Device->GetSamplerState( nStage, D3DSAMP_MAGFILTER, &dwValue );
		pTextureState->mFilterMag = D3D9TypeConvert::GetTextureFilter( dwValue );

		mpD3D9Device->GetSamplerState( nStage, D3DSAMP_MINFILTER, &dwValue );
		pTextureState->mFilterMin = D3D9TypeConvert::GetTextureFilter( dwValue );

		mpD3D9Device->GetSamplerState( nStage, D3DSAMP_MIPFILTER, &dwValue );
		pTextureState->mFilterMip = D3D9TypeConvert::GetTextureFilter( dwValue );

		mpD3D9Device->GetSamplerState( nStage, D3DSAMP_ADDRESSU, &dwValue );
		pTextureState->mAddressU = D3D9TypeConvert::GetTextureAddress( dwValue );

		mpD3D9Device->GetSamplerState( nStage, D3DSAMP_ADDRESSV, &dwValue );
		pTextureState->mAddressV = D3D9TypeConvert::GetTextureAddress( dwValue );

		mpD3D9Device->GetSamplerState( nStage, D3DSAMP_ADDRESSW, &dwValue );
		pTextureState->mAddressW = D3D9TypeConvert::GetTextureAddress( dwValue );

		mpD3D9Device->GetTextureStageState( nStage, D3DTSS_COLOROP, &dwValue );
		pTextureState->mColorOp = D3D9TypeConvert::GetTextureOp( dwValue );

		mpD3D9Device->GetTextureStageState( nStage, D3DTSS_ALPHAOP, &dwValue );
		pTextureState->mAlphaOp = D3D9TypeConvert::GetTextureOp( dwValue );

		mpD3D9Device->GetTextureStageState( nStage, D3DTSS_COLORARG1, &dwValue );
		pTextureState->mColorArg1 = D3D9TypeConvert::GetTextureArg( dwValue );

		mpD3D9Device->GetTextureStageState( nStage, D3DTSS_COLORARG2, &dwValue );
		pTextureState->mColorArg2 = D3D9TypeConvert::GetTextureArg( dwValue );

		mpD3D9Device->GetTextureStageState( nStage, D3DTSS_ALPHAARG1, &dwValue );
		pTextureState->mAlphaArg1 = D3D9TypeConvert::GetTextureArg( dwValue );

		mpD3D9Device->GetTextureStageState( nStage, D3DTSS_ALPHAARG2, &dwValue );
		pTextureState->mAlphaArg2 = D3D9TypeConvert::GetTextureArg( dwValue );
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置纹理状态
	void FKFastcall D3D9RenderSystem::_SetTextureState( const TextureState* pTextureState, UINT nStage )
	{
		HRESULT result;

		if( mpTextureState[nStage].mFilterMag != pTextureState->mFilterMag )
		{
			result = mpD3D9Device->SetSamplerState( nStage, D3DSAMP_MAGFILTER,
				D3D9TypeGet::GetTextureFilter( pTextureState->mFilterMag ) );
			if( FAILED( result ) )
				Except( Exception::ERR_RENDER_API_ERROR, (String)"设置放大纹理过滤类型错误。"
				"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
			mpTextureState[nStage].mFilterMag = pTextureState->mFilterMag;
		}

		if( mpTextureState[nStage].mFilterMin != pTextureState->mFilterMin )
		{
			result = mpD3D9Device->SetSamplerState( nStage, D3DSAMP_MINFILTER,
				D3D9TypeGet::GetTextureFilter( pTextureState->mFilterMin ) );
			if( FAILED( result ) )
				Except( Exception::ERR_RENDER_API_ERROR, (String)"设置缩小纹理过滤类型错误。"
				"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
			mpTextureState[nStage].mFilterMin = pTextureState->mFilterMin;
		}

		if( mpTextureState[nStage].mFilterMip != pTextureState->mFilterMip )
		{
			result = mpD3D9Device->SetSamplerState( nStage, D3DSAMP_MIPFILTER,
				D3D9TypeGet::GetTextureFilter( pTextureState->mFilterMip ) );
			if( FAILED( result ) )
				Except( Exception::ERR_RENDER_API_ERROR, (String)"设置多级纹理过滤类型错误。"
				"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
			mpTextureState[nStage].mFilterMip = pTextureState->mFilterMip;
		}

		if( mpTextureState[nStage].mAddressU != pTextureState->mAddressU )
		{
			result = mpD3D9Device->SetSamplerState( nStage, D3DSAMP_ADDRESSU,
				D3D9TypeGet::GetTextureAddress( pTextureState->mAddressU ) );
			if( FAILED( result ) )
				Except( Exception::ERR_RENDER_API_ERROR, (String)"设置纹理 U 坐标寻址类型错误。"
				"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
			mpTextureState[nStage].mAddressU = pTextureState->mAddressU;
		}

		if( mpTextureState[nStage].mAddressV != pTextureState->mAddressV )
		{
			result = mpD3D9Device->SetSamplerState( nStage, D3DSAMP_ADDRESSV,
				D3D9TypeGet::GetTextureAddress( pTextureState->mAddressV ) );
			if( FAILED( result ) )
				Except( Exception::ERR_RENDER_API_ERROR, (String)"设置纹理 V 坐标寻址类型错误。"
				"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
			mpTextureState[nStage].mAddressV = pTextureState->mAddressV;
		}

		if( mpTextureState[nStage].mAddressW != pTextureState->mAddressW )
		{
			result = mpD3D9Device->SetSamplerState( nStage, D3DSAMP_ADDRESSW,
				D3D9TypeGet::GetTextureAddress( pTextureState->mAddressW ) );
			if( FAILED( result ) )
				Except( Exception::ERR_RENDER_API_ERROR, (String)"设置纹理 W 坐标寻址类型错误。"
				"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
			mpTextureState[nStage].mAddressW = pTextureState->mAddressW;
		}

		if( mpTextureState[nStage].mColorOp != pTextureState->mColorOp )
		{
			result = mpD3D9Device->SetTextureStageState( nStage, D3DTSS_COLOROP,
				D3D9TypeGet::GetTextureOp( pTextureState->mColorOp ) );
			if( FAILED( result ) )
				Except( Exception::ERR_RENDER_API_ERROR, (String)"设置纹理颜色操作类型错误。"
				"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
			mpTextureState[nStage].mColorOp = pTextureState->mColorOp;
		}

		if( mpTextureState[nStage].mAlphaOp != pTextureState->mAlphaOp )
		{
			result = mpD3D9Device->SetTextureStageState( nStage, D3DTSS_ALPHAOP,
				D3D9TypeGet::GetTextureOp( pTextureState->mAlphaOp ) );
			if( FAILED( result ) )
				Except( Exception::ERR_RENDER_API_ERROR, (String)"设置纹理 Alpha 操作类型错误。"
				"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
			mpTextureState[nStage].mAlphaOp = pTextureState->mAlphaOp;
		}

		if( mpTextureState[nStage].mColorArg1 != pTextureState->mColorArg1 )
		{
			result = mpD3D9Device->SetTextureStageState( nStage, D3DTSS_COLORARG1,
				D3D9TypeGet::GetTextureArg( pTextureState->mColorArg1 ) );
			if( FAILED( result ) )
				Except( Exception::ERR_RENDER_API_ERROR, (String)"设置纹理颜色操作参数 1 错误。"
				"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
			mpTextureState[nStage].mColorArg1 = pTextureState->mColorArg1;
		}

		if( mpTextureState[nStage].mColorArg2 != pTextureState->mColorArg2 )
		{
			result = mpD3D9Device->SetTextureStageState( nStage, D3DTSS_COLORARG2,
				D3D9TypeGet::GetTextureArg( pTextureState->mColorArg2 ) );
			if( FAILED( result ) )
				Except( Exception::ERR_RENDER_API_ERROR, (String)"设置纹理颜色操作参数 2 错误。"
				"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
			mpTextureState[nStage].mColorArg2 = pTextureState->mColorArg2;
		}

		if( mpTextureState[nStage].mAlphaArg1 != pTextureState->mAlphaArg1 )
		{
			result = mpD3D9Device->SetTextureStageState( nStage, D3DTSS_ALPHAARG1,
				D3D9TypeGet::GetTextureArg( pTextureState->mAlphaArg1 ) );
			if( FAILED( result ) )
				Except( Exception::ERR_RENDER_API_ERROR, (String)"设置纹理 Alpha 操作参数 1 错误。"
				"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
			mpTextureState[nStage].mAlphaArg1 = pTextureState->mAlphaArg1;
		}

		if( mpTextureState[nStage].mAlphaArg2 != pTextureState->mAlphaArg2 )
		{
			result = mpD3D9Device->SetTextureStageState( nStage, D3DTSS_ALPHAARG2,
				D3D9TypeGet::GetTextureArg( pTextureState->mAlphaArg2 ) );
			if( FAILED( result ) )
				Except( Exception::ERR_RENDER_API_ERROR, (String)"设置纹理 Alpha 操作参数 2 错误。"
				"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
			mpTextureState[nStage].mAlphaArg2 = pTextureState->mAlphaArg2;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//关闭纹理状态
	void FKFastcall D3D9RenderSystem::_DisableTextureState( UINT nStage )
	{
		HRESULT result;

		result = mpD3D9Device->SetTextureStageState( nStage, D3DTSS_COLOROP, D3DTOP_DISABLE );
		if( FAILED( result ) )
			Except( Exception::ERR_RENDER_API_ERROR, (String)"关闭纹理颜色操作错误。"
			"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );

		result = mpD3D9Device->SetTextureStageState( nStage, D3DTSS_ALPHAOP, D3DTOP_DISABLE );
		if( FAILED( result ) )
			Except( Exception::ERR_RENDER_API_ERROR, (String)"关闭纹理 Alpha 操作错误。"
			"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );

		mpTextureState[nStage].mColorOp = TO_DISABLE;
		mpTextureState[nStage].mAlphaOp = TO_DISABLE;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置光照状态
	void FKFastcall D3D9RenderSystem::_SetLightingState( bool bEnable )
	{
		if( mRenderState.mbLighting != bEnable )
		{
			HRESULT result = mpD3D9Device->SetRenderState( D3DRS_LIGHTING, bEnable ? TRUE : FALSE );
			if( FAILED( result ) )
				Except( Exception::ERR_RENDER_API_ERROR, (String)"设置光照状态错误。"
				"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
			mRenderState.mbLighting = bEnable;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置高光状态
	void FKFastcall D3D9RenderSystem::_SetSpecularState( bool bEnable )
	{
		if( mRenderState.mbSpecular != bEnable )
		{
			HRESULT result = mpD3D9Device->SetRenderState( D3DRS_SPECULARENABLE, bEnable ? TRUE : FALSE );
			if( FAILED( result ) )
				Except( Exception::ERR_RENDER_API_ERROR, (String)"设置高光状态错误。"
				"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
			mRenderState.mbSpecular = bEnable;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置雾化状态
	void FKFastcall D3D9RenderSystem::_SetFogState( bool bEnable )
	{
		if( mRenderState.mbFogEnable != bEnable )
		{
			HRESULT result = mpD3D9Device->SetRenderState( D3DRS_FOGENABLE, bEnable ? TRUE : FALSE );
			if( FAILED( result ) )
				Except( Exception::ERR_RENDER_API_ERROR, (String)"设置雾化状态错误。"
				"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
			mRenderState.mbFogEnable = bEnable;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置深度测试状态
	void FKFastcall D3D9RenderSystem::_SetDepthTestState( bool bEnable )
	{
		if( mRenderState.mbDepthTestEnable != bEnable )
		{
			HRESULT result = mpD3D9Device->SetRenderState( D3DRS_ZENABLE, bEnable ? mdwDepthMode : D3DZB_FALSE );
			if( FAILED( result ) )
				Except( Exception::ERR_RENDER_API_ERROR, (String)"设置深度测试状态错误。"
				"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
			mRenderState.mbDepthTestEnable = bEnable;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置深度写入状态
	void FKFastcall D3D9RenderSystem::_SetDepthWriteState( bool bEnable )
	{
		if( mRenderState.mbDepthWriteEnable != bEnable )
		{
			HRESULT result = mpD3D9Device->SetRenderState( D3DRS_ZWRITEENABLE, bEnable ? TRUE : FALSE );
			if( FAILED( result ) )
				Except( Exception::ERR_RENDER_API_ERROR, (String)"设置深度写入状态错误。"
				"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
			mRenderState.mbDepthWriteEnable = bEnable;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置 Alpha 混合状态
	void FKFastcall D3D9RenderSystem::_SetAlphaState( bool bEnable )
	{
		if( mRenderState.mbAlphaEnable != bEnable )
		{
			HRESULT result = mpD3D9Device->SetRenderState( D3DRS_ALPHABLENDENABLE, bEnable ? TRUE : FALSE );
			if( FAILED( result ) )
				Except( Exception::ERR_RENDER_API_ERROR, (String)"设置 Alpha 混合状态错误。"
				"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
			mRenderState.mbAlphaEnable = bEnable;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置 Alpha 测试状态
	void FKFastcall D3D9RenderSystem::_SetAlphaTestState( bool bEnable )
	{
		if( mRenderState.mbAlphaTestEnable != bEnable )
		{
			HRESULT result = mpD3D9Device->SetRenderState( D3DRS_ALPHATESTENABLE, bEnable ? TRUE : FALSE );
			if( FAILED( result ) )
				Except( Exception::ERR_RENDER_API_ERROR, (String)"设置 Alpha 测试状态错误。"
				"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
			mRenderState.mbAlphaTestEnable = bEnable;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置纹理过滤类型
	void FKFastcall D3D9RenderSystem::_SetTextureFilter( SamplerType eSamplerType, TextureFilterType eFilterType, UINT nStage )
	{
		TextureFilterType* pFilterType;
		D3DSAMPLERSTATETYPE eD3DSamplerType;

		switch (eSamplerType)
		{
		case ST_MAGFILTER:		pFilterType = &mpTextureState[nStage].mFilterMag;		eD3DSamplerType = D3DSAMP_MAGFILTER;	break;
		case ST_MINFILTER:		pFilterType = &mpTextureState[nStage].mFilterMin;		eD3DSamplerType = D3DSAMP_MINFILTER;	break;
		case ST_MIPFILTER:		pFilterType = &mpTextureState[nStage].mFilterMip;		eD3DSamplerType = D3DSAMP_MIPFILTER;	break;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "取样类型错误，无法设置纹理过滤类型渲染状态。" );
		}

		if( *pFilterType == eFilterType )
			return;

		HRESULT result = mpD3D9Device->SetSamplerState( nStage, eD3DSamplerType, D3D9TypeGet::GetTextureFilter( eFilterType ) );
		if( FAILED( result ) )
			Except( Exception::ERR_RENDER_API_ERROR, (String)"设置纹理过滤类型错误。"
			"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );

		*pFilterType = eFilterType;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置纹理寻址类型
	void FKFastcall D3D9RenderSystem::_SetTextureAddress( SamplerType eSamplerType, TextureAddressType eAddressType, UINT nStage )
	{
		TextureAddressType* pAddressType;
		D3DSAMPLERSTATETYPE eD3DSamplerType;

		switch (eSamplerType)
		{
		case ST_ADDRESSU:		pAddressType = &mpTextureState[nStage].mAddressU;		eD3DSamplerType = D3DSAMP_ADDRESSU;		break;
		case ST_ADDRESSV:		pAddressType = &mpTextureState[nStage].mAddressV;		eD3DSamplerType = D3DSAMP_ADDRESSV;		break;
		case ST_ADDRESSW:		pAddressType = &mpTextureState[nStage].mAddressW;		eD3DSamplerType = D3DSAMP_ADDRESSW;		break;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "取样类型错误，无法设置纹理寻址类型渲染状态。" );
		}

		if( *pAddressType == eAddressType )
			return;

		HRESULT result = mpD3D9Device->SetSamplerState( nStage, eD3DSamplerType, D3D9TypeGet::GetTextureAddress( eAddressType ) );
		if( FAILED( result ) )
			Except( Exception::ERR_RENDER_API_ERROR, (String)"设置纹理寻址类型错误。"
			"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );

		*pAddressType = eAddressType;
	}
	//--------------------------------------------------------------------------------------------------------------
	//激活灯光
	bool FKFastcall D3D9RenderSystem::_ActiveLight( Light* pLight, const bool bActive )
	{
		HRESULT result;

		//如果要求设置灯光
		if( bActive )
		{
			//如果无可用灯光索引则返回
			if( mFreeLightIndex.Size() == 0 )
				return false;

			//获取索引值
			pLight->mIndex = *mFreeLightIndex.Pop();

			//设置 D3D9 灯光数据
			static D3DLIGHT9 LightData;
			LightData.Type = D3D9TypeGet::GetLightType( pLight->mLightType );
			memcpy( &LightData.Diffuse, &pLight->mDiffuse, sizeof(D3DLIGHT9) - sizeof(D3DLIGHTTYPE) );

			//激活灯光
			result = mpD3D9Device->SetLight( pLight->mIndex, &LightData );
			pLight->mbActived = true;
		}
		else
		{
			//释放索引值
			*mFreeLightIndex.Push() = pLight->mIndex;

			//反激活灯光
			result = mpD3D9Device->SetLight( pLight->mIndex, NULL );
			pLight->mbActived = false;
		}
		
		if( FAILED( result ) )
			Except( Exception::ERR_RENDER_API_ERROR, (String)"设置灯光错误。"
			"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );

		return true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置灯光状态
	void FKFastcall D3D9RenderSystem::_SetLightState( const Light* pLight )
	{
		BOOL nEnable = pLight->mbEnable ? TRUE : FALSE;

		HRESULT result = mpD3D9Device->LightEnable( pLight->mIndex, nEnable );
		if( FAILED( result ) )
			Except( Exception::ERR_RENDER_API_ERROR, (String)"设置灯光状态错误。"
			"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置环境光
	void FKFastcall D3D9RenderSystem::_SetAmbient( const ColorValue& color )
	{
		HRESULT result = mpD3D9Device->SetRenderState( D3DRS_AMBIENT, color.GetARGB() );
		if( FAILED( result ) )
			Except( Exception::ERR_RENDER_API_ERROR, (String)"设置环境光错误。"
			"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置材质颜色来源
	void FKFastcall D3D9RenderSystem::_SetMaterialSource( const MaterialSource& refMatSor )
	{
		//如果有任何项使用顶点颜色作为材质来源
		int eFlag =
			refMatSor.DiffuseSource | refMatSor.SpecularSource |
			refMatSor.AmbientSource | refMatSor.EmissiveSource;

		//开启或关闭顶点颜色状态
		HRESULT result;
		if( eFlag == 0 )
			result = mpD3D9Device->SetRenderState( D3DRS_COLORVERTEX, FALSE );
		else
			result = mpD3D9Device->SetRenderState( D3DRS_COLORVERTEX, TRUE );

		if( FAILED( result ) )
			Except( Exception::ERR_RENDER_API_ERROR, (String)"设置顶点颜色状态错误。"
			"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );

		//设置材质来源
		result = mpD3D9Device->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE, (DWORD)refMatSor.DiffuseSource );
		result |= mpD3D9Device->SetRenderState( D3DRS_SPECULARMATERIALSOURCE, (DWORD)refMatSor.SpecularSource );
		result |= mpD3D9Device->SetRenderState( D3DRS_AMBIENTMATERIALSOURCE, (DWORD)refMatSor.AmbientSource );
		result |= mpD3D9Device->SetRenderState( D3DRS_EMISSIVEMATERIALSOURCE, (DWORD)refMatSor.EmissiveSource );

		if( FAILED( result ) )
			Except( Exception::ERR_RENDER_API_ERROR, (String)"设置材质颜色来源错误。"
			"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置索引混合状态
	void FKFastcall D3D9RenderSystem::_SetIndexVertexBlend( const UINT nNumBlendMatrix )
	{
		HRESULT result;

		//如果要求打开索引混合
		if( nNumBlendMatrix > 0 )
		{
			D3DVERTEXBLENDFLAGS eVertexBlendType;

			switch (nNumBlendMatrix)
			{
			case 1:		eVertexBlendType = D3DVBF_0WEIGHTS;		break;
			case 2:		eVertexBlendType = D3DVBF_1WEIGHTS;		break;
			case 3:		eVertexBlendType = D3DVBF_2WEIGHTS;		break;
			case 4:		eVertexBlendType = D3DVBF_3WEIGHTS;		break;
			default:
				Except( Exception::ERR_INVALIDPARAMS, "错误的蒙皮骨骼数量。" );
			}

			result =	mpD3D9Device->SetRenderState( D3DRS_INDEXEDVERTEXBLENDENABLE, TRUE );
			result |=	mpD3D9Device->SetRenderState( D3DRS_VERTEXBLEND, eVertexBlendType );
		}
		//如果要求关闭索引混合
		else
		{
			result =	mpD3D9Device->SetRenderState( D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE );
			result |=	mpD3D9Device->SetRenderState( D3DRS_VERTEXBLEND, D3DVBF_DISABLE );
		}

		if( FAILED( result ) )
			Except( Exception::ERR_RENDER_API_ERROR, (String)"设置索引混合状态错误。"
			"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置顶点处理方式
	void FKFastcall D3D9RenderSystem::_SetSoftwareVertexProcess( bool bSoftProc )
	{
		if( mbSoftVerProc == bSoftProc || mD3DBehavior == D3DCREATE_SOFTWARE_VERTEXPROCESSING )
			return;

		HRESULT result = mpD3D9Device->SetSoftwareVertexProcessing( bSoftProc ? TRUE : FALSE );
		if( FAILED( result ) )
			Except( Exception::ERR_RENDER_API_ERROR, (String)"切换为顶点处理方式失败。"
			"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );

		mbSoftVerProc = bSoftProc;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
