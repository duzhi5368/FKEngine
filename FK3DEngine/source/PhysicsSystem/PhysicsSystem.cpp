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
	PhysicsSystem::PhysicsSystem()
	{
		mBoundVolumeList.Initialize( 50, 50 );
		mRigidBodyList.Initialize( 10, 10 );

		mIntersectVolumeList.Initialize( 20, 20 );
	}
	//--------------------------------------------------------------------------------------------------------------
	PhysicsSystem::~PhysicsSystem()
	{
		DeleteAllBoundVolume();
		DeleteAllRigidBody();
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建碰撞体
	BoundVolume* PhysicsSystem::CreateBoundVolume( BoundVolume::FnCreator fnCreator )
	{
		BoundVolume* pBoundVolume = fnCreator();
		*mBoundVolumeList.Push() = pBoundVolume;
		return pBoundVolume;
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建刚体
	RigidBody* PhysicsSystem::CreateRigidBody( RigidBody::FnCreator fnCreator )
	{
		RigidBody* pRigidBody = fnCreator();
		*mRigidBodyList.Push() = pRigidBody;
		return pRigidBody;
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除指定碰撞体
	void PhysicsSystem::DeleteBoundVolume( BoundVolume* pBoundVolume )
	{
		BoundVolumeList::Iterator it = mBoundVolumeList.Begin();
		BoundVolumeList::Iterator end = mBoundVolumeList.End();
		for(; it!=end; ++it )
		{
			if( *it == pBoundVolume )
			{
				mBoundVolumeList.Erase( it );
				delete pBoundVolume;
				return;
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除指定刚体
	void PhysicsSystem::DeleteRigidBody( RigidBody* pRigidBody )
	{
		RigidBodyList::Iterator it = mRigidBodyList.Begin();
		RigidBodyList::Iterator end = mRigidBodyList.End();
		for(; it!=end; ++it )
		{
			if( *it == pRigidBody )
			{
				mRigidBodyList.Erase( it );
				delete pRigidBody;
				return;
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除所有碰撞体
	void PhysicsSystem::DeleteAllBoundVolume()
	{
		BoundVolumeList::Iterator it = mBoundVolumeList.Begin();
		BoundVolumeList::Iterator end = mBoundVolumeList.End();
		for(; it!=end; ++it )
			SafeDelete( *it );

		mBoundVolumeList.Clear();
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除所有刚体
	void PhysicsSystem::DeleteAllRigidBody()
	{
		RigidBodyList::Iterator it = mRigidBodyList.Begin();
		RigidBodyList::Iterator end = mRigidBodyList.End();
		for(; it!=end; ++it )
			SafeDelete( *it );

		mRigidBodyList.Clear();
	}
	//--------------------------------------------------------------------------------------------------------------
	//进行碰撞检测
	void PhysicsSystem::DoCollisionDetect( RigidBody* pRigidBody, CollisionInfo* pCollisionInfo )
	{
		memset( pCollisionInfo, 0, sizeof(CollisionInfo) );
		pCollisionInfo->fCollisionDist = FLT_MAX;

		//更新运动数据
		if( !pRigidBody->_UpdateMovement() )
			return;

		//获取在其运动范围包围盒内的碰撞体
		mIntersectVolumeList.Clear();
		mRootBoundNode._GetIntersectBoundVolume( &mIntersectVolumeList, pRigidBody->mMovementBox );

		//循环检测每个在运动范围内的碰撞体
		BoundVolumeList::Iterator it = mIntersectVolumeList.Begin();
		BoundVolumeList::Iterator end = mIntersectVolumeList.End();
		for(; it!=end; ++it )
			pRigidBody->_DoCollisionDetect( *it, pCollisionInfo );
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
