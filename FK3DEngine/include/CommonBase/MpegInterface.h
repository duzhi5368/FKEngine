/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	MpegInterface
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	MPEG 解码接口
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------

	const int	MPEG_MaxFrameSize	= 1792;
	const int	MPEG_SBLimit		= 32;
	const int	MPEG_SSLimit		= 18;
	const int	MPEG_ScaleBlock		= 12;

	extern const DWORD	MPEG_BitRateTable[3][3][16];	// MPEG 声音比特率映射表
	extern const DWORD	MPEG_FrequencyTable[9];			// MPEG 声音频率映射表

	//--------------------------------------------------------------------------------------------------------------

	// MPEG 版本
	const BYTE MPEG_Version1	= 0;
	const BYTE MPEG_Version2	= 1;
	const BYTE MPEG_Version25	= 2;

	// MPEG 层数
	const BYTE MPEG_Layer1		= 0;
	const BYTE MPEG_Layer2		= 1;
	const BYTE MPEG_Layer3		= 2;

	// MPEG 帧信息
	struct MPEGFrameInfo
	{
		BYTE	bMpegVersion;	// MPEG 版本
		BYTE	bMpegLayer;		// MPEG 层数

		bool	bCopyright;		//是否享有版权
		bool	bOriginal;		//是否为原创
		bool	bCRC;			//是否存在 CRC 校验数据
		bool	bPadding;		//是否存在补充位

		DWORD	dwChannels;		//声音声道数
		DWORD	dwFrequency;	//声音频率
		DWORD	dwBitRate;		//声音比特率

		BYTE	bJointMode;		// Joint Stereo 模式
		BYTE	bEmphasis;		// Emphasis 模式

		DWORD	dwFrameSize;	//帧数据长度（包含 4 字节的帧头和 2 字节的 CRC 校验数据）

		int		nFreqIndex;		//频率索引值
		int		nBitRateIndex;	//比特率索引值
		int		nChannelMode;	//声道模式
		int		nJointStereo;	// Joint Stereo 模式解码索引
		int		nTableSelect;	//第二层解码表索引
		void*	pTableAlloc;	//第二层解码表指针
	};

	//--------------------------------------------------------------------------------------------------------------

	//初始化 MPEG 音频解码器
	void InitAudioDecoder();

	//解码 MPEG 音频帧
	int DecodeAudioFrame( MPEGFrameInfo* pFrameInfo, char* pIn, char* pOut );

	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================