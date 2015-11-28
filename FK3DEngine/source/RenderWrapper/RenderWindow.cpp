/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	RenderWindow
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../../include/RenderWrapper/RenderWrapperInsideHeader.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	RenderWindow::RenderWindow()
		: mhWnd				(NULL)
		, mnLeft			(0)
		, mnTop				(0)
		, mnWidth			(0)
		, mnHeight			(0)
		, mdwStyle			(0)
		, mnClientWidth		(0)
		, mnClientHeight	(0)
		, mhCursor			(NULL)
		, mbDestroy			(false)
		, mbActive			(false)
		, mbInner			(false)
		, mbShowCursor		(true)
	{
		memset( &mMsg, 0, sizeof(MSG) );
	}
	//--------------------------------------------------------------------------------------------------------------
	RenderWindow::~RenderWindow()
	{
		//如果为内部创建窗口
		if( mbInner )
		{
			//摧毁窗口
			DestroyWindow( mhWnd );

			//注销窗口类
			UnregisterClass( "MyGame", mWndClass.hInstance );
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//内部窗口消息处理函数（必须在传入的窗口消息函数中调用）
	LRESULT RenderWindow::WinProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
	{
		switch( msg )
		{
			//防止程序的鼠标指针被意外更改
		case WM_SETCURSOR:
			{
				if( mbShowCursor )
					SetCursor( mhCursor );
				else
					SetCursor( NULL );

				return 0;
			}

			//当程序激活状态改变时同时改变程序运行状态
		case WM_ACTIVATEAPP:
			{
				if( LOWORD( wParam ) == WA_INACTIVE )
				{
					mbActive = false;
				}
				else
				{
					mbActive = true;
				}

				break;
			}

			//阻止发出的屏幕保护和显示器节电模式命令
		case WM_SYSCOMMAND:
			{
				switch (wParam)
				{
				case SC_SCREENSAVE:
				case SC_MONITORPOWER:
					return 0;
				}
				break;
			}

			//刷新窗口大小时
		case WM_PAINT:
			{
				ValidateRect( mhWnd, NULL );
				return 0;
			}

			//改变窗口大小时
		case WM_SIZE:
			{
				_GetWindowRect();
				_GetWindowClient();
				ValidateRect( mhWnd, NULL );
				return 0;
			}

			//移动窗口时
		case WM_MOVE:
			{
				_GetWindowRect();
				ValidateRect( mhWnd, NULL );
				return 0;
			}

			//窗口被销毁
		case WM_DESTROY:
			{
				mbDestroy = true;
				mbActive = false;
				break;
			}
		}

		return DefWindowProc( hWnd, msg, wParam, lParam );
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建渲染窗口
	void RenderWindow::Create( const WindowInfo* pWndInfo )
	{
		//检测消息处理函数指针
		if( pWndInfo->fnWndProc == NULL )
			Except( Exception::ERR_INVALIDPARAMS, "游戏窗口消息处理函数为空，无法创建窗口。" );

		//存储窗口信息
		mWndCaption	= pWndInfo->sCaption;
		mnLeft		= pWndInfo->nLeft;
		mnTop		= pWndInfo->nTop;
		mnWidth		= pWndInfo->nWidth;
		mnHeight	= pWndInfo->nHeight;
		mdwStyle	= pWndInfo->dwStyle;

		mhCursor	= pWndInfo->hCursor;
		
		//如果设置窗口居中则自动计算窗口边距
		if( pWndInfo->bCenter )
		{
			mnLeft = ( GetSystemMetrics(SM_CXSCREEN) - mnWidth ) / 2;
			mnTop = ( GetSystemMetrics(SM_CYSCREEN) - mnHeight ) / 2;
		}

		//获取当前进程句柄
		mhInst = (HINSTANCE)GetModuleHandle( NULL );

		//设置并注册窗口类型
		mWndClass.cbSize		= sizeof(WNDCLASSEX);
		mWndClass.style			= CS_HREDRAW | CS_VREDRAW;
		mWndClass.lpfnWndProc	= pWndInfo->fnWndProc;
		mWndClass.cbClsExtra	= 0;
		mWndClass.cbWndExtra	= 0;
		mWndClass.hInstance		= mhInst;
		mWndClass.hIcon			= pWndInfo->hIcon;
		mWndClass.hCursor		= mhCursor;
		mWndClass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
		mWndClass.lpszMenuName	= NULL;
		mWndClass.lpszClassName	= pWndInfo->sCaption;
		mWndClass.hIconSm		= NULL;

		RegisterClassEx( &mWndClass );

		//创建程序窗口
		mhWnd = CreateWindow(
			pWndInfo->sCaption,
			pWndInfo->sCaption,
			mdwStyle,
			mnLeft,
			mnTop,
			mnWidth,
			mnHeight,
			NULL, NULL, mhInst, NULL );
		if( mhWnd == NULL )
			Except( Exception::ERR_WINAPI_ERROR, "创建游戏窗口失败。" );

		//获取窗口客户区尺寸
		_GetWindowClient();

		//先隐藏窗口
		ShowWindow( mhWnd, SW_HIDE );
		UpdateWindow( mhWnd );

		mbInner = true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//使用已存在窗口创建渲染窗口
	void RenderWindow::CreateByExist( HWND hWnd )
	{
		//获取窗口信息
		mhWnd = hWnd;

		//获取程序实例
		mhInst = (HINSTANCE)GetModuleHandle( NULL );

		//获取窗口风格
		mdwStyle = (DWORD)GetWindowLong( mhWnd, GWL_STYLE ) | (DWORD)GetWindowLong( mhWnd, GWL_EXSTYLE );

		//获取窗口坐标及客户区尺寸
		_GetWindowRect();
		_GetWindowClient();

		//获取窗口标题
		char szCaption[512];
		if( 0 == GetWindowText( hWnd, szCaption, 512 ) )
			Except( Exception::ERR_WINAPI_ERROR, "获取外部创建游戏窗口的标题错误。" );
		else
			mWndCaption = szCaption;

		mbInner = false;
	}
	//--------------------------------------------------------------------------------------------------------------
	//显示/隐藏游戏窗口
	void RenderWindow::ShowGameWindow( bool bShow )
	{
		ShowWindow( mhWnd, SW_NORMAL );
		UpdateWindow( mhWnd );
	}
	//--------------------------------------------------------------------------------------------------------------
	//显示/隐藏游戏窗口鼠标指针
	void RenderWindow::ShowCursor( bool bShow )
	{
		if( bShow )
			SetCursor( mhCursor );
		else
			SetCursor( NULL );

		mbShowCursor = bShow;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取窗口位置
	void RenderWindow::_GetWindowRect()
	{
		RECT rc;
		GetWindowRect( mhWnd, &rc );

		mnLeft		= rc.left;
		mnTop		= rc.top;
		mnWidth		= rc.right - rc.left;
		mnHeight	= rc.bottom - rc.top;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取客户区尺寸
	void RenderWindow::_GetWindowClient()
	{
		RECT rc;
		GetClientRect( mhWnd, &rc );

		mnClientWidth	= rc.right - rc.left;
		mnClientHeight	= rc.bottom - rc.top;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
