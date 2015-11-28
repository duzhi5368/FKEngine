/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	TexChunk
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../include/TexChunk.h"
#include "../include/Texture.h"
#include "../include/DrawOpQueue.h"
#include "../include/Bitmap.h"
#include "../include/Graphics.h"
#include "../include/Timing.h"
#include "../include/StringConversion.h"
#include "../include/LogManager.h"
//--------------------------------------------------------------------
#pragma warning( disable:4244 )
//--------------------------------------------------------------------
FK2DEngine::CTexChunk::CTexChunk( CGraphics& p_Graphics, Transforms& p_Transforms, DrawOpQueueStack& p_Queues,
		  boost::shared_ptr< CTexture > p_pTexture, int p_nX, int p_nY, int p_nW, int p_nH, int p_nPadding )
		  : m_pGraphics( &p_Graphics )
		  , m_pTransforms( &p_Transforms )
		  ,	m_pQueues( &p_Queues )
		  , m_pTexture( p_pTexture )
		  , m_nX( p_nX )
		  , m_nY( p_nY )
		  , m_nW( p_nW )
		  , m_nH( p_nH )
		  ,	m_nPadding( p_nPadding ) 
{
	m_Info.m_nTexName		= p_pTexture->TexName();
	m_Info.m_fLeft			= float( m_nX ) / p_pTexture->Size();
	m_Info.m_fTop			= float( m_nY ) / p_pTexture->Size();
	m_Info.m_fRight			= float( m_nX + m_nW ) / p_pTexture->Size();
	m_Info.m_fBottom		= float( m_nY + m_nH ) / p_pTexture->Size();
}
//--------------------------------------------------------------------
FK2DEngine::CTexChunk::~CTexChunk()
{
	m_pTexture->Free( m_nX - m_nPadding, m_nY - m_nPadding );
}
//--------------------------------------------------------------------
void FK2DEngine::CTexChunk::FKDraw( double x1, double y1, CColor c1,
			double x2, double y2, CColor c2,
			double x3, double y3, CColor c3,
			double x4, double y4, CColor c4,
			ZPos p_Z, ENUM_AlphaMode p_eMode )const
{
	SDrawOp newOp( m_pTransforms->back() );
	ReorderCoordinatesIfNecessary( x1, y1, x2, y2, x3, y3, c3, x4, y4, c4 );

	newOp.m_unUsedVertices	= 4;
	newOp.m_Vertices[0]		= SDrawOp::SVertex( x1, y1, c1 );
	newOp.m_Vertices[1]		= SDrawOp::SVertex( x2, y2, c2 );

#ifdef FK_IPHONE
	newOp.m_Vertices[2]		= SDrawOp::SVertex( x3, y3, c3 );
	newOp.m_Vertices[3]		= SDrawOp::SVertex( x4, y4, c4 );
#else
	newOp.m_Vertices[3]		= SDrawOp::SVertex( x3, y3, c3 );
	newOp.m_Vertices[2]		= SDrawOp::SVertex( x4, y4, c4 );
#endif

	newOp.m_pChunk			= this;
	newOp.m_eMode			= p_eMode;

	m_pQueues->back().ScheduleDrawOp( newOp, p_Z );
}
//--------------------------------------------------------------------
boost::optional< FK2DEngine::SGLTexInfo > FK2DEngine::CTexChunk::GLTexInfo() const
{
	return m_Info;
}
//--------------------------------------------------------------------
FK2DEngine::CBitmap FK2DEngine::CTexChunk::ToBitmap() const
{
	return m_pTexture->ToBitmap( m_nX, m_nY, m_nW, m_nH );
}
//--------------------------------------------------------------------
void FK2DEngine::CTexChunk::Insert( const CBitmap& p_Bitmap, int p_nX, int p_nY )
{
	CBitmap alternate;
	const CBitmap* pBitmap = &p_Bitmap;

	if( p_nX < 0 || p_nY < 0 || 
		p_nX + static_cast< int >(p_Bitmap.Width()) > m_nW ||
		p_nY + static_cast< int >(p_Bitmap.Height()) > m_nH )
	{
		int nOffsetX = 0;
		int nOffsetY = 0;
		int nTrimmedWidth = p_Bitmap.Width();
		int nTrimmedHeight = p_Bitmap.Height();

		if( p_nX < 0 )
		{
			nOffsetX = p_nX;
			nTrimmedWidth += p_nX;
			p_nX = 0;
		}
		if( p_nY < 0 )
		{
			nOffsetY = p_nY;
			nTrimmedHeight += p_nY;
			p_nY = 0;
		}
		if( p_nX + nTrimmedWidth > m_nW )
		{
			nTrimmedWidth -= ( m_nW - p_nX - nTrimmedWidth );
		}
		if( p_nY + nTrimmedHeight > m_nH )
		{
			nTrimmedHeight -= ( m_nH - p_nY - nTrimmedHeight );
		}

		if( nTrimmedWidth <= 0 || nTrimmedHeight <= 0 )
		{
			return;
		}

		alternate.Resize( nTrimmedWidth, nTrimmedHeight );
		alternate.Insert( p_Bitmap, nOffsetX, nOffsetY );
		pBitmap = &alternate;
	}

	glBindTexture( GL_TEXTURE_2D, m_pTexture->TexName() );
	glTexSubImage2D( GL_TEXTURE_2D, 0, this->m_nX + p_nX, this->m_nY + p_nY, 
		pBitmap->Width(), pBitmap->Height(), CColor::GL_FORMAT, GL_UNSIGNED_BYTE, pBitmap->Data() );
}
//--------------------------------------------------------------------