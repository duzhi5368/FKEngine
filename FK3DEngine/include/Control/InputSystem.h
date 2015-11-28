/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	InputSystem
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	输入控制系统
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//鼠标移动范围结构
	struct MouseRect
	{
		int Left;
		int Right;
		int Top;
		int Bottom;
		int WheelMin;
		int WheelMax;

		FKForceInline MouseRect& operator = ( const MouseRect& other )
		{
			Left		= other.Left;
			Right		= other.Right;
			Top			= other.Top;
			Bottom		= other.Bottom;
			WheelMin	= other.WheelMin;
			WheelMax	= other.WheelMax;
			return *this;
		};
	};
	//--------------------------------------------------------------------------------------------------------------
	#define KEY_BUF_SIZE		256			//键盘状态缓存大小
	//--------------------------------------------------------------------------------------------------------------
	//输入控制系统
	class InputSystem : public Singleton< InputSystem >
	{
	public:
		bool	mbEnableKeyboard;			//标志键盘输入是否开启
		bool	mbEnableMouse;				//标志鼠标输入是否开启

		bool	mbKeyState[KEY_BUF_SIZE];	//键盘按键状态
		bool	mbKeyDown[KEY_BUF_SIZE];	//键盘按键事件
		bool	mbKeyUp[KEY_BUF_SIZE];		//键盘放键事件

		bool	mbMouseState[3];			//鼠标按键状态
		bool	mbMouseDown[3];				//鼠标按键事件
		bool	mbMouseUp[3];				//鼠标放键事件
		bool	mbMouseDBClick[3];			//鼠标双击事件

		DWORD	mMouseLastDownTime[3];		//鼠标上次的按键时间（毫秒）
		DWORD	mMouseDBClickTime[3];		//鼠标双击判断时间（毫秒）

		MouseRect	mMouseMoveRect;			//鼠标移动区域
		LONG		mMouseSpeed;			//鼠标移动速度

		//鼠标当前坐标
		int		mMouseX;
		int		mMouseY;
		int		mMouseZ;

		//鼠标相对上一帧移动坐标
		int		mMouseRelX;
		int		mMouseRelY;
		int		mMouseRelZ;

	public:
		InputSystem();
		virtual ~InputSystem() = 0;

		//初始化输入系统
		virtual void Initialize() = 0;

		//初始化键盘输入设备
		virtual void InitializeKeyboard( HWND hWnd, bool bExclusive = false ) = 0;

		//初始化鼠标输入设备
		virtual void InitializeMouse( HWND hWnd, bool bExclusive = false ) = 0;

		//打开键盘输入
		void FKFastcall EnableKeyboard( bool bEnable );

		//打开鼠标输入
		void FKFastcall EnableMouse( bool bEnable );

		//设置鼠标双击判定间隔时间（毫秒）
		void FKFastcall SetMouseDBClickTime( DWORD dwLeftTime, DWORD dwRightTime, DWORD dwMidTime );

		//设置鼠标移动速度
		void FKFastcall SetMouseSpeed( LONG nMoveSpeed = 100 );

		//设置鼠标位置
		void FKFastcall SetMousePos( int x, int y, int z );

		//设置鼠标移动区域
		void FKFastcall SetMouseRect( MouseRect* pMouseRect );

		//根据鼠标移动区域修正坐标
		void FKFastcall FixPosByMouseRect( int* pX, int* pY, int* pZ );

		//更新键盘输入
		virtual void UpdateKeyboard() = 0;

		//更新鼠标输入
		virtual void UpdateMouse() = 0;
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
