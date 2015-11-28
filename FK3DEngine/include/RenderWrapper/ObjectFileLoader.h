/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	ObjectFileLoader
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	物体文件载入器
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================
#include "../../include/ParticleSystem/Particle.h"
#include "../../include/ParticleSystem/Emitter.h"
//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//物体文件载入器
	class FK_DLL ObjectFileLoader : public Singleton< ObjectFileLoader >
	{
	public:
		FileObject*		mpObjectPack;			//物体打包文件

		MaterialList	mLoadedMaterialList;	//加载材质列表
		TriMeshList		mLoadedTriMeshList;		//加载三角面网格列表
		StreamMeshList	mLoadedStreamMeshList;	//加载顶点流网格列表
		SkeletonList	mLoadedSkeletonList;	//加载骨骼列表
		ParticleList	mLoadedParticleList;	//加载粒子列表
		EmitterList		mLoadedEmitterList;		//加载粒子发射器列表
		BoundMeshList	mLoadedBoundMeshList;	//加载碰撞节点列表

		DWORD			mFileLength;			//文件长度

	public:
		ObjectFileLoader();
		~ObjectFileLoader();

		//设置物体打包文件
		void SetObjectPackFile( FileObject* pPackFile );

		//载入物体文件
		void LoadObjectFile( const char* szFileName );

	protected:
		//读取文件段标识
		bool _ReadChunkID( ObjectChunkID& eChunkID );

		//读取文件头段
		void _ReadChunkHeader();

		//读取材质段
		void _ReadChunkMaterial();

		//读取三角面网格段
		void _ReadChunkTriMesh();

		//读取顶点流网格段
		void _ReadChunkStreamMesh();

		//读取包围盒段
		void _ReadChunkBoundBox( RenderObject* pRenderObject );

		//读取索引流数据段
		void _ReadChunkIndexStream( RenderObject* pRenderObject, bool bSoftProc );

		//读取顶点流数据段
		void _ReadChunkVertexStream( RenderObject* pRenderObject, UINT nIndex, bool bSoftProc );

		//顶点声明段
		void _ReadChunkVertexElement( RenderObject* pRenderObject );

		//读取子网格段
		void _ReadChunkSubMesh( RenderObject* pRenderObject, UINT nIndex );

		//读取骨骼段
		void _ReadChunkSkeleton();

		//读取公告板段
		void _ReadChunkBillboard();

		//读取碰撞网格段
		void _ReadChunkBoundMesh();
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
