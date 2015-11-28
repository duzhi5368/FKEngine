/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	GuiDialog
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	用户界面对话框 
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================
#include "GuiBase.h"
#include "GuiButton.h"
#include "GuiComboBox.h"
#include "GuiListBox.h"
#include "GuiTrackBar.h"
#include "GuiCheckBox.h"
//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//用户界面对话框
	class FK_DLL GuiDialog : public GuiBase
	{
	public:
		GuiList			mGuiList;			//用户界面对象列表

		GuiDialog*		mpBottomDialog;		//下层对话框

	public:
		GuiDialog( GuiLoca* pLoca, GuiClient* pClient );
		virtual ~GuiDialog();

		//创建按钮对象
		GuiButton* CreateButton( GuiButton::Param* pParam );

		//创建复选框对象
		GuiCheckBox* CreateCheckBox( GuiCheckBox::Param* pParam );

		//创建组合框对象
		GuiComboBox* CreateComboBox( GuiComboBox::Param* pParam );

		//创建滑动条
		GuiTrackBar* CreateTrackBar( GuiTrackBar::Param* pParam );

		//删除用户界面对象
		void DeleteGui( GuiBase* pGui );

		//删除所有用户界面对象
		void DeleteAllGui();

		//获取在当前鼠标位置上的用户界面对象
		GuiBase* FKFastcall GetGuiOnMouse( int x, int y );

		//获取对话框顶部 Z 顺序
		void FKFastcall GetTopZOrder( ZOrderType* pZType, OverlaySurface** ppZRefOverlay );

	public:
		//向用户界面对象发送消息
		LONG_PTR SendMsg( LONG_PTR nGM, LONG_PTR nGMA = 0, LONG_PTR nGMB = 0 );
	};
	//--------------------------------------------------------------------------------------------------------------
	typedef Stack< GuiDialog* >		GuiDlgList;
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================

