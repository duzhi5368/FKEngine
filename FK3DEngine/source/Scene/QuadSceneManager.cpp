/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	QuadSceneManager
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
	QuadSceneManager::QuadSceneManager()
		: mRootSize			(0.0f)
		, mLeafSize			(0.0f)
		, mBoundaryMinX		(0.0f)
		, mBoundaryMaxX		(0.0f)
		, mBoundaryMinZ		(0.0f)
		, mBoundaryMaxZ		(0.0f)
		, mBoundaryMinY		(0.0f)
		, mBoundaryMaxY		(0.0f)
		, mpQuadNode		(NULL)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	QuadSceneManager::~QuadSceneManager()
	{
		SafeDeleteArray( mpQuadNode );
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建场景根节点
	SceneNode* QuadSceneManager::CreateSceneRootNode()
	{
		mpRootNode = new QuadSceneNode;
		return mpRootNode;
	}
	//--------------------------------------------------------------------------------------------------------------
	//载入四叉树场景文件
	void QuadSceneManager::LoadFromFile( FileObject* pFile )
	{
		//读取四叉树数据
		QuadtreeData sQuadtreeData;
		pFile->Read( &sQuadtreeData, sizeof(QuadtreeData) );

		mRootSize		= sQuadtreeData.fRootSize;
		mLeafSize		= sQuadtreeData.fLeafSize;

		mBoundaryMinX	= sQuadtreeData.fBoundaryMinX;
		mBoundaryMaxX	= sQuadtreeData.fBoundaryMaxX;
		mBoundaryMinZ	= sQuadtreeData.fBoundaryMinZ;
		mBoundaryMaxZ	= sQuadtreeData.fBoundaryMaxZ;

		mBoundaryMinY	= sQuadtreeData.fBoundaryMinY;
		mBoundaryMaxY	= sQuadtreeData.fBoundaryMaxY;

		mNumNode		= sQuadtreeData.nNumNode;

		//读取四叉树节点数据
		mpQuadNode = new QuadNode [mNumNode];

		const UINT nNumNodeDataBuf = 0x1000;
		QuadNodeData* pQuadNodeDataBuf = (QuadNodeData*)malloc( sizeof(QuadNodeData) * nNumNodeDataBuf );

		QuadNode* pQuadNode = mpQuadNode;
		UINT nRemainNode = mNumNode;

		while( nRemainNode != 0 )
		{
			UINT nNumReadNodeData = ( nRemainNode < nNumNodeDataBuf ) ? nRemainNode : nNumNodeDataBuf;
			pFile->Read( pQuadNodeDataBuf, sizeof(QuadNodeData) * nNumReadNodeData );

			QuadNodeData* pQuadNodeData = pQuadNodeDataBuf;

			for( UINT i=0; i<nNumReadNodeData; ++i )
			{
				pQuadNode->_SetQuadNode( pQuadNodeData, mpQuadNode );
				++pQuadNodeData;
				++pQuadNode;
			}

			nRemainNode -= nNumReadNodeData;
		}

		free( pQuadNodeDataBuf );
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取与指定包围空间相符的四叉树节点
	QuadNode* QuadSceneManager::GetQuadNodeFromBoundary( const AxisBox3& refAxisBox3 )
	{
		if( refAxisBox3.mMinVer.y < mBoundaryMinY || refAxisBox3.mMaxVer.y > mBoundaryMaxY )
			return mpQuadNode;

		return GetQuadNodeFromBoundary(
			refAxisBox3.mMinVer.x, refAxisBox3.mMaxVer.x,
			refAxisBox3.mMinVer.z, refAxisBox3.mMaxVer.z );
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取与指定包围空间相符的四叉树节点
	QuadNode* QuadSceneManager::GetQuadNodeFromBoundary( float fMinX, float fMaxX, float fMinZ, float fMaxZ )
	{
		//如果指定包围空间在场景最大包围空间之外则直接返回根节点
		if( fMinX < mBoundaryMinX || fMaxX > mBoundaryMaxX || fMinZ < mBoundaryMinZ || fMaxZ > mBoundaryMaxZ )
			return mpQuadNode;

		//计算包围空间数据
		static QuadBoundary sBoundary;

		sBoundary.fMinX = fMinX;
		sBoundary.fMaxX = fMaxX;
		sBoundary.fMinZ = fMinZ;
		sBoundary.fMaxZ = fMaxZ;

		const float fSizeX = fMaxX - fMinX;
		const float fSizeZ = fMaxZ - fMinZ;

		sBoundary.fSize = FK_MAX( fSizeX, fSizeZ );

		return mpQuadNode->_GetQuadNodeFromBoundary( sBoundary );
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取可见渲染实例
	void QuadSceneManager::_GetVisableInstance( Camera* pActiveCam )
	{
		//预先设置好场景包围空间的 Y 值
		static Vector3 pBoundaryVer[8];

		pBoundaryVer[0].y = mBoundaryMinY;
		pBoundaryVer[1].y = mBoundaryMinY;
		pBoundaryVer[2].y = mBoundaryMinY;
		pBoundaryVer[3].y = mBoundaryMinY;
		pBoundaryVer[4].y = mBoundaryMaxY;
		pBoundaryVer[5].y = mBoundaryMaxY;
		pBoundaryVer[6].y = mBoundaryMaxY;
		pBoundaryVer[7].y = mBoundaryMaxY;

		//遍历四叉树节点
		mpQuadNode->_GetVisableInstance( pActiveCam, &mRenderQueue, pBoundaryVer );
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
