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

#pragma once

//-----------------------------------------------------------------------------
#include "Singleton.h"
#include <queue>
#include <Windows.h>
//-----------------------------------------------------------------------------
namespace FK2DEngine
{
	//--------------------------------------------------------------------
	#define BUF_SIZE 256
	//--------------------------------------------------------------------
	// 功能键状态结构定义
	struct SShiftState
	{
		bool m_bCtrlDown;
		bool m_bShiftDown;
		bool m_bAltDown;
	};

	// 鼠标按钮枚举定义
	enum ENUM_MouseButton
	{
		eMB_Left,
		eMB_Right,
		eMB_Middle
	};
	//-----------------------------------------------------------------------------
	//! 专门负责UI消息的管理器
	class CGUIInputManager
	{
	public:
		CGUIInputManager();
		~CGUIInputManager()
		{

		}
	public:
		//! 初始化函数
		void Init( HWND p_hWnd );
		//! 消息处理函数
		void MsgProc( UINT p_Msg, WPARAM p_wParam, LPARAM p_lParam );
		//! 更新函数
		void Update( float p_fDelta );
	public:
		//! 获取上一帧后被按下的键
		std::queue< unsigned int >	GetKeyDown();
		//! 获取上一帧后被松开的键
		std::queue< unsigned int >	GetKeyUp();
		//! 获取上一帧后得到的字符
		std::queue< char >			GetKeyChar();
		//! 获取功能键状态
		SShiftState					GetShiftState();
		//! 获取鼠标按钮状态
		bool						GetMouseState( ENUM_MouseButton p_eMB );
		//! 获取鼠标位置
		POINT						GetMousePos();
		//! 指定键是否刚被按下
		bool						IsKeyJustDown( unsigned int p_dwKey );
		//! 指定键是否刚被松开
		bool						IsKeyJustUp( unsigned int p_dwKey );
		//! 鼠标是否刚被按下
		bool						IsMouseJustDown( ENUM_MouseButton p_eMB );
		//! 鼠标是否刚被松开
		bool						IsMouseJustUp( ENUM_MouseButton p_eMB );
		//! 鼠标是否移动
		bool						IsMouseMoved();
		//! 取鼠标X坐标
		int							GetMousePosX();
		//! 取鼠标Y坐标
		int							GetMousePosY();
	private:
		std::queue< unsigned int >			m_QueueKeyDown;					// 自上次Update()后被按下的键
		std::queue< unsigned int >			m_QueueKeyUp;					// 自上次Update()后被松开的键
		std::queue< char >					m_QueueChar;					// 自上次Update()后得到的字符队列	
		std::queue< unsigned int >			m_QueueKeyDownSave;				// 自上次Update()后被按下的键
		std::queue< unsigned int >			m_QueueKeyUpSave;				// 自上次Update()后被松开的键
		std::queue< char >					m_QueueCharSave;				// 自上次Update()后得到的字符队列	
		SShiftState							m_ShiftState;					// 功能键状态
		BYTE								m_vKeyState[BUF_SIZE];			// 键盘缓冲
		BYTE								m_vKeyStateOld[BUF_SIZE];		// 前次键盘缓冲
		POINT								m_MousePos;						// 鼠标位置
		POINT								m_MousePosOld;					// 前次鼠标位置
		HWND								m_hWnd;							// 窗口句柄
	private:
		DECLARE_SINGLETON_CLASS( CGUIInputManager );
	};
}

typedef FK2DEngine::TSingleton< FK2DEngine::CGUIInputManager >	SSGUIInputManager;