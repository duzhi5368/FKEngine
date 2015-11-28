/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Bitmap
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../include/Bitmap.h"
#include <cassert>
#include <algorithm>
#include <vector>

//--------------------------------------------------------------------
void FK2DEngine::CBitmap::Swap( CBitmap& p_Other )
{
	std::swap( m_vecPixels, p_Other.m_vecPixels );
	std::swap( m_unWidth, p_Other.m_unWidth );
	std::swap( m_unHeight, p_Other.m_unHeight );
}
//--------------------------------------------------------------------
void FK2DEngine::CBitmap::Resize( unsigned int p_unWidth, 
			unsigned int p_unHeight, CColor p_Color /*= CColor::NONE*/ )
{
	if(( p_unWidth == m_unWidth ) && ( p_unHeight == m_unHeight ))
	{
		return;
	}
	CBitmap temp( p_unWidth, p_unHeight, p_Color );
	temp.Insert( *this, 0, 0 );
	Swap( temp );
}
//--------------------------------------------------------------------
void FK2DEngine::CBitmap::Fill( CColor p_Color )
{
	std::fill( m_vecPixels.begin(), m_vecPixels.end(), p_Color );
}
//--------------------------------------------------------------------
void FK2DEngine::CBitmap::Replace( CColor p_OldColor, CColor p_NewColor )
{
	std::replace( m_vecPixels.begin(), m_vecPixels.end(), p_OldColor, p_NewColor );
}
//--------------------------------------------------------------------
//! 在指定位置添加一个图片，若图片出了区域，则被裁减
void FK2DEngine::CBitmap::Insert( const CBitmap& p_Source, int p_nX, int p_nY )
{
	Insert( p_Source, p_nX, p_nY, 0, 0, p_Source.Width(), p_Source.Height() );
}
//--------------------------------------------------------------------
void FK2DEngine::CBitmap::Insert( const CBitmap& p_Source, int p_nX, int p_nY, unsigned int p_unSrcX,
			unsigned int p_unSrcY, unsigned int p_unSrcWidth, unsigned int p_unSrcHeight )
{
	if( p_nX < 0 )
	{
		unsigned int unClipLeft = -p_nX;

		if( unClipLeft >= p_unSrcWidth )
		{
			return;
		}

		p_unSrcX += unClipLeft;
		p_unSrcWidth -= unClipLeft;
		p_nX = 0;
	}

	if( p_nY < 0 )
	{
		unsigned int unClipTop = -p_nY;

		if( unClipTop >= p_unSrcHeight )
		{
			return;
		}

		p_unSrcY += unClipTop;
		p_unSrcHeight -= unClipTop;
		p_nY = 0;
	}

	if( p_nX + p_unSrcWidth > m_unWidth )
	{
		if( static_cast< unsigned int >( p_nX ) >= m_unWidth )
		{
			return ;
		}

		p_unSrcWidth = m_unWidth - p_nX;
	}

	if( p_nY + p_unSrcHeight > m_unHeight )
	{
		if( static_cast< unsigned int >( p_nY ) >= m_unHeight )
		{
			return ;
		}

		p_unSrcHeight = m_unHeight - p_nY;
	}

	for( unsigned int relY = 0; relY < p_unSrcHeight; ++relY )
	{
		for( unsigned int relX = 0; relX < p_unSrcWidth; ++relX )
		{
			SetPixel( p_nX + relX, p_nY + relY, 
				p_Source.GetPixel( p_unSrcX + relX , p_unSrcY + relY ) );
		}
	}
}
//--------------------------------------------------------------------