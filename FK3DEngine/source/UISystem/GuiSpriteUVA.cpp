/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	GuiSpriteUVA
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../../include/UISystem/UISystemInsideHeader.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	GuiSpriteUVA::GuiSpriteUVA( ZOrderType eZType, OverlaySurface* pZRefOverlay, GuiDialog* pDialog,
		GuiLoca* pLoca, GuiClient* pClient, SpriteSet* pSpriteSet, UVAnimation* pUVA )
		: GuiBase		( pLoca, pClient )
		, mpSpriteSet	(NULL)
		, mpSprite		(NULL)
	{
		//创建按钮图像精灵
		mpSprite = pSpriteSet->CreateUVASprite( pLoca->X, pLoca->Y, pLoca->Width, pLoca->Height,
			eZType, pZRefOverlay, pUVA );
		mpSpriteSet = pSpriteSet;
	}
	//--------------------------------------------------------------------------------------------------------------
	GuiSpriteUVA::~GuiSpriteUVA()
	{
		//删除按钮图像精灵
		if( mpSpriteSet != NULL && mpSprite != NULL )
			mpSpriteSet->DeleteSprite( mpSprite );
	}
	//--------------------------------------------------------------------------------------------------------------
	//显示/隐藏控件
	void FKFastcall GuiSpriteUVA::Show( bool bShow )
	{
		mpSprite->SetVisable( bShow );

		mbVisable = bShow;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置位置
	void FKFastcall GuiSpriteUVA::SetPos( long nX, long nY )
	{
		mLoca.X = nX;
		mLoca.Y = nY;

		mpSprite->SetLocation( nX, nY );
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置 Z 顺序
	void FKFastcall GuiSpriteUVA::SetZOrder( ZOrderType eZType, OverlaySurface* pZRefOverlay )
	{
		mpSprite->SetZOrder( eZType, pZRefOverlay );
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取 Z 顺序
	OverlaySurface* FKFastcall GuiSpriteUVA::GetZOrder( ZOrderType eZType )
	{
		return (OverlaySurface*)mpSprite;
	}
	//--------------------------------------------------------------------------------------------------------------
	//播放指定 UV 动画段
	void FKFastcall GuiSpriteUVA::PlaySegment( int nPlaySegment, int nNextSegment, bool bReverse )
	{
		mpSprite->mpUVASprite->PlaySegment( nPlaySegment, nNextSegment, bReverse );
	}
	//--------------------------------------------------------------------------------------------------------------
	//向用户界面对象发送消息
	LONG_PTR GuiSpriteUVA::SendMsg( LONG_PTR nGM, LONG_PTR nGMA, LONG_PTR nGMB )
	{
		switch (nGM)
		{
			case GM_Show:
				Show( ( nGMA == 1 ) ? true : false );
				return TRUE;
			case GM_SetPos:
				SetPos( nGMA, nGMB );
				return TRUE;
			case GM_MovePos:
				SetPos( mLoca.X + nGMA, mLoca.Y + nGMB );
				return TRUE;
			case GM_SetZOrder:
				SetZOrder( (ZOrderType)nGMA, (OverlaySurface*)nGMB );
				return TRUE;
			case GM_GetZOrder:
				return (LONG_PTR)GetZOrder( (ZOrderType)nGMA );
		}

		return FALSE;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================

