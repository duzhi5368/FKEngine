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
	Geometry::Geometry()
		: mbSoftVertexProc		(false)
		, mNumBlendMatrix		(0)
		, mMaxBlendIndex		(0)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	Geometry::~Geometry()
	{
		DeleteAllInstance();

		DelMaterialRef();

		DestroyMeshStream();
	}
	//--------------------------------------------------------------------------------------------------------------
	//建立顶点流
	void Geometry::BuildMeshStream( UINT nVerNum, UINT nIndexNum, bool bHaveUV,
		bool bHaveNormal, bool bHaveColor, UINT nNumBlendMatrix, UINT nMaxBlendIndex )
	{
		mNumBlendMatrix	= nNumBlendMatrix;
		mMaxBlendIndex	= nMaxBlendIndex;

		//判断是否需要使用软件顶点处理
		DWORD dwHardMaxBlendIndex = bHaveNormal ? ( RenderSystem::mdwMaxBlendIndex / 2 ) : RenderSystem::mdwMaxBlendIndex;
		mbSoftVertexProc = ( dwHardMaxBlendIndex < nMaxBlendIndex || RenderSystem::mdwMaxBlendNum < nNumBlendMatrix );

		//创建顶点声明
		mpVertexDeclaration = BufferManager::mpSingleton->AddVertexDeclaration();

		//计算顶点流数量
		UINT nNumVertexBuffer = 1;

		if( bHaveUV )
			++nNumVertexBuffer;
		if( bHaveNormal )
			++nNumVertexBuffer;
		if( bHaveColor )
			++nNumVertexBuffer;

		AllocStreamBuf( nNumVertexBuffer );

		nNumVertexBuffer = 0;

		//建立坐标流
		mpVertexDeclaration->AddElement( nNumVertexBuffer, 0, VET_FLOAT3, VEM_DEFAULT, VEU_POSITION, 0 );

		mppVertexBuffer[nNumVertexBuffer] = BufferManager::mpSingleton->AddVertexBuffer();
		mppVertexBuffer[nNumVertexBuffer]->CreateVertexBuffer( sizeof(float)*3, nVerNum, false, mbSoftVertexProc );
		++nNumVertexBuffer;

		//如果需要进行顶点混合
		if( nNumBlendMatrix > 0 )
		{
			if( nNumBlendMatrix > 4 )
				Except( Exception::ERR_INTERNAL_ERROR, "每个顶点的骨骼数量不允许超过 4 个。" );

			//渲染需要权重数量比实际值少一
			//例如有两个权重 0.6f 0.4f，则只需要复制 0.6f，0.4f 可通过 1.0 - 0.6f = 0.4f 计算得到。
			UINT nNumWeightPerVer = nNumBlendMatrix - 1;

			//如果需要权重数据
			if( nNumWeightPerVer > 0 )
			{
				//权重流
				VertexElementType vet = (VertexElementType)( nNumWeightPerVer - 1 );

				mpVertexDeclaration->AddElement( nNumVertexBuffer, 0, vet, VEM_DEFAULT, VEU_BLENDWEIGHT, 0 );

				mppVertexBuffer[nNumVertexBuffer] = BufferManager::mpSingleton->AddVertexBuffer();
				mppVertexBuffer[nNumVertexBuffer]->CreateVertexBuffer( sizeof(float)*nNumWeightPerVer, nVerNum,
					false, mbSoftVertexProc );
				++nNumVertexBuffer;
			}

			//混合矩阵索引流
			mpVertexDeclaration->AddElement( nNumVertexBuffer, 0, VET_UBYTE4, VEM_DEFAULT, VEU_BLENDINDICES, 0 );

			mppVertexBuffer[nNumVertexBuffer] = BufferManager::mpSingleton->AddVertexBuffer();
			mppVertexBuffer[nNumVertexBuffer]->CreateVertexBuffer( sizeof(DWORD), nVerNum, false, mbSoftVertexProc );
			++nNumVertexBuffer;
		}

		//建立法线流
		if( bHaveNormal )
		{
			mpVertexDeclaration->AddElement( nNumVertexBuffer, 0, VET_FLOAT3, VEM_DEFAULT, VEU_NORMAL, 0 );

			mppVertexBuffer[nNumVertexBuffer] = BufferManager::mpSingleton->AddVertexBuffer();
			mppVertexBuffer[nNumVertexBuffer]->CreateVertexBuffer( sizeof(float)*3, nVerNum, false, mbSoftVertexProc );
			++nNumVertexBuffer;
		}

		//建立颜色流
		if( bHaveColor )
		{
			mpVertexDeclaration->AddElement( nNumVertexBuffer, 0, VET_ARGBCOLOR, VEM_DEFAULT, VEU_COLOR, 0 );

			mppVertexBuffer[nNumVertexBuffer] = BufferManager::mpSingleton->AddVertexBuffer();
			mppVertexBuffer[nNumVertexBuffer]->CreateVertexBuffer( sizeof(DWORD), nVerNum, false, mbSoftVertexProc );
			++nNumVertexBuffer;
		}

		//建立纹理坐标流
		if( bHaveUV )
		{
			mpVertexDeclaration->AddElement( nNumVertexBuffer, 0, VET_FLOAT2, VEM_DEFAULT, VEU_TEXCOORD, 0 );

			mppVertexBuffer[nNumVertexBuffer] = BufferManager::mpSingleton->AddVertexBuffer();
			mppVertexBuffer[nNumVertexBuffer]->CreateVertexBuffer( sizeof(float)*2, nVerNum, false, mbSoftVertexProc );
			++nNumVertexBuffer;
		}


		//完成顶点声明创建
		mpVertexDeclaration->CreateVertexDeclaration();

		//创建网格索引缓存
		mpIndexBuffer = BufferManager::mpSingleton->AddIndexBuffer();
		mpIndexBuffer->CreateIndexBuffer( nIndexNum, IT_INDEX16, false, mbSoftVertexProc );
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除顶点流
	void Geometry::DestroyMeshStream()
	{
		for( UINT i=0; i<mNumVertexBuffer; ++i )
		{
			if( mppVertexBuffer[i] != NULL )
			{
				BufferManager::mpSingleton->DelVertexBuffer( mppVertexBuffer[i] );
				mppVertexBuffer[i] = NULL;
			}
		}

		if( mpIndexBuffer != NULL )
		{
			BufferManager::mpSingleton->DelIndexBuffer( mpIndexBuffer );
			mpIndexBuffer = NULL;
		}

		if( mpVertexDeclaration != NULL )
		{
			BufferManager::mpSingleton->DelVertexDeclaration( mpVertexDeclaration );
			mpVertexDeclaration = NULL;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//初始化渲染实例列表
	void Geometry::InitializeInstanceList( UINT nInitNum, UINT nAddNum )
	{
		mRenderInstList.Initialize( nInitNum, nAddNum );
	}
	//--------------------------------------------------------------------------------------------------------------
	//释放渲染实例列表
	void Geometry::ReleaseInstanceList()
	{
		mRenderInstList.Release();
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建渲染实例
	RenderInstance* Geometry::CreateInstance()
	{
		RenderInstance* pInstance = new RenderInstance( this );
		*mRenderInstList.Push() = pInstance;
		return pInstance;
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除指定渲染实例
	void Geometry::DeleteInstance( RenderInstance* pInstance )
	{
		RenderInstList::Iterator it = mRenderInstList.Begin();
		RenderInstList::Iterator end = mRenderInstList.End();
		for(; it!=end; ++it )
		{
			if( *it == pInstance )
			{
				mRenderInstList.Erase( it );
				delete *it;
				return;
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除所有渲染实例
	void Geometry::DeleteAllInstance()
	{
		RenderInstList::Iterator it = mRenderInstList.Begin();
		RenderInstList::Iterator end = mRenderInstList.End();
		for(; it!=end; ++it )
		{
			if( *it != NULL )
				delete *it;
		}

		mRenderInstList.Clear();
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
