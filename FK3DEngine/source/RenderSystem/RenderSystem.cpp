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

#include "../../include/RenderSystem/RenderSystemInsideHeader.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------

	bool		RenderSystem::mbSoftVerProc			= false;		//是否正在使用软件顶点处理方式
	DWORD		RenderSystem::mdwDepthMode			= D3DZB_TRUE;	//使用深度缓存模式
	DWORD		RenderSystem::mdwMaxBlendNum		= 0;			//硬件支持顶点混合最大矩阵混合数量
	DWORD		RenderSystem::mdwMaxBlendIndex		= 0;			//硬件支持顶点混合最大矩阵索引数量

	UINT		RenderSystem::mnAdpter				= 0;			//适配器序号
	UINT		RenderSystem::mRenderWidth			= 0;			//渲染场景宽
	UINT		RenderSystem::mRenderHeight			= 0;			//渲染场景高

	PixelFormat	RenderSystem::mDisplayFormat		= PF_UNKNOWN;	//显示格式
	UINT		RenderSystem::mDisplayColorDepth	= 0;			//显示格式色深字节数
	UINT		RenderSystem::mDisplayAlphaBits		= 0;			//显示格式 Alpha 值位数

	DWORD		RenderSystem::mMaxTextureNum		= 0;			//最大纹理数量
	DWORD		RenderSystem::mMaxStreamNum			= 0;			//最大顶点流数量

	UINT		RenderSystem::mNumTriangle			= 0;			//上一帧渲染三角形数

	//--------------------------------------------------------------------------------------------------------------
	RenderSystem::RenderSystem( const RenderSystemType eType )
		: mbRun				(false)
		, mpRenderWindow	(NULL)
		, mpConfig			(NULL)
		, mpViewport		(NULL)
		, mSystemType		(eType)
		, mbUseSwapSrcRect	(false)
		, mbUseSwapDestRect	(false)
		, mhSwapDestWindow	(NULL)
	{
		mViewportList.Initialize( 3, 10 );
		mFreeLightIndex.Initialize( 8, 8 );

		memset( mppLastTexture, 0, sizeof(Texture*) * 8 );
		memset( mppLastVB, 0, sizeof(VertexBuffer) * 8 );

		mpLastIB = NULL;
		mpLastVD = NULL;
	}
	//--------------------------------------------------------------------------------------------------------------
	RenderSystem::~RenderSystem()
	{
		DeleteAllViewport();
		mViewportList.Clear();
		mFreeLightIndex.Clear();
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建视口
	Viewport* RenderSystem::CreateViewport( DWORD dwLeft, DWORD dwTop, DWORD dwWidth, DWORD dwHeight,
		float fMinZ, float fMaxZ )
	{
		//如果为调试模式则检测渲染深度值范围是否在 0.0f - 1.0f 之间。
#ifdef _DEBUG
		if( fMinZ > 1.0f || fMinZ < 0.0f || fMaxZ > 1.0f || fMaxZ < 0.0f || fMinZ > fMaxZ )
		{
			Except( Exception::ERR_INVALIDPARAMS, "视口渲染深度值范围必须在 0.0f 到 1.0f 之间。" );
			return NULL;
		}
#endif

		Viewport* pViewport = new Viewport( dwLeft, dwTop, dwWidth, dwHeight, fMinZ, fMaxZ );
		*mViewportList.Push() = pViewport;

		return pViewport;
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除视口
	void RenderSystem::DeleteViewport( Viewport* pViewport )
	{
		if( pViewport == NULL )
			return;

		ViewportList::Iterator it = mViewportList.Begin();
		ViewportList::Iterator end = mViewportList.End();
		for(; it!=end; ++it )
		{
			if( *it == pViewport )
			{
				mViewportList.Erase( it );
				delete pViewport;
				return;
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除所有视口
	void RenderSystem::DeleteAllViewport()
	{
		ViewportList::Iterator it = mViewportList.Begin();
		ViewportList::Iterator end = mViewportList.End();
		for(; it!=end; ++it )
		{
			if( *it != NULL )
				delete *it;
		}

		mViewportList.Clear();
	}
	//--------------------------------------------------------------------------------------------------------------
	//返回视口数量
	UINT RenderSystem::GetNumViewport()
	{
		return mViewportList.Size();
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取当前激活视口
	Viewport* RenderSystem::GetActiveViewport()
	{
		return mpViewport;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置渲染页面交换参数
	void RenderSystem::SetSwapParam( RECT* pSrcRect, RECT* pDestRect, HWND hDestWindow )
	{
		if( !mpConfig->bWindowed )
			Except( Exception::ERR_INVALIDPARAMS, "非窗口渲染模式不允许设置渲染页面交换参数。" );

		if( pSrcRect != NULL )
		{
			mbUseSwapSrcRect = true;
			memcpy( &mSwapSrcRect, pSrcRect, sizeof(RECT) );
		}
		else
		{
			mbUseSwapSrcRect = false;
		}

		if( pDestRect != NULL )
		{
			mbUseSwapDestRect = true;
			memcpy( &mSwapDestRect, pDestRect, sizeof(RECT) );
		}
		else
		{
			mbUseSwapDestRect = false;
		}

		mhSwapDestWindow = hDestWindow;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
