/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	QuadNode
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
	QuadNode::QuadNode()
	{
		mSceneNodeList.Initialize( 10, 10 );
	}
	//--------------------------------------------------------------------------------------------------------------
	QuadNode::~QuadNode()
	{
		_DetachAllQuadSceneNode();
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置四叉树节点
	void FKFastcall QuadNode::_SetQuadNode( const QuadNodeData* pQuadNodeData, QuadNode* pQuadNode )
	{
		mfMinX		= pQuadNodeData->fMinX;
		mfMaxX		= pQuadNodeData->fMaxX;
		mfMinZ		= pQuadNodeData->fMinZ;
		mfMaxZ		= pQuadNodeData->fMaxZ;

		mfHalfSize	= pQuadNodeData->fHalfSize;

		for( UINT i=0; i<2; ++i )
		{
			for( UINT j=0; j<2; ++j )
			{
				mppChild[i][j] = ( pQuadNodeData->ppChildIndex[i][j] != 0 ) ?
					( pQuadNode + pQuadNodeData->ppChildIndex[i][j] ) : NULL;
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//挂接指定四叉树场景节点
	void FKFastcall QuadNode::_AttachQuadSceneNode( QuadSceneNode* pQuadSceneNode )
	{
		*mSceneNodeList.Push() = pQuadSceneNode;
	}
	//--------------------------------------------------------------------------------------------------------------
	//断开指定的四叉树场景节点
	void FKFastcall QuadNode::_DetachQuadSceneNode( QuadSceneNode* pQuadSceneNode )
	{
		QuadSceneNodeList::Iterator it = mSceneNodeList.Begin();
		QuadSceneNodeList::Iterator end = mSceneNodeList.End();
		for(; it!=end; ++it )
		{
			if( *it == pQuadSceneNode )
			{
				//将该场景节点断开
				mSceneNodeList.Erase( it );
				pQuadSceneNode->mpQuadNode = NULL;
				return;
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//断开所有的四叉树场景节点
	void FKFastcall QuadNode::_DetachAllQuadSceneNode()
	{
		QuadSceneNodeList::Iterator it = mSceneNodeList.Begin();
		QuadSceneNodeList::Iterator end = mSceneNodeList.End();
		for(; it!=end; ++it )
		{
			if( *it != NULL )
				(*it)->mpQuadNode = NULL;
		}

		mSceneNodeList.Clear();
	}
	//--------------------------------------------------------------------------------------------------------------
	//判断指定的包围盒是否在节点的包围空间中
	bool FKFastcall QuadNode::_IsInside( const AxisBox3& refBox3 )
	{
		const float fMinY = ( (QuadSceneManager*)SceneManager::mpSingleton )->mBoundaryMinY;
		const float fMaxY = ( (QuadSceneManager*)SceneManager::mpSingleton )->mBoundaryMaxY;

		if( refBox3.mMinVer.x < mfMinX || refBox3.mMaxVer.x > mfMaxX ||
			refBox3.mMinVer.z < mfMinZ || refBox3.mMaxVer.z > mfMaxZ ||
			refBox3.mMinVer.y < fMinY || refBox3.mMaxVer.y > fMaxY )
			return false;

		return true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取与指定包围空间相符的四叉树节点
	QuadNode* QuadNode::_GetQuadNodeFromBoundary( const QuadBoundary& sBoundary )
	{
		//如果指定包围空间尺寸小于当前节点的子节点的尺寸
		if( sBoundary.fSize <= mfHalfSize )
		{
			//获取指定包围空间在该节点空间内的位置
			AxisSpaceRelation eRelationX;
			AxisSpaceRelation eRelationZ;

			//如果指定包围空间没有完全在其中任何一个子节点的空间中则直接返回当前节点
			const float fMidX = mfMinX + mfHalfSize;
			const float fMidZ = mfMinZ + mfHalfSize;

			if( sBoundary.fMinX > fMidX )
				eRelationX = ASR_More;
			else if( sBoundary.fMaxX < fMidX )
				eRelationX = ASR_Less;
			else
				return this;

			if( sBoundary.fMinZ > fMidZ )
				eRelationZ = ASR_More;
			else if( sBoundary.fMaxX < fMidZ )
				eRelationZ = ASR_Less;
			else
				return this;

			//如果指定包围空间对应的子节点存在
			QuadNode* pQuadNode = mppChild[eRelationX][eRelationZ];
			if( pQuadNode != NULL )
				return pQuadNode->_GetQuadNodeFromBoundary( sBoundary );
		}

		//如果不存在包围空间对应的叶节点则返回该节点
		return this;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取该节点及其子节点下的可见渲染实例
	void QuadNode::_GetVisableInstance( Camera* pActiveCam, RenderQueue* pRenderQueue, Vector3 pBoundaryVer[8] )
	{
		//设置四叉树节点包围空间
		pBoundaryVer[0].x = mfMinX;		pBoundaryVer[0].z = mfMinZ;
		pBoundaryVer[1].x = mfMaxX;		pBoundaryVer[1].z = mfMinZ;
		pBoundaryVer[2].x = mfMinX;		pBoundaryVer[2].z = mfMaxZ;
		pBoundaryVer[3].x = mfMaxX;		pBoundaryVer[3].z = mfMaxZ;

		pBoundaryVer[4].x = mfMinX;		pBoundaryVer[4].z = mfMinZ;
		pBoundaryVer[5].x = mfMaxX;		pBoundaryVer[5].z = mfMinZ;
		pBoundaryVer[6].x = mfMinX;		pBoundaryVer[6].z = mfMaxZ;
		pBoundaryVer[7].x = mfMaxX;		pBoundaryVer[7].z = mfMaxZ;

		//检测该四叉树节点在视体中是否可见
		if( pActiveCam->mViewFrustum.IsIntersect( pBoundaryVer ) )
		{
			//将其下挂接的四叉树场景节点中的渲染实例加入队列
			QuadSceneNodeList::Iterator it = mSceneNodeList.Begin();
			QuadSceneNodeList::Iterator end = mSceneNodeList.End();
			for(; it!=end; ++it )
				(*it)->_AddInstanceToQueue( pRenderQueue );

			//递归检测该节点的四个子结点
			for( UINT i=0; i<2; ++i )
				for( UINT j=0; j<2; ++j )
					if( mppChild[i][j] != NULL )
						mppChild[i][j]->_GetVisableInstance( pActiveCam, pRenderQueue, pBoundaryVer );
		}
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
