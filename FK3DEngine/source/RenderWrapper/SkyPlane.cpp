/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	SkyPlane
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
	SkyPlane::SkyPlane()
		: mpBackgroundTexture	(NULL)
		, mpSurfaceTexture		(NULL)
		, mpVD					(NULL)
		, mpPositionVB			(NULL)
		, mpTexcoordVB			(NULL)
		, mCloudUFactor			(0.005f)
		, mCloudVFactor			(0.005f)
		, mCloudU				(0.0f)
		, mCloudV				(0.0f)
	{
		BufferManager* pBufMgr = BufferManager::GetSingletonPtr();

		//创建顶点声明
		mpVD = pBufMgr->AddVertexDeclaration();
		mpVD->AddElement( 0, 0, VET_FLOAT3, VEM_DEFAULT, VEU_POSITION, 0 );
		mpVD->AddElement( 1, 0, VET_FLOAT2, VEM_DEFAULT, VEU_TEXCOORD, 0 );
		mpVD->CreateVertexDeclaration();

		//创建顶点缓存
		mpPositionVB = pBufMgr->AddVertexBuffer();
		mpPositionVB->CreateVertexBuffer( sizeof(float)*3, 8, false );

		mpTexcoordVB = pBufMgr->AddVertexBuffer();
		mpTexcoordVB->CreateVertexBuffer( sizeof(float)*2, 8, true );


		//天空层矩形坐标
		float pRectPos [] =
		{
			-800.0f,	10.0f,		800.0f,		//远左
			-800.0f,	10.0f,		-800.0f,	//近左
			800.0f,		10.0f,		-800.0f,	//近右
			800.0f,		10.0f,		800.0f,		//远右
		};

		//生成顶点坐标
		float* pPos = (float*)mpPositionVB->Lock( 0, 0 );

		for( UINT i=0; i<2; ++i )
		{
			memcpy( pPos, pRectPos, sizeof(pRectPos) );
			pPos += 12;
		}

		mpPositionVB->Unlock();


		//生成纹理坐标
		float* pUV = mpUV;

		*pUV++ = 0.0f;		*pUV++ = 1.0f;
		*pUV++ = 0.0f;		*pUV++ = 0.0f;
		*pUV++ = 1.0f;		*pUV++ = 0.0f;
		*pUV++ = 1.0f;		*pUV++ = 1.0f;
	}
	//--------------------------------------------------------------------------------------------------------------
	SkyPlane::~SkyPlane()
	{
		BufferManager* pBufMgr = BufferManager::GetSingletonPtr();

		//释放顶点声明及顶点缓存
		pBufMgr->DelVertexDeclaration( mpVD );
		pBufMgr->DelVertexBuffer( mpPositionVB );
		pBufMgr->DelVertexBuffer( mpTexcoordVB );
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置天空纹理
	void SkyPlane::SetSkyTexture( Texture* pBackground, Texture* pSurface )
	{
		mpBackgroundTexture = pBackground;
		mpSurfaceTexture = pSurface;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置移动方向
	void SkyPlane::SetMoveDirection( float fMoveFront, float fMoveRight )
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	//更新天空体
	void SkyPlane::_UpdateSky( float fTimeSinceLastFrame )
	{
		//根据系数改变纹理层 UV 值
		mCloudU += mCloudUFactor * fTimeSinceLastFrame;
		mCloudV += mCloudVFactor * fTimeSinceLastFrame;

		if( mCloudU > 1.0f )
			mCloudU -= 1.0f;
		else if( mCloudU < -1.0f )
			mCloudU += 1.0f;

		if( mCloudV > 1.0f )
			mCloudV -= 1.0f;
		else if( mCloudV < -1.0f )
			mCloudV += 1.0f;

		//生成纹理坐标
		float* pUV = mpUV;
		pUV += 8;

		*pUV++ = 0.0f + mCloudU;	*pUV++	= 1.0f + mCloudV;
		*pUV++ = 0.0f + mCloudU;	*pUV++	= 0.0f + mCloudV;
		*pUV++ = 1.0f + mCloudU;	*pUV++	= 0.0f + mCloudV;
		*pUV++ = 1.0f + mCloudU;	*pUV	= 1.0f + mCloudV;

		void* pData = mpTexcoordVB->Lock( 0, 0 );
		memcpy( pData, mpUV, sizeof(float)*16 );
		mpTexcoordVB->Unlock();
	}
	//--------------------------------------------------------------------------------------------------------------
	//渲染天空体
	void SkyPlane::_RenderSky()
	{
		//备份当前世界矩阵和观看矩阵
		RenderSystem::mpSingleton->_GetViewMatrix( &mMatViewSave );
		RenderSystem::mpSingleton->_GetWorldMatrix( &mMatWorldSave );

		//设置渲染天空体的观看矩阵（只改变其观看位置）
		mMatView = mMatViewSave;
		mMatView._41 = 0.0f;	mMatView._42 = 0.0f;	mMatView._43 = 0.0f;

		RenderSystem::mpSingleton->_SetViewMatrix( &mMatView );

		//设置渲染天空体的世界矩阵
		RenderSystem::mpSingleton->_SetWorldMatrix( &mMatWorld );

		//设置天空体纹理状态
		_SetTextureState();

		//设置天空体不透明渲染状态
		RenderSystem::mpSingleton->_SetRenderState( &mRenderOpacity );

		//设置顶点数据来源和顶点声明
		RenderSystem::mpSingleton->_SetVertexStream( mpPositionVB, 0 );
		RenderSystem::mpSingleton->_SetVertexStream( mpTexcoordVB, 1 );
		RenderSystem::mpSingleton->_SetVertexDeclaration( mpVD );

		//渲染背景层
		RenderSystem::mpSingleton->_SetTexture( mpBackgroundTexture, 0 );
		RenderSystem::mpSingleton->_RenderVertex( PT_TRIANGLE_FAN, 0, 2 );

		//设置天空体透明渲染状态
		RenderSystem::mpSingleton->_SetRenderState( &mRenderTransparent );

		//渲染表层
		RenderSystem::mpSingleton->_SetTexture( mpSurfaceTexture, 0 );

		RenderSystem::mpSingleton->_RenderVertex( PT_TRIANGLE_FAN, 4, 2 );

		//恢复原来的世界矩阵和观看矩阵
		RenderSystem::mpSingleton->_SetViewMatrix( &mMatViewSave );
		RenderSystem::mpSingleton->_SetWorldMatrix( &mMatWorldSave );
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
