/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	GuiSprite
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
	GuiSprite::GuiSprite( GuiDialog* pDialog, GuiLoca* pLoca, GuiClient* pClient, SpriteSet* pSpriteSet )
		: GuiBase		( pLoca, pClient, pDialog )
		, mpSpriteSet	(pSpriteSet)
		, mpSprite		(NULL)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	GuiSprite::~GuiSprite()
	{
		//删除按钮图像精灵
		if( mpSpriteSet != NULL && mpSprite != NULL )
			mpSpriteSet->DeleteSprite( mpSprite );
	}
	//--------------------------------------------------------------------------------------------------------------
	//开启/关闭控件
	void FKFastcall GuiSprite::Enable( bool bEnable )
	{
		mbEnable = bEnable;
	}
	//--------------------------------------------------------------------------------------------------------------
	//显示/隐藏控件
	void FKFastcall GuiSprite::Show( bool bShow )
	{
		mpSprite->SetVisable( bShow );

		mbVisable = bShow;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置位置
	void FKFastcall GuiSprite::SetPos( long nX, long nY )
	{
		mLoca.X = nX;
		mLoca.Y = nY;

		mpSprite->SetLocation( nX, nY );
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置 Z 顺序
	void FKFastcall GuiSprite::SetZOrder( ZOrderType eZType, OverlaySurface* pZRefOverlay )
	{
		mpSprite->SetZOrder( eZType, pZRefOverlay );
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取 Z 顺序
	OverlaySurface* FKFastcall GuiSprite::GetZOrder( ZOrderType eZType )
	{
		return (OverlaySurface*)mpSprite;
	}
	//--------------------------------------------------------------------------------------------------------------
	//向用户界面对象发送消息
	LONG_PTR GuiSprite::SendMsg( LONG_PTR nGM, LONG_PTR nGMA, LONG_PTR nGMB )
	{
		switch (nGM)
		{
			case GM_Enable:
				Enable( ( nGMA == 1 ) ? true : false );
				return TRUE;
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

