/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	TextureManager
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../../include/RenderSystem/RenderSystemInsideHeader.h"

//==================================================================================================================


namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	TextureManager::TextureManager()
		: mpTexPackFile				(NULL)
		, mNumTextures				(0)
		, mDefTexLevels				(1)
		, mbCanNotSquare			(false)
		, mbHardCanNotSquare		(false)
		, mbCanNotPow2				(false)
		, mbHardCanNotPow2			(false)
		, mbCanNotPow2Cube			(false)
		, mbHardCanNotPow2Cube		(false)
		, mbCanNotPow2Volume		(false)
		, mbHardCanNotPow2Volume	(false)
	{
		mTextureList.Initialize( 200, 100 );
	}
	//--------------------------------------------------------------------------------------------------------------
	TextureManager::~TextureManager()
	{
		//删除所有纹理
		DeleteAllTextures();

		mTextureList.Release();
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置纹理打包文件
	void TextureManager::SetTexturePack( FileObject* pFile )
	{
		mpTexPackFile = pFile;
	}
	//--------------------------------------------------------------------------------------------------------------
	//强制使用边长相等纹理
	void TextureManager::ForceSquare( bool bForceUse )
	{
		mbCanNotSquare = bForceUse ? false : mbHardCanNotSquare;
	}
	//--------------------------------------------------------------------------------------------------------------
	//强制使用为 2 的 n 次幂尺寸的二维纹理
	void  TextureManager::ForcePow2( bool bForceUse )
	{
		mbCanNotPow2 = bForceUse ? false : mbHardCanNotPow2;
	}
	//--------------------------------------------------------------------------------------------------------------
	//强制使用为 2 的 n 次幂尺寸的立方纹理
	void TextureManager::ForcePow2Cube( bool bForceUse )
	{
		mbCanNotPow2Cube = bForceUse ? false : mbHardCanNotPow2Cube;
	}
	//--------------------------------------------------------------------------------------------------------------
	//强制使用为 2 的 n 次幂尺寸的三维纹理
	void TextureManager::ForcePow2Volume( bool bForceUse )
	{
		mbCanNotPow2Volume = bForceUse ? false : mbHardCanNotPow2Volume;
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除纹理
	void TextureManager::DeleteTexture( Texture* pTexture )
	{
		if( pTexture == NULL )
			return;

		BaseTextureList::Iterator it = mTextureList.Begin();
		BaseTextureList::Iterator end = mTextureList.End();
		for(; it!=end; ++it )
		{
			if( *it == pTexture )
			{
				mTextureList.Erase( it );
				delete pTexture;
				--mNumTextures;
				return;
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除所有纹理
	void TextureManager::DeleteAllTextures()
	{
		BaseTextureList::Iterator it = mTextureList.Begin();
		BaseTextureList::Iterator end = mTextureList.End();
		for(; it!=end; ++it )
		{
			if( *it != NULL )
				delete *it;
		}

		mNumTextures = 0;
		mTextureList.Clear();
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取当前的纹理数量
	FKInline UINT TextureManager::GetNumTextures()
	{
		return mNumTextures;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取最佳二维纹理创建尺寸
	void TextureManager::GetBestSize( UINT inWidth, UINT inHeight, UINT& outWidth, UINT& outHeight )
	{
		//如果图像尺寸大于最大纹理尺寸
		if( inWidth > mMaxWidth || inHeight > mMaxHeight )
			Except( Exception::ERR_INVALIDPARAMS, "指定的纹理尺寸超过了当前设置的最大纹理尺寸。" );

		//计算最合适的纹理创建尺寸
		if( mbCanNotPow2 )
		{
			outWidth = inWidth;
			outHeight = inHeight;
		}
		else
		{
			outWidth = CeilToPow2( inWidth );
			outHeight = CeilToPow2( inHeight );
		}

		if( !mbCanNotSquare )
			outWidth = outHeight = max( outWidth, outHeight );
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取最佳立方纹理创建尺寸
	UINT TextureManager::GetBestSizeCube( UINT inEdgeLength )
	{
		return mbCanNotPow2Cube ? inEdgeLength : CeilToPow2( inEdgeLength );
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取最佳三维纹理创建尺寸
	void TextureManager::GetBestSizeVolume( UINT inWidth, UINT inHeight, UINT inDepth,
		UINT& outWidth, UINT& outHeight, UINT& outDepth )
	{
		//如果图像尺寸大于最大纹理尺寸
		if( inWidth > mMaxVolumeExtent || inHeight > mMaxVolumeExtent || inDepth > mMaxVolumeExtent )
			Except( Exception::ERR_INVALIDPARAMS, "指定的纹理尺寸超过了当前设置的最大纹理尺寸。" );

		//计算最合适的纹理创建尺寸
		if( mbCanNotPow2Volume )
		{
			outWidth = inWidth;
			outHeight = inHeight;
			outDepth = inDepth;
		}
		else
		{
			outWidth = CeilToPow2( inWidth );
			outHeight = CeilToPow2( inHeight );
			outDepth = CeilToPow2( inDepth );
		}

		if( !mbCanNotSquare )
			outWidth = outHeight = outDepth = max( max( outWidth, outHeight ), outDepth );
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取硬件最大支持纹理尺寸
	void TextureManager::GetHardMaxTexSize( UINT& nMaxWidth, UINT& nMaxHeight )
	{
		nMaxWidth = mHardMaxWidth;
		nMaxHeight = mHardMaxHeight;
	}
	//--------------------------------------------------------------------------------------------------------------
	UINT TextureManager::GetHardMaxTexSize()
	{
		return mHardMaxVolumeExtent;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置默认纹理创建级别数
	void TextureManager::SetDefTextureLevels( int nNumLevels )
	{
		mDefTexLevels = 1;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取默认纹理创建级别数
	int TextureManager::GetDefTextureLevels()
	{
		return mDefTexLevels;
	}
	//--------------------------------------------------------------------------------------------------------------
	//从文件加载一块纹理
	Texture* TextureManager::LoadFromFile( const char* szFileName, int nNumLevels, PixelFormat ePixelFormat )
	{
		//载入图片
		mImgDecoder.LoadImageFromFile( mpTexPackFile, szFileName );

		//解析图像头
		mImgDecoder.ParseHeader();

		//如果为纹理压缩图像格式
		if( mImgDecoder.mPixelFormat & PF_COMPRESS_MASK )
		{
			ePixelFormat = mImgDecoder.mPixelFormat;

			//从文件载入多级映射纹理
			if( nNumLevels != 1 )
			{
				if( mImgDecoder.mpDDSInfo->dwLevelCount == 1 )
					Except( Exception::ERR_INTERNAL_ERROR, (String)"DDS 文件 " + szFileName + " 没有包含多级纹理数据，"
					"因此无法创建相应的多级映射纹理。" );

				nNumLevels = mImgDecoder.mpDDSInfo->dwLevelCount;
			}
		}

		//获取图像尺寸
		UINT nWidth = 0;
		UINT nHeight = 0;
		mImgDecoder.GetDimension( nWidth, nHeight );

		//创建纹理
		Texture* pTexture = CreateTexture( nWidth, nHeight, ePixelFormat, TU_NORMAL, nNumLevels );

		//载入每一级纹理数据
		BYTE* pDst = NULL;
		UINT Pitch = 0;

		int nNumFillLevel = ( nNumLevels == 0 ) ? 1 : nNumLevels;

		for( int i=0; i<nNumFillLevel; ++i )
		{
			//设定解码纹理级
			if( mImgDecoder.mpDDSInfo != NULL )
				mImgDecoder.mpDDSInfo->dwDecodeLevel = i;

			//锁定纹理
			pTexture->LockRect( (void**)&pDst, &Pitch, NULL, i );

			//填充图像数据
			mImgDecoder.Decode( pDst, NULL, NULL, Pitch, ePixelFormat );

			//解锁纹理
			pTexture->UnlockRect( i );
		}

		//卸载图片
		if( !mImgDecoder.UnloadImage() )
			Except( Exception::ERR_INTERNAL_ERROR, "关闭纹理图像数据解码器失败。" );

		//自动生成多级纹理映射
		if( nNumLevels == 0 )
		{
			if( !IsPow2( nWidth ) || !IsPow2( nHeight ) )
				Except( Exception::ERR_INTERNAL_ERROR, (String)"因为纹理 " + szFileName + " 的尺寸为非 2 次幂数值，"
				"因此无法自动生成它的多级纹理映射。" );

			pTexture->GenerateMipSubLevels();
		}

		return pTexture;
	}
	//--------------------------------------------------------------------------------------------------------------
	//从内存中加载一块纹理
	Texture* TextureManager::LoadFromMem( BYTE* pMemPtr, int nNumLevels, PixelFormat ePixelFormat )
	{
		return NULL;
	}
	//--------------------------------------------------------------------------------------------------------------
	//检测指定压缩格式纹理是否可用
	bool TextureManager::CheckCompressFormat( const PixelFormat ePixelFormat )
	{
		//如果为压缩纹理格式
		if( ePixelFormat & PF_COMPRESS_MASK )
		{
			//检测指定纹理压缩格式是否可用
			DWORD dwFmtIndex = (DWORD)ePixelFormat - PF_COMPRESS_MASK - 1;
			return mbTexCompFormats[ dwFmtIndex ];
		}

		return true;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
