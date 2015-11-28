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
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../../include/RenderWrapper/RenderWrapperInsideHeader.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	TriMesh::TriMesh()
		: mNumBone			(0)
		, mpBoneIndex		(NULL)
		, mpSkeleton		(NULL)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	TriMesh::~TriMesh()
	{
		SafeFree( mpBoneIndex );
	}
	//--------------------------------------------------------------------------------------------------------------
	//几何体创建函数
	Geometry* TriMesh::Creator()
	{
		return new TriMesh;
	}
	//--------------------------------------------------------------------------------------------------------------
	//生成面向 Z 轴反方向的平面
	void TriMesh::MakePlane3( float fXMin, float fXMax, float fYMin, float fYMax, float fZ,
		bool bHaveUV, bool bHaveNormal, Material* pMaterial )
	{
		const UINT nNumVertex = 4;
		const UINT nNumIndex = 4;

		//建立网格顶点流
		BuildMeshStream( nNumVertex, nNumIndex, bHaveUV, bHaveNormal );

		//锁顶点缓存
		UINT nNumVertexBuffer = 0;

		Vector3* pPos = (Vector3*)mppVertexBuffer[nNumVertexBuffer]->Lock( 0, 0 );

		pPos->x = fXMin;		pPos->y = fYMin;		pPos->z = fZ;			++pPos;
		pPos->x = fXMin;		pPos->y = fYMax;		pPos->z = fZ;			++pPos;
		pPos->x = fXMax;		pPos->y = fYMax;		pPos->z = fZ;			++pPos;
		pPos->x = fXMax;		pPos->y = fYMin;		pPos->z = fZ;			++pPos;

		mppVertexBuffer[nNumVertexBuffer++]->Unlock();

		if( bHaveNormal )
		{
			Vector3* pNormal = (Vector3*)mppVertexBuffer[nNumVertexBuffer]->Lock( 0, 0 );

			pNormal->x = 0.0f;		pNormal->y = 0.0f;		pNormal->z = -1.0f;		++pNormal;
			pNormal->x = 0.0f;		pNormal->y = 0.0f;		pNormal->z = -1.0f;		++pNormal;
			pNormal->x = 0.0f;		pNormal->y = 0.0f;		pNormal->z = -1.0f;		++pNormal;
			pNormal->x = 0.0f;		pNormal->y = 0.0f;		pNormal->z = -1.0f;		++pNormal;

			mppVertexBuffer[nNumVertexBuffer++]->Unlock();
		}

		if( bHaveUV )
		{
			float* pUV = (float*)mppVertexBuffer[nNumVertexBuffer]->Lock( 0, 0 );
			
			*pUV++ = 0.0f;		*pUV++ = 1.0f;
			*pUV++ = 0.0f;		*pUV++ = 0.0f;
			*pUV++ = 1.0f;		*pUV++ = 0.0f;
			*pUV++ = 1.0f;		*pUV++ = 1.0f;

			mppVertexBuffer[nNumVertexBuffer++]->Unlock();
		}

		//锁定索引缓存
		WORD* pIndex = (WORD*)mpIndexBuffer->Lock( 0, 0 );
		*pIndex++ = 0;		*pIndex++ = 1;		*pIndex++ = 2;		*pIndex++ = 3;
		mpIndexBuffer->Unlock();


		//设置包围盒
		mBoundBox.SetMaxMin( fXMax, fYMax, fZ + 1.0f, fXMin, fYMin, fZ - 1.0f );

		//设置渲染参数
		AllocRenderParamBuf();
		mpRenderParam->eDrawType	= RenderParam::DT_Basic;

		mpRenderParam->sBasicParam.ePrimType	= PT_TRIANGLE_FAN;
		mpRenderParam->sBasicParam.nVerOffset	= 0;
		mpRenderParam->sBasicParam.nMinIndex	= 0;
		mpRenderParam->sBasicParam.nIndexOffset	= 0;
		mpRenderParam->sBasicParam.nVerCount	= 4;
		mpRenderParam->sBasicParam.nPrimCount	= 2;

		//设置材质
		if( pMaterial != NULL )
		{
			AllocMaterialBuf();
			mppMaterial[0] = pMaterial;
			mpRenderParam->pMaterial = pMaterial;
			pMaterial->AddRef();
		}
		else
		{
			mpRenderParam->pMaterial = NULL;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//生成立方体网格
	void TriMesh::MakeBox( const Vector3& vCenter, float fSideLen, UINT nSegment,
		bool bHaveUV, bool bHaveNormal, Material* pMaterial )
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	//生成球体网格
	void TriMesh::MakeSphere( const Vector3& vCenter, float fRadius, UINT nRing, UINT nSegment,
		bool bHaveUV, bool bHaveNormal, Material* pMaterial )
	{
		//检测半径
		if( fRadius <= 0.0f )
			Except( Exception::ERR_INVALIDPARAMS, "生成球体网格的半径必须大于零。" );

		//生成球体顶点数据
		UINT nNumVertex = ( nRing + 1 ) * ( nSegment + 1 );
		UINT nNumIndex = 2 * nRing * ( nSegment + 1 );

		//建立网格顶点流
		BuildMeshStream( nNumVertex, nNumIndex, bHaveUV, bHaveNormal );

		//锁顶点缓存
		UINT nNumVertexBuffer = 0;

		Vector3* pPos = (Vector3*)mppVertexBuffer[nNumVertexBuffer++]->Lock( 0, 0 );

		Vector3* pNormal = NULL;
		if( bHaveNormal )
			pNormal = (Vector3*)mppVertexBuffer[nNumVertexBuffer++]->Lock( 0, 0 );

		float* pUV = NULL;
		if( bHaveUV )
			pUV = (float*)mppVertexBuffer[nNumVertexBuffer++]->Lock( 0, 0 );

		//锁定索引缓存
		WORD* pIndex = (WORD*)mpIndexBuffer->Lock( 0, 0 );


		//计算角度
		float fRingAngle = FK_PI / (float)nRing;
		float fSegmentAngle = FK_PI * 2.0f / (float)nSegment;

		float r = 0.0f;
		float y = 0.0f;
		float x = 0.0f;
		float z = 0.0f;

		//生成垂直一环顶点的循环
		WORD wIndex = 0;
		for( UINT ring=0; ring<nRing+1; ++ring )
		{
			SinCos( (float)ring * fRingAngle, &r, &y );

			//生成水平的一段顶点的循环			
			for( UINT seg=0; seg<nSegment+1; ++seg )
			{
				SinCos( (float)seg * fSegmentAngle, &x, &z );

				pPos->x = x * r	* fRadius + vCenter.x;
				pPos->y = y		* fRadius + vCenter.y;
				pPos->z = z * r	* fRadius + vCenter.z;
				++pPos;

				if( bHaveNormal )
				{
					pNormal->x = x;
					pNormal->y = y;
					pNormal->z = z;
					pNormal->Normalize();
					++pNormal;
				}

				if( bHaveUV )
				{
					*pUV++ = 1.0f - (float)seg / (float)nSegment;
					*pUV++ = (float)ring / (float)nRing;
				}

				if( ring != nRing )
				{
					*pIndex++ = wIndex;
					*pIndex++ = wIndex + (WORD)( nSegment + 1 );
					++wIndex;
				}
			}
		}

		//解锁索引缓存
		mpIndexBuffer->Unlock();

		//解锁顶点缓存
		for( UINT i=0; i<mNumVertexBuffer; ++i )
			mppVertexBuffer[i]->Unlock();

		//设置包围盒
		mBoundBox.SetMaxMin( fRadius, fRadius, fRadius, -fRadius, -fRadius, -fRadius );

		//设置渲染参数
		AllocRenderParamBuf();
		mpRenderParam->eDrawType	= RenderParam::DT_Basic;

		mpRenderParam->sBasicParam.ePrimType	= PT_TRIANGLE_STRIP;
		mpRenderParam->sBasicParam.nVerOffset	= 0;
		mpRenderParam->sBasicParam.nMinIndex	= 0;
		mpRenderParam->sBasicParam.nIndexOffset	= 0;
		mpRenderParam->sBasicParam.nVerCount	= nNumVertex;
		mpRenderParam->sBasicParam.nPrimCount	= nNumIndex - 2;

		//设置材质
		if( pMaterial != NULL )
		{
			AllocMaterialBuf();
			mppMaterial[0] = pMaterial;
			mpRenderParam->pMaterial = pMaterial;
			pMaterial->AddRef();
		}
		else
		{
			mpRenderParam->pMaterial = NULL;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置关联骨头索引数据
	void TriMesh::SetBoneIndex( UINT* pBoneIndex, UINT nNumBone )
	{
		mNumBone = nNumBone;

		SafeFree( mpBoneIndex );

		//复制索引数据
		size_t BufLen = sizeof(UINT) * nNumBone;
		mpBoneIndex = (UINT*)malloc( BufLen );
		if( mpBoneIndex == NULL )
			Except( Exception::ERR_NOT_ENOUGH_MEM, "分配网格蒙皮骨头索引数据缓存失败。" );

		memcpy( mpBoneIndex, pBoneIndex, BufLen );
	}
	//--------------------------------------------------------------------------------------------------------------
	//关联骨骼
	void TriMesh::SetSkeleton( Skeleton* pSkeleton )
	{
		//如果使用蒙皮骨头数量超过了指定骨骼的骨头数量
		if( mNumBone > pSkeleton->mNumBones )
			Except( Exception::ERR_INVALIDPARAMS, "网格的蒙皮骨头数量和指定骨骼的骨头数量不匹配，无法关联。" );

		mpSkeleton = pSkeleton;
	}
	//--------------------------------------------------------------------------------------------------------------
	//渲染物体
	void TriMesh::_RenderObj( const RenderParam* pParam, const Matrix4* pNodeMatrix )
	{
		//设置世界矩阵
		RenderSystem::mpSingleton->_SetWorldMatrix( pNodeMatrix );

		//如果需要软件顶点处理则打开它
		if( mbSoftVertexProc )
			RenderSystem::mpSingleton->_SetSoftwareVertexProcess( true );

		//根据渲染参数渲染
		pParam->RenderByParam();

		if( mbSoftVertexProc )
			RenderSystem::mpSingleton->_SetSoftwareVertexProcess( false );
	}
	//--------------------------------------------------------------------------------------------------------------
	//渲染动画物体
	void TriMesh::_RenderAnimObj( const RenderParam* pParam, const AnimControl* pAnimControl, const Matrix4* pNodeMatrix )
	{
		//设置动画矩阵

		//渲染矩阵临时变量
		static Matrix4 TmpRenderMat;

		//获取骨骼骨头矩阵列表
		Matrix4Stack** ppMatStack = mpSkeleton->mBoneMatList.Begin();

		for( UINT i=0; i<mNumBone; ++i )
		{
			//获取当前帧当前骨头的矩阵
			Matrix4* pBoneMat = ppMatStack[ mpBoneIndex[i] ]->Begin() + pAnimControl->mCurKey;

			//将它和场景节点矩阵级联
			Mat4TransMat4( &TmpRenderMat, pBoneMat, pNodeMatrix );

			//设置矩阵
			RenderSystem::mpSingleton->_SetWorldMatrix( &TmpRenderMat, i );
		}


		//如果需要软件顶点处理则打开它
		if( mbSoftVertexProc )
			RenderSystem::mpSingleton->_SetSoftwareVertexProcess( true );

		//如果是顶点混合蒙皮动画
		if( mNumBlendMatrix > 0 )
		{
			//渲染蒙皮动画
			RenderSystem::mpSingleton->_SetIndexVertexBlend( mNumBlendMatrix );
			pParam->RenderByParam();
			RenderSystem::mpSingleton->_SetIndexVertexBlend( 0 );
		}
		else
		{
			//根据渲染参数渲染
			pParam->RenderByParam();
		}

		if( mbSoftVertexProc )
			RenderSystem::mpSingleton->_SetSoftwareVertexProcess( false );
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
