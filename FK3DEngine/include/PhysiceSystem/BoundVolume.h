/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	BoundVolume
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	碰撞体类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	/*
		矢量与平面相交（光线追踪）计算公式：

		光线与平面相交点：	RayOnPlane3	（平面上的一点 或 光线与平面相交点）
		平面法线：			Plane3Nor
		平面距原点距离：	Plane3Dist
		光线起始点：		RayStart
		光线结束点：		RayEnd
		光线运动方向：		MoveDir
		光线运动距离：		MoveLen

		RayOnPlane3	= RayStart + MoveLen * MoveDir
		Plane3Dist	= RayOnPlane3 点乘 Plane3Nor

		等价代入：
		Plane3Dist	= ( RayStart + MoveLen * MoveDir ) 点乘 Plane3Normal
		= RayStart 点乘 Plane3Normal + MoveLen * MoveDir 点乘 Plane3Normal
		左右交换：
		MoveLen	* MoveDir 点乘 Plane3Nor = Plane3Dist - RayStart 点乘 Plane3Normal
		MoveLen = ( Plane3Dist - RayStart 点乘 Plane3Normal )				/ ( MoveDir 点乘 Plane3Normal )
		= ( RayOnPlane3 点乘 Plane3Normal - RayStart 点乘 Plane3Normal )	/ ( MoveDir 点乘 Plane3Normal )
		= ( ( RayOnPlane3 - RayStart ) 点乘 Plane3Normal )			/ ( MoveDir 点乘 Plane3Normal )

		我们只需将上个方程中的 RayOnPlane3 替换为平面上任意一点即可求得
		MoveLen 进而求得光线与平面的实际相交点  数学就是这样倒来倒去的 ：）
	*/
	//--------------------------------------------------------------------------------------------------------------
	class BoundNode;
	class RigidBody;
	class RigidSphere;
	class RigidCylinder;
	class BoundVolume;
	//--------------------------------------------------------------------------------------------------------------
	//碰撞信息
	struct CollisionInfo
	{
		bool			bCollision;			//是否发生碰撞
		float			fCollisionDist;		//碰撞距离
		Vector3			vCollisionPos;		//碰撞质心位置
		Vector3			vFaceNormal;		//碰撞面法线
		BoundVolume*	pCollisionVolume;	//碰撞体指针
	};
	//--------------------------------------------------------------------------------------------------------------
	//碰撞体类
	class FK_DLL BoundVolume
	{
	public:
		//碰撞体创建函数
		typedef BoundVolume* (*FnCreator)();

	public:
		BoundNode*		mpBoundNode;		//挂接碰撞节点指针

		AxisBox3			mLocalBoundBox;		//碰撞体局部坐标包围盒
		AxisBox3			mWorldBoundBox;		//碰撞体世界坐标包围盒

		BoundInfo::Type	mBoundType;			//碰撞类型

	public:
		BoundVolume();
		virtual ~BoundVolume() = 0;

		//射线相交测试
		virtual bool Intersect( const Ray3* pRay, Vector3* pContactPos ) = 0;

		//线段相交测试
		virtual bool Intersect( const Segment3* pSegment, Vector3* pContactPos ) = 0;

		//刚性球体碰撞检测
		virtual void DoCollisionDetect( RigidSphere* pRigidSphere, CollisionInfo* pInfo ) = 0;

		//刚性圆柱体碰撞检测
		virtual void DoCollisionDetect( RigidCylinder* pRigidCylinder, CollisionInfo* pInfo ) = 0;
	};
	//--------------------------------------------------------------------------------------------------------------
	typedef Stack< BoundVolume* >	BoundVolumeList;
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
