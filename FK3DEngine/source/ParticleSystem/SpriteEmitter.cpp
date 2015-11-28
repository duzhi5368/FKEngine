/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	SpriteEmitter
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../../include/ParticleSystem/ParticleSystemInsideHeader.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	SpriteEmitter::SpriteEmitter( Particle* pParticle )
		: Emitter				( PET_Sprite, pParticle )
		, mpVertexData			(NULL)
		, mpIndexData			(NULL)
		, mNumRenderParticle	(0)
	{
		//设置顶点流
		AllocStreamBuf( 1 );

		mppVertexBuffer[0]	= ParticleSystem::mpSingleton->mpVertexBuffer;
		mpIndexBuffer		= ParticleSystem::mpSingleton->mpIndexBuffer;
		mpVertexDeclaration	= ParticleSystem::mpSingleton->mpVertexDeclaration;

		//设置渲染参数
		AllocRenderParamBuf();
		mpRenderParam->eDrawType				= RenderParam::DT_Basic;

		mpRenderParam->sBasicParam.ePrimType	= PT_TRIANGLE_LIST;
		mpRenderParam->sBasicParam.nMinIndex	= 0;
	}
	//--------------------------------------------------------------------------------------------------------------
	SpriteEmitter::~SpriteEmitter()
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	//根据精灵粒子屏幕空间深度重新排序顶点数据
	void SpriteEmitter::_ReorderVertexData()
	{
		if( mNumRenderParticle == 0 )
		{
			mbVisable = false;
			return;
		}

		mbVisable = true;
		UINT nNumVertex = mNumRenderParticle * 4;
		static Vertex tmpVertex;

		//如果是透明材质
		if( !mpRenderParam->pMaterial->mRenderState.mbDepthTestEnable )
		{
			//按照从远到近的顺序排列粒子面片（选择排序法）
			for( UINT i=0; i<nNumVertex; i+=4 )
			{
				int nMaxIndex = i;

				for( UINT j=i; j<nNumVertex; j+=4 )
					if( mpVertexData[nMaxIndex].z < mpVertexData[j].z )
						nMaxIndex = j;

				if( nMaxIndex == i )
					continue;

				memcpy( &tmpVertex, &mpVertexData[i], sizeof(Vertex)*4 );
				memcpy( &mpVertexData[i], &mpVertexData[nMaxIndex], sizeof(Vertex)*4 );
				memcpy( &mpVertexData[nMaxIndex], &tmpVertex, sizeof(Vertex)*4 );
			}
		}
		//如果不是透明材质
		else
		{
			//按照从近到远的顺序排列粒子面片（选择排序法）
			for( UINT i=0; i<nNumVertex; i+=4 )
			{
				int nMinIndex = i;

				for( UINT j=i; j<nNumVertex; j+=4 )
					if( mpVertexData[nMinIndex].z > mpVertexData[j].z )
						nMinIndex = j;

				if( nMinIndex == i )
					continue;

				memcpy( &tmpVertex, &mpVertexData[i], sizeof(Vertex)*4 );
				memcpy( &mpVertexData[i], &mpVertexData[nMinIndex], sizeof(Vertex)*4 );
				memcpy( &mpVertexData[nMinIndex], &tmpVertex, sizeof(Vertex)*4 );
			}
		}

		//生成索引数据
		WORD wIndex = 0;
		WORD* pIndexPtr = mpIndexData;
		for( UINT i=0; i<mNumRenderParticle; ++i )
		{
			*pIndexPtr++ = wIndex;
			*pIndexPtr++ = wIndex + 1;
			*pIndexPtr++ = wIndex + 2;
			*pIndexPtr++ = wIndex + 3;
			*pIndexPtr++ = wIndex + 2;
			*pIndexPtr++ = wIndex + 1;
			wIndex += 4;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//更新精灵粒子发射器顶点数据
	void SpriteEmitter::_UpdateVertexData( Vertex* pVerBuf, WORD* pIndexBuf, UINT nVertexOffset, UINT nIndexOffset )
	{
		memcpy( pVerBuf, mpVertexData, mNumRenderParticle * sizeof(Vertex) * 4 );
		memcpy( pIndexBuf, mpIndexData, mNumRenderParticle * sizeof(WORD) * 6 );

		mpRenderParam->sBasicParam.nVerOffset	= nVertexOffset;
		mpRenderParam->sBasicParam.nIndexOffset	= nIndexOffset;
		mpRenderParam->sBasicParam.nVerCount	= mNumRenderParticle << 2;
		mpRenderParam->sBasicParam.nPrimCount	= mNumRenderParticle << 1;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
