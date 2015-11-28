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
*	文件说明：	渲染窗口类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//窗口信息
	struct WindowInfo
	{
		char*		sCaption;	//窗口标题
		bool		bCenter;	//是否窗口居中
		UINT		nLeft;		//左边距
		UINT		nTop;		//上边距
		UINT		nWidth;		//宽度
		UINT		nHeight;	//高度
		DWORD		dwStyle;	//窗口风格

		WNDPROC		fnWndProc;	//窗口消息处理函数
		HCURSOR		hCursor;	//窗口鼠标指针句柄
		HICON		hIcon;		//窗口图标句柄
	};
	//--------------------------------------------------------------------------------------------------------------
	//渲染窗口类
	class FK_DLL RenderWindow
	{
	public:
		HWND			mhWnd;			//窗口句柄
		HINSTANCE		mhInst;			//程序实例句柄
		WNDCLASSEX		mWndClass;		//窗口类型

		String			mWndCaption;	//窗口标题
		UINT			mnLeft;			//左边距
		UINT			mnTop;			//上边距
		UINT			mnWidth;		//宽度
		UINT			mnHeight;		//高度
		DWORD			mdwStyle;		//窗口风格

		UINT			mnClientWidth;	//客户区宽度
		UINT			mnClientHeight;	//客户区高度

		HCURSOR			mhCursor;		//鼠标指针句柄

		MSG				mMsg;			//窗口消息
		bool			mbDestroy;		//窗口是否被销毁
		bool			mbActive;		//窗口是否被激活
		bool			mbInner;		//是否为内部窗口
		bool			mbShowCursor;	//是否显示鼠标指针

	public:
		RenderWindow();
		~RenderWindow();

		//内部窗口消息处理函数（必须在传入的窗口消息函数中调用）
		LRESULT WinProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

		//创建渲染窗口
		void Create( const WindowInfo* pWndInfo );

		//使用已存在窗口创建渲染窗口
		void CreateByExist( HWND hWnd );

		//显示/隐藏游戏窗口
		void ShowGameWindow( bool bShow = true );

		//显示/隐藏游戏窗口鼠标指针
		void ShowCursor( bool bShow = true );

	protected:
		//获取窗口位置
		void _GetWindowRect();

		//获取客户区尺寸
		void _GetWindowClient();
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
