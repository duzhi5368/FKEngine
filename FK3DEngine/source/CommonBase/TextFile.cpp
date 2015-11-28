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
	TextFile::TextFile()
		: mFileHandle	(NULL)
		, mFileLen		(0)
		, mCurPos		(0)
		, mbIsChanged	(false)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	TextFile::TextFile( LPCSTR szFileName, OpenType eOpenType, bool bOverlapped )
		: mFileHandle	(NULL)
		, mFileLen		(0)
		, mCurPos		(0)
		, mbIsChanged	(false)
	{
		Open( szFileName, eOpenType, bOverlapped );
	}
	//--------------------------------------------------------------------------------------------------------------
	TextFile::~TextFile()
	{
		Close();
	}
	//--------------------------------------------------------------------------------------------------------------
	//打开文件
	void TextFile::Open( LPCSTR szFileName, OpenType eOpenType, bool bOverlapped )
	{
		//打开文件
		DWORD dwOrigFileLen;
		_InternalOpen( szFileName, eOpenType, bOverlapped, &mFileHandle );
		dwOrigFileLen = GetLength();

		//如果源文件存在数据
		if( dwOrigFileLen > 0 )
		{
			//分配数据翻译缓存
			mTranslateBuf.Initialize( dwOrigFileLen, 1024 );
			char* pTransBuf = mTranslateBuf.Push( dwOrigFileLen );

			//将所有文件数据读入缓存
			DWORD dwReadLen;
			BOOL bResult = ReadFile( mFileHandle, pTransBuf, dwOrigFileLen, &dwReadLen, NULL );
			if( bResult == FALSE || dwReadLen != dwOrigFileLen )
				Except( Exception::ERR_CANNOT_READ_FILE, "打开文本文件时读取文件数据失败。" );

			//翻译所有文件数据
			mFileLen = dwOrigFileLen;

			char* pChar = pTransBuf;
			for( DWORD i=0; i<dwOrigFileLen; ++i, ++pChar )
			{
				if( pChar[0] == '\r' && pChar[1] == '\n' )
				{
					mTranslateBuf.Erase( i );
					++i;
					++pChar;
					--mFileLen;
				}
			}
		}
		else
		{
			mTranslateBuf.Initialize( 1024, 1024 );
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//关闭文件
	bool TextFile::Close()
	{
		if( mFileHandle != NULL )
		{
			//如果更改过文本内容
			if( mbIsChanged )
			{
				//翻译文本数据
				char* pTransBuf = mTranslateBuf.Begin();
				char* pChar = pTransBuf;

				for( DWORD i=0; i<mFileLen; ++i, ++pChar )
				{
					if( *pChar == '\n' )
					{
						pChar = mTranslateBuf.Insert( i );
						*pChar = '\r';
						++pChar;
						++i;
						++mFileLen;
					}
				}

				//将文件读写指针移到文件头部
				if( SetFilePointer( mFileHandle, 0, NULL, FILE_BEGIN ) == INVALID_SET_FILE_POINTER )
				{
					SetLastException( Exception::ERR_WINAPI_ERROR, "移动文件读写指针失败。" );
					return false;
				}

				//写入文件数据
				DWORD dwUntransLen = mTranslateBuf.Size();
				void* pUntransData = (void*)mTranslateBuf.Begin();

				DWORD dwWroteLen;
				BOOL bResult = WriteFile( mFileHandle, pUntransData, dwUntransLen, &dwWroteLen, NULL );
				if( bResult == FALSE || dwWroteLen != dwUntransLen )
				{
					SetLastException( Exception::ERR_CANNOT_READ_FILE, "写入更改后的文本文件数据失败。" );
					return false;
				}
			}

			//关闭文件
			if( CloseHandle( mFileHandle ) == FALSE )
			{
				SetLastException( Exception::ERR_INTERNAL_ERROR, "关闭文件失败。" );
				return false;
			}

			mFileHandle = NULL;
		}

		mFileLen = 0;
		mCurPos = 0;
		mbIsChanged = false;

		//释放数据翻译缓存
		mTranslateBuf.Release();

		return true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取文件长度
	DWORD TextFile::GetLength()
	{
		return mFileLen;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置文件读写指针位置
	void TextFile::SetPosition( long Offset, BasePos eBasePos )
	{
		switch (eBasePos)
		{
		case BP_Begin:
			mCurPos = Offset;
			break;
		case BP_End:
			mCurPos = mFileLen + Offset;
			break;
		case BP_Curpos:
			mCurPos += Offset;
			break;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "错误的文件读写指针移动基准类型。" );
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取文件读写指针位置
	DWORD TextFile::GetPosition()
	{
		return mCurPos;
	}
	//--------------------------------------------------------------------------------------------------------------
	//读取文件数据
	void TextFile::Read( void* pBuf, DWORD dwLen )
	{
		char* pTransData = mTranslateBuf.Begin();

		memcpy( pBuf, pTransData + mCurPos, dwLen );

		mCurPos += dwLen;
	}
	//--------------------------------------------------------------------------------------------------------------
	//写入文件数据
	void TextFile::Write( const void* pBuf, DWORD dwLen )
	{
		//如果是在文本数据尾部写入则增加文件长度
		DWORD dwDataRear = mCurPos + dwLen;
		if( dwDataRear > mFileLen )
		{
			DWORD dwAppendLen = dwDataRear - mFileLen;
			mTranslateBuf.Push( dwAppendLen );

			mFileLen = dwDataRear;
		}

		char* pTransData = mTranslateBuf.Begin();

		memcpy( pTransData + mCurPos, pBuf, dwLen );

		mCurPos += dwLen;
		mbIsChanged = true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//异步读取文件数据
	bool TextFile::AsynRead( void* pBuf, DWORD dwLen, LPOVERLAPPED pOverlapped )
	{
		Except( Exception::ERR_CANNOT_READ_FILE, "无法在文本文件类的虚拟文件上进行异步数据读取。" );
	}
	//--------------------------------------------------------------------------------------------------------------
	//异步写入文件数据
	bool TextFile::AsynWrite( const void* pBuf, DWORD dwLen, LPOVERLAPPED pOverlapped )
	{
		Except( Exception::ERR_CANNOT_WRITE_FILE, "无法在文本文件类的虚拟文件上进行异步数据写入。" );
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取异步读写结果
	DWORD TextFile::GetAsynResult( LPOVERLAPPED pOverlapped, bool bWait )
	{
		Except( Exception::ERR_CANNOT_WRITE_FILE, "无法获取文本文件类的异步数据读写结果。" );
	}
	//--------------------------------------------------------------------------------------------------------------
	//写入格式化文件数据
	void TextFile::WriteFormat( const char* szFormat, ... )
	{
		//获取参数列表
		va_list args;
		va_start( args, szFormat );

		//获取格式化信息所需的字符串长度（还需要一个 NULL 结尾字符空间）
		DWORD dwLen = (DWORD)_vscprintf( szFormat, args ) + 1;

		//分配字符串内存
		char* pBuf = (char*)malloc( dwLen );
		if( pBuf == NULL )
			Except( Exception::ERR_NOT_ENOUGH_MEM, "写入格式化文件数据时，分配格式化数据的内存失败。" );

		//格式化数据
		vsprintf( pBuf, szFormat, args );

		//写入数据
		Write( pBuf, dwLen - 1 );
		SafeFree( pBuf );
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
