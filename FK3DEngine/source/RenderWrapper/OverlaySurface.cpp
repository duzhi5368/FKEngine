/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	OverlaySurface
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
	OverlaySurface::OverlaySurface( ZOrderType eZType, OverlaySurface* pZRefOverlay, TextureFilterType eFilterType )
		: mbVisable		(true)
		, mNumVertex	(0)
		, mVertexOffset	(0)
		, mFilterType	(eFilterType)
		, mpVertexPtr	(NULL)
	{
		//加入平面图像队列
		OverlayManager::mpSingleton->_AddOverlayToList( this, eZType, pZRefOverlay );
	}
	//--------------------------------------------------------------------------------------------------------------
	OverlaySurface::~OverlaySurface()
	{
		//从平面图像管理列表中删除
		OverlayManager::mpSingleton->_DelOverlayFromList( this );
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置平面图像可见状态
	void FKFastcall OverlaySurface::SetVisable( bool bVisable )
	{
		if( mbVisable && !bVisable )
		{
			//减少顶点计数
			OverlayManager::mpSingleton->mNumRenderVer -= mNumVertex;
		}
		else if( !mbVisable && bVisable )
		{
			//增加顶点计数
			OverlayManager::mpSingleton->mNumRenderVer += mNumVertex;
		}

		mbVisable = bVisable;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置平面图像 Z 顺序
	void FKFastcall OverlaySurface::SetZOrder( ZOrderType eZType, OverlaySurface* pZRefOverlay )
	{
		OverlayManager::mpSingleton->_SetOverlayZOrder( this, eZType, pZRefOverlay );
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置平面图像纹理过滤类型
	void FKFastcall OverlaySurface::SetFilterType( TextureFilterType eFilterType )
	{
		mFilterType = eFilterType;
	}
	//--------------------------------------------------------------------------------------------------------------
	//更新平面图像顶点
	UINT FKFastcall OverlaySurface::_UpdateVertex( OverlayVertex* pVertexBuf, UINT nOffset )
	{
		if( mNumVertex == 0 )
			return 0;

		memcpy( pVertexBuf, mpVertexPtr, sizeof(OverlayVertex) * mNumVertex );

		mVertexOffset = nOffset;

		return mNumVertex;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
