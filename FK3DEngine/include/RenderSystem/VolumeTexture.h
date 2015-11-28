/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	VolumeTexture
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
	//三维纹理
	class FK_DLL VolumeTexture : public BaseTexture
	{
	public:
		UINT		mWidth;			//纹理宽度
		UINT		mHeight;		//纹理高度
		UINT		mDepth;			//纹理深度

	public:
		VolumeTexture();
		VolumeTexture( UINT nWidth, UINT nHeight, UINT nDepth,
			PixelFormat ePixelFormat, UINT nNumLevel, TextureUsage Type );
		virtual ~VolumeTexture() = 0;

		//获取纹理实际尺寸
		FKInline void GetTextureSize( UINT& nWidth, UINT& nHeight, UINT& nDepth );

		//填充指定纹理区域为某颜色
		void FillColor( DWORD color = 0, Box3* pFillBox = NULL, UINT nLevel = 0 );

		//锁定纹理
		virtual void LockBox( LockBox3* pLocked, Box3* pLockBox = NULL, UINT nLevel = 0, bool bDiscardLock = false ) = 0;

		//解锁纹理
		virtual void UnlockBox( UINT nLevel = 0 ) = 0;
	};
	//--------------------------------------------------------------------------------------------------------------
	typedef Stack< VolumeTexture* >		VolumeTextureList;
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
