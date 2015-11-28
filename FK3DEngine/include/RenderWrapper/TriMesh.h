/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	TriMesh
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	三角面网格类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//三角面网格类
	class FK_DLL TriMesh : public Geometry
	{
	public:
		UINT				mNumBone;			//关联骨头数量
		UINT*				mpBoneIndex;		//关联骨头在骨骼中的索引值
		Skeleton*			mpSkeleton;			//关联骨骼

	public:
		TriMesh();
		~TriMesh();

		//几何体创建函数
		static Geometry* Creator();

		//生成面向 Z 轴反方向的平面
		void MakePlane3( float fXMin, float fXMax, float fYMin, float fYMax, float fZ,
			bool bHaveUV = false, bool bHaveNormal = false, Material* pMaterial = NULL );

		//生成立方体网格
		void MakeBox( const Vector3& vCenter, float fSideLen, UINT nSegment = 1,
			bool bHaveUV = false, bool bHaveNormal = false, Material* pMaterial = NULL );

		//生成球体网格
		void MakeSphere( const Vector3& vCenter, float fRadius, UINT nRing = 20, UINT nSegment = 20,
			bool bHaveUV = false, bool bHaveNormal = false, Material* pMaterial = NULL );

		//设置关联骨头索引数据
		void SetBoneIndex( UINT* pBoneIndex, UINT nNumBone );

		//关联骨骼
		void SetSkeleton( Skeleton* pSkeleton );

	protected:
		//渲染物体
		void _RenderObj( const RenderParam* pParam, const Matrix4* pNodeMatrix );

		//渲染动画物体
		void _RenderAnimObj( const RenderParam* pParam, const AnimControl* pAnimControl, const Matrix4* pNodeMatrix );
	};
	//--------------------------------------------------------------------------------------------------------------
	typedef Stack< TriMesh* >		TriMeshList;
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
