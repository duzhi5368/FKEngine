/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	PicButton
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "UIControl.h"
#include "Image.h"
#include <vector>
//--------------------------------------------------------------------
namespace FK2DEngine
{
	//--------------------------------------------------------------------
	//! 图片按钮类
	class CPicButton : public IGUIControl
	{
	public:
		//! 枚举按钮状态
		enum ENUM_ButtonState
		{
			eBS_Up,
			eBS_Hot,
			eBS_Down,
			eBS_Disabled,
			eBS_MAX,
		};

	public:
		CPicButton();
		virtual ~CPicButton(){}
	public:
		virtual void FKDraw() const = 0;

		//! 设置按钮图片
		//@note 要求一行四列，分别为Up,Hot,Down,Disabled时的图片
		void SetGlygh( CBitmap& p_Bitmap );

		//! 设置按钮按下时图片的偏移
		void SetDownOffset( int p_nX, int p_nY );

		//! 鼠标进入
		virtual void MouseEnter();
		//! 鼠标离开
		virtual void MouseLeave();
		//! 鼠标按下
		virtual void MouseDown( POINT p_Point, ENUM_MouseButton p_eMB );
		//! 鼠标弹起
		virtual void MouseUp( POINT p_Point, ENUM_MouseButton p_eMB );

	public:
		//! 向用户级提供的OnClick事件处理
		SNotifyEvent			OnClick;

	protected:
		CBitmap*			m_pBitmap;		// 按钮图片
		std::vector<CImage>	m_vecImage;		// 图片单元
		ENUM_ButtonState	m_eBS;			// 按钮状态
		POINT				m_DownOffset;	// 按钮按下时的图片偏移
		bool				m_bHolding;		// 鼠标是否持有按钮
		bool				m_bHovering;	// 鼠标是否在按钮上方
	};
	//--------------------------------------------------------------------
}