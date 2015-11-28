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

#include "../include/Math.h"
#include "../include/Color.h"
#include <algorithm>

//--------------------------------------------------------------------
#pragma warning( disable : 4244 )
//--------------------------------------------------------------------
namespace
{
	struct HSV
	{
		double		m_dH;
		double		m_dS;
		double		m_dV;
	};

	//--------------------------------------------------------------------
	HSV ColorToHSV( const FK2DEngine::CColor& p_Color )
	{
		double r = p_Color.Red() / 255.0;
		double g = p_Color.Green() / 255.0;
		double b = p_Color.Blue() / 255.0;

		double dMin = std::min( std::min( r, g ), b );
		double dMax = std::max( std::max( r, g ), b );

		if( dMax == 0 )
		{
			HSV hsv = { 0, 0, 0 };
			return hsv;
		}

		// 亮度
		HSV hsv = { -1, -1, dMax };
		// 饱和度
		double dDelta = dMax - dMin;
		hsv.m_dS = dDelta / dMax;
		// 色调
		if( r == dMax )
		{
			hsv.m_dH = ( g - b ) / dDelta + ( g < b ? 6 : 0 );
		}
		else if( g == dMax )
		{
			hsv.m_dH = ( b - r ) / dDelta + 2;
		}
		else
		{
			hsv.m_dH = ( r - g ) / dDelta + 4;
		}
		hsv.m_dH *= 60;

		return hsv;
	}
}
//--------------------------------------------------------------------
FK2DEngine::CColor FK2DEngine::CColor::FromHSV( double p_dH, double p_dS, double p_dV )
{
	return FromAHSV( 255, p_dH, p_dS, p_dV );
}
//--------------------------------------------------------------------
FK2DEngine::CColor FK2DEngine::CColor::FromAHSV( Channel p_ucAlpha, double p_dH, double p_dS, double p_dV )
{
	if( p_dS == 0 )
	{
		// 灰色
		return CColor( p_ucAlpha, static_cast< boost::uint8_t >(p_dV * 255), 
			static_cast< boost::uint8_t >(p_dV * 255), 
			static_cast< boost::uint8_t >(p_dV * 255) );
	}

	// 标准化色调
	p_dH = NormalizeAngle( p_dH );

	int nSector = static_cast< int >( p_dH / 60 );
	double dFactorial = p_dH / 60 - nSector;

	double dP = p_dV * ( 1 - p_dS );
	double dQ = p_dV * ( 1 - p_dS * dFactorial );
	double dT = p_dV * ( 1 - p_dS * ( 1 - dFactorial ) );

	switch( nSector )
	{
	case 0:
		return CColor( p_ucAlpha, p_dV * 255, dT * 255, dP * 255 );
	case 1:
		return CColor( p_ucAlpha, dQ * 255, p_dV * 255, dP * 255 );
	case 2:
		return CColor( p_ucAlpha, dP * 255, p_dV * 255, dT * 255 );
	case 3:
		return CColor( p_ucAlpha, dP * 255, dQ * 255, p_dV * 255 );
	case 4:
		return CColor( p_ucAlpha, dT * 255, dP * 255, p_dV * 255 );
	default:
		return CColor( p_ucAlpha, p_dV * 255, dP * 255, dQ * 255 );
	}
}
//--------------------------------------------------------------------
double FK2DEngine::CColor::Hue() const
{
	return ColorToHSV( *this ).m_dH;
}
//--------------------------------------------------------------------
void FK2DEngine::CColor::SetHue( double p_dH )
{
	*this = FromAHSV( Alpha(), p_dH, Saturation(), Value() );
}
//--------------------------------------------------------------------
double FK2DEngine::CColor::Saturation() const
{
	return ColorToHSV( *this ).m_dS;
}
//--------------------------------------------------------------------
void FK2DEngine::CColor::SetSaturation( double p_dS )
{
	*this = FromAHSV( Alpha(), Hue(), p_dS, Value() );
}
//--------------------------------------------------------------------
double FK2DEngine::CColor::Value() const
{
	return ColorToHSV( *this ).m_dV;
}
//--------------------------------------------------------------------
void FK2DEngine::CColor::SetValue( double p_dV )
{
	*this = FromAHSV( Alpha(), Hue(), Saturation(), p_dV );
}
//--------------------------------------------------------------------
FK2DEngine::CColor FK2DEngine::Interpolate( CColor p_A, CColor p_B, double p_dWeight )
{
	return CColor( Clamp< int >( Round( FK2DEngine::Interpolate( p_A.Alpha(), p_B.Alpha(), p_dWeight )), 0, 255 ), 
		Clamp< int >( Round( FK2DEngine::Interpolate( p_A.Red(), p_B.Red(), p_dWeight )), 0, 255 ),
		Clamp< int >( Round( FK2DEngine::Interpolate( p_A.Green(), p_B.Green(), p_dWeight )), 0, 255 ),
		Clamp< int >( Round( FK2DEngine::Interpolate( p_A.Blue(), p_B.Blue(), p_dWeight )), 0, 255 ) );
}
//--------------------------------------------------------------------
FK2DEngine::CColor FK2DEngine::Multiply( CColor p_A, CColor p_B )
{
	return CColor( Round( p_A.Alpha() * p_B.Alpha() / 255.0 ),
		Round( p_A.Red() * p_B.Red() / 255.0 ),
		Round( p_A.Green() * p_B.Green() / 255.0 ),
		Round( p_A.Blue() * p_B.Blue() / 255.0 ) );
}
//--------------------------------------------------------------------
const FK2DEngine::CColor FK2DEngine::CColor::NONE		= 0x00000000;
const FK2DEngine::CColor FK2DEngine::CColor::BLACK		= 0xff000000;
const FK2DEngine::CColor FK2DEngine::CColor::GRAY		= 0xff808080;
const FK2DEngine::CColor FK2DEngine::CColor::WHITE		= 0xffffffff;
const FK2DEngine::CColor FK2DEngine::CColor::AQUA		= 0xff00ffff;
const FK2DEngine::CColor FK2DEngine::CColor::RED		= 0xffff0000;
const FK2DEngine::CColor FK2DEngine::CColor::GREEN		= 0xff00ff00;
const FK2DEngine::CColor FK2DEngine::CColor::BLUE		= 0xff0000ff;
const FK2DEngine::CColor FK2DEngine::CColor::YELLOW		= 0xffffff00;
const FK2DEngine::CColor FK2DEngine::CColor::FUCHSIA	= 0xffff00ff;
const FK2DEngine::CColor FK2DEngine::CColor::CYAN		= 0xff00ffff;
//--------------------------------------------------------------------