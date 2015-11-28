/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	GuiListBox
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
	GuiListBox::GuiListBox( GuiDialog* pDialog, Param* pParam )
		: GuiBase		( NULL, NULL, pDialog )
		, mpSpriteSet	(pParam->pSpriteSet)
		, mpTextFont	(pParam->pFontFace)
		, mX			(pParam->nX)
		, mY			(pParam->nY)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	GuiListBox::~GuiListBox()
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	//开启/关闭控件
	void FKFastcall GuiListBox::Enable( bool bEnable )
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	//显示/隐藏控件
	void FKFastcall GuiListBox::Show( bool bShow )
	{
		mbVisable = bShow;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置位置
	void FKFastcall GuiListBox::SetPos( long nX, long nY )
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置 Z 顺序
	void FKFastcall GuiListBox::SetZOrder( ZOrderType eZType, OverlaySurface* pZRefOverlay )
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取 Z 顺序
	OverlaySurface* FKFastcall GuiListBox::GetZOrder( ZOrderType eZType )
	{
		return NULL;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获得/失去焦点
	void FKFastcall GuiListBox::Focus( bool bHasFocus )
	{
		mbFocus = bHasFocus;
	}
	//--------------------------------------------------------------------------------------------------------------
	//鼠标移动
	void FKFastcall GuiListBox::MouseMove( InputSystem* pInputSystem )
	{
		//转换为控件坐标系
		int x = pInputSystem->mMouseX - mLoca.X;
		int y = pInputSystem->mMouseY - mLoca.Y;
	}
	//--------------------------------------------------------------------------------------------------------------
	//左键按下
	void FKFastcall GuiListBox::LDown( InputSystem* pInputSystem )
	{
		//转换为控件坐标系
		int nX = pInputSystem->mMouseX - mLoca.X;
		int nY = pInputSystem->mMouseY - mLoca.Y;
	}
	//--------------------------------------------------------------------------------------------------------------
	//向用户界面对象发送消息
	LONG_PTR GuiListBox::SendMsg( LONG_PTR nGM, LONG_PTR nGMA, LONG_PTR nGMB )
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
			case GM_MouseMove:
				MouseMove( (InputSystem*)nGMA );
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

