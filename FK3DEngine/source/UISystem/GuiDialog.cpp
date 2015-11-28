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
	GuiDialog::GuiDialog( GuiLoca* pLoca, GuiClient* pClient )
		: GuiBase			( pLoca, pClient )
		, mpBottomDialog	(NULL)
	{
		mGuiList.Initialize( 10, 10 );
	}
	//--------------------------------------------------------------------------------------------------------------
	GuiDialog::~GuiDialog()
	{
		DeleteAllGui();

		mGuiList.Release();
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建按钮对象
	GuiButton* GuiDialog::CreateButton( GuiButton::Param* pParam )
	{
		ZOrderType eZType;
		OverlaySurface* pZRefOverlay;
		GetTopZOrder( &eZType, &pZRefOverlay );

		GuiButton* pButton = new GuiButton( eZType, pZRefOverlay, this, pParam );
		*mGuiList.Push() = pButton;
		return pButton;
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建复选框对象
	GuiCheckBox* GuiDialog::CreateCheckBox( GuiCheckBox::Param* pParam )
	{
		ZOrderType eZType;
		OverlaySurface* pZRefOverlay;
		GetTopZOrder( &eZType, &pZRefOverlay );

		GuiCheckBox* pCheckBox = new GuiCheckBox( eZType, pZRefOverlay, this, pParam );
		*mGuiList.Push() = pCheckBox;
		return pCheckBox;
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建组合框对象
	GuiComboBox* GuiDialog::CreateComboBox( GuiComboBox::Param* pParam )
	{
		ZOrderType eZType;
		OverlaySurface* pZRefOverlay;
		GetTopZOrder( &eZType, &pZRefOverlay );

		GuiComboBox* pComboBox = new GuiComboBox( eZType, pZRefOverlay, this, pParam );
		*mGuiList.Push() = pComboBox;
		return pComboBox;
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建滑动条
	GuiTrackBar* GuiDialog::CreateTrackBar( GuiTrackBar::Param* pParam )
	{
		ZOrderType eZType;
		OverlaySurface* pZRefOverlay;
		GetTopZOrder( &eZType, &pZRefOverlay );

		GuiTrackBar* pTrackBar = new GuiTrackBar( eZType, pZRefOverlay, this, pParam );
		*mGuiList.Push() = pTrackBar;
		return pTrackBar;
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除用户界面对象
	void GuiDialog::DeleteGui( GuiBase* pGui )
	{
		if( pGui == NULL )
			return;

		GuiList::Iterator it = mGuiList.Begin();
		GuiList::Iterator end = mGuiList.End();

		for(; it!=end; ++it )
		{
			if( *it == pGui )
			{
				mGuiList.Erase( it );
				delete pGui;
				return;
			}
		}

		Except( Exception::ERR_INVALIDPARAMS, "指定删除的用户界面对象不存在。" );
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除所有用户界面对象
	void GuiDialog::DeleteAllGui()
	{
		GuiList::Iterator it = mGuiList.Begin();
		GuiList::Iterator end = mGuiList.End();

		for(; it!=end; ++it )
		{
			if( *it != NULL )
				delete *it;
		}

		mGuiList.Clear();
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取在当前鼠标位置上的用户界面对象指针
	GuiBase* FKFastcall GuiDialog::GetGuiOnMouse( int x, int y )
	{
		//遍历所有用户界面对象
		GuiList::Iterator it = mGuiList.Begin();
		GuiList::Iterator end = mGuiList.End();
		for(; it!=end; ++it )
		{
			GuiBase* pGui = *it;

			//如果鼠标在该用户界面对象客户区之内
			if( pGui->IsOnClient( x, y ) )
				return pGui;
		}

		return NULL;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取对话框顶部 Z 顺序
	void FKFastcall GuiDialog::GetTopZOrder( ZOrderType* pZType, OverlaySurface** ppZRefOverlay )
	{
		//如果该对话框中未创建任何用户界面控件
		if( mGuiList.Size() == 0 )
		{
			//如果该对话框存在下层对话框
			if( mpBottomDialog != NULL )
			{
				mpBottomDialog->GetTopZOrder( pZType, ppZRefOverlay );
			}
			else
			{
				GuiCursor* pCursor = GuiManager::mpSingleton->mpCursorGui;
				if( pCursor != NULL )
				{
					*pZType = ZOT_Below;
					*ppZRefOverlay = pCursor->mpSprite;
				}
				else
				{
					OverlaySurface* pTopmost = OverlayManager::mpSingleton->GetTopmostOverlay();
					*pZType = ( pTopmost == NULL ) ? ZOT_Top : ZOT_Upon;
					*ppZRefOverlay = pTopmost;
				}
			}
		}
		else
		{
			GuiBase* pGui = *mGuiList.Back();

			*pZType = ZOT_Upon;
			*ppZRefOverlay = (OverlaySurface*)pGui->SendMsg( GM_GetZOrder, (LONG_PTR)ZOT_Top );
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//向用户界面对象发送消息
	LONG_PTR GuiDialog::SendMsg( LONG_PTR nGM, LONG_PTR nGMA, LONG_PTR nGMB )
	{
		return 0;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================

