/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	ASM_Vector2
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	数学库 二维矢量 汇编计算函数
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

	extern void FKFastcall _3DNOW_Vec2Div( Vector2* pOut, const Vector2* pArg1, const Vector2* pArg2 );		//二维矢量相除
	extern void FKFastcall _3DNOW_Vec2DivScalar( Vector2* pOut, const Vector2* pOrg, const float fScalar );	//二维矢量除以标量

	extern float FKFastcall _3DNOW_Vec2UnitLength( const Vector2* pIn );									//二维矢量单位长度
	extern void FKFastcall _3DNOW_Vec2Normalize( Vector2* pOut, const Vector2* pIn );						//单位化二维矢量
	extern void FKFastcall _3DNOW_Vec2NormalizeFast( Vector2* pOut, const Vector2* pIn, float fLength );	//快速单位化二维矢量

	//--------------------------------------------------------------------------------------------------------------
}
//==================================================================================================================
