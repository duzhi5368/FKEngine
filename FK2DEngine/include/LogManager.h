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

#pragma once

//--------------------------------------------------------------------
#include "Log.h"
#include "Singleton.h"
#include <map>
//--------------------------------------------------------------------
namespace FK2DEngine
{
	class CLogManager
	{
		friend class CLog;

		typedef std::map< std::wstring, CLog*, std::less< std::wstring > >		LogList;
	private:
		LogList			m_Logs;
		CLog*			m_pDefaultLog;
	public:
		CLogManager();
		~CLogManager();
	public:
		//! 创建日志
		//@params [IN] p_bDefaultLog 是否设置为默认Log文件。若已有默认Log文件，则设置无效
		//@params
		//@params [IN] p_bSuppressFile 是否禁止文件输出，true 则不输出Log文件
		CLog* CreateLog( const std::wstring& p_szName, bool p_bDefaultLog = false,
			bool p_bDebugOutput = true, bool p_bSuppressFile = false );

		//! 通过名称获取被管理的日志对象指针
		CLog* GetLog( const std::wstring& p_szName );

		//! 获取默认的日志对象指针
		CLog* GetDefaultLog() const;

		//! 设置默认的日志对象指针
		//@returns 返回原来的默认日志对象指针
		CLog* SetDefaultLog( CLog* p_pNewLog );

		//! 写日志
		void LogMessage( const std::wstring& p_szMsg, ENUM_LogMessageLevel p_eLML = eLML_Normal, 
			bool p_bMaskDebug = false );

		//! 设置日志系统等级
		void SetLogDetail( ENUM_LogLevel p_eLL );
	private:
		DECLARE_SINGLETON_CLASS( CLogManager );
	};
}

typedef FK2DEngine::TSingleton< FK2DEngine::CLogManager >	SSLogManager;