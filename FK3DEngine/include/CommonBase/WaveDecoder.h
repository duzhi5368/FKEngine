/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	WaveDecoder
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	Wave 文件解码类
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

	// WAV 文件头
	struct WaveHeader
	{
		char	szRIFF[4];		// "RIFF" 标志
		LONG	dwFileLen;		//文件长度
		char	szWAVE[4];		// "WAVE" 标志
		char	szFMT[4];		// "fmt " 标志（注意此字符串结尾有一个空格）

		int		nReserved;		//保留四字节

		short	wFormatTag;		//格式标记
		WORD	wChannels;		//声道数
		DWORD	dwSamplePerSec;	//采样率
		DWORD	dwBytesPerSec;	//数据传输速率
		WORD	wBlockAlign;	//数据块对齐值
		WORD	wBitsPerSample;	//每样本数据位数

		char	szDATA[4];		// "data" 标志
		DWORD	dwDataLen;		//数据段长度（单位：位）
	};

	#pragma pack ( pop )

	//--------------------------------------------------------------------------------------------------------------
	// Wave 文件解码类
	class FK_DLL WaveDecoder : public SoundDecoder
	{
	public:
		WaveDecoder();
		~WaveDecoder();

		//打开声音文件
		SoundFormat* OpenSoundFile( FileObject* pFile, LPCSTR szFileName );

		//关闭声音文件
		void CloseSoundFile();

		//重置声音读取位置到起点
		void ResetReadPos();

		//读取声音数据到指定缓存
		void ReadSoundData( void* pBuf, DWORD dwLen );
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
