/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	MacKey
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
		EKB_RangeBegin		= 0x00,
		EKB_Escape			= 0x35,
		EKB_F1 				= 0x7a,
		EKB_F2 				= 0x78,
		EKB_F3 				= 0x63,
		EKB_F4 				= 0x76,
		EKB_F5 				= 0x60,
		EKB_F6 				= 0x61,
		EKB_F7 				= 0x62,
		EKB_F8 				= 0x64,
		EKB_F9 				= 0x65,
		EKB_F10 			= 0x6d,
		EKB_F11 			= 0x67,
		EKB_F12 			= 0x6f,
		EKB_1 				= 0x12,
		EKB_2 				= 0x13,
		EKB_3 				= 0x14,
		EKB_4 				= 0x15,
		EKB_5 				= 0x17,
		EKB_6 				= 0x16,
		EKB_7 				= 0x1a,
		EKB_8 				= 0x1c,
		EKB_9 				= 0x19,
		EKB_0 				= 0x1d,
		EKB_A				= 0x00,
		EKB_B				= 0x0b,
		EKB_C				= 0x08,
		EKB_D				= 0x02,
		EKB_E				= 0x0e,
		EKB_F				= 0x03,
		EKB_G				= 0x05,
		EKB_H				= 0x04,
		EKB_I				= 0x22,
		EKB_J				= 0x26,
		EKB_K				= 0x28,
		EKB_L				= 0x25,
		EKB_M				= 0x2e,
		EKB_N				= 0x2d,
		EKB_O				= 0x1f,
		EKB_P				= 0x23,
		EKB_Q				= 0x0c,
		EKB_R				= 0x0f,
		EKB_S				= 0x01,
		EKB_T				= 0x11,
		EKB_U				= 0x20,
		EKB_V				= 0x09,
		EKB_W				= 0x0d,
		EKB_X				= 0x07,
		EKB_Y				= 0x10,
		EKB_Z				= 0x06,
		EKB_Tab 			= 0x30,
		EKB_Return			= 0x24,
		EKB_Space			= 0x31,
		EKB_LeftShift		= 0x38,
		EKB_RightShift		= 0x3c,
		EKB_LeftControl		= 0x3b,
		EKB_RightControl	= 0x3e,
		EKB_LeftAlt			= 0x3a,
		EKB_RightAlt		= 0x3d,
		EKB_LeftMeta		= 0x37,
		EKB_RightMeta		= 0x36,
		EKB_Backspace		= 0x33,
		EKB_Left			= 0x7b,
		EKB_Right			= 0x7c,
		EKB_Up				= 0x7e,
		EKB_Down			= 0x7d,
		EKB_Home			= 0x73,
		EKB_End				= 0x77,
		EKB_Insert 			= 0x72,
		EKB_Delete 			= 0x75,
		EKB_PageUp 			= 0x74,
		EKB_PageDown		= 0x79,
		EKB_Enter			= 0x4c,
		EKB_Numpad1 		= 0x53,
		EKB_Numpad2 		= 0x54,
		EKB_Numpad3 		= 0x55,
		EKB_Numpad4 		= 0x56,
		EKB_Numpad5 		= 0x57,
		EKB_Numpad6 		= 0x58,
		EKB_Numpad7 		= 0x59,
		EKB_Numpad8 		= 0x5b,
		EKB_Numpad9 		= 0x5c,
		EKB_Numpad0 		= 0x52,
		EKB_NumpadAdd		= 0x45,
		EKB_NumpadSubtract	= 0x4e,
		EKB_NumpadMultiply	= 0x43,
		EKB_NumpadDivide	= 0x4b,
		EKB_RangeEnd		= 0xffff,

		MS_RangeBegin,
		MS_Left				= MS_RangeBegin,
		MS_Right,
		MS_Middle,
		MS_WheelUp,
		MS_WheelDown,
		MS_RangeEnd,

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
		GP_RangeEnd			= GP_Button15,

		EKB_Num		= EKB_RangeEnd - EKB_RangeBegin + 1,
		MS_Num		= MS_RangeEnd - MS_RangeBegin + 1,
		GP_Num		= GP_RangeEnd - GP_RangeBegin + 1,

		NumButtons	= GP_RangeEnd + 1,
		NoButton	= 0xffffffff
	};
}