/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	ArchiveManager
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "Singleton.h"
//--------------------------------------------------------------------
namespace FK2DEngine
{
	//--------------------------------------------------------------------
	//! 文档管理器
	class CArchiveManager
	{
		typedef std::map< std::wstring, CArchiveFactory* >		ArchiveFactoryMap;
		typedef std::map< std::wstring, CArchive* >				ArchiveMap;
	protected:
		ArchiveFactoryMap			m_ArchFactories;	// 有效的文档工厂
		ArchiveMap					m_Archives;			// 当前已加载的文档
	public:
		CArchiveManager();
		~CArchiveManager();
		//! 读取一个文档
		//@param [IN] p_ArchiveType 文档类型 例如zip File
		CArchive* Load( const std::wstring& p_Filename, const std::wstring& p_ArchiveType );

		//! 释放文档
		void Unload( CArchive* p_pArch );

		//! 释放文档
		void Unload( const std::wstring& p_szFileName );

		//! 添加文档工厂
		void AddArchiveFactory( CArchiveFactory* p_pFactory );

		//! 得到文档映射表
		CArchiveManager::ArchiveMap& GetArchiveMap();
	private:
		DECLARE_SINGLETON_CLASS( CArchiveManager );
	};
}

typedef FK2DEngine::TSingleton< FK2DEngine::CArchiveManager >	SSArchiveManager;