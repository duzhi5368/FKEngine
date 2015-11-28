/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	BSPPlane
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
	BSPPlane::BSPPlane()
		: mpVertex		(NULL)
		, mbDivPlane3	(false)
		, mV1			(0)
		, mV2			(0)
		, mV3			(0)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	BSPPlane::BSPPlane( Vector3* pVertex, WORD v1, WORD v2, WORD v3 )
		: mpVertex		(pVertex)
		, mbDivPlane3	(false)
		, mV1			(v1)
		, mV2			(v2)
		, mV3			(v3)
	{
		Plane3::SetFromPoints( pVertex[v1], pVertex[v2], pVertex[v3] );
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置二叉树分割面
	void BSPPlane::SetFromPoints( Vector3* pVertex, WORD v1, WORD v2, WORD v3 )
	{
		mbDivPlane3 = false;
		mpVertex = pVertex;

		mV1 = v1;
		mV2 = v2;
		mV3 = v3;

		Plane3::SetFromPoints( pVertex[v1], pVertex[v2], pVertex[v3] );
	}
	//--------------------------------------------------------------------------------------------------------------
	//判断另一个二叉树分割面与该分割面的空间交错关系
	SpaceRelation FKFastcall BSPPlane::GetSide( const BSPPlane& rBSPPlane ) const
	{
		UINT nNumPositive = 0;
		UINT nNumNegative = 0;

		//检测每个顶点与该面的关系
		for( UINT i=0; i<3; ++i )
		{
			SpaceRelation eSR = Plane3::GetSide( mpVertex[ rBSPPlane.mV[i] ] );

			if( eSR == SR_POSITIVE )
				++nNumPositive;
			else if( eSR == SR_NEGATIVE )
				++nNumNegative;
		}

		if( nNumPositive > 0 && nNumNegative == 0 )
			return SR_POSITIVE;
		else if( nNumPositive == 0 && nNumNegative > 0 )
			return SR_NEGATIVE;
		else if( nNumPositive == 0 && nNumNegative == 0 )
			return SR_COINCIDING;
		else
			return SR_SPANNING;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
