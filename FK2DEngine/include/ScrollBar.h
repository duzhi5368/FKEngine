/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	ScrollBar
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
#include "UIManager.h"
#include "Image.h"
#include <vector>
//--------------------------------------------------------------------
namespace FK2DEngine
{
	//--------------------------------------------------------------------
	//! 滚动条类型
	enum ENUM_ScrollBarType
	{
		ESBT_Horizontal,		// 水平
		ESBT_Vertical,			// 垂直
	};
	//--------------------------------------------------------------------
	//! 滚动条
	class CScrollBar : public IGUIControl
	{
	public:
		CScrollBar();
		virtual ~CScrollBar(){}
	public:
		void Create(  IGUIControl* p_pParent, int p_nLeft, int p_nTop,
			ENUM_ScrollBarType p_eType, CBitmap& p_Bitmap, int p_nLength );
		virtual void FKDraw() const;

	public:
		//! 设置滑块图片
		void SetSlider( CBitmap& p_SliderBitmap );
		//! 设置滚动条长度
		void SetLength( int p_nLength );
		//! 获取滑块位置
		int GetPosition() const;
		//! 设置滑块位置
		void SetPosition( int p_nPos );
		//! 设置滚动条类型
		void SetType( ENUM_ScrollBarType p_eType );
		//! 设置滑块位置范围
		void SetRange( int p_nMin, int p_nMax );

	public:
		//! 改写的事件处理函数
		virtual void MouseMove( POINT p_Point );
		virtual void MouseDown( POINT p_Point, ENUM_MouseButton p_eMB );
		virtual void MouseUp( POINT p_Point, ENUM_MouseButton p_eMB );

	public:
		//! 向用户提供的对外事件处理
		SNotifyEvent OnChange;
		SNotifyEvent OnScroll;

	private:
		ENUM_ScrollBarType				m_eType;		// 滚动条类型
		CBitmap*						m_pSlider;		// 滑块图片指针
		int								m_nOffset;		// 鼠标在滑块上的偏移
		int								m_nSliderPos;	// 滑块位置
		int								m_nMin;			// 滑块位置最小值
		int								m_nMax;			// 滑块位置最大值
		bool							m_bHolding;		// 鼠标是否持有滑块
		std::vector< CImage >			m_vecImages;	// 鼠标图片组
	};
	//--------------------------------------------------------------------
}