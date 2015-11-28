/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	MP3Decoder
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	MPEG 解码类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------

	#pragma pack ( push )
	#pragma pack ( 1 )

	// ID3v1 信息结构
	struct ID3v1Data
	{
		char	szTAG[3];		// "TAG" 标志
		char	szTitle[30];	//歌曲名称
		char	szArtist[30];	//艺术家名称
		char	szAlbum[30];	//专辑名称
		char	szYear[4];		//出版年份
		char	szComment[30];	//注释字符串
		BYTE	bGenre;			//流派类型
	};

	// ID3v2 信息头结构
	struct ID3v2Header
	{
		char	szID3[3];		// "ID3" 标志
		BYTE	bMajorVer;		//主版本号
		BYTE	bSubVer;		//次版本号
		BYTE	bFlag;			//选项标志
		BYTE	szSize[4];		// ID3v2 中间数据长度（去除标头脚注长度，安全同步整数值）
	};

	#pragma pack ( pop )

	//--------------------------------------------------------------------------------------------------------------
	// MPEG 解码类
	class FK_DLL MP3Decoder : public SoundDecoder
	{
	public:
		//文件缓存大小 256 KB
		static const DWORD	mdwFileCacheLen = 0x40000;

	public:
		ID3v1Data		mID3v1Data;			// ID3v1 数据

		DWORD			mdwID3v1Len;		// ID3v1 数据长度
		DWORD			mdwID3v2Len;		// ID3v2 数据长度

		DWORD			mdwFramePCMSize;	//帧解压数据长度
		DWORD			mdwNumFrames;		//帧数量
		DWORD			mdwPlayTime;		//总播放时间（秒）

		MPEGFrameInfo	mFrameInfo;			// MPEG 帧信息

	protected:
		DWORD			mdwPrefectchLen;	//正在预读文件数据长度
		DWORD			mdwReadDataLen;		//已预读文件数据长度

		OVERLAPPED		mOverlapped;		//文件异步读取结构
		DWORD			mSoundDataEnd;		//文件声音数据结尾位置

		char*			mpFramePCMBuffer;	//帧解码声音数据缓存
		DWORD			mdwFramePCMLen;		//帧已解码声音数据长度

		char*			mpFileReadPtr;		//文件数据读取缓存
		char*			mpFileWorkPtr;		//文件数据解码缓存

		DWORD			mdwFrameOffset;		//解码帧数据在文件缓存中的偏移量

	public:
		MP3Decoder();
		~MP3Decoder();

		//打开声音文件
		SoundFormat* OpenSoundFile( FileObject* pFile, LPCSTR szFileName );

		//关闭声音文件
		void CloseSoundFile();

		//重置声音读取位置到起点
		void ResetReadPos();

		//读取声音数据到指定缓存
		void ReadSoundData( void* pBuf, DWORD dwLen );

	protected:
		//遍历文件所有帧
		void _CheckAllFrames();

		//解码帧信息头
		bool _DecodeFrameHeader( BYTE szFrameHeader[4], MPEGFrameInfo* pFrameInfo );

		//开始预读文件数据
		void FKFastcall _BeginPrefectch();

		//结束预读文件数据
		void FKFastcall _EndPrefectch();
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
