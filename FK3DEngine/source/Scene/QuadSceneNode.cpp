/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	QuadSceneNode
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
	QuadSceneNode::QuadSceneNode()
		: mpQuadNode	(NULL)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	QuadSceneNode::~QuadSceneNode()
	{
		if( mpQuadNode != NULL )
			mpQuadNode->_DetachQuadSceneNode( this );
	}
	//--------------------------------------------------------------------------------------------------------------
	//节点创建函数
	TransformNode* FKFastcall QuadSceneNode::_NodeCreator()
	{
		return new QuadSceneNode;
	}
	//--------------------------------------------------------------------------------------------------------------
	//通知包围盒已更新
	void FKFastcall QuadSceneNode::_NotifyBoxUpdated()
	{
		//如果该节点没有挂接任何渲染实例则直接返回
		if( mRenderInstList.Size() == 0 )
			return;

		QuadSceneManager* pQuadSceneManager = (QuadSceneManager*)SceneManager::mpSingleton;

		//如果已经挂接在某个四叉树节点上
		if( mpQuadNode != NULL )
		{
			//判断该场景节点包围盒是否还在原先的四叉树节点之内
			if( mpQuadNode->_IsInside( mWorldBoundBox ) )
			{
				//如果还在原先节点则直接返回
				return;
			}
			//如果该场景节点包围盒已不在原先挂接的四叉树节点包围空间内
			else
			{
				//将其从原先四叉树节点上断开
				mpQuadNode->_DetachQuadSceneNode( this );
			}
		}

		//根据其包围盒将该场景节点挂接在适当的四叉树节点上
		QuadNode* pQuadNode = pQuadSceneManager->GetQuadNodeFromBoundary( mWorldBoundBox );

		pQuadNode->_AttachQuadSceneNode( this );
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
