/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	LargeImageData
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../include/LargeImageData.h"
#include "../include/GraphicsCommon.h"
#include "../include/Bitmap.h"
#include "../include/Graphics.h"
#include "../include/Math.h"
#include "../Include/StringConversion.h"
#include "../Include/LogManager.h"
#include "../Include/Timing.h"
#include <cmath>
//--------------------------------------------------------------------
FK2DEngine::CLargeImageData::CLargeImageData( CGraphics& p_Graphics, const CBitmap& p_Source, 
				unsigned int p_unPartWidth, unsigned int p_unPartHeight, unsigned int p_unBorderFlags )
{
	p_unPartWidth	= p_Source.Width();
	p_unPartHeight	= p_Source.Height();
	m_unPartsX		= Trunc( ceil( 1.0 * p_Source.Width() / m_unPartWidth ));
	m_unPartsY		= Trunc( ceil( 1.0 * p_Source.Height() / m_unPartHeight ));

	m_unPartWidth	= p_unPartWidth;
	m_unPartHeight = p_unPartHeight;

	m_Parts.resize( m_unPartsX * m_unPartsY );
	
	for( unsigned int y = 0; y < m_unPartsY; ++y )
	{
		for( unsigned int x = 0; x < m_unPartsX; ++x )
		{
			unsigned int unSrcWidth = p_unPartWidth;
			if( x == m_unPartsX - 1 && p_Source.Width() % p_unPartWidth != 0 )
			{
				unSrcWidth = p_Source.Width() % p_unPartWidth;
			}
			unsigned int unSrcHeight = p_unPartHeight;
			if( y == m_unPartsY - 1 && p_Source.Height() % p_unPartHeight != 0 )
			{
				unSrcHeight = p_Source.Height() % p_unPartHeight;
			}

			unsigned int unLocalBorderFlags = EBF_Tileable;
			if( x == 0 )
			{
				unLocalBorderFlags = ( unLocalBorderFlags & ~EBF_TileableLeft ) | ( p_unBorderFlags & EBF_TileableLeft );
			}
			if( x == m_unPartsX - 1 )
			{
				unLocalBorderFlags = ( unLocalBorderFlags & ~EBF_TileableRight ) | ( p_unBorderFlags & EBF_TileableRight );
			}
			if( y == 0 )
			{
				unLocalBorderFlags = ( unLocalBorderFlags & ~EBF_TileableTop ) | ( p_unBorderFlags & EBF_TileableTop );
			}
			if( y == m_unPartsY - 1 )
			{
				unLocalBorderFlags = ( unLocalBorderFlags & ~EBF_TileableBottom ) | ( p_unBorderFlags & EBF_TileableBottom );
			}

			m_Parts[ y * m_unPartsX + x ].reset( p_Graphics.CreateImage( p_Source,
				 x * p_unPartWidth, y * p_unPartHeight, unSrcWidth, unSrcHeight, unLocalBorderFlags ).release() );
		}
	}
}
//--------------------------------------------------------------------
unsigned int FK2DEngine::CLargeImageData::Width() const
{
	return m_unFullWidth;
}
//--------------------------------------------------------------------
unsigned int FK2DEngine::CLargeImageData::Height() const
{
	return m_unFullHeight;
}
//--------------------------------------------------------------------
namespace
{
	double Ipl( double p_dA, double p_dB, double p_dRatio )
	{
		return p_dA + ( p_dB - p_dA ) * p_dRatio;
	}

	FK2DEngine::CColor Ipl( FK2DEngine::CColor p_A, FK2DEngine::CColor p_B, double p_dRatio )
	{
		FK2DEngine::CColor Result;
		Result.SetAlpha	( FK2DEngine::Round( Ipl( p_A.Alpha(), p_B.Alpha(), p_dRatio ) ) );
		Result.SetRed	( FK2DEngine::Round( Ipl( p_A.Red(), p_B.Red(), p_dRatio ) ) );
		Result.SetGreen	( FK2DEngine::Round( Ipl( p_A.Green(), p_B.Green(), p_dRatio ) ) );
		Result.SetBlue	( FK2DEngine::Round( Ipl( p_A.Blue(), p_B.Blue(), p_dRatio ) ) );
		return Result;
	}
}
//--------------------------------------------------------------------
FK2DEngine::CBitmap FK2DEngine::CLargeImageData::ToBitmap() const
{
	CBitmap bitmap( Width(), Height() );
	for( unsigned int x = 0; x < m_unPartsX; ++x )
	{
		for( unsigned int y = 0; y < m_unPartsY; ++y )
		{
			bitmap.Insert( m_Parts[ y * m_unPartsY + x ]->ToBitmap(), x * m_unPartWidth, y * m_unPartHeight );
		}
	}
	return bitmap;
}
//--------------------------------------------------------------------
void FK2DEngine::CLargeImageData::Insert(const FK2DEngine::CBitmap &p_Bitmap, int p_nX, int p_nY)
{
	for( unsigned int x = 0; x < m_unPartsX; ++x )
	{
		for ( unsigned int y = 0; y < m_unPartsY; ++y )
		{
			m_Parts[ y * m_unPartsX + x ]->Insert( p_Bitmap, p_nX - x * m_unPartWidth, p_nY - y * m_unPartHeight );
		}
	}
}
//--------------------------------------------------------------------
void FK2DEngine::CLargeImageData::FKDraw( double p_dX1, double p_dY1, CColor p_Color1,
			double p_dX2, double p_dY2, CColor p_Color2, double p_dX3, double p_dY3, CColor p_Color3,
			double p_dX4, double p_dY4, CColor p_Color4, ZPos p_Z, ENUM_AlphaMode p_eMode ) const
{
	if( m_Parts.empty() )
	{
		return;
	}

	ReorderCoordinatesIfNecessary( p_dX1, p_dY1, p_dX2, p_dY2, p_dX3, p_dY3, p_Color3, p_dX4, p_dY4, p_Color4 );

	for( unsigned int pY = 0; pY < m_unPartsY; ++pY )
	{
		for( unsigned int pX = 0; pX < m_unPartsX; ++pX )
		{
			CImageData& part = *m_Parts[ pY * m_unPartsX + pX ];

			double relXL = static_cast<double>(pX * m_unPartWidth) / Width();
			double relXR = static_cast<double>(pX * m_unPartWidth + part.Width()) / Width();
			double relYT = static_cast<double>(pY * m_unPartHeight) / Height();
			double relYB = static_cast<double>(pY * m_unPartHeight + part.Height()) / Height();

			double absXTL = Ipl(Ipl(p_dX1, p_dX3, relYT), Ipl(p_dX2, p_dX4, relYT), relXL);
			double absXTR = Ipl(Ipl(p_dX1, p_dX3, relYT), Ipl(p_dX2, p_dX4, relYT), relXR);
			double absXBL = Ipl(Ipl(p_dX1, p_dX3, relYB), Ipl(p_dX2, p_dX4, relYB), relXL);
			double absXBR = Ipl(Ipl(p_dX1, p_dX3, relYB), Ipl(p_dX2, p_dX4, relYB), relXR);

			double absYTL = Ipl(Ipl(p_dY1, p_dY3, relYT), Ipl(p_dY2, p_dY4, relYT), relXL);
			double absYTR = Ipl(Ipl(p_dY1, p_dY3, relYT), Ipl(p_dY2, p_dY4, relYT), relXR);
			double absYBL = Ipl(Ipl(p_dY1, p_dY3, relYB), Ipl(p_dY2, p_dY4, relYB), relXL);
			double absYBR = Ipl(Ipl(p_dY1, p_dY3, relYB), Ipl(p_dY2, p_dY4, relYB), relXR);

			CColor absCTL = Ipl(Ipl(p_Color1, p_Color3, relYT), Ipl(p_Color2, p_Color4, relYT), relXL);
			CColor absCTR = Ipl(Ipl(p_Color1, p_Color3, relYT), Ipl(p_Color2, p_Color4, relYT), relXR);
			CColor absCBL = Ipl(Ipl(p_Color1, p_Color3, relYB), Ipl(p_Color2, p_Color4, relYB), relXL);
			CColor absCBR = Ipl(Ipl(p_Color1, p_Color3, relYB), Ipl(p_Color2, p_Color4, relYB), relXR);

			part.FKDraw( absXTL, absYTL, absCTL, absXTR, absYTR, absCTR, 
				absXBL, absYBL, absCBL, absXBR, absYBR, absCBR, p_Z, p_eMode );
		}
	}
}
//--------------------------------------------------------------------