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
*	文件说明：	四叉树场景生成器
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//四叉树场景生成器
	//
	//     该四叉树可以是稀疏元素四叉树，即每个四叉树节点并不一定存在所有的四个子节点。当场景中实体分布不是很均匀时，
	// 稀疏元素四叉树可以很好得提高四叉树的遍历效率。但是，当场景中的可移动实体较多时，使用稀疏元素四叉树则可能会造
	// 成某些部位拣选精度的降低，因此，在这种情况下使用非稀疏元素四叉树可能会更好。
	//
	//     如果要生成非稀疏元素的四叉树，可通过设置根节点的尺寸和四叉树节点深度来完成四叉树的创建；如果要生成稀疏元
	// 素的四叉树，首先也需要设置叶节点的尺寸和四叉树节点深度，其后，再通过逐一手动指定可能的渲染实体包围空间来自动
	// 生成合适的四叉树节点。（在指定的包围空间不小于叶节点最小尺寸的前题下，程序会尽可能得生成更小、深度更高的四叉
	// 树叶节点。）
	//
	//--------------------------------------------------------------------------------------------------------------
	class FK_DLL QuadGenerator
	{
	public:
		static const UINT	mMaxDepth = 8;		//四叉树最大深度

	protected:
		float				mRootSize;			//根节点尺寸
		float				mLeafSize;			//叶节点尺寸

		float				mBoundaryMinX;		//包围空间 X 最小值
		float				mBoundaryMaxX;		//包围空间 X 最大值
		float				mBoundaryMinZ;		//包围空间 Z 最小值
		float				mBoundaryMaxZ;		//包围空间 Z 最大值

		float				mBoundaryMinY;		//包围空间 Y 最小值
		float				mBoundaryMaxY;		//包围空间 Y 最大值

		UINT				mNumNode;			//四叉树节点数量

		QuadNodeDataStack	mNodeDataStack;		//四叉树节点数据堆栈

	public:
		QuadGenerator();
		~QuadGenerator();

		//生成非稀疏元素四叉树场景
		void GenerateThickQuadScene( float fMidX, float fMidZ, float fMinY, float fMaxY, float fLeafSize, UINT nDepth );

		//生成稀疏元素四叉树场景
		void GenerateLooseQuadScene( float fMidX, float fMidZ, float fMinY, float fMaxY, float fLeafSize, UINT nDepth );

		//根据指定包围空间生成相符的稀疏元素四叉树节点
		UINT GenerateNodeFromBoundary( const AxisBox3& refAxisBox3 );

		//根据指定包围空间生成相符的稀疏元素四叉树节点
		UINT GenerateNodeFromBoundary( float fMinX, float fMaxX, float fMinZ, float fMaxZ );

		//销毁四叉树场景
		void DestroyQuadScene();

		//将四叉树场景数据写入文件
		void WriteToFile( FileObject* pFile );

	protected:
		//设置四叉树场景包围空间
		UINT _SetSceneBoundary( float fMidX, float fMidZ, float fMinY, float fMaxY, float fLeafSize, UINT nDepth );

		//创建四叉树节点
		UINT _CreateNode( float fMidX, float fMidZ, float fHalfSize, UINT nDepth );

		//根据指定包围空间创建四叉树节点或返回对应的四叉树节点
		UINT _CreateNodeFromBoundary( const QuadBoundary& sBoundary, UINT nNodeIndex );
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
