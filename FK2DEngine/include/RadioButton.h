/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	RadioButton
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "CheckBox.h"
#include "Bitmap.h"
//--------------------------------------------------------------------
namespace FK2DEngine
{
	//--------------------------------------------------------------------
	class IGUIControl;
	//--------------------------------------------------------------------
	//! 单选框
	class CRadioButton : public CCheckBox
	{
	public:
		CRadioButton();
		virtual ~CRadioButton(){};
	public:
		void Create( IGUIControl* p_pParent, int p_nLeft, int p_nTop,
			CBitmap& p_Bitmap, const std::wstring& p_szCaption );

		void Check();
		void SetGroup( int p_nGroup );
		int GetGroup() const;
	public:
		virtual void MouseUp( POINT p_Point, ENUM_MouseButton p_eMB );
	private:
		int					m_nGroup;			// 按钮所属组
	};
	//--------------------------------------------------------------------
}