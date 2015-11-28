/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	BoundMesh
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	碰撞网格类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//碰撞网格类
	class FK_DLL BoundMesh : public BoundVolume
	{
	public:
		UINT			mNumFace;			//三角面索引数量
		UINT			mNumVertex;			//三角面顶点数量

		BoundFace*		mpBoundFace;		//碰撞三角面数据
		Vector3*		mpFaceVertex;		//三角面顶点数据

	public:
		BoundMesh();
		~BoundMesh();

		//碰撞体创建函数
		static BoundVolume* Creator();

		//载入碰撞网格数据
		void LoadBoundMesh( BoundInfo* pBoundInfo, BoundFace* pBoundFace, Vector3* pVertex );

		//射线相交测试
		bool Intersect( const Ray3* pRay, Vector3* pContactPos );

		//线段相交测试
		bool Intersect( const Segment3* pSegment, Vector3* pContactPos );

		//刚性球体碰撞检测
		void DoCollisionDetect( RigidSphere* pRigidSphere, CollisionInfo* pInfo );

		//刚性圆柱体碰撞检测
		void DoCollisionDetect( RigidCylinder* pRigidCylinder, CollisionInfo* pInfo );
	};
	//--------------------------------------------------------------------------------------------------------------
	typedef Stack< BoundMesh* >		BoundMeshList;
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
