/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	GraphicsCommon
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "Bitmap.h"
#include "Graphics.h"
#include "Platform.h"
#include <algorithm>
#include <list>
#include <vector>
//--------------------------------------------------------------------
#if defined( FK_WINDOWS )
	#ifndef NOMINMAX
		#define NOMINMAX
	#endif

	#include <Windows.h>
	#include <GL/GL.h>
#elif defined( FK_IPHONE )
	#include <OpenGLES/ES1/gl.h>
	#include <OpenGLES/ES1/glext.h>
#elif defined( FK_MAC )
	#include <OpenGL/gl.h>
#else
	#include <GL/gl.h>
#endif
//--------------------------------------------------------------------
namespace FK2DEngine
{
	class CTexture;
	class CTexChunk;
	class CRenderState;
	struct SDrawOp;
	class CDrawOpQueue;

	typedef std::list< Transform >			Transforms;
	typedef std::vector< CDrawOpQueue >		DrawOpQueueStack;
	class CMacro;

	const GLuint			NO_TEXTURE	= static_cast< GLuint >( -1 );
	const unsigned int		NO_CLIPPING = 0xffffffff;

	template< typename T >
	bool IsPToTheLeftOfAB( T xa, T ya, T xb, T yb, T xp, T yp)
	{
		return (xb - xa) * (yp - ya) - (xp - xa) * (yb - ya) > 0;
	}

	template< typename T, typename C >
	void ReorderCoordinatesIfNecessary(T& x1, T& y1, T& x2, T& y2, T& x3, T& y3, C& c3, T& x4, T& y4, C& c4)
	{
		if ( IsPToTheLeftOfAB(x1, y1, x2, y2, x3, y3) ==
			 IsPToTheLeftOfAB(x2, y2, x3, y3, x4, y4))
		{
			std::swap(x3, x4);
			std::swap(y3, y4);
			std::swap(c3, c4);
		}
	}

	inline Transform MutliplyTransform( const Transform& p_Left, const Transform& p_Right )
	{
		FK2DEngine::Transform		Result;
		Result.assign( 0 );
		for( int i = 0; i < 16; ++i )
		{
			for( int j = 0; j < 16; ++j )
			{
				Result[i] += p_Left[ i / 4 * 4 + j ] * p_Right[ i % 4 + j * 4 ];
			}
		}
		return Result;
	}

	inline void ApplyTransform( const Transform& p_Transform, double& p_dX, double& p_dY )
	{
		double in[4] = { p_dX, p_dY, 0, 1 };
		double out[4] = { 0, 0, 0, 0 };
		for( int i = 0; i < 4; ++i )
		{
			for( int j = 0; j < 4; ++j )
			{
				out[i] += in[j] * p_Transform[ j * 4 + i ];
			}
		}

		p_dX = out[0] / out[3];
		p_dY = out[1] / out[3];
	}

	inline void MultiplyBitmapAlpha( CBitmap& p_Bitmap, CColor::Channel p_Alpha )
	{
		for( unsigned int y = 0; y < p_Bitmap.Height(); ++y )
		{
			for( unsigned int x = 0; x < p_Bitmap.Width(); ++x )
			{
				CColor pColor = p_Bitmap.GetPixel( x, y );
				pColor.SetAlpha( pColor.Alpha() * p_Alpha / 255 );
				p_Bitmap.SetPixel( x, y, pColor );
			}
		}
	}

#ifdef FK_IPHONE
	int ClipRectBaseFactor();
#else
	inline int ClipRectBaseFactor()
	{
		return 1;
	}
#endif

	bool IsEntity( const std::wstring& p_Name );
	const CBitmap& EntityBitmap( const std::wstring& p_Name );
}