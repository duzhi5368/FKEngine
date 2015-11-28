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

#include "../include/Image.h"
#include "../include/Bitmap.h"
#include "../include/Graphics.h"
#include "../include/ImageData.h"
#include "../include/Math.h"
#include "../include/IO.h"
#include "../include/Timing.h"
#include "../include/StringConversion.h"
#include "../include/LogManager.h"

//--------------------------------------------------------------------
FK2DEngine::CImage::CImage( FK2DEngine::CGraphics& p_Graphics, const std::wstring& p_szFileName, bool p_bTileable /* = false */ )
{
	CImage( p_Graphics, LoadImageFile(p_szFileName), p_bTileable ).m_pData.swap( m_pData );
}
//--------------------------------------------------------------------
FK2DEngine::CImage::CImage( FK2DEngine::CGraphics &p_Graphics, const std::wstring &p_szFileName, unsigned int p_unSrcX,
	unsigned int p_unSrcY, unsigned int p_unSrcWidth, unsigned int p_unSrcHeight, bool p_bTileable /*= false*/)
{
	CImage( p_Graphics, LoadImageFile( p_szFileName ), p_unSrcX, p_unSrcY, 
		p_unSrcWidth, p_unSrcHeight, p_bTileable ).m_pData.swap( m_pData );
}
//--------------------------------------------------------------------
FK2DEngine::CImage::CImage( FK2DEngine::CGraphics &p_Graphics, const FK2DEngine::CBitmap &p_Source, bool p_bTileable /*= false*/ )
{
	CImage( p_Graphics, p_Source, 0, 0, p_Source.Width(), p_Source.Height(), p_bTileable ).m_pData.swap( m_pData );
}
//--------------------------------------------------------------------
FK2DEngine::CImage::CImage( CGraphics& p_Graphics, const CBitmap& p_Source, unsigned int p_unSrcX, 
	unsigned int p_unSrcY, unsigned int p_unSrcWidth, unsigned int p_unSrcHeight, bool p_bTileable /* = false */ )
	: m_pData( p_Graphics.CreateImage( p_Source, p_unSrcX, p_unSrcY, p_unSrcWidth, p_unSrcHeight, 
	p_bTileable ? FK2DEngine::EBF_Tileable : FK2DEngine::EBF_Smooth ) )
{
	
}
//--------------------------------------------------------------------
FK2DEngine::CImage::CImage( std::auto_ptr< CImageData > p_Data )
	: m_pData( p_Data.release() )
{

}
//--------------------------------------------------------------------
unsigned int FK2DEngine::CImage::Width() const
{
	return m_pData->Width();
}
//--------------------------------------------------------------------
unsigned int FK2DEngine::CImage::Height() const
{
	return m_pData->Height();
}
//--------------------------------------------------------------------
void FK2DEngine::CImage::FKDraw(double p_dX, double p_dY, FK2DEngine::ZPos p_Z,
			double p_dFactorX /*= 1.000000*/, double p_dFactorY /*= 1.000000*/, 
			FK2DEngine::CColor p_Color /*= CColor::WHITE*/, FK2DEngine::ENUM_AlphaMode p_eMode /*= EAM_Default*/) const
{
	double x2 = p_dX + Width() * p_dFactorX;
	double y2 = p_dY + Height() * p_dFactorY;
	m_pData->FKDraw( p_dX, p_dY, p_Color, x2, p_dY, p_Color, p_dX, y2, p_Color, x2, y2, p_Color, p_Z, p_eMode );
}
//--------------------------------------------------------------------
void FK2DEngine::CImage::DrawMod(double p_dX, double p_dY, FK2DEngine::ZPos p_Z,
			double p_dFactorX, double p_dFactorY, FK2DEngine::CColor p_Color1,
			FK2DEngine::CColor p_Color2, FK2DEngine::CColor p_Color3,
			FK2DEngine::CColor p_Color4, FK2DEngine::ENUM_AlphaMode p_eMode /*= EAM_Default*/) const
{
	double x2 = p_dX + Width() * p_dFactorX;
	double y2 = p_dY + Height() * p_dFactorY;	

	m_pData->FKDraw( p_dX, p_dY, p_Color1, x2, p_dY, p_Color2, p_dX, y2, p_Color3, x2, y2, p_Color4, p_Z, p_eMode );
}
//--------------------------------------------------------------------
void FK2DEngine::CImage::DrawRot(double p_dX, double p_dY, FK2DEngine::ZPos p_Z,
			double p_dAngle, double p_dCenterX /*= 0.500000*/, double p_dCenterY /*= 0.500000*/,
			double p_dFactorX /*= 1.000000*/, double p_dFactorY /*= 1.000000*/, 
			FK2DEngine::CColor p_Color /*= CColor::WHITE*/, FK2DEngine::ENUM_AlphaMode p_eMode /*= EAM_Default*/) const
{
	double sizeX	= Width() * p_dFactorX;
	double sizeY	= Height() * p_dFactorY;
	double offsX	= OffsetX( p_dAngle, 1 );
	double offsY	= OffsetY( p_dAngle, 1 );

	double distToLeftX   = +offsY * sizeX * p_dCenterX;
	double distToLeftY   = -offsX * sizeX * p_dCenterX;
	double distToRightX  = -offsY * sizeX * (1 - p_dCenterX);
	double distToRightY  = +offsX * sizeX * (1 - p_dCenterX);
	double distToTopX    = +offsX * sizeY * p_dCenterY;
	double distToTopY    = +offsY * sizeY * p_dCenterY;
	double distToBottomX = -offsX * sizeY * (1 - p_dCenterY);
	double distToBottomY = -offsY * sizeY * (1 - p_dCenterY);

	m_pData->FKDraw( 
		p_dX + distToLeftX  + distToTopX,
		p_dY + distToLeftY  + distToTopY, p_Color,
		p_dX + distToRightX + distToTopX,
		p_dY + distToRightY + distToTopY, p_Color,
		p_dX + distToLeftX  + distToBottomX,
		p_dY + distToLeftY  + distToBottomY, p_Color,
		p_dX + distToRightX + distToBottomX,
		p_dY + distToRightY + distToBottomY, p_Color,
		p_Z, p_eMode);
}
//--------------------------------------------------------------------
FK2DEngine::CImageData& FK2DEngine::CImage::GetData() const
{
	return *m_pData;
}
//--------------------------------------------------------------------