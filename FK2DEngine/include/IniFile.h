/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	IniFile
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include <list>
#include <vector>
#include "Exception.h"
#include <Windows.h>

//--------------------------------------------------------------------
// 去除对 Exception 强制要求 int类型 抛出的警告。
#pragma warning( disable : 4290 )
//--------------------------------------------------------------------
namespace FK2DEngine
{
	//--------------------------------------------------------------------
	class CFileDataStream;
	//--------------------------------------------------------------------
	class CIniException
	{
	public:
		CIniException(const std::wstring& szFileName, const std::wstring& szKey)
		{ wsprintf(m_szMessage, L"Ini文件 %s 键值 %s 读取失败", szFileName, szKey); }

		virtual wchar_t* GetErrorMessage(void) const
		{ return const_cast< wchar_t* >( &m_szMessage[0] ); }

	private:
		wchar_t m_szMessage[128];
	};
	//--------------------------------------------------------------------
	//! Ini类定义
	class CIniFile
	{
	private:
		//! 段信息结构
		struct SSectionInfo
		{
			std::wstring							m_szName;			// 段名
			std::list< std::wstring >::iterator		m_ItePos;			// 段在链表中的位置
		};

		typedef std::list< std::wstring >			List;				// 存放string的链表
		typedef std::list< std::wstring >::iterator ListIter;			// 游标
		typedef std::vector< SSectionInfo >			VecSecInfo;			// 段信息数组

	public:
		CIniFile();
		~CIniFile();
		
		//! 加载Ini文件
		void Load( CFileDataStream* p_pFile );
		//! 保存Ini文件
		void Save( const std::wstring& p_szFileName );
		//! 设置当前段
		void SetSection( const std::wstring& p_szSection );

	public:		// 读信息函数
		std::wstring ReadString(const std::wstring& szSection, const std::wstring& szKey, const std::wstring& szDefault);
		std::wstring ReadString(const std::wstring& szKey, const std::wstring& szDefault);
		std::wstring SafeReadString(const std::wstring& szSection, const std::wstring& szKey) throw(CIniException);
		std::wstring SafeReadString(const std::wstring& szKey) throw(CIniException);
		int ReadInteger(const std::wstring& szSection, const std::wstring& szKey, int nDefault);
		int ReadInteger(const std::wstring& szKey, int nDefault);
		int SafeReadInteger(const std::wstring& szSection, const std::wstring& szKey) throw( CIniException );
		int SafeReadInteger(const std::wstring& szKey) throw(CIniException);
		double ReadFloat(const std::wstring& szSection, const std::wstring& szKey, double lfDefault);
		double ReadFloat(const std::wstring& szKey, double lfDefault);
		double SafeReadFloat(const std::wstring& szSection, const std::wstring& szKey) throw(CIniException);
		double SafeReadFloat(const std::wstring& szKey) throw(CIniException);
		bool ReadBoolean(const std::wstring& szSection, const std::wstring& szKey, bool bDefault);
		bool ReadBoolean(const std::wstring& szKey, bool bDefault);
		bool SafeReadBoolean(const std::wstring& szSection, const std::wstring& szKey) throw(CIniException);
		bool SafeReadBoolean(const std::wstring& szKey) throw(CIniException);

		// 写信息函数
	public:
		void WriteString(const std::wstring& szSection, const std::wstring& szKey, const std::wstring& szValue);
		void WriteString(const std::wstring& szKey, const std::wstring& szValue);
		void WriteInteger(const std::wstring& szSection, const std::wstring& szKey, int nValue);
		void WriteInteger(const std::wstring& szKey, int nValue);
		void WriteFloat(const std::wstring& szSection, const std::wstring& szKey, double lfValue);
		void WriteFloat(const std::wstring& szKey, double lfValue);
		void WriteBoolean(const std::wstring& szSection, const std::wstring& szKey, bool bValue);
		void WriteBoolean(const std::wstring& szKey, bool bValue);

	private:
		List					m_lData;					// 文件内容链表
		VecSecInfo				m_vecSection;				// 段信息数组
		int						m_nCurSection;				// 当前段
		std::wstring			m_szFileName;				// 文件名
	};
}