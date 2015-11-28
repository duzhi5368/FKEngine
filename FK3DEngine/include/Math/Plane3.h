/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Plane3
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	数学库 三维平面
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//三维平面
	class FK_DLL Plane3
	{
	public:
		Vector3		mNormal;	//平面法线
		float		mDistance;	//平面与原点的距离

	public:
		Plane3();
		Plane3( const Plane3& refPlane3 );
		Plane3( const Vector3& vNormal, const float fDistance );
		Plane3( const Vector3& vNormal, const Vector3& vPoint );

		//复制操作符
		Plane3& operator = ( const Plane3& refPlane3 );

		//获取平面与点之间的距离
		float FKFastcall GetDistance( const Vector3& vPoint ) const;

		//判断平面与点的空间交错关系
		SpaceRelation FKFastcall GetSide( const Vector3& vPoint ) const;

		//根据法线和一个点设置平面
		void SetFromNormal( const Vector3& vNormal, const Vector3& vPoint );

		//根据三个点设置平面
		void SetFromPoints( const Vector3& v1, const Vector3& v2, const Vector3& v3 );

		//获取该面沿 X 轴投影对应的二维平面
		bool GetOrthoPlaneAloneX( Plane2* pOutPlane2, const Vector3& vPointOnPlane, const float fX );

		//获取该面沿 Y 轴投影对应的二维平面
		bool GetOrthoPlaneAloneY( Plane2* pOutPlane2, const Vector3& vPointOnPlane, const float fY );

		//获取该面沿 Z 轴投影对应的二维平面
		bool GetOrthoPlaneAloneZ( Plane2* pOutPlane2, const Vector3& vPointOnPlane, const float fZ );
	};
	//--------------------------------------------------------------------------------------------------------------
	typedef Stack< Plane3 >		Plane3Stack;
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
