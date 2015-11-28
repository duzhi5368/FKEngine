/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Archive
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "DataStream.h"
#include "FKSharedPtr.h"
#include "StringConversion.h"
#include <map>
//--------------------------------------------------------------------
namespace FK2DEngine
{
	//--------------------------------------------------------------------
	class CArchive;
	//--------------------------------------------------------------------
	//! 文件或文件夹信息
	struct SFileInfo
	{
		CArchive*		m_pArchive;				// 文档指针
		std::wstring	m_szFileName;			// 文件全名
		std::wstring	m_szPath;				// 文件路径
		std::wstring	m_szBaseName;			// 文件名
		unsigned int	m_unCompressedSize;		// 压缩后大小
		unsigned int	m_unUnCompressedSize;	// 压缩前大小
	};
	//--------------------------------------------------------------------
	typedef std::vector< SFileInfo >			FileInfoList;
	typedef TSharedPtr< FileInfoList >			FileInfoListPtr;
	//--------------------------------------------------------------------
	//! 文档类
	//@remarks 一个文档可以是一个文件夹，一个压缩文件，或是远程文件
	class CArchive
	{
	protected:
		std::wstring				m_szName;	// 文档名称
		std::wstring				m_szType;	// 文档类型
	public:
		//! 构造函数
		//@remarks 建议不要直接调用，使用Factory创建即可
		CArchive( const std::wstring& p_szName, const std::wstring& p_szType )
			: m_szType( p_szType )
			, m_szName( p_szName )
		{

		}

		virtual ~CArchive()
		{

		}

	public:
		const std::wstring& GetName()
		{
			return m_szName;
		}
		
		const std::wstring& GetType()
		{
			return m_szType;
		}

		//! 是否敏感大小写
		virtual bool IsCaseSensitive() const = 0;

		//! 读取文档
		virtual void Load() = 0;

		//! 释放文档
		virtual void Unload() = 0;

		//! 读取一个文档到一个流内
		virtual DataStreamPtr Open( const std::wstring& p_szFileName ) const = 0;

		//! 文档中所有文件的列表
		//@param [IN] p_bRecursive 是否遍历递归搜索所有文件夹下的文件
		virtual StringVectorPtr List( bool p_bRecursive = true ) = 0;

		//! 文档中所有文件详细信息的列表
		virtual FileInfoListPtr ListFileInfo( bool p_bRecursive = true ) = 0;

		//! 查找与模式串匹配的所有文件
		//@param [IN] p_Pattern 参考的模式串，允许使用'*'号
		virtual StringVectorPtr Find( const std::wstring& p_Pattern, bool p_bRecursive = true ) = 0;

		//! 查找与模式串匹配的所有文件详细信息
		virtual FileInfoListPtr FindFileInfo( const std::wstring& p_Pattern, bool p_bRecursive = true ) = 0;

		//! 检查文件是否存在
		//@note 需要指定完整的相对文件名字，不能仅是文件名
		virtual bool Exists( const std::wstring& p_szFilename ) = 0;
	};
	//--------------------------------------------------------------------
}