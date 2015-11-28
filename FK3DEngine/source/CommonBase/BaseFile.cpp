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
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../../include/CommonBase/CommonBaseInsideHead.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	const char STR_ERR_READ_LEN[]	= "无法读取指定长度的数据，可能是由于读取位置超过了文件尾部。";
	const char STR_ERR_WRITE_LEN[]	= "无法写入指定长度的数据，可能是由于在没有指定附加数据文件打开模式的情况下"
		"写入位置超过了文件尾部。";
	//--------------------------------------------------------------------------------------------------------------
	BaseFile::BaseFile()
		: mFileHandle	(NULL)
		, mpCachePtr	(NULL)
		, mdwCachedLen	(0)
		, mLastBuffedOp	(BOT_None)
		, mPosition		(0)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	BaseFile::BaseFile( LPCSTR szFileName, OpenType eOpenType, bool bOverlapped )
		: mFileHandle	(NULL)
	{
		Open( szFileName, eOpenType, bOverlapped );
	}
	//--------------------------------------------------------------------------------------------------------------
	BaseFile::~BaseFile()
	{
		Close();
	}
	//--------------------------------------------------------------------------------------------------------------
	//打开文件
	void BaseFile::Open( LPCSTR szFileName, OpenType eOpenType, bool bOverlapped )
	{
		//打开文件
		_InternalOpen( szFileName, eOpenType, bOverlapped, &mFileHandle );

		mpCachePtr		= mpCache;
		mdwCachedLen	= 0;
		mLastBuffedOp	= BOT_None;
		mPosition		= 0;
	}
	//--------------------------------------------------------------------------------------------------------------
	//关闭文件
	bool BaseFile::Close()
	{
		if( mFileHandle != NULL )
		{
			//同步读写操作
			_SyncOperation();

			//关闭文件
			if( FALSE == CloseHandle( mFileHandle ) )
			{
				SetLastException( Exception::ERR_WINAPI_ERROR, "关闭文件失败。" );
				return false;
			}

			mFileHandle = NULL;
		}

		return true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取文件长度
	DWORD BaseFile::GetLength()
	{
		DWORD dwLength = GetFileSize( mFileHandle, NULL );
		if( dwLength == INVALID_FILE_SIZE )
			Except( Exception::ERR_WINAPI_ERROR, "获取文件长度失败。" );

		return dwLength;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置文件读写指针位置
	void BaseFile::SetPosition( long Offset, BasePos eBasePos )
	{
		//同步读写操作
		_SyncOperation();

		switch (eBasePos)
		{
		case BP_Begin:
			mPosition = (DWORD)Offset;
			break;
		case BP_End:
			mPosition = (DWORD)( (long)GetLength() + Offset );
			break;
		case BP_Curpos:
			mPosition = (DWORD)( (long)mPosition + Offset );
			break;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "错误的文件读写指针移动基准类型。" );
		}

		if( SetFilePointer( mFileHandle, mPosition, NULL, FILE_BEGIN ) == INVALID_SET_FILE_POINTER )
			Except( Exception::ERR_WINAPI_ERROR, "移动文件读写指针失败。" );
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取文件读写指针位置
	DWORD BaseFile::GetPosition()
	{
		return mPosition;
	}
	//--------------------------------------------------------------------------------------------------------------
	//读取文件数据
	void BaseFile::Read( void* pBuf, DWORD dwLen )
	{
		//如果最后进行的缓存操作不是读取操作
		if( mLastBuffedOp != BOT_Read )
			_SyncOperation();

		BYTE* pDstBuf = (BYTE*)pBuf;

		while( dwLen != 0 )
		{
			//如果缓存中已存在数据
			if( mdwCachedLen != 0 )
			{
				//读取缓存中的剩余数据
				DWORD dwCopyLen = ( dwLen < mdwCachedLen ) ? dwLen : mdwCachedLen;
				memcpy( pBuf, mpCachePtr, dwCopyLen );
				mdwCachedLen -= dwCopyLen;
				mpCachePtr += dwCopyLen;
				dwLen -= dwCopyLen;
				pDstBuf += dwCopyLen;
			}
			//如果缓存中已不存在数据且读取长度大于等于缓存长度
			else if( dwLen >= mCacheLen )
			{
				//直接从文件中读取数据
				if( dwLen != _InternalRead( pDstBuf, dwLen ) )
					Except( Exception::ERR_CANNOT_READ_FILE, STR_ERR_READ_LEN );

				break;
			}
			//如果缓存中已不存在数据且读取长度小于缓存长度
			else
			{
				//从文件中读取数据填充缓存
				DWORD dwReadLen = _InternalRead( mpCache, mCacheLen );
				if( dwReadLen < dwLen )
					Except( Exception::ERR_CANNOT_READ_FILE, STR_ERR_READ_LEN );

				//从缓存中读取指定长度的数据
				memcpy( pDstBuf, mpCache, dwLen );

				mpCachePtr = mpCache + dwLen;
				mdwCachedLen = dwReadLen - dwLen;

				break;
			}
		}

		//记录最后的缓存操作类型
		mLastBuffedOp = BOT_Read;
		mPosition += dwLen;
	}
	//--------------------------------------------------------------------------------------------------------------
	//写入文件数据
	void BaseFile::Write( const void* pBuf, DWORD dwLen )
	{
		//如果最后进行的缓存操作不是写入操作
		if( mLastBuffedOp != BOT_Write )
			_SyncOperation();

		BYTE* pSrcBuf = (BYTE*)pBuf;

		while( dwLen != 0 )
		{
			//如果缓存未满
			if( mdwCachedLen < mCacheLen )
			{
				//将数据写入缓存
				DWORD dwRemainCacheLen = mCacheLen - mdwCachedLen;
				DWORD dwCopyLen = ( dwLen < dwRemainCacheLen ) ? dwLen : dwRemainCacheLen;
				memcpy( mpCachePtr, pSrcBuf, dwCopyLen );
				mdwCachedLen += dwCopyLen;
				mpCachePtr += dwCopyLen;
				dwLen -= dwCopyLen;
				pSrcBuf += dwCopyLen;
			}
			//如果缓存已满且写入长度大于等于缓存长度
			else if( dwLen >= mCacheLen )
			{
				//将先前缓存中数据写入文件
				Flush();

				//直接将数据写入文件
				if( dwLen != _InternalWrite( pSrcBuf, dwLen ) )
					Except( Exception::ERR_CANNOT_WRITE_FILE, STR_ERR_WRITE_LEN );

				break;
			}
			//如果缓存已满且写入长度小于缓存长度
			else
			{
				//将先前缓存中数据写入文件
				Flush();

				//将剩余数据写入缓存中
				memcpy( mpCache, pSrcBuf, dwLen );

				mpCachePtr = mpCache + dwLen;
				mdwCachedLen = dwLen;

				break;
			}
		}

		//记录最后的缓存操作类型
		mLastBuffedOp = BOT_Write;
		mPosition += dwLen;
	}
	//--------------------------------------------------------------------------------------------------------------
	//异步读取文件数据
	bool BaseFile::AsynRead( void* pBuf, DWORD dwLen, LPOVERLAPPED pOverlapped )
	{
		//同步读写操作
		_SyncOperation();

		DWORD dwReadLen;
		BOOL bResult = ReadFile( mFileHandle, pBuf, dwLen, &dwReadLen, pOverlapped );
		if( bResult == FALSE )
		{
			if( ERROR_IO_PENDING == GetLastError() )
				return false;
			else
				Except( Exception::ERR_CANNOT_READ_FILE, "异步读取文件数据失败。" );
		}

		if( dwReadLen != dwLen )
			Except( Exception::ERR_CANNOT_READ_FILE, "异步读取文件数据长度错误。" );

		return true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//异步写入文件数据
	bool BaseFile::AsynWrite( const void* pBuf, DWORD dwLen, LPOVERLAPPED pOverlapped )
	{
		//同步读写操作
		_SyncOperation();

		DWORD dwWroteLen;
		BOOL bResult = WriteFile( mFileHandle, pBuf, dwLen, &dwWroteLen, pOverlapped );
		if( bResult == FALSE )
		{
			if( ERROR_IO_PENDING == GetLastError() )
				return false;
			else
				Except( Exception::ERR_CANNOT_WRITE_FILE, "异步写入文件数据失败。" );
		}

		if( dwWroteLen != dwLen )
			Except( Exception::ERR_CANNOT_WRITE_FILE, "异步写入文件数据长度错误。" );

		return true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取异步读写结果
	DWORD BaseFile::GetAsynResult( LPOVERLAPPED pOverlapped, bool bWait )
	{
		DWORD dwTransLen = 0;
		if( FALSE == GetOverlappedResult( mFileHandle, pOverlapped, &dwTransLen, bWait ? TRUE : FALSE ) )
			return 0;

		return dwTransLen;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置当前读写位置为文件结尾
	void BaseFile::SetEndOfFile()
	{
		//同步读写操作
		_SyncOperation();

		if( FALSE == ::SetEndOfFile( mFileHandle ) )
			Except( Exception::ERR_WINAPI_ERROR, "设置文件结尾失败。" );
	}
	//--------------------------------------------------------------------------------------------------------------
	//将缓存数据写入文件
	void BaseFile::Flush()
	{
		if( mLastBuffedOp != BOT_Write )
			Except( Exception::ERR_INVALIDPARAMS,
			"上次进行的缓存操作并不是写入操作，因此无法执行将缓存数据写入文件的命令。" );

		if( mdwCachedLen == 0 )
			return;

		if( mdwCachedLen != _InternalWrite( mpCache, mdwCachedLen ) )
			Except( Exception::ERR_CANNOT_WRITE_FILE, STR_ERR_WRITE_LEN );

		mpCachePtr = mpCache;
		mdwCachedLen = 0;

		mLastBuffedOp = BOT_None;
	}
	//--------------------------------------------------------------------------------------------------------------
	//同步读写操作
	void BaseFile::_SyncOperation()
	{
		//如果未进行缓存操作则直接返回
		if( mLastBuffedOp == BOT_None )
			return;

		//如果存在缓存数据
		if( mdwCachedLen != 0 )
		{
			//判断最后进行的操作
			switch (mLastBuffedOp)
			{
				//读取操作
			case BOT_Read:
				{
					//同步读写指针位置
					if( SetFilePointer( mFileHandle, -(long)mdwCachedLen, NULL, FILE_CURRENT )
						== INVALID_SET_FILE_POINTER )
						Except( Exception::ERR_WINAPI_ERROR, "同步读写指针位置失败。" );

					mpCachePtr = mpCache;
					mdwCachedLen = 0;
					break;
				}
				//写入操作
			case BOT_Write:
				{
					//将缓存数据写入文件
					Flush();
					break;
				}
			}
		}

		mLastBuffedOp = BOT_None;
	}
	//--------------------------------------------------------------------------------------------------------------
	//内部读取文件数据
	DWORD BaseFile::_InternalRead( BYTE* pBuf, DWORD dwLen )
	{
		DWORD dwReadLen;
		BOOL bResult = ReadFile( mFileHandle, pBuf, dwLen, &dwReadLen, NULL );
		if( bResult == FALSE )
			Except( Exception::ERR_CANNOT_READ_FILE, "读取文件数据失败。" );

		return dwReadLen;
	}
	//--------------------------------------------------------------------------------------------------------------
	//内部写入文件数据
	DWORD BaseFile::_InternalWrite( const BYTE* pBuf, DWORD dwLen )
	{
		DWORD dwWroteLen;
		BOOL bResult = WriteFile( mFileHandle, pBuf, dwLen, &dwWroteLen, NULL );
		if( bResult == FALSE )
			Except( Exception::ERR_CANNOT_WRITE_FILE, "写入文件数据失败。" );

		return dwWroteLen;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
