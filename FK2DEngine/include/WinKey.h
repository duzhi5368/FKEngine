/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	WinKey
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
		// 按键信息
		EKB_RangeBegin		= 0x00,
		EKB_Escape			= 0x01,
		EKB_F1				= 0x3b,
		EKB_F2				= 0x3c,
		EKB_F3				= 0x3d,
		EKB_F4				= 0x3e,
		EKB_F5				= 0x3f,
		EKB_F6				= 0x40,
		EKB_F7				= 0x41,
		EKB_F8				= 0x42,
		EKB_F9				= 0x43,
		EKB_F10				= 0x44,
		EKB_F11				= 0x57,
		EKB_F12				= 0x58,
		EKB_0				= 0x0b,
		EKB_1				= 0x02,
		EKB_2				= 0x03,
		EKB_3				= 0x04,
		EKB_4				= 0x05,
		EKB_5				= 0x06,
		EKB_6				= 0x07,
		EKB_7				= 0x08,
		EKB_8				= 0x09,
		EKB_9				= 0x0a,
		EKB_Tab				= 0x0f,
		EKB_Return			= 0x1c,
		EKB_Space			= 0x39,
		EKB_LeftShift		= 0x2a,
		EKB_RightShift		= 0x36,
		EKB_LeftControl		= 0x1d,
		EKB_RightControl	= 0x9d,
		EKB_LeftAlt			= 0x38,
		EKB_RightAlt		= 0xb8,
		EKB_LeftMeta		= 0xdb,
		EKB_RightMeta		= 0xdc,
		EKB_Backspace		= 0x0e,
		EKB_Left			= 0xcb,
		EKB_Right			= 0xcd,
		EKB_Up				= 0xc8,
		EKB_Down			= 0xd0,
		EKB_Home			= 0xc7,
		EKB_End				= 0xcf,
		EKB_Insert			= 0xd2,
		EKB_Delete			= 0xd3,
		EKB_PageUp			= 0xc9,
		EKB_PageDown		= 0xd1,
		EKB_Enter			= 0x9c,
		EKB_PrintScreen		= 0xb7,
		EKB_A				= 0x1e,
		EKB_B 				= 0x30,
		EKB_C 				= 0x2e,
		EKB_D 				= 0x20,
		EKB_E 				= 0x12,
		EKB_F 				= 0x21,
		EKB_G 				= 0x22,
		EKB_H 				= 0x23,
		EKB_I 				= 0x17,
		EKB_J 				= 0x24,
		EKB_K 				= 0x25,
		EKB_L 				= 0x26,
		EKB_M 				= 0x32,
		EKB_N 				= 0x31,
		EKB_O 				= 0x18,
		EKB_P 				= 0x19,
		EKB_Q 				= 0x10,
		EKB_R 				= 0x13,
		EKB_S 				= 0x1f,
		EKB_T 				= 0x14,
		EKB_U 				= 0x16,
		EKB_V 				= 0x2f,
		EKB_W 				= 0x11,
		EKB_X 				= 0x2d,
		EKB_Y 				= 0x15,
		EKB_Z 				= 0x2c,
		EKB_Numpad0 		= 0x52,
		EKB_Numpad1 		= 0x4f,
		EKB_Numpad2 		= 0x50,
		EKB_Numpad3 		= 0x51,
		EKB_Numpad4 		= 0x4b,
		EKB_Numpad5 		= 0x4c,
		EKB_Numpad6 		= 0x4d,
		EKB_Numpad7 		= 0x47,
		EKB_Numpad8 		= 0x48,
		EKB_Numpad9 		= 0x49,
		EKB_NumpadAdd		= 0x4e,
		EKB_NumpadSubtract	= 0x4a,
		EKB_NumpadMultiply	= 0x37,
		EKB_NumpadDivide	= 0xb5,
		EKB_RangeEnd		= 0xff,

		// 鼠标信息
		MS_RangeBegin,
		MS_Left				= MS_RangeBegin,
		MS_Right,
		MS_Middle,
		MS_WheelUp,
		MS_WheelDown,
		MS_RangeEnd,
		
		// Gamepad按键信息
		GP_RangeBegin,
		GP_Left				= GP_RangeBegin,
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
		GP_RangeEnd = GP_Button15,

		// 按键总数量
		EKB_Num		= EKB_RangeEnd - EKB_RangeBegin + 1,
		MS_Num		= MS_RangeEnd - MS_RangeBegin + 1,
		GP_Num		= GP_RangeEnd - GP_RangeBegin + 1,
		NumButtons	= GP_RangeEnd,
		NoButton = 0xffffffff,
	};
}
