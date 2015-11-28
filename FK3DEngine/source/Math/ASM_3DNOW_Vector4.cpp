/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	ASM_3DNOW_Vector4
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	数学库 3DNOW 四维矢量 汇编计算函数
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
	//四维矢量相除
	void FKFastcall _3DNOW_Vec4Div( Vector4* pOut, const Vector4* pArg1, const Vector4* pArg2 )
	{
		__asm
		{
			mov		eax, pArg1;
			mov		ebx, pArg2;

			movq	mm0, [eax];			//mm0 = ( x1, y1 )
			movq	mm1, [eax + 8];		//mm1 = ( z1, w1 )
			movq	mm2, [ebx];			//mm2 = ( x2, y2 )
			movq	mm3, [ebx + 8];		//mm3 = ( z2, w2 )

			pfrcp	mm2, mm2;			//mm2 = ( 1.0f / x2, 1.0f / y2 )
			pfrcp	mm3, mm3;			//mm3 = ( 1.0f / z2, 1.0f / w2 )
			pfmul	mm0, mm2;			//mm0 = ( x1 / x2, y1 / y2 )
			pfmul	mm1, mm3;			//mm0 = ( z1 / z2, w1 / w2 )

			mov		eax, pOut;
			movq	[eax], mm0;
			movd	[eax + 8], mm1;

			femms;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//四维矢量除以标量
	void FKFastcall _3DNOW_Vec4DivScalar( Vector4* pOut, const Vector4* pOrg, const float fScalar )
	{
		if( fScalar != 0.0f )
		{
			__asm
			{
				mov			eax, pOrg;
				mov			ebx, pOut;

				movq		mm0, [eax];			//mm0 = ( x1, y1 )
				movq		mm1, [eax + 8];		//mm1 = ( z1, w1 )

				movd		mm3, fScalar;		//mm3 = ( fScalar, N );
				punpckldq	mm3, mm3;			//mm3 = ( fScalar, fScalar );
				pfrcp		mm3, mm3;			//mm3 = ( 1.0f/fScalar, 1.0f/fScalar )

				pfmul		mm0, mm3;			//mm0 = ( x1/fScalar, y1/fScalar )
				pfmul		mm1, mm3;			//mm0 = ( z1/fScalar, w1/fScalar )

				movq		[ebx], mm0;
				movd		[ebx + 8], mm1;

				femms;
			}
		}
		else
		{
			pOut->x = 0.0f;
			pOut->y = 0.0f;
			pOut->z = 0.0f;
			pOut->w = 0.0f;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//四维矢量单位长度
	float FKFastcall _3DNOW_Vec4UnitLength( const Vector4* pIn )
	{
		float UnitLength;

		__asm
		{
			mov			eax, pIn;

			movq		mm0, [eax];			//mm0 = ( x, y )
			movq		mm1, [eax + 8];		//mm1 = ( z, w )

			pfmul		mm0, mm0;			//mm0 = ( x*x, y*y )
			pfmul		mm1, mm1;			//mm1 = ( z*z, w*w )
			pfacc		mm0, mm1;			//mm0 = ( x*x + y*y, z*z + w*w )
			pfacc		mm0, mm0;			//mm0 = ( x*x + y*y + z*z + w*w, N )

			pfrsqrt		mm1, mm0;			//mm1 = ( 1.0f / Sqrt( x*x + y*y + z*z + w*w ), N )
			pfmul		mm0, mm1;			//mm0 = ( Sqrt( x*x + y*y + z*z + w*w ), N )

			movd		UnitLength, mm0;	//Length = mm0;

			femms;
		}

		return UnitLength;
	}
	//--------------------------------------------------------------------------------------------------------------
	//单位化四维矢量
	void FKFastcall _3DNOW_Vec4Normalize( Vector4* pOut, const Vector4* pIn )
	{
		if( pIn->x != 0.0f || pIn->y != 0.0f || pIn->z != 0.0f || pIn->w != 0.0f )
		{
			__asm
			{
				mov			eax, pIn;
				mov			ebx, pOut;

				movq		mm0, [eax];			//mm0 = ( x, y )
				movq		mm1, [eax + 8];		//mm1 = ( z, w )
				movq		mm2, mm0;			//mm2 = ( x, y )
				movq		mm3, mm1;			//mm3 = ( z, w )

				pfmul		mm0, mm0;			//mm0 = ( x*x, y*y )
				pfmul		mm1, mm1;			//mm1 = ( z*z, w*w )
				pfacc		mm0, mm1;			//mm0 = ( x*x + y*y, z*z + w*w )
				pfacc		mm0, mm0;			//mm0 = ( x*x + y*y + z*z + w*w, N )

				pfrsqrt		mm0, mm0;			//mm0 = ( RUnitLen = 1.0f / Sqrt( x*x + y*y + z*z + w*w ), N )
				punpckldq	mm0, mm0;			//mm0 = ( RUnitLen, RUnitLen );

				pfmul		mm2, mm0;			//mm2 = ( x / UnitLen, y / UnitLen );
				pfmul		mm3, mm0;			//mm3 = ( z / UnitLen, w / UnitLen );

				movq		[ebx], mm2;
				movq		[ebx + 8], mm3;

				femms;
			}
		}
		else
		{
			pOut->x = 0.0f;
			pOut->y = 0.0f;
			pOut->z = 0.0f;
			pOut->w = 0.0f;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//快速单位化四维矢量
	void FKFastcall _3DNOW_Vec4NormalizeFast( Vector4* pOut, const Vector4* pIn, float fLength )
	{
		if( fLength != 0.0f && ( pIn->x != 0.0f || pIn->y != 0.0f || pIn->z != 0.0f || pIn->w != 0.0f ) )
		{
			__asm
			{
				mov			eax, pIn;
				mov			ebx, pOut;

				movq		mm0, [eax];			//mm0 = ( x, y )
				movq		mm1, [eax + 8];		//mm1 = ( z, w )

				movd		mm3, fLength;		//mm3 = ( UnitLen, N )
				punpckldq	mm3, mm3;			//mm0 = ( UnitLen, UnitLen )
				pfrcp		mm3, mm3;			//mm3 = ( 1.0f / UnitLen, 1.0f / UnitLen )

				pfmul		mm0, mm3;			//mm0 = ( x / UnitLen, y / UnitLen )
				pfmul		mm1, mm3;			//mm1 = ( z / UnitLen, w / UnitLen )

				movq		[ebx], mm0;
				movq		[ebx + 8], mm1;

				femms;
			}
		}
		else
		{
			pOut->x = 0.0f;
			pOut->y = 0.0f;
			pOut->z = 0.0f;
			pOut->w = 0.0f;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
}
#include "../../include/CommonBase/MemoryMarco.h"
//==================================================================================================================
