/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	BaseFile
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	基本文件类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//基本文件类
	class FK_DLL BaseFile : public FileObject
	{
	public:
		//读写缓存大小 64 KB
		static const DWORD	mCacheLen =	0x10000;

	protected:
		//缓存操作类型
		enum BuffedOpType
		{
			BOT_None,		//未进行缓存操作
			BOT_Read,		//读取数据
			BOT_Write,		//写入数据
		};

	protected:
		HANDLE			mFileHandle;			//文件句柄

		BYTE			mpCache[ mCacheLen ];	//读写缓存
		BYTE*			mpCachePtr;				//读写缓存指针
		DWORD			mdwCachedLen;			//已缓存数据长度

		BuffedOpType	mLastBuffedOp;			//最后进行的缓存操作类型

		DWORD			mPosition;				//读写指针位置

	public:
		BaseFile();
		BaseFile( LPCSTR szFileName, OpenType eOpenType = OT_Read, bool bOverlapped = false );
		~BaseFile();

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

		//设置当前读写位置为文件结尾
		void SetEndOfFile();

		//将缓存数据写入文件
		void Flush();

	protected:
		//同步读写操作
		void _SyncOperation();

		//内部读取文件数据
		DWORD _InternalRead( BYTE* pBuf, DWORD dwLen );

		//内部写入文件数据
		DWORD _InternalWrite( const BYTE* pBuf, DWORD dwLen );
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
