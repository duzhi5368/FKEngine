/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	RenderQueue
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
	RenderQueue::RenderQueue()
	{
		mRenderUnitStack.Initialize( 1000, 1000 );
		mRenderGroupStack.Initialize( 20, 20 );
		mRenderSubGroupStack.Initialize( 200, 200 );
	}
	//--------------------------------------------------------------------------------------------------------------
	RenderQueue::~RenderQueue()
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	//清空渲染队列
	void RenderQueue::ClearRenderQueue()
	{
		mRenderUnitStack.Clear();
		mRenderGroupStack.Clear();
		mRenderSubGroupStack.Clear();
	}
	//--------------------------------------------------------------------------------------------------------------
	//排序渲染队列
	void RenderQueue::SortRenderQueue( const Camera* pCamera )
	{
		//根据渲染组优先级排序渲染单元（选择排序法）
		RenderUnit* pUnit = mRenderUnitStack.Begin();
		UINT nNumUnit = mRenderUnitStack.Size();

		if( nNumUnit == 0 )
			return;

		for( UINT i=0; i<nNumUnit; ++i )
		{
			int nMinIndex = i;

			for( UINT j=i; j<nNumUnit; ++j )
				if( pUnit[nMinIndex].pRenderInstance->mnGroupPriority > pUnit[j].pRenderInstance->mnGroupPriority )
					nMinIndex = j;

			SwapRenderUnit( pUnit + i, pUnit + nMinIndex );
		}

		//生成渲染组
		RenderGroup* pGroup = mRenderGroupStack.Push();

		pGroup->mnPriority = pUnit->pRenderInstance->mnGroupPriority;
		pGroup->mpHeadRenderUnit = pUnit;
		pGroup->mpRearRenderUnit = pUnit;

		for( UINT i=1; i<nNumUnit; ++i )
		{
			if( pUnit[i].pRenderInstance->mnGroupPriority == pGroup->mnPriority )
			{
				pGroup->mpRearRenderUnit += 1;
			}
			else
			{
				pGroup->SortRenderGroup( &mRenderSubGroupStack, pCamera );

				pGroup = mRenderGroupStack.Push();

				pGroup->mnPriority = pUnit->pRenderInstance->mnGroupPriority;
				pGroup->mpHeadRenderUnit = pUnit + i;
				pGroup->mpRearRenderUnit = pUnit + i;
			}
		}

		pGroup->SortRenderGroup( &mRenderSubGroupStack, pCamera );
	}
	//--------------------------------------------------------------------------------------------------------------
	//渲染
	void RenderQueue::Render( const Camera* pCamera )
	{
		Material*		pLastMaterial	= NULL;
		RenderObject*	pLastRenderObj	= NULL;

		RenderUnitStack::Iterator it = mRenderUnitStack.Begin();
		RenderUnitStack::Iterator end = mRenderUnitStack.End();
		for(; it!=end; ++it )
		{
			//获取渲染参数
			RenderParam*		pParam		= it->pRenderParam;
			RenderInstance*		pInstance	= it->pRenderInstance;
			RenderObject*		pRenderObj	= pInstance->mpRenderObject;
			Material*			pMaterial	= pParam->pMaterial;

			if( !pParam->NeedRender() )
				continue;

			//如果当前材质和最后一次设置材质不同则应用新材质
			if( pMaterial != pLastMaterial )
			{
				//应用纹理状态
				pMaterial->ApplyTextureState();

				//设置材质和材质渲染状态
				RenderSystem::mpSingleton->_SetMaterial( pMaterial );
				RenderSystem::mpSingleton->_SetRenderState( &pMaterial->mRenderState );

				pLastMaterial = pMaterial;
			}

			//如果该渲染实例使用的渲染物体不是最后设置的渲染物体
			if( pRenderObj != pLastRenderObj )
			{
				//设置顶点声明、顶点流和索引流。
				RenderSystem::mpSingleton->_SetVertexDeclaration( pRenderObj->mpVertexDeclaration );

				for( UINT i=0; i<pRenderObj->mNumVertexBuffer; ++i )
					RenderSystem::mpSingleton->_SetVertexStream( pRenderObj->mppVertexBuffer[i], i );

				RenderSystem::mpSingleton->_SetIndexStream( pRenderObj->mpIndexBuffer );

				pLastRenderObj = pRenderObj;
			}

			//渲染该实例
			pInstance->Render( pParam );
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//添加渲染实例
	void FKFastcall RenderQueue::AddRenderInstance( RenderInstance* pInstance )
	{
		const UINT nNumParam = pInstance->mpRenderObject->mNumRenderParam;
		RenderParam* pParams = pInstance->mpRenderObject->mpRenderParam;

		RenderUnit* pUnits = mRenderUnitStack.Push( nNumParam );

		for( UINT i=0; i<nNumParam; ++i )
		{
			pUnits[i].pRenderInstance = pInstance;
			pUnits[i].pRenderParam = pParams + i;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//添加渲染实例列表
	void FKFastcall RenderQueue::AddRenderInstList( RenderInstList* pRenderInstList )
	{
		RenderInstList::Iterator it = pRenderInstList->Begin();
		RenderInstList::Iterator end = pRenderInstList->End();
		for(; it!=end; ++it )
			AddRenderInstance( *it );
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
