/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	QuadGenerator
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../../include/Scene/SceneInsideHeader.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	QuadGenerator::QuadGenerator()
		: mRootSize			(0.0f)
		, mLeafSize			(0.0f)
		, mBoundaryMinX		(0.0f)
		, mBoundaryMaxX		(0.0f)
		, mBoundaryMinZ		(0.0f)
		, mBoundaryMaxZ		(0.0f)
		, mBoundaryMinY		(0.0f)
		, mBoundaryMaxY		(0.0f)
		, mNumNode			(0)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	QuadGenerator::~QuadGenerator()
	{
		DestroyQuadScene();
	}
	//--------------------------------------------------------------------------------------------------------------
	//生成非稀疏元素四叉树场景
	void QuadGenerator::GenerateThickQuadScene( float fMidX, float fMidZ, float fMinY, float fMaxY, float fLeafSize, UINT nDepth )
	{
		mNumNode = _SetSceneBoundary( fMidX, fMidZ, fMinY, fMaxY, fLeafSize, nDepth );

		mNodeDataStack.Initialize( mNumNode, 100 );

		//递归创建所有四叉树节点
		_CreateNode( fMidX, fMidZ, mRootSize, nDepth );
	}
	//--------------------------------------------------------------------------------------------------------------
	//生成稀疏元素四叉树场景
	void QuadGenerator::GenerateLooseQuadScene( float fMidX, float fMidZ, float fMinY, float fMaxY, float fLeafSize, UINT nDepth )
	{
		UINT nNumNode = _SetSceneBoundary( fMidX, fMidZ, fMinY, fMaxY, fLeafSize, nDepth );

		mNodeDataStack.Initialize( nNumNode, 100 );

		//创建根节点
		_CreateNode( fMidX, fMidZ, mRootSize, 0 );

		mNumNode = 1;
	}
	//--------------------------------------------------------------------------------------------------------------
	//根据指定包围空间生成相符的稀疏元素四叉树节点
	UINT QuadGenerator::GenerateNodeFromBoundary( const AxisBox3& refAxisBox3 )
	{
		return GenerateNodeFromBoundary(
			refAxisBox3.mMinVer.x, refAxisBox3.mMaxVer.x, refAxisBox3.mMinVer.z, refAxisBox3.mMaxVer.z );
	}
	//--------------------------------------------------------------------------------------------------------------
	//根据指定包围空间生成相符的稀疏元素四叉树节点
	UINT QuadGenerator::GenerateNodeFromBoundary( float fMinX, float fMaxX, float fMinZ, float fMaxZ )
	{
		//如果指定包围空间超过了场景包围空间则返回根节点
		if( fMinX < mBoundaryMinX || fMaxX > mBoundaryMaxX || fMinZ < mBoundaryMinZ || fMaxZ > mBoundaryMaxZ )
			return 0;

		//计算包围空间数据
		QuadBoundary sBoundary;

		sBoundary.fMinX = fMinX;
		sBoundary.fMaxX = fMaxX;
		sBoundary.fMinZ = fMinZ;
		sBoundary.fMaxZ = fMaxZ;

		const float fSizeX = fMaxX - fMinX;
		const float fSizeZ = fMaxZ - fMinZ;

		sBoundary.fSize = FK_MAX( fSizeX, fSizeZ );

		UINT nIndex = _CreateNodeFromBoundary( sBoundary, 0 );

		//更新节点数量
		mNumNode = mNodeDataStack.Size();

		return nIndex;
	}
	//--------------------------------------------------------------------------------------------------------------
	//销毁四叉树场景
	void QuadGenerator::DestroyQuadScene()
	{
		mRootSize		= 0.0f;
		mLeafSize		= 0.0f;
		mBoundaryMinX	= 0.0f;
		mBoundaryMaxX	= 0.0f;
		mBoundaryMinZ	= 0.0f;
		mBoundaryMaxZ	= 0.0f;
		mNumNode		= 0.0f;

		mNodeDataStack.Release();
	}
	//--------------------------------------------------------------------------------------------------------------
	//将四叉树场景数据写入文件
	void QuadGenerator::WriteToFile( FileObject* pFile )
	{
		if( mNumNode == 0 )
			Except(Exception::ERR_INVALIDPARAMS, "不存在已生成的四叉树场景数据可以写入文件。" );

		//写入四叉树数据
		QuadtreeData sQuadtreeData;

		sQuadtreeData.fRootSize		= mRootSize;
		sQuadtreeData.fLeafSize		= mLeafSize;

		sQuadtreeData.fBoundaryMinX	= mBoundaryMinX;
		sQuadtreeData.fBoundaryMaxX	= mBoundaryMaxX;
		sQuadtreeData.fBoundaryMinZ	= mBoundaryMinZ;
		sQuadtreeData.fBoundaryMaxZ	= mBoundaryMaxZ;

		sQuadtreeData.fBoundaryMinY = mBoundaryMinY;
		sQuadtreeData.fBoundaryMaxY = mBoundaryMaxY;

		sQuadtreeData.nNumNode		= mNumNode;

		pFile->Write( &sQuadtreeData, sizeof(QuadtreeData) );

		//写入所有四叉树节点数据
		pFile->Write( mNodeDataStack.Begin(), sizeof(QuadNodeData) * mNumNode );
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置四叉树场景包围空间
	UINT QuadGenerator::_SetSceneBoundary( float fMidX, float fMidZ, float fMinY, float fMaxY, float fLeafSize, UINT nDepth )
	{
		if( nDepth > mMaxDepth )
		{
			Except( Exception::ERR_INVALIDPARAMS,
			Misc::FormatString( NULL, "指定的四叉树深度值（%d）超过了允许的最大深度值（%d）。", nDepth, mMaxDepth ) );
		}

		//计算根节点尺寸
		mLeafSize = fLeafSize;

		UINT nSegment = pow( static_cast< float >( 2 ), static_cast< float >( nDepth ) );
		mRootSize = fLeafSize * (float)nSegment;

		//计算最大包围空间数据
		const float fHalfSize = mRootSize / 2.0f;

		mBoundaryMinX = fMidX - fHalfSize;
		mBoundaryMaxX = fMidX + fHalfSize;
		mBoundaryMinZ = fMidZ - fHalfSize;
		mBoundaryMaxZ = fMidZ + fHalfSize;

		mBoundaryMinY = fMinY;
		mBoundaryMaxY = fMaxY;

		//计算节点数量
		UINT nNumNode = 1;
		UINT nCount = 1;
		for( UINT i=0; i<nDepth; ++i )
		{
			nCount *= 4;
			nNumNode += nCount;
		}

		return nNumNode;
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建四叉树节点
	UINT QuadGenerator::_CreateNode( float fMidX, float fMidZ, float fHalfSize, UINT nDepth )
	{
		UINT nIndex = mNodeDataStack.Size();
		QuadNodeData* pNodeData = mNodeDataStack.Push();

		//设置当前结点数据
		pNodeData->fMinX = fMidX - fHalfSize;
		pNodeData->fMaxX = fMidX + fHalfSize;
		pNodeData->fMinZ = fMidZ - fHalfSize;
		pNodeData->fMaxZ = fMidZ + fHalfSize;

		pNodeData->fHalfSize = fHalfSize;

		//如果未到达指定深度则创建子节点
		if( nDepth != 0 )
		{
			const UINT nChildDepth = nDepth - 1;
			const float fHalfChildSize = fHalfSize / 2.0f;

			float pMidX[2];
			float pMidZ[2];

			pMidX[0] = fMidX - fHalfChildSize;
			pMidX[1] = fMidX + fHalfChildSize;
			pMidZ[0] = fMidZ - fHalfChildSize;
			pMidZ[1] = fMidZ + fHalfChildSize;

			for( UINT i=0; i<2; ++i )
				for( UINT j=0; j<2; ++j )
					pNodeData->ppChildIndex[i][j] = _CreateNode( pMidX[i], pMidZ[j], fHalfChildSize, nChildDepth );
		}
		else
		{
			pNodeData->ppChildIndex[0][0] = 0;
			pNodeData->ppChildIndex[1][0] = 0;
			pNodeData->ppChildIndex[0][1] = 0;
			pNodeData->ppChildIndex[1][1] = 0;
		}

		return nIndex;
	}
	//--------------------------------------------------------------------------------------------------------------
	//根据指定包围空间创建四叉树节点或返回对应的四叉树节点
	UINT QuadGenerator::_CreateNodeFromBoundary( const QuadBoundary& sBoundary, UINT nNodeIndex )
	{
		QuadNodeData* pNodeData = mNodeDataStack.Begin() + nNodeIndex;

		const float fChildSize = pNodeData->fHalfSize;

		//如果指定包围空间尺寸小于当前节点的子节点的尺寸并且小于叶节点的尺寸
		if( sBoundary.fSize <= fChildSize && sBoundary.fSize >= mLeafSize )
		{
			//获取指定包围空间在该节点空间内的位置
			AxisSpaceRelation eRelationX;
			AxisSpaceRelation eRelationZ;

			//如果指定包围空间没有完全在其中任何一个子节点的空间中则直接返回当前节点
			const float fMidX = pNodeData->fMinX + fChildSize;
			const float fMidZ = pNodeData->fMinZ + fChildSize;

			if( sBoundary.fMinX > fMidX )
				eRelationX = ASR_More;
			else if( sBoundary.fMaxX < fMidX )
				eRelationX = ASR_Less;
			else
				return nNodeIndex;

			if( sBoundary.fMinZ > fMidZ )
				eRelationZ = ASR_More;
			else if( sBoundary.fMaxX < fMidZ )
				eRelationZ = ASR_Less;
			else
				return nNodeIndex;

			//如果指定包围空间对应的子节点不存在
			UINT& nChildIndex = pNodeData->ppChildIndex[eRelationX][eRelationZ];
			if( nChildIndex == 0 )
			{
				//创建指定包围空间对应的子节点
				const float fHalfChildSize = fChildSize / 2.0f;

				float pMidX[2];
				float pMidZ[2];

				pMidX[0] = fMidX - fHalfChildSize;
				pMidX[1] = fMidX + fHalfChildSize;
				pMidZ[0] = fMidZ - fHalfChildSize;
				pMidZ[1] = fMidZ + fHalfChildSize;

				nChildIndex = _CreateNode( pMidX[eRelationX], pMidZ[eRelationZ], fHalfChildSize, 0 );
			}

			//继续递归创建子节点
			return _CreateNodeFromBoundary( sBoundary, nChildIndex );
		}

		return nNodeIndex;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
