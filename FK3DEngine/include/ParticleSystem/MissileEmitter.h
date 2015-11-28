/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	MissileEmitter
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	导弹粒子发射器
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//导弹粒子
	class FK_DLL MissileParticle : public Particle
	{
	public:
		//导弹粒子创建参数
		struct Param
		{
			Material*	pMaterial;		//粒子材质
			float		fLife;			//粒子寿命
			int			nBeginAlpha;	//初始透明度
			int			nEndAlpha;		//结束透明度
			float		fBeginSize;		//初始尺寸
			float		fEndSize;		//结束尺寸
		};

	public:
		Material*		mpMaterial;		//粒子材质
		float			mfLife;			//粒子寿命
		float			mfSize;			//粒子尺寸
		int				mnAlpha;		//粒子透明度
		float			mfSizeFactor;	//粒子尺寸变化系数
		float			mfAlphaFactor;	//粒子透明度变化系数

	public:
		MissileParticle( const Param* pParam );
		~MissileParticle();

		//粒子创建函数
		static Particle* Creator( const void* pParam );
	};
	//--------------------------------------------------------------------------------------------------------------
	//导弹粒子发射器
	class FK_DLL MissileEmitter : public SpriteEmitter
	{
	public:
		//导弹粒子发射器创建参数
		struct Param
		{
			Particle*	pParticle;
			float		fEmitRate;
		};

		//导弹粒子实例数据
		struct Data
		{
			Vector3		vPosition;		//粒子位置
			float		fLifeTime;		//粒子寿命时间
			float		fSize;			//粒子尺寸（直径）
			int			nAlpha;			//粒子透明度
		};
		typedef Stack< Data >	DataStack;

	protected:
		DataStack		mParticleData;	//粒子实例数据
		BYTEStack		mVertexData;	//粒子顶点数据

		float			mfEmitRate;		//粒子发射速率
		float			mfInterval;		//发射间隔时间

	public:
		MissileEmitter( const Param* pParam );

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
