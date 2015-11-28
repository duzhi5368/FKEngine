/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	RigidBody
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	刚体类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//刚体类
	class FK_DLL RigidBody
	{
	public:
		friend class PhysicsSystem;

		//刚体创建函数
		typedef RigidBody* (*FnCreator)();

	public:
		Vector3			mMassCenter;	//质心世界坐标

		Vector3			mLinearMove;	//线性移动量
		Vector3			mLinearMoveDir;	//线性移动方向
		float			mLinearMoveLen;	//线性移动长度

		AxisBox3			mBoundBox;		//刚体包围盒（局部坐标）
		AxisBox3			mMovementBox;	//运动范围包围盒（世界坐标）

	public:
		RigidBody();
		virtual ~RigidBody() = 0;

		//设置质心世界坐标
		void FKFastcall SetMassCenter( const Vector3& vMassCenter );

		//设置线性移动量
		void FKFastcall SetLinearMove( const Vector3& vLinearMove );

	protected:
		//更新运动状态
		virtual bool FKFastcall _UpdateMovement() = 0;

		//进行碰撞检测
		virtual void FKFastcall _DoCollisionDetect( BoundVolume* pBoundVolume, CollisionInfo* pInfo ) = 0;
	};
	//--------------------------------------------------------------------------------------------------------------
	typedef Stack< RigidBody* >		RigidBodyList;
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
