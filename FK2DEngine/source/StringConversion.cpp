/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	StringConversion
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../include/Platform.h"
#include "../include/StringConversion.h"
#include <cstddef>
#include <cstdlib>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <iostream>
#include <sstream>
#include <assert.h>

#ifndef FK_WINDOWS
#include "../include/IConvert.h"
#endif
//--------------------------------------------------------------------
namespace FK2DEngine
{
	static const std::wstring EMPTY_STRING		= std::wstring( L"" );
}
//--------------------------------------------------------------------
using namespace std;
#pragma warning( disable:4996 )
//--------------------------------------------------------------------
#ifndef FK_WINDOWS
	namespace 
	{
		extern const char UTF_8[] = "UTF-8";
	#ifdef FK_BIG_ENDIAN
		extern const char UCS_4_INTERNAL[] = "UCS-4BE";
	#else
		extern const char UCS_4_INTERNAL[] = "UCS-4LE";
	#endif
	}

	// 转换 std::string 为 std::wstring
	std::wstring FK2DEngine::UTF8ToWstring( const std::string& p_UTF8 )
	{
		return Iconvert< wstring, UCS_4_INTERNAL, UTF_8 >( p_UTF8 );
	}
	// 转换 std::wstring 为 std::string
	std::string FK2DEngine::WstringToUTF8( const std::wstring& p_WString )
	{
		return Iconvert< wstring, UTF_8, UCS_4_INTERNAL >( p_WString );
	}

	#ifdef FK_MAC
		namespace
		{
			extern const char MACROMAN[] = "MacRoman";
			extern const char UCS_2_INTERNAL[] = "UCS-2-INTERNAL";
		}

		namespace FK2DEngine
		{
			wstring MacRomanToWstring( const string& p_UTF8 )
			{
				return iconvert< wstring, UCS_4_INTERNAL, MACROMAN >( p_UTF8 );
			}

			vector< unsigned short > WstringToUniChars( const wstring& p_WString )
			{
				return iconvert< vector< unsigned short >, UCS_2_INTERNAL, UCS_4_INTERNAL >( p_WString );
			}
		}
	#endif
#else
	#ifndef NOMINMAX
	#define NOMINMAX
	#endif

	#include <Windows.h>
	wstring FK2DEngine::UTF8ToWstring( const string& p_UTF8 )
	{
		vector< wchar_t > buffer( p_UTF8.size() + 1 );
		MultiByteToWideChar( CP_UTF8, 0, p_UTF8.c_str(), p_UTF8.size() + 1, &buffer[0], buffer.size() );
		return &buffer[0];
	}

	string FK2DEngine::WstringToUTF8( const wstring& p_WString )
	{
		unsigned int unSize = WideCharToMultiByte( CP_UTF8, 0, p_WString.c_str(), p_WString.size(), 0, 0, 0, 0 );
		vector< char > buffer( unSize + 1 );
		WideCharToMultiByte( CP_UTF8, 0, p_WString.c_str(), p_WString.size(), &buffer[0], buffer.size(), 0, 0 );
		return &buffer[0];
	}
#endif


// 本地解码方式 转换 std::string 为 std::wstring
std::wstring FK2DEngine::Widen( const std::string& p_UTF8 )
{
#ifdef FK_X
	setlocale( LC_ALL, "" );
#endif

	size_t unWideLen = std::mbstowcs( 0, p_UTF8.c_str(), 0 );
	if( unWideLen == static_cast< size_t >( -1 ) )
	{
		throw std::runtime_error( "转换wstring为string失败：" + p_UTF8 );
	}

	vector< wchar_t > vecBuf( unWideLen + 1 );
	mbstowcs( &vecBuf.front(), p_UTF8.c_str(), vecBuf.size() );

	return wstring( vecBuf.begin(), vecBuf.end() - 1 );
}
// 本地解码方式 转换 std::wstring 为 std::string
std::string FK2DEngine::Narrow( const std::wstring& p_WString )
{
	size_t unNarrowLen = std::wcstombs( 0, p_WString.c_str(), 0 );
	if( unNarrowLen == static_cast< size_t >( -1 ) )
	{
		throw std::runtime_error( "转换wstring为string失败：" + string( p_WString.begin(), p_WString.end() ) );
	}
	vector< char > vecBuf( unNarrowLen + 1 );
	wcstombs( &vecBuf.front(), p_WString.c_str(), vecBuf.size() );

	return string( vecBuf.begin(), vecBuf.end() - 1 );
}
//--------------------------------------------------------------------
void FK2DEngine::Trim( std::wstring& p_Str, bool p_bLeft /* = true */, bool p_bRight /* = true */ )
{
	static const std::wstring s_Delims = L" \t\r";

	if( p_bRight )
	{
		p_Str.erase( p_Str.find_last_not_of( s_Delims ) + 1 );
	}
	if( p_bLeft )
	{
		p_Str.erase( 0, p_Str.find_first_not_of( s_Delims ) );
	}
}
//--------------------------------------------------------------------
FK2DEngine::StringVector FK2DEngine::Split( const std::wstring& p_Str, std::wstring& p_Delims, 
				   unsigned int p_unMaxSplits /* = 0 */ )
{
	if( p_Delims.empty() )
	{
		p_Delims = L"\t\n\r ";
	}

	FK2DEngine::StringVector	Result;
	unsigned int	unNumSplits	= 0;
	size_t			ulStart = 0;
	size_t			ulPos = 0;

	do
	{
		ulPos	= p_Str.find_first_of( p_Delims, ulStart );

		if( ulPos == ulStart )
		{
			ulStart = ulPos + 1;
		}
		else if( (ulPos == std::wstring::npos) || ( p_unMaxSplits && unNumSplits == p_unMaxSplits ) )
		{
			Result.push_back( p_Str.substr( ulStart ) );
			break;
		}
		else
		{
			Result.push_back( p_Str.substr( ulStart, ulPos - ulStart ) );
			ulStart = ulPos + 1;
		}

		ulStart = p_Str.find_last_not_of( p_Delims, ulStart );
		++unNumSplits;
	}while( ulPos != std::wstring::npos );

	return Result;
}
//--------------------------------------------------------------------
void FK2DEngine::ToUpperCase( std::wstring& p_Str )
{
	std::transform( p_Str.begin(), p_Str.end(), p_Str.begin(), toupper );
}
//--------------------------------------------------------------------
void FK2DEngine::ToLowerCase( std::wstring& p_Str )
{
	std::transform( p_Str.begin(), p_Str.end(), p_Str.begin(), tolower );
}
//--------------------------------------------------------------------
bool FK2DEngine::IsStartWith( const std::wstring& p_Str,
			const std::wstring& p_Pattern, bool p_bCaseSensitive /* = false */ )
{
	size_t	ulThisLen		= p_Str.length();
	size_t	ulPatternLen	= p_Pattern.length();
	if(( ulThisLen < ulPatternLen ) || ( ulPatternLen == 0 ))
	{
		return false;
	}

	std::wstring szStartOfThis = p_Str.substr( 0, ulPatternLen );
	if( !p_bCaseSensitive )
	{
		FK2DEngine::ToLowerCase( szStartOfThis );
	}

	return( szStartOfThis == p_Pattern );
}
//--------------------------------------------------------------------
bool FK2DEngine::IsEndWith( const std::wstring& p_Str,
			const std::wstring& p_Pattern, bool p_bCaseSensitive /* = false */ )
{
	size_t	ulThisLen		= p_Str.length();
	size_t	ulPatternLen	= p_Pattern.length();
	if(( ulThisLen < ulPatternLen ) || ( ulPatternLen == 0 ))
	{
		return false;
	}

	std::wstring szEndOfThis = p_Str.substr( ulThisLen - ulPatternLen, ulPatternLen );
	if( !p_bCaseSensitive )
	{
		FK2DEngine::ToLowerCase( szEndOfThis );
	}

	return( szEndOfThis == p_Pattern );
}
//--------------------------------------------------------------------
std::wstring FK2DEngine::StandardisePath( const std::wstring& p_Init )
{
	std::wstring szPath = p_Init;

	std::replace( szPath.begin(), szPath.end(), '\\', '/' );
	if( szPath[ szPath.length() - 1 ] != '/' )
	{
		szPath += '/';
	}

	return szPath;
}
//--------------------------------------------------------------------
void FK2DEngine::SplitFilename( const std::wstring& p_QualifiedName,
				std::wstring& p_szOutBasename, std::wstring& p_szOutPath )
{
	std::wstring szPath	= p_QualifiedName;

	std::replace( szPath.begin(), szPath.end(), '\\', '/' );

	size_t i = szPath.find_last_of( '/' );

	if( i == std::wstring::npos )
	{
		p_szOutPath	= L"";
		p_szOutBasename = p_QualifiedName;
	}
	else
	{
		p_szOutBasename = szPath.substr( i + 1, szPath.size() - i - 1 );
		p_szOutPath = szPath.substr( 0, i + 1 );
	}
}
//--------------------------------------------------------------------
bool FK2DEngine::Match( const std::wstring& p_Str,
			const std::wstring& p_Pattern, bool p_bCaseSensitive /* = true */ )
{
	std::wstring tempStr = p_Str;
	std::wstring tempPattern	= p_Pattern;

	if( !p_bCaseSensitive )
	{
		FK2DEngine::ToLowerCase( tempStr );
		FK2DEngine::ToLowerCase( tempPattern );
	}

	std::wstring::const_iterator	IteStr	= tempStr.begin();
	std::wstring::const_iterator	ItePat	= tempPattern.begin();
	std::wstring::const_iterator	IteLastWildCard	= tempPattern.end();
	while( IteStr != tempStr.end() && ItePat != tempPattern.end() )
	{
		if( *ItePat	== '*' )
		{
			IteLastWildCard = ItePat;

			++ItePat;

			if( ItePat == tempPattern.end() )
			{
				IteStr = tempStr.end();
			}
			else
			{
				while( IteStr != tempStr.end() && (*IteStr != * ItePat) )
				{
					++IteStr;
				}
			}
		}
		else
		{
			if( *ItePat != *IteStr )
			{
				if( IteLastWildCard != tempPattern.end() )
				{
					ItePat = IteLastWildCard;
					IteLastWildCard = tempPattern.end();
				}
				else
				{
					return false;
				}
			}
			else
			{
				++ItePat;
				++IteStr;
			}
		}
	}

	if( ItePat == tempPattern.end() && IteStr == tempStr.end() )
	{
		return true;
	}
	else
	{
		return false;
	}

	return true;
}
//--------------------------------------------------------------------
std::wstring FK2DEngine::ToString( float p_fValue, unsigned short p_usPrecision /*= 6*/,
					  unsigned short p_usWidth /*= 0*/, char p_Fill /*= ' '*/, 
					  std::ios::fmtflags p_Flags /*= std::ios::fmtflags( 0 )*/ )
{
	FK2DEngine::StrStreamType	stream;
	stream.precision( p_usPrecision );
	stream.width( p_usWidth );
	stream.fill( p_Fill );
	if( p_Flags )
	{
		stream.setf( p_Flags );
	}

	stream << p_fValue;
	return stream.str();
}
//--------------------------------------------------------------------
std::wstring FK2DEngine::ToString( double p_dValue, unsigned short p_usPrecision /*= 6*/,
					  unsigned short p_usWidth /*= 0*/, char p_Fill /*= ' '*/, 
					  std::ios::fmtflags p_Flags /*= std::ios::fmtflags( 0 )*/ )
{
	FK2DEngine::StrStreamType	stream;
	stream.precision( p_usPrecision );
	stream.width( p_usWidth );
	stream.fill( p_Fill );
	if( p_Flags )
	{
		stream.setf( p_Flags );
	}

	stream << p_dValue;
	return stream.str();
}
//--------------------------------------------------------------------
std::wstring FK2DEngine::ToString( wchar_t p_Val )
{
	wchar_t temp[2];
	temp[0]	= p_Val;
	temp[1]	= '\0';
	return std::wstring( temp );
}
//--------------------------------------------------------------------
std::wstring FK2DEngine::ToString( int p_nVal, unsigned short p_usWidth /*= 0*/, 
		char p_Fill /*= ' '*/, std::ios::fmtflags p_Flags /*= std::ios::fmtflags( 0 )*/ )
{
	FK2DEngine::StrStreamType	stream;
	stream.width( p_usWidth );
	stream.fill( p_Fill );
	if( p_Flags )
	{
		stream.setf( p_Flags );
	}

	stream << p_nVal;
	return stream.str();
}
//--------------------------------------------------------------------
std::wstring FK2DEngine::ToString( size_t p_unVal, unsigned short p_usWidth /*= 0*/, 
								char p_Fill /*= ' '*/, std::ios::fmtflags p_Flags /*= std::ios::fmtflags( 0 )*/ )
{
	FK2DEngine::StrStreamType	stream;
	stream.width( p_usWidth );
	stream.fill( p_Fill );
	if( p_Flags )
	{
		stream.setf( p_Flags );
	}

	stream << p_unVal;
	return stream.str();
}
//--------------------------------------------------------------------
std::wstring FK2DEngine::ToString( unsigned long p_ulVal, unsigned short p_usWidth /*= 0*/, 
								char p_Fill /*= ' '*/, std::ios::fmtflags p_Flags /*= std::ios::fmtflags( 0 )*/ )
{
	FK2DEngine::StrStreamType	stream;
	stream.width( p_usWidth );
	stream.fill( p_Fill );
	if( p_Flags )
	{
		stream.setf( p_Flags );
	}

	stream << p_ulVal;
	return stream.str();
}
//--------------------------------------------------------------------
std::wstring FK2DEngine::ToString( long p_lVal, unsigned short p_usWidth /*= 0*/, 
								char p_Fill /*= ' '*/, std::ios::fmtflags p_Flags /*= std::ios::fmtflags( 0 )*/ )
{
	FK2DEngine::StrStreamType	stream;
	stream.width( p_usWidth );
	stream.fill( p_Fill );
	if( p_Flags )
	{
		stream.setf( p_Flags );
	}

	stream << p_lVal;
	return stream.str();
}
//--------------------------------------------------------------------
std::wstring FK2DEngine::ToString( bool p_bValue, bool p_bYesNo /*= false*/ )
{
	if( p_bValue )
	{
		return p_bYesNo ? L"yes" : L"true";
	}
	else
	{
		return p_bYesNo ? L"no" : L"false";
	}
}
//--------------------------------------------------------------------
std::wstring FK2DEngine::ToString( const StringVector& p_Value )
{
	FK2DEngine::StrStreamType stream;
	StringVector::const_iterator IteBegin = p_Value.begin();
	StringVector::const_iterator IteEnd = p_Value.end();
	StringVector::const_iterator Ite = IteBegin;

	for( ; Ite != IteEnd; ++Ite )
	{
		if( Ite != IteBegin )
		{
			stream << L" ";
		}

		stream << *Ite;
	}

	return stream.str();
}
//--------------------------------------------------------------------
float FK2DEngine::ParseFloat( const std::wstring& p_Value )
{
	char szBuf[1024];
	FK2DEngine::Wide2Ansi( szBuf, p_Value.c_str(), p_Value.length() );
	return static_cast< float >( atof( szBuf ) );
}
//--------------------------------------------------------------------
double FK2DEngine::ParseDouble( const std::wstring& p_Value )
{
	char szBuf[1024];
	FK2DEngine::Wide2Ansi( szBuf, p_Value.c_str(), p_Value.length() );
	return atof( szBuf );	
}
//--------------------------------------------------------------------
int FK2DEngine::ParseInt( const std::wstring& p_Value )
{
	char szBuf[1024];
	FK2DEngine::Wide2Ansi( szBuf, p_Value.c_str(), p_Value.length() );
	return atoi( szBuf );
}
//--------------------------------------------------------------------
unsigned int FK2DEngine::ParseUnsignedInt( const std::wstring& p_Value )
{
	char szBuf[1024];
	FK2DEngine::Wide2Ansi( szBuf, p_Value.c_str(), p_Value.length() );
	return static_cast< unsigned int >( strtoul( szBuf, 0, 10 ) );
}
//--------------------------------------------------------------------
long FK2DEngine::ParseLong( const std::wstring& p_Value )
{
	char szBuf[1024];
	FK2DEngine::Wide2Ansi( szBuf, p_Value.c_str(), p_Value.length() );
	return strtol( szBuf, 0, 10 );
}
//--------------------------------------------------------------------
unsigned long FK2DEngine::ParseUnsignedLong( const std::wstring& p_Value )
{
	char szBuf[1024];
	FK2DEngine::Wide2Ansi( szBuf, p_Value.c_str(), p_Value.length() );
	return strtol( szBuf, 0, 10 );
}
//--------------------------------------------------------------------
bool FK2DEngine::ParseBool( const std::wstring& p_Value )
{
	return ( p_Value == L"true" || p_Value == L"yes" );
}
//--------------------------------------------------------------------
FK2DEngine::StringVector FK2DEngine::ParseStringVector( const std::wstring& p_Value )
{
	std::wstring szDelims = L"\t\n\r ";
	return FK2DEngine::Split( p_Value, szDelims );
}
//--------------------------------------------------------------------
bool FK2DEngine::IsNumber( const std::wstring& p_Value )
{
	std::wistringstream	str;
	str.str( p_Value + L" " );
	float fst;
	str >> fst;
	return str.good();
}
//--------------------------------------------------------------------
void FK2DEngine::Ansi2Wide( wchar_t* p_pDest, const char* p_pSrc, size_t p_unLen )
{
	assert( p_pDest != NULL );
	assert( p_pSrc != NULL );
	assert( p_unLen >= 1 );

	MultiByteToWideChar( CP_ACP, 0, p_pSrc, -1, p_pDest, static_cast< int >( p_unLen ) );
	p_pDest[p_unLen] = L'\0';
}
//--------------------------------------------------------------------
void FK2DEngine::Wide2Ansi( char* p_pDest, const wchar_t* p_pSrc, size_t p_unLen )
{
	assert( p_pDest != NULL );
	assert( p_pSrc != NULL );
	assert( p_unLen >= 1 );

	WideCharToMultiByte( CP_ACP, 0, p_pSrc, -1, p_pDest, static_cast< int >( 2 * p_unLen ), 0, 0 );
	p_pDest[ 2 * p_unLen ] = '\0';
}
//--------------------------------------------------------------------
void FK2DEngine::AnsiString2Array( char* p_pDest, const std::string& p_Str )
{
	std::istringstream	Ism( p_Str );
	Ism >> p_pDest;
}	
//--------------------------------------------------------------------
void FK2DEngine::WideString2Array( wchar_t* p_pDest, const std::wstring& p_Str )
{
	std::wstringstream WIsm( p_Str );
	WIsm >> p_pDest;
}
//--------------------------------------------------------------------