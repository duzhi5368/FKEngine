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
	SceneManager::SceneManager()
		: mpRootNode	(NULL)
		, mpSkyEntity	(NULL)
	{
		mCameraList.Initialize( 10, 10 );
		mLightList.Initialize( 50, 50 );

		mNoCullRenderInsts.Initialize( 50, 50 );
	}
	//--------------------------------------------------------------------------------------------------------------
	SceneManager::~SceneManager()
	{
		DeleteAllCamera();
		DeleteAllLight();

		SafeDelete( mpRootNode );

		SafeDelete( mpSkyEntity );
	}
	//--------------------------------------------------------------------------------------------------------------
	//载入场景文件
	void SceneManager::LoadFromFile( FileObject* pFile )
	{

	}
	//--------------------------------------------------------------------------------------------------------------
	//更新场景节点树
	void SceneManager::UpdateSceneNodeTree()
	{
		//更新节点矩阵
		mpRootNode->_UpdateMatrix();

		//更新节点包围盒
		mpRootNode->_UpdateBox();
	}
	//--------------------------------------------------------------------------------------------------------------
	//渲染场景
	void SceneManager::RenderScene( float fTimeSinceLastFrame )
	{
		//更新场景节点树
		UpdateSceneNodeTree();

		//更新天空体
		if( mpSkyEntity != NULL )
			mpSkyEntity->_UpdateSky( fTimeSinceLastFrame );

		//更新平面图像渲染数据
		OverlayManager* pOverlayManager = OverlayManager::GetSingletonPtr();
		pOverlayManager->_UpdateVertexData();

		//更新粒子发射器
		ParticleSystem::mpSingleton->_UpdateEmitter( fTimeSinceLastFrame );


		//准备设备
		RenderSystem::mpSingleton->_PrepareDevice();

		//循环渲染所有视口
		ViewportList::Iterator it = RenderSystem::mpSingleton->mViewportList.Begin();
		ViewportList::Iterator end = RenderSystem::mpSingleton->mViewportList.End();
		for(; it!=end; ++it )
		{
			Viewport* pViewport = *it;

			//如果没有激活的摄像机则跳过
			Camera* pCamera = pViewport->mpCamera;
			if( pCamera == NULL )
				continue;

			//设置视口
			RenderSystem::mpSingleton->_SetViewport( pViewport );

			//更新摄像机
			pCamera->_UpdateCamera();

			//更新公告板粒子方向
			ParticleSystem::mpSingleton->_UpdateSprite( pCamera );

			//设置观察矩阵和投影矩阵
			RenderSystem::mpSingleton->_SetViewMatrix( &pCamera->GetViewMatrix() );
			RenderSystem::mpSingleton->_SetProjMatrix( &pCamera->GetProjMatrix() );

			//清空原有渲染队列
			mRenderQueue.ClearRenderQueue();

			//将场景中的可见渲染实例添加到渲染队列中
			_GetVisableInstance( pCamera );

			//添加非拣选渲染物体到渲染队列中
			mRenderQueue.AddRenderInstList( &mNoCullRenderInsts );

			//排序渲染队列
			mRenderQueue.SortRenderQueue( pCamera );


			//通知设备开始渲染
			RenderSystem::mpSingleton->_BeginRender();

			//渲染天空体
			if( mpSkyEntity != NULL )
				mpSkyEntity->_RenderSky();

			//渲染
			mRenderQueue.Render( pCamera );

			//渲染平面图像（GUI）
			pOverlayManager->_RenderAllOverlays();

			//通知设备结束渲染
			RenderSystem::mpSingleton->_EndRender();
		}

		//交换渲染页面
		RenderSystem::mpSingleton->_SwapRenderBuffer();
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建场景根节点
	SceneNode* SceneManager::CreateSceneRootNode()
	{
		mpRootNode = new SceneNode;
		return mpRootNode;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取场景根节点
	SceneNode* SceneManager::GetSceneRootNode()
	{
		return mpRootNode;
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建摄像机
	Camera* SceneManager::CreateCamera()
	{
		Camera* pCamera = new Camera;
		*mCameraList.Push() = pCamera;
		return pCamera;
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建灯光
	Light* SceneManager::CreateLight()
	{
		Light* pLight = new Light;
		*mLightList.Push() = pLight;
		return pLight;
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除摄像机
	void SceneManager::DeleteCamera( Camera* pCamera )
	{
		CameraList::Iterator it = mCameraList.Begin();
		CameraList::Iterator end = mCameraList.End();
		for(; it!=end; ++it )
		{
			if( *it == pCamera )
			{
				mCameraList.Erase( it );
				delete pCamera;
				return;
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除灯光
	void SceneManager::DeleteLight( Light* pLight )
	{
		LightList::Iterator it = mLightList.Begin();
		LightList::Iterator end = mLightList.End();
		for(; it!=end; ++it )
		{
			if( *it == pLight )
			{
				mLightList.Erase( it );
				delete pLight;
				return;
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除所有摄像机
	void SceneManager::DeleteAllCamera()
	{
		CameraList::Iterator it = mCameraList.Begin();
		CameraList::Iterator end = mCameraList.End();
		for(; it!=end; ++it )
			SafeDelete( *it );

		mCameraList.Clear();
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除所有灯光
	void SceneManager::DeleteAllLight()
	{
		LightList::Iterator it = mLightList.Begin();
		LightList::Iterator end = mLightList.End();
		for(; it!=end; ++it )
			SafeDelete( *it );

		mLightList.Clear();
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建天空面
	SkyPlane* SceneManager::CreateSkyPlane()
	{
		mpSkyEntity = new SkyPlane;
		return (SkyPlane*)mpSkyEntity;
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建天空盒
	SkyBox* SceneManager::CreateSkyBox()
	{
		mpSkyEntity = new SkyBox;
		return (SkyBox*)mpSkyEntity;
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建天空球
	SkySphere* SceneManager::CreateSkySphere( UINT nNumRing, UINT nNumSegment )
	{
		mpSkyEntity = new SkySphere( nNumRing, nNumSegment );
		return (SkySphere*)mpSkyEntity;
	}
	//--------------------------------------------------------------------------------------------------------------
	//摧毁天空体
	void SceneManager::DestroySky()
	{
		SafeDelete( mpSkyEntity );
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取可见渲染实例
	void SceneManager::_GetVisableInstance( Camera* pActiveCam )
	{
		mpRootNode->_GetVisibleInstance( pActiveCam, &mRenderQueue );
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
