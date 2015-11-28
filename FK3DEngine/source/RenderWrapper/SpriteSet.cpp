/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	SpriteSet
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
	//创建一个空的精灵集
	SpriteSet::SpriteSet( UINT nTexWidth, UINT nTexHeight, PixelFormat ePixelFormat,
		UINT InitSpriteNum, UINT AddSpriteNum, TextureFilterType eFilter )
		: mFilterType	(eFilter)
	{
		//检测可以创建的纹理尺寸
		UINT TexWidth = 0;
		UINT TexHeight = 0;

		TextureManager* pTexMgr = TextureManager::GetSingletonPtr();
		pTexMgr->GetBestSize( nTexWidth, nTexHeight, TexWidth, TexHeight );

		//创建纹理
		mpTexture = pTexMgr->CreateTexture( TexWidth, TexHeight, ePixelFormat, TU_DYNAMIC, 1 );

		//初始化精灵列表容量
		mSpriteList.Initialize( InitSpriteNum, AddSpriteNum );
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建一个从文件载入图像的精灵集
	SpriteSet::SpriteSet( FileObject* pFile, const char* szName, PixelFormat ePixelFormat,
		UINT InitSpriteNum, UINT AddSpriteNum, TextureFilterType eFilter )
		: mFilterType	(eFilter)
	{
		//获取图像解码器
		ImageDecoder* pImgDecoder = &OverlayManager::mpSingleton->mImgDecoder;

		//载入图片
		pImgDecoder->LoadImageFromFile( pFile, szName );

		//获取图像尺寸
		UINT Width = 0;
		UINT Height = 0;
		pImgDecoder->ParseHeader();
		pImgDecoder->GetDimension( Width, Height );

		//检测可以创建的纹理尺寸
		UINT TexWidth = 0;
		UINT TexHeight = 0;

		TextureManager::mpSingleton->GetBestSize( Width, Height, TexWidth, TexHeight );

		//如果硬件无法创建图片尺寸的纹理则报错（否则会造成与载入 UV 坐标的错误）
		//if( Width != TexWidth || Height != TexHeight )
		//	Except( Exception::ERR_INVALIDPARAMS, "创建精灵集时发现硬件无法创建要求的图片尺寸的纹理。" );

		//创建纹理
		mpTexture = TextureManager::mpSingleton->CreateTexture( Width, Height, ePixelFormat );

		//将图像数据填充到纹理中
		BYTE* pDst = NULL;
		UINT pitch = 0;
		mpTexture->LockRect( (void**)&pDst, &pitch );
		pImgDecoder->Decode( pDst, NULL, NULL, pitch, ePixelFormat );
		mpTexture->UnlockRect();

		//关闭图片解码器
		if( !pImgDecoder->UnloadImage() )
			Except( Exception::ERR_INTERNAL_ERROR, "关闭精灵纹理图像数据解码器失败。" );

		//初始化精灵列表容量
		mSpriteList.Initialize( InitSpriteNum, AddSpriteNum );
	}
	//--------------------------------------------------------------------------------------------------------------
	SpriteSet::~SpriteSet()
	{
		OverlayManager* pOverlayMgr = OverlayManager::GetSingletonPtr();

		//删除所有精灵
		DeleteAllSprites();

		//释放该纹理
		TextureManager::mpSingleton->DeleteTexture( mpTexture );
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建一个属于该精灵集的静态精灵
	Sprite* SpriteSet::CreateSprite( float fLeft, float fTop, float fWidth, float fHeight,
		ZOrderType eZType, OverlaySurface* pZRefOverlay, UV4* pUV, DWORD color )
	{
		Sprite* pSprite = new Sprite( this, fLeft, fTop, fWidth, fHeight,
			eZType, pZRefOverlay, mFilterType, pUV, color );

		//将它加入队列
		*mSpriteList.Push() = pSprite;

		return pSprite;
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建一个属于该精灵集的动画精灵
	Sprite* SpriteSet::CreateUVASprite( float fLeft, float fTop, float fWidth, float fHeight,
		ZOrderType eZType, OverlaySurface* pZRefOverlay, UVAnimation* pUVAnimation, DWORD color )
	{
		Sprite* pSprite = new Sprite( this, fLeft, fTop, fWidth, fHeight,
			eZType, pZRefOverlay, mFilterType, pUVAnimation, color );

		//将它加入队列
		*mSpriteList.Push() = pSprite;

		return pSprite;
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除指定精灵
	void SpriteSet::DeleteSprite( Sprite* pSprite )
	{
		if( pSprite == NULL )
			return;

		//将它从队列中删除
		SpriteList::Iterator it = mSpriteList.Begin();
		SpriteList::Iterator end = mSpriteList.End();
		for(; it!=end; ++it )
		{
			if( *it == pSprite )
			{
				mSpriteList.Erase( it );
				delete pSprite;
				return;
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除所有精灵
	void SpriteSet::DeleteAllSprites()
	{
		//将它从队列中删除
		SpriteList::Iterator it = mSpriteList.Begin();
		SpriteList::Iterator end = mSpriteList.End();
		for(; it!=end; ++it )
		{
			if( *it != NULL )
				delete *it;
		}

		mSpriteList.Clear();
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置所有精灵显示状态
	void SpriteSet::SetAllVisable( bool bVisable )
	{
		SpriteList::Iterator it = mSpriteList.Begin();
		SpriteList::Iterator end = mSpriteList.End();
		for(; it!=end; ++it )
			(*it)->SetVisable( bVisable );
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
