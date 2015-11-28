/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	DataStream
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "FKSharedPtr.h"
#include "StringConversion.h"
#include <list>
//--------------------------------------------------------------------
namespace FK2DEngine
{
	// 每次读流的默认大小
	#define FK_STREAM_TEMP_SIZE	512

	//! 数据流读取类
	//@note 该类非线程安全
	class CDataStream
	{
	protected:
		std::wstring			m_szName;	// 流名称
		unsigned int			m_unSize;	// 流中数据长度( 零则代表长度不定 )
	public:
		CDataStream()
			: m_unSize( 0 )
		{

		}

		CDataStream( const std::wstring& p_Name )
			: m_szName( p_Name )
			, m_unSize( 0 )
		{

		}

		virtual ~CDataStream()
		{

		}

		const std::wstring& GetName() const
		{
			return m_szName;
		}

		//! 返回从流中读取的数据长度，0 代表不确定
		unsigned int Size() const
		{
			return m_unSize;
		}

		//! 流操作符重载
		template< typename T >
		CDataStream& operator >> ( T& val );

		//! 读取文件流，遇到文件尾结束
		//@returns 成功读取的字节数
		virtual unsigned int Read( void* p_pBufBegin, unsigned int p_unCount ) = 0;

		//! 从流中读取一行
		//@note 使用该方式访问流必须使用二进制打开流
		virtual unsigned int ReadLine( char* p_pBuf, unsigned int p_unMaxCount, const std::wstring& p_Delim = L"\n" );

		//! 从流中读取一行，默认删除前后空格
		virtual std::wstring GetLine( bool p_bTrimAfter = true );

		//! 得到一个字符串，该字符串包括流中全部数据
		virtual std::wstring GetAllString();

		//! 跳过一行
		//@return 跳过的字节数
		virtual unsigned int SkipLine( const std::wstring& p_Delim = L"\n" );
		
		//! 跳过制定字节数的数据
		//@remark 参数可以为负数，可以理解为向前跳
		virtual void Skip( long p_lCount ) = 0;

		//! 重定位数据位置
		virtual void Seek( unsigned int p_unPos ) = 0;

		//! 返回当前数据读取位置
		virtual unsigned int Tell() const = 0;

		//! 判断数据流是否结束
		virtual bool Eof() const = 0;

		//! 关闭数据流
		virtual void Close() = 0;
	};

	//! 数据流只能指针
	typedef TSharedPtr< CDataStream >			DataStreamPtr;
	//! 数据流表
	typedef std::list< DataStreamPtr >			DataStreamList;
	//! 数据流表智能指针
	typedef TSharedPtr< DataStreamList >		DataStreamListPtr;
}