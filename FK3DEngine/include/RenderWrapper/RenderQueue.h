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
*	文件说明：	渲染队列类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================
#include "RenderObject.h"
#include "RenderSubGroup.h"
#include "RenderGroup.h"
#include "RenderInstance.h"
//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	class Camera;
	//--------------------------------------------------------------------------------------------------------------
	//渲染队列类
	class FK_DLL RenderQueue
	{
	public:
		RenderUnitStack			mRenderUnitStack;		//渲染单元堆栈
		RenderGroupStack		mRenderGroupStack;		//渲染组堆栈
		RenderSubGroupStack		mRenderSubGroupStack;	//子渲染组堆栈

	public:
		RenderQueue();
		~RenderQueue();

		//清空渲染队列
		void ClearRenderQueue();

		//排序渲染队列
		void SortRenderQueue( const Camera* pCamera );

		//渲染
		void Render( const Camera* pCamera );

		//添加渲染实例
		void FKFastcall AddRenderInstance( RenderInstance* pInstance );

		//添加渲染实例列表
		void FKFastcall AddRenderInstList( RenderInstList* pRenderInstList );
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
