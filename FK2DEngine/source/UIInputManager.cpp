/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	UIInputManager
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../Include/UIInputManager.h"
#include "../Include/Exception.h"

//-----------------------------------------------------------------------------
namespace FK2DEngine
{
	//-----------------------------------------------------------------------------
	CGUIInputManager::CGUIInputManager()
	{
		m_ShiftState.m_bAltDown = false;
		m_ShiftState.m_bCtrlDown = false;
		m_ShiftState.m_bShiftDown = false;
		ZeroMemory(m_vKeyState, BUF_SIZE);
		ZeroMemory(m_vKeyStateOld, BUF_SIZE);
		m_MousePos.x = 0;
		m_MousePos.y = 0;
		m_hWnd = NULL;
	}
	//-----------------------------------------------------------------------------
	void CGUIInputManager::Init( HWND p_hWnd )
	{
		m_hWnd = p_hWnd;
	}
	//-----------------------------------------------------------------------------
	void CGUIInputManager::MsgProc( UINT p_Msg, WPARAM p_wParam, LPARAM p_lParam )
	{
		switch ( p_Msg )
		{
		case WM_KEYDOWN:
			m_QueueKeyDown.push((DWORD)p_wParam);
			break;
		case WM_KEYUP:
			m_QueueKeyUp.push((DWORD)p_wParam);
			break;
		case WM_CHAR:
			m_QueueChar.push((char)p_wParam);
			break;
		}
	}
	//-----------------------------------------------------------------------------
	void CGUIInputManager::Update( float p_fDelta )
	{
		FKAssert( m_hWnd != NULL , "UIInpt 窗口句柄不允许为空" );

		// 记录保存
		m_QueueKeyDownSave = m_QueueKeyDown;
		m_QueueKeyUpSave = m_QueueKeyUp;
		m_QueueCharSave = m_QueueChar;

		// 清空老记录
		while( !m_QueueKeyDown.empty() )
		{
			m_QueueKeyDown.pop();
		}
		while( !m_QueueKeyUp.empty() )
		{
			m_QueueKeyUp.pop();
		}
		while( !m_QueueChar.empty() )
		{
			m_QueueChar.pop();
		}

		// 刷新所有键状态
		memcpy(m_vKeyStateOld, m_vKeyState, BUF_SIZE);
		GetKeyboardState(m_vKeyState);

		// 更新功能键状态
		m_ShiftState.m_bCtrlDown = (m_vKeyState[VK_CONTROL] & 0x80) != 0;
		m_ShiftState.m_bShiftDown = (m_vKeyState[VK_SHIFT] & 0x80) != 0;
		m_ShiftState.m_bAltDown = (m_vKeyState[VK_MENU] & 0x80) != 0;

		// 更新鼠标状态
		m_MousePosOld = m_MousePos;
		GetCursorPos( &m_MousePos );
		ScreenToClient( m_hWnd, &m_MousePos );
	}
	//-----------------------------------------------------------------------------
	std::queue< unsigned int >	CGUIInputManager::GetKeyDown()
	{
		return m_QueueKeyDownSave;
	}
	//-----------------------------------------------------------------------------
	std::queue< unsigned int >	CGUIInputManager::GetKeyUp()
	{
		return m_QueueKeyUpSave;
	}
	//-----------------------------------------------------------------------------
	std::queue< char >	CGUIInputManager::GetKeyChar()
	{
		return m_QueueCharSave;
	}
	//-----------------------------------------------------------------------------
	SShiftState CGUIInputManager::GetShiftState()
	{
		return m_ShiftState;
	}
	//-----------------------------------------------------------------------------
	bool CGUIInputManager::GetMouseState( ENUM_MouseButton p_eMB )
	{
		switch( p_eMB )
		{
		case eMB_Left:
			return ( GetKeyState(VK_LBUTTON) == 0 ) ? false : true ;
		case eMB_Right:
			return ( GetKeyState(VK_RBUTTON) == 0 ) ? false : true ;
		case eMB_Middle:
			return ( GetKeyState(VK_MBUTTON) == 0 ) ? false : true ;
		default:
			return false;
		}
	}
	//-----------------------------------------------------------------------------
	POINT CGUIInputManager::GetMousePos()
	{
		return m_MousePos;
	}
	//-----------------------------------------------------------------------------
	bool CGUIInputManager::IsKeyJustDown( unsigned int p_dwKey )
	{
		return ( m_vKeyState[p_dwKey] & 0x80 ) && !( m_vKeyStateOld[p_dwKey] & 0x80 );
	}
	//-----------------------------------------------------------------------------
	bool CGUIInputManager::IsKeyJustUp( unsigned int p_dwKey )
	{
		return !( m_vKeyState[p_dwKey] & 0x80 ) &&( m_vKeyStateOld[p_dwKey] & 0x80 );
	}
	//-----------------------------------------------------------------------------
	bool CGUIInputManager::IsMouseJustDown( ENUM_MouseButton p_eMB )
	{
		switch( p_eMB )
		{
		case eMB_Left:
			return IsKeyJustDown(VK_LBUTTON);
		case eMB_Right:
			return IsKeyJustDown(VK_RBUTTON);
		case eMB_Middle:
			return IsKeyJustDown(VK_MBUTTON);
		default:
			return false;
		}
	}
	//-----------------------------------------------------------------------------
	bool CGUIInputManager::IsMouseJustUp( ENUM_MouseButton p_eMB )
	{
		switch( p_eMB )
		{
		case eMB_Left:
			return IsKeyJustUp(VK_LBUTTON);
		case eMB_Right:
			return IsKeyJustUp(VK_RBUTTON);
		case eMB_Middle:
			return IsKeyJustUp(VK_MBUTTON);
		default:
			return false;
		}
	}
	//-----------------------------------------------------------------------------
	bool CGUIInputManager::IsMouseMoved()
	{
		return (( m_MousePos.x != m_MousePosOld.x ) || ( m_MousePos.y != m_MousePosOld.y ));
	}
	//-----------------------------------------------------------------------------
	int CGUIInputManager::GetMousePosX()
	{
		return m_MousePos.x;
	}
	//-----------------------------------------------------------------------------
	int CGUIInputManager::GetMousePosY()
	{
		return m_MousePos.y;
	}
	//-----------------------------------------------------------------------------
}