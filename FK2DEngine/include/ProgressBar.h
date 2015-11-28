/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	ProgressBar
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
	//! 进度条类
	class CProgressBar : public CWinControl
	{
	public:
		void Create( IGUIControl* p_pParent, int p_nLeft, int p_nTop,
			CBitmap& p_BarBmp, CBitmap& p_BackgroudBmp, CBitmap& p_HeadBmp,
			CBitmap& p_MidBmp, CBitmap& p_TailBmp );
		void SetRange( unsigned int p_unBegin, unsigned int p_unEnd );
		void SetPos( unsigned int p_unPos );
		unsigned int GetMax() const;
		unsigned int GetMin() const;
		unsigned int GetPos() const;
	protected:
		unsigned int	m_unPos;			// 当前位置
		unsigned int	m_unMin;			// 最小位置
		unsigned int	m_unMax;			// 最大位置
		CBitmap*		m_BarBmp;
		CBitmap*		m_BackgroudBmp;
		CBitmap*		m_HeadBmp;
		CBitmap*		m_MidBmp;
		CBitmap*		m_TailBmp;
	};
	//--------------------------------------------------------------------
}