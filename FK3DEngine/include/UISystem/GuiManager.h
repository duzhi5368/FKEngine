/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	GuiManager
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	用户界面管理器
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================
class GuiBase;
#include "GuiCursor.h"
#include "GuiDialog.h"
//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	struct UVAnimation;
	//--------------------------------------------------------------------------------------------------------------
	//用户界面管理器
	class FK_DLL GuiManager : public Singleton< GuiManager >
	{
	public:
		bool			mbEnable;			//是否开启用户界面
		bool			mbVisalbe;			//是否显示用户界面
		bool			mbCursorVisable;	//是否显示光标

		GuiCursor*		mpCursorGui;		//光标用户界面对象

		bool			mbLockFocus;		//是否锁定焦点
		GuiBase*		mpFocusedGui;		//获得焦点的用户界面对象

	protected:
		GuiDlgList		mDialogList;		//用户界面对话框列表

	public:
		GuiManager();
		~GuiManager();

		//设置用户界面系统开启状态
		void SetEnable( bool bEnable );

		//设置用户界面显示状态
        void SetVisable( bool bVisable );

		//锁定焦点
		void LockFocus( GuiBase* pFocusGui );

		//解锁焦点
		void UnlockFocus();

		//设置光标显示状态
		void SetCursorVisable( bool bVisable );

		//创建光标对象
		void CreateCursor( UINT nWidth, UINT nHeight, int nTargetX, int nTargetY, SpriteSet* pSpriteSet, UVAnimation* pUVA );

		//删除光标对象
		void DeleteCursor();

		//显示/隐藏鼠标指针
		void ShowCursor( bool bShow = true );

		//创建对话框
		GuiDialog* CreateDialog( ZOrderType eZType, GuiDialog* pZRefDialog, GuiLoca* pLoca, GuiClient* pClient );

		//删除对话框
		void DeleteDialog( GuiDialog* pDialog );

		//删除所有对话框
		void DeleteAllDialog();

		//处理用户界面输入事件
		void ProcGuiInput( InputSystem* pInputSystem, float fFrameTime );

		//获取在当前鼠标位置上的用户界面对象
		GuiBase* GetGuiOnMouse( int x, int y );
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
