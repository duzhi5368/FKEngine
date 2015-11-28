/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	ArrowEmitter
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	箭型粒子发射器
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//箭型粒子发射器
	//class FK_DLL ArrowEmitter : public Emitter
	//{
	//public:
	//	//箭型粒子发射器参数
	//	struct Param
	//	{
	//		//粒子发射器基本参数
	//		Emitter::Param	sBaseParam;

	//		float	fGenerateTime;	//每个粒子的生成时间
	//		float	fMaxEmission;	//最大散发速度
	//		float	fMinEmission;	//最小散发速度
	//		float	fFlowSpeed;		//吹动速度
	//		float	fFlowFactor;	//吹动加速系数
	//	};

	//	//粒子数据
	//	struct ParticleData
	//	{
	//		Vector3		vPosition;		//粒子位置
	//		Vector3		vGravity;		//重力加速度
	//		float		fEmissionX;		//散发速度 X
	//		float		fEmissionZ;		//散发速度 Z
	//		float		fFlowSpeed;		//吹动速度
	//		float		fSize;			//粒子尺寸
	//		float		fLife;			//粒子生命
	//	};
	//	typedef Stack< ParticleData >	ParticleDatas;

	//protected:
	//	ParticleDatas	mParticleDatas;	//粒子数据

	//	float			mfIntervalTime;	//上次发射的间隔时间

	//	float			mfGenerateTime;	//每个粒子的生成时间
	//	float			mfMaxEmission;	//最大散发速度
	//	float			mfMinEmission;	//最小散发速度
	//	float			mfFlowSpeed;	//吹动速度
	//	float			mfFlowFactor;	//吹动加速系数

	//public:
	//	ArrowEmitter( Param* pParam );
	//	~ArrowEmitter();

	//protected:
	//	//更新发射器
	//	void _UpdateEmitter( float fFrameTime, Vector3* pFaceDir, Vector3* pGravity );
	//};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
