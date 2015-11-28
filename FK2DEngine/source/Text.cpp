/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Text
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../include/Text.h"
#include "../include/Bitmap.h"
#include "../include/Graphics.h"
#include "../include/Image.h"
#include "../include/Math.h"
#include "../include/StringConversion.h"
#include "../include/GraphicsCommon.h"
#include "../include/FormattedString.h"
#include "../../depend/boost/algorithm/string.hpp"
#include "../../depend/boost/bind.hpp"
#include "../../depend/boost/foreach.hpp"
#include "../../depend/boost/shared_ptr.hpp"
#include <cassert>
#include <cmath>
#include <algorithm>
#include <map>
#include <vector>
//--------------------------------------------------------------------
using namespace std;
//--------------------------------------------------------------------
namespace FK2DEngine
{
	namespace
	{
		bool IsBreakingAsianGlygh( wchar_t p_Ch )
		{
			if( p_Ch >= 0x3040 && p_Ch <= 0x3096 )
			{
				return true;		// 日语平假名
			}
			if( p_Ch >= 0x30a0 && p_Ch <= 0x30fa )
			{
				return true;		// 日语片假名
			}
			if( p_Ch >= 0x4e00 && p_Ch <= 0x9fff )
			{
				return true;
			}
			if( p_Ch >= 0x3400 && p_Ch <= 0x4db5 )
			{
				return true;
			}

			return false;
		}

		struct SWordInfo
		{
			CFormattedString	m_Text;
			unsigned int		m_unWidth;
			unsigned int		m_unSpaceWidth;
		};
		typedef vector< SWordInfo >			VEC_Words;

		class CTextBlockBuilder
		{
		private:
			CBitmap					m_Bitmap;
			unsigned int			m_unUsedLines;
			unsigned int			m_unAllocatedLines;
			wstring					m_szFontName;
			unsigned int			m_unFontHeight;
			int						m_nLineSpacing;
			ENUM_TextAlign			m_eAlign;
			unsigned int			m_unSpaceWidth;
		private:
			void	AllocNextLine()
			{
				++m_unUsedLines;
				if( m_unUsedLines == m_unAllocatedLines )
				{
					m_unAllocatedLines += 10;
					m_Bitmap.Resize( m_Bitmap.Width(),
						m_unFontHeight * m_unAllocatedLines + m_unAllocatedLines * ( m_unAllocatedLines - 1 ),
						CColor::NONE );

				}
			}
		public:
			CTextBlockBuilder( const wstring& p_FontName, unsigned int p_unFontHeight, 
				int p_nLineSpacing, unsigned int p_unWidth, ENUM_TextAlign p_eAlign )
			{
				m_unUsedLines = 0;
				m_unAllocatedLines = 10;

				m_Bitmap.Resize( p_unWidth, ( p_nLineSpacing + p_unFontHeight ) * m_unAllocatedLines, 0x00ffffff );

				this->m_szFontName		= p_FontName;
				this->m_unFontHeight	= p_unFontHeight;
				this->m_nLineSpacing	= p_nLineSpacing;
				this->m_eAlign			= p_eAlign;

				m_unSpaceWidth	= TextWidth( CFormattedString( L" ", 0 ) );
			}

			unsigned int Width() const
			{
				return m_Bitmap.Width();
			}

			unsigned TextWidth( const CFormattedString& p_Text ) const
			{
				if( p_Text.Length() == 0 )
				{
					return 0;
				}

				if( p_Text.EntityAt( 0 ) )
				{
					return EntityBitmap( p_Text.EntityAt( 0 ) ).Width();
				}

				std::vector< CFormattedString >	parts = p_Text.SplitParts();
				unsigned int unResult = 0;
				BOOST_FOREACH( const CFormattedString& part, parts )
				{
					unResult += FK2DEngine::TextWidth( part.Unformat(), m_szFontName, 
						m_unFontHeight, part.FlagAt( 0 ));
				}
				return unResult;
			}

			void AddLine( VEC_Words::const_iterator p_IteBegin, VEC_Words::const_iterator p_IteEnd,
				unsigned int p_unWordsWidth, bool p_bOverrideAlign )
			{
				AllocNextLine();

				unsigned int unWords = p_IteEnd - p_IteBegin;

				unsigned int unTotalSpacing = 0;
				if( p_IteBegin < p_IteEnd )
				{
					for( VEC_Words::const_iterator i = p_IteBegin;
						i != p_IteEnd - 1; ++i )
					{
						unTotalSpacing += i->m_unSpaceWidth;
					}
				}

				unsigned int unTop = ( m_unUsedLines - 1 ) * ( m_unFontHeight + m_nLineSpacing );

				double dPos = 0.0;
				switch( m_eAlign )
				{
				case ETA_Right:
					dPos = m_Bitmap.Width() - p_unWordsWidth - unTotalSpacing;
					break;
				case ETA_Center:
					dPos = m_Bitmap.Width() - p_unWordsWidth - unTotalSpacing;
					dPos /= 2;
					break;
				default:
					dPos = 0;
				}

				for( VEC_Words::const_iterator CurIte = p_IteBegin; 
					CurIte != p_IteEnd; ++CurIte )
				{
					std::vector< CFormattedString >	Parts = CurIte->m_Text.SplitParts();
					int x = 0;
					BOOST_FOREACH( const CFormattedString& part, Parts )
					{
						if( part.EntityAt( 0 ) )
						{
							FK2DEngine::CBitmap entity = EntityBitmap( part.EntityAt( 0 ) );
							MultiplyBitmapAlpha( entity, part.ColorAt( 0 ).Alpha() );
							m_Bitmap.Insert( entity, Trunc( dPos ) + x, Trunc( unTop ) );
							x += entity.Width();
							continue;
						}

						std::wstring szUnformttedPart = part.Unformat();
						FK2DEngine::FKDrawText( m_Bitmap, szUnformttedPart, Trunc( dPos ) + x, 
							Trunc( unTop ), part.ColorAt( 0 ), m_szFontName, 
							m_unFontHeight, part.FlagAt( 0 ) );
						x += FK2DEngine::TextWidth( szUnformttedPart, m_szFontName,
							m_unFontHeight, part.FlagAt( 0 ) );
					}

					if( m_eAlign == ETA_Justify && !p_bOverrideAlign )
					{
						dPos += CurIte->m_unWidth + 1.0 * ( Width() - p_unWordsWidth ) /( unWords - 1 );
					}
					else
					{
						dPos += CurIte->m_unWidth + CurIte->m_unSpaceWidth;
					}
				}
			}

			void AddEmptyLine()
			{
				AllocNextLine();
			}
	
			CBitmap Result() const
			{
				CBitmap Result = m_Bitmap;
				Result.Resize( Result.Width(), 
					m_unFontHeight * m_unUsedLines + m_nLineSpacing * ( m_unUsedLines - 1 ) );
				return Result;
			}

			unsigned int SpaceWidth() const
			{
				return m_unSpaceWidth;
			}
		};

		void ProcessWords( CTextBlockBuilder& p_Builder, const VEC_Words& p_Words )
		{
			if( p_Words.empty() )
			{
				return p_Builder.AddEmptyLine();
			}

			VEC_Words::const_iterator IteLineBegin = p_Words.begin();

			unsigned int unWordsWidth = 0;
			unsigned int unSpaceWidth = 0;

			for( VEC_Words::const_iterator IteW = p_Words.begin(); IteW != p_Words.end(); ++IteW )
			{
				unsigned int unNewWordsWidth = unWordsWidth + IteW->m_unWidth;

				if( unNewWordsWidth + unSpaceWidth <= p_Builder.Width() )
				{
					unWordsWidth = unNewWordsWidth;
					unSpaceWidth += IteW->m_unSpaceWidth;
				}
				else
				{
					p_Builder.AddLine( IteLineBegin, IteW, unWordsWidth, false );

					IteLineBegin = IteW;
					unWordsWidth = IteW->m_unWidth;
					unSpaceWidth = IteW->m_unSpaceWidth;
				}
			}

			if( p_Words.empty() || IteLineBegin != p_Words.end() )
			{
				p_Builder.AddLine( IteLineBegin, p_Words.end(), unWordsWidth, true );
			}
		}

		void ProcessParagraph( CTextBlockBuilder& p_Builder, const CFormattedString& p_Paragraph )
		{
			VEC_Words		CollectedWords;

			unsigned int	unBeginOfWord	= 0;

			for( unsigned int unCur = 0; unCur < p_Paragraph.Length(); ++unCur )
			{
				SWordInfo newWord;

				if( p_Paragraph.CharAt( unCur ) == L' ' )
				{
					if( unBeginOfWord != unCur )
					{
						newWord.m_Text		= p_Paragraph.Range( unBeginOfWord, unCur );
						newWord.m_unWidth	= p_Builder.TextWidth( newWord.m_Text );
						newWord.m_unSpaceWidth = p_Builder.SpaceWidth();
						CollectedWords.push_back( newWord );
					}
					unBeginOfWord = unCur + 1;
				}
				else if( IsBreakingAsianGlygh( p_Paragraph.CharAt( unCur ) ) )
				{
					if( unBeginOfWord != unCur )
					{
						newWord.m_Text		= p_Paragraph.Range( unBeginOfWord, unCur );
						newWord.m_unWidth	= p_Builder.TextWidth( newWord.m_Text );
						newWord.m_unSpaceWidth	= 0;
						CollectedWords.push_back( newWord );
					}

					newWord.m_Text		= p_Paragraph.Range( unCur, unCur + 1 );
					newWord.m_unWidth	= p_Builder.TextWidth( newWord.m_Text );
					newWord.m_unSpaceWidth	= 0;
					CollectedWords.push_back( newWord );

					unBeginOfWord = unCur + 1;
				}
			}

			if( unBeginOfWord < p_Paragraph.Length() )
			{
				SWordInfo lastWord;
				lastWord.m_Text			= p_Paragraph.Range( unBeginOfWord, p_Paragraph.Length() );
				lastWord.m_unWidth		= p_Builder.TextWidth( lastWord.m_Text );
				lastWord.m_unSpaceWidth	= 0;
				CollectedWords.push_back( lastWord );
			}

			ProcessWords( p_Builder, CollectedWords );
		}

		void ProcessText( CTextBlockBuilder& p_Builder, const CFormattedString& p_Text )
		{
			vector< CFormattedString >	paragraphs = p_Text.SplitParts();
			BOOST_FOREACH( CFormattedString& fs, paragraphs )
			{
				ProcessParagraph( p_Builder, fs );
			}
		}
	}
}
//--------------------------------------------------------------------
FK2DEngine::CBitmap FK2DEngine::CreateText( const std::wstring& p_Text,
			const std::wstring& p_FontName, unsigned int p_unFontHeight,
		int p_nLineSpacing, unsigned int p_unMaxWidth, 
		ENUM_TextAlign p_eAlign, unsigned int p_unFontFlags /* = 0 */ )
{
	if( p_nLineSpacing <= -static_cast< int >( p_unFontHeight ) )
	{
		throw std::logic_error( "行空间不允许小于行间距" );
	}
	CFormattedString fs( boost::replace_all_copy( p_Text, L"\r\n", L"\n" ), p_unFontFlags );
	if( fs.Length() == 0 )
	{
		return CBitmap( p_unMaxWidth, p_unFontHeight );
	}

	CTextBlockBuilder Builder( p_FontName, p_unFontHeight, p_nLineSpacing, p_unMaxWidth, p_eAlign );

	ProcessText( Builder, fs );

	return Builder.Result();
}
//--------------------------------------------------------------------
FK2DEngine::CBitmap FK2DEngine::CreateText( const std::wstring& p_Text,
	const std::wstring& p_FontName, unsigned int p_unFontHeight, unsigned int p_unFontFlags /* = 0 */ )
{
	CFormattedString fs( boost::replace_all_copy( p_Text, L"\r\n", L"\n" ), p_unFontFlags );
	if( fs.Length() == 0 )
	{
		return CBitmap( 1, p_unFontHeight );
	}

	vector< CFormattedString >  Lines = fs.SplitLines();

	CBitmap bitmap( 1, Lines.size() * p_unFontHeight );

	for( unsigned int i = 0; i < Lines.size(); ++i )
	{
		if( Lines[i].Length() == 0 )
		{
			continue;
		}

		unsigned int x = 0;
		std::vector< CFormattedString > parts = Lines[i].SplitParts();
		for( unsigned int p = 0; p < parts.size(); ++p )
		{
			const CFormattedString& part = parts[p];

			if( part.Length() == 1 && part.EntityAt( 0 ) )
			{
				FK2DEngine::CBitmap Entity = EntityBitmap( part.EntityAt( 0 ) );
				MultiplyBitmapAlpha( Entity, part.ColorAt(0).Alpha() );
				bitmap.Resize( max( bitmap.Width(), x + Entity.Width() ), bitmap.Height() );
				bitmap.Insert( Entity, x, i * p_unFontHeight );
				x += Entity.Width();
				continue;
			}

			assert( part.Length() > 0 );
			std::wstring UnformattedText = part.Unformat();
			unsigned int unPartWidth = TextWidth( UnformattedText, 
				p_FontName, p_unFontHeight, part.FlagAt( 0 ) );
			bitmap.Resize( max( bitmap.Width(), x + unPartWidth ), bitmap.Height() );
			FKDrawText( bitmap, UnformattedText, x, i * p_unFontHeight,
				part.ColorAt( 0 ), p_FontName, p_unFontHeight, part.FlagAt( 0 ) );
			x += unPartWidth;
		}
	}

	return bitmap;
}
//--------------------------------------------------------------------
namespace
{
	std::map< std::wstring, boost::shared_ptr< FK2DEngine::CBitmap > >	Entityes;
}
//--------------------------------------------------------------------
void FK2DEngine::RegisterEntity( const std::wstring& p_Name, const FK2DEngine::CBitmap& p_Replacement )
{
	Entityes[p_Name].reset( new CBitmap(p_Replacement) );
}
//--------------------------------------------------------------------
bool FK2DEngine::IsEntity( const std::wstring& p_Name )
{
	return Entityes[p_Name];
}
//--------------------------------------------------------------------
const FK2DEngine::CBitmap& FK2DEngine::EntityBitmap( const std::wstring& p_Name )
{
	boost::shared_ptr< FK2DEngine::CBitmap >& ptr = Entityes[p_Name];
	if( !ptr )
	{
		throw std::runtime_error( "无法找到的实体 " + FK2DEngine::WstringToUTF8( p_Name ) );
	}
	return *ptr;
}