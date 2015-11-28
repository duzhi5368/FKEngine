/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Directories
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../include/Directories.h"
#include "../include/WinUtility.h"
#include <cwchar>
#include <stdexcept>
#include <ShlObj.h>

//--------------------------------------------------------------------
namespace
{
	std::wstring SpecialFolderPath( int p_nCsidl )
	{
		WCHAR szBuf[ MAX_PATH + 2 ];
		if( !SHGetSpecialFolderPath( 0, szBuf, p_nCsidl, TRUE ) )
		{
			throw std::runtime_error( "获取文件夹路径失败" );
		}
		std::size_t unLen = std::wcslen( szBuf );
		if( szBuf[unLen - 1] != '\\' )
		{
			szBuf[unLen] = L'\\';
			szBuf[unLen + 1] = L'\0';
		}
		return szBuf;
	}
}
//--------------------------------------------------------------------
std::wstring FK2DEngine::ResourcePrefix()
{
	return Win::AppDirectory();
}
//--------------------------------------------------------------------
std::wstring FK2DEngine::ShareResourcePrefix()
{
	return Win::AppDirectory();
}
//--------------------------------------------------------------------
std::wstring FK2DEngine::UserSettingPrefix()
{
	return SpecialFolderPath( CSIDL_APPDATA );
}
//--------------------------------------------------------------------
std::wstring FK2DEngine::UserDocsPrefix()
{
	return SpecialFolderPath( CSIDL_PERSONAL );
}
//--------------------------------------------------------------------