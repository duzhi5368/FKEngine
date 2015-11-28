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
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../../include/Control/ControlInsideHeader.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	DI8InputSystem::DI8InputSystem()
		: mpDirectInput8	(NULL)
		, mpDI8Keyboard		(NULL)
		, mpDI8Mouse		(NULL)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	DI8InputSystem::~DI8InputSystem()
	{
		SafeRelease( mpDI8Keyboard );
		SafeRelease( mpDI8Mouse );
		SafeRelease( mpDirectInput8 );
	}
	//--------------------------------------------------------------------------------------------------------------
	//初始化输入系统
	void DI8InputSystem::Initialize()
	{
		//创建 DirectInput8 对象
		HRESULT result = DirectInput8Create( (HINSTANCE)GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8,
			(void**)&mpDirectInput8, NULL );
		if( FAILED( result ) )
			Except( Exception::ERR_INTERNAL_ERROR, "创建 DirectInput8 对象错误。" );
	}
	//--------------------------------------------------------------------------------------------------------------
	//初始化键盘输入设备
	void DI8InputSystem::InitializeKeyboard( HWND hWnd, bool bExclusive )
	{
		//创建键盘设备
		HRESULT result = mpDirectInput8->CreateDevice( GUID_SysKeyboard, &mpDI8Keyboard, NULL );
		if( FAILED( result ) )
			Except( Exception::ERR_INTERNAL_ERROR, (String)"创建 DirectInput8 键盘设备错误！"
			"\nDX 错误描述：" + DXGetErrorDescription9( result ) );

		//设置键盘数据格式
		result = mpDI8Keyboard->SetDataFormat( &c_dfDIKeyboard );
		if( FAILED( result ) )
			Except( Exception::ERR_INTERNAL_ERROR, (String)"设置 DirectInput8 键盘设备数据格式错误！"
			"\nDX 错误描述：" + DXGetErrorDescription9( result ) );

		//设置键盘协作等级
		DWORD flag = bExclusive ? ( DISCL_FOREGROUND | DISCL_EXCLUSIVE ) : ( DISCL_BACKGROUND | DISCL_NONEXCLUSIVE );

		result = mpDI8Keyboard->SetCooperativeLevel( hWnd, flag );
		if( FAILED( result ) )
			Except( Exception::ERR_INTERNAL_ERROR, (String)"设置 DirectInput8 键盘设备协作等级错误！"
			"\nDX 错误描述：" + DXGetErrorDescription9( result ) );

		//设置缓存模式键盘
		DIPROPDWORD dipdw;
		dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		dipdw.diph.dwObj        = 0;
		dipdw.diph.dwHow        = DIPH_DEVICE;
		dipdw.dwData            = DI8_KEY_BUF_SIZE;

		result = mpDI8Keyboard->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph );
		if( FAILED( result ) )
			Except( Exception::ERR_INTERNAL_ERROR, (String)"设置 DirectInput8 键盘设备数据缓存模式错误！"
			"\nDX 错误描述：" + DXGetErrorDescription9( result ) );

		//获取键盘
		mpDI8Keyboard->Acquire();

		mbEnableKeyboard = true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//初始化鼠标输入设备
	void DI8InputSystem::InitializeMouse( HWND hWnd, bool bExclusive )
	{
		HRESULT result = mpDirectInput8->CreateDevice( GUID_SysMouse, &mpDI8Mouse, NULL );
		if( FAILED( result ) )
			Except( Exception::ERR_INTERNAL_ERROR, (String)"创建 DirectInput8 鼠标设备错误！"
			"\nDX 错误描述：" + DXGetErrorDescription9( result ) );

		//设置数据格式
		result = mpDI8Mouse->SetDataFormat( &c_dfDIMouse2 );
		if( FAILED( result ) )
			Except( Exception::ERR_INTERNAL_ERROR, (String)"设置 DirectInput8 鼠标设备数据格式错误！"
			"\nDX 错误描述：" + DXGetErrorDescription9( result ) );

		//设置键盘协作等级
		DWORD flag = bExclusive ? ( DISCL_FOREGROUND | DISCL_EXCLUSIVE ) : ( DISCL_BACKGROUND | DISCL_NONEXCLUSIVE );

		result = mpDI8Mouse->SetCooperativeLevel( hWnd, flag );
		if( FAILED( result ) )
			Except( Exception::ERR_INTERNAL_ERROR, (String)"设置 DirectInput8 鼠标设备协作等级错误！"
			"\nDX 错误描述：" + DXGetErrorDescription9( result ) );

		//设置缓存模式鼠标
		DIPROPDWORD dipdw;
		dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		dipdw.diph.dwObj        = 0;
		dipdw.diph.dwHow        = DIPH_DEVICE;
		dipdw.dwData			= DI8_MOUSE_BUF_SIZE;

		result = mpDI8Mouse->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph );
		if( FAILED( result ) )
			Except( Exception::ERR_INTERNAL_ERROR, (String)"设置 DirectInput8 鼠标设备数据缓存模式错误！"
			"\nDX 错误描述：" + DXGetErrorDescription9( result ) );

		//获取鼠标
		mpDI8Mouse->Acquire();

		mbEnableMouse = true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//更新键盘输入
	void DI8InputSystem::UpdateKeyboard()
	{
		if( !mbEnableKeyboard )
			return;

		static DIDEVICEOBJECTDATA didod[DI8_KEY_BUF_SIZE];
		DWORD dwElements = DI8_KEY_BUF_SIZE;

		//清空上一帧事件变量
		memset( mbKeyDown, 0, sizeof(bool)*KEY_BUF_SIZE*2 );

Acquire:
		HRESULT result = mpDI8Keyboard->GetDeviceData( sizeof(DIDEVICEOBJECTDATA), didod, &dwElements, 0 );
		if( FAILED( result ) )
		{
			result = mpDI8Keyboard->Acquire();
			while( result == DIERR_INPUTLOST )
				result = mpDI8Keyboard->Acquire();

			if( SUCCEEDED( result ) )
				goto Acquire;

			Except( Exception::ERR_INTERNAL_ERROR, (String)"获取 DirectInput8 键盘设备出错！"
				"\nDX 错误描述：" + DXGetErrorDescription9( result ) );
		}

		//处理所有缓存数据
		for( DWORD i=0; i<dwElements; ++i )
		{
			//键盘按键编号
			DWORD key = didod[i].dwOfs;

			//如果按下了某键
			if( didod[i].dwData & 0x80 )
			{
				mbKeyState[key] = true;
				mbKeyDown[key] = true;
				mbKeyUp[key] = false;
			}
			//如果放开了某键
			else
			{
				mbKeyState[key] = false;
				mbKeyUp[key] = true;
				mbKeyDown[key] = false;
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//更新鼠标输入
	void DI8InputSystem::UpdateMouse()
	{
		if( !mbEnableKeyboard )
			return;

		static DIMOUSESTATE2 mouseState;
		static DIDEVICEOBJECTDATA didod[DI8_MOUSE_BUF_SIZE];
		DWORD dwElements = DI8_MOUSE_BUF_SIZE;
		DWORD dwTime = 0;
		bool bIsPressed = false;

		//清空上一帧事件变量
		memset( mbMouseDown, 0, sizeof(bool)*9 );

		mMouseRelX = 0;
		mMouseRelY = 0;
		mMouseRelZ = 0;

Acquire:
		HRESULT result = mpDI8Mouse->GetDeviceData( sizeof(DIDEVICEOBJECTDATA), didod, &dwElements, 0 );
		if( FAILED( result ) )
		{
			result = mpDI8Mouse->Acquire();
			while( result == DIERR_INPUTLOST )
				result = mpDI8Mouse->Acquire();

			if( SUCCEEDED( result ) )
				goto Acquire;

			Except( Exception::ERR_INTERNAL_ERROR, (String)"获取 DirectInput8 鼠标设备出错！"
				"\nDX 错误描述：" + DXGetErrorDescription9( result ) );
		}

		//处理所有缓存数据
		for( DWORD i=0; i<dwElements; ++i )
		{
			//消息产生时间
			dwTime = didod[i].dwTimeStamp;

			//是否为按键消息
			bIsPressed = ( didod[i].dwData & 0x80 ) ? true : false;

			switch( didod[i].dwOfs )
			{
				//左键消息
			case DIMOFS_BUTTON0:
				mbMouseState[0] = bIsPressed;
				mbMouseDown[0] = bIsPressed;
				mbMouseUp[0] = !bIsPressed;

				//如果按下
				if( bIsPressed )
				{
					//如果这次按键的时间与上次按键的时间间隔小于双击时间
					if( dwTime - mMouseLastDownTime[0] < mMouseDBClickTime[0] )
					{
						mbMouseDBClick[0] = true;
					}
					else
					{
						mMouseLastDownTime[0] = dwTime;
					}
				}
				break;

				//右键消息
			case DIMOFS_BUTTON1:
				mbMouseState[1] = bIsPressed;
				mbMouseDown[1] = bIsPressed;
				mbMouseUp[1] = !bIsPressed;

				//如果按下
				if( bIsPressed )
				{
					//如果这次按键的时间与上次按键的时间间隔小于双击时间
					if( dwTime - mMouseLastDownTime[1] < mMouseDBClickTime[1] )
					{
						mbMouseDBClick[1] = true;
					}
					else
					{
						mMouseLastDownTime[1] = dwTime;
					}
				}
				break;

				//中键消息
			case DIMOFS_BUTTON2:
				mbMouseState[2] = bIsPressed;
				mbMouseDown[2] = bIsPressed;
				mbMouseUp[2] = !bIsPressed;

				//如果按下
				if( bIsPressed )
				{
					//如果这次按键的时间与上次按键的时间间隔小于双击时间
					if( dwTime - mMouseLastDownTime[2] < mMouseDBClickTime[2] )
					{
						mbMouseDBClick[2] = true;
					}
					else
					{
						mMouseLastDownTime[2] = dwTime;
					}
				}
				break;

				//坐标移动消息
			case DIMOFS_X:
				mMouseRelX += (int)( (long)didod[i].dwData * mMouseSpeed / 65 );
				break;
			case DIMOFS_Y:
				mMouseRelY += (int)( (long)didod[i].dwData * mMouseSpeed / 65 );
				break;
			case DIMOFS_Z:
				mMouseRelZ += (int)( (long)didod[i].dwData * mMouseSpeed / 65 );
				break;

			default:
				break;
			}
		}

		//计算鼠标位置
		mMouseX += mMouseRelX;
		mMouseY += mMouseRelY;
		mMouseZ += mMouseRelZ;

		FixPosByMouseRect( &mMouseX, &mMouseY, &mMouseZ );
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
