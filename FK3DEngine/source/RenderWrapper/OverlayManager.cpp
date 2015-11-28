/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	OverlayManager
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
	OverlayManager::OverlayManager()
		: mRenderWidth		(800)
		, mRenderHeight		(600)
		, mNumRenderVer		(0)
		, mpOverlayPack		(NULL)
		, mpVertexBuf		(NULL)
		, mpVertexDecl		(NULL)
		, mMaxNumVer		(0)
		, mOffsetVer		(0)
	{
		mOverlayList.Initialize( 80, 32 );
		mJointList.Initialize( 10, 10 );
		mSpriteList.Initialize( 60, 20 );
		mSpriteSetList.Initialize( 10, 10 );
		mUVASprite.Initialize( 20, 20 );

		//初始化平面图像渲染矩阵
		mWorldMatrix.SetIdentity();
		mProjMatrix.SetOrthoOffCenterLH( 0.0f, 800.0f, 0.0f, 600.0f, 0.0f, 10.0f );
		mViewMatrix.SetLookAtLH( Vector3( 0.0f, 0.0f, -1.0f ), Vector3::Zero, Vector3( 0.0f, 1.0f, 0.0f ) );

		//初始化平面图像渲染状态
		mRenderState.mbLighting		= false;
		mRenderState.mbFogEnable	= false;

		mRenderState.SetTransparent();
	}
	//--------------------------------------------------------------------------------------------------------------
	OverlayManager::~OverlayManager()
	{
		//释放顶点缓存及顶点声明
		BufferManager::mpSingleton->DelVertexBuffer( mpVertexBuf );
		BufferManager::mpSingleton->DelVertexDeclaration( mpVertexDecl );

		DeleteAllJointSurfaces();
		DeleteAllSpriteSets();
	}
	//--------------------------------------------------------------------------------------------------------------
	//初始化平面图像管理类
	void OverlayManager::Initialize( UINT nMaxNumVer )
	{
		//计算顶点缓存创建大小
		mMaxNumVer = nMaxNumVer;

		//创建顶点缓存
		mpVertexBuf = BufferManager::mpSingleton->AddVertexBuffer();
		mpVertexDecl = BufferManager::mpSingleton->AddVertexDeclaration();

		mpVertexBuf->CreateVertexBuffer( sizeof(OverlayVertex), mMaxNumVer, true );

		mpVertexDecl->AddElement( 0, 0, VET_FLOAT3, VEM_DEFAULT, VEU_POSITION, 0 );
		mpVertexDecl->AddElement( 0, 12, VET_ARGBCOLOR, VEM_DEFAULT, VEU_COLOR, 0 );
		mpVertexDecl->AddElement( 0, 16, VET_FLOAT2, VEM_DEFAULT, VEU_TEXCOORD, 0 );

		mpVertexDecl->CreateVertexDeclaration();
	}
	//--------------------------------------------------------------------------------------------------------------
	//更新顶点数据
	void OverlayManager::_UpdateVertexData()
	{
		if( mNumRenderVer == 0 )
			return;

		//检查渲染顶点数量是否超过最大限制
		if( mNumRenderVer > mMaxNumVer )
			Except( Exception::ERR_INTERNAL_ERROR, (String)"Direct3D 9 平面图像渲染顶点数量超过了顶点缓存大小，更新失败。" +
			Misc::FormatString( NULL, "\n当前顶点渲染数量：%d    最大渲染顶点数量：%d", mNumRenderVer, mMaxNumVer ) );

		//如果超过了最大限则返回起始位置锁定，并且在这一帧使用 D3DLOCK_DISCARD 锁定。
		bool bNoOverwrite = true;

		if( mOffsetVer + mNumRenderVer > mMaxNumVer )
		{
			mOffsetVer = 0;
			bNoOverwrite = false;
		}

		//更新所有平面图像的顶点数据
		OverlayList::Iterator it = mOverlayList.Begin();
		OverlayList::Iterator end = mOverlayList.End();

		//锁定顶点缓存
		OverlayVertex* pVertexDst = (OverlayVertex*)mpVertexBuf->Lock( mOffsetVer, mNumRenderVer, bNoOverwrite );
		UINT nVerOffset = mOffsetVer;

		for(; it!=end; ++it )
		{
			OverlaySurface* pOverlay = *it;

			//如果该平面图像可见
			if( pOverlay->mbVisable )
			{
				//处理平面图像顶点数据
				pOverlay->_ProcessVertex();

				//更新平面图像顶点数据
				UINT nNumVertex = pOverlay->_UpdateVertex( pVertexDst, nVerOffset );
				pVertexDst += nNumVertex;
				nVerOffset += nNumVertex;
			}
		}

		//解锁顶点缓存
		mpVertexBuf->Unlock();
		
		mOffsetVer += mNumRenderVer;
	}
	//--------------------------------------------------------------------------------------------------------------
	//渲染所有平面图像
	void OverlayManager::_RenderAllOverlays()
	{
		//设置顶点流及顶点声明
		RenderSystem::mpSingleton->_SetVertexStream( mpVertexBuf, 0 );
		RenderSystem::mpSingleton->_SetVertexDeclaration( mpVertexDecl );

		//设置渲染矩阵
		RenderSystem::mpSingleton->_SetWorldMatrix( &mWorldMatrix );
		RenderSystem::mpSingleton->_SetProjMatrix( &mProjMatrix );
		RenderSystem::mpSingleton->_SetViewMatrix( &mViewMatrix );

		//镜向纹理寻址以避免出现接缝
		RenderSystem::mpSingleton->_SetTextureAddress( ST_ADDRESSU, TAT_MIRROR, 0 );
		RenderSystem::mpSingleton->_SetTextureAddress( ST_ADDRESSV, TAT_MIRROR, 0 );

		//设置平面图像渲染状态
		RenderSystem::mpSingleton->_SetRenderState( &mRenderState );

		//按照 Z 顺序渲染所有平面图像
		OverlayList::Iterator it = mOverlayList.Begin();
		OverlayList::Iterator end = mOverlayList.End();

		for(; it!=end; ++it )
		{
			OverlaySurface* pOverlay = *it;

			//如果该平面图像可见
			if( pOverlay->mbVisable )
				pOverlay->_RenderOverlay();
		}

		//更新字体渲染数据
		FontManager::mpSingleton->UpdateFont();
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置面板打包文件
	void OverlayManager::SetOverlayPack( FileObject* pFile )
	{
		mpOverlayPack = pFile;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置渲染面尺寸
	void OverlayManager::SetRenderPlaneSize( UINT nWidth, UINT nHeight )
	{
		mRenderWidth = nWidth;
		mRenderHeight = nHeight;
		mProjMatrix.SetOrthoOffCenterLH( 0.0f, (float)nWidth, 0.0f, (float)nHeight, 0.0f, 10.0f );
	}
	//--------------------------------------------------------------------------------------------------------------
	//更新平面图像动画帧延时
	void OverlayManager::_UpdateFrameTime( float fFrameTime )
	{
		UVASpriteList::Iterator it = mUVASprite.Begin();
		UVASpriteList::Iterator end = mUVASprite.End();
		for(; it!=end; ++it )
			(*it)->UpdateFrameTime( fFrameTime );
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建空拼接图像
	JointSurface* OverlayManager::CreateJointSurface( float fLeft, float fTop, float fWidth, float fHeight,
		ZOrderType eZType, OverlaySurface* pZRefOverlay, UINT nTexWidth, UINT nTexHeight, PixelFormat ePixelFormat,
		TextureFilterType eFilter, bool bDynamic, DWORD dwColor )
	{
		//创建拼接图像
		JointSurface* pJointSurface = new JointSurface( eZType, pZRefOverlay, eFilter );
		*mJointList.Push() = pJointSurface;

		pJointSurface->CreateJointSurface( fLeft, fTop, fWidth, fHeight, nTexWidth, nTexHeight, ePixelFormat,
			bDynamic, dwColor );

		return pJointSurface;
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建空精灵集
	SpriteSet* OverlayManager::CreateSpriteSet( UINT nTexWidth, UINT nTexHeight, PixelFormat ePixelFormat,
		UINT InitSpriteNum, UINT AddSpriteNum, TextureFilterType eFilter )
	{
		SpriteSet* pSpriteSet = new SpriteSet( nTexWidth, nTexHeight, ePixelFormat,
			InitSpriteNum, AddSpriteNum, eFilter );
		*mSpriteSetList.Push() = pSpriteSet;

		return pSpriteSet;
	}
	//--------------------------------------------------------------------------------------------------------------
	//从图像文件中载入拼接图像
	JointSurface* OverlayManager::LoadJointSurfaceFromFile( float fLeft, float fTop, float fWidth, float fHeight,
		ZOrderType eZType, OverlaySurface* pZRefOverlay, const char* szName, PixelFormat ePixelFormat,
		TextureFilterType eFilter, bool bDynamic, DWORD dwColor )
	{
		//载入平面图像
		JointSurface* pJointSurface = new JointSurface( eZType, pZRefOverlay, eFilter );
		*mJointList.Push() = pJointSurface;

		pJointSurface->LoadJointSurfaceFromFile( fLeft, fTop, fWidth, fHeight, mpOverlayPack, szName,
			ePixelFormat, bDynamic, dwColor );

		return pJointSurface;
	}
	//--------------------------------------------------------------------------------------------------------------
	//从图像文件中载入精灵集
	SpriteSet* OverlayManager::LoadSpriteSetFromFile( const char* szName, PixelFormat ePixelFormat,
		UINT InitSpriteNum, UINT AddSpriteNum, TextureFilterType eFilter )
	{
		SpriteSet* pSpriteSet = new SpriteSet( mpOverlayPack, szName, ePixelFormat,
			InitSpriteNum, AddSpriteNum, eFilter );
		*mSpriteSetList.Push() = pSpriteSet;

		return pSpriteSet;
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除拼接图像
	void OverlayManager::DeleteJointSurface( JointSurface* pJointSurface )
	{
		if( pJointSurface == NULL )
			return;

		JointList::Iterator it = mJointList.Begin();
		JointList::Iterator end = mJointList.End();
		for(; it!=end; ++it )
		{
			if( *it == pJointSurface )
			{
				mJointList.Erase( it );
				delete *it;
				return;
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除精灵集
	void OverlayManager::DeleteSpriteSet( SpriteSet* pSpriteSet )
	{
		if( pSpriteSet == NULL )
			return;

		SpriteSetList::Iterator it = mSpriteSetList.Begin();
		SpriteSetList::Iterator end = mSpriteSetList.End();
		for(; it!=end; ++it )
		{
			if( *it == pSpriteSet )
			{
				mSpriteSetList.Erase(it);
				delete pSpriteSet;
				return;
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除所有拼接图像
	void OverlayManager::DeleteAllJointSurfaces()
	{
		JointList::Iterator it = mJointList.Begin();
		JointList::Iterator end = mJointList.End();
		for(; it!=end; ++it )
		{
			if( *it != NULL )
				delete *it;
		}

		mJointList.Clear();
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除所有精灵集
	void OverlayManager::DeleteAllSpriteSets()
	{
		SpriteSetList::Iterator it = mSpriteSetList.Begin();
		SpriteSetList::Iterator end = mSpriteSetList.End();
		for(; it!=end; ++it )
		{
			if( *it != NULL )
				delete *it;
		}

		mSpriteSetList.Clear();
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取最顶层平面图像指针
	OverlaySurface* FKFastcall OverlayManager::GetTopmostOverlay()
	{
		if( mOverlayList.Size() == 0 )
			return NULL;

		return *mOverlayList.Back();
	}
	//--------------------------------------------------------------------------------------------------------------
	//将平面图像加入到管理列表中
	void FKFastcall OverlayManager::_AddOverlayToList( OverlaySurface* pOverlay,
		ZOrderType eZType, OverlaySurface* pZRefOverlay )
	{
		switch (eZType)
		{
			//最上层（最后渲染）
		case ZOT_Top:
			{
				*mOverlayList.Push() = pOverlay;
				return;
			}
			//最下层（最先渲染）
		case ZOT_Bottom:
			{
				*mOverlayList.Insert( (unsigned int)0 ) = pOverlay;
				return;
			}
			//指定平面图像之上（后渲染）
		case ZOT_Upon:
			{
				//如果指定基准平面图像是最后一个平面图像
				OverlayList::Iterator back = mOverlayList.Back();
				if( pZRefOverlay == *back )
				{
					*mOverlayList.Push() = pOverlay;
					return;
				}
				else
				{
					OverlayList::Iterator beg = mOverlayList.Begin();
					OverlayList::Iterator it = --back;
					for(; it>=beg; --it )
					{
						if( *it == pZRefOverlay )
						{
							*mOverlayList.Insert( ++it ) = pOverlay;
							return;
						}
					}
				}
				break;
			}
			//指定平面图像之下（先渲染）
		case ZOT_Below:
			{
				OverlayList::Iterator it = mOverlayList.Begin();
				OverlayList::Iterator end = mOverlayList.End();
				for(; it!=end; ++it )
				{
					if( *it == pZRefOverlay )
					{
						*mOverlayList.Insert( it ) = pOverlay;
						return;
					}
				}
				break;
			}
		}

		//Except( Exception::ERR_INVALIDPARAMS, "指定的平面图像 Z 顺序错误，无法将其加入列表。" );
	}
	//--------------------------------------------------------------------------------------------------------------
	//将平面图像从管理列表中删除
	void FKFastcall OverlayManager::_DelOverlayFromList( OverlaySurface* pOverlay )
	{
		OverlayList::Iterator it = mOverlayList.Begin();
		OverlayList::Iterator end = mOverlayList.End();
		for(; it!=end; ++it )
		{
			if( *it == pOverlay )
			{
				//减少顶点计数
				if( pOverlay->mbVisable )
					mNumRenderVer -= pOverlay->mNumVertex;

				mOverlayList.Erase( it );
				return;
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置平面图像 Z 顺序
	void FKFastcall OverlayManager::_SetOverlayZOrder( OverlaySurface* pOverlay,
		ZOrderType eZType, OverlaySurface* pZRefOverlay )
	{
		//将该平面图像从列表中删除
		OverlayList::Iterator it = mOverlayList.Begin();
		OverlayList::Iterator end = mOverlayList.End();
		for(; it!=end; ++it )
		{
			if( *it == pOverlay )
			{
				mOverlayList.Erase( it );

				//将其按照新的 Z 顺序加入列表
				_AddOverlayToList( pOverlay, eZType, pZRefOverlay );
				return;
			}
		}

		//Except( Exception::ERR_INVALIDPARAMS, "指定的平面图像不存在，无法设置其 Z 顺序。" );
	}
	//--------------------------------------------------------------------------------------------------------------
	//将 UV 动画处理器加入到管理列表中
	void FKFastcall OverlayManager::_AddUVASpriteToList( UVASprite* pUVASprite )
	{
		if( pUVASprite == NULL )
			return;

		*mUVASprite.Push() = pUVASprite;
	}
	//--------------------------------------------------------------------------------------------------------------
	//将 UV 动画处理器从管理列表中删除
	void FKFastcall OverlayManager::_DelUVASpriteFromList( UVASprite* pUVASprite )
	{
		if( pUVASprite == NULL )
			return;

		UVASpriteList::Iterator it = mUVASprite.Begin();
		UVASpriteList::Iterator end = mUVASprite.End();
		for(; it!=end; ++it )
		{
			if( *it == pUVASprite )
			{
				mUVASprite.Erase( it );
				return;
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
