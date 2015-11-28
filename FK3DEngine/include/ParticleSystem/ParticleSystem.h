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
*	文件说明：	粒子系统
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//粒子系统
	class FK_DLL ParticleSystem : public Singleton< ParticleSystem >
	{
	public:
		ParticleList		mParticleList;		//粒子列表
		EmitterList			mEmitterList;		//粒子发射器列表

		VertexBuffer*		mpVertexBuffer;		//精灵粒子顶点缓存
		IndexBuffer*		mpIndexBuffer;		//精灵粒子索引缓存
		VertexDeclaration*	mpVertexDeclaration;//精灵粒子顶点声明

		UINT				mMaxNumSParticle;	//缓存最大容纳精灵粒子数
		UINT				mOffsetSParticle;	//当前帧偏移精灵粒子数
		UINT				mRenderSParticle;	//当前帧渲染精灵粒子数

	public:
		ParticleSystem( UINT nMaxNumSpriteParticle );
		virtual ~ParticleSystem();

		//创建粒子
		Particle* CreateParticle( Particle::FnCreator fnCreator, void* pParam = NULL );

		//删除粒子
		void DeleteParticle( Particle* pParticle );

		//删除所有粒子
		void DeleteAllParticle();

		//创建粒子发射器
		Emitter* CreateEmitter( Emitter::FnCreator fnCreator, void* pParam = NULL );

		//删除粒子发射器
		void DeleteEmitter( Emitter* pEmitter );

		//删除所有粒子发射器
		void DeleteAllEmitter();

		//更新发射器
		void _UpdateEmitter( float fFrameTime );

		//更新精灵粒子方向
		void _UpdateSprite( const Camera* pCamera );
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
