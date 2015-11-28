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

#pragma once

//--------------------------------------------------------------------
#include "MsgListener.h"
#include "Singleton.h"
#include <list>
//--------------------------------------------------------------------
namespace FK2DEngine
{
	class CWinMsgListenerManager 
	{ 
		typedef std::list< IWinMsgListener* >		MsgListenerList;
	public:
		CWinMsgListenerManager();
		~CWinMsgListenerManager();
	public:
		void Add( IWinMsgListener* p_pListener );
		void Remove( IWinMsgListener* p_pListener );
		void Distribute( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	private:
		MsgListenerList		m_listMsgListener;

		DECLARE_SINGLETON_CLASS( CWinMsgListenerManager );
	};
}

typedef FK2DEngine::TSingleton< FK2DEngine::CWinMsgListenerManager >		SSWinMsgListenerMgr;