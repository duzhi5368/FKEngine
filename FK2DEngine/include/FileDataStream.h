/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	FileDataStream
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------

#include "DataStream.h"

//--------------------------------------------------------------------
namespace FK2DEngine
{
	class CFileDataStream : public CDataStream
	{
	protected:
		std::ifstream*			m_pStream;			// 原始数据流
		char*					m_pNormalStream;	// 原始数据流（当从内存读取，而无法使用ifstream时采用本指针）
		char*					m_pCurrent;			// 当使用 char* 内存管理时，该值有效，记录文件当前指针
		bool					m_bFreeOnClose;		// 是否在关闭流时删除数据流
	public:
		CFileDataStream( std::ifstream* p_pStream, bool p_bFreeOnClose = true );
		//! 从标准数据流中构造并指定一个名称
		//@param [IN] p_szName 指定流的名称
		CFileDataStream( const std::wstring& p_szName, std::ifstream* p_pS, bool p_bFreeOnClose );
		CFileDataStream( const std::wstring& p_szName, std::ifstream* p_pS,
			unsigned int p_unSize, bool p_bFreeOnClose );
		CFileDataStream( const std::wstring& p_szName, char* p_pBuf, 
			unsigned int p_unSize, bool p_bFreeOnClose );
		~CFileDataStream();
	public:
		unsigned int Read( void* p_pBuf, unsigned int p_unCount );
		unsigned int ReadLine( char* p_pBuf, unsigned int p_unMaxCount, const std::wstring& p_Delim = L"\n" );
		void Skip( long p_lCount );
		void Seek( unsigned int p_unPos );
		unsigned int Tell() const;
		bool Eof() const;
		void Close();
	};
}

