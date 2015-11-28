/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	FKWindow
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
#include "Input.h"
#include "../../depend/boost/scoped_ptr.hpp"
#include "../../depend/boost/shared_ptr.hpp"
#include "../../depend/boost/function.hpp"
#include <string>
//--------------------------------------------------------------------
#ifdef FK_WINDOWS
	#ifndef NOMINMAX
		#define NOMINMAX
	#endif
#include <windows.h>
#endif
//--------------------------------------------------------------------
namespace FK2DEngine
{
	class CFKWindow
	{
		struct			SImpl;
	private:
		boost::scoped_ptr< SImpl >		m_pImpl;
	public:
		CFKWindow( unsigned int p_unWidth, unsigned int p_unHeight, bool p_bFullScreen, double p_dUpdateInterval = 16.666666 );
		virtual ~CFKWindow();
		//--------------------------------------------------------------------
		// 核心逻辑函数
		//--------------------------------------------------------------------
	public:
		double UpdateInterval() const;

		void Show();
		bool Close();
		//--------------------------------------------------------------------
		// 功能性函数
		//--------------------------------------------------------------------
	public:
		std::wstring Caption() const;
		void SetCaption( const std::wstring& p_szCaption );
		// 设置静态鼠标
		void SetNewCursor( const std::wstring& p_lpCursorName );
		// 截屏函数
		void PrintScreen( const std::wstring& p_lpDestName );
		// 获取时间
		unsigned int GetTime() const;
		// 获取渲染窗口宽度
		unsigned int GetScreenWidth() const;
		// 获取渲染窗口高度
		unsigned int GetScreenHeight() const;
		// 设置背景颜色
		void SetBackgroudColor( CColor p_Color );
		//--------------------------------------------------------------------
		// 功能性函数
		//--------------------------------------------------------------------
	public:
		virtual void Update( float p_fDelta )
		{

		}
		virtual void FKDraw()
		{
			
		}
		virtual bool NeedsReDraw() const 
		{
			return true;
		}
		virtual bool NeedsCursor() const
		{
			return true;
		}
		virtual void LoseFocus() const
		{

		}
		virtual void ReleaseMemory() const
		{

		}
		virtual void KeyDown( FK2DEngine::CKey )
		{

		}
		virtual void KeyUp( FK2DEngine::CKey )
		{

		}
		//! 关闭销毁
		//@return 返回false则不销毁，返回true则进行销毁
		virtual bool OnClose()
		{
			return true;
		}
#ifndef SWIG

		const CGraphics&	Graphics() const;
		CGraphics&			Graphics();
		const CInput&		Input() const;
		CInput&				Input();
		const CAudio&		Audio() const;
		CAudio&				Audio();

		#ifdef FK_WINDOWS
			HWND Handle() const;
			virtual LRESULT HandleMessage( UINT p_unMessage, WPARAM p_wParam, LPARAM p_lParam );
		#endif

		#ifdef FK_UNIX
			typedef boost::shared_ptr< boost::function< void() > >	SharedContext;
			SharedContext CreateSharedContext();
		#endif

#ifdef FK_IPHONE
	virtual void TouchBegin( STouch p_Touch )
	{

	}
	virtual void TouchMoved( STouch p_Touch )
	{

	}
	virtual void TouchEnded( STouch p_Touch )
	{

	}
#endif
#endif

	};
}