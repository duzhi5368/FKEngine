/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Color
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "Platform.h"
#include <boost/cstdint.hpp>
//--------------------------------------------------------------------
namespace FK2DEngine
{
	class CColor
	{
	private:
		boost::uint32_t				m_unRep;
		#ifdef FK_LITTLE_ENDIAN
				enum { RED_OFFSET = 0, GREEN_OFFSET = 8, BLUE_OFFSET = 16, ALPHA_OFFSET = 24 };
		#else
				enum { RED_OFFSET = 24, GREEN_OFFSET = 16, BLUE_OFFSET = 8, ALPHA_OFFSET = 0 };
		#endif
	public:
		typedef boost::uint8_t		Channel;
		static const unsigned int	GL_FORMAT		= 0x1908;
	public:
		CColor()
		{

		}
		CColor( boost::uint32_t p_unArgb )
		{
			*this = CColor(	(p_unArgb >> 24) & 0xff, (p_unArgb >> 16) & 0xff,
							(p_unArgb >>  8) & 0xff, (p_unArgb >>  0) & 0xff );
		}
		CColor( Channel p_ucRed, Channel p_ucGreen, Channel p_ucBlue )
		{
			*this = CColor( 0xff, p_ucRed, p_ucGreen, p_ucBlue );
		}
		CColor( Channel p_ucAlpha, Channel p_ucRed, Channel p_ucGreen, Channel p_ucBlue )
		{
			m_unRep =(	(p_ucAlpha << ALPHA_OFFSET) | (p_ucRed << RED_OFFSET) |
						(p_ucGreen << GREEN_OFFSET) | (p_ucBlue << BLUE_OFFSET) );
		}

		//! 将HSV格式转换为ARGB格式。
		//! \params p_dH 色调	取值范围 0 - 360
		//! \params p_dS 饱和度	取值范围 0 - 1
		//! \params p_dV 亮度	取值范围 0 - 1
		static CColor FromHSV( double p_dH, double p_dS, double p_dV );
		static CColor FromAHSV( Channel p_ucAlpha, double p_dH, double p_dS, double p_dV );

		Channel Red() const
		{
			return static_cast< Channel >( m_unRep >> RED_OFFSET );
		}
		Channel Green() const
		{
			return static_cast< Channel >( m_unRep >> GREEN_OFFSET );
		}
		Channel Blue() const
		{
			return static_cast< Channel >( m_unRep >> BLUE_OFFSET );
		}
		Channel Alpha() const
		{
			return static_cast< Channel >( m_unRep >> ALPHA_OFFSET );
		}

		void SetRed( Channel p_ucValue )
		{
			m_unRep &= ~(0xff << RED_OFFSET);
			m_unRep |= p_ucValue << RED_OFFSET;
		}
		void SetGreen( Channel p_ucValue )
		{
			m_unRep &= ~(0xff << GREEN_OFFSET);
			m_unRep |= p_ucValue << GREEN_OFFSET;
		}
		void SetBlue( Channel p_ucValue )
		{
			m_unRep &= ~(0xff << BLUE_OFFSET);
			m_unRep |= p_ucValue << BLUE_OFFSET;
		}
		void SetAlpha( Channel p_ucValue )
		{
			m_unRep &= ~(0xff << ALPHA_OFFSET);
			m_unRep |= p_ucValue << ALPHA_OFFSET;
		}

		//! 返回色调值	取值范围 0 - 360
		double	Hue() const;
		void	SetHue( double p_dH );
		//! 返回饱和度	取值范围 0 - 1
		double	Saturation() const;
		void	SetSaturation( double p_dS );
		//! 返回亮度	取值范围 0 - 1
		double	Value() const;
		void	SetValue( double p_dV );

		//! 返回 0xaarrggbb 格式
		boost::uint32_t Argb() const
		{
			return Alpha() << 24 | Red() << 16 | Green() << 8 | Blue();
		}
		//! 返回 0x00bbggrr 格式
		boost::uint32_t Bgr() const
		{
			return Blue() << 16 | Green() << 8 | Red();
		}
		//! 返回 0xaabbggrr 格式
		boost::uint32_t Abgr() const
		{
			return Alpha() << 24 | Blue() << 16 | Green() << 8 | Red();
		}
		//! 返回在内存中的颜色表现格式( RGBA )
		boost::uint32_t Gl() const
		{
			return m_unRep;
		}

		static const CColor NONE;
		static const CColor BLACK;
		static const CColor GRAY;
		static const CColor WHITE;

		static const CColor AQUA;
		static const CColor RED;
		static const CColor GREEN;
		static const CColor BLUE;
		static const CColor YELLOW;
		static const CColor FUCHSIA;
		static const CColor CYAN;
	};

	#ifndef SWIG
		inline bool operator< ( CColor p_A, CColor p_B )
		{
			return p_A.Gl() < p_B.Gl();
		}
		inline bool operator== ( CColor p_A, CColor p_B )
		{
			return p_A.Gl() == p_B.Gl();
		}
		inline bool operator!= ( CColor p_A, CColor p_B )
		{
			return p_A.Gl() != p_B.Gl();
		}
	#endif

	//! 根据权重，取两个颜色值得中间值
	CColor Interpolate( CColor p_A, CColor p_B, double p_dWeight = 0.5 );
	//! 混合两个颜色（最终保证其所有通道颜色值在 0 - 1 内）
	CColor Multiply( CColor p_A, CColor p_B );

	namespace Colors
	{
		const CColor none    = 0x00000000;
		const CColor black   = 0xff000000;
		const CColor gray    = 0xff808080;
		const CColor white   = 0xffffffff;

		const CColor aqua    = 0xff00ffff;
		const CColor red     = 0xffff0000;
		const CColor green   = 0xff00ff00;
		const CColor blue    = 0xff0000ff;
		const CColor yellow  = 0xffffff00;
		const CColor fuchsia = 0xffff00ff;
		const CColor cyan    = 0xff00ffff;
	}
}