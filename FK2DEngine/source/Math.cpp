/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Math
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../include/Math.h"
#include <cmath>
#include <cstdlib>

//--------------------------------------------------------------------
double FK2DEngine::Random( double p_dMin, double p_dMax )
{
	double dRandom = std::rand();
	return dRandom / ( static_cast<double>( RAND_MAX ) + 1 ) * ( p_dMax - p_dMin ) + p_dMin;
}
//--------------------------------------------------------------------
double FK2DEngine::OffsetX( double p_dAngle, double p_dRadius )
{
	return +std::sin( p_dAngle / 180 * PI ) * p_dRadius;
}
//--------------------------------------------------------------------
double FK2DEngine::OffsetY( double p_dAngle, double p_dRadius )
{
	return +std::cos( p_dAngle / 180 * PI ) * p_dRadius;
}
//--------------------------------------------------------------------
double FK2DEngine::Angle( double p_dFromX, double p_dFromY, double p_dToX, 
			 double p_dToY, double p_dDefaultValue )
{
	double dDisX = p_dToX - p_dFromX;
	double dDisY = p_dToY - p_dFromY;

	if(( dDisX == 0 ) && ( dDisY == 0 ))
	{
		return p_dDefaultValue;
	}
	else
	{
		return NormalizeAngle( RadiansToFKAngle( std::atan2( dDisY, dDisX )));
	}
}
//--------------------------------------------------------------------
double FK2DEngine::AngleDiff( double p_dFrom, double p_dTo )
{
	return NormalizeAngle( p_dTo - p_dFrom + 180 ) - 180;
}
//--------------------------------------------------------------------
double FK2DEngine::NormalizeAngle( double p_dAngle )
{
	return Wrap( p_dAngle, 0.0, 360.0 );
}
//--------------------------------------------------------------------
int FK2DEngine::Wrap( int p_nValue, int p_nMin, int p_nMax )
{
	int nResult = ( p_nValue - p_nMin ) % ( p_nMax - p_nMin );
	if( nResult < 0 )
	{
		return nResult + p_nMax;
	}
	else 
	{
		return nResult + p_nMin;
	}
}
//--------------------------------------------------------------------
float FK2DEngine::Wrap( float p_fValue, float p_fMin, float p_fMax )
{
	double dResult = std::fmod( p_fValue - p_fMin, p_fMax - p_fMin );
	if( dResult < 0 )
	{
		return static_cast< float >( dResult + p_fMax );
	}
	else
	{
		return static_cast< float >( dResult + p_fMin );
	}
}
//--------------------------------------------------------------------
double FK2DEngine::Wrap( double p_dValue, double p_dMin, double p_dMax )
{
	double dResult = std::fmod( p_dValue - p_dMin, p_dMax - p_dMin );
	if( dResult < 0 )
	{
		return dResult + p_dMax;
	}
	else
	{
		return dResult + p_dMin;
	}
}
//--------------------------------------------------------------------
double FK2DEngine::DistanceSqr( double p_dX1, double p_dY1, double p_dX2, double p_dY2 )
{
	return Square( p_dX1 - p_dX2 ) + Square( p_dY1 - p_dY2 );
}
//--------------------------------------------------------------------
double FK2DEngine::Distance( double p_dX1, double p_dY1, double p_dX2, double p_dY2 )
{
	return std::sqrt( DistanceSqr( p_dX1, p_dY1, p_dX2, p_dY2 ) );
}
//--------------------------------------------------------------------