/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	MsgListener
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include <windows.h>
//--------------------------------------------------------------------
namespace FK2DEngine
{
	//! Windows消息监听器类
	//@remarks 对Windows消息感兴趣的类均可集成本类，实现具体操作
	class IWinMsgListener
	{
	public:
		virtual ~IWinMsgListener()
		{

		};
	public:
		//! 分发Windows消息
		virtual void Distribute( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) = 0;
	};
}