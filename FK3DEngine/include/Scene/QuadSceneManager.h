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
*	文件说明：	四叉树场景管理器
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//四叉树场景管理器
	//
	//     当场景节点进行创建删除操作时，或场景中存在可移动物体时，即四叉树场景节点发生空间变换时，都会引起四叉树所
	// 挂接的场景节点发生变化。此时，场景节点将会从预先挂接的四叉树节点上断开，并且自动挂接到对应的新的四叉树节点上，
	// 而不会进行任何的四叉树节点动态创建、删除操作。这虽然可以提高性能，但是会造成某些四叉树节点没有挂接任何渲染实
	// 体的情况出现，因此，为了达到较好的拣选效率，用户必须预先创建并加载已根据场景优化过的四叉树。
	//
	//     更多信息请参考 QuadGenerator.h 文件中的四叉树场景生成器说明。
	//
	//--------------------------------------------------------------------------------------------------------------
	class FK_DLL QuadSceneManager : public SceneManager
	{
	public:
		friend class QuadNode;
		friend class QuadSceneNode;

	protected:
		float			mRootSize;			//根节点尺寸
		float			mLeafSize;			//叶节点尺寸

		float			mBoundaryMinX;		//包围空间 X 最小值
		float			mBoundaryMaxX;		//包围空间 X 最大值
		float			mBoundaryMinZ;		//包围空间 Z 最小值
		float			mBoundaryMaxZ;		//包围空间 Z 最大值

		float			mBoundaryMinY;		//包围空间 Y 最小值
		float			mBoundaryMaxY;		//包围空间 Y 最大值


		UINT			mNumNode;			//四叉树节点数量

		QuadNode*		mpQuadNode;			//四叉树节点数组

	public:
		QuadSceneManager();
		~QuadSceneManager();

		//创建场景根节点
		SceneNode* CreateSceneRootNode();

		//载入四叉树场景文件
		void LoadFromFile( FileObject* pFile );

		//获取与指定包围空间相符的四叉树节点
		QuadNode* GetQuadNodeFromBoundary( const AxisBox3& refAxisBox3 );

		//获取与指定包围空间相符的四叉树节点
		QuadNode* GetQuadNodeFromBoundary( float fMinX, float fMaxX, float fMinZ, float fMaxZ );

	protected:
		//获取可见渲染实例
		void _GetVisableInstance( Camera* pActiveCam );
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
