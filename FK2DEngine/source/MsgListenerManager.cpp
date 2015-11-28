/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	MsgListenerManager
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../include/MsgListenerManager.h"

//--------------------------------------------------------------------
namespace FK2DEngine
{
	//--------------------------------------------------------------------
	CWinMsgListenerManager::CWinMsgListenerManager()
	{
		m_listMsgListener.clear();
	}
	//--------------------------------------------------------------------
	CWinMsgListenerManager::~CWinMsgListenerManager()
	{
		m_listMsgListener.clear();
	}
	//--------------------------------------------------------------------
	void CWinMsgListenerManager::Add( IWinMsgListener *p_pListener)
	{
		m_listMsgListener.push_front( p_pListener );
	}
	//--------------------------------------------------------------------
	void CWinMsgListenerManager::Remove( IWinMsgListener* p_pListener )
	{
		m_listMsgListener.remove( p_pListener );
	}
	//--------------------------------------------------------------------
	void CWinMsgListenerManager::Distribute( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
		MsgListenerList::const_iterator Ite	= m_listMsgListener.begin();
		MsgListenerList::const_iterator IteEnd	= m_listMsgListener.end();
		for( ; Ite != IteEnd; ++Ite )
		{
			(*Ite)->Distribute( hWnd, uMsg, wParam, lParam );
		}
	}
	//--------------------------------------------------------------------
}