/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	UserMsgNotify
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "UIManager.h"
//--------------------------------------------------------------------
namespace FK2DEngine
{
	//--------------------------------------------------------------------
	//@remarks 用户用该组宏将控件和回调事件捆绑起来
	//--------------------------------------------------------------------
	#define ON_MOUSEENTER(obj, fun) \
		{ \
		(obj)->OnMouseEnter.function = NotifyFun(fun); \
		(obj)->OnMouseEnter.invoker = this; \
	}

	#define ON_MOUSELEAVE(obj, fun) \
		{ \
		(obj)->OnMouseLeave.function = NotifyFun(fun); \
		(obj)->OnMouseLeave.invoker = this; \
	}

	#define ON_MOUSEMOVE(obj, fun) \
		{ \
		(obj)->OnMouseMove.function = MouseMoveFun(fun); \
		(obj)->OnMouseMove.invoker = this; \
	}

	#define ON_MOUSEDOWN(obj, fun) \
		{ \
		(obj)->OnMouseDown.function = MouseFun(fun); \
		(obj)->OnMouseDown.invoker = this; \
	}

	#define ON_MOUSEUP(obj, fun) \
		{ \
		(obj)->OnMouseUp.function = MouseFun(fun); \
		(obj)->OnMouseUp.invoker = this; \
	}

	#define ON_KEYDOWN(obj, fun) \
		{ \
		(obj)->OnKeyDown.function = KeyFun(fun); \
		(obj)->OnKeyDown.invoker = this; \
	}

	#define ON_KEYUP(obj, fun) \
		{ \
		(obj)->OnKeyUp.function = KeyFun(fun); \
		(obj)->OnKeyUp.invoker = this; \
	}

	#define ON_KEYPRESS(obj, fun) \
		{ \
		(obj)->OnKeyPress.function = KeyPressFun(fun); \
		(obj)->OnKeyPress.invoker = this; \
	}

	#define ON_ENTER(obj, fun) \
		{ \
		(obj)->OnEnter.function = NotifyFun(fun); \
		(obj)->OnEnter.invoker = this; \
	}

	#define ON_LEAVE(obj, fun) \
		{ \
		(obj)->OnLeave.function = NotifyFun(fun); \
		(obj)->OnLeave.invoker = this; \
	}

	#define ON_CLICK(obj, fun) \
		{ \
		(obj)->OnClick.function = NotifyFun(fun); \
		(obj)->OnClick.invoker = this; \
	}

	#define ON_CHANGE(obj, fun) \
		{ \
		(obj)->OnChange.function = NotifyFun(fun); \
		(obj)->OnChange.invoker = this; \
	}

	#define ON_SCROLL(obj, fun) \
		{ \
		(obj)->OnScroll.function = NotifyFun(fun); \
		(obj)->OnScroll.invoker = this; \
	}
}
