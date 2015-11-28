/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	RenderSubGroup
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../../include/RenderWrapper/RenderWrapperInsideHeader.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//交换渲染单元
	FKInline void SwapRenderUnit( RenderUnit* pUnitA, RenderUnit* pUnitB )
	{
		static RenderUnit sTmpUnit;

		sTmpUnit.pRenderInstance	= pUnitA->pRenderInstance;
		sTmpUnit.pRenderParam		= pUnitA->pRenderParam;

		pUnitA->pRenderInstance		= pUnitB->pRenderInstance;
		pUnitA->pRenderParam		= pUnitB->pRenderParam;

		pUnitB->pRenderInstance		= sTmpUnit.pRenderInstance;
		pUnitB->pRenderParam		= sTmpUnit.pRenderParam;
	}
	//--------------------------------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------------------------------
	//排序子渲染组
	void RenderSubGroup::SortRenderSubGroup( const Camera* pCamera )
	{
		//将所有不透明渲染单元排序在前面
		RenderUnit* pTransFindStart = mpHeadRenderUnit;
		RenderUnit* pSolidFindStart = mpRearRenderUnit;

		for(;;)
		{
			//查找第一个透明渲染单元
			RenderUnit* pFirstTransUnit = _FindFirstTransUnit( pTransFindStart );
			if( pFirstTransUnit == NULL )
			{
				mpHeadSolidUnit = mpHeadRenderUnit;
				mpRearSolidUnit = mpRearRenderUnit;

				mpHeadTransUnit = NULL;
				mpRearTransUnit = NULL;

				break;
			}

			//查找最后一个不透明渲染单元
			RenderUnit* pLastSolidUnit = _FindLastSolidUnit( pSolidFindStart );
			if( pLastSolidUnit == NULL )
			{
				mpHeadSolidUnit = NULL;
				mpRearSolidUnit = NULL;

				mpHeadTransUnit = mpHeadRenderUnit;
				mpRearTransUnit = mpRearRenderUnit;

				break;
			}

			if( pLastSolidUnit > pFirstTransUnit )
			{
				mpHeadSolidUnit = mpHeadRenderUnit;
				mpRearSolidUnit = pLastSolidUnit;

				mpHeadTransUnit = pFirstTransUnit;
				mpRearTransUnit = mpRearRenderUnit;
			}

			SwapRenderUnit( pFirstTransUnit, pLastSolidUnit );

			pTransFindStart = pFirstTransUnit + 1;
			pSolidFindStart = pLastSolidUnit - 1;
		}

		_SortSolidRenderUnit();				//更新不透明渲染单元
		_SortTransRenderUnit( pCamera );	//更新透明渲染单元
	}
	//--------------------------------------------------------------------------------------------------------------
	//查找最前透明渲染单元
	FKInline RenderUnit* RenderSubGroup::_FindFirstTransUnit( RenderUnit* pStartUnit )
	{
		for(; pStartUnit<=mpRearRenderUnit; ++pStartUnit )
		{
			if( pStartUnit->pRenderParam->pMaterial->mRenderState.IsTransparent() )
				return pStartUnit;
		}

		return NULL;
	}
	//--------------------------------------------------------------------------------------------------------------
	//查找最末不透明渲染单元
	FKInline RenderUnit* RenderSubGroup::_FindLastSolidUnit( RenderUnit* pStartUnit )
	{
		for(; pStartUnit>=mpHeadRenderUnit; --pStartUnit )
		{
			if( !pStartUnit->pRenderParam->pMaterial->mRenderState.IsTransparent() )
				return pStartUnit;
		}

		return NULL;
	}
	//--------------------------------------------------------------------------------------------------------------
	//排序不透明渲染单元
	FKInline void RenderSubGroup::_SortSolidRenderUnit()
	{
		if( mpHeadSolidUnit == NULL )
			return;

		//根据材质排序渲染单元（选择排序法）
		RenderUnit* pUnit = mpHeadSolidUnit;
		UINT nNumUnit = (UINT)( mpRearSolidUnit - mpHeadSolidUnit ) + 1;

		for( UINT i=0; i<nNumUnit; ++i )
		{
			int nMinIndex = i;

			for( UINT j=i; j<nNumUnit; ++j )
				if( pUnit[nMinIndex].pRenderParam->pMaterial > pUnit[j].pRenderParam->pMaterial )
					nMinIndex = j;

			SwapRenderUnit( pUnit + i, pUnit + nMinIndex );
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//排序透明渲染单元
	FKInline void RenderSubGroup::_SortTransRenderUnit( const Camera* pCamera )
	{
		if( mpHeadTransUnit == NULL )
			return;

		RenderUnit* pUnit = mpHeadTransUnit;
		UINT nNumUnit = (UINT)( mpRearTransUnit - mpHeadTransUnit ) + 1;

		//根据从远到近的观察距离排序渲染单元（选择排序法）
		for( UINT i=0; i<nNumUnit; ++i )
		{
			//更新透明渲染单元观察深度
			pUnit[i].pRenderInstance->UpdateViewDepth( pCamera );

			int nMinIndex = i;

			for( UINT j=i; j<nNumUnit; ++j )
				if( pUnit[nMinIndex].pRenderInstance->mfViewDepth < pUnit[j].pRenderInstance->mfViewDepth )
					nMinIndex = j;

			SwapRenderUnit( pUnit + i, pUnit + nMinIndex );
		}
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
