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

#pragma once

//--------------------------------------------------------------------
#include "ArchiveFactory.h"
//--------------------------------------------------------------------
namespace FK2DEngine
{
	//--------------------------------------------------------------------
	//! 文件系统文档工厂类
	class CFileSystemArchiveFactory : public CArchiveFactory
	{
	public:
		virtual ~CFileSystemArchiveFactory(){}
	public:
		const std::wstring& GetType() const;
		CArchive* CreateInstance( const std::wstring& p_szName );
		void DestroyInstance( CArchive* p_pPtr );
	};
	//--------------------------------------------------------------------
}