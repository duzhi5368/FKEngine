/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	ZipArchive
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../include/ZipArchive.h"
#include "../include/LogManager.h"
#include "../include/StringConversion.h"
#include "../include/Exception.h"
#include "../Include/ZipDataStream.h"
#include "../../Depend/zip114/zzip/zzip.h"

//--------------------------------------------------------------------
namespace FK2DEngine
{
	//--------------------------------------------------------------------
	// 格式化 ZZip 错误
	std::wstring GetZipErrorDescription( zzip_error_t p_ZzipError )
	{
		std::wstring szErrorMsg;
		switch( p_ZzipError )
		{
		case ZZIP_NO_ERROR:
			break;
		case ZZIP_OUTOFMEM:
			szErrorMsg = L"Zip:内存溢出";
			break;
		case ZZIP_DIR_OPEN:
		case ZZIP_DIR_STAT: 
		case ZZIP_DIR_SEEK:
		case ZZIP_DIR_READ:
			szErrorMsg = L"Zip:无法读取Zip文档";
			break;            
		case ZZIP_UNSUPP_COMPR:
			szErrorMsg = L"Zip:不支持的压缩格式";
			break;            
		case ZZIP_CORRUPTED:
			szErrorMsg = L"Zip:Zip文档损坏";
			break;            
		default:
			szErrorMsg = L"Zip:未知错误";
			break;  
		}

		return szErrorMsg;
	}
	//--------------------------------------------------------------------
	CZipArchive::CZipArchive( const std::wstring& p_Name, const std::wstring& p_Type )
		: CArchive( p_Name, p_Type )
		, m_pZipDir( NULL )
	{
		
	}
	//--------------------------------------------------------------------
	CZipArchive::~CZipArchive()
	{
		Unload();
	}
	//--------------------------------------------------------------------
	bool CZipArchive::IsCaseSensitive() const
	{
		return false;
	}
	//--------------------------------------------------------------------
	void CZipArchive::Load()
	{
		if( !m_pZipDir )
		{
			zzip_error_t zzipError;

			char szBuf[ 1024 ];
			Wide2Ansi( szBuf, m_szName.c_str(), m_szName.length() );
			m_pZipDir = zzip_dir_open( szBuf, &zzipError );
			CheckZipError( zzipError, L"打开文件 " );

			ZZIP_DIRENT zzipEntry;
			while( zzip_dir_read( m_pZipDir, &zzipEntry ) )
			{
				wchar_t szBuf[ 1024 ];
				Ansi2Wide( szBuf, zzipEntry.d_name, strlen( zzipEntry.d_name ) );

				SFileInfo info;
				info.m_pArchive	= this;
				SplitFilename( std::wstring( szBuf ), info.m_szBaseName, info.m_szPath );

				if( info.m_szBaseName.empty() )
				{
					continue;
				}

				info.m_szFileName = std::wstring( szBuf );
				info.m_unCompressedSize = static_cast< unsigned int >( zzipEntry.d_csize );
				info.m_unUnCompressedSize = static_cast< unsigned int >( zzipEntry.st_size );

				m_pFileList.push_back( info );
			}
		}
	}
	//-------------------------------------------------------------------
	void CZipArchive::Unload()
	{
		if( m_pZipDir )
		{
			zzip_dir_close( m_pZipDir );
			m_pZipDir = NULL;
			m_pFileList.clear();
		}
	}
	//--------------------------------------------------------------------
	DataStreamPtr CZipArchive::Open( const std::wstring& p_FileName ) const
	{
		char szBuf[ 1024 ];
		Wide2Ansi( szBuf, p_FileName.c_str(), p_FileName.length() );

		ZZIP_FILE* pZipFile = zzip_file_open( m_pZipDir, szBuf, ZZIP_ONLYZIP | ZZIP_CASELESS );
		if( !pZipFile )
		{
			int nZerr = zzip_error( m_pZipDir );

			std::wstring szDesc = GetZipErrorDescription( ( zzip_error_t ) nZerr );

			SSLogManager::Instance()->LogMessage( m_szName + 
				L"不能打开文件" + p_FileName + L", 错误为" + szDesc );

			return DataStreamPtr();
		}

		ZZIP_STAT stat;
		zzip_dir_stat( m_pZipDir, szBuf, &stat, ZZIP_CASEINSENSITIVE );
		return DataStreamPtr( new CZipDataStream( p_FileName, pZipFile, static_cast< unsigned int >( stat.st_size ) ) );
	}
	//--------------------------------------------------------------------
	StringVectorPtr CZipArchive::List( bool p_bRecursive /*= true*/ )
	{
		StringVectorPtr ret = StringVectorPtr( new StringVector() );

		FileInfoList::iterator Ite = m_pFileList.begin();
		FileInfoList::const_iterator IteEnd = m_pFileList.end();

		for( ; Ite != IteEnd; ++Ite )
		{
			if( p_bRecursive || Ite->m_szPath.empty() )
			{
				ret->push_back( Ite->m_szFileName );
			}
		}
		return ret;
	}	
	//--------------------------------------------------------------------
	FileInfoListPtr CZipArchive::ListFileInfo( bool p_bRecursive /*= true*/ )
	{
		FileInfoList* pFileInfoList = new FileInfoList();

		FileInfoList::iterator Ite = m_pFileList.begin();
		FileInfoList::const_iterator IteEnd = m_pFileList.end();

		for( ; Ite != IteEnd; ++Ite )
		{
			if( p_bRecursive || Ite->m_szPath.empty() )
			{
				pFileInfoList->push_back( *Ite );
			}
		}

		return FileInfoListPtr( pFileInfoList );
	}
	//--------------------------------------------------------------------
	StringVectorPtr CZipArchive::Find( const std::wstring& p_Pattern, bool p_bRecursive /*= true*/ )
	{
		StringVectorPtr ret = StringVectorPtr( new StringVector() );

		FileInfoList::iterator Ite = m_pFileList.begin();
		FileInfoList::const_iterator IteEnd = m_pFileList.end();

		for( ; Ite != IteEnd; ++Ite )
		{
			if( p_bRecursive || Ite->m_szPath.empty() )
			{
				if( Match( Ite->m_szBaseName, p_Pattern, false ) )
				{
					ret->push_back( Ite->m_szFileName );
				}
			}
			else
			{
				if( Match( Ite->m_szFileName, p_Pattern, false ) )
				{
					ret->push_back( Ite->m_szFileName );
				}
			}
		}

		return ret;
	}
	//--------------------------------------------------------------------
	FileInfoListPtr CZipArchive::FindFileInfo( const std::wstring& p_Pattern, bool p_bRecursive /*= true*/ )
	{
		FileInfoListPtr ret = FileInfoListPtr( new FileInfoList() );

		FileInfoList::iterator Ite = m_pFileList.begin();
		FileInfoList::const_iterator IteEnd = m_pFileList.end();

		for( ; Ite != IteEnd; ++Ite )
		{
			if( p_bRecursive || Ite->m_szPath.empty() )
			{
				if( Match( Ite->m_szBaseName, p_Pattern, false ) )
				{
					ret->push_back( *Ite );
				}
			}
			else
			{
				if( Match( Ite->m_szFileName, p_Pattern, false ) )
				{
					ret->push_back( *Ite );
				}
			}
		}

		return ret;
	}
	//--------------------------------------------------------------------
	bool CZipArchive::Exists( const std::wstring& p_szFilename )
	{
		ZZIP_STAT stat;
		char szBuf[ 1024 ];
		Wide2Ansi( szBuf, p_szFilename.c_str(), p_szFilename.length() );
		int nRes = zzip_dir_stat( m_pZipDir, szBuf, &stat, ZZIP_CASEINSENSITIVE );

		return ( nRes == ZZIP_NO_ERROR );
	}
	//--------------------------------------------------------------------
	void CZipArchive::CheckZipError( int p_nZipError, const std::wstring& p_szOperation )const
	{
		if( p_nZipError != ZZIP_NO_ERROR )
		{
			std::wstring szErrorMsg = GetZipErrorDescription( static_cast< zzip_error_t >( p_nZipError ) );

			FK_EXCEPTION( CException::eEC_ErrorInternal, 
				m_szName + L" 错误: " + p_szOperation + szErrorMsg,
				L"CZipArchive::CheckZipError" );
		}
	}
	//--------------------------------------------------------------------
}