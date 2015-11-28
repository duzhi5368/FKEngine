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
	MissileParticle::MissileParticle( const Param* pParam )
		: Particle		(PT_Sprite)
		, mpMaterial	(pParam->pMaterial)
		, mfLife		(pParam->fLife)
		, mfSize		(pParam->fBeginSize)
		, mnAlpha		(pParam->nBeginAlpha)
	{
		//计算粒子变化系数
		mfSizeFactor = ( pParam->fEndSize - pParam->fBeginSize ) / pParam->fLife;
		mfAlphaFactor = (float)( pParam->nEndAlpha - pParam->nBeginAlpha ) / pParam->fLife;

		//增加粒子材质使用计数
		mpMaterial->AddRef();
	}
	//--------------------------------------------------------------------------------------------------------------
	MissileParticle::~MissileParticle()
	{
		//减少粒子材质使用计数
		mpMaterial->DelRef();
	}
	//--------------------------------------------------------------------------------------------------------------
	//粒子创建函数
	Particle* MissileParticle::Creator( const void* pParam )
	{
		return new MissileParticle( (const Param*)pParam );
	}
	//--------------------------------------------------------------------------------------------------------------
	MissileEmitter::MissileEmitter( const Param* pParam )
		: SpriteEmitter		(pParam->pParticle)
		, mfEmitRate		(pParam->fEmitRate)
		, mfInterval		(0.0f)
	{
		MissileParticle* pParticle = (MissileParticle*)mpParticle;
		UINT nMaxParticle = CeilToInt( pParticle->mfLife / mfEmitRate );

		mParticleData.Initialize( nMaxParticle, nMaxParticle );

		UINT nVertexDataLen = nMaxParticle * ( sizeof(Vertex)*4 + sizeof(WORD)*6 );
		mVertexData.Initialize( nVertexDataLen, nVertexDataLen );

		//设置渲染参数
		AllocMaterialBuf();
		mppMaterial[0] = pParticle->mpMaterial;

		mpRenderParam->pMaterial = pParticle->mpMaterial;
	}
	//--------------------------------------------------------------------------------------------------------------
	//粒子发射器创建函数
	Emitter* MissileEmitter::Creator( const void* pParam )
	{
		return new MissileEmitter( (const Param*)pParam );
	}
	//--------------------------------------------------------------------------------------------------------------
	//更新发射器
	void MissileEmitter::_UpdateEmitter( const float fFrameTime )
	{
		MissileParticle* pParticle = (MissileParticle*)mpParticle;

		//更新粒子数据
		const float fSizeFactor = pParticle->mfSizeFactor;
		const float fAlphaFactor = pParticle->mfAlphaFactor;

		UINT nNumDead = 0;

		DataStack::Iterator it = mParticleData.Begin();
		DataStack::Iterator end = mParticleData.End();
		for(; it!=end; ++it )
		{
			Data* pData = it;

			//计算当前粒子生命
			pData->fLifeTime += fFrameTime;
			const float fLifeTime = pData->fLifeTime;

			//如果粒子已死亡
			if( fLifeTime >= pParticle->mfLife )
			{
				++nNumDead;
				continue;
			}

			//计算尺寸
			pData->fSize = pParticle->mfSize + fSizeFactor * fLifeTime;

			//计算透明值
			pData->nAlpha = pParticle->mnAlpha + FloorToInt( fAlphaFactor * fLifeTime );
		}

		//删除死亡粒子
		mParticleData.PopFront( nNumDead );

		mNumParticle = mParticleData.Size();

		//增加帧延时
		mfInterval += fFrameTime;

		//如果帧延时大于粒子生成时间
		while( mfInterval >= mfEmitRate )
		{
			mfInterval -= mfEmitRate;

			//如果剩余时间小于粒子寿命
			if( mfInterval < pParticle->mfLife )
			{
				//生成新的粒子
				Data* pData = mParticleData.Push();

				pData->fLifeTime = mfInterval;

				pData->vPosition.x = 0.0f;
				pData->vPosition.y = 10.0f;
				pData->vPosition.z = 0.0f;
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//更新精灵粒子方向
	UINT MissileEmitter::_UpdateSprite( const Camera* pCamera )
	{
		//更新顶点数据
		mVertexData.Clear();

		//计算顶点变换矩阵
		static Matrix4 TransformMat;
		static Vector4 vTransPos;
		const Viewport* pViewport = RenderSystem::mpSingleton->mpViewport;

		TransformMat = mpSceneNode->mWorldMatrix;
		TransformMat *= pCamera->mViewProjMatrix;
		TransformMat *= pViewport->mViewportMat;

		//计算每个粒子的顶点数据
		mNumRenderParticle = 0;
		Data* pData = mParticleData.Begin();
		for( UINT i=0; i<mNumParticle; ++i, ++pData )
		{
			//计算颜色值
			const DWORD dwAlpha = ( (DWORD)pData->nAlpha ) << 24;
			DWORD dwColor = 0x00FFFFFF | dwAlpha;

			//转换粒子坐标到屏幕空间
			vTransPos = pData->vPosition;
			vTransPos.z = 20.0f * pData->fLifeTime;

			vTransPos *= TransformMat;

			const float fInvW = 1.0f / vTransPos.w;
			vTransPos.x *= fInvW;
			vTransPos.y *= fInvW;
			vTransPos.z *= fInvW;
			vTransPos.w = fInvW;

			//计算粒子大小
			const float fDist = ( pData->vPosition - pCamera->mViewPos ).GetLength();
			const float fHalfSize = pViewport->mHeight * pData->fSize * 0.5f / fDist; 

			//根据粒子屏幕坐标进行裁剪
			if( vTransPos.x < pViewport->mLeft - fHalfSize || vTransPos.x > pViewport->mWidth + fHalfSize ||
				vTransPos.y < pViewport->mLeft - fHalfSize || vTransPos.y > pViewport->mHeight + fHalfSize ||
				vTransPos.z < pViewport->mMinZ || vTransPos.z > pViewport->mMaxZ )
				continue;

			//添加新的粒子顶点数据
			++mNumRenderParticle;
			Vertex* pVertexPtr = (Vertex*)mVertexData.Push( sizeof(Vertex)*4 );

			//左下角
			pVertexPtr[0].x = vTransPos.x - fHalfSize;
			pVertexPtr[0].y = vTransPos.y + fHalfSize;
			pVertexPtr[0].z = vTransPos.z;
			pVertexPtr[0].w = vTransPos.w;
			pVertexPtr[0].fU = 0.0f;
			pVertexPtr[0].fV = 1.0f;
			pVertexPtr[0].dwColor = dwColor;

			//左上角
			pVertexPtr[1].x = vTransPos.x - fHalfSize;
			pVertexPtr[1].y = vTransPos.y - fHalfSize;
			pVertexPtr[1].z = vTransPos.z;
			pVertexPtr[1].w = vTransPos.w;
			pVertexPtr[1].fU = 0.0f;
			pVertexPtr[1].fV = 0.0f;
			pVertexPtr[1].dwColor = dwColor;

			//右下角
			pVertexPtr[2].x = vTransPos.x + fHalfSize;
			pVertexPtr[2].y = vTransPos.y + fHalfSize;
			pVertexPtr[2].z = vTransPos.z;
			pVertexPtr[2].w = vTransPos.w;
			pVertexPtr[2].fU = 1.0f;
			pVertexPtr[2].fV = 1.0f;
			pVertexPtr[2].dwColor = dwColor;

			//右上角
			pVertexPtr[3].x = vTransPos.x + fHalfSize;
			pVertexPtr[3].y = vTransPos.y - fHalfSize;
			pVertexPtr[3].z = vTransPos.z;
			pVertexPtr[3].w = vTransPos.w;
			pVertexPtr[3].fU = 1.0f;
			pVertexPtr[3].fV = 0.0f;
			pVertexPtr[3].dwColor = dwColor;
		}

		//存储顶点数据指针
		mpIndexData = (WORD*)mVertexData.Push( mNumRenderParticle * sizeof(WORD) * 6 );
		mpVertexData = (Vertex*)mVertexData.Begin();

		_ReorderVertexData();

		if( mNumRenderParticle == 0 )
			mpRenderParam->sBasicParam.nPrimCount = 0;

		return mNumRenderParticle;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
