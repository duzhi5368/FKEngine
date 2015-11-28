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
	//生成二叉树场景
	void BSPGenerator::GenerateBSPScene( const Vector3* pVertex, const FaceIndex* pFaceIndex, UINT nNumVertex,
		UINT nNumFace, UINT nMaxVertex, UINT nMaxFace )
	{
		mNumVertex		= nNumVertex;
		mNumFace		= nNumFace;
		mMaxVertex		= nMaxVertex;
		mMaxFace		= nMaxFace;

		mNumNode = 0;

		//生成顶点数据
		mpVertexBuf = (Vector3*)malloc( sizeof(Vector3) * nMaxVertex );
		memset( mpVertexBuf, 0, sizeof(Vector3) * nMaxVertex );
		memcpy( mpVertexBuf, pVertex, sizeof(Vector3) * nNumVertex );


		//生成每个三角面的二叉树分割面数据及其索引数据
		mpPlane3Buf = (BSPPlane*)malloc( sizeof(BSPPlane) * nMaxFace );
		memset( mpPlane3Buf, 0, sizeof(BSPPlane) * nMaxFace );


		BSPPlaneList sPlane3List;
		sPlane3List.Initialize( nMaxFace, nMaxFace );

		BSPPlane* pPlane3 = mpPlane3Buf;
		FaceIndex* pFace = (FaceIndex*)pFaceIndex;

		for( UINT i=0; i<nNumFace; ++i, ++pFace, ++pPlane3 )
		{
			pPlane3->SetFromPoints( mpVertexBuf, pFace->v1, pFace->v2, pFace->v3 );
			*sPlane3List.Push() = pPlane3;
		}

		//生成二叉树
		BSPNode* pRootNode = new BSPNode;
		_GenerateBSPTree( pRootNode, &sPlane3List );

		delete pRootNode;
	}
	//--------------------------------------------------------------------------------------------------------------
	//生成二叉树
	void BSPGenerator::_GenerateBSPTree( BSPNode* pBSPNode, const BSPPlaneList* pBSPPlaneList )
	{
		++mNumNode;

		//如果是凸多边形集合则生成叶节点
		if( _IsConvexSet( pBSPPlaneList ) )
		{
			//生成分割后的面片数据
			return;
		}

		//选择分割面
		BSPPlane* pDivPlane3 = _ChooseDivPlane3( pBSPPlaneList );
		pBSPNode->mpDivPlane3 = pDivPlane3;

		UINT nNumInputPlane3 = pBSPPlaneList->Size();

		BSPPlaneList sPosPlane3List;
		BSPPlaneList sNegPlane3List;
		sPosPlane3List.Initialize( nNumInputPlane3, nNumInputPlane3 );
		sNegPlane3List.Initialize( nNumInputPlane3, nNumInputPlane3 );

		//循环集合中的每个多边形
		BSPPlaneList::Iterator it = pBSPPlaneList->Begin();
		BSPPlaneList::Iterator end = pBSPPlaneList->End();
		for(; it!=end; ++it )
		{
			BSPPlane* pPlane3 = *it;

			//判断该面在分割面哪一边
			SpaceRelation eSR = pDivPlane3->GetSide( *pPlane3 );

			if( eSR == SR_POSITIVE || eSR == SR_COINCIDING )
			{
				*sPosPlane3List.Push() = pPlane3;
			}
			else if( eSR == SR_NEGATIVE )
			{
				*sNegPlane3List.Push() = pPlane3;
			}
			else
			{
				//将跨过分割面的三角形分割
				_SplitPlane3( pPlane3, pDivPlane3, &sPosPlane3List, &sNegPlane3List );
			}
		}

		if( sPosPlane3List.Size() == 0 || sNegPlane3List.Size() == 0 )
			Except( Exception::ERR_INTERNAL_ERROR, "该二叉树节点不是凸多边形集合，但是却无法正确地进行空间分割。" );

		//创建前子节点
		pBSPNode->mpPosNode = new BSPNode;
		_GenerateBSPTree( pBSPNode->mpPosNode, &sPosPlane3List );

		//创建后子节点
		pBSPNode->mpNegNode = new BSPNode;
		_GenerateBSPTree( pBSPNode->mpNegNode, &sNegPlane3List );
	}
	//--------------------------------------------------------------------------------------------------------------
	//选择初始分割面
	BSPPlane* BSPGenerator::_ChooseDivPlane3( const BSPPlaneList* pBSPPlaneList )
	{
		float fBestRelation = 0.0f;					//当前的最佳分割比值
		UINT nLeastSplits = UINT_MAX;				//当前的最小分割面数

		BSPPlaneList::Iterator itBestPlane3 = NULL;	//最佳分割面

		BSPPlaneList::Iterator begin = pBSPPlaneList->Begin();
		BSPPlaneList::Iterator end = pBSPPlaneList->End();

		//循环每一个在集合中的多边形
		for( BSPPlaneList::Iterator itA=begin; itA!=end; ++itA )
		{
			//如果该多边形在建立二叉树的过程中被当作过分割面则略过
			if( (*itA)->mbDivPlane3 )
				continue;

			//计算在该分割面正面、反面和被该面分割的多边形数量。
			UINT nNumPositive = 0;
			UINT nNumNegative = 0;
			UINT nNumSpanning = 0;

			//循环在集合中除了 itA 之外的多边形
			for( BSPPlaneList::Iterator itB=begin; itB!=end; ++itB )
			{
				if( itB == itA )
					continue;

				const BSPPlane* pPlane3A = *itA;
				const BSPPlane* pPlane3B = *itB;

				SpaceRelation eSR = pPlane3A->GetSide( *pPlane3B );
				if( eSR == SR_POSITIVE || eSR == SR_COINCIDING )
					++nNumPositive;
				else if( eSR == SR_NEGATIVE )
					++nNumNegative;
				else
					++nNumSpanning;
			}

			//计算在该分割面正面和反面的多边形数量比值（值越大分割越均匀）
			float fRelation = 0.0f;
			if( nNumPositive != 0 && nNumNegative != 0 )
			{
				fRelation = ( nNumNegative > nNumPositive ) ?
					(float)nNumPositive / (float)nNumNegative : (float)nNumNegative / (float)nNumPositive;

				//如果当前多边形分割后的子集合比值可接受，并且分割了更少的多边形，
				//那么保存当前的多边形为新的候选分割面。
				//如果当前多边形和最佳分割面一样分割了相同数量的多边形而分割后的
				//子集合比值更大的话，将当前多边形作为新的候选分割面。
				if( fRelation > fBestRelation && nNumSpanning <= nLeastSplits )
					goto SetBestPlane3;
			}
			else if( ( nNumPositive != 0 || nNumNegative != 0 ) && nNumSpanning != 0 )
			{
				if( fBestRelation == 0.0f && nNumSpanning < nLeastSplits )
					goto SetBestPlane3;
			}

			continue;

SetBestPlane3:
			itBestPlane3 = itA;
			nLeastSplits = nNumSpanning;
			fBestRelation = fRelation;
		}

		BSPPlane* pDivPlane3 = *itBestPlane3;
		pDivPlane3->mbDivPlane3 = true;
		return pDivPlane3;
	}
	//--------------------------------------------------------------------------------------------------------------
	//判断几何体是否为凸多边形
	bool BSPGenerator::_IsConvexSet( const BSPPlaneList* pBSPPlaneList )
	{
		//循环检测每个多边形判断是否其它多边形相对于该多边形都在正面
		BSPPlaneList::Iterator begin = pBSPPlaneList->Begin();
		BSPPlaneList::Iterator end = pBSPPlaneList->End();

		for( BSPPlaneList::Iterator itA=begin; itA!=end; ++itA )
		{
			for( BSPPlaneList::Iterator itB=begin; itB!=end; ++itB )
			{
				if( itA == itB )
					continue;

				const BSPPlane* pPlane3A = *itA;
				const BSPPlane* pPlane3B = *itB;

				const SpaceRelation eSR = pPlane3A->GetSide( *pPlane3B );

				if( eSR == SR_NEGATIVE || eSR == SR_SPANNING )
					return false;
			}
		}

		return true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//分割二叉树分割面
	void BSPGenerator::_SplitPlane3( BSPPlane* pPlane3, BSPPlane* pDivPlane3,
		BSPPlaneList* pFrontPlane3List, BSPPlaneList* pBackPlane3List )
	{
		static WORD aIndex[] = { 2, 0, 1 };

		static Vector3 vEdge;
		static WORD pFrontVer[4];	//分割后的前面的多边形顶点索引
		static WORD pBackVer[4];	//分割后的后面的多边形顶点索引

		int nFrontVer = 0;
		int nBackVer = 0;

		const Vector3& vVerDivPlane3 = mpVertexBuf[ pDivPlane3->mV1 ];

		//循环计算分割面在三边上分别的分割顶点坐标
		for( UINT i=0; i<3; ++i )
		{
			//使用矢量与平面相交（光线追踪）公式计算相交顶点坐标
			const WORD wIndexA = pPlane3->mV[ aIndex[i] ];
			const WORD wIndexB = pPlane3->mV[i];

			const Vector3& vA = mpVertexBuf[ wIndexA ];
			const Vector3& vB = mpVertexBuf[ wIndexB ];

			const float fDistA = pDivPlane3->GetDistance( vA );
			const float fDistB = pDivPlane3->GetDistance( vB );

			//计算边法线
			vEdge = ( vB - vA ).GetNormalizedVec();

			if( fDistB > 0.0f )
			{
				if( fDistA < 0.0f )
				{
					//增加新的顶点
					float fEdgeDist = -fDistA / pDivPlane3->mNormal.GetDot( vEdge );
					mpVertexBuf[ mNumVertex ] = vA + ( vEdge * fEdgeDist );

					pFrontVer[nFrontVer++] = pBackVer[nBackVer++] = (WORD)mNumVertex;
					++mNumVertex;
				}
				pFrontVer[nFrontVer++] = wIndexB;
			}
			else if( fDistB < 0.0f )
			{
				if( fDistA > 0.0f )
				{
					//增加新的顶点
					float fEdgeDist = -fDistA / pDivPlane3->mNormal.GetDot( vEdge );
					mpVertexBuf[ mNumVertex ] = vA + ( vEdge * fEdgeDist );

					pFrontVer[nFrontVer++] = pBackVer[nBackVer++] = (WORD)mNumVertex;
					++mNumVertex;
				}
				pBackVer[nBackVer++] = wIndexB;
			}
			else
			{
				pFrontVer[nFrontVer++] = pBackVer[nBackVer++] = wIndexB;
			}
		}

		//增加新的三角面
		_BuildNewPlane3( pFrontVer, nFrontVer, pFrontPlane3List );
		_BuildNewPlane3( pBackVer, nBackVer, pBackPlane3List );
	}
	//--------------------------------------------------------------------------------------------------------------
	//建立新二叉树分割面
	void BSPGenerator::_BuildNewPlane3( WORD pVerIndex[4], int nNumVer, BSPPlaneList* pNewPlane3List )
	{
		//根据顶点数量判断建立的三角面数量
		switch (nNumVer)
		{
		case 3:
			{
				*pNewPlane3List->Push() = mpPlane3Buf + mNumFace;
				mpPlane3Buf[ mNumFace++ ].SetFromPoints( mpVertexBuf, pVerIndex[0], pVerIndex[1], pVerIndex[2] );

				break;
			}
		case 4:
			{
				*pNewPlane3List->Push() = mpPlane3Buf + mNumFace;
				mpPlane3Buf[ mNumFace++ ].SetFromPoints( mpVertexBuf, pVerIndex[0], pVerIndex[1], pVerIndex[2] );

				*pNewPlane3List->Push() = mpPlane3Buf + mNumFace;
				mpPlane3Buf[ mNumFace++ ].SetFromPoints( mpVertexBuf, pVerIndex[0], pVerIndex[2], pVerIndex[3] );

				break;
			}
		default:
			Except( Exception::ERR_INTERNAL_ERROR, "三角形分割后生成的新顶点数量错误。" );
		}
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
