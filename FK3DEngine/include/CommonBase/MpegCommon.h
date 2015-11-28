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
*	文件说明：	MPEG 解码核心
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

#include "MpegInterface.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------

	#ifndef M_PI
	#define M_PI		3.14159265358979323846f
	#endif

	#ifndef M_SQRT2
	#define M_SQRT2		1.41421356237309504880f
	#endif

	//--------------------------------------------------------------------------------------------------------------

	// MPEG 音频解码数据结构
	struct MPEGDecodeData
	{
		int				nPrevSize;
		unsigned char	pBuffer[2][ MPEG_MaxFrameSize + 512 ];
		int				nBuffer;

		float			ppHybridBlock[2][2][ MPEG_SBLimit * MPEG_SSLimit ];
		int				pHybridBlock[2];

		float			ppSynthBuf[2][2][0x110];
		int				nSynthBo;
	};

	struct MEPGHuffman 
	{
		unsigned int	nLinBits;
		short*			pTable;
	};

	//--------------------------------------------------------------------------------------------------------------

	extern MPEGDecodeData		g_DecodeData;		// MPEG 音频解码数据

	extern int					g_nBitIndex;
	extern unsigned char*		g_pDataPtr;
	extern float				g_pMuls[27][64];

	extern float				MPEG_DecWin[512+32];
	extern float*				MPEG_CosPtr[];

	extern MEPGHuffman			MPEG_HuffmanT[];
	extern MEPGHuffman			MPEG_HuffmanTC[];

	//--------------------------------------------------------------------------------------------------------------

	extern void FKFastcall SetPointer( long nBackstep );

	unsigned int FKFastcall GetBitOne();
	unsigned int FKFastcall GetBit( int nNumBits );
	unsigned int FKFastcall GetBitFast( int nNumBits );

	extern void InitLayer3( int nSBLimit );
	extern void InitLayer2();
	extern void BuildTables( long nScal );

	extern int DecodeLayer1( MPEGFrameInfo* pFrameInfo, unsigned char* pOut, int* pOutSize );
	extern int DecodeLayer2( MPEGFrameInfo* pFrameInfo, unsigned char* pOut, int* pOutSize );
	extern int DecodeLayer3( MPEGFrameInfo* pFrameInfo, unsigned char* pOut, int* pOutSize );

	extern int Synth_1to1( float* pBand, int nChannel, unsigned char* pOut, int* pPoint );
	extern int Synth_1to1_Mono( float* pBand, unsigned char* pSamples, int* pPoint );

	extern void Dct64( float* pA, float* pB, float* pC );

	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
