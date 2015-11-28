/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	WinText
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include <windows.h>
#include "../include/Bitmap.h"
#include "../include/Text.h"
#include "../include/StringConversion.h"
#include "../include/WinUtility.h"
#include "../../depend/boost/utility.hpp"
#include <cstdlib>
#include <cwchar>
#include <algorithm>
#include <stdexcept>
#include <map>
#include <set>

//--------------------------------------------------------------------
std::wstring FK2DEngine::DefaultFontName()
{
	return L"Arial";
}
//--------------------------------------------------------------------
namespace FK2DEngine
{
	std::wstring GetNameFromTTFFile( const std::wstring& p_FileName );

	namespace
	{
		class CWinBitmap : boost::noncopyable
		{
		private:
			HDC			m_Hdc;
			HBITMAP		m_hBitmap;
			char*		m_pPixels;
		public:
			CWinBitmap( unsigned int p_unWidth, unsigned int p_unHeight )
			{
				m_Hdc = Win::Check( ::CreateCompatibleDC( 0 ), "创建一个设备上下文" );

				BITMAPCOREHEADER		CoreHeader;
				CoreHeader.bcSize		= sizeof( CoreHeader );
				CoreHeader.bcWidth		= p_unWidth;
				CoreHeader.bcHeight		= p_unHeight;
				CoreHeader.bcPlanes		= 1;
				CoreHeader.bcBitCount	= 24;

				m_hBitmap	= ::CreateDIBSection( m_Hdc, 
					reinterpret_cast< BITMAPINFO* >( &CoreHeader ),
					DIB_RGB_COLORS, 
					reinterpret_cast< VOID** >( &m_pPixels ), 0, 0 );
				if( m_hBitmap == NULL )
				{
					::DeleteDC( m_Hdc );
					Win::ThrowLastError( "创建一个Bitmap" );
				}

				::SelectObject( m_Hdc, m_hBitmap );

				HBRUSH hBrush = ::CreateSolidBrush( 0x000000 );
				RECT rc = { 0, 0, p_unWidth, p_unHeight };
				::FillRect( m_Hdc, &rc, hBrush );
				::DeleteObject( hBrush );
			}

			~CWinBitmap()
			{
				::DeleteObject( m_hBitmap );
				::SelectObject( m_Hdc, ::GetStockObject( SYSTEM_FONT ) );
				::DeleteDC( m_Hdc );
			}

			HDC Context() const
			{
				return m_Hdc;
			}

			HBITMAP Handle() const
			{
				return m_hBitmap;
			}

			void SelectFont( std::wstring p_szFontName, unsigned int p_unFontHeight,
				unsigned int p_unFontFlags ) const
			{
				static std::map< std::wstring, std::wstring >		CustomFonts;

				if( p_szFontName.find( L"/" ) != std::wstring::npos )
				{
					if( CustomFonts.count( p_szFontName ) == 0 )
					{
						AddFontResourceEx( p_szFontName.c_str(), FR_PRIVATE, 0 );
						p_szFontName = CustomFonts[p_szFontName] = GetNameFromTTFFile(p_szFontName);
					}
					else
					{
						p_szFontName = CustomFonts[p_szFontName];
					}
				}

				static std::map< std::pair< std::wstring, unsigned int >, HFONT >	LoadedFonts;

				HFONT hFont;
				std::pair< std::wstring, unsigned int > Key = std::make_pair( p_szFontName, p_unFontHeight | p_unFontFlags << 16 );
				if( LoadedFonts.count( Key ) == 0 )
				{
					LOGFONT logFont = 
					{
						p_unFontHeight, 
						0,
						0,
						0,
						p_unFontFlags & EFF_Bold ? FW_BOLD : FW_NORMAL,
						p_unFontFlags & EFF_Italic ? TRUE : FALSE,
						p_unFontFlags & EFF_Underline ? TRUE : FALSE,
						FALSE,
						DEFAULT_CHARSET,
						OUT_DEFAULT_PRECIS,
						CLIP_DEFAULT_PRECIS,
						ANTIALIASED_QUALITY,
						DEFAULT_PITCH | FF_DONTCARE,
						L""
					};
			
					wcsncpy_s( logFont.lfFaceName, p_szFontName.c_str(), LF_FACESIZE );
					logFont.lfFaceName[ LF_FACESIZE - 1 ] = 0;

					hFont = LoadedFonts[ Key ] = Win::Check( ::CreateFontIndirect( &logFont ),
						"创建字体对象 " + Narrow( p_szFontName ) );
				}
				else
				{
					hFont = LoadedFonts[ Key ];
				}

				::SelectObject( m_Hdc, hFont );
			}
		};
	}
}
//--------------------------------------------------------------------
unsigned int FK2DEngine::TextWidth( const std::wstring& p_Text, const std::wstring& p_FontName,
					unsigned int p_unFontHeight, unsigned int p_unFontFlags /* = 0 */ )
{
	CWinBitmap helper( 1, 1 );
	helper.SelectFont( p_FontName, p_unFontHeight, p_unFontFlags );

	SIZE size;
	if( ! ::GetTextExtentPoint32( helper.Context(), p_Text.c_str(), p_Text.length(), &size ) )
	{
		Win::ThrowLastError( "计算文字宽度" );
	}
	return size.cx;
}
//--------------------------------------------------------------------
void FK2DEngine::FKDrawText( CBitmap& p_Bitmap, const std::wstring& p_Text, int p_nX, 
	int p_nY, CColor p_Color, const std::wstring& p_FontName, 
	unsigned int p_unFontHeight, unsigned int p_unFontFlags /* = 0 */ )
{
	unsigned int unWidth = TextWidth( p_Text, p_FontName, p_unFontHeight, p_unFontFlags );

	CWinBitmap helper( unWidth, p_unFontHeight );
	helper.SelectFont( p_FontName, p_unFontHeight, p_unFontFlags );

	if( ::SetTextColor( helper.Context(), 0xffffff ) == CLR_INVALID )
	{
		Win::ThrowLastError( "设置文字颜色" );
	}
	Win::Check( ::SetBkColor( helper.Context(), TRANSPARENT ), "设置文字图片背景颜色为透明" );

	::ExtTextOut( helper.Context(), 0, 0, 0, 0, p_Text.c_str(), p_Text.length(), 0 );

	for( unsigned int relY = 0; relY < p_unFontHeight; ++relY )
	{
		for( unsigned int relX = 0; relX < unWidth; ++relX )
		{
			CColor pixel = p_Color;
			CColor::Channel srcAlpha = GetPixel( helper.Context(), relX, relY ) & 0xff;
			if( srcAlpha == 0 )
			{
				continue;
			}
			pixel = Multiply( p_Color, CColor( srcAlpha, 255, 255, 255 ) );
			if( pixel != 0 && p_nX + relX >= 0 && p_nX + relX < p_Bitmap.Width()
				&& p_nY + relY >= 0 && p_nY + relY < p_Bitmap.Height() )
			{
				p_Bitmap.SetPixel( p_nX + relX, p_nY + relY, pixel );
			}
		}
	}
}
//--------------------------------------------------------------------