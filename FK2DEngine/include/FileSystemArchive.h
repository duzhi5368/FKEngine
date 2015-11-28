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

#pragma once

//--------------------------------------------------------------------
#include "Archive.h"
#include "ArchiveFactory.h"
#include <queue>
//--------------------------------------------------------------------
namespace FK2DEngine
{
	//--------------------------------------------------------------------
	#define FK_MAX_PATH		256
	//--------------------------------------------------------------------
	//! 文件系统文档
	//@remarks 支持读取文件夹
	class CFileSystemArchive : public CArchive
	{
		typedef std::deque< std::wstring >			DirectoryStack;
	protected:
		std::wstring			m_szBasePath;			// 基本路径
		mutable DirectoryStack	m_DirectoryStack;		// 目录堆栈
		mutable char			m_szPath[FK_MAX_PATH];	// 路径
	public:
		CFileSystemArchive( const std::wstring& p_szName, const std::wstring& p_szType );
		~CFileSystemArchive();
	public:
		bool 				IsCaseSensitive() const;
		void 				Load();
		void 				Unload();
		DataStreamPtr		Open( const std::wstring& p_FileName ) const;
		//! 开启Zip文件，若有密码需告知
		//@params [IN] p_ZipFileName Zip文件路径名
		//@params [IN] p_FileName 需要检查的文件名（路径为针对zip文件的相对路径名）
		//@params [IN] szPassword 注意，密码只允许是单字节
		DataStreamPtr		OpenZipFile( const std::wstring& p_ZipFileName, const std::wstring& p_FileName, const std::string& szPassword = "" ) const;
		StringVectorPtr		List( bool p_bRecursive = true );
		FileInfoListPtr		ListFileInfo( bool p_bRecursive = true );
		StringVectorPtr		Find( const std::wstring& p_Pattern, bool p_bRecursive = true );
		FileInfoListPtr		FindFileInfo( const std::wstring& p_Pattern, bool p_bRecursive = true );
		bool				Exists( const std::wstring& p_szFilename );
	protected:
		void	FindFiles( const std::wstring& p_szPattern, bool p_bRecursive, StringVector* p_pSimpleList,
			FileInfoList* p_pDetailList, const std::wstring& p_CurrentDir = L"" );
		void	ChangeDirectory( const std::wstring& p_szDir ) const;
		void	PushDirectory( const std::wstring& p_szDir ) const;
		void	PopDirectory() const;
	};
}