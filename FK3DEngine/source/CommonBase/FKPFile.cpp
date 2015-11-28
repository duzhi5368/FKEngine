/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	FKPFile
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
	FKPFile::FKPFile()
		: mpCurFileInfo	(NULL)
		, mnRemainLen	(0)
		, mnCurPos		(0)
	{
		memset( &mPackInfo, 0, sizeof(FKPPackInfo) );

		mDirInfoList.Initialize( 32, 32 );
		mFileInfoList.Initialize( 256, 256 );
	}
	//--------------------------------------------------------------------------------------------------------------
	FKPFile::~FKPFile()
	{
		ClosePack();

		mDirInfoList.Release();
		mFileInfoList.Release();
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建打包文件
	void FKPFile::CreatePack( LPCSTR szPackName, LPCSTR szBasePath, StringArray* pFileNames, int nSubLevel )
	{
		//首先关闭文件包
		ClosePack();

		//备份当前程序路径
		char szPathBackup[MAX_PATH];
		if( FALSE == GetCurrentDirectory( MAX_PATH, szPathBackup ) )
			Except( Exception::ERR_WINAPI_ERROR, "获取程序当前路径失败。" );

		//将当前路径设置为需要打包文件的基准路径
		if( szBasePath != NULL )
		{
			if( FALSE == SetCurrentDirectory( szBasePath ) )
				Except( Exception::ERR_WINAPI_ERROR, (String)"进入指定的路径 '" + szBasePath + "' 失败。" );
		}


		//遍历根目录下需要打包的文件
		StringArray::Iterator it = pFileNames->Begin();
		StringArray::Iterator end = pFileNames->End();
		for(; it!=end; ++it )
		{
			//判断文件状态是否允许
			int nFileStatus = FileObject::GetFileStatus( it->mpCharString );
			if( nFileStatus == FS_NotExist )
			{
				Except( Exception::ERR_INVALIDPARAMS, (String)"文件 '" + it->mpCharString + "' 不存在，无法打包。" );
			}
			else if( nFileStatus & FS_Compressed )
			{
				Except( Exception::ERR_INVALIDPARAMS, (String)"文件 '" + it->mpCharString + "' 是压缩文件，无法打包。" );
			}

			//截去路径名并检查文件名长度
			int nFileTitlePos = it->GetLastCharIndex('\\') + 1;
			int nFileTitleLen = (int)it->mnCharLength - nFileTitlePos;
			char* pFileName = it->mpCharString + nFileTitlePos;
			if( nFileTitleLen > FKP_FileNameLen - 1 )
			{
				Except( Exception::ERR_INVALIDPARAMS, (String)"文件 '" + it->mpCharString + "' 名称过长，无法打包。" );
			}

			//如果文件为目录
			if( nFileStatus & FS_Directory )
			{
				//将其加入目录列表
				FKPDirInfo* pDirInfo = mDirInfoList.Push();

				memcpy( pDirInfo->szDirName, pFileName, nFileTitleLen + 1 );
				pDirInfo->nNameID = Misc::HashString( pFileName );
			}
			//如果是普通文件（非目录）
			else
			{
				//将其加入文件列表
				FKPFileInfo* pFileInfo = mFileInfoList.Push();

				memcpy( pFileInfo->szFileName, pFileName, nFileTitleLen + 1 );
				pFileInfo->nNameID = Misc::HashString( pFileName );
			}
		}

		mPackInfo.nRootDirEnd		= mDirInfoList.Size();
		mPackInfo.nRootFileEnd		= mFileInfoList.Size();

		//排序根目录下的文件和目录信息列表
		_SortDirInfoByName( mDirInfoList.Begin(), mPackInfo.nRootDirEnd );
		_SortFileInfoByName( mFileInfoList.Begin(), mPackInfo.nRootFileEnd );


		//如果存在目录并且要求打包子目录内的文件
		if( mDirInfoList.Size() > 0 && ( nSubLevel > 0 || nSubLevel == -1 ) )
		{
			FKPDirInfoList::Iterator it = mDirInfoList.Begin();
			FKPDirInfoList::Iterator end = mDirInfoList.End();
			for(; it!=end; ++it )
				_GetFileInfoList( it, nSubLevel );
		}


		//设置打包文件头信息
		mPackInfo.nNumFiles			= mFileInfoList.Size();
		mPackInfo.nNumDirs			= mDirInfoList.Size();
		mPackInfo.nUncompLen		= 0;
		mPackInfo.nCompLen			= 0;

		mPackInfo.nDirInfoLen		= sizeof(FKPDirInfo) * mPackInfo.nNumDirs;
		mPackInfo.nFileInfoLen		= sizeof(FKPFileInfo) * mPackInfo.nNumFiles;


		//创建打包文件
		mPackFile.Open( szPackName, OT_Write );

		//写入文件标识
		mPackFile.Write( szFKPFileHeader, sizeof(szFKPFileHeader) );

		//为打包信息数据段预留空间
		UINT nPackHeaderChunkSize = sizeof(FKPPackInfo) + mPackInfo.nDirInfoLen + mPackInfo.nFileInfoLen;

		mPackFile.SetPosition( nPackHeaderChunkSize, BP_Curpos );
		mPackFile.SetEndOfFile();

		//按照列表压缩文件

		//压缩根目录下文件
		FKPFileInfoList::Iterator fit = mFileInfoList.Begin();
		FKPFileInfoList::Iterator fend = mFileInfoList.Begin() + mPackInfo.nRootFileEnd;
		for(; fit!=fend; ++fit )
			_CompressFile( fit );

		//压缩根目录下目录中的文件
		FKPDirInfoList::Iterator dit = mDirInfoList.Begin();
		FKPDirInfoList::Iterator dend = mDirInfoList.Begin() + mPackInfo.nRootDirEnd;
		for(; dit!=dend; ++dit )
			_CompressFilesInDir( dit );

		//写入已统计完成的文件头数据段信息
		PRTDataChunk sHeaderData;
		sHeaderData.SetPassword( pFKPPassword );
		sHeaderData.Allocate( nPackHeaderChunkSize );

		sHeaderData.Code( &mPackInfo, sizeof(FKPPackInfo) );

		//写入目录信息列表和文件信息列表
		sHeaderData.Code( mDirInfoList.Begin(), mPackInfo.nDirInfoLen );
		sHeaderData.Code( mFileInfoList.Begin(), mPackInfo.nFileInfoLen );

		mPackFile.SetPosition( sizeof(szFKPFileHeader) );
		sHeaderData.SetPosition( 0 );

		mPackFile.Write( sHeaderData.GetPointer(), nPackHeaderChunkSize );

		//关闭文件
		mPackFile.Close();

		sHeaderData.Free();


		//恢复原路径
		if( FALSE == SetCurrentDirectory( szPathBackup ) )
			Except( Exception::ERR_WINAPI_ERROR, (String)"进入指定的路径 '" + szPathBackup + "' 失败。" );
	}
	//--------------------------------------------------------------------------------------------------------------
	//打开打包文件
	void FKPFile::OpenPack( LPCSTR szPackName )
	{
		//首先关闭文件包
		ClosePack();

		//打开打包文件
		mPackFile.Open( szPackName, OT_Read );

		//读取文件标识
		char szHeaderString[ sizeof(szFKPFileHeader) ];
		mPackFile.Read( szHeaderString, sizeof(szFKPFileHeader) );

		//验证文件正确性
		if( 0 != memcmp( szHeaderString, szFKPFileHeader, sizeof(szFKPFileHeader) ) )
			Except( Exception::ERR_INTERNAL_ERROR, (String)"文件 '" + szPackName + "' 不是正确的 FKP 打包文件。" );

		//读取打包文件头数据段
		PRTDataChunk sHeaderData;
		sHeaderData.SetPassword( pFKPPassword );

		sHeaderData.Allocate( sizeof(FKPPackInfo) );
		mPackFile.Read( sHeaderData.GetPointer(), sizeof(FKPPackInfo) );

		//解析打包文件头信息
		sHeaderData.Decode( &mPackInfo, sizeof(FKPPackInfo) );

		//计算加上文件和目录信息列表的头数据段长度
		UINT nPackHeaderChunkSize = sizeof(FKPPackInfo) + mPackInfo.nDirInfoLen + mPackInfo.nFileInfoLen;
		sHeaderData.Reallocate( nPackHeaderChunkSize );

		mPackFile.Read( sHeaderData.GetPointer(), mPackInfo.nDirInfoLen + mPackInfo.nFileInfoLen );

		FKPDirInfo* pDirInfo = mDirInfoList.Push( mPackInfo.nNumDirs );
		FKPFileInfo* pFileInfo = mFileInfoList.Push( mPackInfo.nNumFiles );

		sHeaderData.Decode( pDirInfo, mPackInfo.nDirInfoLen );
		sHeaderData.Decode( pFileInfo, mPackInfo.nFileInfoLen );

		sHeaderData.Free();
	}
	//--------------------------------------------------------------------------------------------------------------
	//在打包文件内增加文件
	void FKPFile::AddToPack( StringArray* pFileNames, int nSubLevel )
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	//从打包文件内删除文件
	void FKPFile::DelFromPack( StringArray* pFileNames )
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取包内文件列表
	void FKPFile::GetFileListInPack( FileSelectList* pFileList )
	{
		//将根路径下的文件加入列表
		FKPFileInfoList::Iterator fit = mFileInfoList.Begin();
		FKPFileInfoList::Iterator fend = mFileInfoList.Begin() + mPackInfo.nRootFileEnd;
		for(; fit!=fend; ++fit )
			pFileList->AddFileToList( fit->szFileName );

		//将根路径下的目录加入列表
		FKPDirInfoList::Iterator dit = mDirInfoList.Begin();
		FKPDirInfoList::Iterator dend = mDirInfoList.Begin() + mPackInfo.nRootDirEnd;
		for(; dit!=dend; ++dit )
		{
			FileSelectList* pSubFileList = pFileList->AddDirToList( dit->szDirName );

			//生成该子目录的文件列表
			_MakeFileListOfDir( dit, pSubFileList );
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//关闭打包文件
	bool FKPFile::ClosePack()
	{
		//关闭包中打开的文件
		if( !Close() )
			return false;

		if( !mPackFile.Close() )
			return false;

		memset( &mPackInfo, 0, sizeof(FKPPackInfo) );

		mDirInfoList.Clear();
		mFileInfoList.Clear();

		return true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//打开文件
	void FKPFile::Open( LPCSTR szFileName, OpenType eOpenType, bool bOverlapped )
	{
		UINT nBeginDir = 0;
		UINT nEndDir = mPackInfo.nRootDirEnd;
		UINT nBeginFile = 0;
		UINT nEndFile = mPackInfo.nRootFileEnd;

		//分析路径中的目录
		static char szPathNameBuf[ MAX_PATH ] = { NULL };

		int nStrLen = (int)strlen( szFileName );
		memcpy( szPathNameBuf, szFileName, nStrLen + 1 );

		char* pPathName = szPathNameBuf;

		//循环分析每一级目录
		for(;;)
		{
			//寻找第一个 '\' 字符
			char* pChar = pPathName;
			for( int i=0; i<nStrLen; ++i )
			{
				if( *pChar == '\\' )
					goto FoundDir;

				++pChar;
			}

			//如果找不到目录则退出循环
			break;

			//截去目录名称
FoundDir:
			*pChar = '\0';
			char* pDirName = pPathName;
			pPathName = ++pChar;

			//计算该目录名哈希值
			UINT nDirNameID = Misc::HashString( pDirName );

			//在目录信息列表中寻找该级目录
			FKPDirInfo* pDirInfo = mDirInfoList.Begin() + nBeginDir;

			//使用折半查找法计算该目录信息的位置
			int nFindBegin = 0;
			int nFindEnd = nEndDir - nBeginDir - 1;
			int nFindMiddle;
			FKPDirInfo* pFindDirInfo = NULL;
			
			while( nFindBegin <= nFindEnd )
			{
				nFindMiddle = ( nFindBegin + nFindEnd ) / 2;

				pFindDirInfo = pDirInfo + nFindMiddle;

				//如果找到哈希值相同的项
				if( pFindDirInfo->nNameID == nDirNameID )
				{
					//再次进行字符串比对
					if( 0 == strcmp( pFindDirInfo->szDirName, pDirName ) )
						goto FoundDirInfo;
				}

				if( nDirNameID >= pFindDirInfo->nNameID )
					nFindBegin = nFindMiddle + 1;
				else
					nFindEnd = nFindMiddle - 1;
			}

			Except( Exception::ERR_INVALIDPARAMS, (String)"在包内找不到指定的目录 '" + pDirName + "'。" );

			//找到指定的目录
FoundDirInfo:
			nBeginDir	= pFindDirInfo->nSubDirInfoBegin;
			nEndDir		= pFindDirInfo->nSubDirInfoEnd;
			nBeginFile	= pFindDirInfo->nFileInfoBegin;
			nEndFile	= pFindDirInfo->nFileInfoEnd;
		}
		

		//计算该文件名哈希值
		char* pFileName = pPathName;
		UINT nFileNameID = Misc::HashString( pFileName );

		//使用折半查找法计算该文件信息的位置
		FKPFileInfo* pFileInfo = mFileInfoList.Begin() + nBeginFile;

		int nFindBegin = 0;
		int nFindEnd = nEndFile - nBeginFile - 1;
		int nFindMiddle;
		FKPFileInfo* pFindFileInfo = NULL;

		while( nFindBegin <= nFindEnd )
		{
			nFindMiddle = ( nFindBegin + nFindEnd ) / 2;

			pFindFileInfo = pFileInfo + nFindMiddle;

			//如果找到哈希值相同的项
			if( pFindFileInfo->nNameID == nFileNameID )
			{
				//再次进行字符串比对
				if( 0 == strcmp( pFindFileInfo->szFileName, pFileName ) )
					goto FoundFileInfo;
			}

			if( nFileNameID >= pFindFileInfo->nNameID )
				nFindBegin = nFindMiddle + 1;
			else
				nFindEnd = nFindMiddle - 1;
		}

		Except( Exception::ERR_INVALIDPARAMS, (String)"在包内找不到指定的文件 '" + pFileName + "'。" );

		//找到指定文件
FoundFileInfo:
		mpCurFileInfo = pFindFileInfo;
		mnRemainLen = pFindFileInfo->nUncompLen;
	}
	//--------------------------------------------------------------------------------------------------------------
	//关闭文件
	bool FKPFile::Close()
	{
		mpCurFileInfo = NULL;
		mnCurPos = mnRemainLen = 0;

		return true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取文件长度
	DWORD FKPFile::GetLength()
	{
		if( mpCurFileInfo == NULL )
			return 0;

		return mpCurFileInfo->nUncompLen;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置文件读写指针位置
	void FKPFile::SetPosition( long Offset, BasePos eBasePos )
	{
		long TmpRemainLen = (long)mnRemainLen;

		//计算剩余可读写长度
		switch (eBasePos)
		{
		case BP_Begin:
			TmpRemainLen = (long)mpCurFileInfo->nUncompLen - Offset;
			break;
		case BP_End:
			TmpRemainLen = -Offset;
			break;
		case BP_Curpos:
			TmpRemainLen -= Offset;
			break;
		}

		//检查是否超出可读取范围
		if( TmpRemainLen > (long)mpCurFileInfo->nUncompLen || TmpRemainLen < 0 )
			Except( Exception::ERR_INVALIDPARAMS, "不允许将数据块读写指针移动到 FKP 文件读写范围之外。" );

		mnRemainLen = TmpRemainLen;

		mnCurPos = mpCurFileInfo->nUncompLen - mnRemainLen;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取文件读写指针位置
	DWORD FKPFile::GetPosition()
	{
		return mpCurFileInfo->nUncompLen;
	}
	//--------------------------------------------------------------------------------------------------------------
	//读取文件数据
	void FKPFile::Read( void* pBuf, DWORD dwLen )
	{
		//如果剩余长度不足则禁止读取
		if( dwLen > mnRemainLen )
			Except( Exception::ERR_INVALIDPARAMS, "无法读取文件结尾之后的数据。" );

		//将打包文件读写指针移到该文件数据存储起始位置
		mPackFile.SetPosition( mpCurFileInfo->nOffset );

		UINT nBeginBlock = mnCurPos / FKP_BlockLen;		//当前需要读取的起始数据块
		UINT nBeginOffset = mnCurPos % FKP_BlockLen;	//起始数据块中的读取起始位置

		UINT nEndBlock;
		UINT nEndOffset;
		
		//如果读取总长超过首个数据块读取长度
		if( dwLen > FKP_BlockLen - nBeginOffset )
		{
			//除去第一个读取数据块的读取长度后的剩余长度
			DWORD dwRemainLen = dwLen - ( FKP_BlockLen - nBeginOffset );

			nEndBlock = dwRemainLen / FKP_BlockLen + 1;	//当前需要读取的结束数据块
			nEndOffset = dwRemainLen % FKP_BlockLen;	//结束数据块中的读取结束位置
		}
		else
		{
			nEndBlock = nBeginBlock;
			nEndOffset = nBeginOffset + dwLen;
		}

		//首先将读写指针移动到第一个读取数据块的位置
		FKPBlockHeader sBlockHeader;
		UINT nBlock = 0;
		for(; nBlock<nBeginBlock; ++nBlock )
		{
			//读取数据块头
			mPackFile.Read( &sBlockHeader, sizeof(FKPBlockHeader) );

			//略过该数据块
			mPackFile.SetPosition( sBlockHeader.nCompLen, BP_Curpos );
		}

		//循环读取所有数据块
		for(; nBlock<nEndBlock+1; ++nBlock )
		{
			mPackFile.Read( &sBlockHeader, sizeof(FKPBlockHeader) );//读取数据块头			
			mPackFile.Read( mpCompData, sBlockHeader.nCompLen );	//读取原始数据

			if( sBlockHeader.nUncompLen > FKP_BlockLen )
				Except( Exception::ERR_INTERNAL_ERROR, "压缩包数据块长度错误。" );

			BYTE* pDecodeData;
			size_t nCopyLen = FKP_BlockLen;

			//如果是压缩数据段
			if( sBlockHeader.nCompLen != sBlockHeader.nUncompLen )
			{
				if( sBlockHeader.nUncompLen != (UINT)_Decompress( mpCompData, sBlockHeader.nCompLen, mpUncompData ) )
					Except( Exception::ERR_INTERNAL_ERROR, "解压缩输出数据长度错误。" );

				pDecodeData = mpUncompData;
			}
			//如果是未压缩数据段
			else
			{
				pDecodeData = mpCompData;
			}

			//如果是结束数据块
			if( nBlock == nEndBlock )
			{
				nCopyLen = nEndOffset;
			}

			//如果是起始数据块
			if( nBlock == nBeginBlock )
			{
				pDecodeData += nBeginOffset;
				nCopyLen -= nBeginOffset;
			}

			memcpy( pBuf, pDecodeData, nCopyLen );
			pBuf = (BYTE*)pBuf + nCopyLen;
		}

		mnCurPos += dwLen;
		mnRemainLen -= dwLen;
	}
	//--------------------------------------------------------------------------------------------------------------
	//写入文件数据
	void FKPFile::Write( const void* pBuf, DWORD dwLen )
	{
		Except( Exception::ERR_INVALIDPARAMS, "无法在 FKP 打包文件内写入数据。" );
	}
	//--------------------------------------------------------------------------------------------------------------
	//异步读取文件数据
	bool FKPFile::AsynRead( void* pBuf, DWORD dwLen, LPOVERLAPPED pOverlapped )
	{
		Except( Exception::ERR_INVALIDPARAMS, "无法在 FKP 打包文件内异步读取数据。" );
	}
	//--------------------------------------------------------------------------------------------------------------
	//异步写入文件数据
	bool FKPFile::AsynWrite( const void* pBuf, DWORD dwLen, LPOVERLAPPED pOverlapped )
	{
		Except( Exception::ERR_INVALIDPARAMS, "无法在 FKP 打包文件内异步写入数据。" );
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取异步读写结果
	DWORD FKPFile::GetAsynResult( LPOVERLAPPED pOverlapped, bool bWait )
	{
		Except( Exception::ERR_CANNOT_WRITE_FILE, "无法获取 FKP 打包文件的异步数据读写结果。" );
	}
	//--------------------------------------------------------------------------------------------------------------
	//根据文件名生成一个唯一数字 ID （经过测试，重复几率非常微小，但不敢保证绝对不重复。）
	unsigned int FKPFile::FileNameToID( const char* szFileName )
	{
		return Misc::HashString( szFileName );
	}
	//--------------------------------------------------------------------------------------------------------------
	//生成包内一级目录下的文件列表
	void FKPFile::_MakeFileListOfDir( FKPDirInfo* pDirInfo, FileSelectList* pFileList )
	{
		//将根路径下的文件加入列表
		FKPFileInfoList::Iterator fit = mFileInfoList.Begin() + pDirInfo->nFileInfoBegin;
		FKPFileInfoList::Iterator fend = mFileInfoList.Begin() + pDirInfo->nFileInfoEnd;
		for(; fit!=fend; ++fit )
			pFileList->AddFileToList( fit->szFileName );

		//将根路径下的目录加入列表
		FKPDirInfoList::Iterator dit = mDirInfoList.Begin() + pDirInfo->nSubDirInfoBegin;
		FKPDirInfoList::Iterator dend = mDirInfoList.Begin() + pDirInfo->nSubDirInfoEnd;
		for(; dit!=dend; ++dit )
		{
			FileSelectList* pSubFileList = pFileList->AddDirToList( dit->szDirName );

			//生成该子目录的文件列表
			_MakeFileListOfDir( dit, pSubFileList );
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取指定路径下的文件信息列表
	void FKPFile::_GetFileInfoList( FKPDirInfo* pDirInfo, int nSubLevel )
	{
		//设置当前路径
		if( FALSE == SetCurrentDirectory( pDirInfo->szDirName ) )
			Except( Exception::ERR_WINAPI_ERROR, (String)"进入指定的路径 '" + pDirInfo->szDirName + "' 失败。" );

		//查找第一个文件
		WIN32_FIND_DATA FileData;
		HANDLE hFind = FindFirstFile( "*.*", &FileData );
		if( hFind == INVALID_HANDLE_VALUE )
		{
			//如果查找失败
			if( GetLastError() != ERROR_NO_MORE_FILES )
				Except( Exception::ERR_WINAPI_ERROR, (String)"在路径 '" + pDirInfo->szDirName + "' 下查找文件失败。" );
		}

		//记录该目录下文件及子目录在列表中的开始位置
		UINT nFileListPos = mFileInfoList.Size();
		UINT nDirListPos = mDirInfoList.Size();
		pDirInfo->nFileInfoBegin = nFileListPos;
		pDirInfo->nSubDirInfoBegin = nDirListPos;

		//循环查找所有文件
		for(;;)
		{
			//获取文件名长度
			size_t nNameLen = strlen( FileData.cFileName ) + 1;
			if( nNameLen > FKP_FileNameLen )
				Except( Exception::ERR_INVALIDPARAMS, (String)"目录 '" + pDirInfo->szDirName +
				"' 名称过长，无法打包。" );

			//如果是目录
			if( FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			{
				if( 0 == strcmp( FileData.cFileName, "." ) || 0 == strcmp( FileData.cFileName, ".." ) )
					goto FindNext;

				//将其加入目录列表
				FKPDirInfo* pSubDirInfo = mDirInfoList.Push();

				memcpy( pSubDirInfo->szDirName, FileData.cFileName, nNameLen );
				pSubDirInfo->nNameID = Misc::HashString( pSubDirInfo->szDirName );

				++nDirListPos;
			}
			//如果是普通文件
			else if( FileData.dwFileAttributes )
			{
				//将其加入文件列表
				FKPFileInfo* pSubFileInfo = mFileInfoList.Push();

				memcpy( pSubFileInfo->szFileName, FileData.cFileName, nNameLen );
				pSubFileInfo->nNameID = Misc::HashString( pSubFileInfo->szFileName );

				++nFileListPos;
			}

FindNext:
			//寻找下一个文件
			if( FALSE == FindNextFile( hFind, &FileData ) )
			{
				//如果找不到文件
				if( GetLastError() == ERROR_NO_MORE_FILES )
				{
					break;
				}
				//如果查找失败
				else
				{
					Except( Exception::ERR_WINAPI_ERROR, (String)"在路径 '" + pDirInfo->szDirName + "' 下查找文件失败。" );
				}
			}
		}

		//关闭查找
		FindClose( hFind );

		//记录该目录下文件及子目录在列表中的结束（超尾）位置
		pDirInfo->nFileInfoEnd = nFileListPos;
		pDirInfo->nSubDirInfoEnd = nDirListPos;

		//排序根目录下的文件和目录信息列表
		_SortDirInfoByName( mDirInfoList.Begin() + pDirInfo->nSubDirInfoBegin,
			pDirInfo->nSubDirInfoEnd - pDirInfo->nSubDirInfoBegin );
		_SortFileInfoByName( mFileInfoList.Begin() + pDirInfo->nFileInfoBegin,
			pDirInfo->nFileInfoEnd - pDirInfo->nFileInfoBegin );


		//如果要求继续获取子目录下的文件信息
		if( nSubLevel == -1 || ( --nSubLevel >= 0 ) )
		{
			//获取子目录下的文件信息列表
			FKPDirInfoList::Iterator it = mDirInfoList.Begin() + pDirInfo->nSubDirInfoBegin;
			FKPDirInfoList::Iterator end = mDirInfoList.Begin() + pDirInfo->nSubDirInfoEnd;
			for(; it!=end; ++it )
				_GetFileInfoList( it, nSubLevel );
		}

		//返回上级路径
		if( FALSE == SetCurrentDirectory( ".." ) )
			Except( Exception::ERR_WINAPI_ERROR, "返回上级路径失败。" );
	}
	//--------------------------------------------------------------------------------------------------------------
	//根据名称哈希码排序目录信息
	void FKPFile::_SortDirInfoByName( FKPDirInfo* pDirInfo, UINT nNumInfos )
	{
		if( nNumInfos < 2 )
			return;

		FKPDirInfo TmpDirInfo;
		for( UINT i=0; i<nNumInfos-1; i++ )
		{
			for( unsigned j=i+1; j<nNumInfos; j++ )
			{
				if( pDirInfo[i].nNameID > pDirInfo[j].nNameID )
				{
					memcpy( &TmpDirInfo, pDirInfo + i, sizeof(FKPDirInfo) );
					memcpy( pDirInfo + i, pDirInfo + j, sizeof(FKPDirInfo) );
					memcpy( pDirInfo + j, &TmpDirInfo, sizeof(FKPDirInfo) );
				}
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//根据名称哈希码排序文件信息
	void FKPFile::_SortFileInfoByName( FKPFileInfo* pFileInfo, UINT nNumInfos )
	{
		if( nNumInfos < 2 )
			return;

		FKPFileInfo TmpFileInfo;
		for( UINT i=0; i<nNumInfos-1; i++ )
		{
			for( unsigned j=i+1; j<nNumInfos; j++ )
			{
				if( pFileInfo[i].nNameID > pFileInfo[j].nNameID )
				{
					memcpy( &TmpFileInfo, pFileInfo + i, sizeof(FKPFileInfo) );
					memcpy( pFileInfo + i, pFileInfo + j, sizeof(FKPFileInfo) );
					memcpy( pFileInfo + j, &TmpFileInfo, sizeof(FKPFileInfo) );
				}
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//按照列表信息压缩文件
	void FKPFile::_CompressFilesInDir( FKPDirInfo* pDirInfo )
	{
		//设置当前路径
		if( FALSE == SetCurrentDirectory( pDirInfo->szDirName ) )
			Except( Exception::ERR_WINAPI_ERROR, (String)"进入指定的路径 '" + pDirInfo->szDirName + "' 失败。" );

		//循环压缩该目录下所有文件
		FKPFileInfoList::Iterator fit = mFileInfoList.Begin() + pDirInfo->nFileInfoBegin;
		FKPFileInfoList::Iterator fend = mFileInfoList.Begin() + pDirInfo->nFileInfoEnd;
		for(; fit!=fend; ++fit )
			_CompressFile( fit );

		//循环压缩该目录下所有子目录
		FKPDirInfoList::Iterator dit = mDirInfoList.Begin() + pDirInfo->nSubDirInfoBegin;
		FKPDirInfoList::Iterator dend = mDirInfoList.Begin() + pDirInfo->nSubDirInfoEnd;
		for(; dit!=dend; ++dit )
			_CompressFilesInDir( dit );

		//返回上级路径
		if( FALSE == SetCurrentDirectory( ".." ) )
			Except( Exception::ERR_WINAPI_ERROR, "返回上级路径失败。" );
	}
	//--------------------------------------------------------------------------------------------------------------
	//压缩指定文件
	void FKPFile::_CompressFile( FKPFileInfo* pFileInfo )
	{
		//打开要进行打包的文件
		BaseFile SrcFile;
		SrcFile.Open( pFileInfo->szFileName );

		//获取当前读写位置
		pFileInfo->nOffset = mPackFile.GetPosition();

		//获取文件要进行打包的文件长度
		pFileInfo->nUncompLen = SrcFile.GetLength();
		pFileInfo->nCompLen = 0;

		//计算文件压缩分块数及最后一块长度
		UINT nNumFullBlock = (UINT)( pFileInfo->nUncompLen / FKP_BlockLen );
		UINT nRemLen = pFileInfo->nUncompLen % FKP_BlockLen;

		pFileInfo->nNumBlocks = ( nRemLen > 0 ) ? ( nNumFullBlock + 1 ) : nNumFullBlock;

		FKPBlockHeader sBlockHeader;
		void* pBlockData;

		//分块压缩文件
		for( UINT i=0; i<nNumFullBlock; ++i )
		{
			//读取未压缩数据
			SrcFile.Read( mpUncompData, FKP_BlockLen );

			//压缩数据
			sBlockHeader.nUncompLen = FKP_BlockLen;
			sBlockHeader.nCompLen = (UINT)_Compress( mpUncompData, FKP_BlockLen, mpCompData );

			//如果压缩后的数据比源数据长度还大则写入未压缩数据
			if( sBlockHeader.nCompLen > FKP_BlockLen )
			{
				sBlockHeader.nCompLen = FKP_BlockLen;
				pBlockData = mpUncompData;
			}
			//否则写入压缩后的数据
			else
			{
				pBlockData = mpCompData;
			}

			pFileInfo->nCompLen += sBlockHeader.nCompLen;

			//写入数据段头
			mPackFile.Write( &sBlockHeader, sizeof(FKPBlockHeader) );

			//写入段数据
			mPackFile.Write( pBlockData, sBlockHeader.nCompLen );
		}

		//写入剩余长度块
		if( nRemLen > 0 )
		{
			//读取未压缩数据
			SrcFile.Read( mpUncompData, nRemLen );

			//压缩数据
			sBlockHeader.nUncompLen = nRemLen;
			sBlockHeader.nCompLen = (UINT)_Compress( mpUncompData, nRemLen, mpCompData );

			//如果压缩后的数据比源数据长度还大则写入未压缩数据
			if( sBlockHeader.nCompLen > nRemLen )
			{
				sBlockHeader.nCompLen = nRemLen;
				pBlockData = mpUncompData;
			}
			//否则写入压缩后的数据
			else
			{
				pBlockData = mpCompData;
			}

			pFileInfo->nCompLen += sBlockHeader.nCompLen;

			//写入数据段头
			mPackFile.Write( &sBlockHeader, sizeof(FKPBlockHeader) );

			//写入段数据
			mPackFile.Write( pBlockData, sBlockHeader.nCompLen );
		}

		//关闭要进行打包的文件
		SrcFile.Close();

		//累计打包文件压缩和未压缩数据长度
		mPackInfo.nUncompLen += pFileInfo->nUncompLen;
		mPackInfo.nCompLen += pFileInfo->nCompLen;
	}
	//--------------------------------------------------------------------------------------------------------------
	//关闭转换指针地址到数值的警告
	#pragma warning( disable: 4311 )
	//--------------------------------------------------------------------------------------------------------------
	//进行压缩编码核心
	unsigned int FKPFile::_DoCompress( unsigned char* in, unsigned in_len, unsigned char* out, unsigned* out_len )
	{
		static long wrkmem [16384L] ;
		register unsigned char* ip;
		unsigned char* op;
		unsigned char* in_end = in + in_len;
		unsigned char* ip_end = in + in_len - 13;
		unsigned char* ii;
		unsigned char** dict = (unsigned char**)wrkmem;

		op = out;
		ip = in;
		ii = ip;

		ip += 4;

		for(;;) 
		{
			register unsigned char* m_pos;
			unsigned m_off;
			unsigned m_len;
			unsigned dindex;

			dindex = ((0x21 * (((((((unsigned)(ip[3]) << 6) ^ ip[2]) << 5) ^ ip[1]) << 5) ^ ip[0])) >> 5) & 0x3fff;
			m_pos = dict[dindex];

			if( ((unsigned)m_pos < (unsigned)in) || (m_off = (unsigned)((unsigned)ip-(unsigned)m_pos) ) <= 0 || m_off > 0xbfff )
			{
				goto literal;
			}

			if( m_off <= 0x0800  || m_pos[3] == ip[3])
				goto try_match;
			dindex  = (dindex  & 0x7ff) ^ 0x201f ;

			m_pos = dict[dindex];
			if( (unsigned)(m_pos)  < (unsigned)(in) || (m_off = (unsigned)( (int)((unsigned)ip-(unsigned)m_pos))) <= 0 || m_off>0xbfff )
			{
				goto literal;
			}
			if( m_off <= 0x0800  || m_pos[3] == ip[3])
				goto try_match;
			goto literal;

	try_match:
			if( *( unsigned short*)m_pos == *( unsigned short*)ip && m_pos[2] == ip[2]) 
				goto match;

	literal:
			dict[dindex] = ip;
			++ip;
			if( ip >= ip_end)
				break;
			continue;

	match:
			dict[dindex] =  ip;
			if( ip - ii > 0)
			{
				register unsigned t = (unsigned)ip - (unsigned)ii;

				if( t <= 3) 
					op[-2] |= (unsigned char)t;
				else if( t <= 18)
					*op++ = (unsigned char) (t - 3);
				else 
				{
					register unsigned tt = t - 18;
					*op++ = 0;
					while( tt > 255)
					{
						tt -= 255;
						*op++ = 0;
					}
					*op ++ = (unsigned char)tt;
				}
				do
				{
					*op ++ = *ii ++;
				}while( --t>0 );
			}

			ip += 3;

			if( m_pos[3] != *ip++ || m_pos[4] != *ip++ || m_pos[5] != *ip++ ||
				m_pos[6] != *ip++ || m_pos[7] != *ip++ || m_pos[8] != *ip++ )
			{
				--ip;
				m_len = (unsigned)ip - (unsigned)ii;

				if( m_off <= 0x0800)
				{
					-- m_off;
					*op ++ = (unsigned char)( ((m_len - 1) << 5) | ((m_off & 7) << 2) );
					*op ++ = (unsigned char)( m_off >> 3 );
				}
				else if( m_off <= 0x4000)
				{
					-- m_off;
					*op ++ = (unsigned char) (32  | (m_len - 2));
					goto m3_m4_offset;
				}
				else 
				{
					m_off -= 0x4000;
					*op ++ = (unsigned char) (16 | ((m_off & 0x4000) >> 11) | (m_len - 2) );
					goto m3_m4_offset;
				}
			}
			else
			{
				{
					unsigned char *end = in_end;
					unsigned char *m = m_pos + 9;
					while( ip < end && *m == *ip)
						m++, ip++;
					m_len = ( (unsigned)ip - (unsigned)ii );
				}

				if( m_off <= 0x4000) 
				{
					-- m_off;
					if( m_len <= 33)
						*op ++ = (unsigned char) (32  | (m_len - 2));
					else
					{
						m_len -= 33;
						*op ++=32;
						goto m3_m4_len;
					}
				}
				else
				{
					m_off -= 0x4000;
					if( m_len <= 9)
						*op++ = (unsigned char)(16|((m_off & 0x4000) >> 11) | (m_len - 2));
					else 
					{
						m_len -= 9;
						*op++ = (unsigned char) ( 16  | ((m_off & 0x4000) >> 11) );
	m3_m4_len:
						while( m_len > 255)
						{
							m_len -= 255;
							*op ++ = 0;
						}
						*op++ = (unsigned char)m_len;
					}
				}

	m3_m4_offset:
				*op ++ = (unsigned char) ((m_off & 63) << 2);
				*op ++ = (unsigned char) (m_off >> 6);
			}

			ii = ip;
			if( ip >= ip_end)
				break;
		}

		*out_len = (unsigned)op - (unsigned)out;
		return (unsigned)( in_end - ii );
	}
	//--------------------------------------------------------------------------------------------------------------
	//解压缩核心
	int FKPFile::_Decompress( void* in, unsigned in_len, void* out )
	{
		register unsigned char* op;
		register unsigned char* ip;
		register unsigned t;
		register unsigned char* m_pos;

		unsigned char* IpEnd = (unsigned char *)in + in_len;

		op = (unsigned char*)out;
		ip = (unsigned char*)in;

		if(*ip > 17)
		{
			t = *ip++ - 17;
			if(t < 4) goto match_next;
			do
			{
				*op++ = *ip++;
			}while (-- t > 0);
			goto first_literal_run;
		}

		while(1)
		{
			t = *ip++;
			if(t >= 16) goto match;
			if(t == 0)
			{
				while(*ip == 0)
				{
					t += 255;
					ip++;
				}
				t += 15 + *ip ++;
			}
			*(unsigned *)op = *(unsigned *)ip;
			op += 4;
			ip += 4;
			if(--t > 0)
			{
				if(t >= 4)
				{
					do
					{
						*(unsigned *) op = *(unsigned *) ip;
						op += 4;
						ip += 4;
						t -= 4;
					} while(t >= 4);
				if(t > 0) 
					do *op++ = *ip++; 
					while (--t > 0);
				}
				else
				{
					do
					{
						*op++ = *ip++;
					}while(-- t > 0);
				}
			}
	first_literal_run:
			t = *ip ++;
			if(t >= 16) goto match;

			m_pos = op - 0x0801;
			m_pos -= t >> 2;
			m_pos -= *ip ++ << 2;

			*op ++ = *m_pos ++;
			*op ++ = *m_pos ++;
			*op ++ = *m_pos;

			goto match_done;

			while(1)
			{
	match:
				if(t >= 64)
				{
					m_pos = op - 1;
					m_pos -= (t >> 2) & 7;
					m_pos -= *ip++ << 3;
					t = (t >> 5) - 1;

					goto copy_match;
				}
				else if(t >= 32)
				{
					t &= 31;
					if(t == 0)
					{
						while (*ip == 0)
						{
							t += 255;
							ip++;
						}
					t += 31 + *ip ++;
					}
					m_pos = op - 1;
					m_pos -= (*(unsigned short *)ip) >> 2;
					ip += 2;
				}
				else if(t >= 16)
				{
					m_pos = op;
					m_pos -= (t & 8) << 11;
					t &= 7;
					if(t == 0)
					{
						while (*ip == 0)
						{
							t += 255;
							ip++;
						}
						t += 7 + *ip ++;
					}
					m_pos -= (*(unsigned short *)ip) >> 2;
					ip += 2;
					if(m_pos == op) goto eof_found;
					m_pos -= 0x4000;
				}
				else
				{
					m_pos = op - 1;
					m_pos -= t >> 2;
					m_pos -= *ip ++ << 2;
					*op ++ = *m_pos ++;
					*op ++ = *m_pos;
					goto match_done;
				}

				if(t >= 6 && (op - m_pos) >= 4)
				{
					*(unsigned *)op = *(unsigned *)m_pos;
					op += 4;
					m_pos += 4;
					t -= 2;
					do
					{
						*(unsigned *)op = *(unsigned *)m_pos;
						op += 4;
						m_pos += 4;
						t -= 4;
					} while(t >= 4);
					if(t > 0)
					{
						do
						{
							*op ++ = *m_pos ++;
						}
						while (--t > 0);
					}
				}
				else
				{
	copy_match:
				*op ++ = *m_pos ++;
				*op ++ = *m_pos ++;
				do *op ++ = *m_pos ++; while(--t > 0);
				}
	match_done:
				t = ip[-2] & 3;
				if(t == 0) break;
	match_next:
				do
				{
					*op ++ = *ip ++;
				}while (--t > 0);
				t = *ip ++;
			}
		}
	eof_found:
		if(ip != IpEnd)
			return -1;
		return int(op - (unsigned char*)out);
	}
	//--------------------------------------------------------------------------------------------------------------
	//压缩数据
	int FKPFile::_Compress( void* in, unsigned in_len, void* out )
	{
		unsigned char* op = (unsigned char*)out;
		unsigned t, out_len;

		if(in_len <= 13)
			t = in_len;
		else
		{
			t = _DoCompress( (unsigned char*)in, in_len, op, &out_len );
			op += out_len;
		}

		if(t > 0)
		{
			unsigned char *ii = (unsigned char*)in + in_len - t;
			if( op == (unsigned char*)out && t <= 238 )
				*op ++ = (unsigned char) ( 17 + t );
			else if( t <= 3 )
				op[-2] |= (unsigned char)t ;
			else if( t <= 18 )
				*op ++ = (unsigned char)(t - 3);
			else
			{
				unsigned tt = t - 18;

				*op ++ = 0;
				while( tt > 255 )
				{
					tt -= 255;
					*op++ = 0;
				}
				*op ++ = (unsigned char)tt;
			}
			do
			{
				*op++ = *ii++;
			}while( --t > 0 );
		}

		*op++ = 17;
		*op++ = 0;
		*op++ = 0;

		return ( (unsigned)op - (unsigned)( (unsigned char*)out ) );
	}
	//--------------------------------------------------------------------------------------------------------------
	//恢复转换指针地址到数值的警告
	#pragma warning( default: 4311 )
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
