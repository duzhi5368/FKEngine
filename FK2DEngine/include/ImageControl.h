/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	ImageControl
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
#include <vector>
#include "Image.h"
//--------------------------------------------------------------------
namespace FK2DEngine
{
	//--------------------------------------------------------------------
	//! 图片控件
	//@remarks 本类可显示一幅静态图片
	class CImageControl : public IGUIControl
	{
	public:
		CImageControl();
	public:
		void Create( IGUIControl* p_pParent, int p_nLeft, int p_nTop,
			CBitmap& p_Bitmap );
		virtual void FKDraw() const;
		//! 该控件不可接受焦点
		virtual bool CanHaveFocus() const
		{
			return false;
		}
	public:
		void SetImage( CBitmap& p_Bitmap );
	private:
		CBitmap*					m_pBitmap;		// 图片指针
		std::vector< CImage >		m_vecImages;	// 图块列表
	};
	//--------------------------------------------------------------------
}