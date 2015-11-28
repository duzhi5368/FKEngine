/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	ZipArchiveFactory
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../Include/Archive.h"
#include "../include/ZipArchive.h"
#include "../include/ZipArchiveFactory.h"

//--------------------------------------------------------------------
namespace FK2DEngine
{
	//--------------------------------------------------------------------
	const std::wstring& CZipArchiveFactory::GetType() const
	{
		static std::wstring szName = L"Zip";
		return szName;
	}
	//--------------------------------------------------------------------
	CArchive* CZipArchiveFactory::CreateInstance( const std::wstring& p_szName )
	{
		 return new CZipArchive( p_szName, L"Zip" );
	}
	//--------------------------------------------------------------------
	void CZipArchiveFactory::DestroyInstance( CArchive* p_pPtr )
	{
		delete p_pPtr;
	}
	//--------------------------------------------------------------------
}