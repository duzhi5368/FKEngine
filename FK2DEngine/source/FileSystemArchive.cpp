/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	FileSystemArchive
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../Include/FileDataStream.h"
#include "../Include/LogManager.h"
#include "../Include/Exception.h"
#include "../Include/StringConversion.h"
#include "../Include/FileSystemArchive.h"
#include "../Include/FileSystemArchiveFactory.h"
#include "../../Depend/simpleZip/unzip.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <Windows.h>
#include <direct.h>
#include <io.h>

//--------------------------------------------------------------------
namespace FK2DEngine
{
	//--------------------------------------------------------------------
	CFileSystemArchive::CFileSystemArchive( const std::wstring& p_szName, const std::wstring& p_szType )
		: CArchive( p_szName, p_szType )
	{

	}
	//--------------------------------------------------------------------
	CFileSystemArchive::~CFileSystemArchive()
	{
		Unload();
	}
	//--------------------------------------------------------------------
	bool CFileSystemArchive::IsCaseSensitive() const
	{
		return false;
	}
	//--------------------------------------------------------------------
	void CFileSystemArchive::Load()
	{
		m_szBasePath = m_szName;
		PushDirectory( m_szBasePath );
		PopDirectory();
	}
	//--------------------------------------------------------------------
	void CFileSystemArchive::Unload()
	{	

	}
	//--------------------------------------------------------------------
	DataStreamPtr CFileSystemArchive::Open( const std::wstring& p_FileName ) const
	{
		PushDirectory( m_szBasePath );

		struct stat tagStat;
		char szBuf[ 1024 ];

		Wide2Ansi( szBuf, p_FileName.c_str(), p_FileName.length() );
		int nRet = stat( szBuf, &tagStat );
		assert( nRet == 0 && L"文件大小读取错误" );

		std::ifstream* pStream = new std::ifstream();
		pStream->open( p_FileName.c_str(), std::ios::in | std::ios::binary );
		PopDirectory();

		if( pStream->fail() )
		{
			delete pStream;
			FK_EXCEPTION( CException::eEC_FileCannotFind,
				L"无法打开文件" + p_FileName,
				L"CFileSystemArchive::Open" );
		}

		CFileDataStream* stream = new CFileDataStream( p_FileName, pStream, tagStat.st_size, true );
		return DataStreamPtr( stream );
	}
	//--------------------------------------------------------------------
	DataStreamPtr CFileSystemArchive::OpenZipFile( const std::wstring& p_ZipFileName, const std::wstring& p_FileName, const std::string& szPassword ) const
	{
		PushDirectory( m_szBasePath );

		// 打开zip压缩包
		HZIP hZip = OpenZip( (m_szBasePath + p_ZipFileName).c_str(), szPassword.c_str() );
		if( hZip == 0 )
		{
			FK_EXCEPTION( CException::eEC_ErrorInternal, L"无法正确读取一个zip文件", L"CFileSystemArchive::Open");
		}

		// 处理路径
		char szBuf[ 1024 ] = { 0 };
		Wide2Ansi( szBuf, p_FileName.c_str(), p_FileName.length() );
		for (int i = 0; i < (int)strlen(szBuf); ++i)
		{
			if (szBuf[i] == '\\')
			{
				szBuf[i] = '/';
			}
		}
		std::string szTemp( szBuf );
		std::wstring szChangedFilePath = UTF8ToWstring( szTemp );

		// 在Zip包中查找指定文件
		ZIPENTRY ze;
		int index;
		FindZipItem( hZip, szChangedFilePath.c_str(), true, &index, &ze );
		if (index == -1)
		{
			FK_EXCEPTION( CException::eEC_FileCannotFind, L"无法在Zip文件中找到对应文件", L"CFileSystemArchive::Open");
		}

		// 将文件内容解压至pBuffer
		int nSize = ze.unc_size;
		char* pBuffer = new char[nSize+1];
		ZRESULT unRet = ZR_OK;
		unRet = UnzipItem((HZIP)hZip, index, pBuffer, nSize);
		if( unRet != ZR_OK )
		{
			FK_EXCEPTION( CException::eEC_ErrorInternal, L"无法在Zip文件中解压对应文件", L"CFileSystemArchive::Open");
		}
		pBuffer[nSize] = 0;

		// 关闭压缩包
		CloseZip(hZip);

		PopDirectory();

		// 创建并返回对象
		CFileDataStream* stream = new CFileDataStream( p_FileName, pBuffer, nSize + 1, true );
		return DataStreamPtr( stream );
	}
	//--------------------------------------------------------------------
	StringVectorPtr CFileSystemArchive::List( bool p_bRecursive /*= true*/ )
	{
		PushDirectory( m_szBasePath );
		StringVectorPtr ret( new StringVector() );
		FindFiles( L"*", p_bRecursive, ret.GetPointer(), 0 );
		PopDirectory();

		return ret;
	}
	//--------------------------------------------------------------------
	FileInfoListPtr CFileSystemArchive::ListFileInfo( bool p_bRecursive /*= true*/ )
	{
		PushDirectory( m_szBasePath );
		FileInfoListPtr ret( new FileInfoList() );
		FindFiles( L"*", p_bRecursive, 0, ret.GetPointer() );
		PopDirectory();

		return ret;
	}
	//--------------------------------------------------------------------
	StringVectorPtr CFileSystemArchive::Find( const std::wstring& p_Pattern, bool p_bRecursive /*= true*/ )
	{
		PushDirectory( m_szBasePath );
		StringVectorPtr ret( new StringVector() );
		FindFiles( p_Pattern, p_bRecursive, ret.GetPointer(), 0 );
		PopDirectory();

		return ret;
	}
	//--------------------------------------------------------------------
	FileInfoListPtr CFileSystemArchive::FindFileInfo( const std::wstring& p_Pattern, bool p_bRecursive /*= true*/ )
	{
		PushDirectory( m_szBasePath );
		FileInfoListPtr ret( new FileInfoList() );
		FindFiles( p_Pattern, p_bRecursive, 0, ret.GetPointer() );
		PopDirectory();

		return ret;
	}
	//--------------------------------------------------------------------
	bool CFileSystemArchive::Exists( const std::wstring& p_szFilename )
	{
		PushDirectory( m_szBasePath );
		struct stat tagStat;
		char szBuf[ 1024 ];
		Wide2Ansi( szBuf, p_szFilename.c_str(), p_szFilename.length() );
		bool bRet = ( stat( szBuf, &tagStat ) == 0 );
		PopDirectory();
		
		return bRet;
	}	
	//--------------------------------------------------------------------
	void CFileSystemArchive::FindFiles( const std::wstring& p_szPattern, bool p_bRecursive, StringVector* p_pSimpleList,
		FileInfoList* p_pDetailList, const std::wstring& p_CurrentDir /*= L""*/ )
	{
		long lHandle = 0;
		long lRes = 0;

		struct _finddata_t tagData;

		char szBuf[ 1024 ];
		Wide2Ansi( szBuf, p_szPattern.c_str(), p_szPattern.length() );

		lHandle = _findfirst( szBuf, &tagData );
		while( lHandle != -1 && lRes != -1 )
		{
			if( !( tagData.attrib & _A_SUBDIR ) )
			{
				wchar_t szBuf[ 1024 ];
				Ansi2Wide( szBuf, tagData.name, strlen( tagData.name ) );

				if( p_pSimpleList )
				{
					p_pSimpleList->push_back( p_CurrentDir + std::wstring( szBuf ) );
				}
				else if( p_pDetailList )
				{
					SFileInfo tagFI;
					tagFI.m_pArchive	= this;
					tagFI.m_szFileName	= p_CurrentDir + std::wstring( szBuf );
					tagFI.m_szBaseName	= std::wstring( szBuf );
					tagFI.m_szPath		= p_CurrentDir;
					tagFI.m_unCompressedSize	= tagData.size;
					tagFI.m_unUnCompressedSize	= tagData.size;
					p_pDetailList->push_back( tagFI );
				}
			}

			lRes = _findnext( lHandle, &tagData );
		}
	
		if( lHandle != -1 )
		{
			_findclose( lHandle );
		}

		if( p_bRecursive )
		{
			lHandle = _findfirst( "*", &tagData );
			lRes = 0;
			while( lHandle != -1 && lRes != -1 )
			{
				if( ( tagData.attrib & _A_SUBDIR )
					&& strcmp( tagData.name, "." )
					&& strcmp( tagData.name, ".." ) )
				{
					wchar_t szBuf[ 1024 ];
					Ansi2Wide( szBuf, tagData.name, strlen( tagData.name ) );
					std::wstring szDir = p_CurrentDir + std::wstring( szBuf ) + L"/";
					PushDirectory( std::wstring( szBuf ) );
					FindFiles( p_szPattern, p_bRecursive, p_pSimpleList, p_pDetailList, szDir );
					PopDirectory();
				}
				lRes = _findnext( lHandle, &tagData );
			}

			if( lHandle != -1 )
			{
				_findclose( lHandle );
			}
		}
	}
	//--------------------------------------------------------------------
	void CFileSystemArchive::ChangeDirectory( const std::wstring& p_szDir ) const
	{
		char szBuf[ 1024 ];
		Wide2Ansi( szBuf, p_szDir.c_str(), p_szDir.length() );

		if( chdir( szBuf ) == -1 )
		{
			FK_EXCEPTION( CException::eEC_FileCannotFind,
				L"无法打开指定目录：" + p_szDir ,
				L"CFileSystemArchive::ChangeDirectory" );
		}
	}
	//--------------------------------------------------------------------
	void CFileSystemArchive::PushDirectory( const std::wstring& p_szDir ) const
	{
		getcwd( m_szPath, FK_MAX_PATH );
		wchar_t szBuf[ 1024 ];
		Ansi2Wide( szBuf, m_szPath, strlen( m_szPath ) );
		m_DirectoryStack.push_back( std::wstring( szBuf ) );
		ChangeDirectory( p_szDir );
	}
	//--------------------------------------------------------------------
	void CFileSystemArchive::PopDirectory() const
	{
		if( m_DirectoryStack.empty() )
		{
			FK_EXCEPTION( CException::eEC_InvalidParams, 
				L"堆栈中没有目录路径",
				L"CFileSystemArchive::PopDirectory" );
		}

		ChangeDirectory( m_DirectoryStack.back() );
		m_DirectoryStack.pop_back();
	}
	//--------------------------------------------------------------------
}