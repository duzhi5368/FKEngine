/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	FileSystemArchiveFactory
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../Include/FileSystemArchive.h"
#include "../include/FileSystemArchiveFactory.h"

//--------------------------------------------------------------------
namespace FK2DEngine
{
	//--------------------------------------------------------------------
	const std::wstring& CFileSystemArchiveFactory::GetType() const
	{
		static std::wstring szName = L"FileSystem";
		return szName;
	}
	//--------------------------------------------------------------------
	CArchive* CFileSystemArchiveFactory::CreateInstance( const std::wstring& p_szName )
	{
		return new CFileSystemArchive( p_szName, L"FileSystem" );
	}
	//--------------------------------------------------------------------
	void CFileSystemArchiveFactory::DestroyInstance( CArchive* p_pPtr )
	{
		delete p_pPtr;
	}
	//--------------------------------------------------------------------
}