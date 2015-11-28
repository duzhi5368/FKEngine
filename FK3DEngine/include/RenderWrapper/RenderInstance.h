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
*	文件说明：	渲染实例类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	class RenderObject;
	class AnimControl;
	class RenderParam;
	class Camera;
	//--------------------------------------------------------------------------------------------------------------

	//预定义渲染组优先级
	const UINT RGP_Background		= 0;		//背景（最先渲染组）
	const UINT RGP_SkiesEarly		= 10;		//提前渲染天空体
	const UINT RGP_GeometyEarly		= 25;		//提前渲染几何体
	const UINT RGP_GeometyDefault	= 50;		//几何体（默认渲染组）
	const UINT RGP_GeometyLate		= 75;		//延后渲染几何体
	const UINT RGP_SkiesLate		= 90;		//延后渲染天空体
	const UINT RGP_Overlays			= 100;		//平面图像（最后渲染组）

	const UINT RSGP_Default			= 50;		//默认子渲染组优先级

	//--------------------------------------------------------------------------------------------------------------
	//渲染实例类
	class FK_DLL RenderInstance
	{
	public:
		RenderObject*		mpRenderObject;		//渲染物体指针

		SceneNode*			mpSceneNode;		//挂接场景节点
		AnimControl*		mpAnimControl;		//动画控制器

		UINT				mnGroupPriority;	//渲染组优先级

		bool				mbVisable;			//实例是否可见
		bool				mbCullable;			//实例是否可被拣选

		float				mfViewDepth;		//观察深度

	public:
		RenderInstance( UINT nGroupPriority = RGP_GeometyDefault );
		RenderInstance( RenderObject* pRenderObj, UINT nGroupPriority = RGP_GeometyDefault );
		virtual ~RenderInstance();

		//渲染该实例
		void FKFastcall Render( RenderParam* pParam );

		//更新观察深度
		void FKFastcall UpdateViewDepth( const Camera* pCamera );
	};
	//--------------------------------------------------------------------------------------------------------------
	typedef Stack< RenderInstance* >	RenderInstList;
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
