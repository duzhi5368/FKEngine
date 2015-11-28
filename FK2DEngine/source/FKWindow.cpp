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

#include "../include/FKWindow.h"
#include "../Include/System.h"
#include "../include/WinUtility.h"
#include "../include/Timing.h"
#include "../include/Graphics.h"
#include "../include/Input.h"
#include "../include/TextInput.h"
#include "../include/GraphicsCommon.h"
#include "../Include/UIInputManager.h"
#include "../Include/UIManager.h"
#include "../../depend/boost/bind.hpp"
#include "../include/Audio.h"
#include "../include/MsgListenerManager.h"
#include "../include/Singleton.h"
#include "../Include/UIControl.h"
#include "../Include/LogManager.h"
#include "../include/StringConversion.h"
#include "../include/Directories.h"
#include "../include/AnimationCursorManager.h"
#include <cassert>
#include <stdexcept>
#include <vector>
//--------------------------------------------------------------------
// 帧间隔敏感时间
#define MAX_FRAME_INTERVAL_TIME		1000 / 24
//--------------------------------------------------------------------
#pragma warning ( disable : 4244 )
//--------------------------------------------------------------------
namespace FK2DEngine
{
	namespace FPS
	{
		void RegisterFrame();
	}

	namespace Hook
	{
		typedef void( *HookOfHorror )();
		HookOfHorror OncePerTick = 0;
	}

	unsigned int ScreenWidth()
	{
		return GetSystemMetrics( SM_CXSCREEN );
	}

	unsigned int ScreenHeight()
	{
		return GetSystemMetrics( SM_CYSCREEN );
	}

	namespace
	{
		int FindClosestVideoMode( int* w, int* h, int* bpp, int* refresh )
		{
			int nMode			= 0;
			int nBestMode		= 0;
			int nMatch			= 0;
			int nBestMatch		= 0;
			int nRr				= 0;
			int nBestRr			= 0;
			int nSuccess		= 0;

			DEVMODE dm;

			nBestMatch			= 0x7fffffff;
			nBestRr				= 0x7fffffff;

			nMode = nBestMode	= 0;

			do
			{
				dm.dmSize		= sizeof( DEVMODE );

				nSuccess		= EnumDisplaySettings( NULL, nMode, &dm );
				if( nSuccess )
				{
					nMatch		= dm.dmBitsPerPel - *bpp;
					if( nMatch < 0 )
					{
						nMatch =- nMatch;
					}

					nMatch = ( nMatch << 25 ) |
						(( dm.dmPelsWidth - *w ) * ( dm.dmPelsWidth - *w ) +
						( dm.dmPelsHeight - *h ) * ( dm.dmPelsHeight - *h ));

					if( nMatch < nBestMatch )
					{
						nBestMatch = nMatch;
						nBestMode = nMode;
						nBestRr = ( dm.dmDisplayFrequency - *refresh ) * ( dm.dmDisplayFrequency - *refresh );
					}
					else if( nMatch == nBestMatch && *refresh > 0 )
					{
						nRr = ( dm.dmDisplayFrequency - *refresh ) * ( dm.dmDisplayFrequency - *refresh );

						if( nRr  < nBestRr )
						{
							nBestMatch = nMatch;
							nBestMode = nMode;
							nBestRr = nRr;
						}
					}
				}
				++nMode;
			}
			while( nSuccess );

			dm.dmSize		= sizeof( DEVMODE );
			EnumDisplaySettings( NULL, nBestMode, &dm );

			*w		= dm.dmPelsWidth;
			*h		= dm.dmPelsHeight;
			*bpp	= dm.dmBitsPerPel;
			*refresh= dm.dmDisplayFrequency;

			return nBestMode;
		}

		void SetVideoMode( int p_nMode )
		{
			DEVMODE	dm;
			dm.dmSize	= sizeof( DEVMODE );
			EnumDisplaySettings( NULL, p_nMode, &dm );

			dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

			dm.dmSize	= sizeof( DEVMODE );

			if( ChangeDisplaySettings( &dm, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL )
			{
				throw std::runtime_error( "不允许设置全屏模式" );
			}
		}

		void SetupVSync()
		{
			char* pExtensions = ( char* )glGetString( GL_EXTENSIONS );

			if( !strstr( pExtensions, "WGL_EXT_swap_control") )
			{
				return;
			}

			typedef void ( APIENTRY* PFNWGLEXTSWAPCONTROLPROC )( int );

			PFNWGLEXTSWAPCONTROLPROC wglSwapIntervalEXT = 
				( PFNWGLEXTSWAPCONTROLPROC ) wglGetProcAddress( "wglSwapIntervalEXT" );
			if( !wglSwapIntervalEXT )
			{
				return;
			}

			wglSwapIntervalEXT( 1 );
		}

		// 窗口一级回调消息处理函数
		LRESULT CALLBACK FKWindowProc( HWND p_hWnd, UINT p_unMsg, WPARAM p_wParam, LPARAM p_lParam )
		{
			// 向各个注册的监听器分发消息
			SSWinMsgListenerMgr::Instance()->Distribute( p_hWnd, p_unMsg, p_wParam, p_lParam );
			
			// 向GUI注册消息
			SSGUIInputManager::Instance()->MsgProc( p_unMsg, p_wParam, p_lParam );

			// 获取窗口关联用户数据
			LONG_PTR lPtr = GetWindowLongPtr( p_hWnd, GWLP_USERDATA );
			if( lPtr )
			{
				CFKWindow* pObj = reinterpret_cast< CFKWindow* >( lPtr );
				return pObj->HandleMessage( p_unMsg, p_wParam, p_lParam );
			}
			else
			{
				return DefWindowProc( p_hWnd, p_unMsg, p_wParam, p_lParam );
			}
		}

		LPCTSTR WindowClass()
		{
			static LPCTSTR s_szName = 0;
			if( s_szName )
			{
				return s_szName;
			}

			WNDCLASSEX wc;
			ZeroMemory( &wc, sizeof( WNDCLASSEX ) );
			wc.cbSize				= sizeof( WNDCLASSEX );
			wc.lpszClassName		= L"FKEnginWindow";
			wc.style				= CS_OWNDC;
			wc.lpfnWndProc			= FKWindowProc;
			wc.cbClsExtra			= 0;
			wc.cbWndExtra			= 0;
			wc.hInstance			= FK2DEngine::Win::Instance();
			wc.hIcon				= ExtractIcon( wc.hInstance, Win::AppFileName().c_str(), 0 );
			wc.hCursor				= 0;
			wc.hbrBackground		= CreateSolidBrush(0);
			wc.lpszMenuName			= 0;
			wc.hIconSm				= ( HICON )CopyImage( wc.hIcon, IMAGE_ICON,
				GetSystemMetrics( SM_CXSMICON ), GetSystemMetrics( SM_CYSMICON ),
				LR_COPYFROMRESOURCE | LR_COPYRETURNORG );

			s_szName = reinterpret_cast< LPCTSTR >( RegisterClassEx( &wc ) );
			Win::Check( s_szName, "注册一个窗口类" );

			return s_szName;
		}
	}
}
//--------------------------------------------------------------------
struct FK2DEngine::CFKWindow::SImpl
{
	HWND							m_hHandle;				// 窗口句柄
	HDC								m_hHDC;					// 窗口HDC
	HGLRC							m_hHRC;
	boost::scoped_ptr< CGraphics >	m_pGraphics;			// 渲染器
	boost::scoped_ptr< CInput >		m_pInput;				// 控制器
	double							m_dUpdateInterval;		// 窗口刷新率
	bool							m_bIconConified;
	unsigned int					m_unOriginalWidth;		// 窗口宽
	unsigned int					m_unOriginalHeight;		// 窗口高度
	HCURSOR							m_hArrowCursor;			// 静态鼠标指针
	CColor							m_bkColor;				// 背景颜色

	SImpl()
		: m_hHandle( NULL )
		, m_hHDC( NULL )
		, m_bIconConified( false )
	{
		m_hArrowCursor = LoadCursor( 0, IDC_ARROW );
	}

	~SImpl()
	{
		if( m_hHDC )
		{
			ReleaseDC( m_hHandle, m_hHDC );
		}
		if( m_hHandle )
		{
			DestroyWindow( m_hHandle );
		}
		if( m_hArrowCursor )
		{
			DeleteObject( m_hArrowCursor );
		}
	}
};
//--------------------------------------------------------------------
class FK2DEngine::CAudio
{

};
//--------------------------------------------------------------------
namespace
{
	FK2DEngine::CAudio		s_DummyAudio;
}
//--------------------------------------------------------------------
FK2DEngine::CFKWindow::CFKWindow(unsigned int p_unWidth, unsigned int p_unHeight,
				bool p_bFullScreen, double p_dUpdateInterval /*= 16.666666*/ )
				: m_pImpl( new SImpl )
{
	m_pImpl->m_unOriginalWidth		= p_unWidth;
	m_pImpl->m_unOriginalHeight		= p_unHeight;

	DWORD dwStyle	= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	DWORD dwStyleEx	= WS_EX_APPWINDOW;

	if( p_bFullScreen )
	{
		dwStyle |= WS_POPUP;
#ifdef NDEBUG
		dwStyleEx |= WS_EX_TOPMOST;
#endif
	}
	else
	{
		dwStyle |= WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
		dwStyleEx |= WS_EX_WINDOWEDGE;
	}
	// 创建窗口
	m_pImpl->m_hHandle = CreateWindowEx( dwStyleEx, WindowClass(), 0, dwStyle,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, 
		Win::Instance(), 0 );
	Win::Check( m_pImpl->m_hHandle );

	// 获取窗口DC
	m_pImpl->m_hHDC		= GetDC( Handle() );
	Win::Check( m_pImpl->m_hHDC );

	// 设置OpenGL绘制平面属性
	PIXELFORMATDESCRIPTOR		pFD;
	ZeroMemory( &pFD, sizeof( pFD ) );
	pFD.nSize					= sizeof( pFD );
	pFD.nVersion				= 1;
	pFD.dwFlags					= PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pFD.iLayerType				= PFD_MAIN_PLANE;
	pFD.iPixelType				= PFD_TYPE_RGBA;
	pFD.cColorBits				= 32;
	int nPF	= ChoosePixelFormat( m_pImpl->m_hHDC, &pFD );
	Win::Check( nPF );
	Win::Check( SetPixelFormat( m_pImpl->m_hHDC, nPF, &pFD ) );

	// 创建OpenGL渲染器环境并设置RC为当前DC
	HGLRC hRc = Win::Check( wglCreateContext( m_pImpl->m_hHDC ), "检查渲染上下文" );
	m_pImpl->m_hHRC = hRc;			// 保存HRC
	Win::Check( wglMakeCurrent( m_pImpl->m_hHDC, hRc ), "选择渲染上下文" );

	SetupVSync();

	// 设置窗口用户定义
	SetLastError(0);
	SetWindowLongPtr( Handle(), GWLP_USERDATA, reinterpret_cast< LONG_PTR >( this ) );
	if( GetLastError() != 0 )
	{
		Win::ThrowLastError( "设置窗口的GWLP_USERDATA 指针失败");
	}

	// 非全屏模式
	if( !p_bFullScreen )
	{
		double dFactor = std::min( 0.9 * FK2DEngine::ScreenWidth() / p_unWidth, 0.8 * FK2DEngine::ScreenHeight() / p_unHeight );

		if( dFactor < 1 )
		{
			p_unWidth *= dFactor;
			p_unHeight *= dFactor;
		}
	}

	RECT rc = { 0, 0, p_unWidth, p_unHeight };

	// 重设置窗口风格
	AdjustWindowRectEx( &rc, dwStyle, FALSE, dwStyleEx );
	unsigned int unWindowW = rc.right - rc.left;
	unsigned int unWindowH = rc.bottom - rc.top;

	int nWindowX	= 0;
	int	nWindowY	= 0;

	if( !p_bFullScreen )
	{
		HWND hDesttopWindow = GetDesktopWindow();
		RECT rcDesktop;
		GetClientRect( hDesttopWindow, &rcDesktop );
		int nDestTopW = rcDesktop.right - rcDesktop.left;
		int nDestTopH = rcDesktop.bottom - rcDesktop.top;
		nWindowX = ( nDestTopW - unWindowW ) / 2;
		nWindowY = ( nDestTopH - unWindowH ) / 2;
	}

	MoveWindow( Handle(), nWindowX, nWindowY, unWindowW, unWindowH, false );

	// 创建并设置图形系统属性
	m_pImpl->m_pGraphics.reset( new FK2DEngine::CGraphics( p_unWidth, p_unHeight, p_bFullScreen ) );
	Graphics().SetResolution( m_pImpl->m_unOriginalWidth, m_pImpl->m_unOriginalHeight );

	// 创建并设置输入系统属性
	m_pImpl->m_pInput.reset( new FK2DEngine::CInput( Handle() ) );
	Input().SetMouseFactors( 1.0 * m_pImpl->m_unOriginalWidth / p_unWidth,
		1.0 * m_pImpl->m_unOriginalHeight / p_unHeight );
	Input().OnButtonDown = boost::bind( &CFKWindow::KeyDown, this, _1 );
	Input().OnButtonUp = boost::bind( &CFKWindow::KeyUp, this, _1 );

	// 设置刷新率
	m_pImpl->m_dUpdateInterval = p_dUpdateInterval;

	// 设置UIInput
	SSGUIInputManager::Instance()->Init( Handle() );

	// 初始化GUI
	SSGUIManager::Instance()->Init( Graphics() );

	// 初始化动画光标管理器
	SSAnimationCursorManager::Instance()->Init( Graphics(), 100 );

	// 开启DEBUG内存泄漏检测
	System::EnableMemoryLeakCheck();

	// 开启Log
	SSLogManager::Instance()->CreateLog( FK2DEngine::ShareResourcePrefix() + L"FK2DEngineDefaultLog.txt", true, false );
	SSLogManager::Instance()->LogMessage( L"引擎初始化完毕" );
}
//--------------------------------------------------------------------
FK2DEngine::CFKWindow::~CFKWindow()
{
	SSLogManager::Instance()->LogMessage( L"引擎释放完成" );
	wglMakeCurrent( 0, 0 );
}
//--------------------------------------------------------------------
std::wstring FK2DEngine::CFKWindow::Caption() const
{
	int nBufLen = GetWindowTextLength( Handle() ) + 1;
	if( nBufLen < 2 )
	{
		return L"";
	}

	std::vector< TCHAR >		Buf( nBufLen );
	GetWindowText( Handle(), &Buf.front(), nBufLen );
	return &Buf.front();
}
//--------------------------------------------------------------------
void FK2DEngine::CFKWindow::SetCaption(const std::wstring &p_szCaption)
{
	SetWindowText( Handle(), p_szCaption.c_str() );
}
//--------------------------------------------------------------------
double FK2DEngine::CFKWindow::UpdateInterval() const
{
	return m_pImpl->m_dUpdateInterval;
}
//--------------------------------------------------------------------
void FK2DEngine::CFKWindow::Show()
{
	int w = m_pImpl->m_unOriginalWidth;
	int h = m_pImpl->m_unOriginalHeight;
	int bpp = 32;
	int rr = 60;

	if( Graphics().FullScreen() )
	{
		 SetVideoMode( FindClosestVideoMode( &w, &h, &bpp, &rr ) );
	}

	ShowWindow( Handle(), SW_SHOW );
	UpdateWindow( Handle() );

	try
	{
		Win::ProcessMessage();

		unsigned int unLastTick = 0;

		for( ; ; )
		{
			// 消息分发
			Win::ProcessMessage();
			
			if( !::IsWindowVisible( Handle() ) )
			{
				Sleep( 50 );
				return;
			}

			// 单位时间帧循环
			unsigned int ulMS = MilliSeconds();
			if( ulMS < unLastTick ||( ulMS - unLastTick ) >= static_cast< unsigned int >( m_pImpl->m_dUpdateInterval ) )
			{
				unsigned int nDelta = 1;
				if( unLastTick != 0 )
				{
					nDelta = ulMS - unLastTick;
				}

				// 帧延迟性能警告
				if( nDelta >= MAX_FRAME_INTERVAL_TIME )
				{
					SSLogManager::Instance()->LogMessage( L"MainLoop延迟时间过长: " + ToString(nDelta) );
				}

				// 更新Input
				Input().Update( nDelta );
				
				// 为兼容不同平台，Handle未传入，在这里进行一下鼠标恢复逻辑
				if( Input().MouseX() >= 0 && Input().MouseY() >= 0 )
				{
					SendMessage( Handle(), WM_SETCURSOR, reinterpret_cast< WPARAM >( Handle() ), HTCLIENT );
				}
				
				// UIInput消息更新
				SSGUIInputManager::Instance()->Update( nDelta );

				// 更新UI
				SSGUIManager::Instance()->Update( nDelta );

				// 更新光标管理器
				SSAnimationCursorManager::Instance()->Update( nDelta );
				
				// 用户逻辑更新
				Update( nDelta );

				// 每帧重绘
				if( NeedsReDraw() )
				{
					::InvalidateRect( Handle(), 0, FALSE );
					// 刷新FPS
					FPS::RegisterFrame();
				}
				// Hook 事件刷新
				if( Hook::OncePerTick )
				{
					Hook::OncePerTick();
				}

				unLastTick = ulMS;
			}
			else
			{
				FK2DEngine::FKSleep( 5 );
			}
		}
	}
	catch( ... )
	{
		Close();
		throw;
	}
}
//--------------------------------------------------------------------
bool FK2DEngine::CFKWindow::Close()
{
	// 用户销毁处理
	if( !OnClose() )
	{
		return false;
	}
	ShowWindow( Handle(), SW_HIDE );
	if( Graphics().FullScreen() )
	{
		ChangeDisplaySettings( NULL, CDS_FULLSCREEN );
	}
	return true;
}
//--------------------------------------------------------------------
void FK2DEngine::CFKWindow::SetNewCursor( const std::wstring& p_lpCursorName )
{
	HCURSOR hCursor			=	::LoadCursorFromFileW( p_lpCursorName.c_str() );
	if( hCursor == NULL )
	{
		SSLogManager::Instance()->LogMessage( L"加载鼠标失败：" + p_lpCursorName );
	}
	::SetClassLong( m_pImpl->m_hHandle, GCL_HCURSOR, (LONG) hCursor );
	if( hCursor != NULL  )
	{
		m_pImpl->m_hArrowCursor =  hCursor;
	}
}
//--------------------------------------------------------------------
// 截屏函数
void FK2DEngine::CFKWindow::PrintScreen( const std::wstring& p_lpDestName )
{
	// 获取窗口DC
	Win::Check( m_pImpl->m_hHDC );
	// 为屏幕DC创建兼容的内容设备描述DC
	HDC hMemDC = CreateCompatibleDC( m_pImpl->m_hHDC );
	// 获取窗口分辨率
	int nScreenX = m_pImpl->m_unOriginalWidth;
	int nScreenY = m_pImpl->m_unOriginalHeight;
	// 创建位图
	HBITMAP hBitmap = CreateCompatibleBitmap( m_pImpl->m_hHDC, nScreenX, nScreenY );
	// 装载新文图到内存设备描述表内
	HBITMAP hOldBitmap = (HBITMAP)( SelectObject( hMemDC, hBitmap ) );
	// 将屏幕设备描述表拷贝到内存设备秒鼠标内
	BitBlt( hMemDC, 0, 0, nScreenX, nScreenY, m_pImpl->m_hHDC, 0, 0, SRCCOPY );
	// 获取屏幕位图句柄
	hBitmap = (HBITMAP)(SelectObject( hMemDC, hOldBitmap ) );

	// 保存图像文件
	if( SaveHBITMAP2File( p_lpDestName, hBitmap, hMemDC ) )
	{
		SSLogManager::Instance()->LogMessage( L"创建截图文件成功!" + p_lpDestName );
	}
	else
	{
		SSLogManager::Instance()->LogMessage( L"创建截图文件失败!" + p_lpDestName );
	}
	// 清除DC
	DeleteDC( hMemDC );
}
//--------------------------------------------------------------------
// 获取时间
unsigned int FK2DEngine::CFKWindow::GetTime() const
{
	return MilliSeconds();
}
//--------------------------------------------------------------------
// 获取渲染窗口宽度
unsigned int FK2DEngine::CFKWindow::GetScreenWidth() const
{
	return m_pImpl->m_unOriginalWidth;
}
//--------------------------------------------------------------------
// 获取渲染窗口高度
unsigned int FK2DEngine::CFKWindow::GetScreenHeight() const
{
	return m_pImpl->m_unOriginalHeight;
}
//--------------------------------------------------------------------
// 设置背景颜色
void FK2DEngine::CFKWindow::SetBackgroudColor( CColor p_Color )
{
	m_pImpl->m_bkColor = p_Color;
}
//--------------------------------------------------------------------
const FK2DEngine::CGraphics& FK2DEngine::CFKWindow::Graphics() const
{
	return *m_pImpl->m_pGraphics;
}
//--------------------------------------------------------------------
FK2DEngine::CGraphics& FK2DEngine::CFKWindow::Graphics()
{
	return *m_pImpl->m_pGraphics;
}
//--------------------------------------------------------------------
const FK2DEngine::CInput& FK2DEngine::CFKWindow::Input() const
{
	return *m_pImpl->m_pInput;
}
//--------------------------------------------------------------------
FK2DEngine::CInput& FK2DEngine::CFKWindow::Input()
{
	return *m_pImpl->m_pInput;
}
//--------------------------------------------------------------------
HWND FK2DEngine::CFKWindow::Handle() const
{
	return m_pImpl->m_hHandle;
}
//--------------------------------------------------------------------
const FK2DEngine::CAudio& FK2DEngine::CFKWindow::Audio() const
{
	return s_DummyAudio;
}
//--------------------------------------------------------------------
FK2DEngine::CAudio& FK2DEngine::CFKWindow::Audio()
{
	return s_DummyAudio;
}
//--------------------------------------------------------------------
HRESULT FK2DEngine::CFKWindow::HandleMessage(UINT p_unMessage, WPARAM p_wParam, LPARAM p_lParam)
{
	if( p_unMessage == WM_SETFOCUS )
	{
		if( LOWORD( p_lParam ) != HTCLIENT || GetForegroundWindow() != Handle() || NeedsCursor() )
		{
			SetCursor( m_pImpl->m_hArrowCursor );
		}
		else
		{
			SetCursor( NULL );
		}

		return TRUE;
	}

	if( p_unMessage == WM_SETFOCUS && Graphics().FullScreen() && IsWindowVisible( m_pImpl->m_hHandle ) )
	{
		if( m_pImpl->m_bIconConified )
		{
			OpenIcon( m_pImpl->m_hHandle );

			int w = Graphics().Width();
			int h = Graphics().Height();
			int bpp = 32;
			int rr = 60;

			SetVideoMode( FindClosestVideoMode( &w, &h, &bpp, &rr ) );

			m_pImpl->m_bIconConified = false;
		}

		return 0;
	}

	// 激活消息
	if( p_unMessage == WM_ACTIVATEAPP )
	{
		// TODO： 加是否暂停
	}

	if( p_unMessage == WM_SETFOCUS && Graphics().FullScreen() && IsWindowVisible( m_pImpl->m_hHandle ) )
	{
		if( !m_pImpl->m_bIconConified )
		{
			ChangeDisplaySettings( NULL, CDS_FULLSCREEN );
			CloseWindow( m_pImpl->m_hHandle );
			m_pImpl->m_bIconConified = true;
		}

		return 0;
	}

	if( p_unMessage == WM_DESTROY )
	{
		PostQuitMessage( 0 );
		return 0;
	}

	if( p_unMessage == WM_CLOSE )
	{
		if( Close() )
		{
			DestroyWindow( Handle() );
			return 0;
		}
	}
	// 主绘制
	if( p_unMessage == WM_PAINT )
	{
		// 清屏
		// InvalidateRect( Handle(), NULL, TRUE );

		if( m_pImpl->m_pGraphics && Graphics().Begin( m_pImpl->m_bkColor ) )
		{
			try
			{
				SSAnimationCursorManager::Instance()->FKDraw();
				SSGUIManager::Instance()->FKDraw();
				FKDraw();
			}
			catch( ... )
			{
				Graphics().End();
				throw;
			}
			Graphics().End();
		}

		SwapBuffers( m_pImpl->m_hHDC );

		// 刷新窗口
		ValidateRect( Handle(), NULL );
		return 0;
	}
	if( p_unMessage == WM_SYSCOMMAND )
	{
		switch( p_wParam )
		{
		case SC_SCREENSAVE:
		case SC_MONITORPOWER:
			if( Graphics().FullScreen() )
			{
				return 0;
			}
			else
			{
				break;
			}
		case SC_KEYMENU:
			return 0;
		}
	}
	// 设置光标
	if(( p_unMessage == WM_MOUSEMOVE ) || ( p_unMessage == WM_SETCURSOR ))
	{
		SetCursor( m_pImpl->m_hArrowCursor );

		return 0;
	}

	if( m_pImpl->m_pInput && Input().TextInput() && Input().TextInput()->FeedMessage( p_unMessage, p_wParam, p_lParam ) )
	{
		return 0;
	}

	return DefWindowProc( Handle(), p_unMessage, p_wParam, p_lParam );
}