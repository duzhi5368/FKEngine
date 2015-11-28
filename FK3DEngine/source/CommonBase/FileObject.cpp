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
	FileObject::FileObject()
		: mnCurDirLen	(0)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	FileObject::~FileObject()
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置文件当前目录
	void FileObject::SetFileDirectory( LPCSTR szCurDir )
	{
		if( szCurDir == NULL )
		{
			mszCurDir[0] = '\0';
			mnCurDirLen = 0;
		}

		mnCurDirLen = (UINT)strlen( szCurDir );
		memcpy( mszCurDir, szCurDir, mnCurDirLen );

		mszCurDir[ mnCurDirLen ] = '\\';
		mszCurDir[ ++mnCurDirLen ] = '\0';
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取指定文件的状态
	int FileObject::GetFileStatus( LPCSTR szFileName )
	{
		//获取文件属性
		DWORD dwAttributes = GetFileAttributes( szFileName );

		//首先判断文件是否存在
		int nFileStatus;
		if( dwAttributes == INVALID_FILE_ATTRIBUTES )
			return FS_NotExist;
		else
			nFileStatus = FS_Exist;

		//是否为目录
		if( dwAttributes & FILE_ATTRIBUTE_DIRECTORY )
			nFileStatus |= FS_Directory;
		//是否为只读
		if( dwAttributes & FILE_ATTRIBUTE_READONLY )
			nFileStatus |= FS_ReadOnly;
		//是否为隐藏
		if( dwAttributes & FILE_ATTRIBUTE_HIDDEN )
			nFileStatus |= FS_Hidden;
		//是否为系统
		if( dwAttributes & FILE_ATTRIBUTE_SYSTEM )
			nFileStatus |= FS_System;
		//是否为存档
		if( dwAttributes & FILE_ATTRIBUTE_ARCHIVE )
			nFileStatus |= FS_Archive;
		//是否为压缩
		if( dwAttributes & FILE_ATTRIBUTE_COMPRESSED )
			nFileStatus |= FS_Compressed;

		return nFileStatus;
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除文件
	bool FileObject::Delete( LPCSTR szFileName, bool bDeleteReadOnly )
	{
		//删除文件
		BOOL nResult = DeleteFile( szFileName );

		//如果删除失败
		if( nResult == FALSE )
		{
			//如果是因为该文件为只读属性且允许删除只读文件
			if( ERROR_ACCESS_DENIED == GetLastError() && bDeleteReadOnly )
			{
				//去掉文件只读属性
				if( FALSE == SetFileAttributes( szFileName, FILE_ATTRIBUTE_NORMAL ) )
					return false;

				//删除文件
				return ( FALSE == DeleteFile( szFileName ) ) ? false : true;
			}
		}

		return true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//内部打开文件操作
	void FileObject::_InternalOpen( LPCSTR szFileName, OpenType eOpenType, bool bOverlapped, HANDLE* pHandle )
	{
		//检查文件名
		if( szFileName[0] == '\0' )
			Except( Exception::ERR_INVALIDPARAMS, "打开文件名为空。" );

		char* pFilePath = (char*)szFileName;

		//判断输入路径是否为绝对路径
		bool bAbsPath = false;
		for( char* pChar = (char*)szFileName; *pChar != '\0'; ++pChar )
		{
			if( *pChar == ':' )
			{
				bAbsPath = true;
				break;
			}
		}

		//如果不是绝对路径则在文件当前目录名附加文件名
		if( !bAbsPath )
		{
			UINT nFileNameLen = (UINT)strlen( szFileName );
			memcpy( mszCurDir + mnCurDirLen, szFileName, nFileNameLen + 1 );
			pFilePath = mszCurDir;
		}

		//检测是否已打开文件
		if( *pHandle != NULL )
			Except( Exception::ERR_INTERNAL_ERROR, "无法重复打开文件。" );

		DWORD dwAccessFlag, dwShareMode, dwCreationDisposition;
		DWORD dwFlags = bOverlapped ? FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED : FILE_ATTRIBUTE_NORMAL;

		//检测文件打开类型
		switch (eOpenType)
		{
			//只允许读取（文件必须存在）
		case OT_Read:
			dwAccessFlag			= GENERIC_READ;
			dwShareMode				= FILE_SHARE_READ;
			dwCreationDisposition	= OPEN_EXISTING;
			break;
			//只允许写入（覆盖原文件）
		case OT_Write:
			dwAccessFlag			= GENERIC_WRITE;
			dwShareMode				= FILE_SHARE_WRITE;
			dwCreationDisposition	=
				( FileObject::GetFileStatus( szFileName ) != FS_NotExist ) ? TRUNCATE_EXISTING : CREATE_NEW;
			break;
			//只允许写入（允许附加数据）
		case OT_WriteAppend:
			dwAccessFlag			= GENERIC_WRITE;
			dwShareMode				= FILE_SHARE_WRITE;
			dwCreationDisposition	= OPEN_ALWAYS;
			break;
			//读取并写入（文件必须存在）
		case OT_ReadWirte:
			dwAccessFlag			= GENERIC_READ | GENERIC_WRITE;
			dwShareMode				= FILE_SHARE_READ | FILE_SHARE_WRITE;
			dwCreationDisposition	= OPEN_ALWAYS;
			break;
		default:
			Except( Exception::ERR_INVALIDPARAMS,
				(String)"使用非法的打开类型打开文件'" + (String)szFileName + "'。" );
		}

		//创建文件句柄
		*pHandle = CreateFile( pFilePath, dwAccessFlag, dwShareMode, NULL, dwCreationDisposition, dwFlags, NULL );
		if( *pHandle == INVALID_HANDLE_VALUE )
			Except( Exception::ERR_CANNOT_OPEN_FILE, (String)"无法打开文件 '" + pFilePath + "'。" );

		//恢复当前目录名结尾
		if( !bAbsPath )
			mszCurDir[ mnCurDirLen ] = '\0';
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
