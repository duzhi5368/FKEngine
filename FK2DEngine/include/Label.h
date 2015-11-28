/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Label
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
	//! 标签类
	//@remarks 本类可显示一段静态文字
	class CLable : public IGUIControl
	{
	public:
		void Create( IGUIControl* p_pParent, int p_nLeft, int p_nTop, const std::wstring& p_szCaption );
		virtual void FKDraw() const;

		//! 本控件不可接受焦点
		virtual bool CanHaveFocus() const
		{
			return false;
		}
	};
	//--------------------------------------------------------------------
}