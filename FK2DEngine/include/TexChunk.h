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

#pragma once

//--------------------------------------------------------------------
#include "InsidePreInclude.h"
#include "ImageData.h"
#include "GraphicsCommon.h"
#include <memory>
#include <vector>
#include <stdexcept>
#include "../../depend/boost/shared_ptr.hpp"
//--------------------------------------------------------------------
class FK2DEngine::CTexChunk : public FK2DEngine::CImageData
{
private:
	CGraphics*							m_pGraphics;
	Transforms*							m_pTransforms;
	DrawOpQueueStack*					m_pQueues;
	boost::shared_ptr< CTexture >		m_pTexture;
	int									m_nX;
	int									m_nY;
	int									m_nW;
	int									m_nH;
	int									m_nPadding;

	SGLTexInfo							m_Info;
public:
	CTexChunk( CGraphics& p_Graphics, Transforms& p_Transforms, DrawOpQueueStack& p_Queues,
		boost::shared_ptr< CTexture > p_pTexture, int p_nX, int p_nY, int p_nW, int p_nH, int p_nPadding );
	~CTexChunk();
public:
	unsigned int Width() const
	{
		return m_nW;
	}
	unsigned int Height() const
	{
		return m_nH;
	}
	GLuint TexName() const
	{
		return m_Info.m_nTexName;
	}
	void GetCoords( float& p_fLeft, float& p_fTop, float& p_fRight, float& p_fBottom )const
	{
		p_fLeft		= m_Info.m_fLeft;
		p_fTop		= m_Info.m_fTop;
		p_fRight	= m_Info.m_fRight;
		p_fBottom	= m_Info.m_fBottom;
	}

	void FKDraw( double x1, double y1, CColor c1,
		double x2, double y2, CColor c2,
		double x3, double y3, CColor c3,
		double x4, double y4, CColor c4,
		ZPos p_Z, ENUM_AlphaMode p_eMode )const;

	boost::optional< SGLTexInfo >		GLTexInfo() const;
	FK2DEngine::CBitmap					ToBitmap() const;
	void								Insert( const CBitmap& p_Bitmap, int p_nX, int p_nY );
};
