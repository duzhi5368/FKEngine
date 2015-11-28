/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Image
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "InsidePreInclude.h"
#include "Bitmap.h"
#include "../../depend/boost/shared_ptr.hpp"
#include <memory>
#include "Macro.h"
#include "LargeImageData.h"
#include "TexChunk.h"
//--------------------------------------------------------------------
namespace FK2DEngine
{
	class CImage
	{
	private:
		boost::shared_ptr< CImageData >			m_pData;
	public:
		CImage(){}
		//! 从文件中或从Bitmap加载一个 Image，绘制出来
		CImage( CGraphics& p_Graphics, const std::wstring& p_szFileName, bool p_bTileable = false );
		CImage( CGraphics& p_Graphics, const std::wstring& p_szFileName, unsigned int p_unSrcX,
			unsigned int p_unSrcY, unsigned int p_unSrcWidth, unsigned int p_unSrcHeight, bool p_bTileable = false );
		CImage( CGraphics& p_Graphics, const CBitmap& p_Source, bool p_bTileable = false );
		CImage( CGraphics& p_Graphics, const CBitmap& p_Source, unsigned int p_unSrcX, 
			unsigned int p_unSrcY, unsigned int p_unSrcWidth, unsigned int p_unSrcHeight, bool p_bTileable = false );
		explicit CImage( std::auto_ptr< CImageData > p_Data );
	public:
		unsigned int	Width() const;
		unsigned int	Height() const;
		//! 绘制一个Image
		void FKDraw( double p_dX, double p_dY, ZPos p_Z, double p_dFactorX = 1.0, 
			double p_dFactorY = 1.0, CColor p_Color = CColor::WHITE, ENUM_AlphaMode p_eMode = EAM_Default ) const;
		void DrawMod( double p_dX, double p_dY, ZPos p_Z, double p_dFactorX, 
			double p_dFactorY, CColor p_Color1, CColor p_Color2, CColor p_Color3, 
			CColor p_Color4, ENUM_AlphaMode p_eMode = EAM_Default ) const;
		void DrawRot( double p_dX, double p_dY, ZPos p_Z, double p_dAngle,
			double p_dCenterX = 0.5, double p_dCenterY = 0.5, 
			double p_dFactorX = 1.0, double p_dFactorY = 1.0,
			CColor p_Color = CColor::WHITE, ENUM_AlphaMode p_eMode = EAM_Default ) const;

		CImageData& GetData() const;
	};

	template< typename Container >
	void ImagesFromTiledBitmap( CGraphics& p_Graphics, const std::wstring& p_szFileName,
		int p_nTileWidth, int p_nTileHeight, bool p_bTileable, Container& p_AppendTo )
	{
		ImagesFromTiledBitmap( p_Graphics, LoadImageFile(p_szFileName), p_nTileWidth,
			p_nTileHeight, p_bTileable, p_AppendTo );
	}

	template< typename Container >
	void ImagesFromTiledBitmap( CGraphics& p_Graphics, const CBitmap& p_Bitmap,
		int p_nTileWidth, int p_nTileHeight, bool p_bTileable, Container& p_AppendTo )
	{
		int nTilesX = 0;
		int nTilesY = 0;

		if( p_nTileWidth > 0 )
		{
			nTilesX = p_Bitmap.Width() / p_nTileWidth;
		}
		else
		{
			nTilesX = -p_nTileWidth;
			p_nTileWidth = p_Bitmap.Width() / nTilesX;
		}

		if( p_nTileHeight > 0 )
		{
			nTilesY = p_Bitmap.Height() / p_nTileHeight;
		}
		else
		{
			nTilesY = -p_nTileHeight;
			p_nTileHeight = p_Bitmap.Height() / nTilesY;
		}

		for( int y = 0; y < nTilesY; ++y )
		{
			for( int x = 0; x < nTilesX; ++x )
			{
				p_AppendTo.push_back( typename Container::value_type( new CImage( p_Graphics,
					p_Bitmap, x * p_nTileWidth, y * p_nTileHeight, p_nTileWidth, p_nTileHeight, p_bTileable ) ) ); 
			}
		}
	}
}