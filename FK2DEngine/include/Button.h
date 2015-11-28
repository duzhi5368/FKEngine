/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Button
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "../Include/PicButton.h"
#include "../Include/Bitmap.h"
//--------------------------------------------------------------------
namespace FK2DEngine
{
	//--------------------------------------------------------------------
	class IGUIControl;
	//--------------------------------------------------------------------
	class CButton : public CPicButton
	{
	public:
		void Create( IGUIControl* p_pParent, int p_nLeft, int p_nTop, 
			CBitmap& p_Bitmap, const std::wstring& p_szCaption );

		virtual void FKDraw() const;

		void SetCaption( const std::wstring& p_szCaption );
	private:
		POINT				m_TextPos;		// 标题位置
	};
	//--------------------------------------------------------------------
}