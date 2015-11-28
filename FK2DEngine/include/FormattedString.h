/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	FormattedString
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "Color.h"
#include "StringConversion.h"
#include "../../depend/boost/cstdint.hpp"
#include "../../depend/boost/foreach.hpp"
#include "../../depend/boost/variant.hpp"
#include <stdexcept>
#include <utility>
#include <vector>
#include <cwchar>
//--------------------------------------------------------------------
namespace FK2DEngine
{
	class CFormattedString
	{
		struct SFormattedChar
		{
		public:
			wchar_t				m_Wc;
			FK2DEngine::CColor	m_Color;
			unsigned int		m_nFlags;
			std::wstring		m_szEntity;
		public:
			bool SameStyleAs( const SFormattedChar& p_Other ) const
			{
				return m_Wc && p_Other.m_Wc && m_Color == p_Other.m_Color && m_nFlags == p_Other.m_nFlags;
			}
		};

		typedef std::pair< std::wstring, unsigned int >		SimpleImpl;
		typedef std::vector< SFormattedChar >				FancyImpl;
	private:
		static unsigned int Flags( int p_B, int p_U, int p_I )
		{
			unsigned int unFlags = 0;
			if( p_B > 0 )
			{
				unFlags |= EFF_Bold;
			}
			if( p_U > 0 )
			{
				unFlags |= EFF_Underline;
			}
			if( p_I > 0 )
			{
				unFlags |= EFF_Italic;
			}
			return unFlags;
		}
	private:
		boost::variant< SimpleImpl, FancyImpl >			m_Impl;
	public:
		CFormattedString()
		{

		}

		explicit CFormattedString( const std::wstring& p_Html, unsigned int p_unBaseFlags )
		{
			if( p_Html.find_first_of( L"<&" ) == std::wstring::npos )
			{
				m_Impl = SimpleImpl( p_Html, p_unBaseFlags );
				return;
			}

			FancyImpl chars;

			unsigned int unPos = 0;
			int b = ( p_unBaseFlags & EFF_Bold ) ? 1 : 0;
			int u = ( p_unBaseFlags & EFF_Underline ) ? 1 : 0;
			int i = ( p_unBaseFlags & EFF_Italic ) ? 1 : 0;
			std::vector< FK2DEngine::CColor >	vecColor;
			vecColor.push_back( 0xffffffff );
			while( unPos < p_Html.length() )
			{
				if( p_Html.substr( unPos, 3 ) == L"<b>" )
				{
					b += 1;
					unPos += 3;
					continue;
				}
				if( p_Html.substr( unPos, 4) == L"</b>" )
				{
					b -= 1;
					unPos += 4;
					continue;
				}
				if( p_Html.substr( unPos, 3 ) == L"<u>" )
				{
					u += 1;
					unPos += 3;
					continue;
				}
				if( p_Html.substr( unPos, 4 ) == L"</u>" )
				{
					u -= 1;
					unPos += 4;
					continue;
				}
				if( p_Html.substr( unPos, 3 ) == L"<i>" )
				{
					i += 1;
					unPos += 3;
					continue;
				}
				if( p_Html.substr( unPos, 4 ) == L"</i>" )
				{
					i -= 1;
					unPos += 4;
					continue;
				}
				if( p_Html.length() >= unPos + 10 &&
					p_Html.substr( unPos, 3 ) == L"<c=" &&
					p_Html.at( unPos + 9 ) == L'>' )
				{
					using namespace std;
					boost::uint32_t Rgb = std::wcstoul( p_Html.c_str() + unPos + 3, 0, 16 );
					vecColor.push_back( 0xff000000 | Rgb );
					unPos += 10;
					continue;
				}
				if( p_Html.length() >= unPos + 12 &&
					p_Html.substr( unPos, 3 ) == L"<c=" &&
					p_Html.at( unPos + 11 ) == L'>' )
				{
					using namespace std;
					boost::uint32_t Argb = std::wcstoul( p_Html.c_str() + unPos + 3, 0, 16 );
					vecColor.push_back( Argb );
					unPos += 12;
					continue;
				}
				if( p_Html.substr( unPos, 4 ) == L"</c>" )
				{
					vecColor.pop_back();
					unPos += 4;
					continue;
				}
				if( p_Html.substr( unPos, 4 ) == L"&lt;" )
				{
					SFormattedChar fc = { L'<', vecColor.back(), Flags( b, u, i ) };
					chars.push_back( fc );
					unPos += 4;
					continue;
				}
				if( p_Html.substr( unPos, 4 ) == L"&gt;" )
				{
					SFormattedChar fc = { L'>', vecColor.back(), Flags( b, u, i ) };
					chars.push_back( fc );
					unPos += 4;
					continue;
				}
				if( p_Html.substr( unPos, 5 ) == L"&amp;" )
				{
					SFormattedChar fc = { L'&', vecColor.back(), Flags( b, u, i ) };
					chars.push_back( fc );
					unPos += 5;
					continue;
				}
				if( p_Html[unPos] == L'&' && unPos < p_Html.length() - 1 )
				{
					unsigned int nEndOfEntity = unPos + 1;
					while( p_Html[nEndOfEntity] != L';' )
					{	
						using namespace std;
						if( ! iswalnum( static_cast< wint_t >( p_Html[nEndOfEntity] ) ) )
						{
							goto LabelNormalChar;
						}
						nEndOfEntity += 1;
						if( nEndOfEntity >= p_Html.size() )
						{
							goto LabelNormalChar;
						}
					}

					SFormattedChar fc = { 0, vecColor.back(), 0, 
						std::wstring( p_Html.begin() + unPos + 1, p_Html.begin() + nEndOfEntity ) };
					if( !IsEntity( fc.m_szEntity ) )
					{
						goto LabelNormalChar;
					}
					chars.push_back( fc );
					unPos = nEndOfEntity + 1;
					continue;
				}
LabelNormalChar:
				SFormattedChar fc = { p_Html[unPos], vecColor.back(), Flags( b, u, i ) };
				chars.push_back( fc );
				unPos += 1;
			}

			m_Impl = chars;
		}

		std::wstring Unformat() const
		{
			if( const SimpleImpl* pSimpl = boost::get< SimpleImpl >(&m_Impl) )
			{
				return pSimpl->first;
			}

			const FancyImpl& chars = boost::get< FancyImpl >( m_Impl );
			std::wstring Result( Length(), L'' );
			for( unsigned int i = 0; i < chars.size(); ++i )
			{
				Result[i] = chars[i].m_Wc;
			}
			return Result;
		}

		unsigned int Length() const
		{
			if( const SimpleImpl* pSimpl = boost::get< SimpleImpl >(&m_Impl) )
			{
				return pSimpl->first.size();
			}

			const FancyImpl& chars = boost::get< FancyImpl >( m_Impl );
			return chars.size();
		}

		const wchar_t* EntityAt( unsigned int p_unIndex ) const
		{
			if( m_Impl.which() == 0 )
			{
				return 0;
			}

			const FancyImpl& chars = boost::get< FancyImpl >( m_Impl );
			if( chars[p_unIndex].m_Wc != 0 || chars[p_unIndex].m_szEntity.empty() )
			{
				return 0;
			}
			return chars[p_unIndex].m_szEntity.c_str();
		}

		wchar_t CharAt( unsigned int p_unIndex ) const
		{
			if( const SimpleImpl* pSimpl = boost::get< SimpleImpl >(&m_Impl) )
			{
				return pSimpl->first[p_unIndex];
			}

			const FancyImpl& chars = boost::get< FancyImpl >( m_Impl );
			return chars[p_unIndex].m_Wc;
		}

		unsigned int FlagAt( unsigned int p_unIndex ) const
		{
			if( const SimpleImpl* pSimpl = boost::get< SimpleImpl >(&m_Impl) )
			{
				return pSimpl->second;
			}

			const FancyImpl& chars = boost::get< FancyImpl >( m_Impl );
			return chars[p_unIndex].m_nFlags;
		}

		FK2DEngine::CColor ColorAt( unsigned int p_unIndex )const
		{
			if( m_Impl.which() == 0 )
			{
				return CColor::WHITE;
			}

			const FancyImpl& chars = boost::get< FancyImpl >( m_Impl );
			return chars[p_unIndex].m_Color;
		}

		CFormattedString Range( unsigned int p_unBegin, unsigned int p_unEnd ) const
		{
			CFormattedString Result;
			if( const SimpleImpl* pSimpl = boost::get< SimpleImpl >(&m_Impl) )
			{
				Result.m_Impl = SimpleImpl( std::wstring( pSimpl->first.begin() + p_unBegin,
					pSimpl->first.begin() + p_unEnd ), pSimpl->second );
			}
			else
			{
				const FancyImpl& chars = boost::get< FancyImpl >( m_Impl );
				Result.m_Impl = FancyImpl( chars.begin() + p_unBegin, chars.begin() + p_unEnd );
			}
			return Result;
		}

		std::vector< CFormattedString > SplitLines() const
		{
			std::vector< CFormattedString > Result;
			unsigned int unBegin = 0;
			for( unsigned int unCur = 0; unCur < Length(); ++unCur )
			{
				if( CharAt( unCur ) == L'\n')
				{
					Result.push_back( Range( unBegin, unCur ) );
					unBegin = unCur + 1;
				}
			}

			Result.push_back( Range( unBegin, Length() ) );
			return Result;
		}

		std::vector< CFormattedString > SplitParts() const
		{
			if( m_Impl.which() == 0 )
			{
				return std::vector< CFormattedString >( 1, *this );
			}
			
			const FancyImpl& chars = boost::get< FancyImpl >( m_Impl );
			std::vector< CFormattedString > Result;
			unsigned int unBegin = 0;
			for( unsigned int unCur = 1; unCur < Length(); ++unCur )
			{
				if( !chars[unBegin].SameStyleAs( chars[unCur] ) )
				{
					Result.push_back( Range( unBegin, unCur ) );
					unBegin = unCur;
				}
			}
			Result.push_back( Range( unBegin, Length() ) );
			return Result;
		}
	};
}