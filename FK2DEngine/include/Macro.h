/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Macro
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "InsidePreInclude.h"
#include "ImageData.h"
#include "GraphicsCommon.h"
#include "DrawOpQueue.h"
#include "Timing.h"
#include "LogManager.h"
#include "StringConversion.h"
#include "../../depend/boost/bind.hpp"
#include "../../depend/boost/foreach.hpp"
#include "../../depend/boost/scoped_ptr.hpp"
#include <cmath>
#include <algorithm>
//--------------------------------------------------------------------
class FK2DEngine::CMacro : public FK2DEngine::CImageData
{
private:
	VectexArray						m_VertexArray;
	unsigned int					m_unW;
	unsigned int					m_unH;
	CGraphics&						m_Graphics;
public:
	CMacro( CGraphics& p_Graphics, CDrawOpQueue& p_Queue )
		: m_Graphics( p_Graphics )
	{
		p_Queue.CompileTo( m_VertexArray );
		double dLeft = 0.0;
		double dRight = 0.0;
		double dTop = 0.0;
		double dBottom = 0.0;

		BOOST_FOREACH( const SArrayVertex& av, m_VertexArray )
		{
			dLeft	= std::min< double >( dLeft, av.m_fVectices[0] );
			dRight	= std::max< double >( dRight, av.m_fVectices[0] );
			dTop	= std::min< double >( dTop, av.m_fVectices[1] );
			dBottom	= std::max< double >( dBottom, av.m_fVectices[1] );
		}

		m_unW = static_cast< unsigned int >( std::ceil( dRight - dLeft ));
		m_unH = static_cast< unsigned int >( std::ceil( dBottom - dTop ));
	}

	unsigned int Width() const
	{
		return m_unW;
	}

	unsigned int Height() const
	{
		return m_unH;
	}

	void FKDraw( double p_dX1, double p_dY1, CColor p_Color1,
		double p_dX2, double p_dY2, CColor p_Color2,
		double p_dX3, double p_dY3, CColor p_Color3,
		double p_dX4, double p_dY4, CColor p_Color4,
		ZPos p_Z, ENUM_AlphaMode p_eMode ) const
	{
		boost::function< void() > f = boost::bind( &CMacro::RealDraw,
			this, p_dX1, p_dY1, p_dX2, p_dY2, p_dX3, p_dY3 );
		m_Graphics.ScheduleGL( f, p_Z );
	}

	void RealDraw( double p_dX1, double p_dY1, double p_dX2,
		double p_dY2, double p_dX3, double p_dY3 ) const
	{
#ifndef FK_IPHONE
		glEnable( GL_BLEND );
		CRenderState	rs;
		rs.SetTexName( 1 );
		rs.SetAlphaMode( EAM_Default );
		
		glMatrixMode( GL_MODELVIEW );
		glTranslated( p_dX1, p_dY1, 0 );
		glScaled( ( p_dX2 - p_dX1 ) / Width(), ( p_dY3 - p_dY1 ) / Height(), 1 );

		glInterleavedArrays( GL_T2F_C4UB_V3F, 0, &m_VertexArray[0] );

		glDrawArrays( GL_QUADS, 0, m_VertexArray.size() );
		glFlush();
#endif
	}

	boost::optional< FK2DEngine::SGLTexInfo >	GLTexInfo() const
	{
		return boost::none;
	}

	FK2DEngine::CBitmap ToBitmap() const
	{
		throw std::logic_error( "Macro类型不允许渲染为Bitmap类型" );
	}
	
	void Insert( const CBitmap& p_Bitmap, int p_nX, int p_nY )
	{
		throw std::logic_error( "Macro类型不允许转换为Bitmap类型" );
	}
};