/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	BaseMath
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../../include/Math/MathInsideHead.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	// 数学库配置函数
	//--------------------------------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------------------------------
	//初始化数学库
	bool InitializeMathLibrary( const CPUInfo* pCPUInfo )
	{
		//如果传入 CPU 指令集支持信息
		if( pCPUInfo != NULL )
		{
			//根据不同的 CPU 指令集支持情况设置函数指针
			if( pCPUInfo->_Ext._3DNOW_InstructionExtensions )
			{
				Sqrt	= _3DNOW_Sqrt;
				RSqrt	= _3DNOW_RSqrt;

				Vec2Div				= _3DNOW_Vec2Div;
				Vec2DivScalar		= _3DNOW_Vec2DivScalar;

				Vec2Normalize		= _3DNOW_Vec2Normalize;
				Vec2NormalizeFast	= _3DNOW_Vec2NormalizeFast;

				Vec3Div				= _3DNOW_Vec3Div;
				Vec3DivScalar		= _3DNOW_Vec3DivScalar;

				Vec3Normalize		= _3DNOW_Vec3Normalize;
				Vec3NormalizeFast	= _3DNOW_Vec3NormalizeFast;

				Vec4Div				= _3DNOW_Vec4Div;
				Vec4DivScalar		= _3DNOW_Vec4DivScalar;

				Vec4Normalize		= _3DNOW_Vec4Normalize;
				Vec4NormalizeFast	= _3DNOW_Vec4NormalizeFast;

				Mat4TransVec4		= _3DNOW_Mat4TransVec4;
			}
			else if( pCPUInfo->_Ext.SSE_StreamingSIMD_Extensions )
			{
				Sqrt	= _SSE_Sqrt;
				RSqrt	= _SSE_RSqrt;
			}

			if( pCPUInfo->_Ext.SSE2_StreamingSIMD2_Extensions )
			{
				Cos		= _SSE2_Cos;
				SinCos	= _SSE2_SinCos;
			}
			else if( pCPUInfo->_Ext.SSE_StreamingSIMD_Extensions )
			{
				Cos		= _SSE_Cos;
				SinCos	= _SSE_SinCos;
			}
		}

		return true;
	}
	//--------------------------------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------------------------------
	// 基本数学运算函数指针
	//--------------------------------------------------------------------------------------------------------------

	float	( FKFastcall *Sin )		( float radian )							= _Def_Sin;		//计算正弦
	float	( FKFastcall *Cos )		( float radian )							= _Def_Cos;		//计算余弦
	void	( FKFastcall *SinCos )	( float radian, float* pSin, float* pCos )	= _Def_SinCos;	//计算正弦和余弦
	float	( FKFastcall *ASin )	( float sin )								= _Def_ASin;	//根据正弦计算度数
	float	( FKFastcall *ACos )	( float cos )								= _Def_ACos;	//根据余弦计算度数
	float	( FKFastcall *Tan )		( float radian )							= _Def_Tan;		//计算正切
	float	( FKFastcall *Cot )		( float radian )							= _Def_Cot;		//计算余切
	float	( FKFastcall *ATan )	( float x, float y )						= _Def_ATan;	//根据正切计算度数
	float	( FKFastcall *Abs )		( float val )								= _Def_Abs;		//求绝对值
	float	( FKFastcall *Sqrt )	( float val )								= _Def_Sqrt;	//计算平方根
	float	( FKFastcall *RSqrt )	( float val )								= _Def_RSqrt;	//计算平方根的倒数

	//--------------------------------------------------------------------------------------------------------------
	// 默认数学运算函数
	//--------------------------------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------------------------------
	//计算正弦
	float FKFastcall _Def_Sin( float radian )
	{
		__asm
		{
			fld radian;
			fsin;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//计算余弦
	float FKFastcall _Def_Cos( float radian )
	{
		__asm
		{
			fld radian;
			fcos;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//计算正弦和余弦
	void FKFastcall _Def_SinCos( float radian, float* pSin, float* pCos )
	{
		__asm
		{
			fld			radian;
			fsincos;

			mov			eax, pCos;
			mov			ebx, pSin;

			fstp		[eax];
			fstp		[ebx];
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//根据正弦计算度数
	float FKFastcall _Def_ASin( float sin )
	{
		return asinf( sin );
	}
	//--------------------------------------------------------------------------------------------------------------
	//根据余弦计算度数
	float FKFastcall _Def_ACos( float cos )
	{
		return acosf( cos );
	}
	//--------------------------------------------------------------------------------------------------------------
	//计算正切
	float FKFastcall _Def_Tan( float radian )
	{
		__asm
		{
			fld		radian;			// st0 = radian
			fptan;					// y / x = tan( radian ) st0 = x, st1 = y
			fdivp	st(1), st(0);	// st0 = y / x
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//计算余切
	float FKFastcall _Def_Cot( float radian )
	{
		__asm
		{
			fld		radian;			// st0 = radian
			fptan;					// y / x = tan( radian ) st0 = x, st1 = y
			fdiv	st(0), st(1);	// st0 = x / y
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//根据正切计算度数
	float FKFastcall _Def_ATan( float x, float y )
	{
		__asm
		{
			fld		x;		// st0 = 对边
			fld		y;		// st0 = 邻边, st1 = 对边
			fpatan;			// st0 = 度数
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//求绝对值
	float FKFastcall _Def_Abs( float val )
	{
		__asm
		{
			fld val;
			fabs;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//计算平方根
	float FKFastcall _Def_Sqrt( float val )
	{
		__asm
		{
			fld val;
			fsqrt;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//计算平方根的倒数
	float FKFastcall _Def_RSqrt( float val )
	{
		__asm
		{
			fld		val;			// st0 = val
			fsqrt;					// st0 = sqrt( val )
			fld1;					// st0 = 1.0, st1 = sqrt( val )
			fdiv	st(0), st(1);	// st0 = 1.0 / sqrt( val )
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//检测是否为 2 次幂无符号整数
	FKForceInline bool IsPow2( UINT value )
	{
		//检测宽度
		bool bTrueBit = false;

		while( value != 0 )
		{
			if( value & 1 )
			{
				if( bTrueBit )
					return false;
				else
					bTrueBit = true;
			}

			value >>= 1;
		}

		return true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//将无符号整数转换为最接近但比它小的 2 次幂无符号整数
	FKForceInline UINT FloorToPow2( UINT value )
	{
		UINT nNearest = 0x80000000;

		while( nNearest != 0 )
		{
			if( nNearest <= value )
				return nNearest;

			nNearest >>= 1;
		}

		return nNearest;
	}
	//--------------------------------------------------------------------------------------------------------------
	//将无符号整数转换为最接近但比它大的 2 次幂无符号整数
	FKForceInline UINT CeilToPow2( UINT value )
	{
		UINT nNearest = 1;

		while( nNearest != 0x80000000 )
		{
			if( nNearest >= value )
				return nNearest;

			nNearest <<= 1;
		}

		return nNearest;
	}
	//--------------------------------------------------------------------------------------------------------------
	//有符号整数除法
	FKForceInline void Div( const int value, const int divisor, int& quotient, int& remainder )
	{
		__asm
		{
			mov		eax, value;
			cdq;
			idiv	divisor;
			mov		ebx, quotient;
			mov		ecx, remainder;
			mov		[ebx], eax;
			mov		[ecx], edx;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//无符号整数除法
	FKForceInline void Div( const unsigned int value, const unsigned int divisor,
		unsigned int& quotient, unsigned int& remainder )
	{
		__asm
		{
			mov		eax, value;
			xor		edx, edx;
			div		divisor;
			mov		ebx, quotient;
			mov		ecx, remainder;
			mov		[ebx], eax;
			mov		[ecx], edx;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//将浮点数转换为最接近但比它小的整数（比 (int)floor(value) 快 5 倍左右）
	FKForceInline int FloorToInt( float value )
	{
		int CtrlwdHolder;
		int CtrlwdSetter;
		int ReturnValue;

		__asm
		{
			fld    value;
			fnstcw CtrlwdHolder;
			movzx  eax, CtrlwdHolder;
			and    eax, 0xFFFFF3FF;
			or     eax, 0x00000400;
			mov    CtrlwdSetter, eax;
			fldcw  CtrlwdSetter;
			fistp  ReturnValue;
			fldcw  CtrlwdHolder;
		}

		return ReturnValue;
	}
	//--------------------------------------------------------------------------------------------------------------
	//将浮点数转换为最接近但比它大的整数（比 (int)ceil(value) 快 5 倍左右）
	FKForceInline int CeilToInt( float value )
	{
		int CtrlwdHolder;
		int CtrlwdSetter;
		int ReturnValue;

		__asm
		{
			fld    value;
			fnstcw CtrlwdHolder;
			movzx  eax, CtrlwdHolder;
			and    eax, 0xFFFFF3FF;
			or     eax, 0x00000800;
			mov    CtrlwdSetter, eax;
			fldcw  CtrlwdSetter;
			fistp  ReturnValue;
			fldcw  CtrlwdHolder;
		}

		return ReturnValue;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
