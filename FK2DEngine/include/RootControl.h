/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	RootControl
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "UIControl.h"
//--------------------------------------------------------------------
namespace FK2DEngine
{
	//--------------------------------------------------------------------
	//! 根UI窗口
	//@remarks 全局唯一，所有控件均为该控件的挂接子控件
	class CRootControl : public IGUIControl
	{
	public:
		CRootControl()
		{
			m_bVisible = false;
			m_nLeft = 0;
			m_nTop = 0;
			m_nHeight = 0;
			m_nWidth = 0;
		}
		~CRootControl()
		{

		};
	public:
		//! 本控件不接受焦点
		virtual bool CanHaveFocus() const
		{
			return false;
		}
		//! 本控件无需绘制
		virtual void FKDraw() const
		{
			return;
		}
	};
}