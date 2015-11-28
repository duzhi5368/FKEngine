/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	BSPGenerator
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	二叉树场景生成器
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================
#include "../RenderWrapper/ObjectFileFormat.h"
//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//二叉树场景生成器
	class FK_DLL BSPGenerator
	{
	protected:
		BSPPlane*		mpPlane3Buf;
		Vector3*		mpVertexBuf;

		UINT			mMaxFace;
		UINT			mMaxVertex;

		UINT			mNumFace;
		UINT			mNumVertex;

		UINT			mNumNode;

	public:
		//生成二叉树场景
		void GenerateBSPScene( const Vector3* pVertex, const FaceIndex* pFaceIndex, UINT nNumVertex,
			UINT nNumFace, UINT nMaxVertex, UINT nMaxFace );

	protected:
		//生成二叉树
		void _GenerateBSPTree( BSPNode* pBSPNode, const BSPPlaneList* pBSPPlaneList );

		//选择初始分割面
		BSPPlane* _ChooseDivPlane3( const BSPPlaneList* pBSPPlaneList );

		//判断几何体是否为凸多边形
		bool _IsConvexSet( const BSPPlaneList* pBSPPlaneList );

		//分割二叉树分割面
		void _SplitPlane3( BSPPlane* pPlane3, BSPPlane* pDivPlane3,
			BSPPlaneList* pFrontPlane3List, BSPPlaneList* pBackPlane3List );

		//建立新二叉树分割面
		void _BuildNewPlane3( WORD pVerIndex[4], int nNumVer, BSPPlaneList* pNewPlane3List );
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
