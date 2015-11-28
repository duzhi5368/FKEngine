/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	QuadDataFormat
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	四叉树数据结构
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------

	//指定包围空间数据
	struct QuadBoundary
	{
		float fMinX;
		float fMaxX;
		float fMinZ;
		float fMaxZ;
		float fSize;
	};

	//四叉树节点数据
	struct QuadNodeData
	{
		float		fMinX;				//包围空间 X 最小值
		float		fMaxX;				//包围空间 X 最大值
		float		fMinZ;				//包围空间 Z 最小值
		float		fMaxZ;				//包围空间 Z 最大值
		float		fHalfSize;			//包围空间半尺寸

		UINT		ppChildIndex[2][2];	//子节点索引
	};
	typedef Stack< QuadNodeData >	QuadNodeDataStack;

	//四叉树数据
	struct QuadtreeData
	{
		float		fRootSize;			//根节点尺寸
		float		fLeafSize;			//叶节点尺寸

		float		fBoundaryMinX;		//包围空间 X 最小值
		float		fBoundaryMaxX;		//包围空间 X 最大值
		float		fBoundaryMinZ;		//包围空间 Z 最小值
		float		fBoundaryMaxZ;		//包围空间 Z 最大值

		float		fBoundaryMinY;		//包围空间 Y 最小值
		float		fBoundaryMaxY;		//包围空间 Y 最大值

		UINT		nNumNode;			//四叉树节点数量
	};

	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
