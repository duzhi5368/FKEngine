/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	ASM_BaseMath
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	数学库 基本 汇编计算函数
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================
namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------

	//关闭不需要的警告
	#pragma warning ( disable : 4244 )	//将 'const int' 强制转换为 'float' 可能导致数据丢失
	#pragma warning ( disable : 4730 )	//混合 _m64 和浮点表达式可能导致不正确的代码

	//--------------------------------------------------------------------------------------------------------------
	// 定义使用 SSE 宏汇编需要的宏和常量
	//--------------------------------------------------------------------------------------------------------------

	#define _PS_EXTERN_CONST( Name, Val ) \
	const FKAlign16 float _ps_##Name[4] = { Val, Val, Val, Val }

	#define _PS_EXTERN_CONST_TYPE( Name, Type, Val ) \
	const FKAlign16 Type _ps_##Name[4] = { Val, Val, Val, Val }

	#define _EPI32_CONST( Name, Val ) \
	static FKAlign16 __int32 _epi32_##Name[4] = { Val, Val, Val, Val }

	#define _PS_CONST( Name, Val ) \
	static FKAlign16 float _ps_##Name[4] = { Val, Val, Val, Val }

	_PS_EXTERN_CONST( am_0, 0.0f );
	_PS_EXTERN_CONST( am_1, 1.0f );
	_PS_EXTERN_CONST( am_m1, -1.0f );
	_PS_EXTERN_CONST( am_0p5, 0.5f );
	_PS_EXTERN_CONST( am_1p5, 1.5f );
	_PS_EXTERN_CONST( am_pi, (float)FK_PI );
	_PS_EXTERN_CONST( am_pi_o_2, (float)( FK_PI / 2.0 ) );
	_PS_EXTERN_CONST( am_2_o_pi, (float)( 2.0 / FK_PI ) );
	_PS_EXTERN_CONST( am_pi_o_4, (float)( FK_PI / 4.0 ) );
	_PS_EXTERN_CONST( am_4_o_pi, (float)( 4.0 / FK_PI ) );
	_PS_EXTERN_CONST_TYPE( am_sign_mask, long, 0x80000000 );
	_PS_EXTERN_CONST_TYPE( am_inv_sign_mask, long, ~0x80000000 );
	_PS_EXTERN_CONST_TYPE( am_min_norm_pos, long, 0x00800000);
	_PS_EXTERN_CONST_TYPE( am_mant_mask, long, 0x7f800000);
	_PS_EXTERN_CONST_TYPE( am_inv_mant_mask, long, ~0x7f800000 );

	_EPI32_CONST( 1, 1 );
	_EPI32_CONST( 2, 2 );

	_PS_CONST( sincos_p0, 0.15707963267948963959e1f );
	_PS_CONST( sincos_p1, -0.64596409750621907082e0f );
	_PS_CONST( sincos_p2, 0.7969262624561800806e-1f );
	_PS_CONST( sincos_p3, -0.468175413106023168e-2f );

	static const unsigned long CONST_SINCOS_MASK[]		= { (unsigned long)0x0,  (unsigned long)~0x0 };
	static const unsigned long CONST_SINCOS_INV_MASK[]	= { (unsigned long)~0x0, (unsigned long)0x0 };

	//--------------------------------------------------------------------------------------------------------------
	// 3DNOW 计算函数
	//--------------------------------------------------------------------------------------------------------------

	extern float FKFastcall _3DNOW_Sqrt( float val );	//计算平方根
	extern float FKFastcall _3DNOW_RSqrt( float val );	//计算平方根的倒数

	//--------------------------------------------------------------------------------------------------------------
	// SSE 计算函数
	//--------------------------------------------------------------------------------------------------------------

	extern float FKFastcall _SSE_Cos( float degree );								//计算余弦
	extern void FKFastcall _SSE_SinCos( float degree, float* pSin, float* pCos );	//计算正弦和余弦
	extern float FKFastcall _SSE_Sqrt( float val );									//计算平方根
	extern float FKFastcall _SSE_RSqrt( float val );								//计算平方根的倒数

	//--------------------------------------------------------------------------------------------------------------
	// SSE2 计算函数
	//--------------------------------------------------------------------------------------------------------------

	extern float FKFastcall _SSE2_Cos( float degree );								//计算余弦
	extern void FKFastcall _SSE2_SinCos( float degree, float* pSin, float* pCos );	//计算正弦和余弦

	//--------------------------------------------------------------------------------------------------------------
}
//==================================================================================================================
