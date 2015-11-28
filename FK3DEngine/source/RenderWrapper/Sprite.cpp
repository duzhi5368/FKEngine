/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Sprite
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
	//创建精灵
	Sprite::Sprite( SpriteSet* pSpriteSet, float fLeft, float fTop, float fWidth, float fHeight,
		ZOrderType eZType, OverlaySurface* pZRefOverlay, TextureFilterType eFilterType,
		UV4* pUV, DWORD color )
		: OverlaySurface	( eZType, pZRefOverlay, eFilterType )
		, mpSpriteSet		(pSpriteSet)
		, mpUVASprite		(NULL)
		, mfWidth			(fWidth)
		, mfHeight			(fHeight)
	{
		//设置顶点数据
		OverlayVertex* pVer = mpVertexData;

		float Right = fLeft + mfWidth;
		float Bottom = fTop + mfHeight;

		//左下
		pVer->x = fLeft;	pVer->y = Bottom;	pVer->z = 1.0f;
		pVer->u = pUV->u1;	pVer->v = pUV->v2;	pVer->color = color;	++pVer;

		//左上
		pVer->x = fLeft;	pVer->y = fTop;		pVer->z = 1.0f;
		pVer->u = pUV->u1;	pVer->v = pUV->v1;	pVer->color = color;	++pVer;

		//右上
		pVer->x = Right;	pVer->y = fTop;		pVer->z = 1.0f;
		pVer->u = pUV->u2;	pVer->v = pUV->v1;	pVer->color = color;	++pVer;

		//右下
		pVer->x = Right;	pVer->y = Bottom;	pVer->z = 1.0f;
		pVer->u = pUV->u2;	pVer->v = pUV->v2;	pVer->color = color;

		mNumVertex = 4;
		mpVertexPtr = mpVertexData;

		//增加顶点计数
		OverlayManager::mpSingleton->mNumRenderVer += 4;
	}
	//--------------------------------------------------------------------------------------------------------------
	Sprite::Sprite( SpriteSet* pSpriteSet, float fLeft, float fTop, float fWidth, float fHeight,
		ZOrderType eZType, OverlaySurface* pZRefOverlay, TextureFilterType eFilterType,
		UVAnimation* pUVAnimation, DWORD color )
		: OverlaySurface	( eZType, pZRefOverlay, eFilterType )
		, mpSpriteSet		(pSpriteSet)
		, mfWidth			(fWidth)
		, mfHeight			(fHeight)
	{
		//创建 UV 动画管理器
		mpUVASprite = new UVASprite( this, pUVAnimation );
		OverlayManager::mpSingleton->_AddUVASpriteToList( mpUVASprite );

		const UV4* pUV = mpUVASprite->GetCurrentUV();

		//设置顶点数据
		OverlayVertex* pVer = mpVertexData;

		float Right = fLeft + mfWidth;
		float Bottom = fTop + mfHeight;

		//左下
		pVer->x = fLeft;	pVer->y = Bottom;	pVer->z = 1.0f;
		pVer->u = pUV->u1;	pVer->v = pUV->v2;	pVer->color = color;	++pVer;

		//左上
		pVer->x = fLeft;	pVer->y = fTop;		pVer->z = 1.0f;
		pVer->u = pUV->u1;	pVer->v = pUV->v1;	pVer->color = color;	++pVer;

		//右上
		pVer->x = Right;	pVer->y = fTop;		pVer->z = 1.0f;
		pVer->u = pUV->u2;	pVer->v = pUV->v1;	pVer->color = color;	++pVer;

		//右下
		pVer->x = Right;	pVer->y = Bottom;	pVer->z = 1.0f;
		pVer->u = pUV->u2;	pVer->v = pUV->v2;	pVer->color = color;

		mNumVertex = 4;
		mpVertexPtr = mpVertexData;

		//增加顶点计数
		OverlayManager::mpSingleton->mNumRenderVer += 4;
	}
	//--------------------------------------------------------------------------------------------------------------
	Sprite::~Sprite()
	{
		//删除 UV 动画处理器
		if( mpUVASprite != NULL )
		{
			OverlayManager::mpSingleton->_DelUVASpriteFromList( mpUVASprite );
			delete mpUVASprite;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//将该精灵从它的精灵集内删除
	void Sprite::DeleteFromSpriteSet()
	{
		mpSpriteSet->DeleteSprite( this );
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置精灵 UV 坐标
	void FKFastcall FKFastcall Sprite::SetSpriteUV( const UV4* pUV )
	{
		//设置顶点 UV 数据
		OverlayVertex* pVer = mpVertexData;

		pVer->u = pUV->u1;	pVer->v = pUV->v2;	++pVer;	//左下
		pVer->u = pUV->u1;	pVer->v = pUV->v1;	++pVer;	//左上
		pVer->u = pUV->u2;	pVer->v = pUV->v1;	++pVer;	//右上
		pVer->u = pUV->u2;	pVer->v = pUV->v2;			//右下
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置图像位置
	void FKFastcall Sprite::SetLocation( float fLeft, float fTop )
	{
		//设置顶点数据
		OverlayVertex* pVer = mpVertexData;

		float Right = fLeft + mfWidth;
		float Bottom = fTop + mfHeight;
		
		pVer->x = fLeft;	pVer->y = Bottom;	++pVer;	//左下
		pVer->x = fLeft;	pVer->y = fTop;		++pVer;	//左上
		pVer->x = Right;	pVer->y = fTop;		++pVer;	//右上
		pVer->x = Right;	pVer->y = Bottom;			//右下
	}
	//--------------------------------------------------------------------------------------------------------------
	//移动精灵位置
	void FKFastcall Sprite::MoveLocation( float fX, float fY )
	{
		//设置顶点数据
		OverlayVertex* pVer = mpVertexData;

		pVer->x += fX;	pVer->y += fY;	++pVer;	//左下
		pVer->x += fX;	pVer->y += fY;	++pVer;	//左上
		pVer->x += fX;	pVer->y += fY;	++pVer;	//右上
		pVer->x += fX;	pVer->y += fY;			//右下
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置精灵尺寸
	void FKFastcall Sprite::SetSize( float fWidth, float fHeight )
	{
		//设置顶点数据
		OverlayVertex* pVer = mpVertexData;

		mfWidth = fWidth;
		mfHeight = fHeight;

		float fRight = pVer[0].x + fWidth;
		float fBottom = pVer[1].y + fHeight;

		pVer[2].x = fRight;
		pVer[3].x = fRight;
		pVer[0].y = fBottom;
		pVer[3].y = fBottom;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置图像颜色
	void FKFastcall Sprite::SetColor( DWORD color )
	{
		//设置顶点数据
		OverlayVertex* pVer = mpVertexData;

		pVer->color = color;	++pVer;	//左下
		pVer->color = color;	++pVer;	//左上
		pVer->color = color;	++pVer;	//右上
		pVer->color = color;			//右下
	}
	//--------------------------------------------------------------------------------------------------------------
	//处理平面图像顶点
	void FKFastcall Sprite::_ProcessVertex()
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	//渲染平面图像
	void FKFastcall Sprite::_RenderOverlay()
	{
		//设置纹理过滤类型
		RenderSystem::mpSingleton->_SetTextureFilter( ST_MAGFILTER, mFilterType, 0 );
		RenderSystem::mpSingleton->_SetTextureFilter( ST_MINFILTER, mFilterType, 0 );

		//设置纹理
		RenderSystem::mpSingleton->_SetTexture( mpSpriteSet->mpTexture, 0 );

		//绘制图元
		RenderSystem::mpSingleton->_RenderVertex( PT_TRIANGLE_FAN, mVertexOffset, 2 );
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
