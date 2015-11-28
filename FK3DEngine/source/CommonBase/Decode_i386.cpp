/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Decode_i386
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
	int Synth_1to1_Mono( float* pBand, unsigned char* pSamples, int* pPoint )
	{
		short pSamplesBuf[64];
		short* pBufPtr = pSamplesBuf;
		int nPoint = 0;

		int nRet = Synth_1to1( pBand, 0, (unsigned char*)pSamplesBuf, &nPoint );
		pSamples += *pPoint;

		for( int i=0; i<32; ++i )
		{
			*(short*)pSamples = *pBufPtr;
			pSamples += 2;
			pBufPtr += 2;
		}
		*pPoint += 64;

		return nRet;
	}
	//--------------------------------------------------------------------------------------------------------------
	int Synth_1to1( float* pBand, int nChannel, unsigned char* pOut, int* pPoint )
	{
		static const int step = 2;
		short *pSamples = (short*)( pOut + *pPoint );

		float* b0;
		float (*buf)[0x110];

		int nClip = 0; 
		int bo1;

		int bo = g_DecodeData.nSynthBo;

		if( nChannel == 0 )
		{
			bo--;
			bo &= 0xf;
			buf = g_DecodeData.ppSynthBuf[0];
		}
		else
		{
			pSamples++;
			buf = g_DecodeData.ppSynthBuf[1];
		}

		if( bo & 0x1 )
		{
			b0 = buf[0];
			bo1 = bo;
			Dct64(buf[1]+((bo+1)&0xf),buf[0]+bo,pBand);
		}
		else
		{
			b0 = buf[1];
			bo1 = bo+1;
			Dct64(buf[0]+bo,buf[1]+bo+1,pBand);
		}

		g_DecodeData.nSynthBo = bo;
		{
			register int j;
			float* window = MPEG_DecWin + 16 - bo1;

			for( j=16; j!=0; --j, b0+=0x10, window+=0x20, pSamples+=step )
			{
				float sum;
				sum  = window[0x0] * b0[0x0];
				sum -= window[0x1] * b0[0x1];
				sum += window[0x2] * b0[0x2];
				sum -= window[0x3] * b0[0x3];
				sum += window[0x4] * b0[0x4];
				sum -= window[0x5] * b0[0x5];
				sum += window[0x6] * b0[0x6];
				sum -= window[0x7] * b0[0x7];
				sum += window[0x8] * b0[0x8];
				sum -= window[0x9] * b0[0x9];
				sum += window[0xA] * b0[0xA];
				sum -= window[0xB] * b0[0xB];
				sum += window[0xC] * b0[0xC];
				sum -= window[0xD] * b0[0xD];
				sum += window[0xE] * b0[0xE];
				sum -= window[0xF] * b0[0xF];

				if( sum > 32767.0f )
				{
					*pSamples = 0x7fff;
					++nClip;
				}
				else if( sum < -32768.0f )
				{
					*pSamples = -0x8000;
					++nClip;
				}
				else
				{
					*pSamples = (short)sum;
				}
			}

			{
				float sum;
				sum  = window[0x0] * b0[0x0];
				sum += window[0x2] * b0[0x2];
				sum += window[0x4] * b0[0x4];
				sum += window[0x6] * b0[0x6];
				sum += window[0x8] * b0[0x8];
				sum += window[0xA] * b0[0xA];
				sum += window[0xC] * b0[0xC];
				sum += window[0xE] * b0[0xE];

				if( sum > 32767.0f )
				{
					*pSamples = 0x7fff;
					++nClip;
				}
				else if( sum < -32768.0f )
				{
					*pSamples = -0x8000;
					++nClip;
				}
				else
				{
					*pSamples = (short)sum;
				}

				b0-=0x10,window-=0x20,pSamples+=step;
			}
			window += bo1<<1;

			for( j=15; j!=0; --j, b0-=0x10, window-=0x20, pSamples+=step )
			{
				float sum;
				sum = -window[-0x1] * b0[0x0];
				sum -= window[-0x2] * b0[0x1];
				sum -= window[-0x3] * b0[0x2];
				sum -= window[-0x4] * b0[0x3];
				sum -= window[-0x5] * b0[0x4];
				sum -= window[-0x6] * b0[0x5];
				sum -= window[-0x7] * b0[0x6];
				sum -= window[-0x8] * b0[0x7];
				sum -= window[-0x9] * b0[0x8];
				sum -= window[-0xA] * b0[0x9];
				sum -= window[-0xB] * b0[0xA];
				sum -= window[-0xC] * b0[0xB];
				sum -= window[-0xD] * b0[0xC];
				sum -= window[-0xE] * b0[0xD];
				sum -= window[-0xF] * b0[0xE];
				sum -= window[-0x0] * b0[0xF];

				if( sum > 32767.0f )
				{
					*pSamples = 0x7fff;
					++nClip;
				}
				else if( sum < -32768.0f )
				{
					*pSamples = -0x8000;
					++nClip;
				}
				else
				{
					*pSamples = (short)sum;
				}
			}
		}
		*pPoint += 128;

		return nClip;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
