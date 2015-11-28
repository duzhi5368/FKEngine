/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Layer1
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
	void I_step_one(unsigned int balloc[], unsigned int scale_index[2][MPEG_SBLimit],MPEGFrameInfo* fr)
	{
		unsigned int *ba=balloc;
		unsigned int *sca = (unsigned int *) scale_index;

		if( fr->dwChannels )
		{
			int i;
			int jsbound = fr->nJointStereo;
			for (i=0;i<jsbound;i++) { 
				*ba++ = GetBit(4);
				*ba++ = GetBit(4);
			}
			for (i=jsbound;i<MPEG_SBLimit;i++)
				*ba++ = GetBit(4);

			ba = balloc;

			for (i=0;i<jsbound;i++) {
				if ((*ba++))
					*sca++ = GetBit(6);
				if ((*ba++))
					*sca++ = GetBit(6);
			}
			for (i=jsbound;i<MPEG_SBLimit;i++)
				if ((*ba++)) {
					*sca++ =  GetBit(6);
					*sca++ =  GetBit(6);
				}
		}
		else
		{
			int i;
			for (i=0;i<MPEG_SBLimit;i++)
				*ba++ = GetBit(4);
			ba = balloc;
			for (i=0;i<MPEG_SBLimit;i++)
				if ((*ba++))
					*sca++ = GetBit(6);
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	void I_step_two(float fraction[2][MPEG_SBLimit],unsigned int balloc[2*MPEG_SBLimit],
		unsigned int scale_index[2][MPEG_SBLimit],MPEGFrameInfo* fr)
	{
		int i,n;
		int smpb[2*MPEG_SBLimit]; /* values: 0-65535 */
		int *sample;
		register unsigned int *ba;
		register unsigned int *sca = (unsigned int *) scale_index;

		if(fr->dwChannels) {
			int jsbound = fr->nJointStereo;
			register float *f0 = fraction[0];
			register float *f1 = fraction[1];
			ba = balloc;
			for (sample=smpb,i=0;i<jsbound;i++)  {
				if ((n = *ba++))
					*sample++ = GetBit(n+1);
				if ((n = *ba++))
					*sample++ = GetBit(n+1);
			}
			for (i=jsbound;i<MPEG_SBLimit;i++) 
				if ((n = *ba++))
					*sample++ = GetBit(n+1);

			ba = balloc;
			for (sample=smpb,i=0;i<jsbound;i++) {
				if((n=*ba++))
					*f0++ = (float) ( ((-1)<<n) + (*sample++) + 1) * g_pMuls[n+1][*sca++];
				else
					*f0++ = 0.0f;
				if((n=*ba++))
					*f1++ = (float) ( ((-1)<<n) + (*sample++) + 1) * g_pMuls[n+1][*sca++];
				else
					*f1++ = 0.0f;
			}
			for (i=jsbound;i<MPEG_SBLimit;i++) {
				if ((n=*ba++)) {
					float samp = (float)( ((-1)<<n) + (*sample++) + 1);
					*f0++ = samp * g_pMuls[n+1][*sca++];
					*f1++ = samp * g_pMuls[n+1][*sca++];
				}
				else
					*f0++ = *f1++ = 0.0f;
			}
		}
		else {
			register float *f0 = fraction[0];
			ba = balloc;
			for (sample=smpb,i=0;i<MPEG_SBLimit;i++)
				if ((n = *ba++))
					*sample++ = GetBit(n+1);
			ba = balloc;
			for (sample=smpb,i=0;i<MPEG_SBLimit;i++) {
				if((n=*ba++))
					*f0++ = (float) ( ((-1)<<n) + (*sample++) + 1) * g_pMuls[n+1][*sca++];
				else
					*f0++ = 0.0f;
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	int DecodeLayer1( MPEGFrameInfo* pFrameInfo, unsigned char* pOut, int* pOutSize )
	{
		int clip=0;
		int i,stereo = pFrameInfo->dwChannels;
		unsigned int balloc[2*MPEG_SBLimit];
		unsigned int scale_index[2][MPEG_SBLimit];
		float pFrameInfoaction[2][MPEG_SBLimit];

		pFrameInfo->nJointStereo = ( pFrameInfo->nChannelMode == 1 ) ? ( pFrameInfo->bJointMode << 2 ) + 4 : 32;

		I_step_one(balloc,scale_index,pFrameInfo);

		for (i=0;i<MPEG_ScaleBlock;i++)
		{
			I_step_two(pFrameInfoaction,balloc,scale_index,pFrameInfo);

			if( stereo == 1 ) 
			{
				clip += Synth_1to1_Mono( (float*)pFrameInfoaction[0],pOut,pOutSize);
			}
			else
			{
				int p1 = *pOutSize;
				clip += Synth_1to1( (float*)pFrameInfoaction[0],0,pOut,&p1);
				clip += Synth_1to1( (float*)pFrameInfoaction[1],1,pOut,pOutSize);
			}
		}

		return clip;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
