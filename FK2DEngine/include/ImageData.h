/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	ImageData
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "Color.h"
#include "GraphicsBase.h"
#include "../../depend/boost/optional.hpp"
#include "../../depend/boost/utility.hpp"
//--------------------------------------------------------------------
namespace FK2DEngine
{
	struct SGLTexInfo
	{
		int				m_nTexName;
		float			m_fLeft;
		float			m_fRight;
		float			m_fTop;
		float			m_fBottom;
	};

	class CLargeImageData;
	class CMacro;
	class CTexChunk;
	class CImageData : boost::noncopyable
	{
	public:
		CImageData()
		{

		}
		virtual ~CImageData()
		{
			
		}
	public:
		virtual unsigned int Width() const = 0;
		virtual unsigned int Height() const = 0;

		virtual void FKDraw( double p_dX1, double p_dY1, CColor p_Color1,
			double p_dX2, double p_dY2, CColor p_Color2,
			double p_dX3, double p_dY3, CColor p_Color3,
			double p_dX4, double p_dY4, CColor p_Color4,
			ZPos p_Z, ENUM_AlphaMode p_eMode ) const = 0;

		virtual boost::optional< SGLTexInfo > GLTexInfo() const = 0;
		virtual CBitmap ToBitmap() const = 0;

		virtual void Insert( const CBitmap& p_Bitmap, int p_nX, int p_nY ) = 0;
	};
}