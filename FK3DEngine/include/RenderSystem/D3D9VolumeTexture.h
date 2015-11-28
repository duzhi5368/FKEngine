/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	D3D9VolumeTexture
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	D3D9 三维纹理类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	// D3D9 纹理类
	class FK_DLL D3D9VolumeTexture : public VolumeTexture, public D3D9BaseTexture
	{
	public:
		D3D9BaseTexture		mBaseTexture;

	public:
		D3D9VolumeTexture( UINT nWidth, UINT nHeight, UINT nDepth,
			PixelFormat ePixelFormat, UINT nNumLevel, TextureUsage Type );

		//锁定纹理
		void LockBox( LockBox3* pLocked, Box3* pLockBox, UINT nLevel, bool bDiscardLock );

		//解锁纹理
		void UnlockBox( UINT nLevel = 0 );

		//自动生成多级纹理映射
		void GenerateMipSubLevels( TextureFilterType eFilterType = TFT_LINEAR );
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
