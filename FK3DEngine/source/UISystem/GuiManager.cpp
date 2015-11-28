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
	GuiManager::GuiManager()
		: mbEnable			(true)
		, mbVisalbe			(true)
		, mbCursorVisable	(true)
		, mpCursorGui		(NULL)
		, mbLockFocus		(false)
		, mpFocusedGui		(NULL)
	{
		mDialogList.Initialize( 6, 6 );
	}
	//--------------------------------------------------------------------------------------------------------------
	GuiManager::~GuiManager()
	{
		DeleteCursor();
		DeleteAllDialog();

		mDialogList.Release();
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置用户界面系统开启状态
	void GuiManager::SetEnable( bool bEnable )
	{
		mbEnable = bEnable;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置用户界面显示状态
	void GuiManager::SetVisable( bool bVisable )
	{
		mbVisalbe = bVisable;

		//设置所有用户界面对象显示状态
		LONG_PTR nGMA = bVisable ? 1 : 0;

		GuiDlgList::Iterator it = mDialogList.Begin();
		GuiDlgList::Iterator end = mDialogList.End();

		for(; it!=end; ++it )
		{
			if( *it != NULL )
				(*it)->SendMsg( GM_Show, nGMA );
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//锁定焦点
	void GuiManager::LockFocus( GuiBase* pFocusGui )
	{
		mbLockFocus = true;
		mpFocusedGui = pFocusGui;
	}
	//--------------------------------------------------------------------------------------------------------------
	//解锁焦点
	void GuiManager::UnlockFocus()
	{
		mbLockFocus = false;
		mpFocusedGui = NULL;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置光标显示状态
	void GuiManager::SetCursorVisable( bool bVisable )
	{
		mbCursorVisable = bVisable;

		if( mpCursorGui != NULL )
			mpCursorGui->Show( bVisable );
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建光标对象
	void GuiManager::CreateCursor( UINT nWidth, UINT nHeight, int nTargetX, int nTargetY, SpriteSet* pSpriteSet, UVAnimation* pUVA )
	{
		GuiLoca sLoca;
		sLoca.X = 0;
		sLoca.Y = 0;
		sLoca.Width = nWidth;
		sLoca.Height = nHeight;

		mpCursorGui = new GuiCursor( &sLoca, nTargetX, nTargetY, pSpriteSet, pUVA );
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除光标对象
	void GuiManager::DeleteCursor()
	{
		SafeDelete( mpCursorGui );
	}
	//--------------------------------------------------------------------------------------------------------------
	//显示/隐藏鼠标指针
	void GuiManager::ShowCursor( bool bShow )
	{
		if( mpCursorGui != NULL )
			mpCursorGui->Show( bShow );
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建对话框
	GuiDialog* GuiManager::CreateDialog( ZOrderType eZType, GuiDialog* pZRefDialog, GuiLoca* pLoca, GuiClient* pClient )
	{
		//创建新对话框
		GuiDialog* pDialog = new GuiDialog( pLoca, pClient );

		GuiDialog* pBelowDialog = NULL;	//新建对话框之下对话框
		GuiDialog* pUponDialog = NULL;	//新建对话框之上对话框

		switch (eZType)
		{
			//最上层（最后渲染）
		case ZOT_Top:
			{
				if( mDialogList.Size() > 0 )
					pUponDialog = *mDialogList.Back();

				*mDialogList.Push() = pDialog;
				goto FixList;
			}
			//最下层（最先渲染）
		case ZOT_Bottom:
			{
				if( mDialogList.Size() > 0 )
					pBelowDialog = *mDialogList.Begin();

				*mDialogList.Insert( (unsigned int)0 ) = pDialog;
				goto FixList;
			}
			//指定对话框之上（后渲染）
		case ZOT_Upon:
			{
				if( mDialogList.Size() == 0 )
					break;

				//如果指定基准对话框是最顶层对话框
				GuiDlgList::Iterator back = mDialogList.Back();
				if( pZRefDialog == *back )
				{
					pBelowDialog = pZRefDialog;
					*mDialogList.Push() = pDialog;
					goto FixList;
				}
				else
				{
					GuiDlgList::Iterator beg = mDialogList.Begin();
					GuiDlgList::Iterator it = --back;
					for(; it>=beg; --it )
					{
						if( *it == pZRefDialog )
						{
							pBelowDialog = *it;
							pUponDialog = *( ++it );
							*mDialogList.Insert( it ) = pDialog;
							goto FixList;
						}
					}
				}
				break;
			}
			//指定对话框之下（先渲染）
		case ZOT_Below:
			{
				if( mDialogList.Size() == 0 )
					break;

				//如果指定基准对话框是最底层对话框
				GuiDlgList::Iterator begin = mDialogList.Begin();
				if( pZRefDialog == *begin )
				{
					pUponDialog = *begin;
					*mDialogList.Insert( (unsigned int)0 ) = pDialog;
					goto FixList;
				}
				else
				{
					GuiDlgList::Iterator it = ++begin;
					GuiDlgList::Iterator end = mDialogList.End();
					for(; it!=end; ++it )
					{
						if( *it == pZRefDialog )
						{
							pBelowDialog = *( it - 1 );
							pUponDialog = *it;
							*mDialogList.Insert( it ) = pDialog;
							goto FixList;
						}
					}
				}
				break;
			}
		}

		//Except( Exception::ERR_INVALIDPARAMS, "指定的对话框 Z 顺序错误，无法将其加入列表。" );

FixList:
		//重新设置对话框链表指针
		pDialog->mpBottomDialog = pBelowDialog;

		if( pUponDialog != NULL )
			pUponDialog->mpBottomDialog = pDialog;

		return pDialog;
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除对话框
	void GuiManager::DeleteDialog( GuiDialog* pDialog )
	{
		if( pDialog == NULL )
			return;

		GuiDlgList::Iterator begin = mDialogList.Begin();
		GuiDlgList::Iterator back = mDialogList.Back();
		GuiDlgList::Iterator it = begin;
		for(; it>=back; ++it )
		{
			if( *it == pDialog )
			{
				//如果删除对话框存在上层对话框
				if( it != back )
				{
					GuiDialog* pUponDialog = *( it + 1 );
					pUponDialog->mpBottomDialog = ( it != begin ) ? *( it - 1 ) : NULL;
				}

				mDialogList.Erase( it );
				delete pDialog;
				return;
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除所有对话框
	void GuiManager::DeleteAllDialog()
	{
		GuiDlgList::Iterator it = mDialogList.Begin();
		GuiDlgList::Iterator end = mDialogList.End();
		for(; it!=end; ++it )
		{
			if( *it != NULL )
				delete *it;
		}

		mDialogList.Clear();
	}
	//--------------------------------------------------------------------------------------------------------------
	//处理用户界面输入事件
	void GuiManager::ProcGuiInput( InputSystem* pInputSystem, float fFrameTime )
	{
		//如果用户界面系统关闭则直接返回
		if( !mbEnable )
			return;

		//如果没有锁定焦点
		if( !mbLockFocus )
		{
			//获取在当前鼠标位置上的用户界面对象
			GuiBase* pTargetGui = GetGuiOnMouse( pInputSystem->mMouseX, pInputSystem->mMouseY );

			//如果焦点发生变化
			if( mpFocusedGui != pTargetGui )
			{
				//通知上次获得焦点的对象失去焦点
				if( mpFocusedGui != NULL )
					mpFocusedGui->SendMsg( GM_Focus, 0 );

				//通知新获得焦点的对象
				if( pTargetGui != NULL )
					pTargetGui->SendMsg( GM_Focus, 1 );

				mpFocusedGui = pTargetGui;
			}
		}

		//优先处理双击事件，其次处理按键、放键。
		if( mpFocusedGui != NULL )
		{
			if( pInputSystem->mbMouseDBClick[0] )
				mpFocusedGui->SendMsg( GM_LDBClick, (LONG_PTR)pInputSystem, NULL );
			else if( pInputSystem->mbMouseDown[0] )
				mpFocusedGui->SendMsg( GM_LDown, (LONG_PTR)pInputSystem, NULL );
			else if( pInputSystem->mbMouseUp[0] )
				mpFocusedGui->SendMsg( GM_LUp, (LONG_PTR)pInputSystem, NULL );
		}

		if( mpFocusedGui != NULL )
		{
			if( pInputSystem->mbMouseDBClick[1] )
				mpFocusedGui->SendMsg( GM_RDBClick, (LONG_PTR)pInputSystem, NULL );
			else if( pInputSystem->mbMouseDown[1] )
				mpFocusedGui->SendMsg( GM_RDown, (LONG_PTR)pInputSystem, NULL );
			else if( pInputSystem->mbMouseUp[1] )
				mpFocusedGui->SendMsg( GM_RUp, (LONG_PTR)pInputSystem, NULL );
		}

		//发送鼠标移动消息
		if( mpFocusedGui != NULL && ( pInputSystem->mMouseRelX != 0 || pInputSystem->mMouseRelY != 0 ) )
			mpFocusedGui->SendMsg( GM_MouseMove, (LONG_PTR)pInputSystem );

		//如果存在光标用户界面对象
		if( mpCursorGui != NULL )
		{
			//更新光标位置
			mpCursorGui->SetPos( (long)pInputSystem->mMouseX, (long)pInputSystem->mMouseY );

			//如果光标位置已改变
			if( pInputSystem->mMouseRelX != 0 || pInputSystem->mMouseRelY != 0 )
			{
				//更新光标激活状态
				mpCursorGui->UpdateActivate();
			}
			else
			{
				//更新光标闲置时间
				mpCursorGui->UpdateIdleTime( fFrameTime );
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取在当前鼠标位置上的用户界面对象指针
	GuiBase* GuiManager::GetGuiOnMouse( int x, int y )
	{
		//遍历所有对话框
		GuiDlgList::Iterator it = mDialogList.Begin();
		GuiDlgList::Iterator end = mDialogList.End();
		for(; it!=end; ++it )
		{
			GuiDialog* pDialog = *it;

			//如果鼠标在该对话框客户区之内
			if( pDialog->IsOnClient( x, y ) )
			{
				GuiBase* pGui = pDialog->GetGuiOnMouse( x, y );
				if( pGui != NULL )
					return pGui;
			}
		}

		return NULL;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
