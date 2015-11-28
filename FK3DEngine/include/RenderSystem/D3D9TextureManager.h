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
*	文件说明：	D3D9 纹理管理类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	// D3D9 纹理管理类
	class FK_DLL D3D9TextureManager : public TextureManager
	{
	public:
		D3D9TextureManager();

		//创建二维纹理
		Texture* CreateTexture( UINT nWidth, UINT nHeight, PixelFormat ePixelFormat,
			TextureUsage Type, int nNumLevels );

		//创建立方纹理
		CubeTexture* CreateCubeTexture( UINT nEdgeLength, PixelFormat ePixelFormat,
			TextureUsage Type, int nNumLevels );

		//创建三维纹理
		VolumeTexture* CreateVolumeTexture( UINT nWidth, UINT nHeight, UINT nDepth,
			PixelFormat ePixelFormat, TextureUsage Type, int nNumLevels );

		//设置纹理尺寸最大限值
		void SetMaxTextureSize( UINT nWidth, UINT nHeight );
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
