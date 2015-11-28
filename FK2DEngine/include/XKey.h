/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	XKey
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
namespace FK2DEngine
{
	enum ENUM_KeyName
	{
		EKB_RangeBegin			= 0x00,
		EKB_A 					= 'a',
		EKB_B 					= 'b',
		EKB_C 					= 'c',
		EKB_D 					= 'd',
		EKB_E 					= 'e',
		EKB_F 					= 'f',
		EKB_G 					= 'g',
		EKB_H 					= 'h',
		EKB_I 					= 'i',
		EKB_J 					= 'j',
		EKB_K 					= 'k',
		EKB_L 					= 'l',
		EKB_M 					= 'm',
		EKB_N 					= 'n',
		EKB_O 					= 'o',
		EKB_P 					= 'p',
		EKB_Q 					= 'q',
		EKB_R 					= 'r',
		EKB_S 					= 's',
		EKB_T 					= 't',
		EKB_U 					= 'u',
		EKB_V 					= 'v',
		EKB_W 					= 'w',
		EKB_X 					= 'x',
		EKB_Y 					= 'y',
		EKB_Z 					= 'z',
		EKB_Escape				= XK_Escape,
		EKB_F1 					= XK_F1,
		EKB_F2 					= XK_F2,
		EKB_F3 					= XK_F3,
		EKB_F4 					= XK_F4,
		EKB_F5 					= XK_F5,
		EKB_F6 					= XK_F6,
		EKB_F7 					= XK_F7,
		EKB_F8 					= XK_F8,
		EKB_F9 					= XK_F9,
		EKB_F10 				= XK_F10,
		EKB_F11 				= XK_F11,
		EKB_F12 				= XK_F12,
		EKB_1 					= XK_1,
		EKB_2 					= XK_2,
		EKB_3 					= XK_3,
		EKB_4 					= XK_4,
		EKB_5 					= XK_5,
		EKB_6 					= XK_6,
		EKB_7 					= XK_7,
		EKB_8 					= XK_8,
		EKB_9 					= XK_9,
		EKB_0 					= XK_0,
		EKB_Tab					= XK_Tab,
		EKB_Return				= XK_Return,
		EKB_Space				= XK_space,
		EKB_LeftShift			= XK_Shift_L,
		EKB_RightShift			= XK_Shift_R,
		EKB_LeftControl			= XK_Control_L,
		EKB_RightControl		= XK_Control_R,
		EKB_LeftAlt				= XK_Alt_L,
		EKB_RightAlt			= XK_Alt_R,
		EKB_LeftMeta			= 0, 
		EKB_RightMeta			= 0,
		EKB_Backspace			= XK_BackSpace,
		EKB_Left				= XK_Left,
		EKB_Right				= XK_Right,
		EKB_Up					= XK_Up,
		EKB_Down				= XK_Down,
		EKB_Home				= XK_Home,
		EKB_End					= XK_End,
		EKB_Insert 				= XK_Insert,
		EKB_Delete 				= XK_Delete,
		EKB_PageUp 				= XK_Prior,
		EKB_PageDown			= XK_Next,
		EKB_Enter				= XK_KP_Enter,
		EKB_Numpad1 			= XK_KP_1,
		EKB_Numpad2 			= XK_KP_2,
		EKB_Numpad3 			= XK_KP_3,
		EKB_Numpad4 			= XK_KP_4,
		EKB_Numpad5 			= XK_KP_5,
		EKB_Numpad6 			= XK_KP_6,
		EKB_Numpad7 			= XK_KP_7,
		EKB_Numpad8 			= XK_KP_8,
		EKB_Numpad9 			= XK_KP_9,
		EKB_Numpad0 			= XK_KP_0,
		EKB_NumpadAdd			= XK_KP_Add,
		EKB_NumpadSubtract 		= XK_KP_Subtract,
		EKB_NumpadMultiply 		= XK_KP_Multiply,
		EKB_NumpadDivide		= XK_KP_Divide,
		EKB_RangeEnd			= 0xffff,

		MS_RangeBegin,
		MS_Left					= MS_RangeBegin,
		MS_Right,
		MS_Middle,
		MS_WheelUp,
		MS_WheelDown,
		MS_RangeEnd,

		GP_RangeBegin,
		GP_Left					= GP_RangeBegin,
		GP_Right,
		GP_Up,
		GP_Down,
		GP_Button0,
		GP_Button1,
		GP_Button2,
		GP_Button3,
		GP_Button4,
		GP_Button5,
		GP_Button6,
		GP_Button7,
		GP_Button8,
		GP_Button9,
		GP_Button10,
		GP_Button11,
		GP_Button12,
		GP_Button13,
		GP_Button14,
		GP_Button15,
		GP_RangeEnd				= GP_Button15,

		EKB_Num		= EKB_RangeEnd - EKB_RangeBegin + 1,
		MS_Num		= MS_RangeEnd - MS_RangeBegin + 1,
		GP_Num		= GP_RangeEnd - GP_RangeBegin + 1,

		NumButtons	= GP_RangeEnd,
		NoButton	= 0xffffffff
	};
}