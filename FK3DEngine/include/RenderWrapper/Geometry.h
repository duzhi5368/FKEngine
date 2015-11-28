/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Geometry
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	几何体类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//几何体类
	class FK_DLL Geometry : public RenderObject
	{
	public:
		//几何体创建函数
		typedef Geometry* (*FnCreator)();

	public:
		RenderInstList		mRenderInstList;	//渲染实例列表

		bool				mbSoftVertexProc;	//是否为软件顶点处理
		UINT				mNumBlendMatrix;	//顶点混合矩阵数量
		UINT				mMaxBlendIndex;		//使用最大索引混合值

	public:
		Geometry();
		virtual ~Geometry();

		//建立顶点流
		void BuildMeshStream( UINT nVerNum, UINT nIndexNum, bool bHaveUV = false,
			bool bHaveNormal = false, bool bHaveColor = false, UINT nNumBlendMatrix = 0, UINT nMaxBlendIndex = 0 );

		//删除顶点流
		void DestroyMeshStream();

		//初始化渲染实例列表
		void InitializeInstanceList( UINT nInitNum, UINT nAddNum );

		//释放渲染实例列表
		void ReleaseInstanceList();

		//创建渲染实例
		RenderInstance* CreateInstance();

		//删除指定渲染实例
		void DeleteInstance( RenderInstance* pInstance );

		//删除所有渲染实例
		void DeleteAllInstance();
	};
	//--------------------------------------------------------------------------------------------------------------
	typedef Stack< Geometry* >		GeometryList;
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
