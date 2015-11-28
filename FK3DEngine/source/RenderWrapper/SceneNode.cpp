/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	SceneNode
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
	SceneNode::SceneNode()
	{
		mLightList.Initialize( 2, 6 );
		mCameraList.Initialize( 2, 6 );
		mRenderInstList.Initialize( 20, 20 );
	}
	//--------------------------------------------------------------------------------------------------------------
	SceneNode::~SceneNode()
	{
		DetachAllRenderInstance();
	}
	//--------------------------------------------------------------------------------------------------------------
	//挂接灯光
	void SceneNode::AttachLight( Light* pLight )
	{
		*mLightList.Push() = pLight;
	}
	//--------------------------------------------------------------------------------------------------------------
	//断开灯光
	void SceneNode::DetachLight( Light* pLight )
	{
		LightList::Iterator it = mLightList.Begin();
		LightList::Iterator end = mLightList.End();
		for(; it!=end; ++it )
		{
			if( *it == pLight )
			{
				mLightList.Erase( it );
				return;
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//挂接摄像机
	void SceneNode::AttachCamera( Camera* pCamera )
	{
		*mCameraList.Push() = pCamera;
	}
	//--------------------------------------------------------------------------------------------------------------
	//断开摄像机
	void SceneNode::DetachCamera( Camera* pCamera )
	{
		CameraList::Iterator it = mCameraList.Begin();
		CameraList::Iterator end = mCameraList.End();
		for(; it!=end; ++it )
		{
			if( *it == pCamera )
			{
				mCameraList.Erase( it );
				return;
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//挂接渲染实例
	void SceneNode::AttachRenderInstance( RenderInstance* pInstance )
	{
		*mRenderInstList.Push() = pInstance;

		//设置实例场景节点指针
		pInstance->mpSceneNode = this;

		//如果挂接实例为可拣选渲染实例
		if( pInstance->mbCullable )
		{
			//通知父节点需要更新挂接物体包围盒
			mbUpdateObjBox = true;
			_NotifyParentUpdateBox();
		}
		//如果挂接实例为非拣选渲染实例
		else
		{
			//将其添加到场景管理器非拣选渲染实例队列中
			*Root::mpSingleton->mpSceneManager->mNoCullRenderInsts.Push() = pInstance;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//断开渲染实例
	void SceneNode::DetachRenderInstance( RenderInstance* pInstance )
	{
		//将指定渲染实例从该节点的渲染实例列表中删除
		RenderInstList::Iterator it = mRenderInstList.Begin();
		RenderInstList::Iterator end = mRenderInstList.End();
		for(; it!=end; ++it )
		{
			if( *it == pInstance )
			{
				mRenderInstList.Erase( it );
				break;
			}
		}
		pInstance->mpSceneNode = NULL;

		//如果断开实例为可拣选渲染实例
		if( pInstance->mbCullable )
		{
			//通知父节点需要更新挂接物体包围盒
			mbUpdateObjBox = true;
			_NotifyParentUpdateBox();
		}
		//如果断开实例为非拣选渲染实例
		else
		{
			//将它从场景管理器的非拣选实例列表中删除
			RenderInstList& NoCullInsts = Root::mpSingleton->mpSceneManager->mNoCullRenderInsts;
			RenderInstList::Iterator it = NoCullInsts.Begin();
			RenderInstList::Iterator end = NoCullInsts.End();
			for(; it!=end; ++it )
			{
				if( *it == pInstance )
				{
					NoCullInsts.Erase( it );
					return;
				}
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//断开所有渲染实例
	void SceneNode::DetachAllRenderInstance()
	{
		//将指定实例从该节点的渲染实例列表中删除
		RenderInstList::Iterator it = mRenderInstList.Begin();
		RenderInstList::Iterator end = mRenderInstList.End();
		for(; it!=end; ++it )
		{
			RenderInstance* pInstance = *it;
			pInstance->mpSceneNode = NULL;

			//如果断开实例为非拣选实例
			if( !pInstance->mbCullable )
			{
				//将它从场景管理器的非拣选实例列表中删除
				RenderInstList& NoCullInsts = Root::mpSingleton->mpSceneManager->mNoCullRenderInsts;
				RenderInstList::Iterator it = NoCullInsts.Begin();
				RenderInstList::Iterator end = NoCullInsts.End();
				for(; it!=end; ++it )
				{
					if( *it == pInstance )
					{
						NoCullInsts.Erase( it );
						continue;
					}
				}
			}
		}

		mRenderInstList.Clear();
	}
	//--------------------------------------------------------------------------------------------------------------
	//节点创建函数
	TransformNode* FKFastcall SceneNode::_NodeCreator()
	{
		return new SceneNode;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取节点挂接物体包围盒
	const AxisBox3& FKFastcall SceneNode::_GetLocalObjectBox()
	{
		//如果该节点挂接物体包围盒需要更新
		if( mbUpdateObjBox )
		{
			mObjBoundBox.SetEmpty( true );

			RenderInstList::Iterator it = mRenderInstList.Begin();
			RenderInstList::Iterator end = mRenderInstList.End();
			for(; it!=end; ++it )
				mObjBoundBox.Combine( (*it)->mpRenderObject->mBoundBox );

			mbUpdateObjBox = false;
		}

		return mObjBoundBox;
	}
	//--------------------------------------------------------------------------------------------------------------
	//将该节点下的所有渲染实例加入渲染队列
	void FKFastcall SceneNode::_AddInstanceToQueue( RenderQueue* pRenderQueue )
	{
		RenderInstList::Iterator it = mRenderInstList.Begin();
		RenderInstList::Iterator end = mRenderInstList.End();
		for(; it!=end; ++it )
		{
			RenderInstance* pInstance = *it;

			//如果为非拣选渲染实例则略过（它将在场景管理器非拣选渲染实例队列中被包含）
			if( !pInstance->mbCullable )
				continue;

			//如果渲染实例可见
			if( pInstance->mbVisable )
			{
				//将其加入渲染队列
				pRenderQueue->AddRenderInstance( pInstance );
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取该节点及其子节点下的可见渲染实例
	void FKFastcall SceneNode::_GetVisibleInstance( Camera* pActiveCam, RenderQueue* pRenderQueue )
	{
		//判断本节点是否可见（因为节点的包围盒包含了该节点下所有渲染实例，如果它不可见，则所有的实例都不可见。）
		if( mNodeBoundBox.mbEmpty || !pActiveCam->mViewFrustum.IsIntersect( mBoundBoxVer ) )
			return;

		//添加本节点的所有渲染实例到渲染队列中
		_AddInstanceToQueue( pRenderQueue );

		//所有的子节点一个一个的循环，判断是否可见。
		TransformNodeList::Iterator itChild = mNodeChildren.Begin();
		TransformNodeList::Iterator itChildEnd = mNodeChildren.End();
		for(; itChild != itChildEnd; ++itChild )
		{
			SceneNode* pNode = static_cast< SceneNode* >( *itChild );
			pNode->_GetVisibleInstance( pActiveCam, pRenderQueue );
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//通知包围盒已更新
	void FKFastcall SceneNode::_NotifyBoxUpdated()
	{
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
