/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	WinControl
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "UIControl.h"
#include "Bitmap.h"
#include "Image.h"
#include <vector>
//--------------------------------------------------------------------
namespace FK2DEngine
{
	//--------------------------------------------------------------------
	//! 窗体控件类
	//@remarks	本控件是作为一个控件容器存在的，该对象的所有子控件将共享
	//			本控件的有效性和可见性。
	//			同时，由于子控件受父控件坐标系影响，所有子控件将与父控件
	//			保证位置的同步。
	class CWinControl : public IGUIControl
	{
	public:
		CWinControl();
		virtual ~CWinControl();
	public:
		virtual void FKDraw() const;

		//! 设置背景图片
		void SetBG( CBitmap& p_bBackground );

		//! 本控件不可接受焦点
		virtual bool CanHaveFocus() const
		{
			return false;
		}
	protected:
		CBitmap*				m_pBackGround;		// 背景图片指针
		std::vector< CImage >	m_vecImage;			// 背景图片
	};
	//--------------------------------------------------------------------
}