/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Font
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "../include/Font.h"
#include "../include/Graphics.h"
#include "../include/Image.h"
#include "../include/Math.h"
#include "../include/Text.h"
#include "../include/GraphicsCommon.h"
#include "../include/FormattedString.h"
#include "../../depend/boost/array.hpp"
#include "../../depend/boost/shared_ptr.hpp"
#include <map>
//--------------------------------------------------------------------
using namespace std;
//--------------------------------------------------------------------
namespace
{
	bool	IsRtlChar( wchar_t p_Wc )
	{
		return p_Wc == 0x202e;
	}

	bool	IsLtrChar( wchar_t p_Wc )
	{
		return p_Wc == 0x202d;
	}

	bool	IsFormattedChar( wchar_t p_Wc )
	{
		return IsLtrChar( p_Wc ) || IsRtlChar( p_Wc );
	}
}
//--------------------------------------------------------------------
struct FK2DEngine::CFont::SImpl
{
	typedef boost::array< boost::scoped_ptr< CImage >, 65536 >		CharChunk;
public:
	CGraphics*		m_pGraphics;
	wstring			m_szName;
	unsigned int	m_unHeight;
	unsigned int	m_unFlags;
	boost::scoped_ptr< CharChunk >		m_pChunks[65536][EFF_Combinations];
	std::map< std::wstring, boost::shared_ptr< CImage > >	m_EntityCache;
public:
	const CImage& ImageAt( const CFormattedString& p_FS, unsigned int p_unI )
	{
		if( p_FS.EntityAt(p_unI) )
		{
			boost::shared_ptr< CImage >& ptr = m_EntityCache[ p_FS.EntityAt(p_unI) ];
			if( !ptr )
			{
				ptr.reset( new CImage( *m_pGraphics, EntityBitmap( p_FS.EntityAt(p_unI)), false ) );
			}
			return *ptr;
		}

		wchar_t wc = p_FS.CharAt( p_unI );
		unsigned int unFlags = p_FS.FlagAt( p_unI );

		assert( m_unFlags < EFF_Combinations );

		size_t unChunkIndex = wc / 65536;
		size_t unCharIndex = wc % 65536;

		if( !m_pChunks[unChunkIndex][unFlags] )
		{
			m_pChunks[unChunkIndex][unFlags].reset( new CharChunk );
		}

		boost::scoped_ptr< CImage >& pImgPtr = ( *m_pChunks[unChunkIndex][unFlags] )[unCharIndex];

		if( pImgPtr )
		{
			return *pImgPtr;
		}

		wstring szCharString( 1, wc );
		if( IsFormattedChar( wc ) )
		{
			szCharString.clear();
		}

		unsigned int unCharWidth = FK2DEngine::TextWidth( szCharString, m_szName, m_unHeight, unFlags );

		CBitmap bmp( unCharWidth, m_unHeight );
		FKDrawText( bmp, szCharString, 0, 0, CColor::WHITE, m_szName, m_unHeight, unFlags );
		pImgPtr.reset( new CImage( *m_pGraphics, bmp ) );
		return *pImgPtr;
	}

	double FactorAt( const CFormattedString& p_FS, unsigned int p_unIndex )const
	{
		if( p_FS.EntityAt( p_unIndex ) )
		{
			return 1;
		}
		else
		{
			return 0.5;
		}
	}
};
//--------------------------------------------------------------------
FK2DEngine::CFont::CFont(FK2DEngine::CGraphics &p_Graphics, const std::wstring &p_FontName,
	unsigned int p_unFontHeight, unsigned int p_unFontFlags /*= EFF_Bold*/)
	: m_pImpl( new SImpl )
{
	m_pImpl->m_pGraphics	= &p_Graphics;
	m_pImpl->m_szName		= p_FontName;
	m_pImpl->m_unHeight		= p_unFontHeight * 2;
	m_pImpl->m_unFlags		= p_unFontFlags;
}
//--------------------------------------------------------------------
std::wstring FK2DEngine::CFont::Name() const
{
	return m_pImpl->m_szName;
}
//--------------------------------------------------------------------
unsigned int FK2DEngine::CFont::Height() const
{
	return m_pImpl->m_unHeight / 2;
}
//--------------------------------------------------------------------
unsigned int FK2DEngine::CFont::Flags() const
{
	return m_pImpl->m_unFlags;
}
//--------------------------------------------------------------------
double FK2DEngine::CFont::TextWidth(const std::wstring &p_szText,
								  double p_dFactorX /*= 1.000000*/) const
{
	CFormattedString fs( p_szText, Flags() );
	double dResult = 0;
	for( unsigned int i = 0; i < fs.Length(); ++i )
	{
		dResult += m_pImpl->ImageAt( fs, i ).Width() * m_pImpl->FactorAt( fs, i );
	}
	return dResult * p_dFactorX;
}
//--------------------------------------------------------------------
void FK2DEngine::CFont::FKDraw(const std::wstring &p_szText, double p_dX, double p_dY,
	FK2DEngine::ZPos p_Z, double p_dFactorX /*= 1.000000*/, 
	double p_dFactorY /*= 1.000000*/, FK2DEngine::CColor p_Color /*= CColor::WHITE*/,
	FK2DEngine::ENUM_AlphaMode p_eMode /*= EAM_Default*/) const
{
	CFormattedString fs( p_szText, Flags() );

	for( unsigned int i = 0; i < fs.Length(); ++i )
	{
		const CImage& CurChar = m_pImpl->ImageAt( fs, i );
		double dCurFactor;
		FK2DEngine::CColor Color;
		if( fs.EntityAt( i ) )
		{
			dCurFactor = 1.0;
			Color = FK2DEngine::CColor( fs.ColorAt(i).Alpha() * p_Color.Alpha() / 255, 255, 255, 255 );
		}
		else
		{
			dCurFactor = 0.5;
			Color = FK2DEngine::Multiply( fs.ColorAt(i), p_Color );
		}

		CurChar.FKDraw( p_dX, p_dY, p_Z, p_dFactorX * dCurFactor, p_dFactorY * dCurFactor,
			Color, p_eMode );

		p_dX += CurChar.Width() * p_dFactorX * dCurFactor;
	}
}
//--------------------------------------------------------------------
void FK2DEngine::CFont::DrawRel(const std::wstring &p_szText, double p_dX, 
		double p_dY, FK2DEngine::ZPos p_Z, double p_dRelX, double p_dRelY,
		double p_dFactorX /*= 1.000000*/, double p_dFactorY /*= 1.000000*/,
		FK2DEngine::CColor p_Color /*= CColor::WHITE*/, 
		FK2DEngine::ENUM_AlphaMode p_eMode /*= EAM_Default*/) const
{
	p_dX -= TextWidth( p_szText ) * p_dFactorX * p_dRelX;
	p_dY -= Height() * p_dFactorY * p_dRelY;

	FKDraw( p_szText, p_dX, p_dY, p_Z, p_dFactorX, p_dFactorY, p_Color, p_eMode );
}
//--------------------------------------------------------------------
void FK2DEngine::CFont::DrawDot(const std::wstring &p_szText, double p_dX,
		double p_dY, FK2DEngine::ZPos p_Z, double p_dAngle,
		double p_dFactorX /*= 1.000000*/, double p_dFactorY /*= 1.000000*/,
		FK2DEngine::CColor p_Color /*= CColor::WHITE*/,
		FK2DEngine::ENUM_AlphaMode p_eMode /*= EAM_Default*/) const
{
	m_pImpl->m_pGraphics->PushTransform( Rotate(p_dAngle, p_dX, p_dY) );
	FKDraw( p_szText, p_dX, p_dY, p_Z, p_dFactorX, p_dFactorY, p_Color, p_eMode );
	m_pImpl->m_pGraphics->PopTransform();
}
//--------------------------------------------------------------------