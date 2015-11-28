/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	ASM_3DNOW_Vector3
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	数学库 3DNOW 三维矢量 汇编计算函数
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
	//三维矢量相除
	void FKFastcall _3DNOW_Vec3Div( Vector3* pOut, const Vector3* pArg1, const Vector3* pArg2 )
	{
		__asm
		{
			mov		eax, pArg1;
			mov		ebx, pArg2;

			movq	mm0, [eax];			//mm0 = ( x1, y1 )
			movd	mm1, [eax + 8];		//mm1 = ( z1, N )
			movq	mm2, [ebx];			//mm2 = ( x2, y2 )
			movd	mm3, [ebx + 8];		//mm3 = ( z2, N )

			pfrcp	mm2, mm2;			//mm2 = ( 1.0f / x2, 1.0f / y2 )
			pfrcp	mm3, mm3;			//mm3 = ( 1.0f / z2, N )
			pfmul	mm0, mm2;			//mm0 = ( x1 / x2, y1 / y2 )
			pfmul	mm1, mm3;			//mm0 = ( z1 / z2, N )

			mov		eax, pOut;
			movq	[eax], mm0;
			movd	[eax + 8], mm1;

			femms;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//三维矢量除以标量
	void FKFastcall _3DNOW_Vec3DivScalar( Vector3* pOut, const Vector3* pOrg, const float fScalar )
	{
		if( fScalar != 0 )
		{
			__asm
			{
				mov			eax, pOrg;
				mov			ebx, pOut;

				movq		mm0, [eax];			//mm0 = ( x1, y1 )
				movd		mm1, [eax + 8];		//mm1 = ( z1, N )

				movd		mm2, fScalar;		//mm2 = ( fScalar, N );
				punpckldq	mm2, mm2;			//mm2 = ( fScalar, fScalar );
				pfrcp		mm2, mm2;			//mm2 = ( 1.0f/fScalar, 1.0f/fScalar )

				pfmul		mm0, mm2;			//mm0 = ( x1/fScalar, y1/fScalar )
				pfmul		mm1, mm2;			//mm0 = ( z1/fScalar, N )

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
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//三维矢量单位长度
	float FKFastcall _3DNOW_Vec3UnitLength( const Vector3* pIn )
	{
		float UnitLength;

		__asm
		{
			mov			eax, pIn;

			movq		mm0, [eax];			//mm0 = ( x, y )
			movd		mm1, [eax + 8];		//mm1 = ( z, N )

			pfmul		mm0, mm0;			//mm0 = ( x*x, y*y )
			pfacc		mm0, mm0;			//mm0 = ( x*x + y*y, N )

			pfmul		mm1, mm1;			//mm1 = ( z*z, N )
			pfadd		mm0, mm1;			//mm0 = ( x*x + y*y + z*z, N )

			pfrsqrt		mm1, mm0;			//mm1 = ( 1.0f / Sqrt( x*x + y*y + z*z ), N )
			pfmul		mm0, mm1;			//mm0 = ( Sqrt( x*x + y*y + z*z ), N )

			movd		UnitLength, mm0;	//Length = mm0;

			femms;
		}

		return UnitLength;
	}
	//--------------------------------------------------------------------------------------------------------------
	//单位化三维矢量
	void FKFastcall _3DNOW_Vec3Normalize( Vector3* pOut, const Vector3* pIn )
	{
		if( pIn->x != 0.0f || pIn->y != 0.0f || pIn->z != 0.0f )
		{
			__asm
			{
				mov			eax, pIn;
				mov			ebx, pOut;

				movq		mm0, [eax];			//mm0 = ( x, y )
				movd		mm1, [eax + 8];		//mm1 = ( z, N )
				movq		mm2, mm0;			//mm2 = ( x, y )
				movq		mm3, mm1;			//mm3 = ( z, N )

				pfmul		mm0, mm0;			//mm0 = ( x*x, y*y )
				pfacc		mm0, mm0;			//mm0 = ( x*x + y*y, N )

				pfmul		mm1, mm1;			//mm1 = ( z*z, N )
				pfadd		mm0, mm1;			//mm0 = ( x*x + y*y + z*z, N )

				pfrsqrt		mm0, mm0;			//mm0 = ( RUnitLen = 1.0f / Sqrt( x*x + y*y + z*z ), N )
				punpckldq	mm0, mm0;			//mm0 = ( RUnitLen, RUnitLen );

				pfmul		mm2, mm0;			//mm2 = ( x / UnitLen, y / UnitLen );
				pfmul		mm3, mm0;			//mm3 = ( z / UnitLen, N );

				movq		[ebx], mm2;
				movd		[ebx + 8], mm3;

				femms;
			}
		}
		else
		{
			pOut->x = 0.0f;
			pOut->y = 0.0f;
			pOut->z = 0.0f;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//快速单位化三维矢量
	void FKFastcall _3DNOW_Vec3NormalizeFast( Vector3* pOut, const Vector3* pIn, float fLength )
	{
		if( fLength != 0 && ( pIn->x != 0.0f || pIn->y != 0.0f || pIn->z != 0.0f ) )
		{
			__asm
			{
				mov			eax, pIn;
				mov			ebx, pOut;

				movq		mm0, [eax];			//mm0 = ( x, y )
				movd		mm1, [eax + 8];		//mm1 = ( z, N )

				movd		mm2, fLength;		//mm2 = ( UnitLen, N )
				punpckldq	mm2, mm2;			//mm2 = ( UnitLen, UnitLen )
				pfrcp		mm2, mm2;			//mm2 = ( 1.0f / UnitLen, 1.0f / UnitLen )

				pfmul		mm0, mm2;			//mm0 = ( x / UnitLen, y / UnitLen )
				pfmul		mm1, mm2;			//mm1 = ( z / UnitLen, N )

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
		}
	}
	//--------------------------------------------------------------------------------------------------------------
}
#include "../../include/CommonBase/MemoryMarco.h"
//==================================================================================================================
