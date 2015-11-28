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
*	文件说明：	数学库 三维射线类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//三维射线类
	class FK_DLL Ray3
	{
	public:
		Vector3		mOrigin;	//射线原点
		Vector3		mDirection;	//射线方向

	public:
		//不同的构造函数
		Ray3();
		Ray3( const Vector3& vOrigin, const Vector3& vDirection );

		//各种运算符的重载
		Ray3& operator = ( const Ray3& ray3ref );
		Ray3 operator * ( const Matrix4& mat4ref ) const;
		void operator *= ( const Matrix4& mat4ref );

		//获取与指定面的相交点坐标
		float FKFastcall GetIntersectPos( const Vector3& vNormal, const Vector3& vVerOnPlane3, Vector3* pIntersectPos ) const;
		float FKFastcall GetIntersectPos( const Plane3& sPlane3, Vector3* pIntersectPos ) const;
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
