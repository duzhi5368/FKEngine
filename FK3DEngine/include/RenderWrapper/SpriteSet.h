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
*	文件说明：	精灵集类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//精灵集类
	// 一个精灵集将用来管理使用同一块纹理的所有精灵
	class FK_DLL SpriteSet
	{
	public:
		Texture*			mpTexture;		//精灵集纹理
		TextureFilterType	mFilterType;	//纹理过滤类型

		SpriteList			mSpriteList;	//属于该精灵集的精灵列表

	public:
		SpriteSet( UINT nWidth, UINT nHeight, PixelFormat ePixelFormat,
			UINT InitSpriteNum, UINT AddSpriteNum, TextureFilterType eFilter = TFT_LINEAR );
		SpriteSet( FileObject* pFile, const char* szName, PixelFormat ePixelFormat,
			UINT InitSpriteNum, UINT AddSpriteNum, TextureFilterType eFilter = TFT_LINEAR );
		~SpriteSet();

		//创建一个属于该精灵集的静态精灵
		Sprite* CreateSprite( float fLeft, float fTop, float fWidth, float fHeight,
			ZOrderType eZType, OverlaySurface* pZRefOverlay, UV4* pUV, DWORD color = 0xFFFFFFFF );

		//创建一个属于该精灵集的动画精灵
		Sprite* CreateUVASprite( float fLeft, float fTop, float fWidth, float fHeight,
			ZOrderType eZType, OverlaySurface* pZRefOverlay, UVAnimation* pUVAnimation, DWORD color = 0xFFFFFFFF );

		//删除指定精灵
		void DeleteSprite( Sprite* pSprite );

		//删除所有精灵
		void DeleteAllSprites();

		//设置所有精灵显示状态
		void SetAllVisable( bool bVisable );
	};
	//--------------------------------------------------------------------------------------------------------------
	typedef Stack< SpriteSet*>	SpriteSetList;
	//--------------------------------------------------------------------------------------------------------------
};

//==================================================================================================================
