/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	D3D9Texture
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	D3D9 二维纹理类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	// D3D9 二维纹理类
	class FK_DLL D3D9Texture : public Texture
	{
	public:
		D3D9BaseTexture		mBaseTexture;

	public:
		D3D9Texture( UINT nWidth, UINT nHeight, PixelFormat ePixelFormat, UINT nNumLevel, TextureUsage Type );

		//锁定纹理
		void LockRect( void** ppBuf, UINT* pPitch, RECT* pLockRect, UINT nLevel, bool bDiscardLock );

		//解锁纹理
		void UnlockRect( UINT nLevel = 0 );

		//自动生成多级纹理映射
		void GenerateMipSubLevels( TextureFilterType eFilterType = TFT_LINEAR );
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
