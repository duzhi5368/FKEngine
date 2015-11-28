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
	//ArrowEmitter::ArrowEmitter( Param* pParam )
	//	: Emitter	( &pParam->sBaseParam )
	//	, mfIntervalTime	(0.0f)
	//{
	//	memcpy( &mfGenerateTime, &pParam->fGenerateTime, sizeof(float)*5 );

	//	mParticleDatas.Initialize( pParam->sBaseParam.nMaxParticleNum, 100 );
	//}
	////--------------------------------------------------------------------------------------------------------------
	//ArrowEmitter::~ArrowEmitter()
	//{
	//	mParticleDatas.Release();
	//}
	////--------------------------------------------------------------------------------------------------------------
	////更新发射器
	//UINT ArrowEmitter::_UpdateEmitter( float fFrameTime, Vector3* pFaceDir, Vector3* pGravity )
	//{
	//	//重置粒子数量
	//	mCurParticleNum = 0;

	//	////增加帧延时
	//	//mfIntervalTime += fFrameTime;

	//	////如果帧延时大于粒子生成时间
	//	//while( mfIntervalTime >= mfGenerateTime )
	//	//{
	//	//	mfIntervalTime -= mfGenerateTime;

	//	//	//生成新的粒子
	//	//	ParticleData* pParticleData = mParticleDatas.Push();

	//	//	pParticleData->fSize = mpParticle->mSize;
	//	//	pParticleData->fLife = mpParticle->mLife;

	//	//	memset( &pParticleData->vPosition, 0, sizeof(Vector3)*2 );

	//	//	//生成粒子的随机散发速度
	//	//	pParticleData->fEmissionX = Misc::GetFloatRandomValue( mfMaxEmission, mfMinEmission );
	//	//	pParticleData->fEmissionZ = Misc::GetFloatRandomValue( mfMaxEmission, mfMinEmission );

	//	//	//设置吹动速度
	//	//	pParticleData->fFlowSpeed = mfFlowSpeed;
	//	//}

	//	////更新粒子数据
	//	//int nNumDead = 0;
	//	//ParticleVertex* pVertexPtr = mpVertexData;
	//	//WORD* pIndexPtr = mpIndexData;

	//	//ParticleDatas::Iterator it = mParticleDatas.Begin();
	//	//ParticleDatas::Iterator end = mParticleDatas.End();
	//	//for(; it!=end; ++it )
	//	//{
	//	//	ParticleData* pParticle = *it;

	//	//	//计算当前粒子生命
	//	//	pParticle->fLife -= mpParticle->mFade * fFrameTime;

	//	//	//如果粒子已死亡
	//	//	if( pParticle->fLife <= 0 )
	//	//	{
	//	//		++nNumDead;
	//	//		continue;
	//	//	}

	//	//	//计算粒子大小
	//	//	pParticle->fSize += mpParticle->mSizeFactor * fFrameTime;

	//	//	//计算粒子吹动加速度
	//	//	pParticle->fFlowSpeed *= mfFlowFactor;

	//	//	//计算粒子重力加速度
	//	//	pParticle->vGravity += *pGravity;

	//	//	//计算粒子坐标
	//	//	pParticle->vPosition += pParticle->vGravity;
	//	//	pParticle->vPosition.x += pParticle->fEmissionX;
	//	//	pParticle->vPosition.z += pParticle->fEmissionZ;
	//	//	pParticle->vPosition.y += pParticle->fFlowSpeed;

	//	//	//如果还未到达粒子数量最大限
	//	//	if( mCurParticleNum < mMaxParticleNum )
	//	//	{
	//	//		++mCurParticleNum;

	//	//		//生成粒子顶点数据
	//	//		float fSize = pParticle->fSize;

	//	//		//左上
	//	//		//pVertexPtr->vPosition.x = pParticle->vPosition.x * 
	//	//}

	//	////删除死亡粒子
	//	//mParticleDatas.PopFront( nNumDead );

	//	return mCurParticleNum;
	//}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
