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

#pragma once

//--------------------------------------------------------------------
#include "GraphicsCommon.h"
//--------------------------------------------------------------------
class FK2DEngine::CRenderState
{
private:
	GLuint				m_TexName;
	Transform*			m_pTransform;
	unsigned int		m_unClipX;
	unsigned int		m_unClipY;
	unsigned int		m_unClipWidth;
	unsigned int		m_unClipHeight;
	ENUM_AlphaMode		m_eMode;
public:
	CRenderState();
	~CRenderState();
public:
	void SetTransform( Transform* p_pTransform );
	void SetTexName( GLuint p_TexName );
	void SetClipRect( unsigned int p_unClipX, unsigned int p_unClipY,
		unsigned int p_unClipWidth, unsigned int p_unClipHeight );
	void SetAlphaMode( ENUM_AlphaMode p_eMode );
};