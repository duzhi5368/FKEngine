/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Emitter
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	粒子发射器
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================
#include "../RenderWrapper/RenderObject.h"
#include "../RenderWrapper/RenderInstance.h"
//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//粒子发射器
	class FK_DLL Emitter : public RenderObject, public RenderInstance
	{
	public:
		friend class ParticleSystem;

		//粒子发射器类型
		enum Type
		{
			PET_Sprite,		//精灵粒子发射器
			PET_Mesh,		//网格粒子发射器
		};

		//粒子发射器创建函数
		typedef Emitter* (*FnCreator)( const void* pParam );

	public:
		Type			mType;			//粒子发射器类型

		Particle*		mpParticle;		//粒子指针
		UINT			mNumParticle;	//当前粒子数量

	public:
		Emitter( Type eType, Particle* pParticle );
		virtual ~Emitter() = 0;

	protected:
		//更新发射器
		virtual void _UpdateEmitter( const float fFrameTime ) = 0;

		//渲染物体
		void _RenderObj( const RenderParam* pParam, const Matrix4* pNodeMatrix );

		//渲染动画物体
		void _RenderAnimObj( const RenderParam* pParam, const AnimControl* pAnimControl, const Matrix4* pNodeMatrix );
	};
	//--------------------------------------------------------------------------------------------------------------
	typedef Stack< Emitter* >	EmitterList;
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
