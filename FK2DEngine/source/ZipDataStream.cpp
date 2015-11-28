/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	ZipDataStream
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../Include/ZipDataStream.h"
#include "../../Depend/zip114/zzip/zzip.h"

//--------------------------------------------------------------------
namespace FK2DEngine
{
	//--------------------------------------------------------------------
	CZipDataStream::CZipDataStream( ZZIP_FILE* p_pZipFile, unsigned int p_unUncompressedSize )
		: m_pZipFile( p_pZipFile )
	{
		m_unSize = p_unUncompressedSize;
	}
	//--------------------------------------------------------------------
	CZipDataStream::CZipDataStream( const std::wstring& p_szName, ZZIP_FILE* p_pZipFile,
		unsigned int p_unUncompressedSize )
		: CDataStream( p_szName )
		, m_pZipFile( p_pZipFile )
	{
		m_unSize = p_unUncompressedSize;
	}
	//--------------------------------------------------------------------
	CZipDataStream::~CZipDataStream()
	{
		Close();
	}
	//--------------------------------------------------------------------
	unsigned int CZipDataStream::Read( void* p_pBuf, unsigned int p_unCount )
	{
		return zzip_file_read( m_pZipFile, ( char* )p_pBuf, p_unCount );
	}
	//--------------------------------------------------------------------
	void CZipDataStream::Skip( long p_lCount )
	{
		zzip_seek( m_pZipFile, static_cast< zzip_off_t >( p_lCount ), SEEK_CUR );
	}
	//--------------------------------------------------------------------
	void CZipDataStream::Seek( unsigned int p_unPos )
	{
		zzip_seek( m_pZipFile, static_cast< zzip_off_t >( p_unPos ), SEEK_SET );
	}
	//--------------------------------------------------------------------
	unsigned int CZipDataStream::Tell() const
	{
		return zzip_tell( m_pZipFile );
	}
	//--------------------------------------------------------------------
	bool CZipDataStream::Eof() const
	{
		return ( zzip_tell( m_pZipFile ) >= static_cast< zzip_off_t >( m_unSize ) );
	}
	//--------------------------------------------------------------------
	void CZipDataStream::Close()
	{
		zzip_file_close( m_pZipFile );
	}
	//--------------------------------------------------------------------
}
//--------------------------------------------------------------------