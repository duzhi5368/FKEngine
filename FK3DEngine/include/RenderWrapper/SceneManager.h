/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	SceneManager
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	场景管理器
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================
#include "RenderQueue.h"
#include "RenderInstance.h"
#include "Camera.h"
#include "Light.h"
//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	class SceneNode;
	class SkyEntity;
	class SkyPlane;
	class SkyBox;
	class SkySphere;
	//--------------------------------------------------------------------------------------------------------------
	//场景管理器类型
	enum SceneManagerType
	{
		SMT_Basic,		//基本场景管理器
		SMT_BSP,		//二叉树场景管理器
		SMT_Quadtree,	//四叉树场景管理器
		SMT_Octree,		//八叉树场景管理器
	};
	//--------------------------------------------------------------------------------------------------------------
	//场景管理器
	class FK_DLL SceneManager : public Singleton< SceneManager >
	{
	public:
		friend class SceneNode;

	protected:
		RenderQueue				mRenderQueue;			//渲染队列

		RenderInstList			mNoCullRenderInsts;		//非拣选渲染实例队列

		CameraList				mCameraList;			//摄像机列表
		LightList				mLightList;				//灯光列表

	public:
		SceneNode*				mpRootNode;				//场景根节点

		SkyEntity*				mpSkyEntity;			//天空体

	public:
		SceneManager();
		virtual ~SceneManager();

		//载入场景文件
		virtual void LoadFromFile( FileObject* pFile );

		//更新场景节点树
		void UpdateSceneNodeTree();

		//渲染场景
		void RenderScene( float fTimeSinceLastFrame );

		//创建场景根节点
		virtual SceneNode* CreateSceneRootNode();

		//获取场景根节点
		SceneNode* GetSceneRootNode();

		//创建摄像机
		Camera* CreateCamera();

		//创建灯光
		Light* CreateLight();

		//删除摄像机
		void DeleteCamera( Camera* pCamera );

		//删除灯光
		void DeleteLight( Light* pLight );

		//删除所有摄像机
		void DeleteAllCamera();

		//删除所有灯光
		void DeleteAllLight();

		//创建天空面
		SkyPlane* CreateSkyPlane();

		//创建天空盒
		SkyBox* CreateSkyBox();

		//创建天空球
		SkySphere* CreateSkySphere( UINT nNumRing = 20, UINT nNumSegment = 20 );

		//摧毁天空体
		void DestroySky();

	protected:
		//获取可见渲染实例
		virtual void _GetVisableInstance( Camera* pActiveCam );
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
