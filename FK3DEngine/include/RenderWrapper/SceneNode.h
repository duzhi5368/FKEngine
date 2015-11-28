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
*	文件说明：	场景节点类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================
#include "TransformNode.h"
#include "Light.h"
#include "RenderInstance.h"
#include "Camera.h"
//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	class SceneNode;
	class RenderQueue;
	typedef Stack< SceneNode* >		SceneNodeList;
	//--------------------------------------------------------------------------------------------------------------
	//场景节点类
	class FK_DLL SceneNode : public TransformNode
	{
	protected:
		LightList			mLightList;			//该节点下挂接的灯光
		CameraList			mCameraList;		//该节点下挂接的摄像机
		RenderInstList		mRenderInstList;	//该节点下挂接的渲染实例

		bool				mbUpdateObjBox;		//是否需要更新挂接物体包围盒
		AxisBox3			mObjBoundBox;		//挂接物体包围盒

	public:
		SceneNode();
		virtual ~SceneNode();

		//挂接灯光
		void AttachLight( Light* pLight );

		//断开灯光
		void DetachLight( Light* pLight );

		//挂接摄像机
		void AttachCamera( Camera* pCamera );

		//断开摄像机
		void DetachCamera( Camera* pCamera );

		//挂接渲染实例
		void AttachRenderInstance( RenderInstance* pInstance );

		//断开渲染实例
		void DetachRenderInstance( RenderInstance* pInstance );

		//断开所有渲染实例
		void DetachAllRenderInstance();

	protected:
		//节点创建函数
		virtual TransformNode* FKFastcall _NodeCreator();

		//获取节点挂接物体包围盒
		const AxisBox3& FKFastcall _GetLocalObjectBox();

		//将该节点下的所有渲染实例加入渲染队列
		void FKFastcall _AddInstanceToQueue( RenderQueue* pRenderQueue );

	public:
		//获取该节点及其子节点下的可见渲染实例
		void FKFastcall _GetVisibleInstance( Camera* pActiveCam, RenderQueue* pRenderQueue );

		//通知包围盒已更新
		virtual void FKFastcall _NotifyBoxUpdated();
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
