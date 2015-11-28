/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	ASM_3DNOW_Matrix4
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	数学库 3DNOW 4x4 矩阵 汇编计算函数
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../../include/Math/MathInsideHead.h"
#include "../../include/Math/amd3dx.h"
//#include "../../include/CommonBase/NoMemoryMarco.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	void FKFastcall _3DNOW_Mat4TransVec4( Vector4* pOutV4, const Matrix4* pMat4, const Vector4* pInV4 )
	{
		__asm
		{
			mov			edx, [pOutV4];	//目标四维矢量指针
			mov			eax, [pMat4];	// 4x4 变换矩阵指针
			mov			ebx, [pInV4];	//来源四维矢量指针

			femms;						//清除 MMX 寄存器状态
			align		16;				// 16 位对齐

			movq		mm0, [ebx];		//mm0 = ( x, y )
			movq		mm1, [ebx+8];	//mm1 = ( z, w )
			movq		mm2, mm0;		//mm2 = ( x, y )
			movq		mm3, [eax];		//mm3 = ( _11, _12 )
			punpckldq	mm0, mm0;		//mm0 = ( x, x )
			movq		mm4, [eax+16];	//mm4 = ( _21, _22 )
			pfmul		mm3, mm0;		//mm3 = ( x*_11, x*_12 )
			punpckhdq	mm2, mm2;		//mm2 = ( y, y )
			pfmul		mm4, mm2;		//mm4 = ( y*_21, y *_22 )
			movq		mm5, [eax+8];	//mm5 = ( _13, _14 )
			movq		mm7, [eax+24];	//mm7 = ( _23, _24 )
			movq		mm6, mm1;		//mm6 = ( z, w )
			pfmul		mm5, mm0;		//mm5 = ( x*_13, y*_14 )
			movq		mm0, [eax+32];	//mm0 = ( _31, _32 )
			punpckldq	mm1, mm1;		//mm1 = ( z, z )
			pfmul		mm7, mm2;		//mm7 = ( y*_23, y*_24 )
			movq		mm2, [eax+40];	//mm2 = ( _33, _34 )
			pfmul		mm0, mm1;		//mm0 = ( z*_31, z*_32 )
			pfadd		mm3, mm4;		//mm3 = ( z*_11 + y*_21, x*_12 + y*22 )
			movq		mm4, [eax+48];	//mm4 = ( _41, _42 )
			pfmul		mm2, mm1;		//mm2 = ( z*_33, z*_34 );
			pfadd		mm5, mm7;		//mm5 = ( x*_13 + y*_23, x*_14 + y*_24 )
			movq		mm1, [eax+56];	//mm1 = ( _43, _44 )
			punpckhdq	mm6, mm6;		//mm6 = ( w, w )
			pfadd		mm3, mm0;		//mm3 = ( x*_11 + y*_21 + z*_31, x*_12 + y*_22 + z*32 )
			pfmul		mm4, mm6;		//mm4 = ( w*_41, w*_42 )
			pfmul		mm1, mm6;		//mm1 = ( w*_43, w*_44 )
			pfadd		mm5, mm2;		//mm5 = ( x*_13 + y*23 + z*_33, x*14 + y*_24 + z*_34 )
			pfadd		mm3, mm4;		//mm3 = ( x*_11 + y*_21 + z*_31 + w*_41, x*_12 + y*_22 + z*_32 + w*_42 )
			movq		[edx], mm3;		//保存 pOutV4->x, pOutV4->y 计算结果
			pfadd		mm5, mm1;		//mm5 = ( x*_13 + y*_23 + z*_33 + w*_43, x*_14 + y*_24 + z*_34 + w*_44 )
			movq		[edx+8], mm5;	//保存 pOutV4->z, pOutV4->w 计算结果

			femms;						//清除 MMX 寄存器状态
		}
	}
	//--------------------------------------------------------------------------------------------------------------
}
#include "../../include/CommonBase/MemoryMarco.h"
//==================================================================================================================
