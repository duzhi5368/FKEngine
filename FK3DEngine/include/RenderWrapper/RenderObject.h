/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	RenderObject
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	渲染物体类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	class VertexDeclaration;
	class VertexBuffer;
	class IndexBuffer;
	class Material;
	class RenderParam;
	class AnimControl;
	//--------------------------------------------------------------------------------------------------------------
	//渲染物体类
	class FK_DLL RenderObject
	{
	public:
		friend class RenderInstance;

	public:
		UINT				mNumVertexBuffer;	//使用顶点缓存数量
		VertexBuffer**		mppVertexBuffer;	//顶点缓存指针数组
		IndexBuffer*		mpIndexBuffer;		//索引缓存指针
		VertexDeclaration*	mpVertexDeclaration;//顶点描述指针

		Material**			mppMaterial;		//材质数组
		UINT				mNumMaterial;		//材质数量

		RenderParam*		mpRenderParam;		//渲染参数数组
		UINT				mNumRenderParam;	//渲染参数数量

		AxisBox3			mBoundBox;			//渲染物体包围盒

	public:
		RenderObject();
		virtual ~RenderObject();

		//分配顶点流指针缓存
		void AllocStreamBuf( UINT nNumber = 1 );

		//释放顶点流指针缓存
		void FreeStreamBuf();

		//分配材质指针缓存
		void AllocMaterialBuf( UINT nNumber = 1 );

		//释放材质指针缓存
		void FreeMaterialBuf();

		//增加所有材质使用计数
		void AddMaterialRef();

		//减少所有材质使用计数
		void DelMaterialRef();

		//分配渲染参数缓存
		void AllocRenderParamBuf( UINT nNumParam = 1 );

		//释放渲染参数缓存
		void FreeRenderParamBuf();

	protected:
		//渲染物体
		virtual void _RenderObj( const RenderParam* pParam, const Matrix4* pNodeMatrix ) = 0;

		//渲染动画物体
		virtual void _RenderAnimObj( const RenderParam* pParam, const AnimControl* pAnimControl, const Matrix4* pNodeMatrix ) = 0;
	};
	//--------------------------------------------------------------------------------------------------------------
	typedef Stack< RenderObject* >	RenderObjList;
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
