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
	BoundMesh::BoundMesh()
		: mNumFace			(0)
		, mNumVertex		(0)
		, mpBoundFace		(NULL)
		, mpFaceVertex		(NULL)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	BoundMesh::~BoundMesh()
	{
		SafeFree( mpBoundFace );
	}
	//--------------------------------------------------------------------------------------------------------------
	//碰撞体创建函数
	BoundMesh::BoundVolume* BoundMesh::Creator()
	{
		return new BoundMesh;
	}
	//--------------------------------------------------------------------------------------------------------------
	//载入碰撞网格数据
	void BoundMesh::LoadBoundMesh( BoundInfo* pBoundInfo, BoundFace* pBoundFace, Vector3* pVertex )
	{
		//复制三角面索引数据及其顶点数据
		size_t nFaceBufLen = pBoundInfo->nNumFace * sizeof(BoundFace);
		size_t nVerBufLen = pBoundInfo->nNumVertex * sizeof(Vector3);

		mpBoundFace = (BoundFace*)malloc( nFaceBufLen + nVerBufLen );
		mpFaceVertex = (Vector3*)( mpBoundFace + pBoundInfo->nNumFace );

		memcpy( mpBoundFace, pBoundFace, nFaceBufLen );
		memcpy( mpFaceVertex, pVertex, nVerBufLen );

		mNumFace = pBoundInfo->nNumFace;
		mNumVertex = pBoundInfo->nNumVertex;

		//设置包围盒数据
		mLocalBoundBox.SetMaxMin( pBoundInfo->vBoundBoxMax, pBoundInfo->vBoundBoxMin );
		mWorldBoundBox = mLocalBoundBox;

		//存储碰撞类型
		mBoundType = pBoundInfo->eType;
	}
	//--------------------------------------------------------------------------------------------------------------
	//射线碰撞检测
	bool BoundMesh::Intersect( const Ray3* pRay, Vector3* pContactPos )
	{
		return false;
	}
	//--------------------------------------------------------------------------------------------------------------
	//线段碰撞检测
	bool BoundMesh::Intersect( const Segment3* pSegment, Vector3* pContactPos )
	{
		return false;
	}
	//--------------------------------------------------------------------------------------------------------------
	//刚性球体碰撞检测
	void BoundMesh::DoCollisionDetect( RigidSphere* pRigidSphere, CollisionInfo* pInfo )
	{
		//转换刚性球体坐标和移动方向到碰撞体空间
		BoundFace* pCollisionFace = NULL;

		static Vector3 vSpherePos;
		static Vector3 vMoveDir;
		static Vector3 vCollisionPos;

		const float fRadius = pRigidSphere->mRadius;

		Mat4TransVec3( &vSpherePos, &mpBoundNode->mInvWorldMatrix, &pRigidSphere->mMassCenter );
		Mat4TransVec3( &vMoveDir, &mpBoundNode->mInvWorldMatrix, &pRigidSphere->mLinearMoveDir );

		//循环检测碰撞网格中所有三角面
		for( UINT i=0; i<mNumFace; ++i )
		{
			const BoundFace& rFace = mpBoundFace[i];
			const Vector3& vA = mpFaceVertex[ rFace.nV1Index ];
			const Vector3& vB = mpFaceVertex[ rFace.nV2Index ];
			const Vector3& vC = mpFaceVertex[ rFace.nV3Index ];

			//计算与该面的距离
			//MoveLen = ( ( RayOnPlane3 - RayStart ) 点乘 Plane3Nor ) / ( MoveDir 点乘 Plane3Nor )
			float fDotProduct = vMoveDir.GetDot( rFace.vNormal );

			//如果移动方向和碰撞面法线方向相同则无须检测
			if( fDotProduct >= 0.0f )
				continue;

			float fDist = ( ( vA - vSpherePos ).GetDot( rFace.vNormal ) + fRadius ) / fDotProduct;

			//如果未与球体相交
			if( fDist < -fRadius )
				continue;

			//如果相交距离大于移动距离则无须检测
			if( fDist > pRigidSphere->mLinearMoveLen )
				continue;

			//如果大于已知最短碰撞距离也无须检测
			if( fDist > pInfo->fCollisionDist )
				continue;

			//计算相交点坐标
			static Vector3 vIntersectPos;
			vIntersectPos = vSpherePos + vMoveDir * fDist;

			//计算三角形边界判断坐标
			static Vector3 vSideJudgePos;

			//如果球体将要穿过碰撞面
			if( fDist > 0.0f )
				vSideJudgePos = vIntersectPos;
			//如果球体与碰撞面相切
			else
				vSideJudgePos = vSpherePos + rFace.vNormal * fDist;

			//检测相交点是否在三角形内

			//计算三角形三条边的向量
			static Vector3 vAB, vBC, vCA;
			static Vector3 vSideNormal;
			float fSideDist;

			Vec3Sub( &vAB, &vB, &vA );
			Vec3Sub( &vBC, &vC, &vB );
			Vec3Sub( &vCA, &vA, &vC );


			//判断碰撞点是否在 AB 边内

			//计算垂直于 AB 边和三角形法线的向量（即 AB 边所在的垂直于三角形的平面）
			Vec3Cross( &vSideNormal, &rFace.vNormal, &vAB );
			vSideNormal.Normalize();

			//计算向量 AC 在 AB 边平面上的投影长度，如果为负值，则证明该法线指向三角形外，因此反转它。
			if( vSideNormal.GetDot( -vCA ) < 0.0f )
				vSideNormal = -vSideNormal;

			//如果碰撞点在 AB 面外（即三角形外）则没有与该面发生碰撞
			fSideDist = vSideNormal.GetDot( vA ) - fRadius;
			if( vSideNormal.GetDot( vSideJudgePos ) - fSideDist < 0.0f )
				continue;

			//计算用来消去 AB 边对角 C 的多余部分的平面与原点的距离
			//（由于该平面与 AB 面平行，所以直接用 AB 面的法线进行计算。）
			fSideDist = vSideNormal.GetDot( vC ) + fRadius;
			//如果物体的当前位置不在该平面内则说明未与该三角形发生碰撞，返回否。
			if( vSideNormal.GetDot( vSideJudgePos ) - fSideDist > 0.0f )
				continue;


			//判断碰撞点是否在 BC 边内
			Vec3Cross( &vSideNormal, &rFace.vNormal, &vBC );
			vSideNormal.Normalize();

			if( vSideNormal.GetDot( -vAB ) < 0.0f )
				vSideNormal = -vSideNormal;

			fSideDist = vSideNormal.GetDot( vB ) - fRadius;
			if( vSideNormal.GetDot( vSideJudgePos ) - fSideDist < 0.0f )
				continue;

			fSideDist = vSideNormal.GetDot( vA ) + fRadius;
			if( vSideNormal.GetDot( vSideJudgePos ) - fSideDist > 0.0f )
				continue;


			//判断碰撞点是否在 CA 边内
			Vec3Cross( &vSideNormal, &rFace.vNormal, &vCA );
			vSideNormal.Normalize();

			if( vSideNormal.GetDot( -vBC ) < 0.0f )
				vSideNormal = -vSideNormal;

			fSideDist = vSideNormal.GetDot( vC ) - fRadius;
			if( vSideNormal.GetDot( vSideJudgePos ) - fSideDist < 0.0f )
				continue;

			fSideDist = vSideNormal.GetDot( vB ) + fRadius;
			if( vSideNormal.GetDot( vSideJudgePos ) - fSideDist > 0.0f )
				continue;

			//如果这次的碰撞距离最短
			if( fDist < pInfo->fCollisionDist )
			{
				vCollisionPos = vIntersectPos;
				pCollisionFace = &mpBoundFace[i];
				pInfo->fCollisionDist = fDist;
			}
		}

		//如果发生碰撞
		if( pCollisionFace != NULL )
		{
			//将碰撞目标延法线方向平移一小段距离，以防下一帧穿越平面。
			pInfo->vCollisionPos = ( vCollisionPos + pCollisionFace->vNormal * 0.0001f ) * mpBoundNode->mWorldMatrix;
			pInfo->vFaceNormal = pCollisionFace->vNormal * mpBoundNode->mWorldMatrix;
			pInfo->pCollisionVolume = this;
			pInfo->bCollision = true;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//刚性圆柱体碰撞检测
	void BoundMesh::DoCollisionDetect( RigidCylinder* pRigidCylinder, CollisionInfo* pInfo )
	{
		//转换刚性圆柱体坐标和移动方向到碰撞体空间
		BoundFace* pCollisionFace = NULL;

		static Vector3 vCylinderPos;
		static Vector3 vCylinderDir;
		static Vector3 vMoveDir;
		static Vector3 vCollisionPos;

		Mat4TransVec3( &vCylinderPos, &mpBoundNode->mInvWorldMatrix, &pRigidCylinder->mMassCenter );
		Mat4TransVec3( &vCylinderDir, &mpBoundNode->mInvWorldMatrix, &Vector3::UnitY );
		Mat4TransVec3( &vMoveDir, &mpBoundNode->mInvWorldMatrix, &pRigidCylinder->mLinearMoveDir );

		//循环检测碰撞网格中所有三角面
		for( UINT i=0; i<mNumFace; ++i )
		{
			const BoundFace& rFace = mpBoundFace[i];
			const Vector3& vA = mpFaceVertex[ rFace.nV1Index ];
			const Vector3& vB = mpFaceVertex[ rFace.nV2Index ];
			const Vector3& vC = mpFaceVertex[ rFace.nV3Index ];

			//计算与该面的距离
			//MoveLen = ( ( RayOnPlane3 - RayStart ) 点乘 Plane3Nor ) / ( MoveDir 点乘 Plane3Nor )
			float fDotProduct = vMoveDir.GetDot( rFace.vNormal );

			//如果移动方向和碰撞面法线方向相同则无须检测
			if( fDotProduct >= 0.0f )
				continue;

			//计算圆柱体外径
			float fTerm = pRigidCylinder->_GetCylinderTerm( vCylinderDir, rFace.vNormal );
			float fDist = ( ( vA - vCylinderPos ).GetDot( rFace.vNormal ) + fTerm ) / fDotProduct;

			//如果未与圆柱体相交
			if( fDist < -fTerm )
				continue;

			//如果相交距离大于移动距离则无须检测
			if( fDist > pRigidCylinder->mLinearMoveLen )
				continue;

			//如果大于已知最短碰撞距离也无须检测
			if( fDist > pInfo->fCollisionDist )
				continue;

			//计算相交点坐标
			static Vector3 vIntersectPos;
			vIntersectPos = vCylinderPos + vMoveDir * fDist;

			//计算三角形边界判断坐标
			static Vector3 vSideJudgePos;

			//如果圆柱体将要穿过碰撞面
			if( fDist > 0.0f )
				vSideJudgePos = vIntersectPos;
			//如果圆柱体与碰撞面相切
			else
				vSideJudgePos = vCylinderPos + rFace.vNormal * fDist;

			//检测相交点是否在三角形内

			//计算三角形三条边的向量
			static Vector3 vAB, vBC, vCA;
			static Vector3 vSideNormal;
			float fSideDist;

			Vec3Sub( &vAB, &vB, &vA );
			Vec3Sub( &vBC, &vC, &vB );
			Vec3Sub( &vCA, &vA, &vC );


			//判断碰撞点是否在 AB 边内

			//计算垂直于 AB 边和三角形法线的向量（即 AB 边所在的垂直于三角形的平面）
			Vec3Cross( &vSideNormal, &rFace.vNormal, &vAB );
			vSideNormal.Normalize();

			//计算向量 AC 在 AB 边平面上的投影长度，如果为负值，则证明该法线指向三角形外，因此反转它。
			if( vSideNormal.GetDot( -vCA ) < 0.0f )
				vSideNormal = -vSideNormal;

			fTerm = pRigidCylinder->_GetCylinderTerm( vCylinderDir, vSideNormal );

			//如果碰撞点在 AB 面外（即三角形外）则没有与该面发生碰撞
			fSideDist = vSideNormal.GetDot( vA ) - fTerm;
			if( vSideNormal.GetDot( vSideJudgePos ) - fSideDist < 0.0f )
				continue;

			//计算用来消去 AB 边对角 C 的多余部分的平面与原点的距离
			//（由于该平面与 AB 面平行，所以直接用 AB 面的法线进行计算。）
			fSideDist = vSideNormal.GetDot( vC ) + fTerm;
			//如果物体的当前位置不在该平面内则说明未与该三角形发生碰撞，返回否。
			if( vSideNormal.GetDot( vSideJudgePos ) - fSideDist > 0.0f )
				continue;


			//判断碰撞点是否在 BC 边内
			Vec3Cross( &vSideNormal, &rFace.vNormal, &vBC );
			vSideNormal.Normalize();

			if( vSideNormal.GetDot( -vAB ) < 0.0f )
				vSideNormal = -vSideNormal;

			fTerm = pRigidCylinder->_GetCylinderTerm( vCylinderDir, vSideNormal );

			fSideDist = vSideNormal.GetDot( vB ) - fTerm;
			if( vSideNormal.GetDot( vSideJudgePos ) - fSideDist < 0.0f )
				continue;

			fSideDist = vSideNormal.GetDot( vA ) + fTerm;
			if( vSideNormal.GetDot( vSideJudgePos ) - fSideDist > 0.0f )
				continue;


			//判断碰撞点是否在 CA 边内
			Vec3Cross( &vSideNormal, &rFace.vNormal, &vCA );
			vSideNormal.Normalize();

			if( vSideNormal.GetDot( -vBC ) < 0.0f )
				vSideNormal = -vSideNormal;

			fTerm = pRigidCylinder->_GetCylinderTerm( vCylinderDir, vSideNormal );

			fSideDist = vSideNormal.GetDot( vC ) - fTerm;
			if( vSideNormal.GetDot( vSideJudgePos ) - fSideDist < 0.0f )
				continue;

			fSideDist = vSideNormal.GetDot( vB ) + fTerm;
			if( vSideNormal.GetDot( vSideJudgePos ) - fSideDist > 0.0f )
				continue;

			//如果这次的碰撞距离最短
			if( fDist < pInfo->fCollisionDist )
			{
				vCollisionPos = vIntersectPos;
				pCollisionFace = &mpBoundFace[i];
				pInfo->fCollisionDist = fDist;
			}
		}

		//如果发生碰撞
		if( pCollisionFace != NULL )
		{
			//将碰撞目标延法线方向平移一小段距离，以防下一帧穿越平面。
			pInfo->vCollisionPos = ( vCollisionPos + pCollisionFace->vNormal * FK_EPSILON ) * mpBoundNode->mWorldMatrix;
			pInfo->vFaceNormal = pCollisionFace->vNormal * mpBoundNode->mWorldMatrix;
			pInfo->pCollisionVolume = this;
			pInfo->bCollision = true;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
