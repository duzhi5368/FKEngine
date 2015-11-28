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
*	文件说明：	四叉树节点类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//四叉树节点类
	class FK_DLL QuadNode
	{
	public:
		friend class QuadSceneNode;
		friend class QuadSceneManager;

	public:
		float				mfMinX;			//包围空间 X 最小值
		float				mfMaxX;			//包围空间 X 最大值
		float				mfMinZ;			//包围空间 Z 最小值
		float				mfMaxZ;			//包围空间 Z 最大值

		float				mfHalfSize;		//包围空间半尺寸

		QuadNode*			mppChild[2][2];	//子节点

		QuadSceneNodeList	mSceneNodeList;	//挂接四叉树场景节点列表

	public:
		QuadNode();
		~QuadNode();

	protected:
		//设置四叉树节点
		void FKFastcall _SetQuadNode( const QuadNodeData* pQuadNodeData, QuadNode* pQuadNode );

		//挂接指定四叉树场景节点
		void FKFastcall _AttachQuadSceneNode( QuadSceneNode* pQuadSceneNode );

		//断开指定的四叉树场景节点
		void FKFastcall _DetachQuadSceneNode( QuadSceneNode* pQuadSceneNode );

		//断开所有的四叉树场景节点
		void FKFastcall _DetachAllQuadSceneNode();

		//判断指定的包围盒是否在节点的包围空间中
		bool FKFastcall _IsInside( const AxisBox3& refBox3 );

		//获取与指定包围空间相符的四叉树节点
		QuadNode* _GetQuadNodeFromBoundary( const QuadBoundary& sBoundary );

		//获取该节点及其子节点下的可见渲染实例
		void _GetVisableInstance( Camera* pActiveCam, RenderQueue* pRenderQueue, Vector3 pBoundaryVer[8] );
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
