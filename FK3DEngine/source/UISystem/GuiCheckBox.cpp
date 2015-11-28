/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	GuiCheckBox
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
	GuiCheckBox::GuiCheckBox( ZOrderType eZType, OverlaySurface* pZRefOverlay, GuiDialog* pDialog, Param* pParam )
		: GuiSprite		( pDialog, &pParam->sLoca, &pParam->sClient, pParam->pSpriteSet )
		, mbChecked		(false)
		, mFnEvent		(pParam->fnEvent)
	{
		Texture* pSpriteTex = pParam->pSpriteSet->mpTexture;
		float fWidth = (float)mLoca.Width / (float)pSpriteTex->mWidth;
		float fHeight = (float)mLoca.Height / (float)pSpriteTex->mHeight;

		//计算复选框 UV
		mNormalUV.u1 = 0.0f;
		mNormalUV.u2 = fWidth;
		mNormalUV.v1 = 0.0f;
		mNormalUV.v2 = fHeight;

		mNormalLightUV.u1 = 0.0f;
		mNormalLightUV.u2 = fWidth;
		mNormalLightUV.v1 = fHeight;
		mNormalLightUV.v2 = fHeight + fHeight;

		mCheckedUV.u1 = fWidth;
		mCheckedUV.u2 = fWidth + fWidth;
		mCheckedUV.v1 = 0.0f;
		mCheckedUV.v2 = fHeight;

		mCheckedLightUV.u1 = fWidth;
		mCheckedLightUV.u2 = fWidth + fWidth;
		mCheckedLightUV.v1 = fHeight;
		mCheckedLightUV.v2 = fHeight + fHeight;

		//创建精灵
		mpSprite = mpSpriteSet->CreateSprite( mLoca.X, mLoca.Y, mLoca.Width, mLoca.Height,
			eZType, pZRefOverlay, &mNormalUV );
	}
	//--------------------------------------------------------------------------------------------------------------
	//获得/失去焦点
	void FKFastcall GuiCheckBox::Focus( bool bHasFocus )
	{
		mbFocus = bHasFocus;

		//如果获得焦点
		if( bHasFocus )
		{
			//切换为高光显示状态
			mpSprite->SetSpriteUV( mbChecked ? &mCheckedLightUV : &mNormalLightUV );
		}
		//如果失去焦点
		else
		{
			//切换为普通显示状态
			mpSprite->SetSpriteUV( mbChecked ? &mCheckedUV : &mNormalUV );
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//左键按下
	void FKFastcall GuiCheckBox::LDown( InputSystem* pInputSystem )
	{
		//切换复选状态
		mbChecked = !mbChecked;

		//如果设定复选
		if( mbChecked )
		{
			//切换为复选显示状态
			mpSprite->SetSpriteUV( mbFocus ? &mCheckedLightUV : &mCheckedUV );
		}
		//如果取消被复选
		else
		{
			//切换为普通显示状态
			mpSprite->SetSpriteUV( mbFocus ? &mNormalLightUV : &mNormalUV );
		}

		//将复选框事件加入帧事件列表
		if( mFnEvent != NULL && Root::mpSingleton->mpFrameListener != NULL )
			Root::mpSingleton->mpFrameListener->AddFrameEvent( mFnEvent, (LONG_PTR)this );
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置复选框事件函数
	void FKFastcall GuiCheckBox::SetEvent( FnFrameEvent fnEvent )
	{
		mFnEvent = fnEvent;
	}
	//--------------------------------------------------------------------------------------------------------------
	//向用户界面对象发送消息
	LONG_PTR GuiCheckBox::SendMsg( LONG_PTR nGM, LONG_PTR nGMA, LONG_PTR nGMB )
	{
		switch (nGM)
		{
			case GM_Enable:
				Enable( ( nGMA == 1 ) ? true : false );
				return TRUE;
			case GM_Show:
				Show( ( nGMA == 1 ) ? true : false );
				return TRUE;
			case GM_Focus:
				Focus( ( nGMA == 1 ) ? true : false );
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
			case GM_LDown:
				LDown( (InputSystem*)nGMA );
				return TRUE;
		}

		return FALSE;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================

