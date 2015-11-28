/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	PhysicsSystem
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	物理系统
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//物理系统
	class FK_DLL PhysicsSystem : public Singleton< PhysicsSystem >
	{
	public:
		BoundVolumeList		mBoundVolumeList;		//碰撞体列表
		RigidBodyList		mRigidBodyList;			//刚体列表

		BoundNode			mRootBoundNode;			//根级碰撞节点

	protected:
		BoundVolumeList		mIntersectVolumeList;	//相交碰撞体堆栈

	public:
		PhysicsSystem();
		~PhysicsSystem();

		//创建碰撞体
		BoundVolume* CreateBoundVolume( BoundVolume::FnCreator fnCreator );

		//创建刚体
		RigidBody* CreateRigidBody( RigidBody::FnCreator fnCreator );

		//删除指定碰撞节点
		void DeleteBoundNode( BoundNode* pBoundNode );

		//删除指定碰撞体
		void DeleteBoundVolume( BoundVolume* pBoundVolume );

		//删除指定刚体
		void DeleteRigidBody( RigidBody* pRigidBody );

		//删除所有碰撞体
		void DeleteAllBoundVolume();

		//删除所有刚体
		void DeleteAllRigidBody();

		//进行碰撞检测
		void DoCollisionDetect( RigidBody* pRigidBody, CollisionInfo* pCollisionInfo );
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
