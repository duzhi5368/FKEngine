/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Ray3
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
	Ray3::Ray3()
		: mOrigin		(Vector3::Zero)
		, mDirection	(Vector3::Zero)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	Ray3::Ray3( const Vector3& vOrigin, const Vector3& vDirection )
		: mOrigin		(vOrigin)
		, mDirection	(vDirection)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	Ray3& Ray3::operator = ( const Ray3& ray3ref )
	{
		memcpy( this, &ray3ref, sizeof(Ray3) );
		return *this;
	}
	//--------------------------------------------------------------------------------------------------------------
	Ray3 Ray3::operator * ( const Matrix4& mat4ref ) const
	{
		static Ray3 tmp;
		tmp.mOrigin = mOrigin * mat4ref;
		tmp.mDirection = mDirection * mat4ref;
		tmp.mDirection.Normalize();
		return tmp;
	}
	//--------------------------------------------------------------------------------------------------------------
	void Ray3::operator *= ( const Matrix4& mat4ref )
	{
		mOrigin *= mat4ref;
		mDirection *= mat4ref;
		mDirection.Normalize();
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取与指定面的相交点坐标
	float FKFastcall Ray3::GetIntersectPos( const Vector3& vNormal, const Vector3& vVerOnPlane3, Vector3* pIntersectPos ) const
	{
		//计算相交点坐标
		//
		//原型为：	撞击距离 = ( RayOnPlane3 点乘 Plane3Normal - RayStart 点乘 Plane3Normal ) / ( MoveDir 点乘 Plane3Normal )
		//			RayOnPlane3 = RayStart + MoveDir * 撞击距离
		float fDot = mDirection.GetDot( vNormal );

		//如果点乘积大于等于零则证明不相交
		if( fDot >= 0.0f )
			return -1.0f;

		float fDist = ( vVerOnPlane3 - mOrigin ).GetDot( vNormal ) / fDot;
		*pIntersectPos = mOrigin + mDirection * fDist;

		return fDist;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取与指定面的相交点坐标
	float FKFastcall Ray3::GetIntersectPos( const Plane3& sPlane3, Vector3* pIntersectPos ) const
	{
		float fDot = mDirection.GetDot( sPlane3.mNormal );

		if( fDot >= 0.0f )
			return -1.0f;

		float fDist = ( -sPlane3.mDistance - mOrigin.GetDot( sPlane3.mNormal ) ) / fDot;
		*pIntersectPos = mOrigin + mDirection * fDist;

		return fDist;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
