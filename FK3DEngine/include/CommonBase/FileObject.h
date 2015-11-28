/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	FileObject
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	文件对象类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================
#include "../CommonBase/BaseTypeDefine.h"
//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------

	//文件状态
	const int FS_NotExist	= 0x0;		//文件不存在
	const int FS_Exist		= 0x1;		//文件存在

	const int FS_Directory	= 0x2;		//该文件为目录
	const int FS_ReadOnly	= 0x10;		//文件为只读属性
	const int FS_Hidden		= 0x20;		//文件为隐藏属性
	const int FS_System		= 0x40;		//文件为系统属性
	const int FS_Archive	= 0x80;		//文件为存档属性
	const int FS_Compressed	= 0x100;	//该文件是压缩文件

	//--------------------------------------------------------------------------------------------------------------
	//文件打开类型
	enum OpenType
	{
		OT_Read,		//只允许读取（文件必须存在）
		OT_Write,		//只允许写入（覆盖原文件）
		OT_WriteAppend,	//只允许写入（允许附加数据）
		OT_ReadWirte,	//读取并写入（文件必须存在）
	};
	//--------------------------------------------------------------------------------------------------------------
	//文件对象类 （纯虚类）
	class FK_DLL FileObject
	{
	public:
		char	mszCurDir[MAX_PATH];//当前目录字符串
		UINT	mnCurDirLen;		//当前目录字符串长度

	public:
		FileObject();
		virtual ~FileObject() = 0;

		//设置文件当前目录
		void SetFileDirectory( LPCSTR szCurDir );

		//打开文件
		virtual void Open( LPCSTR szFileName, OpenType eOpenTyp = OT_Read, bool bOverlapped = false ) = 0;

		//关闭文件
		virtual bool Close() = 0;

		//获取文件长度
		virtual DWORD GetLength() = 0;

		//设置文件读写指针位置
		virtual void SetPosition( long Offset, BasePos eBasePos = BP_Begin ) = 0;
		
		//获取文件读写指针位置
		virtual DWORD GetPosition() = 0;

		//读取文件数据
		virtual void Read( void* pBuf, DWORD dwLen ) = 0;

		//写入文件数据
		virtual void Write( const void* pBuf, DWORD dwLen ) = 0;

		//异步读取文件数据
		virtual bool AsynRead( void* pBuf, DWORD dwLen, LPOVERLAPPED pOverlapped ) = 0;

		//异步写入文件数据
		virtual bool AsynWrite( const void* pBuf, DWORD dwLen, LPOVERLAPPED pOverlapped ) = 0;

		//获取异步读写结果
		virtual DWORD GetAsynResult( LPOVERLAPPED pOverlapped, bool bWait = true ) = 0;

	protected:
		//内部打开文件操作
		void _InternalOpen( LPCSTR szFileName, OpenType eOpenType, bool bOverlapped, HANDLE* pHandle );

	public:
		//获取指定文件的状态
		static int GetFileStatus( LPCSTR szFileName );

		//删除文件
		static bool Delete( LPCSTR szFileName, bool bDeleteReadOnly = false );
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
