/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	DI8InputSystem
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	DirectInput8 输入类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------

	#define DI8_KEY_BUF_SIZE		8	//键盘输入缓存大小
	#define DI8_MOUSE_BUF_SIZE		16	//鼠标输入缓存大小

	//--------------------------------------------------------------------------------------------------------------
	//DirectInput8 输入类
	class DI8InputSystem : public InputSystem
	{
	protected:
		IDirectInput8*			mpDirectInput8;
		IDirectInputDevice8*	mpDI8Keyboard;
		IDirectInputDevice8*	mpDI8Mouse;

	public:
		DI8InputSystem();
		~DI8InputSystem();

		//初始化输入系统
		void Initialize();

		//初始化键盘输入设备
		void InitializeKeyboard( HWND hWnd, bool bExclusive = false );

		//初始化鼠标输入设备
		void InitializeMouse( HWND hWnd, bool bExclusive = false );

		//更新键盘输入
		void UpdateKeyboard();

		//更新鼠标输入
		void UpdateMouse();
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
