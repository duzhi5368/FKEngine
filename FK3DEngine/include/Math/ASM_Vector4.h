/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	ASM_Vector4
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	数学库 四维矢量 汇编计算函数
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================
namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	// 3DNOW 计算函数
	//--------------------------------------------------------------------------------------------------------------

	extern void FKFastcall _3DNOW_Vec4Div( Vector4* pOut, const Vector4* pArg1, const Vector4* pArg2 );		//四维矢量相除
	extern void FKFastcall _3DNOW_Vec4DivScalar( Vector4* pOut, const Vector4* pOrg, const float fScalar );	//四维矢量除以标量

	extern float FKFastcall _3DNOW_Vec4UnitLength( const Vector4* pIn );									//四维矢量单位长度
	extern void FKFastcall _3DNOW_Vec4Normalize( Vector4* pOut, const Vector4* pIn );						//单位化四维矢量
	extern void FKFastcall _3DNOW_Vec4NormalizeFast( Vector4* pOut, const Vector4* pIn, float fLength );	//快速单位化四维矢量

	//--------------------------------------------------------------------------------------------------------------
}
//==================================================================================================================
