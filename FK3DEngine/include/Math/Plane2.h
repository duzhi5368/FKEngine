/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Plane2
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	数学库 二维平面
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//二维平面
	class FK_DLL Plane2
	{
	public:
		Vector2		mNormal;	//平面法线
		float		mDistance;	//平面与原点的距离

	public:
		Plane2();
		Plane2( const Plane2& refPlane2 );
		Plane2( const Vector2& vNormal, const float fDistance );
		Plane2( const Vector2& vNormal, const Vector2& vPoint );

		//复制操作符
		Plane2& operator = ( const Plane2& refPlane2 );

		//获取平面与点之间的距离
		float FKFastcall GetDistance( const Vector2& vPoint ) const;

		//判断平面与点的空间交错关系
		SpaceRelation FKFastcall GetSide( const Vector2& vPoint ) const;

		//根据法线和一个点设置平面
		void SetFromNormal( const Vector2& vNormal, const Vector2& vPoint );

		//根据两个点设置平面
		void SetFromPoints( const Vector2& v1, const Vector2& v2 );
	};
	//--------------------------------------------------------------------------------------------------------------
	typedef Stack< Plane2 >		Plane2Stack;
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
