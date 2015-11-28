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
	Plane2::Plane2()
		: mNormal	( Vector2( 0.0f, 0.0f ) )
		, mDistance	(0.0f)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	Plane2::Plane2( const Plane2& refPlane2 )
		: mNormal	(refPlane2.mNormal)
		, mDistance	(refPlane2.mDistance)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	Plane2::Plane2( const Vector2& vNormal, const float fDistance )
		: mNormal	(vNormal)
		, mDistance	(fDistance)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	Plane2::Plane2( const Vector2& vNormal, const Vector2& vPoint )
		: mNormal	(vNormal)
	{
		mDistance = -vNormal.GetDot( vPoint );
	}
	//--------------------------------------------------------------------------------------------------------------
	//复制操作符
	Plane2& Plane2::operator = ( const Plane2& refPlane2 )
	{
		mNormal = refPlane2.mNormal;
		mDistance = refPlane2.mDistance;

		return *this;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取平面与点之间的距离
	float FKFastcall Plane2::GetDistance( const Vector2& vPoint ) const
	{
		return mDistance + mNormal.GetDot( vPoint );
	}
	//--------------------------------------------------------------------------------------------------------------
	//判断平面与点的空间交错关系
	SpaceRelation FKFastcall Plane2::GetSide( const Vector2& vPoint ) const
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
	void Plane2::SetFromNormal( const Vector2& vNormal, const Vector2& vPoint )
	{
		mNormal = vNormal;
		mDistance = -vNormal.GetDot( vPoint );
	}
	//--------------------------------------------------------------------------------------------------------------
	//根据两个点设置平面
	void Plane2::SetFromPoints( const Vector2& v1, const Vector2& v2 )
	{
		mNormal.x = v2.y - v1.y;
		mNormal.y = v2.x - v1.x;

		mNormal.Normalize();

		mDistance = -mNormal.GetDot( v1 );
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
