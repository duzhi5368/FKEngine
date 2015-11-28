/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	DrawOp
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "GraphicsBase.h"
#include "Color.h"
#include "GraphicsCommon.h"
#include "TexChunk.h"
#include "RenderState.h"
#include "../../depend/boost/cstdint.hpp"
//--------------------------------------------------------------------
namespace FK2DEngine
{
	struct SArrayVertex
	{
		float			m_fTexCoords[2];
		boost::uint32_t	m_unColor;
		float			m_fVectices[3];
	};

	typedef std::vector< SArrayVertex >		VectexArray;

	struct SDrawOp
	{
	public:
		struct SVertex
		{
			float		m_fX;
			float		m_fY;
			CColor		m_Color;

			SVertex()
				: m_fX( 0.0f )
				, m_fY( 0.0f )
				, m_Color( 0x00000000 )
			{

			}
			SVertex( float x, float y, CColor c )
				: m_fX( x )
				, m_fY( y )
				, m_Color( c )
			{

			}
		};

		ZPos						m_Z;
		FK2DEngine::Transform*		m_pTransform;
		int							m_nClipX;
		int							m_nClipY;
		unsigned int				m_unClipWidth;
		unsigned int				m_unClipHeight;
		SVertex						m_Vertices[4];
		unsigned int				m_unUsedVertices;
		const CTexChunk*			m_pChunk;
		ENUM_AlphaMode				m_eMode;

		SDrawOp( FK2DEngine::Transform& p_Transform )
			: m_pTransform( &p_Transform )
			, m_unClipWidth( NO_CLIPPING )
			, m_pChunk( NULL )
		{

		}

		void Perform( CRenderState& p_Cur, const SDrawOp* p_pNext ) const
		{
#ifdef FK_IPHONE
			static const unsigned int MAX_AUTOGROUP = 24;
			static int SpriteCounter = 0;
			static float SpriteVectices[ 12 * MAX_AUTOGROUP ];
			static float SpriteTexcoords[ 12 * MAX_AUTOGROUP ];
			static boost::uint32_t SpriteColors[ 6 * MAX_AUTOGROUP ];
			static bool IsSetup = false;

			if( !IsSetup )
			{
				glVertexPointer( 2, GL_FLOAT, 0, SpriteVectices );
				glEnableClientState(GL_VERTEX_ARRAY);
				glTexCoordPointer( 2, GL_FLOAT, 0, SpriteTexcoords );
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glColorPointer( 4, GL_UNSIGNED_BYTE, 0, SpriteColors );
				glEnableClientState(GL_COLOR_ARRAY);

				IsSetup = true;
			}
#endif

			p_Cur.SetClipRect( m_nClipX, m_nClipY, m_unClipWidth, m_unClipHeight );
			p_Cur.SetTransform( m_pTransform );
			p_Cur.SetAlphaMode( m_eMode );

			if( m_pChunk )
			{
				p_Cur.SetTexName( m_pChunk->TexName() );

#ifdef FK_IPHONE
				float fLeft = 1.0f;
				float fTop = 1.0f;
				float fRight = 1.0f;
				float fBottom = 1.0f;

				m_pChunk->GetCoords( fLeft, fTop, fRight, fBottom );

				SpriteTexcoords[ SpriteCounter * 12 + 0 ] = fLeft;
				SpriteTexcoords[ SpriteCounter * 12 + 1 ] = fTop;
				SpriteTexcoords[ SpriteCounter * 12 + 2 ] = fRight;
				SpriteTexcoords[ SpriteCounter * 12 + 3 ] = fTop;
				SpriteTexcoords[ SpriteCounter * 12 + 4 ] = fLeft;
				SpriteTexcoords[ SpriteCounter * 12 + 5 ] = fBottom;

				SpriteTexcoords[ SpriteCounter * 12 + 6 ] = fRight;
				SpriteTexcoords[ SpriteCounter * 12 + 7 ] = fTop;
				SpriteTexcoords[ SpriteCounter * 12 + 8 ] = fLeft;
				SpriteTexcoords[ SpriteCounter * 12 + 9 ] = fBottom;
				SpriteTexcoords[ SpriteCounter * 12 + 10 ] = fRight;
				SpriteTexcoords[ SpriteCounter * 12 + 11 ] = fBottom;
#endif
			}
			else
			{
				p_Cur.SetTexName( NO_TEXTURE );
			}

#ifndef FK_IPHONE
			if( m_unUsedVertices == 2 )
			{
				glBegin( GL_LINES );
			}
			else if( m_unUsedVertices == 3 )
			{
				glBegin( GL_TRIANGLES );
			}
			else if( m_unUsedVertices == 4 )
			{
				glBegin( GL_QUADS );
			}

			float fLeft = 0.0f;
			float fRight = 0.0f;
			float fTop = 0.0f;
			float fBottom = 0.0f;

			if( m_pChunk )
			{
				m_pChunk->GetCoords( fLeft, fTop, fRight, fBottom );
			}

			for( unsigned int i = 0; i < m_unUsedVertices; ++i )
			{
				glColor4ubv( reinterpret_cast< const GLubyte* >(&m_Vertices[i].m_Color ) );
				if( m_pChunk )
				{
					switch( i )
					{
					case 0:
						glTexCoord2f( fLeft, fTop );
						break;
					case 1:
						glTexCoord2f( fRight, fTop );
						break;
					case 2:
						glTexCoord2f( fRight, fBottom );
						break;
					case 3:
						glTexCoord2f( fLeft, fBottom );
						break;
					}
				}

				glVertex2f( m_Vertices[i].m_fX, m_Vertices[i].m_fY );
			}

			glEnd();
#else
			for( int i = 0; i < 3; ++i )
			{
				SpriteVectices[ SpriteCounter * 12 + i * 2 ] = m_Vertices[i].m_fX;
				SpriteVectices[ SpriteCounter * 12 + i * 2 + 1 ] = m_Vertices[i].m_fY;
				SpriteColors[ SpriteCounter * 6 + i ] = m_Vertices[i].m_Color.Abgr();
			}
			for( int i = 0; i < 3; ++i )
			{
				SpriteVectices[ SpriteCounter * 12 + 6 + i * 2 ] = m_Vertices[i + 1].m_fX;
				SpriteVectices[ SpriteCounter * 12 + 6 + i * 2 + 1 ] = m_Vertices[i + 1].m_fY;
				SpriteColors[ SpriteCounter * 6 + 3 + i ] = m_Vertices[i + 1].m_Color.Abgr();
			}

			++SpriteCounter;

			if( SpriteCounter == MAX_AUTOGROUP or p_pNext == 0 or m_pChunk == 0
				or p_pNext->m_pChunk == 0 or p_pNext->m_pTransform != m_pTransform
				or p_pNext->m_pChunk->TexName() != m_pChunk->TexName() 
				or p_pNext->m_eMode != m_eMode or m_unClipWidth != NO_CLIPPING
				or p_pNext->m_unClipWidth != NO_CLIPPING )
			{
				glDrawArrays( GL_TRIANGLES, 0, 6 * SpriteCounter );
				SpriteCounter = 0;
			}
#endif
		}

		void CompileTo( VectexArray& p_VA ) const
		{
			SArrayVertex Result[4];

			for( int i = 0; i < 4; ++i )
			{
				Result[i].m_fVectices[0] = m_Vertices[i].m_fX;
				Result[i].m_fVectices[1] = m_Vertices[i].m_fY;
				Result[i].m_fVectices[2] = 0;
				Result[i].m_unColor = m_Vertices[i].m_Color.Abgr();
			}

			float fLeft = 0.0f;
			float fRight = 0.0f;
			float fTop = 0.0f;
			float fBottom = 0.0f;

			if( m_pChunk )
			{
				m_pChunk->GetCoords( fLeft, fTop, fRight, fBottom );
			}

			Result[0].m_fTexCoords[0]	= fLeft;
			Result[0].m_fTexCoords[1]	= fTop;
			Result[1].m_fTexCoords[0]	= fRight;
			Result[1].m_fTexCoords[1]	= fTop;
			Result[2].m_fTexCoords[0]	= fRight;
			Result[2].m_fTexCoords[1]	= fBottom;
			Result[3].m_fTexCoords[0]	= fLeft;
			Result[3].m_fTexCoords[1]	= fBottom;

			p_VA.insert( p_VA.end(), Result, Result + 4 );
		}

		bool operator < ( const SDrawOp& p_Other )const
		{
			return m_Z < p_Other.m_Z;
		}
	};
}