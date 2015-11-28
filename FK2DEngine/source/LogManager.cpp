/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	LogManager
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../include/LogManager.h"
#include "../include/Exception.h"

//--------------------------------------------------------------------
namespace FK2DEngine
{
	//--------------------------------------------------------------------
	CLogManager::CLogManager()
	{
		m_pDefaultLog = NULL;
	}
	//--------------------------------------------------------------------
	CLogManager::~CLogManager()
	{
		LogList::const_iterator IteEnd = m_Logs.end();

		for( LogList::iterator Ite = m_Logs.begin(); Ite != IteEnd; ++Ite )
		{
			delete Ite->second;
		}
	}
	//--------------------------------------------------------------------
	CLog* CLogManager::CreateLog(const std::wstring &p_szName, bool p_bDefaultLog /*= false*/,
		bool p_bDebugOutput /*= true*/, bool p_bSuppressFile /*= false*/)
	{
		CLog* pNewLog = new CLog( p_szName, p_bDebugOutput, p_bSuppressFile );

		if( !m_pDefaultLog || p_bDefaultLog )
		{
			m_pDefaultLog = pNewLog;
		}

		m_Logs.insert( LogList::value_type( p_szName, pNewLog ) );

		return pNewLog;
	}
	//--------------------------------------------------------------------
	CLog* CLogManager::GetDefaultLog() const
	{
		return m_pDefaultLog;
	}
	//--------------------------------------------------------------------
	CLog* CLogManager::SetDefaultLog( CLog* p_pNewLog )
	{
		CLog* pOldLog = m_pDefaultLog;
		m_pDefaultLog = p_pNewLog;
		return pOldLog;
	}
	//--------------------------------------------------------------------
	CLog* CLogManager::GetLog(const std::wstring &p_szName)
	{
		LogList::iterator Ite = m_Logs.find( p_szName );
		if( Ite != m_Logs.end() )
		{
			return Ite->second;
		}
		else
		{
			FK_EXCEPTION( CException::eEC_InvalidParams,
				L"未找到日志", L"CLogManager::GetLog" );
		}
	}
	//--------------------------------------------------------------------
	void CLogManager::LogMessage( const std::wstring& p_szMsg,
		ENUM_LogMessageLevel p_eLML /* = eLML_Normal */, bool p_bMaskDebug /* = false */ )
	{
		if( m_pDefaultLog )
		{
			m_pDefaultLog->LogMessage( p_szMsg, p_eLML, p_bMaskDebug );
		}
	}
	//--------------------------------------------------------------------
	void CLogManager::SetLogDetail(FK2DEngine::ENUM_LogLevel p_eLL)
	{
		if( m_pDefaultLog )
		{
			m_pDefaultLog->SetLogDetail( p_eLL );
		}
	}
	//--------------------------------------------------------------------
}

