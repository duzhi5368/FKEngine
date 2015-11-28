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

#pragma once

//--------------------------------------------------------------------
#include "InsidePreInclude.h"
#include "ImageData.h"
#include "../../depend/boost/none.hpp"
#include "../../depend/boost/shared_ptr.hpp"
#include <vector>
//--------------------------------------------------------------------
namespace FK2DEngine
{
	class CLargeImageData : public CImageData
	{
	private:
		unsigned int		m_unFullWidth;
		unsigned int		m_unFullHeight;
		unsigned int		m_unPartsX;
		unsigned int		m_unPartsY;
		unsigned int		m_unPartWidth;
		unsigned int		m_unPartHeight;
		std::vector< boost::shared_ptr< CImageData > >	m_Parts;
	public:
		CLargeImageData( CGraphics& p_Graphics, const CBitmap& p_Source, 
			unsigned int p_unPartWidth, unsigned int p_unPartHeight, unsigned int p_unBorderFlags );
	public:
		unsigned int	Width() const;
		unsigned int	Height() const;

		void FKDraw( double p_dX1, double p_dY1, CColor p_Color1,
			double p_dX2, double p_dY2, CColor p_Color2,
			double p_dX3, double p_dY3, CColor p_Color3,
			double p_dX4, double p_dY4, CColor p_Color4,
			ZPos p_Z, ENUM_AlphaMode p_eMode ) const;

		boost::optional< SGLTexInfo >		GLTexInfo() const
		{
			return boost::none;
		}

		CBitmap ToBitmap() const;
		void Insert( const CBitmap& p_Bitmap, int p_nX, int p_nY );
	};
}