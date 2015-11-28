/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	ParticleSystem
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
	ParticleSystem::ParticleSystem( UINT nMaxNumSpriteParticle )
		: mpVertexBuffer		(NULL)
		, mpIndexBuffer			(NULL)
		, mpVertexDeclaration	(NULL)
		, mMaxNumSParticle		(nMaxNumSpriteParticle)
		, mOffsetSParticle		(0)
		, mRenderSParticle		(0)
	{
		mParticleList.Initialize( 10, 10 );
		mEmitterList.Initialize( 60, 20 );

		//创建粒子顶点声明
		mpVertexDeclaration = BufferManager::mpSingleton->AddVertexDeclaration();

		mpVertexDeclaration->AddElement( 0, 0, VET_FLOAT4, VEM_DEFAULT, VEU_POSITIONT, 0 );
		mpVertexDeclaration->AddElement( 0, 16, VET_ARGBCOLOR, VEM_DEFAULT, VEU_COLOR, 0 );
		mpVertexDeclaration->AddElement( 0, 20, VET_FLOAT2, VEM_DEFAULT, VEU_TEXCOORD, 0 );
		mpVertexDeclaration->CreateVertexDeclaration();

		//创建粒子顶点缓存和索引缓存
		mpVertexBuffer = BufferManager::mpSingleton->AddVertexBuffer();
		mpIndexBuffer = BufferManager::mpSingleton->AddIndexBuffer();

		mpVertexBuffer->CreateVertexBuffer( sizeof(SpriteEmitter::Vertex), nMaxNumSpriteParticle * 4, true );
		mpIndexBuffer->CreateIndexBuffer( nMaxNumSpriteParticle * 6, IT_INDEX16, true );
	}
	//--------------------------------------------------------------------------------------------------------------
	ParticleSystem::~ParticleSystem()
	{
		DeleteAllParticle();
		DeleteAllEmitter();

		mParticleList.Release();
		mEmitterList.Release();

		if( mpIndexBuffer != NULL )
		{
			BufferManager::mpSingleton->DelIndexBuffer( mpIndexBuffer );
			mpIndexBuffer = NULL;
		}

		if( mpVertexBuffer != NULL )
		{
			BufferManager::mpSingleton->DelVertexBuffer( mpVertexBuffer );
			mpVertexBuffer = NULL;
		}

		if( mpVertexDeclaration != NULL )
		{
			BufferManager::mpSingleton->DelVertexDeclaration( mpVertexDeclaration );
			mpVertexDeclaration = NULL;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建粒子
	Particle* ParticleSystem::CreateParticle( Particle::FnCreator fnCreator, void* pParam )
	{
		Particle* pParticle = fnCreator( pParam );
		*mParticleList.Push() = pParticle;
		return pParticle;
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除粒子
	void ParticleSystem::DeleteParticle( Particle* pParticle )
	{
		if( pParticle == NULL )
			return;

		ParticleList::Iterator it = mParticleList.Begin();
		ParticleList::Iterator end = mParticleList.End();
		for(; it!=end; ++it )
		{
			if( *it == pParticle )
			{
				mParticleList.Erase( it );
				delete pParticle;
				return;
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除所有粒子
	void ParticleSystem::DeleteAllParticle()
	{
		ParticleList::Iterator it = mParticleList.Begin();
		ParticleList::Iterator end = mParticleList.End();
		for(; it!=end; ++it )
		{
			if( *it != NULL )
				delete *it;
		}

		mParticleList.Clear();
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建粒子发射器
	Emitter* ParticleSystem::CreateEmitter( Emitter::FnCreator fnCreator, void* pParam )
	{
		Emitter* pEmitter = fnCreator( pParam );
		*mEmitterList.Push() = pEmitter;
		return pEmitter;
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除粒子发射器
	void ParticleSystem::DeleteEmitter( Emitter* pEmitter )
	{
		if( pEmitter == NULL )
			return;

		EmitterList::Iterator it = mEmitterList.Begin();
		EmitterList::Iterator end = mEmitterList.End();
		for(; it!=end; ++it )
		{
			if( *it == pEmitter )
			{
				mEmitterList.Erase( it );
				delete pEmitter;
				return;
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除所有粒子发射器
	void ParticleSystem::DeleteAllEmitter()
	{
		EmitterList::Iterator it = mEmitterList.Begin();
		EmitterList::Iterator end = mEmitterList.End();
		for(; it!=end; ++it )
		{
			if( *it != NULL )
				delete *it;
		}

		mEmitterList.Clear();
	}
	//--------------------------------------------------------------------------------------------------------------
	//更新发射器
	void ParticleSystem::_UpdateEmitter( float fFrameTime )
	{
		//更新所有粒子发射器
		EmitterList::Iterator it = mEmitterList.Begin();
		EmitterList::Iterator end = mEmitterList.End();
		for(; it!=end; ++it )
		{
			//更新粒子发射器
			(*it)->_UpdateEmitter( fFrameTime );
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//更新精灵粒子方向
	void ParticleSystem::_UpdateSprite( const Camera* pCamera )
	{
		mRenderSParticle = 0;

		//更新精灵粒子渲染数据
		EmitterList::Iterator begin = mEmitterList.Begin();
		EmitterList::Iterator end = mEmitterList.End();
		for( EmitterList::Iterator it=begin; it!=end; ++it )
		{
			Emitter* pEmitter = *it;

			//如果是精灵粒子发射器
			if( pEmitter->mType == Emitter::PET_Sprite )
			{
				//如果粒子未挂接在场景节点或者数量为零则直接略过
				if( pEmitter->mpSceneNode == NULL || pEmitter->mNumParticle == 0 )
				{
					pEmitter->mbVisable = false;
					continue;
				}

				//更新粒子渲染数据
				SpriteEmitter* pSpriteEmitter = reinterpret_cast< SpriteEmitter* >( pEmitter );
				mRenderSParticle += pSpriteEmitter->_UpdateSprite( pCamera );
			}
		}

		//如果没有需要渲染的精灵粒子则返回
		if( mRenderSParticle == 0 )
			return;

		//如果超过了最大限则返回起始位置锁定，并且在这一帧使用 D3DLOCK_DISCARD 锁定。
		bool bNoOverwrite = true;
		if( mOffsetSParticle + mRenderSParticle > mMaxNumSParticle )
		{
			mOffsetSParticle = 0;
			bNoOverwrite = false;
		}

		//锁定顶点缓存和索引缓存
		UINT nVertexOffset = mOffsetSParticle * 4;
		UINT nIndexOffset = mOffsetSParticle * 6;

		SpriteEmitter::Vertex* pVertexPtr = (SpriteEmitter::Vertex*)mpVertexBuffer->Lock( nVertexOffset,
			mRenderSParticle * 4, bNoOverwrite );

		WORD* pIndexPtr = (WORD*)mpIndexBuffer->Lock( nIndexOffset, mRenderSParticle * 6, bNoOverwrite );

		//复制精灵粒子顶点数据
		for( EmitterList::Iterator it=begin; it!=end; ++it )
		{
			Emitter* pEmitter = *it;

			//如果是精灵粒子发射器
			if( pEmitter->mType == Emitter::PET_Sprite )
			{
				SpriteEmitter* pSpriteEmitter = reinterpret_cast< SpriteEmitter* >( pEmitter );

				//如果粒子不可见则略过
				if( !pSpriteEmitter->mbVisable )
					continue;

				//复制粒子顶点数据
				pSpriteEmitter->_UpdateVertexData( pVertexPtr, pIndexPtr, nVertexOffset, nIndexOffset );

				//计算数据偏移量
				UINT nNumVertex = pSpriteEmitter->mNumRenderParticle * 4;
				UINT nNumIndex = pSpriteEmitter->mNumRenderParticle * 6;

				pVertexPtr += nNumVertex;
				pIndexPtr += nNumIndex;

				nVertexOffset += nNumVertex;
				nIndexOffset += nNumIndex;
			}
		}

		//解锁顶点缓存和索引缓存
		mpVertexBuffer->Unlock();
		mpIndexBuffer->Unlock();

		mOffsetSParticle += mRenderSParticle;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
