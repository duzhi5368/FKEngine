/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	BillboardEmitter
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	公告板粒子发射器
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//公告板粒子
	class FK_DLL BillboardParticle : public Particle
	{
	public:
		//公告板粒子创建参数
		struct Param
		{
			Material*	pMaterial;		//粒子材质
			float		fSizeX;			//粒子尺寸 X
			float		fSizeY;			//粒子尺寸 Y
		};

	public:
		Material*		mpMaterial;		//粒子材质
		float			mfSizeX;		//粒子尺寸 X
		float			mfSizeY;		//粒子尺寸 Y

	public:
		BillboardParticle( const Param* pParam );
		~BillboardParticle();

		//粒子创建函数
		static Particle* Creator( const void* pParam );
	};
	//--------------------------------------------------------------------------------------------------------------
	//公告板粒子发射器
	class FK_DLL BillboardEmitter : public SpriteEmitter
	{
	public:
		//公告板粒子发射器创建参数
		struct Param
		{
			Particle*		pParticle;
			Vector3*		pPosition;
			UINT			nNumBillboard;
		};

	protected:
		Vector3*		mpParticleData;	//粒子实例数据

	public:
		BillboardEmitter( const Param* pParam );
		~BillboardEmitter();

		//粒子发射器创建函数
		static Emitter* Creator( const void* pParam );

	protected:
		//更新发射器
		void _UpdateEmitter( const float fFrameTime );

		//更新精灵粒子方向
		UINT _UpdateSprite( const Camera* pCamera );
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
