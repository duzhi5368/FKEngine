/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	ASM_Vector3
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	数学库 三维矢量 汇编计算函数
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

	extern void FKFastcall _3DNOW_Vec3Div( Vector3* pOut, const Vector3* pArg1, const Vector3* pArg2 );		//三维矢量相除
	extern void FKFastcall _3DNOW_Vec3DivScalar( Vector3* pOut, const Vector3* pOrg, const float fScalar );	//三维矢量除以标量

	extern float FKFastcall _3DNOW_Vec3UnitLength( const Vector3* pIn );									//三维矢量单位长度
	extern void FKFastcall _3DNOW_Vec3Normalize( Vector3* pOut, const Vector3* pIn );						//单位化三维矢量
	extern void FKFastcall _3DNOW_Vec3NormalizeFast( Vector3* pOut, const Vector3* pIn, float fLength );	//快速单位化三维矢量

	//--------------------------------------------------------------------------------------------------------------
}
//==================================================================================================================
