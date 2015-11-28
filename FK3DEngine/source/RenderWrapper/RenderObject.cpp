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
	RenderObject::RenderObject()
		: mNumVertexBuffer		(0)
		, mppVertexBuffer		(NULL)
		, mpIndexBuffer			(NULL)
		, mpVertexDeclaration	(NULL)
		, mppMaterial			(NULL)
		, mNumMaterial			(0)
		, mpRenderParam			(NULL)
		, mNumRenderParam		(0)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	RenderObject::~RenderObject()
	{
		FreeStreamBuf();
		FreeMaterialBuf();
		FreeRenderParamBuf();
	}
	//--------------------------------------------------------------------------------------------------------------
	//分配顶点流指针缓存
	void RenderObject::AllocStreamBuf( UINT nNumber )
	{
		SafeFree( mppVertexBuffer );

		mppVertexBuffer = (VertexBuffer**)malloc( sizeof(VertexBuffer*) * nNumber );
		memset( mppVertexBuffer, 0, sizeof(VertexBuffer*) * nNumber );
		mNumVertexBuffer = nNumber;
	}
	//--------------------------------------------------------------------------------------------------------------
	//释放顶点流指针缓存
	void RenderObject::FreeStreamBuf()
	{
		SafeFree( mppVertexBuffer );
		mNumVertexBuffer = 0;
	}
	//--------------------------------------------------------------------------------------------------------------
	//分配材质指针缓存
	void RenderObject::AllocMaterialBuf( UINT nNumber )
	{
		SafeFree( mppMaterial );

		mppMaterial = (Material**)malloc( sizeof(Material*) * nNumber );
		memset( mppMaterial, 0, sizeof(Material*) * nNumber );
		mNumMaterial = nNumber;
	}
	//--------------------------------------------------------------------------------------------------------------
	//释放材质指针缓存
	void RenderObject::FreeMaterialBuf()
	{
		SafeFree( mppMaterial );
		mNumMaterial = 0;
	}
	//--------------------------------------------------------------------------------------------------------------
	//增加所有材质使用计数
	void RenderObject::AddMaterialRef()
	{
		for( UINT i=0; i<mNumMaterial; ++i )
		{
			if( mppMaterial[i] != NULL )
				mppMaterial[i]->AddRef();
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//减少所有材质使用计数
	void RenderObject::DelMaterialRef()
	{
		for( UINT i=0; i<mNumMaterial; ++i )
		{
			if( mppMaterial[i] != NULL )
				mppMaterial[i]->DelRef();
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//分配渲染参数缓存
	void RenderObject::AllocRenderParamBuf( UINT nNumParam )
	{
		SafeFree( mpRenderParam );

		//根据每个子网格信息创建一个渲染参数结构
		mpRenderParam = (RenderParam*)malloc( sizeof(RenderParam) * nNumParam );
		memset( mpRenderParam, 0, sizeof(RenderParam) * nNumParam );
		mNumRenderParam = nNumParam;
	}
	//--------------------------------------------------------------------------------------------------------------
	//释放渲染参数缓存
	void RenderObject::FreeRenderParamBuf()
	{
		SafeFree( mpRenderParam );
		mNumRenderParam = 0;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
