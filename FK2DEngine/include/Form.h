/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Form
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "WinControl.h"
#include "Bitmap.h"
//--------------------------------------------------------------------
namespace FK2DEngine
{
	//--------------------------------------------------------------------
	class CForm : public CWinControl
	{
	public:
		CForm();
		virtual ~CForm(){}
	public:
		void Create( IGUIControl* p_pParent, int p_nLeft, int p_nTop,
			int p_nWidth, int p_nHeight );
		void Create( IGUIControl* p_pParent, int p_nLeft, int p_nTop,
			CBitmap& p_Bitmap );
	public:
		//! 设置热矩形
		//@remarks 热矩形内鼠标按下可拖动窗口
		void SetHotRect( RECT& p_RC );

		//! 改写事件处理函数
		virtual void MouseMove( POINT p_Point );
		virtual void MouseDown( POINT p_Point, ENUM_MouseButton p_eMB );
		virtual void MouseUp( POINT p_Point, ENUM_MouseButton p_eMB );
	private:
		RECT						m_HotRC;		// 热矩形
		POINT						m_OffsetPT;		// 鼠标按下时热矩形中的偏移
		bool						m_bHolding;		// 鼠标是否正持有窗体
		bool						m_bMoveable;	// 窗体是否可以移动
	};
	//--------------------------------------------------------------------
}