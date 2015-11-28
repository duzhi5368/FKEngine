/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	RenderGroup
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
	//排序渲染组
	void RenderGroup::SortRenderGroup( RenderSubGroupStack* pRenderSubGroupStack, const Camera* pCamera )
	{
		if( mpHeadRenderUnit == NULL )
			return;

		//根据子渲染组优先级排序渲染单元（选择排序法）
		RenderUnit* pUnit = mpHeadRenderUnit;
		UINT nNumUnit = (UINT)( mpRearRenderUnit - mpHeadRenderUnit ) + 1;

		for( UINT i=0; i<nNumUnit; ++i )
		{
			int nMinIndex = i;

			for( UINT j=i; j<nNumUnit; ++j )
				if( pUnit[nMinIndex].pRenderParam->nPriority > pUnit[j].pRenderParam->nPriority )
					nMinIndex = j;

			SwapRenderUnit( pUnit + i, pUnit + nMinIndex );
		}

		//生成子渲染组
		RenderSubGroup* pSubGroup = pRenderSubGroupStack->Push();

		pSubGroup->mnPriority = pUnit->pRenderParam->nPriority;
		pSubGroup->mpHeadRenderUnit = pUnit;
		pSubGroup->mpRearRenderUnit = pUnit;

		for( UINT i=1; i<nNumUnit; ++i )
		{
			if( pUnit[i].pRenderParam->nPriority == pSubGroup->mnPriority )
			{
				pSubGroup->mpRearRenderUnit += 1;
			}
			else
			{
				pSubGroup->SortRenderSubGroup( pCamera );

				pSubGroup = pRenderSubGroupStack->Push();

				pSubGroup->mnPriority = pUnit->pRenderParam->nPriority;
				pSubGroup->mpHeadRenderUnit = pUnit + i;
				pSubGroup->mpRearRenderUnit = pUnit + i;
			}
		}

		pSubGroup->SortRenderSubGroup( pCamera );
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
