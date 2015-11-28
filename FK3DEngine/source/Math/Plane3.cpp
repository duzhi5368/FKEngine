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
	Plane3::Plane3()
		: mNormal	( Vector3( 0.0f, 0.0f, 0.0f ) )
		, mDistance	(0.0f)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	Plane3::Plane3( const Plane3& refPlane3 )
		: mNormal	(refPlane3.mNormal)
		, mDistance	(refPlane3.mDistance)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	Plane3::Plane3( const Vector3& vNormal, const float fDistance )
		: mNormal	(vNormal)
		, mDistance	(fDistance)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	Plane3::Plane3( const Vector3& vNormal, const Vector3& vPoint )
		: mNormal	(vNormal)
	{
		mDistance = -vNormal.GetDot( vPoint );
	}
	//--------------------------------------------------------------------------------------------------------------
	//复制操作符
	Plane3& Plane3::operator = ( const Plane3& refPlane3 )
	{
		mNormal = refPlane3.mNormal;
		mDistance = refPlane3.mDistance;

		return *this;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取平面与点之间的距离
	float FKFastcall Plane3::GetDistance( const Vector3& vPoint ) const
	{
		return mDistance + mNormal.GetDot( vPoint );
	}
	//--------------------------------------------------------------------------------------------------------------
	//判断平面与点的空间交错关系
	SpaceRelation FKFastcall Plane3::GetSide( const Vector3& vPoint ) const
	{
		float Distance = mDistance + mNormal.GetDot( vPoint );

		if( Distance > 0.001f )
			return SR_POSITIVE;
		else if( Distance < -0.001f )
			return SR_NEGATIVE;
		else
			return SR_COINCIDING;
	}
	//--------------------------------------------------------------------------------------------------------------
	//根据法线和一个点设置平面
	void Plane3::SetFromNormal( const Vector3& vNormal, const Vector3& vPoint )
	{
		mNormal = vNormal;
		mDistance = -vNormal.GetDot( vPoint );
	}
	//--------------------------------------------------------------------------------------------------------------
	//根据三个点设置平面
	void Plane3::SetFromPoints( const Vector3& v1, const Vector3& v2, const Vector3& v3 )
	{
		Vector3 edge1 = v2 - v1;
		Vector3 edge2 = v3 - v1;

		Vec3Cross( &mNormal, &edge1, &edge2 );
		mNormal.Normalize();

		mDistance = -mNormal.GetDot( v1 );
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取该面沿 X 轴投影对应的二维平面
	bool Plane3::GetOrthoPlaneAloneX( Plane2* pOutPlane2, const Vector3& vPointOnPlane, const float fX )
	{
		if( mNormal.y == 0.0f && mNormal.z == 0.0f && mDistance != -fX )
			return false;

		pOutPlane2->mNormal.x = mNormal.y;
		pOutPlane2->mNormal.y = mNormal.z;

		pOutPlane2->mDistance = -( fX * mNormal.x + vPointOnPlane.y * mNormal.y + vPointOnPlane.z * mNormal.z );

		return true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取该面沿 Y 轴投影对应的二维平面
	bool Plane3::GetOrthoPlaneAloneY( Plane2* pOutPlane2, const Vector3& vPointOnPlane, const float fY )
	{
		if( mNormal.x == 0.0f && mNormal.z == 0.0f && mDistance != -fY )
			return false;

		pOutPlane2->mNormal.x = mNormal.x;
		pOutPlane2->mNormal.y = mNormal.z;

		pOutPlane2->mDistance = -( vPointOnPlane.x * mNormal.x + fY * mNormal.y + vPointOnPlane.z * mNormal.z );

		return true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取该面沿 Z 轴投影对应的二维平面
	bool Plane3::GetOrthoPlaneAloneZ( Plane2* pOutPlane2, const Vector3& vPointOnPlane, const float fZ )
	{
		if( mNormal.x == 0.0f && mNormal.y == 0.0f && mDistance != -fZ )
			return false;

		pOutPlane2->mNormal.x = mNormal.x;
		pOutPlane2->mNormal.y = mNormal.y;

		pOutPlane2->mDistance = -( vPointOnPlane.x * mNormal.x + vPointOnPlane.y * mNormal.y + fZ * mNormal.z );

		return true;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
