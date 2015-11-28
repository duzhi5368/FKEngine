/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	ASM_SSE2_BaseMath
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	数学库 SSE2 基本 汇编计算函数
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../../include/Math/MathInsideHead.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	float FKFastcall _SSE2_Cos( float degree )  
	{
		__asm
		{
			movss	xmm0, degree;
			movss	xmm1, _ps_am_inv_sign_mask;
			movss	xmm2, _ps_am_pi_o_2;
			movss	xmm3, _ps_am_2_o_pi;
			andps	xmm0, xmm1;
			addss	xmm0, xmm2;
			mulss	xmm0, xmm3;

			pxor	xmm3, xmm3;
			movd	xmm5, _epi32_1;
			movss	xmm4, _ps_am_1;
			cvttps2dq	xmm2, xmm0;
			pand	xmm5, xmm2;
			movd	xmm1, _epi32_2;
			pcmpeqd	xmm5, xmm3;
			cvtdq2ps	xmm6, xmm2;
			pand	xmm2, xmm1;
			pslld	xmm2, (31 - 1);

			subss	xmm0, xmm6;
			movss	xmm3, _ps_sincos_p3;
			minss	xmm0, xmm4;
			subss	xmm4, xmm0;
			andps	xmm0, xmm5;
			andnps	xmm5, xmm4;
			orps	xmm0, xmm5;

			movaps	xmm1, xmm0;
			movss	xmm4, _ps_sincos_p2;
			mulss	xmm0, xmm0;
			movss	xmm5, _ps_sincos_p1;
			orps	xmm1, xmm2;
			movaps	xmm7, xmm0;
			mulss	xmm0, xmm3;
			movss	xmm6, _ps_sincos_p0;
			addss	xmm0, xmm4;
			mulss	xmm0, xmm7;
			addss	xmm0, xmm5;
			mulss	xmm0, xmm7;
			addss	xmm0, xmm6;
			mulss	xmm0, xmm1;
			movss   degree, xmm0;
		}

		return degree;
	}
	//--------------------------------------------------------------------------------------------------------------
	void FKFastcall _SSE2_SinCos( float degree, float* pSin, float* pCos )
	{
		__asm
		{
			movss	xmm0, degree;
			movaps	xmm7, xmm0;
			movss	xmm1, _ps_am_inv_sign_mask;
			movss	xmm2, _ps_am_sign_mask;
			movss	xmm3, _ps_am_2_o_pi;
			andps	xmm0, xmm1;
			andps	xmm7, xmm2;
			mulss	xmm0, xmm3;

			pxor	xmm3, xmm3;
			movd	xmm5, _epi32_1;
			movss	xmm4, _ps_am_1;

			cvttps2dq	xmm2, xmm0;
			pand	xmm5, xmm2;
			movd	xmm1, _epi32_2;
			pcmpeqd	xmm5, xmm3;
			movd	xmm3, _epi32_1;
			cvtdq2ps	xmm6, xmm2;
			paddd	xmm3, xmm2;
			pand	xmm2, xmm1;
			pand	xmm3, xmm1;
			subss	xmm0, xmm6;
			pslld	xmm2, (31 - 1);
			minss	xmm0, xmm4;

			mov		eax, pSin; // mov eax, [esp + 4 + 16]
			mov		edx, pCos; // mov edx, [esp + 4 + 16 + 4]

			subss	xmm4, xmm0;
			pslld	xmm3, (31 - 1);

			movaps	xmm6, xmm4;
			xorps	xmm2, xmm7;
			movaps	xmm7, xmm5;
			andps	xmm6, xmm7;
			andnps	xmm7, xmm0;
			andps	xmm0, xmm5;
			andnps	xmm5, xmm4;
			movss	xmm4, _ps_sincos_p3;
			orps	xmm6, xmm7;
			orps	xmm0, xmm5;
			movss	xmm5, _ps_sincos_p2;

			movaps	xmm1, xmm0;
			movaps	xmm7, xmm6;
			mulss	xmm0, xmm0;
			mulss	xmm6, xmm6;
			orps	xmm1, xmm2;
			orps	xmm7, xmm3;
			movaps	xmm2, xmm0;
			movaps	xmm3, xmm6;
			mulss	xmm0, xmm4;
			mulss	xmm6, xmm4;
			movss	xmm4, _ps_sincos_p1;
			addss	xmm0, xmm5;
			addss	xmm6, xmm5;
			movss	xmm5, _ps_sincos_p0;
			mulss	xmm0, xmm2;
			mulss	xmm6, xmm3;
			addss	xmm0, xmm4;
			addss	xmm6, xmm4;
			mulss	xmm0, xmm2;
			mulss	xmm6, xmm3;
			addss	xmm0, xmm5;
			addss	xmm6, xmm5;
			mulss	xmm0, xmm1;
			mulss	xmm6, xmm7;

			movss	[eax], xmm0;
			movss	[edx], xmm6;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
