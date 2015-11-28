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

#include "../include/Archive.h"
#include "../include/ArchiveFactory.h"
#include "../include/ArchiveManager.h"
#include "../include/Exception.h"
#include "../include/LogManager.h"
#include <vector>

//--------------------------------------------------------------------
namespace FK2DEngine
{
	//--------------------------------------------------------------------
	CArchiveManager::CArchiveManager()
	{
		
	}
	//--------------------------------------------------------------------
	CArchiveManager::~CArchiveManager()
	{
		// 释放资源
		ArchiveMap::iterator Ite = m_Archives.begin();
		ArchiveMap::const_iterator IteEnd = m_Archives.end();

		for( ; Ite != IteEnd; ++Ite )
		{
			CArchive* pArch = Ite->second;

			// 释放文档
			pArch->Unload();

			// 找到对应文档工厂
			ArchiveFactoryMap::iterator IteFind = m_ArchFactories.find( pArch->GetType() );
			if( IteFind == m_ArchFactories.end() )
			{
				// 工厂文档没有找到
				FK_EXCEPTION( CException::eEC_ItemCannotFind,
					L"没有找到可处理: " + IteFind->second->GetType() + L" 的文档工厂",
					L"CArchiveManager::~CArchiveManager" );
			}

			IteFind->second->DestroyInstance( pArch );
		}

		// 清空文档映射表
		m_Archives.clear();
	}
	//--------------------------------------------------------------------
	CArchive* CArchiveManager::Load( const std::wstring& p_Filename, const std::wstring& p_ArchiveType )
	{
		ArchiveMap::iterator i = m_Archives.find( p_Filename );
		CArchive* pArch = 0;

		if( i == m_Archives.end() )
		{
			// 搜索文档工厂
			ArchiveFactoryMap::iterator Ite = m_ArchFactories.find( p_ArchiveType );
			if( Ite == m_ArchFactories.end() )
			{
				FK_EXCEPTION( CException::eEC_ItemCannotFind, 
					L"没有找到可以处理" + p_ArchiveType + L"的文档工厂",
					L"CArchiveManager::Load" );
			}
			pArch = Ite->second->CreateInstance( p_Filename );
			pArch->Load();
			m_Archives[ p_Filename ] = pArch;
		}
		else
		{
			pArch = i->second;
		}

		return pArch;
	}
	//--------------------------------------------------------------------
	void CArchiveManager::Unload( CArchive* p_pArch )
	{
		Unload( p_pArch->GetName() );
	}
	//--------------------------------------------------------------------
	void CArchiveManager::Unload( const std::wstring& p_szFileName )
	{
		ArchiveMap::iterator i = m_Archives.find( p_szFileName );

		if( i != m_Archives.end() )
		{
			i->second->Unload();

			// 搜索文档工厂
			ArchiveFactoryMap::iterator Ite = m_ArchFactories.find( i->second->GetType() );
			if( Ite == m_ArchFactories.end() )
			{
				FK_EXCEPTION( CException::eEC_ItemCannotFind, 
					L"没有找到可以处理" + i->second->GetType() + L"的文档工厂",
					L"CArchiveManager::Load" );
			}
			Ite->second->DestroyInstance( i->second );
			m_Archives.erase( i );
		}
	}
	//--------------------------------------------------------------------
	void CArchiveManager::AddArchiveFactory( CArchiveFactory* p_pFactory )
	{
		m_ArchFactories.insert( ArchiveFactoryMap::value_type( p_pFactory->GetType(), p_pFactory ) );
		SSLogManager::Instance()->LogMessage( L"用于创建" + p_pFactory->GetType() + L"类型的文档工厂已成功注册过." );
	}
	//--------------------------------------------------------------------
	CArchiveManager::ArchiveMap& CArchiveManager::GetArchiveMap()
	{
		return m_Archives;
	}
	//--------------------------------------------------------------------
}	