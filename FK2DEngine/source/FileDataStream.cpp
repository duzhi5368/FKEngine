/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	FileDataStream
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../include/FileDataStream.h"
#include "../include/Exception.h"
#include "../include/LogManager.h"
#include <fstream>

//--------------------------------------------------------------------
namespace FK2DEngine
{
	//--------------------------------------------------------------------
	CFileDataStream::CFileDataStream( std::ifstream* p_pStream, bool p_bFreeOnClose /* = true */ )
		: CDataStream()
		, m_pStream( p_pStream )
		, m_bFreeOnClose( p_bFreeOnClose )
		, m_pNormalStream( NULL )
	{
		if( m_pStream == NULL )
		{
			return;
		}

		m_pStream->seekg( 0, std::ios_base::end );
		m_unSize = m_pStream->tellg();
		m_pStream->seekg( 0, std::ios_base::beg );
	}
	//--------------------------------------------------------------------
	CFileDataStream::CFileDataStream( const std::wstring& p_szName, std::ifstream* p_pStream, bool p_bFreeOnClose )
		: CDataStream( p_szName )
		, m_pStream( p_pStream )
		, m_bFreeOnClose( p_bFreeOnClose )
		, m_pNormalStream( NULL )
	{
		if( m_pStream == NULL )
		{
			return;
		}

		m_pStream->seekg( 0, std::ios_base::end );
		m_unSize = m_pStream->tellg();
		m_pStream->seekg( 0, std::ios_base::beg );
	}
	//--------------------------------------------------------------------
	CFileDataStream::CFileDataStream( const std::wstring& p_szName, std::ifstream* p_pStream,
		unsigned int p_unSize, bool p_bFreeOnClose )
		: CDataStream( p_szName )
		, m_pStream( p_pStream )
		, m_bFreeOnClose( p_bFreeOnClose )
		, m_pNormalStream( NULL )
	{
		m_unSize	= p_unSize;
	}
	//--------------------------------------------------------------------
	CFileDataStream::CFileDataStream( const std::wstring& p_szName, char* p_pBuf, 
		unsigned int p_unSize, bool p_bFreeOnClose )
		: CDataStream( p_szName )
		, m_pStream( NULL )
		, m_bFreeOnClose( p_bFreeOnClose )
		, m_pNormalStream( p_pBuf )
	{
		m_unSize	= p_unSize;
		m_pCurrent	= p_pBuf;
	}
	//--------------------------------------------------------------------
	CFileDataStream::~CFileDataStream()
	{
		Close();
	}
	//--------------------------------------------------------------------
	unsigned int CFileDataStream::Read( void* p_pBuf, unsigned int p_unCount )
	{
		if( m_pStream )
		{
			m_pStream->read( static_cast< char* >( p_pBuf ), static_cast< int >( p_unCount ) );
			return m_pStream->gcount();
		}
		else
		{
			// 若未读到文件尾
			if( !Eof() )
			{
				memcpy( p_pBuf, m_pNormalStream, p_unCount );
				m_pCurrent += p_unCount;
				return p_unCount;
			}
			else
			{
				memcpy( p_pBuf, m_pNormalStream, m_unSize - ( m_pCurrent - m_pNormalStream ) );
				m_pCurrent = m_pNormalStream + m_unSize;
				return (m_unSize - ( m_pCurrent - m_pNormalStream ));
			}
		}
	}
	//--------------------------------------------------------------------
	unsigned int CFileDataStream::ReadLine( char* p_pBuf, unsigned int p_unMaxCount,
		const std::wstring& p_Delim /*= L"\n"*/ )
	{
		// 检查分割符合理性
		if( p_Delim.empty() )
		{
			FK_EXCEPTION( CException::eEC_InvalidParams, L"分割串不可为空",
				L"CFileDataStream::ReadLine" );
		}
		if( p_Delim.size() > 1 )
		{
			SSLogManager::Instance()->LogMessage( L"警告：CFileDataStream::ReadLine - 只使用分割串的第一个字符" );
		}
		bool bTrimCR = false;
		if( p_Delim.at( 0 ) == '\n' )
		{
			bTrimCR = true;
		}
		char cDelim[32];
		Wide2Ansi( cDelim, p_Delim.c_str(), p_Delim.length() );

		if( m_pStream != NULL )
		{
			m_pStream->getline( p_pBuf, static_cast< std::streamsize >( p_unMaxCount + 1 ), cDelim[0] );
			unsigned int unRet = m_pStream->gcount();

			if( m_pStream->eof() )
			{

			}
			else
				if( m_pStream->fail() )
				{
					if( unRet == p_unMaxCount )
					{
						m_pStream->clear();
					}
					else
					{
						FK_EXCEPTION( CException::eEC_ErrorInternal, L"产生数据流错误", L"CFileDataStream::ReadLine" );
					}
				}
				else
				{
					--unRet;
				}

				if( bTrimCR && p_pBuf[unRet - 1] == '\r' )
				{
					--unRet;
					p_pBuf[unRet] = '\0';
				}

				return unRet;
		}
		else if( m_pNormalStream != NULL )
		{
			if( !Eof() )
			{
				unsigned int count = 0;
				char *pos = strchr((char*)m_pCurrent, '\r');

				if (pos == NULL)
				{
					count = unsigned int((char*)m_pNormalStream + m_unSize - (char*)m_pCurrent);
				}
				else
				{
					count = unsigned int(pos - (char*)m_pCurrent);
				}

				if (count >= p_unMaxCount)
				{
					count = p_unMaxCount - 1;
				}

				memcpy(p_pBuf, m_pCurrent, count);
				p_pBuf[count] = '\0';

				m_pCurrent += count;
				if (*m_pCurrent == '\r')
				{
					m_pCurrent += 2;	// 跳过 '\r' 和 '\n'
				}

				return count;
			}
			else
			{
				return 0;
			}
		}

		return 0;
	}
	//--------------------------------------------------------------------
	void CFileDataStream::Skip( long p_lCount )
	{
		if( m_pStream != NULL )
		{
			m_pStream->clear();
			m_pStream->seekg( static_cast< std::ifstream::pos_type >( p_lCount ), std::ios::cur );
		}
		else
		{
			m_pCurrent += p_lCount;
		}
	}
	//--------------------------------------------------------------------
	void CFileDataStream::Seek( unsigned int p_unPos )
	{
		if( m_pStream != NULL )
		{
			m_pStream->clear();
			m_pStream->seekg( static_cast< std::streamoff >( p_unPos ), std::ios::beg );
		}
		else
		{
			m_pCurrent = m_pNormalStream + p_unPos;
		}
	}
	//--------------------------------------------------------------------
	unsigned int CFileDataStream::Tell() const
	{
		if( m_pStream != NULL )
		{
			m_pStream->clear();
			return m_pStream->tellg();
		}
		else
		{
			return (m_pCurrent - m_pNormalStream);
		}
	}
	//--------------------------------------------------------------------
	bool CFileDataStream::Eof() const
	{
		if( m_pStream != NULL )
		{
			return m_pStream->eof();
		}
		else
		{
			return m_pCurrent >= m_pNormalStream + m_unSize;
		}
	}
	//--------------------------------------------------------------------
	void CFileDataStream::Close()
	{
		if( m_pStream )
		{
			m_pStream->close();

			if( m_bFreeOnClose )
			{
				delete m_pStream;
				m_pStream = NULL;
			}
		}
		else if( m_pNormalStream )
		{
			m_pCurrent = m_pNormalStream + m_unSize;
			if( m_bFreeOnClose )
			{
				delete m_pNormalStream;
				m_pNormalStream = NULL;
			}
		}
	}
	//--------------------------------------------------------------------
}