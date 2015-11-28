/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	MpegCommon
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

	// MPEG 声音比特率映射表
	static const DWORD MPEG_BitRateTable[3][3][16] =
	{
		// MPEG-1
		{
			// Layer1
			{ 0, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448, 0 },
				// Layer2
			{ 0, 32, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 384, 0 },
			// Layer3
			{ 0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 0 },
		},
		// MPEG-2
		{
			// Layer1
			{ 0, 32, 48, 56, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 256, 0 },
				// Layer2
			{ 0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160, 0 },
			// Layer3
			{ 0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160, 0 },
		},
		// MPEG-2.5f
		{
			// Layer1 （不可用）
			{ 0, 32, 48, 56, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 256, 0 },
				// Layer2 （不可用）
			{ 0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160, 0 },
			// Layer3
			{ 0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160, 0 },
		},
	};

	// MPEG 声音频率映射表
	static const DWORD MPEG_FrequencyTable[9] =
	{
		44100, 48000, 32000,	// MPEG-1
		22050, 24000, 16000,	// MPEG-2
		11025, 12000, 8000		// MPEG-2.5
	};

	//--------------------------------------------------------------------------------------------------------------

	MPEGDecodeData		g_DecodeData;		// MPEG 音频解码数据

	int					g_nBitIndex;
	unsigned char*		g_pDataPtr;

	float				g_pMuls[27][64];

	//--------------------------------------------------------------------------------------------------------------
	//初始化 MPEG 音频解码器
	void InitAudioDecoder()
	{
		memset( &g_DecodeData, 0, sizeof(MPEGDecodeData) );

		g_DecodeData.nPrevSize = -1;
		g_DecodeData.nSynthBo = 1;

		BuildTables( 32767 );
		InitLayer2();
		InitLayer3( MPEG_SBLimit );
	}
	//--------------------------------------------------------------------------------------------------------------
	//解码 MPEG 音频帧
	int DecodeAudioFrame( MPEGFrameInfo* pFrameInfo, char* pIn, char* pOut )
	{
		//保留上一帧解码数据
		g_pDataPtr = g_DecodeData.pBuffer[ g_DecodeData.nBuffer ] + 512;
		g_DecodeData.nBuffer = ( g_DecodeData.nBuffer + 1 ) & 0x1;

		g_nBitIndex = 0;

		memcpy( g_pDataPtr, pIn, pFrameInfo->dwFrameSize );

		//如果存在 CRC 校验数据则略过它
		if( pFrameInfo->bCRC )
			GetBit(16);

		int nOutSize = 0;
		switch( pFrameInfo->bMpegLayer )
		{
		case MPEG_Layer1:
			DecodeLayer1( pFrameInfo,(unsigned char*)pOut, &nOutSize );
			break;
		case MPEG_Layer2:
			DecodeLayer2( pFrameInfo,(unsigned char*)pOut, &nOutSize );
			break;
		case MPEG_Layer3:
			DecodeLayer3( pFrameInfo,(unsigned char*)pOut, &nOutSize );
			break;
		}

		g_DecodeData.nPrevSize = pFrameInfo->dwFrameSize;

		return nOutSize;
	}
	//--------------------------------------------------------------------------------------------------------------
	unsigned int FKFastcall GetBitOne()
	{
		unsigned char nVal;
		nVal = *g_pDataPtr << g_nBitIndex;

		++g_nBitIndex;
		g_pDataPtr += g_nBitIndex >> 3;
		g_nBitIndex &= 7;

		return nVal >> 7;
	}
	//--------------------------------------------------------------------------------------------------------------
	unsigned int FKFastcall GetBit( int nNumBits )
	{
		unsigned long nVal;

		if( nNumBits == 0 )
			return 0;

		nVal = g_pDataPtr[0];
		nVal <<= 8;
		nVal |= g_pDataPtr[1];
		nVal <<= 8;
		nVal |= g_pDataPtr[2];
		nVal <<= g_nBitIndex;
		nVal &= 0xffffff;

		g_nBitIndex += nNumBits;
		nVal >>= 24 - nNumBits;
		g_pDataPtr += g_nBitIndex >> 3;
		g_nBitIndex &= 7;

		return nVal;
	}
	//--------------------------------------------------------------------------------------------------------------
	unsigned int FKFastcall GetBitFast( int nNumBits )
	{
		unsigned long nVal;

		nVal = g_pDataPtr[0];
		nVal <<= 8;	
		nVal |= g_pDataPtr[1];
		nVal <<= g_nBitIndex;
		nVal &= 0xffff;

		g_nBitIndex += nNumBits;
		nVal >>= 16 - nNumBits;
		g_pDataPtr += g_nBitIndex >> 3;
		g_nBitIndex &= 7;

		return nVal;
	}
	//--------------------------------------------------------------------------------------------------------------
	void FKFastcall SetPointer( long nBackstep )
	{
		unsigned char* pPrevFrame = g_DecodeData.pBuffer[ g_DecodeData.nBuffer ] + 512;
		g_pDataPtr -= nBackstep;

		if( nBackstep > 0 )
			memcpy( g_pDataPtr, pPrevFrame + g_DecodeData.nPrevSize - nBackstep, nBackstep );

		g_nBitIndex = 0;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
