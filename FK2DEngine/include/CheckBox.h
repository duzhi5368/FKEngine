/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	CheckBox
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "PicButton.h"
//--------------------------------------------------------------------
namespace FK2DEngine
{
	//--------------------------------------------------------------------
	//! 复选框
	class CCheckBox : public CPicButton
	{
	public:
		CCheckBox();
		virtual ~CCheckBox(){}
	public:
		void Create( IGUIControl* p_pParent, int p_nLeft, int p_nTop,
			CBitmap& p_Bitmap, const std::wstring& p_szCaption );

		virtual void FKDraw() const;

		//! 设置图片
		//@note 要求一行六列图片，前四列和 CPicButton 一致，第五列为选中记号，第六列为未选中状态
		void SetGlygh( CBitmap& p_Bitmap );

		void Check();
		void UnCheck();
		bool IsChecked();
	public:
		//! 改写鼠标事件函数
		virtual void MouseUp( POINT p_Point, ENUM_MouseButton p_eMB );
	protected:
		bool			m_bChecked;		// 是否被选中
	};
	//--------------------------------------------------------------------
}