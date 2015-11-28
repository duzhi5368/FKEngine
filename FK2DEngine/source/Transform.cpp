/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Transform
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "../include/Graphics.h"
#include "../include/Math.h"
#include "../include/GraphicsCommon.h"
#include <cmath>
//--------------------------------------------------------------------
FK2DEngine::Transform FK2DEngine::Rotate( double p_dAngle, double p_dAroundX /* = 0 */, double p_dAroundY /* = 0 */ )
{
	double c = std::cos( DegreesToRadians( p_dAngle ) );
	double s = std::sin( DegreesToRadians( p_dAngle ) );
	FK2DEngine::Transform Result = 
	{
		+c, +s, 0, 0,
		-s, +c, 0, 0,
		0,  0,  1, 0,
		0,  0,  0, 1
	};
	if( p_dAroundX != 0 || p_dAroundY != 0 )
	{
		Result = MutliplyTransform( MutliplyTransform( Translate( -p_dAroundX, -p_dAroundY ), Result ),
			Translate( p_dAroundX, p_dAroundY ) );
	}
	return Result;
}
//--------------------------------------------------------------------
FK2DEngine::Transform FK2DEngine::Translate( double p_dX, double p_dY )
{
	FK2DEngine::Transform Result = 
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		p_dX, p_dY, 0, 1
	};

	return Result;
}
//--------------------------------------------------------------------
FK2DEngine::Transform FK2DEngine::Scale( double p_dFactor )
{
	FK2DEngine::Transform Result = 
	{
		p_dFactor,	0,			0, 0,
		0,			p_dFactor,	0, 0,
		0,			0,			1, 0,
		0,			0,			0, 1
	};

	return Result;
}
//--------------------------------------------------------------------
FK2DEngine::Transform FK2DEngine::Scale( double p_dFactorX, double p_dFactorY,
					double p_dAroundX /* = 0.0 */, double p_dAroundY /* = 0.0 */ )
{
	FK2DEngine::Transform Result = 
	{
		p_dFactorX,	0,			0, 0,
		0,			p_dFactorY,	0, 0,
		0,			0,			1, 0,
		0,			0,			0, 1
	};

	if( p_dAroundX != 0 || p_dAroundY != 0 )
	{
		Result = MutliplyTransform( MutliplyTransform( Translate( -p_dAroundX, -p_dAroundY ), Result ),
			Translate( p_dAroundX, p_dAroundY ) );
	}
	return Result;
}
//--------------------------------------------------------------------