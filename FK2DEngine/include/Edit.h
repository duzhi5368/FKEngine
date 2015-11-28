/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Edit
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "Color.h"
#include "UIControl.h"
#include "Font.h"
//--------------------------------------------------------------------
namespace FK2DEngine
{
	//--------------------------------------------------------------------
	//! 光标类型（垂直/水平）
	enum ENUM_CaretStyle
	{
		ECS_Vertical,
		ECS_Horizontal,
	};
	//--------------------------------------------------------------------
	//! 文本编辑框
	//@remarks 暂时不支持剪切版和中文
	class CEdit : public IGUIControl
	{
	public:
		CEdit();
		virtual ~CEdit(){}
	public:
		void Create( IGUIControl* p_pParent, int p_nLeft, int p_nTop, int p_nWidth );
		virtual void FKDraw() const;
		virtual void Update( float p_fDelta );

	public:
		void SetParent( IGUIControl* p_pParent );
		void SetFont( CFont& p_Font );
		void SetText( const std::wstring& p_Text );
		std::wstring GetText() const;
		//! 设置光标颜色
		void SetCaretColor( CColor p_Color );
		//! 设置光标类型
		void SetCaretType( ENUM_CaretStyle p_eStyle );

	public:
		//! 改写的事件处理函数
		//! 鼠标按下
		virtual void MouseDown( POINT p_Point, ENUM_MouseButton p_eMB );
		//! 键盘按下
		virtual void KeyDown( unsigned long p_ulKey, SShiftState p_State );
		//! 字符键按下
		virtual void KeyPress( char p_cKey );

	private:
		//! 移动光标
		void MoveCaret( int p_nOffset );

	private:
		std::wstring			m_szText;		// 文本内容
		float					m_fTime;		// 计时器
		int						m_nCaretPos;	// 光标位置
		int						m_nCharWidth;	// 字符宽度
		bool					m_bShowCaret;	// 是否显示光标
		CColor					m_CaretColor;	// 光标颜色
		ENUM_CaretStyle			m_eCaretStyle;	// 光标类型
	};
}