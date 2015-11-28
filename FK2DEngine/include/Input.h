/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Input
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "InsidePreInclude.h"
#include "Platform.h"
#include "../../depend/boost/function.hpp"
#include "../../depend/boost/scoped_ptr.hpp"
#include <vector>
//--------------------------------------------------------------------
#ifdef FK_WINDOWS
	#include "WinKey.h"
		#ifndef NOMINMAX
		#define NOMINMAX
		#endif
	#include <Windows.h>
#endif
//--------------------------------------------------------------------
#ifdef FK_MAC
	#include "MacKey.h"
#endif
//--------------------------------------------------------------------
#ifdef FK_X
	#include <X11/Xlib.h>
	#include <X11/Xutil.h>
	#include <X11/keysym.h>
	#include "XKey.h"
#endif
//--------------------------------------------------------------------

namespace FK2DEngine
{
	//--------------------------------------------------------------------
	class CKey
	{
	private:
		unsigned int		m_unID;
	public:
		explicit CKey( unsigned int p_unID )
			: m_unID( p_unID )
		{

		}

		unsigned int ID() const
		{
			return m_unID;
		}

		CKey()
			: m_unID( NoButton )
		{

		}

		CKey( ENUM_KeyName p_eName )
			: m_unID( p_eName )
		{

		}
	};

	inline bool operator == ( CKey p_Left, CKey p_Right )
	{
		return ( p_Left.ID() == p_Right.ID() );
	}
	inline bool operator != ( CKey p_Left, CKey p_Right )
	{
		return !( p_Left == p_Right );
	}
	inline bool operator < ( CKey p_Left, CKey p_Right )
	{
		return ( p_Left.ID() < p_Right.ID() );
	}

	//--------------------------------------------------------------------
	struct STouch
	{
		void*		m_pID;
		// 触摸屏的X,Y坐标
		float		m_fX;
		float		m_fY;
	};

	typedef std::vector< STouch >		VEC_Touches;
	//--------------------------------------------------------------------
	class CInput
	{
	private:
		struct SImpl;
		boost::scoped_ptr< SImpl >			m_pImpl;
	public:
		#ifdef FK_WINDOWS
			CInput( HWND p_hWnd );
		#endif

		#ifdef FK_MAC
			#ifdef FK_IPHONE
				CInput( void* p_pView, float p_fUpdateInterval );
				void FeedTouchEvent( int p_nType, void* p_pTouches );
			#else
				CInput( void* p_pWindow );
				bool FeedNSEvent( void* p_pEvent );
			#endif
		#endif

		#ifdef FK_X
			CInput( ::Display* p_pDisplay, ::Window p_Wnd );
			bool FeedXEvent( ::XEvent& p_Event );
		#endif
	public:
		~CInput();

		static wchar_t	IDToChar( CKey p_Key );
		static CKey		CharToID( wchar_t p_Char );
		static wchar_t	IDToChar( unsigned int p_unKey );

		//! 判断一个键是否按下( 要求每帧进行检测 )
		bool Down( CKey p_Key ) const;

		//! 返回鼠标的位置
		double MouseX() const;
		double MouseY() const;

		//! 设置鼠标位置（绝对像素坐标）
		void SetMousePosition( double p_dX, double p_dY );
		//! 设置鼠标位置（百分比）
		void SetMouseFactors( double p_dFactorX, double p_dFactorY );

		//! 当前触摸记录
		const VEC_Touches& CurrentTouches() const;

		//! 移动加速度
		double AccelerometerX() const;
		double AccelerometerY() const;
		double AccelerometerZ() const;

		//! 每帧的更新
		void Update( float p_fDelta );

		//! 返回当前被激活的 TextInput 实例，若没有，则返回 NULL
		CTextInput* TextInput() const;
		//! 设置当前被激活的 TextInput 实例，若参数为 NULL，则表示清除
		void SetTextInput( CTextInput* p_pTextInput );
	
		//! 每帧的回调,在Update中被调用
		boost::function< void( CKey ) >  OnButtonDown;
		boost::function< void( CKey ) >	 OnButtonUp;

		boost::function< void( STouch ) >	OnTouchBegin;
		boost::function< void( STouch ) >	OnTouchEnded;
		boost::function< void( STouch ) >	OnTouchMoved;
	};
}