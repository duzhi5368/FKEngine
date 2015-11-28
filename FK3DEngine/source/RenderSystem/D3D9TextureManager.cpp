/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	D3D9TextureManager
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../../include/RenderSystem/RenderSystemInsideHeader.h"
#include "../../../Depend/dx9/d3dx9.h"
#include "../../../Depend/dx9/dxerr9.h"
#include "../../include/CommonBase/MemoryMarco.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	D3D9TextureManager::D3D9TextureManager()
	{
		//设置默认纹理尺寸（检查硬件是否支持该尺寸纹理）
		SetMaxTextureSize( mMaxWidth, mMaxHeight );


		//获取硬件支持最大纹理尺寸
		mHardMaxWidth = (UINT)D3D9RenderSystem::mD3D9CurCaps.MaxTextureWidth;
		mHardMaxHeight = (UINT)D3D9RenderSystem::mD3D9CurCaps.MaxTextureHeight;

		mMaxWidth = mHardMaxWidth;
		mMaxHeight = mHardMaxHeight;

		mHardMaxVolumeExtent = (UINT)D3D9RenderSystem::mD3D9CurCaps.MaxVolumeExtent;
		mMaxVolumeExtent = mHardMaxVolumeExtent;


		//判断硬件是否支持非边长相等纹理
		mbHardCanNotSquare = ( D3D9RenderSystem::mD3D9CurCaps.TextureCaps & D3DPTEXTURECAPS_SQUAREONLY ) == 0;
		mbCanNotSquare = mbHardCanNotSquare;

		//判断硬件是否支持非 2 的 n 次幂尺寸的二维纹理
		mbHardCanNotPow2 = ( D3D9RenderSystem::mD3D9CurCaps.TextureCaps & D3DPTEXTURECAPS_POW2 ) != 0;
		mbCanNotPow2 = mbHardCanNotPow2;

		//判断硬件是否支持非 2 的 n 次幂尺寸的立方纹理
		mbHardCanNotPow2Cube = ( D3D9RenderSystem::mD3D9CurCaps.TextureCaps & D3DPTEXTURECAPS_CUBEMAP_POW2 ) == 0;
		mbCanNotPow2Cube = mbHardCanNotPow2Cube;

		//判断硬件是否支持非 2 的 n 次幂尺寸的三维纹理
		mbHardCanNotPow2Volume = ( D3D9RenderSystem::mD3D9CurCaps.TextureCaps & D3DPTEXTURECAPS_VOLUMEMAP_POW2 ) == 0;
		mbCanNotPow2Volume = mbHardCanNotPow2Volume;


		//获取硬件支持纹理压缩格式
		for( UINT i=0; i<5; ++i )
		{
			HRESULT result = D3D9RenderSystem::mpD3D9->CheckDeviceFormat( D3D9RenderSystem::mnAdpter,
				D3D9RenderSystem::mDeviceType, D3D9RenderSystem::mD3Dpp.BackBufferFormat, 0, D3DRTYPE_TEXTURE,
				D3D9TypeGet::GetPixelFormat( (PixelFormat)( PF_DXT1 + i ) ) );

			mbTexCompFormats[i] = ( result == S_OK );
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建二维纹理
	Texture* D3D9TextureManager::CreateTexture( UINT nWidth, UINT nHeight, PixelFormat ePixelFormat,
		TextureUsage Type, int nNumLevels )
	{
		//如果为压缩纹理格式
		if( ePixelFormat & PF_COMPRESS_MASK )
		{
			//检测指定纹理压缩格式是否可用
			if( !CheckCompressFormat( ePixelFormat ) )
				Except( Exception::ERR_INTERNAL_ERROR, "硬件不支持指定的纹理压缩格式，无法创建纹理。" );

			if( !IsPow2( nWidth ) || !IsPow2( nHeight ) )
				Except( Exception::ERR_INTERNAL_ERROR, "无法创建非 2 次幂尺寸的压缩纹理。" );
		}

		//获取最佳的纹理尺寸
		UINT texWidth = 0;
		UINT texHeight = 0;
		GetBestSize( nWidth, nHeight, texWidth, texHeight );

		nNumLevels = (UINT)( ( nNumLevels == -1 ) ? mDefTexLevels : nNumLevels );

		Texture* pTex = new D3D9Texture( texWidth, texHeight, ePixelFormat, nNumLevels, Type );

		*mTextureList.Push() = pTex;
		++mNumTextures;

		return pTex;
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建立方纹理
	CubeTexture* D3D9TextureManager::CreateCubeTexture( UINT nEdgeLength, PixelFormat ePixelFormat,
		TextureUsage Type, int nNumLevels )
	{
		//如果为压缩纹理格式
		if( ePixelFormat & PF_COMPRESS_MASK )
		{
			//检测指定纹理压缩格式是否可用
			if( !CheckCompressFormat( ePixelFormat ) )
				Except( Exception::ERR_INTERNAL_ERROR, "硬件不支持指定的纹理压缩格式，无法创建纹理。" );

			if( !IsPow2( nEdgeLength ) )
				Except( Exception::ERR_INTERNAL_ERROR, "无法创建非 2 次幂尺寸的压缩纹理。" );
		}

		//获取最佳的纹理尺寸
		UINT texEdgeLen = GetBestSizeCube( nEdgeLength );

		nNumLevels = (UINT)( ( nNumLevels == -1 ) ? mDefTexLevels : nNumLevels );

		CubeTexture* pTex = new D3D9CubeTexture( texEdgeLen, ePixelFormat, nNumLevels, Type );

		*mTextureList.Push() = pTex;
		++mNumTextures;

		return pTex;
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建三维纹理
	VolumeTexture* D3D9TextureManager::CreateVolumeTexture( UINT nWidth, UINT nHeight, UINT nDepth,
		PixelFormat ePixelFormat, TextureUsage Type, int nNumLevels )
	{
		//如果为压缩纹理格式
		if( ePixelFormat & PF_COMPRESS_MASK )
		{
			//检测指定纹理压缩格式是否可用
			if( !CheckCompressFormat( ePixelFormat ) )
				Except( Exception::ERR_INTERNAL_ERROR, "硬件不支持指定的纹理压缩格式，无法创建纹理。" );

			if( !IsPow2( nWidth ) || !IsPow2( nHeight ) || !IsPow2( nDepth ) )
				Except( Exception::ERR_INTERNAL_ERROR, "无法创建非 2 次幂尺寸的压缩纹理。" );
		}

		//获取最佳的纹理尺寸
		UINT texWidth = 0;
		UINT texHeight = 0;
		UINT texDepth = 0;
		GetBestSizeVolume( nWidth, nHeight, nDepth, texWidth, texHeight, texDepth );

		nNumLevels = (UINT)( ( nNumLevels == -1 ) ? mDefTexLevels : nNumLevels );

		VolumeTexture* pTex = new D3D9VolumeTexture( texWidth, texHeight, texDepth, ePixelFormat, nNumLevels, Type );

		*mTextureList.Push() = pTex;
		++mNumTextures;

		return pTex;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置纹理尺寸最大限值
	void D3D9TextureManager::SetMaxTextureSize( UINT nWidth, UINT nHeight )
	{
		UINT nMaxTexWidth = (UINT)D3D9RenderSystem::mD3D9CurCaps.MaxTextureWidth;
		UINT nMaxTexHeight = (UINT)D3D9RenderSystem::mD3D9CurCaps.MaxTextureHeight;

		mMaxWidth	= FK_MIN( nMaxTexWidth, nWidth );
		mMaxHeight	= FK_MIN( nMaxTexHeight, nHeight );
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
