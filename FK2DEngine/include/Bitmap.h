/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Bitmap
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "Color.h"
#include "InsidePreInclude.h"
#include "GraphicsBase.h"
#include "Platform.h"
#include "../../depend/boost/scoped_ptr.hpp"
#include <string>
#include <vector>
#include <Windows.h>
//--------------------------------------------------------------------
namespace FK2DEngine
{
	class CBitmap
	{
	private:
		unsigned int				m_unWidth;
		unsigned int				m_unHeight;
		std::vector< CColor >		m_vecPixels;
	public:
		CBitmap() 
			: m_unWidth( 0 )
			, m_unHeight( 0 )
		{

		}

		CBitmap( unsigned int p_unWidth, unsigned int p_unHeight, CColor p_Color = CColor::NONE )
			: m_unWidth( p_unWidth )
			, m_unHeight( p_unHeight )
			, m_vecPixels( p_unHeight * p_unWidth, p_Color )
		{

		}

		unsigned int Width() const
		{
			return m_unWidth;
		}

		unsigned int Height() const
		{
			return m_unHeight;
		}

		CColor GetPixel( unsigned int p_unX, unsigned int p_unY ) const
		{
			return m_vecPixels[ p_unY * m_unWidth + p_unX ];
		}

		void SetPixel( unsigned int p_unX, unsigned int p_unY, CColor p_Color )
		{
			m_vecPixels[ p_unY * m_unWidth + p_unX ] = p_Color;
		}

		void Swap( CBitmap& p_Other );
		void Resize( unsigned int p_unWidth, unsigned int p_unHeight, CColor p_Color = CColor::NONE );
		//! 在指定位置添加一个图片，若图片出了区域，则被裁减
		void Insert( const CBitmap& p_Source, int p_nX, int p_nY );
		void Insert( const CBitmap& p_Source, int p_nX, int p_nY, unsigned int p_unSrcX,
			unsigned int p_unSrcY, unsigned int p_unSrcWidth, unsigned int p_unSrcHeight ); 

		const CColor* Data() const
		{
			return &m_vecPixels[0];
		}
		CColor* Data()
		{
			return &m_vecPixels[0];
		}

		FK_DEPRECATED void Fill( CColor p_Color );
		FK_DEPRECATED void Replace( CColor p_OldColor, CColor p_NewColor );
	};


	//! 读取图片文件
	CBitmap LoadImageFile( const std::wstring& p_szFileName );
	CBitmap LoadImageFile( CReader p_Input );

	//! 保存图片文件
	void SaveImageFile( const CBitmap& p_Bitmap, const std::wstring& p_szFileName );
	void SaveImageFile( const CBitmap& p_Bitmap, FK2DEngine::CWriter p_Writer, const std::wstring& p_szFormatHint = L"png" );
	bool SaveHBITMAP2File( const std::wstring& pszFile, HBITMAP p_hBMP, HDC p_hDC );

	//! 设置颜色等于 key 值得像素点 alpha 值为 0
	void ApplyColorKey( CBitmap& p_Bitmap, CColor p_Key );
	//! 设置所有 alpha 值为 0 的像素点为 BackGround 指定的颜色
	void UnApplyColorKey( CBitmap& p_Bitmap, CColor p_BackGround );

	void ApplyBorderFlags( CBitmap& p_DestBitmap, const CBitmap& p_SrcBitmap,
		unsigned int p_unSrcX, unsigned int p_unSrcY, unsigned int p_unSrcWidth,
		unsigned int p_unSrcHeight, unsigned int p_unBorderFlags );

	FK_DEPRECATED CReader LoadFromBMP( CBitmap& p_Bitmap, CReader p_Reader );
	FK_DEPRECATED CWriter SaveToBMP( const CBitmap& p_Bitmap, CWriter p_Writer );
	FK_DEPRECATED CReader LoadFromPNG( CBitmap& p_Bitmap, CReader p_Reader );
	FK_DEPRECATED CWriter SaveToPNG( const CBitmap& p_Bitmap, CWriter p_Writer );
}