/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	SkyBox
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
	SkyBox::SkyBox()
		: mpTopTexture		(NULL)
		, mpFrontTexture	(NULL)
		, mpBackTexture		(NULL)
		, mpLeftTexture		(NULL)
		, mpRightTexture	(NULL)
		, mpVD				(NULL)
		, mpPositionVB		(NULL)
		, mpTexcoordVB		(NULL)
	{
		BufferManager* pBufMgr = BufferManager::GetSingletonPtr();

		//创建顶点声明
		mpVD = pBufMgr->AddVertexDeclaration();
		mpVD->AddElement( 0, 0, VET_FLOAT3, VEM_DEFAULT, VEU_POSITION, 0 );
		mpVD->AddElement( 1, 0, VET_FLOAT2, VEM_DEFAULT, VEU_TEXCOORD, 0 );
		mpVD->CreateVertexDeclaration();

		//创建顶点缓存
		mpPositionVB = pBufMgr->AddVertexBuffer();
		mpPositionVB->CreateVertexBuffer( sizeof(float)*3, 20 );

		mpTexcoordVB = pBufMgr->AddVertexBuffer();
		mpTexcoordVB->CreateVertexBuffer( sizeof(float)*2, 20 );

		//生成顶点坐标数据
		float* pPos = (float*)mpPositionVB->Lock( 0, 0 );

		//顶面
		*pPos++ = -10.0f;	*pPos++ = 10.0f;	*pPos++ = 10.0f;
		*pPos++ = -10.0f;	*pPos++ = 10.0f;	*pPos++ = -10.0f;
		*pPos++ = 10.0f;	*pPos++ = 10.0f;	*pPos++ = -10.0f;
		*pPos++ = 10.0f;	*pPos++ = 10.0f;	*pPos++ = 10.0f;

		//前面
		*pPos++ = -10.0f;	*pPos++ = -10.0f;	*pPos++ = 10.0f;
		*pPos++ = -10.0f;	*pPos++ = 10.0f;	*pPos++ = 10.0f;
		*pPos++ = 10.0f;	*pPos++ = 10.0f;	*pPos++ = 10.0f;
		*pPos++ = 10.0f;	*pPos++ = -10.0f;	*pPos++ = 10.0f;

		//后面
		*pPos++ = 10.0f;	*pPos++ = -10.0f;	*pPos++ = -10.0f;
		*pPos++ = 10.0f;	*pPos++ = 10.0f;	*pPos++ = -10.0f;
		*pPos++ = -10.0f;	*pPos++ = 10.0f;	*pPos++ = -10.0f;
		*pPos++ = -10.0f;	*pPos++ = -10.0f;	*pPos++ = -10.0f;

		//左面
		*pPos++ = -10.0f;	*pPos++ = -10.0f;	*pPos++ = -10.0f;
		*pPos++ = -10.0f;	*pPos++ = 10.0f;	*pPos++ = -10.0f;
		*pPos++ = -10.0f;	*pPos++ = 10.0f;	*pPos++ = 10.0f;
		*pPos++ = -10.0f;	*pPos++ = -10.0f;	*pPos++ = 10.0f;

		//右面
		*pPos++ = 10.0f;	*pPos++ = -10.0f;	*pPos++ = 10.0f;
		*pPos++ = 10.0f;	*pPos++ = 10.0f;	*pPos++ = 10.0f;
		*pPos++ = 10.0f;	*pPos++ = 10.0f;	*pPos++ = -10.0f;
		*pPos++ = 10.0f;	*pPos++ = -10.0f;	*pPos++ = -10.0f;

		mpPositionVB->Unlock();

		float pUVdata [] =
		{
			0.0f,	1.0f,
			0.0f,	0.0f,
			1.0f,	0.0f,
			1.0f,	1.0f,
		};

		//生成纹理坐标数据
		float* pUV = (float*)mpTexcoordVB->Lock( 0, 0 );

		for( UINT i=0; i<5; ++i )
		{
			memcpy( pUV, pUVdata, sizeof(pUVdata) );
			pUV += 8;
		}

		mpTexcoordVB->Unlock();
	}
	//--------------------------------------------------------------------------------------------------------------
	SkyBox::~SkyBox()
	{
		BufferManager* pBufMgr = BufferManager::GetSingletonPtr();

		//释放顶点声明及顶点缓存
		pBufMgr->DelVertexDeclaration( mpVD );
		pBufMgr->DelVertexBuffer( mpPositionVB );
		pBufMgr->DelVertexBuffer( mpTexcoordVB );
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置天空盒纹理
	void SkyBox::SetSkyBoxTexture( SkyBoxTexture* pSkyBoxTexture )
	{
		mpTopTexture	= pSkyBoxTexture->pTopTexture;
		mpFrontTexture	= pSkyBoxTexture->pFrontTexture;
		mpBackTexture	= pSkyBoxTexture->pBackTexture;
		mpLeftTexture	= pSkyBoxTexture->pLeftTexture;
		mpRightTexture	= pSkyBoxTexture->pRightTexture;
	}
	//--------------------------------------------------------------------------------------------------------------
	//更新天空体
	void SkyBox::_UpdateSky( float fTimeSinceLastFrame )
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	void SkyBox::_RenderSky()
	{
		//备份当前世界矩阵和观看矩阵
		RenderSystem::mpSingleton->_GetViewMatrix( &mMatViewSave );
		RenderSystem::mpSingleton->_GetWorldMatrix( &mMatWorldSave );

		//设置渲染天空体的观看矩阵（只改变其观看位置）
		mMatView = mMatViewSave;
		mMatView._41 = 0.0f;	mMatView._42 = 0.0f;	mMatView._43 = 0.0f;

		RenderSystem::mpSingleton->_SetViewMatrix( &mMatView );

		//设置天空体纹理状态
		_SetTextureState();

		//设置渲染天空体的世界矩阵
		RenderSystem::mpSingleton->_SetWorldMatrix( &mMatWorld );

		//设置天空体不透明渲染状态
		RenderSystem::mpSingleton->_SetRenderState( &mRenderOpacity );

		//设置顶点数据来源和顶点声明
		RenderSystem::mpSingleton->_SetVertexStream( mpPositionVB, 0 );
		RenderSystem::mpSingleton->_SetVertexStream( mpTexcoordVB, 1 );
		RenderSystem::mpSingleton->_SetVertexDeclaration( mpVD );

		//渲染天空盒
		RenderSystem::mpSingleton->_SetTexture( mpTopTexture, 0 );
		RenderSystem::mpSingleton->_RenderVertex( PT_TRIANGLE_FAN, 0, 2 );

		RenderSystem::mpSingleton->_SetTexture( mpFrontTexture, 0 );
		RenderSystem::mpSingleton->_RenderVertex( PT_TRIANGLE_FAN, 4, 2 );

		RenderSystem::mpSingleton->_SetTexture( mpBackTexture, 0 );
		RenderSystem::mpSingleton->_RenderVertex( PT_TRIANGLE_FAN, 8, 2 );

		RenderSystem::mpSingleton->_SetTexture( mpLeftTexture, 0 );
		RenderSystem::mpSingleton->_RenderVertex( PT_TRIANGLE_FAN, 12, 2 );

		RenderSystem::mpSingleton->_SetTexture( mpRightTexture, 0 );
		RenderSystem::mpSingleton->_RenderVertex( PT_TRIANGLE_FAN, 16, 2 );

		//恢复原来的世界矩阵和观看矩阵
		RenderSystem::mpSingleton->_SetViewMatrix( &mMatViewSave );
		RenderSystem::mpSingleton->_SetWorldMatrix( &mMatWorldSave );
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
