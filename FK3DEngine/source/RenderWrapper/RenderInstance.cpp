/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	RenderInstance
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
	RenderInstance::RenderInstance( UINT nGroupPriority )
		: mpRenderObject	(NULL)
		, mpSceneNode		(NULL)
		, mpAnimControl		(NULL)
		, mnGroupPriority	(nGroupPriority)
		, mbVisable			(true)
		, mbCullable		(true)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	RenderInstance::RenderInstance( RenderObject* pRenderObj, UINT nGroupPriority )
		: mpRenderObject	(pRenderObj)
		, mpSceneNode		(NULL)
		, mpAnimControl		(NULL)
		, mnGroupPriority	(nGroupPriority)
		, mbVisable			(true)
		, mbCullable		(true)
		, mfViewDepth		(0.0f)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	RenderInstance::~RenderInstance()
	{
		//自动将渲染实例从挂接的场景节点上断开
		if( mpSceneNode != NULL )
			mpSceneNode->DetachRenderInstance( this );
	}
	//--------------------------------------------------------------------------------------------------------------
	//渲染该实例
	void FKFastcall RenderInstance::Render( RenderParam* pParam )
	{
		//如果存在动画控制器
		if( mpAnimControl != NULL )
		{
			mpRenderObject->_RenderAnimObj( pParam, mpAnimControl, &mpSceneNode->mWorldMatrix );
		}
		//如果不存在动画控制器
		else
		{
			mpRenderObject->_RenderObj( pParam, &mpSceneNode->mWorldMatrix );
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//更新观察深度
	void FKFastcall RenderInstance::UpdateViewDepth( const Camera* pCamera )
	{
		if( mpSceneNode == NULL )
			return;

		mfViewDepth = ( pCamera->mViewPos - mpSceneNode->mDerivedPosition ).GetLength();
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
