/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	DataStream
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../include/DataStream.h"
#include "../include/LogManager.h"
#include "../include/Exception.h"
#include "../include/StringConversion.h"
#include <limits>

//--------------------------------------------------------------------
namespace FK2DEngine
{
	//--------------------------------------------------------------------
	template< typename T >
	CDataStream& CDataStream::operator >> ( T& p_Val )
	{
		read( static_cast< void* >(&p_Val), sizeof(T) );
		return *this;
	}
	//--------------------------------------------------------------------
	std::wstring CDataStream::GetLine( bool p_bTrimAfter /* = true */ )
	{
		char tmpBuf[ FK_STREAM_TEMP_SIZE ];
		std::wstring szRetString;
		unsigned int unReadCount;

		// 未找到分隔符，则持续循环
		while( ( unReadCount = Read( tmpBuf, FK_STREAM_TEMP_SIZE - 1 )) != 0 )
		{
			tmpBuf[ unReadCount ]		= '\0';

			char* p = strchr( tmpBuf, '\n' );

			if( p != 0 )
			{
				Skip( (long)( p + 1 - tmpBuf - unReadCount ) );
				*p = '\0';
			}

			wchar_t tmpUnicodeBuf[ FK_STREAM_TEMP_SIZE ];
			Ansi2Wide( tmpUnicodeBuf, tmpBuf, strlen(tmpBuf) );
			szRetString += tmpUnicodeBuf;

			if( p != 0 )
			{
				if( szRetString.length() && szRetString[szRetString.length() - 1] == '\r' )
				{
					szRetString.erase( szRetString.length() - 1, 1 );
				}
				break;
			}
		}

		if( p_bTrimAfter )
		{
			Trim( szRetString );
		}

		return szRetString;
	}
	//--------------------------------------------------------------------
	unsigned int CDataStream::ReadLine( char* p_pBuf, unsigned int p_unMaxCount, const std::wstring& p_Delim )
	{
		bool bTrimCR = false;
		if( p_Delim.find_first_of('\n') != std::wstring::npos )
		{
			bTrimCR = true;
		}

		char tmpBuf[ FK_STREAM_TEMP_SIZE ];
		unsigned int unChunkSize = std::min< unsigned int >( p_unMaxCount, (unsigned int)(FK_STREAM_TEMP_SIZE - 1 ) );
		unsigned int unTotalCount = 0;
		unsigned int unReadCount = 0;
		while( unChunkSize && ( unReadCount = Read( tmpBuf, unChunkSize ) ) )
		{
			tmpBuf[ unReadCount ] = '\0';

			// 查找第一个分割符
			char cDelim[32];
			Wide2Ansi( cDelim, p_Delim.c_str(), p_Delim.length() );
			unsigned int unPos = strcspn( tmpBuf, cDelim );

			if( unPos < unReadCount )
			{
				Skip( (long)( unPos + 1 - unReadCount ) );
			}

			if( p_pBuf )
			{
				memcpy( p_pBuf + unTotalCount, tmpBuf, unPos );
			}

			unTotalCount += unPos;

			if( unPos < unReadCount )
			{
				if( bTrimCR && unTotalCount && p_pBuf[unTotalCount - 1] == '\r' )
				{
					--unTotalCount;
				}
				break;
			}

			unChunkSize = std::min< unsigned int >( p_unMaxCount - unTotalCount, ( unsigned int )( FK_STREAM_TEMP_SIZE - 1 ) );
		}

		p_pBuf[unTotalCount]	= '\0';

		return unTotalCount;
	}
	//--------------------------------------------------------------------
	unsigned int CDataStream::SkipLine( const std::wstring& p_Delim /* = */ )
	{
		char tmpBuf[ FK_STREAM_TEMP_SIZE ];
		unsigned int unTotal = 0;
		unsigned int unReadCount = 0;

		while( ( unReadCount = Read( tmpBuf, FK_STREAM_TEMP_SIZE - 1  ) ) != 0 )
		{
			tmpBuf[ unReadCount ]	= '\0';

			char cDelim[32];
			Wide2Ansi( cDelim, p_Delim.c_str(), p_Delim.length() );
			unsigned int unPos = strcspn( tmpBuf, cDelim );

			if( unPos < unReadCount )
			{
				Skip((long)( unPos + 1 - unReadCount ) );
				unTotal += unPos + 1;
				break;
			}

			unTotal += unReadCount;
		}
		return unTotal;
	}
	//--------------------------------------------------------------------
	std::wstring CDataStream::GetAllString()
	{
		char* pBuf = new char[m_unSize + 1];
		Seek( 0 );
		Read( pBuf, m_unSize );
		pBuf[ m_unSize ] = '\0';

		wchar_t* p_UnicodeBuf = new wchar_t[ m_unSize + 1 ];
		Ansi2Wide( p_UnicodeBuf, pBuf, m_unSize );
		std::wstring szOut( p_UnicodeBuf );
		delete[]pBuf;
		delete[]p_UnicodeBuf;

		return szOut;
	}
	//--------------------------------------------------------------------
}
