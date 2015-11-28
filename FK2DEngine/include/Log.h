/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Log
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include <string>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <fstream>
#include <sstream>
//--------------------------------------------------------------------
namespace FK2DEngine
{
	//! 日志重要等级
	enum ENUM_LogLevel
	{
		eLL_Low			=	1,
		eLL_Normal		=	2,
		eLL_BoreMe		=	3,
	};

	//! 日志信息等级
	enum ENUM_LogMessageLevel
	{
		eLML_Trivial	=	1,		// 轻微消息
		eLML_Normal		=	2,		// 普通等级
		eLML_Critical	=	3,		// 紧急消息
	};

	//! 如果 LogMessageLevel + LogLevel > LOG_THRESHOLD，则记录到日志文件中
	#define LOG_THRESHOLD 4

	class CLog
	{
		friend class CLogManager;
	private:
		//! 请使用CLogManager创建
		//@param	[IN] p_bDebugOutput	是否调试输出
		//@param	[IN] p_bSuppressFile 是否禁止写文件
		CLog( const std::wstring& p_szName, bool p_bDebugOutput = true, bool p_bSuppressFile = false );

		~CLog();
	public:
		//! 写日志
		void LogMessage( const std::wstring& p_szMsg, ENUM_LogMessageLevel p_eLML = eLML_Normal, 
			bool p_bMaskDebug = false );

		//! 设置日志系统等级
		void SetLogDetail( ENUM_LogLevel p_eLL );
	protected:
		std::wofstream		m_fpLog;		// 文件流
		ENUM_LogLevel		m_eLogLevel;	// 日志系统等级
		bool				m_bDebugOut;	// 是否将日志写到控制台
		bool				m_bSuppressFile;// 是否禁止将日志写入文件
		std::wstring		m_szName;		// 日志文件名
	};
}