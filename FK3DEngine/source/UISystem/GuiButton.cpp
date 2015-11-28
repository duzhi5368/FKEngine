/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	GuiButton
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
	GuiButton::GuiButton( ZOrderType eZType, OverlaySurface* pZRefOverlay, GuiDialog* pDialog, Param* pParam )
		: GuiSprite		( pDialog, &pParam->sLoca, &pParam->sClient, pParam->pSpriteSet )
		, mpUV			(pParam->pUV)
		, mFnEvent		(pParam->fnEvent)
		, mbPressed		(false)
	{
		//创建精灵
		mpSprite = mpSpriteSet->CreateSprite( mLoca.X, mLoca.Y, mLoca.Width, mLoca.Height,
			eZType, pZRefOverlay, mpUV + mnNormalStatus );
	}
	//--------------------------------------------------------------------------------------------------------------
	//获得/失去焦点
	void FKFastcall GuiButton::Focus( bool bHasFocus )
	{
		mbFocus = bHasFocus;

		//如果获得焦点
		if( bHasFocus )
		{
			//切换为焦点显示状态
			mpSprite->SetSpriteUV( mpUV + mnFocusStatus );
		}
		//如果失去焦点
		else
		{
			//切换为普通显示状态
			mpSprite->SetSpriteUV( mpUV + mnNormalStatus );

			//设置按钮还未被按下过
			mbPressed = false;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//左键按下
	void FKFastcall GuiButton::LDown( InputSystem* pInputSystem )
	{
		//切换为按下显示状态
		mpSprite->SetSpriteUV( mpUV + mnPressStatus );

		//设置按钮已被按下过
		mbPressed = true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//左键释放
	void FKFastcall GuiButton::LUp( InputSystem* pInputSystem )
	{
		//切换为普通显示状态
		mpSprite->SetSpriteUV( mpUV + mnFocusStatus );

		//如果按钮已被按下过
		if( mbPressed )
		{
			//将按钮事件加入帧事件列表
			if( mFnEvent != NULL && Root::mpSingleton->mpFrameListener != NULL )
				Root::mpSingleton->mpFrameListener->AddFrameEvent( mFnEvent, (LONG_PTR)this );

			mbPressed = false;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置按钮事件函数
	void FKFastcall GuiButton::SetEvent( FnFrameEvent fnEvent )
	{
		mFnEvent = fnEvent;
	}
	//--------------------------------------------------------------------------------------------------------------
	//向用户界面对象发送消息
	LONG_PTR GuiButton::SendMsg( LONG_PTR nGM, LONG_PTR nGMA, LONG_PTR nGMB )
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
			case GM_LUp:
				LUp( (InputSystem*)nGMA );
				return TRUE;
		}

		return FALSE;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================

