/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	SoundDecoder
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	声音文件解码类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once


//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------

	//声音格式结构
	struct SoundFormat
	{
		DWORD	dwChannels;		//声音声道数
		DWORD	dwFrequency;	//声音频率
		DWORD	dwBitRate;		//声音比特率
		DWORD	dwBytesPerSec;	//播放每秒字节数
		DWORD	dwLength;		//数据长度
	};

	//--------------------------------------------------------------------------------------------------------------
	//声音文件解码类
	class FK_DLL SoundDecoder
	{
	public:
		FileObject*		mpSoundFile;	//声音文件指针

		SoundFormat		mSoundFormat;	//声音格式信息

		DWORD			mdwReadPos;		//声音数据读取位置

	public:
		SoundDecoder();
		virtual ~SoundDecoder() = 0;

		//打开声音文件
		virtual SoundFormat* OpenSoundFile( FileObject* pFile, LPCSTR szFileName ) = 0;

		//关闭声音文件
		virtual void CloseSoundFile() = 0;

		//重置声音读取位置到起点
		virtual void ResetReadPos() = 0;

		//读取声音数据到指定缓存
		virtual void ReadSoundData( void* pBuf, DWORD dwLen ) = 0;
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
