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
*	文件说明：	刚性圆柱体类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//刚性圆柱体
	class FK_DLL RigidCylinder : public RigidBody
	{
	public:
		float		mRadius;	//圆柱体半径
		float		mHeight;	//圆柱体高度

	public:
		RigidCylinder();

		//碰撞体创建函数
		static RigidBody* Creator();

		//设置刚性圆柱体
		void FKFastcall SetRigidCylinder( float fRadius, float fHeight );

	protected:
		//更新运动状态
		bool FKFastcall _UpdateMovement();

		//进行碰撞检测
		void FKFastcall _DoCollisionDetect( BoundVolume* pBoundVolume, CollisionInfo* pInfo );

	public:
		//计算某方向上的圆柱体外径
		float FKFastcall _GetCylinderTerm( const Vector3& vDirection, const Vector3& vNormal );
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
