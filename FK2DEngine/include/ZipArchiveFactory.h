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

#pragma once

//--------------------------------------------------------------------
#include "ZipArchive.h"
#include "ArchiveFactory.h"
//--------------------------------------------------------------------
namespace FK2DEngine
{
	//--------------------------------------------------------------------
	//! Zip文档工厂类
	class CZipArchiveFactory : public CArchiveFactory
	{
	public:
		virtual ~CZipArchiveFactory() {}
	public:
		const std::wstring& GetType() const;
		CArchive* CreateInstance( const std::wstring& p_szName );
		void DestroyInstance( CArchive* p_pPtr );
	};
	//--------------------------------------------------------------------
}