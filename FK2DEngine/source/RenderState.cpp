/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	RenderState
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../Include/RenderState.h"

//--------------------------------------------------------------------
namespace FK2DEngine
{
	//--------------------------------------------------------------------
	CRenderState::CRenderState()
		: m_TexName( NO_TEXTURE )
		, m_pTransform( NULL )
		, m_unClipWidth( NO_CLIPPING )
		, m_unClipHeight( NO_CLIPPING )
		, m_eMode ( EAM_Default )
	{
		glMatrixMode( GL_MODELVIEW );
		glPushMatrix();
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	}
	//--------------------------------------------------------------------
	CRenderState::~CRenderState()
	{
		SetClipRect( NO_CLIPPING, NO_CLIPPING, NO_CLIPPING, NO_CLIPPING );
		SetTexName( NO_TEXTURE );
		glPopMatrix();
	}
	//--------------------------------------------------------------------
	void CRenderState::SetTransform( Transform* p_pTransform )
	{
		if( p_pTransform == m_pTransform )
		{
			return;
		}

		glPopMatrix();
		glPushMatrix();
#ifndef FK_IPHONE
		glMultMatrixd( p_pTransform->data() );
#else
		boost::array< float, 16 >	matrix;
		matrix = *p_pTransform;
		glMultMatrixf( matrix.data() );
#endif
		m_pTransform = p_pTransform;
	}
	//--------------------------------------------------------------------
	void CRenderState::SetTexName( GLuint p_TexName )
	{
		if( p_TexName == m_TexName )
		{
			return;
		}

		if( p_TexName != NO_TEXTURE )
		{
			if( m_TexName == NO_TEXTURE )
			{
				glEnable( GL_TEXTURE_2D );
			}
			glBindTexture( GL_TEXTURE_2D, p_TexName );
		}
		else if( m_TexName != NO_TEXTURE )
		{
			glDisable( GL_TEXTURE_2D );
		}
		m_TexName = p_TexName;
	}
	//--------------------------------------------------------------------
	void CRenderState::SetClipRect( unsigned int p_unClipX, unsigned int p_unClipY,
		unsigned int p_unClipWidth, unsigned int p_unClipHeight )
	{
		if( p_unClipWidth == NO_CLIPPING )
		{
			if( m_unClipWidth != NO_CLIPPING )
			{
				glDisable( GL_SCISSOR_TEST );
				m_unClipWidth = NO_CLIPPING;
			}
		}
		else
		{
			if( m_unClipWidth == NO_CLIPPING )
			{
				glEnable( GL_SCISSOR_TEST );
				glScissor( m_unClipX = p_unClipX, m_unClipY = p_unClipY,
					m_unClipWidth = p_unClipWidth, m_unClipHeight = p_unClipHeight );
			}
			else if( m_unClipX != p_unClipX || m_unClipY != p_unClipY ||
				m_unClipWidth != p_unClipWidth || m_unClipHeight != p_unClipHeight )
			{
				glScissor( m_unClipX = p_unClipX, m_unClipY = p_unClipY,
					m_unClipWidth = p_unClipWidth, m_unClipHeight = p_unClipHeight );
			}
		}
	}
	//--------------------------------------------------------------------
	void CRenderState::SetAlphaMode( ENUM_AlphaMode p_eMode )
	{
		if( p_eMode == m_eMode )
		{
			return;
		}

		m_eMode = p_eMode;
		if( m_eMode == EAM_Additive )
		{
			glBlendFunc( GL_SRC_ALPHA, GL_ONE );
		}
		else if( m_eMode == EAM_Mutiply )			// 抠底
		{
			glBlendFunc( GL_DST_COLOR, GL_ZERO );
		}
		else if( m_eMode == EAM_Default )			// 普通混合
		{
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		}
		else if( m_eMode == EAM_SrcAlphaOne )		// 半透明曝光
		{
			glBlendFunc( GL_SRC_ALPHA, GL_ONE );
		}
		else if( m_eMode == EAM_OneSrcAlpha )
		{
			glBlendFunc( GL_ONE, GL_SRC_ALPHA );
		}
	}
	//--------------------------------------------------------------------
}