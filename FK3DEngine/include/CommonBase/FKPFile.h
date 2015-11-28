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
*	文件说明：	FKP 打包文件对象类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------

	//FKP 文件头标识
	const char szFKPFileHeader[] = "FK Pack File";

	const BYTE pFKPPassword[16] = {	0x29, 0x73, 0x0b, 0x27, 0xa0, 0x8d, 0x18, 0x02,
									0x5a, 0x58, 0xba, 0x3c, 0x67, 0x60, 0x25, 0x32 };

	const int		FKP_FileNameLen		= 32;			//文件名最大长度
	const UINT		FKP_BlockLen		= 0x20000;		//段长度
	const UINT		FKP_ProcBufLen		= 0x30000;		//数据处理缓存大小

	//--------------------------------------------------------------------------------------------------------------
	//FKP 打包文件对象类
	class FK_DLL FKPFile : public PackObject
	{
	protected:
		//打包文件头信息
		struct FKPPackInfo
		{
			UINT	nNumFiles;					//包内文件总数
			UINT	nNumDirs;					//包内目录总数
			DWORD	nUncompLen;					//所有文件未压缩大小
			DWORD	nCompLen;					//所有文件压缩大小

			DWORD	nDirInfoLen;				//目录信息段长度
			DWORD	nFileInfoLen;				//文件信息段长度

			UINT	nRootDirEnd;				//根目录下目录结束编号
			UINT	nRootFileEnd;				//根目录下文件结束编号
		};

		//被打包的目录信息
		struct FKPDirInfo
		{
			char	szDirName[FKP_FileNameLen];	//目录名称
			UINT	nNameID;					//目录名哈希码
			UINT	nSubDirInfoBegin;			//该目录下子目录起始编号
			UINT	nSubDirInfoEnd;				//该目录下子目录结束编号
			UINT	nFileInfoBegin;				//该目录下文件起始编号
			UINT	nFileInfoEnd;				//该目录下文件结束编号
		};
		typedef Stack< FKPDirInfo >		FKPDirInfoList;

		//被打包的文件信息
		struct FKPFileInfo
		{
			char	szFileName[FKP_FileNameLen];//文件名称
			UINT	nNameID;					//文件名哈希码
			UINT	nOffset;					//该文件起始数据段在打包文件内的位置
			UINT	nNumBlocks;					//该文件的数据段总数
			DWORD	nUncompLen;					//文件未压缩大小
			DWORD	nCompLen;					//文件压缩后大小
		};
		typedef Stack< FKPFileInfo >	FKPFileInfoList;

		//数据段头信息
		struct FKPBlockHeader
		{
			UINT nCompLen;		//段压缩数据长度
			UINT nUncompLen;	//段解压缩数据长度
		};

	protected:
		BYTE			mpUncompData[FKP_BlockLen];		//未压缩数据缓冲区指针
		BYTE			mpCompData[FKP_ProcBufLen];		//已压缩数据缓冲区指针

		FKPPackInfo		mPackInfo;		//打包文件头信息
		FKPDirInfoList	mDirInfoList;	//打包目录信息列表
		FKPFileInfoList	mFileInfoList;	//打包文件信息列表

		FKPFileInfo*	mpCurFileInfo;	//当前打开文件的文件信息
		DWORD			mnRemainLen;	//当前文件未读取长度
		DWORD			mnCurPos;		//当前文件读写位置

	public:
		FKPFile();
		virtual ~FKPFile();

		//--------------------------------
		// 以下为 PackObject 类的继承函数
		//--------------------------------

		//创建打包文件
		void CreatePack( LPCSTR szPackName, LPCSTR szBasePath, StringArray* pFileNames, int nSubLevel );

		//打开打包文件
		void OpenPack( LPCSTR szPackName );

		//在打包文件内增加文件
		void AddToPack( StringArray* pFileNames, int nSubLevel );

		//从打包文件内删除文件
		void DelFromPack( StringArray* pFileNames );

		//获取包内文件列表
		void GetFileListInPack( FileSelectList* pFileList );

		//关闭打包文件
		bool ClosePack();

		//--------------------------------
		// 以下为 FileObject 类的继承函数
		//--------------------------------

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

		//根据文件名生成一个唯一数字 ID （经过测试，重复几率非常微小，但不敢保证绝对不重复。）
		unsigned int FileNameToID( const char* szFileName );

	protected:
		//生成包内一级目录下的文件列表
		void _MakeFileListOfDir( FKPDirInfo* pDirInfo, FileSelectList* pFileList );

		//获取指定路径下的文件信息列表
		void _GetFileInfoList( FKPDirInfo* pDirInfo, int nSubLevel );

		//根据名称哈希码排序目录信息
		void _SortDirInfoByName( FKPDirInfo* pDirInfo, UINT nNumInfos );

		//根据名称哈希码排序文件信息
		void _SortFileInfoByName( FKPFileInfo* pFileInfo, UINT nNumInfos );

		//按照列表信息压缩文件
		void _CompressFilesInDir( FKPDirInfo* pDirInfo );

		//压缩指定文件
		void _CompressFile( FKPFileInfo* pFileInfo );
		
		//进行压缩编码核心
		unsigned int _DoCompress( unsigned char* in, unsigned in_len, unsigned char* out, unsigned* out_len );

		//解压缩核心
		int _Decompress( void* in, unsigned in_len, void* out );

		//压缩数据
		int _Compress( void* in, unsigned in_len, void* out );
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
