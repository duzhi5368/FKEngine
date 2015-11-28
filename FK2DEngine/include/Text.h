/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Text
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "InsidePreInclude.h"
#include "Color.h"
#include "GraphicsBase.h"
#include <string>
//--------------------------------------------------------------------
namespace FK2DEngine
{
	//! 返回当前平台下的默认字体名称
	std::wstring DefaultFontName();

	//! 计算一段文字所占的像素宽度
	unsigned int TextWidth( const std::wstring& p_Text, const std::wstring& p_FontName,
		unsigned int p_unFontWight, unsigned int p_unFontFlags = 0 );

	//! 在一个 Bitmap 上绘制一行文字
	void FKDrawText( CBitmap& p_Bitmap, const std::wstring& p_Text,
		int p_nX, int p_nY, CColor p_Color, const std::wstring& p_FontName,
		unsigned int p_unFontHeight, unsigned int p_unFontFlags = 0 );

	//! 创建一个 Bitmap ，在上面绘制一行文字
	CBitmap CreateText( const std::wstring& p_Text, const std::wstring& p_FontName,
		unsigned int p_unFontHeight, unsigned int p_unFontFlags = 0 );

	CBitmap CreateText( const std::wstring& p_Text, const std::wstring& p_FontName,
		unsigned int p_unFontHeight, int p_nLineSpacing, unsigned int p_unMaxWidth,
		ENUM_TextAlign p_eAlign, unsigned int p_unFontFlags = 0 );

	void RegisterEntity( const std::wstring& p_Name, const CBitmap& p_Replacement );
}