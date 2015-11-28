/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	TextFile
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	文本文件类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================
#include "Stack.h"
//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//文本文件类
	//
	//     该文本文件类的功能为：在写入数据时不将字符串结尾符 '\0' 写入，并且自动将文本中的单个换行符 '\n' 翻译为走
	// 纸符换行符 '\r\n' 组合，在读取数据时自动将文本中的走纸符换行符 '\r\n' 组合翻译为单个换行符 '\n'。同时，操作
	// 该类时的文件、数据长度均为单个换行符的数据长度。实际上，调用该类的 Read()、Write() 函数时进行的是内存读写操
	// 作，真正的文件数据读写和翻译工作仅在打开和关闭文件时进行。并且，为了保持长度一致，只要改写文件中的任意数据，
	// 整个文件在关闭时都将重新写入。
	// 
	//     因此，该类只适合在进行小量、频繁的文本文件处理时使用。
	//
	//--------------------------------------------------------------------------------------------------------------
	class FK_DLL TextFile : public FileObject
	{
	protected:
		HANDLE		mFileHandle;	//文件句柄
		DWORD		mFileLen;		//文件长度

		DWORD		mCurPos;		//文件当前读写位置

		bool		mbIsChanged;	//是否改写过文本内容

		CHARStack	mTranslateBuf;	//文件翻译缓存

	public:
		TextFile();
		TextFile( LPCSTR szFileName, OpenType eOpenType = OT_Read, bool bOverlapped = false );
		~TextFile();

		//打开文件
		void Open( LPCSTR szFileName, OpenType eOpenType = OT_Read, bool bOverlapped = false );

		//关闭文件
		bool Close();

		//获取文件长度
		DWORD GetLength();

		//设置文件读写指针位置
		void SetPosition( long Offset, BasePos eBasePos = BP_Begin );

		//获取文件读写指针位置
		DWORD GetPosition();

		//读取文件数据
		void Read( void* pBuf, DWORD dwLen );

		//写入文件数据
		void Write( const void* pBuf, DWORD dwLen );

		//异步读取文件数据
		bool AsynRead( void* pBuf, DWORD dwLen, LPOVERLAPPED pOverlapped );

		//异步写入文件数据
		bool AsynWrite( const void* pBuf, DWORD dwLen, LPOVERLAPPED pOverlapped );

		//获取异步读写结果
		DWORD GetAsynResult( LPOVERLAPPED pOverlapped, bool bWait = true );

		//写入格式化文件数据
		void WriteFormat( const char* szFormat, ... );
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
