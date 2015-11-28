/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	SkySphere
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
	SkySphere::SkySphere( UINT nNumRing, UINT nNumSegment )
		: mSkyWorldMatrix	(Matrix4::Identity)
		, mpTexture			(NULL)
		, mpVD				(NULL)
		, mpPositionVB		(NULL)
		, mpTexcoordVB		(NULL)
		, mpIB				(NULL)
		, mRotaValue		(0.0f)
		, mRotaDegree		(0.0f)
		, mRotaAxis			(Vector3::UnitX)
	{
		mRing		= nNumRing;							//环数
		mSegment	= nNumSegment;						//段数
		mNumVertex	= ( mRing + 1 ) * ( mSegment + 1 );	//顶点数量
		mNumIndex	= 2 * mRing * ( mSegment + 1 );		//索引数量
		mNumFace	= mNumIndex - 2;					//三角面数量


		BufferManager* pBufMgr = BufferManager::GetSingletonPtr();

		//创建顶点声明
		mpVD = pBufMgr->AddVertexDeclaration();
		mpVD->AddElement( 0, 0, VET_FLOAT3, VEM_DEFAULT, VEU_POSITION, 0 );
		mpVD->AddElement( 1, 0, VET_FLOAT2, VEM_DEFAULT, VEU_TEXCOORD, 0 );
		mpVD->CreateVertexDeclaration();

		//创建顶点缓存
		mpPositionVB = pBufMgr->AddVertexBuffer();
		mpPositionVB->CreateVertexBuffer( sizeof(float)*3, mNumVertex * 2, false );

		mpTexcoordVB = pBufMgr->AddVertexBuffer();
		mpTexcoordVB->CreateVertexBuffer( sizeof(float)*2, mNumVertex * 2, false );

		mpIB = pBufMgr->AddIndexBuffer();
		mpIB->CreateIndexBuffer( mNumIndex * 2, IT_INDEX16 );

		//生成原始顶点数据
		size_t nPosDataLen = sizeof(float) * mNumVertex * 3;
		size_t nUVDataLen = sizeof(float) * mNumVertex * 2;
		size_t nIndexDataLen = sizeof(WORD) * mNumIndex;

		char* pDataBuf = (char*)malloc( nPosDataLen + nUVDataLen + nIndexDataLen );
		if( pDataBuf == NULL )
			Except( Exception::ERR_NOT_ENOUGH_MEM, "创建天空球时分配顶点数据生成缓存失败。" );

		float* pPosData		= (float*)pDataBuf;
		float* pUVData		= (float*)( pDataBuf + nPosDataLen );
		WORD* pIndexData	= (WORD*)( pDataBuf + nPosDataLen + nUVDataLen );

		Vector3* pPos = (Vector3*)pPosData;
		float* pUV = pUVData;
		WORD* pIndex = pIndexData;

		//计算角度
		const float fRing = (float)mRing;
		const float fSegment = (float)mSegment;
		const float fRingAngle = FK_PI / (float)fRing;
		const float fSegmentAngle = FK_PI * 2.0f / (float)fSegment;

		float r = 0.0f;
		float y = 0.0f;
		float x = 0.0f;
		float z = 0.0f;

		//生成垂直一环顶点的循环
		WORD wIndex = ( mRing + 1 ) * ( mSegment + 1 );
		for( float ring=0; ring<fRing+1.0f; ++ring )
		{
			SinCos( ring * fRingAngle, &r, &y );

			//生成水平的一段顶点的循环			
			for( float seg=0; seg<fSegment+1.0f; ++seg )
			{
				SinCos( seg * fSegmentAngle, &x, &z );

				pPos->x = x * r	* 60.0f;
				pPos->y = y * 60.0f;
				pPos->z = z * r	* 60.0f;
				++pPos;

				*pUV++ = 1.0f - seg / fSegment;
				*pUV++ = ring / fRing * 0.5f;

				if( ring != fRing )
				{
					--wIndex;
					*pIndex++ = wIndex - (WORD)mSegment - 1;
					*pIndex++ = wIndex;
				}
			}
		}

		//将数据写入缓存
		char* pDstPos = (char*)mpPositionVB->Lock( 0, 0 );

		memcpy( pDstPos, pPosData, nPosDataLen );
		pDstPos += nPosDataLen;
		memcpy( pDstPos, pPosData, nPosDataLen );

		mpPositionVB->Unlock();
        
		char* pDstUV = (char*)mpTexcoordVB->Lock( 0, 0 );
		char* pDstIndex = (char*)mpIB->Lock( 0, 0 );
		
		memcpy( pDstUV, pUVData, nUVDataLen );
		memcpy( pDstIndex, pIndexData, nIndexDataLen );
		pDstUV += nUVDataLen;
		pDstIndex += nIndexDataLen;

		//生成绘制纹理下半边的顶点数据
		pUV = pUVData;
		for( UINT i=0; i<(mRing+1)*(mSegment+1); ++i )
		{
			++pUV;
			*pUV += 0.5f;
			++pUV;
		}

		pIndex = pIndexData;
		for( UINT i=0; i<mRing*(mSegment+1); ++i )
		{
			*pIndex += mNumIndex;	++pIndex;
			*pIndex += mNumIndex;	++pIndex;
		}

		memcpy( pDstUV, pUVData, nUVDataLen );
		memcpy( pDstIndex, pIndexData, nIndexDataLen );

		mpTexcoordVB->Unlock();
		mpIB->Unlock();

		//释放顶点数据生成缓存
		free( pDataBuf );
	}
	//--------------------------------------------------------------------------------------------------------------
	SkySphere::~SkySphere()
	{
		BufferManager* pBufMgr = BufferManager::GetSingletonPtr();

		//释放顶点声明及顶点缓存
		pBufMgr->DelVertexDeclaration( mpVD );
		pBufMgr->DelVertexBuffer( mpPositionVB );
		pBufMgr->DelVertexBuffer( mpTexcoordVB );
		pBufMgr->DelIndexBuffer( mpIB );
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置天空球纹理
	void SkySphere::SetSphereTexture( Texture* pTexture )
	{
		mpTexture = pTexture;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置表层旋转
	void SkySphere::SetSurfaceRotation( float fRotaVale, const Vector3& vAxis )
	{
		mRotaValue = fRotaVale;
		mRotaAxis = vAxis;
	}
	//--------------------------------------------------------------------------------------------------------------
	//更新天空体
	void SkySphere::_UpdateSky( float fTimeSinceLastFrame )
	{
		mRotaDegree += mRotaValue * fTimeSinceLastFrame;

		if( mRotaDegree > 180.0f )
			mRotaDegree -= 360.0f;
		else if( mRotaDegree < -180.0f )
			mRotaDegree += 360.0f;

		mSkyWorldMatrix.SetRotationAxis( mRotaDegree, mRotaAxis );
	}
	//--------------------------------------------------------------------------------------------------------------
	//渲染天空体
	void SkySphere::_RenderSky()
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
		RenderSystem::mpSingleton->_SetIndexStream( mpIB );
		RenderSystem::mpSingleton->_SetVertexDeclaration( mpVD );

		//设置纹理
		RenderSystem::mpSingleton->_SetTexture( mpTexture, 0 );

		//渲染背景层
		RenderSystem::mpSingleton->_RenderVertex( PT_TRIANGLE_STRIP, 0, 0, mNumVertex, 0, mNumFace );

		//设置天空体透明渲染状态
		RenderSystem::mpSingleton->_SetRenderState( &mRenderTransparent );

		//渲染表层
		RenderSystem::mpSingleton->_SetWorldMatrix( &mSkyWorldMatrix );
		RenderSystem::mpSingleton->_RenderVertex( PT_TRIANGLE_STRIP, mNumVertex, 0, mNumVertex, 0, mNumFace );

		//恢复原来的世界矩阵和观看矩阵
		RenderSystem::mpSingleton->_SetViewMatrix( &mMatViewSave );
		RenderSystem::mpSingleton->_SetWorldMatrix( &mMatWorldSave );
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
