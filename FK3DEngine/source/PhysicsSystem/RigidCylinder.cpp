/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	RigidCylinder
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../../include/PhysiceSystem/PhysicsSystemInsideHeader.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	RigidCylinder::RigidCylinder()
		: mRadius		(0.0f)
		, mHeight		(0.0f)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	//碰撞体创建函数
	RigidBody* RigidCylinder::Creator()
	{
		return new RigidCylinder;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置刚性圆柱体
	void FKFastcall RigidCylinder::SetRigidCylinder( float fRadius, float fHeight )
	{
		mRadius = fRadius;
		mHeight = fHeight;

		float fHalfHeight = fHeight * 0.5f;

		mBoundBox.SetMaxMin( fRadius, fHalfHeight, fRadius, -fRadius, -fHalfHeight, -fRadius );
	}
	//--------------------------------------------------------------------------------------------------------------
	//更新运动状态
	bool FKFastcall RigidCylinder::_UpdateMovement()
	{
		//计算线性移动长度和方向
		mLinearMoveLen = mLinearMove.GetLength();
		mLinearMoveDir = mLinearMove.GetNormalizedVecFast( mLinearMoveLen );

		//计算运动范围包围盒
		mMovementBox.mMaxVer = mBoundBox.mMaxVer + mMassCenter;
		mMovementBox.mMinVer = mBoundBox.mMinVer + mMassCenter;
		mMovementBox.mbEmpty = false;

		if( mLinearMove.x >= 0 )
			mMovementBox.mMaxVer.x += mLinearMove.x;
		else
			mMovementBox.mMinVer.x += mLinearMove.x;

		if( mLinearMove.y >= 0 )
			mMovementBox.mMaxVer.y += mLinearMove.y;
		else
			mMovementBox.mMinVer.y += mLinearMove.y;

		if( mLinearMove.z >= 0 )
			mMovementBox.mMaxVer.z += mLinearMove.z;
		else
			mMovementBox.mMinVer.z += mLinearMove.z;

		//如果没有运动则直接返回
		if( mLinearMove == Vector3::Zero )
			return false;

		return true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//进行碰撞检测
	void FKFastcall RigidCylinder::_DoCollisionDetect( BoundVolume* pBoundVolume, CollisionInfo* pInfo )
	{
		//进行碰撞检测
		if( mLinearMoveLen == 0.0f )
			return;

		pBoundVolume->DoCollisionDetect( this, pInfo );
	}
	//--------------------------------------------------------------------------------------------------------------
	//计算某方向上的圆柱体外径
	float FKFastcall RigidCylinder::_GetCylinderTerm( const Vector3& vDirection, const Vector3& vNormal )
	{
		float fAbsNdW = Abs( vNormal.GetDot( vDirection ) );
		float fRoot = Sqrt( Abs( 1.0 - fAbsNdW * fAbsNdW ) );
		return mRadius * fRoot + mHeight * 0.5f * fAbsNdW;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
