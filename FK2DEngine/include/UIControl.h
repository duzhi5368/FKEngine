/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	UIControl
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include <Windows.h>
#include "WinKey.h"
#include "Font.h"
#include <list>
#include "UIInputManager.h"
//--------------------------------------------------------------------

namespace FK2DEngine
{
	//--------------------------------------------------------------------
	class IGUIControl;
	//--------------------------------------------------------------------
	// 事件处理函数
	typedef void ( IGUIControl::*NotifyFun)( void );
	struct SNotifyEvent
	{
		SNotifyEvent() : function(NULL), invoker(NULL) {}
		NotifyFun function;
		IGUIControl *invoker;
	};

	typedef void ( IGUIControl::*MouseMoveFun)( POINT pt );
	struct SMouseMoveEvent
	{
		SMouseMoveEvent() : function(NULL), invoker(NULL) {}
		MouseMoveFun function;
		IGUIControl *invoker;
	};

	typedef void ( IGUIControl::*MouseFun)( POINT pt, ENUM_MouseButton mb );
	struct SMouseEvent
	{
		SMouseEvent() : function(NULL), invoker(NULL) {}
		MouseFun function;
		IGUIControl *invoker;
	};

	typedef void ( IGUIControl::*KeyFun)( unsigned long p_ulKey, SShiftState p_State );
	struct SKeyEvent
	{
		SKeyEvent() : function(NULL), invoker(NULL) {}
		KeyFun function;
		IGUIControl *invoker;
	};

	typedef void ( IGUIControl::*KeyPressFun)( char p_cKey );
	struct SKeyPressEvent
	{
		SKeyPressEvent() : function(NULL), invoker(NULL) {}
		KeyPressFun function;
		IGUIControl *invoker;
	};
	//--------------------------------------------------------------------
	//! UI控件基类
	//@remarks	本类核心提供了两个级别的事件处理能力。
	//			第一是控件级，当事件发生后将被 Gui 类调用，各子类继承实现各自功能。
	//			第二是用户级，函数指针开放给用户，用户自行编写事件处理函数，同时，
	//				用户将函数地址给控件。
	//			控件在执行完毕自身事件后，调用用户事件。
	class IGUIControl
	{
	public:
		IGUIControl();
		virtual ~IGUIControl();
	private:
		IGUIControl( const IGUIControl& );
		IGUIControl& operator= ( const IGUIControl& );
	public:
		virtual void FKDraw() const = 0;
		virtual void Update( float p_fDelta ){}
		//! 是否可以获取焦点
		//@ 可以获得焦点的控件将可以处理键盘事件
		virtual bool CanHaveFocus() const{ return true; }
	public:
		//! 鼠标进入
		virtual void MouseEnter();
		//! 鼠标离开
		virtual void MouseLeave();
		//! 鼠标移动
		virtual void MouseMove( POINT p_Point );
		//! 鼠标按下
		virtual void MouseDown( POINT p_Point, ENUM_MouseButton p_eMB );
		//! 鼠标弹起
		virtual void MouseUp( POINT p_Point, ENUM_MouseButton p_eMB );
		//! 键盘按下
		virtual void KeyDown( unsigned long p_ulKey, SShiftState p_State );
		//! 键盘弹起
		virtual void KeyUp( unsigned long p_ulKey, SShiftState p_State );
		//! 字符键按下
		virtual void KeyPress( char p_cKey );
		//! 获得焦点
		virtual void Enter();
		//! 失去焦点
		virtual void Leave();
	public:
		//! 创建控件，在 Gui 类的唯一实例中注册本空间
		void Create();
		//! 设置父控件
		void SetParent( IGUIControl* p_pParent );
		//! 添加子控件
		void AddChild( IGUIControl* p_pChild );
		//! 删除子控件
		void DelChild( IGUIControl* p_pChild );
		//! 使控件有效
		void Enable();
		//! 使控件无效
		void Disable();
		//! 显示控件
		void Show();
		//! 隐藏控件
		void Hide();
		//! 控件坐标转屏幕（程序窗口坐标）
		void ClientToScreen( POINT &p_Point ) const;
		//! 屏幕（程序窗口）坐标转控件坐标
		void ScreenToClient( POINT &p_Point ) const;
		//! 判断一点是否在控件内
		bool IsPointIn( POINT p_Point ) const;
		//! 使控件获取焦点
		void SetFocus();
		//! 判断控件是否有效
		bool IsEnabled() const;
		//! 判断控件是否可见
		bool IsVisiable() const;
		//! 判断控件是否获得焦点
		bool IsFocesed() const;
		//! 设置控件和父控件左侧距离
		void SetLeft( int p_nLeft );
		//! 设置控件和父控件顶端距离
		void SetTop( int p_nTop );
		//! 取控件距父控件左侧距离
		int GetLeft() const;
		//! 取控件距父控件顶端距离
		int GetTop() const;
		//! 设置控件宽度
		void SetWidth( int p_nWidth );
		//! 设置控件高度
		void SetHeight( int p_nHeight );
		//! 取控件宽度
		int GetWidth() const;
		//! 取控件高度
		int GetHeight() const;
		//! 设置控件字体
		void SetFont( CFont &p_Font );
		//! 获取控件字体
		CFont* GetFont() const;
		//! 设置控件字体颜色
		void SetFontColor( CColor &p_Color );
		//! 获取控件字体颜色
		CColor* GetColor() const;
		//! 设置控件标题
		void SetCaption( const std::wstring& p_szCaption );
		//! 获取控件标题
		std::wstring GetCaption() const;
		//! 设置控件深度
		void SetZPos( ZPos p_nZPos );
		//! 返回控件深度
		ZPos GetZPos() const;
	public:
		//! 用户自定义事件处理函数指针
		//@note 其中传入的鼠标指针位置是相对于父控件的，不是相对屏幕的
		SNotifyEvent OnMouseEnter;
		SNotifyEvent OnMouseLeave;
		SMouseMoveEvent OnMouseMove;
		SMouseEvent OnMouseDown;
		SMouseEvent OnMouseUp;
		SKeyEvent OnKeyDown;
		SKeyEvent OnKeyUp;
		SKeyPressEvent OnKeyPress;
		SNotifyEvent OnEnter;
		SNotifyEvent OnLeave;

	protected:
		IGUIControl*				m_pParent;			// 父控件指针
		int							m_nLeft;			// 距父控件左侧距离
		int							m_nTop;				// 距父控件顶端距离
		int							m_nWidth;			// 宽度
		int							m_nHeight;			// 高度
		bool						m_bEnabled;			// 是否有效
		bool						m_bVisible;			// 是否可见
		bool						m_bFocused;			// 是否获得焦点
		CFont*						m_pFont;			// 字体
		std::wstring				m_szCaption;		// 标题
		std::list< IGUIControl* >	m_listChild;		// 子控件链表
		CColor*						m_pColor;			// 字体颜色
		ZPos						m_dZpos;			// UI深度
	};
	//--------------------------------------------------------------------
}