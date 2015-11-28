/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Panel
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
	//! 面板类
	class CPanel : public CWinControl
	{
	public:
		void Create( IGUIControl* p_pParent, int p_nLeft, int p_nTop,
			CBitmap& p_Bitmap );
		void Create( IGUIControl* p_pParent, int p_nLeft, int p_nTop,
			int p_nWidth, int p_nHeight );
	};
	//--------------------------------------------------------------------
}