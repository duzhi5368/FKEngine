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

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//纹理管理器
	class FK_DLL TextureManager : public Singleton< TextureManager >
	{
	public:
		UINT				mMaxWidth;				//二维纹理最大宽度
		UINT				mMaxHeight;				//二维纹理最大高度
		UINT				mHardMaxWidth;			//硬件支持二维纹理最大宽度
		UINT				mHardMaxHeight;			//硬件支持二维纹理最大高度

		UINT				mMaxVolumeExtent;		//三维纹理最大尺寸
		UINT				mHardMaxVolumeExtent;	//硬件支持三维纹理最大尺寸

		BaseTextureList		mTextureList;			//纹理列表
		UINT				mNumTextures;			//纹理数量
		int					mDefTexLevels;			//默认纹理创建级别数

		bool				mbCanNotSquare;			//是否允许使用非边长相等纹理
		bool				mbHardCanNotSquare;		//硬件是否支持非边长相等纹理

		bool				mbCanNotPow2;			//是否允许使用非 2 的 n 次幂尺寸二维纹理
		bool				mbHardCanNotPow2;		//硬件是否支持非 2 的 n 次幂尺寸二维纹理

		bool				mbCanNotPow2Cube;		//是否允许使用非 2 的 n 次幂尺寸立方纹理
		bool				mbHardCanNotPow2Cube;	//硬件是否支持非 2 的 n 次幂尺寸立方纹理

		bool				mbCanNotPow2Volume;		//是否允许使用非 2 的 n 次幂尺寸三维纹理
		bool				mbHardCanNotPow2Volume;	//硬件是否支持非 2 的 n 次幂尺寸三维纹理

	protected:
		FileObject*			mpTexPackFile;			//纹理打包文件指针
		ImageDecoder		mImgDecoder;			//纹理图像解码器

		bool				mbTexCompFormats[5];	//可用纹理压缩格式（DXT1-DXT5）

	public:
		TextureManager();
		virtual ~TextureManager();

		//设置纹理打包文件
		void SetTexturePack( FileObject* pFile );

		//强制使用边长相等纹理
		void ForceSquare( bool bForceUse = true );

		//强制使用为 2 的 n 次幂尺寸的二维纹理
		void ForcePow2( bool bForceUse = true );

		//强制使用为 2 的 n 次幂尺寸的立方纹理
		void ForcePow2Cube( bool bForceUse = true );

		//强制使用为 2 的 n 次幂尺寸的三维纹理
		void ForcePow2Volume( bool bForceUse = true );

		//创建二维纹理
		virtual Texture* CreateTexture( UINT nWidth, UINT nHeight, PixelFormat ePixelFormat,
			TextureUsage Type = TU_NORMAL, int nNumLevels = -1 ) = 0;

		//创建立方纹理
		virtual CubeTexture* CreateCubeTexture( UINT nEdgeLength, PixelFormat ePixelFormat,
			TextureUsage Type = TU_NORMAL, int nNumLevels = -1 ) = 0;

		//创建三维纹理
		virtual VolumeTexture* CreateVolumeTexture( UINT nWidth, UINT nHeight, UINT nDepth,
			PixelFormat ePixelFormat, TextureUsage Type = TU_NORMAL, int nNumLevels = -1 ) = 0;

		//删除纹理
		void DeleteTexture( Texture* pTexture );

		//删除所有纹理
		void DeleteAllTextures();

		//获取当前的纹理数量
		FKInline UINT GetNumTextures();

		//获取最佳二维纹理创建尺寸
		void GetBestSize( UINT inWidth, UINT inHeight, UINT& outWidth, UINT& outHeight );

		//获取最佳立方纹理创建尺寸
		UINT GetBestSizeCube( UINT inEdgeLength );

		//获取最佳三维纹理创建尺寸
		void GetBestSizeVolume( UINT inWidth, UINT inHeight, UINT inDepth,
			UINT& outWidth, UINT& outHeight, UINT& outDepth );

		//获取硬件最大支持纹理尺寸
		void GetHardMaxTexSize( UINT& nMaxWidth, UINT& nMaxHeight );
		UINT GetHardMaxTexSize();

		//设置纹理尺寸最大限值
		virtual void SetMaxTextureSize( UINT nWidth, UINT nHeight ) = 0;

		//设置默认纹理创建级别数
		void SetDefTextureLevels( int nNumLevels = 0 );

		//获取默认纹理创建级别数
		int GetDefTextureLevels();

		//从文件加载一块纹理
		Texture* LoadFromFile( const char* szFileName, int nNumLevels = 1, PixelFormat ePixelFormat = PF_A8R8G8B8 );

		//从内存中加载一块纹理
		Texture* LoadFromMem( BYTE* pMemPtr, int nNumLevels = 1, PixelFormat ePixelFormat = PF_A8R8G8B8 );

		//检测指定压缩格式纹理是否可用
		bool CheckCompressFormat( const PixelFormat ePixelFormat );
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
