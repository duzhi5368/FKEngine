/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	WinInput
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "../include/Input.h"
#include "../include/Platform.h"
#include "../include/WinUtility.h"
#include "../include/WinKey.h"
#include "../../depend/boost/array.hpp"
#include "../../depend/boost/shared_ptr.hpp"
#include <cwchar>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <queue>
//--------------------------------------------------------------------
#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif
#include "../../depend/dinput/dinput.h"
//--------------------------------------------------------------------
namespace 
{
	boost::array< bool, FK2DEngine::NumButtons >	ArrKeys;
}
//--------------------------------------------------------------------
struct FK2DEngine::CInput::SImpl
{
	struct SEventInfo
	{
		enum{ EKeyUp, EKeyDown }	m_eAction;
		unsigned int				m_unID;
	};

	typedef boost::shared_ptr< IDirectInputDevice8 >		Device;
	typedef std::vector< SEventInfo >						Events;
public:
	CTextInput*							m_pTextInput;
	boost::shared_ptr< IDirectInput8 >	m_pInput;
	HWND								m_hWnd;
	Device								m_pKeyBoard;
	Device								m_pMouse;
	std::vector< Device >				m_vecGamepads;
	double								m_dMouseX;
	double								m_dMouseY;
	double								m_dMouseFactorX;
	double								m_dMouseFactorY;
	bool								m_bSwapMouse;
	Events								m_vecEvents;			// 每次Update中间间隔时保存的消息列表	
public:
	static const unsigned int			INPUT_BUFFER_SIZE	= 32;
	static const int					STICK_RANGE			= 500;
	static const int					STICK_THRESHOLD		= 250;
public:
	SImpl()
		: m_pTextInput( NULL )
	{

	}

	void ForceKey( unsigned int p_unID, bool p_bDown, bool p_bCollectEvent )
	{
		ArrKeys.at( p_unID ) = p_bDown;

		if( !p_bCollectEvent )
		{
			return;
		}

		SEventInfo newEvent;
		if( p_bDown )
		{
			newEvent.m_eAction	= SEventInfo::EKeyDown;
		}
		else
		{
			newEvent.m_eAction	= SEventInfo::EKeyUp;
		}

		newEvent.m_unID	= p_unID;
		m_vecEvents.push_back( newEvent );
	}

	void SetKey( unsigned int p_unID, bool p_bDown, bool p_bCollectEvent )
	{
		if( ArrKeys.at( p_unID ) != p_bDown )
		{
			ForceKey( p_unID, p_bDown, p_bCollectEvent );
		}
	}

	static void ThrowError( const char* p_szAction, HRESULT hr )
	{
		std::ostringstream stream;
		stream << "当进行 " << p_szAction << " 操作时，DirectInput 出现了错误 "
			<< std::hex << std::setw( 8 ) << hr;
		throw std::runtime_error( stream.str() );
	}

	static inline HRESULT Check( const char* p_szAction, HRESULT hr )
	{
		if( FAILED( hr ) )
		{
			ThrowError( p_szAction, hr );
		}
		return hr;
	}

	static BOOL CALLBACK AxisCallback( LPCDIDEVICEOBJECTINSTANCE p_lpInstance, LPVOID p_lpUserData )
	{
		IDirectInputDevice8* pDev = static_cast< IDirectInputDevice8* >( p_lpUserData );

		DIPROPRANGE range;
		range.diph.dwSize		= sizeof( DIPROPRANGE );
		range.diph.dwHeaderSize	= sizeof( DIPROPHEADER );
		range.diph.dwHow		= DIPH_BYID;
		range.diph.dwObj		= p_lpInstance->dwType;
		range.lMin				= -STICK_RANGE;
		range.lMax				= +STICK_RANGE;
		pDev->SetProperty( DIPROP_RANGE, &range.diph );

		return DIENUM_CONTINUE;
	}

	static BOOL CALLBACK GamepadCallback( LPCDIDEVICEINSTANCE p_lpDevice, LPVOID p_lpUserData )
	{
		SImpl* pImpl = static_cast< SImpl* >( p_lpUserData );

		IDirectInputDevice8* pGamepad = NULL;
		if( FAILED( pImpl->m_pInput->CreateDevice( p_lpDevice->guidInstance, &pGamepad, 0 ) ) )
		{
			return DIENUM_CONTINUE;
		}

		if( FAILED( pGamepad->SetDataFormat( &c_dfDIJoystick ) ) ||
			FAILED( pGamepad->SetCooperativeLevel( pImpl->m_hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND ) ) ||
			FAILED( pGamepad->EnumObjects( AxisCallback, pGamepad, DIDFT_AXIS ) ) )
		{
			pGamepad->Release();
			return DIENUM_CONTINUE;
		}

		pImpl->m_vecGamepads.push_back( Win::ShareComPtr( pGamepad ) );
		return DIENUM_CONTINUE;
	}

	void UpdateMousePos()
	{
		POINT pos;
		if( !::GetCursorPos( &pos ) )
		{
			return;
		}

		Win::Check( ::ScreenToClient( m_hWnd, &pos ) );

		m_dMouseX	= pos.x;
		m_dMouseY	= pos.y;
	}

	void UpdateKeys( bool p_bCollectEvents )
	{
		DIDEVICEOBJECTDATA		data[ INPUT_BUFFER_SIZE ];
		DWORD					dwInOut;
		HRESULT					hr;

		RECT rect;
		::GetClientRect( m_hWnd, &rect );
		bool bIgnoreClicks = ( m_dMouseX < 0 ) ||
			( m_dMouseX > rect.right ) ||
			( m_dMouseY < 0 ) ||
			( m_dMouseY > rect.bottom );

		dwInOut = INPUT_BUFFER_SIZE;
		hr = m_pMouse->GetDeviceData( sizeof( data[0] ), data, &dwInOut, 0 );
		switch( hr )
		{
		case DI_OK:
		case DI_BUFFEROVERFLOW:
			{
				for( unsigned int i = 0; i < dwInOut; ++i )
				{
					bool bDown = ( data[i].dwData & 0x80 ) != 0 && ! bIgnoreClicks;

					switch( data[i].dwOfs )
					{
					case DIMOFS_BUTTON0:
						{
							unsigned int unID = m_bSwapMouse ? MS_Right : MS_Left;
							SetKey( unID, bDown, p_bCollectEvents );
							break;
						}
					case DIMOFS_BUTTON1:
						{
							unsigned int unID = m_bSwapMouse ? MS_Left : MS_Right;
							SetKey( unID, bDown, p_bCollectEvents );
							break;
						}
					case DIMOFS_BUTTON2:
						{
							SetKey( MS_Middle, bDown, p_bCollectEvents );
							break;
						}
					case DIMOFS_Z:
						{
							if( !p_bCollectEvents || data[i].dwData == 0 )
							{
								break;
							}

							SEventInfo Event;
							Event.m_eAction	= SEventInfo::EKeyDown;
							if( int( data[i].dwData ) < 0 )
							{
								Event.m_unID = MS_WheelDown;
							}
							else
							{
								Event.m_unID = MS_WheelUp;
							}

							m_vecEvents.push_back( Event );
							Event.m_eAction = SEventInfo::EKeyUp;
							m_vecEvents.push_back( Event );
							break;
						}
					}
				}
				break;
			}
		case DIERR_NOTACQUIRED:
		case DIERR_INPUTLOST:
			{
				for( unsigned int unID = MS_RangeBegin; unID < MS_RangeEnd; ++unID )
				{
					SetKey( unID, false, p_bCollectEvents );
				}
				m_pMouse->Acquire();
				break;
			}
		}

		dwInOut = INPUT_BUFFER_SIZE;
		hr = m_pKeyBoard->GetDeviceData( sizeof( data[0] ), data, &dwInOut, 0 );
		switch( hr )
		{
		case DI_OK:
		case DI_BUFFEROVERFLOW:
			{
				for( unsigned int i = 0; i < dwInOut; ++i )
				{
					ForceKey( data[i].dwOfs, ( data[i].dwData & 0x80 ) != 0, p_bCollectEvents );
				}
				break;
			}
		case DIERR_NOTACQUIRED:
		case DIERR_INPUTLOST:
			{
				for( unsigned int unID = EKB_RangeBegin; unID < EKB_RangeEnd; ++unID )
				{
					SetKey( unID, false, p_bCollectEvents );
				}
				m_pKeyBoard->Acquire();
				break;
			}
		}

		boost::array< bool, GP_Num >		ArrGPBuffers;
		ArrGPBuffers.assign( false );
		for( unsigned int unGp = 0; unGp < m_vecGamepads.size(); ++unGp )
		{
			m_vecGamepads[ unGp ]->Poll();

			DIJOYSTATE joy;
			hr = m_vecGamepads[ unGp ]->GetDeviceState( sizeof( joy ), &joy );
			switch( hr )
			{
			case DI_OK:
				{
					if( joy.lX < -STICK_THRESHOLD )
					{
						ArrGPBuffers[ GP_Left - GP_RangeBegin ]	= true;
					}
					else if( joy.lX > STICK_THRESHOLD )
					{
						ArrGPBuffers[ GP_Right - GP_RangeBegin ] = true;
					}

					if( joy.lY < -STICK_THRESHOLD )
					{
						ArrGPBuffers[ GP_Up - GP_RangeBegin ] = true;
					}
					else
					{
						ArrGPBuffers[ GP_Down - GP_RangeBegin ] = true;
					}

					for( unsigned int unID = GP_Button0; unID < GP_RangeEnd; ++unID )
					{
						if( joy.rgbButtons[unID - GP_Button0] )
						{
							ArrGPBuffers[unID - GP_RangeBegin] = true;
						}
					}
					break;
				}
			case DIERR_NOTACQUIRED:
			case DIERR_INPUTLOST:
				{
					m_vecGamepads[ unGp ]->Acquire();
					break;
				}
			}
		}

		for( unsigned int unID = GP_RangeBegin; unID < GP_RangeEnd; ++unID )
		{
			SetKey( unID, ArrGPBuffers[ unID - GP_RangeBegin ], p_bCollectEvents );
		}
	}
};
//--------------------------------------------------------------------
FK2DEngine::CInput::CInput( HWND p_hWnd )
	: m_pImpl( new SImpl )
{
	m_pImpl->m_hWnd = p_hWnd;
	m_pImpl->m_dMouseFactorX = 1.2;
	m_pImpl->m_dMouseFactorY = 1.2;

	IDirectInput8* pInput = NULL;
	SImpl::Check( "创建Dinput设备对象", ::DirectInput8Create( Win::Instance(),
		DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast< void** >( &pInput ), 0 ));
	m_pImpl->m_pInput = Win::ShareComPtr( pInput );

	DIPROPDWORD BufferSize;
	BufferSize.diph.dwSize			= sizeof( DIPROPDWORD );
	BufferSize.diph.dwHeaderSize	= sizeof( DIPROPHEADER );
	BufferSize.diph.dwHow			= DIPH_DEVICE;
	BufferSize.diph.dwObj			= 0;
	BufferSize.dwData				= SImpl::INPUT_BUFFER_SIZE;

	// 键盘
	IDirectInputDevice8* KeyBoard = NULL;
	SImpl::Check( "创建键盘设备对象", m_pImpl->m_pInput->CreateDevice( GUID_SysKeyboard,
		&KeyBoard, 0 ) );
	m_pImpl->m_pKeyBoard = Win::ShareComPtr( KeyBoard );

	SImpl::Check( "设置键盘数据格式", KeyBoard->SetDataFormat( &c_dfDIKeyboard ) );
	SImpl::Check( "设置键盘协调层级", KeyBoard->SetCooperativeLevel( p_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE ) );
	SImpl::Check( "设置键盘缓冲", KeyBoard->SetProperty( DIPROP_BUFFERSIZE, &BufferSize.diph ) );

	KeyBoard->Acquire();

	// 鼠标
	IDirectInputDevice8* Mouse;
	SImpl::Check( "创建鼠标对象", m_pImpl->m_pInput->CreateDevice( GUID_SysMouse,
		&Mouse, 0 ) );
	m_pImpl->m_pMouse = Win::ShareComPtr( Mouse );

	SImpl::Check( "设置鼠标数据格式", Mouse->SetDataFormat( &c_dfDIMouse ) );
	SImpl::Check( "设置鼠标协调层级", Mouse->SetCooperativeLevel( p_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE ) );
	SImpl::Check( "设置鼠标缓冲", Mouse->SetProperty( DIPROP_BUFFERSIZE, &BufferSize.diph ) );

	Mouse->Acquire();

	m_pImpl->m_bSwapMouse = ::GetSystemMetrics( SM_SWAPBUTTON ) != 0;

	// 设置所有Gamepads
	m_pImpl->m_pInput->EnumDevices( DI8DEVCLASS_GAMECTRL, SImpl::GamepadCallback,
		m_pImpl.get(), DIEDFL_ATTACHEDONLY );

	// 进入一个可用的默认状态
	m_pImpl->m_dMouseX = 0;
	m_pImpl->m_dMouseY = 0;
	m_pImpl->UpdateMousePos();
	ArrKeys.assign( false );
}
//--------------------------------------------------------------------
FK2DEngine::CInput::~CInput()
{

}
//--------------------------------------------------------------------
FK2DEngine::CKey FK2DEngine::CInput::CharToID( wchar_t p_Char )
{
	SHORT sKey = ::VkKeyScan( tolower( p_Char ) );

	if( HIBYTE( sKey ) == static_cast< unsigned char >( -1 ) &&
		LOBYTE( sKey ) == static_cast< unsigned char >( -1 ) )
	{
		return NoButton;
	}

	if( HIBYTE( sKey ) != 0 )
	{
		return NoButton;
	}

	return CKey( ::MapVirtualKey( sKey, 0 ) );
}
//--------------------------------------------------------------------
wchar_t FK2DEngine::CInput::IDToChar( CKey p_Key )
{
	if( p_Key.ID() > 255 )
	{
		return 0;
	}

	if( p_Key.ID() == EKB_Space )
	{
		return L' ';
	}

	wchar_t Buf[3];
	if( ::GetKeyNameText( p_Key.ID() << 16, Buf, 3 ) == 1 )
	{
		return towlower( Buf[0] );
	}
	return 0;
}
//--------------------------------------------------------------------
wchar_t	FK2DEngine::CInput::IDToChar( unsigned int p_unKey )
{
	if( p_unKey > 255 )
	{
		return 0;
	}

	if( p_unKey == EKB_Space )
	{
		return L' ';
	}

	wchar_t Buf[3];
	if( ::GetKeyNameText( p_unKey << 16, Buf, 3 ) == 1 )
	{
		return towlower( Buf[0] );
	}
	return 0;
}
//--------------------------------------------------------------------
bool FK2DEngine::CInput::Down( CKey p_Key )const
{
	if(( p_Key == NoButton ) || ( p_Key.ID() >= NumButtons ))
	{
		return false;
	}
	return ArrKeys.at( p_Key.ID() );
}
//--------------------------------------------------------------------
double FK2DEngine::CInput::MouseX() const
{
	return ( m_pImpl->m_dMouseX * m_pImpl->m_dMouseFactorX );
}
//--------------------------------------------------------------------
double FK2DEngine::CInput::MouseY() const
{
	return ( m_pImpl->m_dMouseY * m_pImpl->m_dMouseFactorY );
}
//--------------------------------------------------------------------
void FK2DEngine::CInput::SetMousePosition( double p_dX, double p_dY )
{
	POINT Pos = { static_cast< LONG >( p_dX / m_pImpl->m_dMouseFactorX ), 
		static_cast< LONG >( p_dY / m_pImpl->m_dMouseFactorY ) };
	::ClientToScreen( m_pImpl->m_hWnd, &Pos );
	::SetCursorPos( Pos.x, Pos.y );
	m_pImpl->UpdateMousePos();
}
//--------------------------------------------------------------------
void FK2DEngine::CInput::SetMouseFactors( double p_dFactorX, double p_dFactorY )
{
	m_pImpl->m_dMouseFactorX = p_dFactorX;
	m_pImpl->m_dMouseFactorY = p_dFactorY;
}
//--------------------------------------------------------------------
const FK2DEngine::VEC_Touches& FK2DEngine::CInput::CurrentTouches() const
{
	static FK2DEngine::VEC_Touches None;
	return None;
}
//--------------------------------------------------------------------
double FK2DEngine::CInput::AccelerometerX() const
{
	return 0.0;
}
//--------------------------------------------------------------------
double FK2DEngine::CInput::AccelerometerY() const
{
	return 0.0;
}
//--------------------------------------------------------------------
double FK2DEngine::CInput::AccelerometerZ() const
{
	return 0.0;
}
//--------------------------------------------------------------------
void FK2DEngine::CInput::Update( float p_fDelta )
{
	m_pImpl->UpdateMousePos();
	m_pImpl->UpdateKeys( true );

	// 清空老的消息队列
	SImpl::Events events;
	events.swap( m_pImpl->m_vecEvents );

	// 分发上一帧到本帧间的消息
	for( unsigned int i = 0; i < events.size(); ++i )
	{
		if( events[i].m_eAction == SImpl::SEventInfo::EKeyDown )
		{
			if( OnButtonDown )
			{
				// 回调给回调函数
				OnButtonDown( CKey( events[i].m_unID ) );
			}
		}
		else
		{
			if( OnButtonUp )
			{
				// 回调给回调函数
				OnButtonUp( CKey( events[i].m_unID ) );
			}
		}
	}
}
//--------------------------------------------------------------------
FK2DEngine::CTextInput* FK2DEngine::CInput::TextInput() const
{
	return m_pImpl->m_pTextInput;
}
//--------------------------------------------------------------------
void FK2DEngine::CInput::SetTextInput( CTextInput* p_pTextInput )
{
	m_pImpl->m_pTextInput = p_pTextInput;
}
//--------------------------------------------------------------------