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
#include "InsidePreInclude.h"
#include "Color.h"
#include "GraphicsBase.h"
#include "Platform.h"
#include "../../depend/boost/shared_ptr.hpp"
#include <string>
//--------------------------------------------------------------------
namespace FK2DEngine
{
	class CFont
	{
		struct SImpl;
	private:
		boost::shared_ptr< SImpl >		m_pImpl;

	public:
		CFont( CGraphics& p_Graphics, const std::wstring& p_FontName,
			unsigned int p_unFontHeight, unsigned int p_unFontFlags = EFF_Bold );

		std::wstring Name() const;

		unsigned int Height() const;

		unsigned int Flags() const;

		double TextWidth( const std::wstring& p_szText, double p_dFactorX = 1.0 ) const;

		void FKDraw( const std::wstring& p_szText, double p_dX, double p_dY,
			ZPos p_Z, double p_dFactorX = 1.0, double p_dFactorY = 1.0, 
			CColor p_Color = CColor::WHITE, ENUM_AlphaMode p_eMode = EAM_Default ) const;

		void DrawRel( const std::wstring& p_szText, double p_dX, double p_dY,
			ZPos p_Z, double p_dRelX, double p_dRelY, double p_dFactorX = 1.0,
			double p_dFactorY = 1.0, CColor p_Color = CColor::WHITE,
			ENUM_AlphaMode p_eMode = EAM_Default )const;

#ifndef SWIG
		FK_DEPRECATED
#endif
		void DrawDot( const std::wstring& p_szText, double p_dX, double p_dY,
			ZPos p_Z, double p_dAngle, double p_dFactorX = 1.0, double p_dFactorY = 1.0, 
			CColor p_Color = CColor::WHITE, ENUM_AlphaMode p_eMode = EAM_Default )const;

	};
}