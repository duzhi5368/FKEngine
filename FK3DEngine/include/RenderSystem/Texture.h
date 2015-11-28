/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Texture
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
	//二维纹理类
	class FK_DLL Texture : public BaseTexture
	{
	public:
		UINT		mWidth;			//纹理宽度
		UINT		mHeight;		//纹理高度

	public:
		Texture( UINT nWidth, UINT nHeight, PixelFormat ePixelFormat, UINT nNumLevel, TextureUsage Type );
		virtual ~Texture() = 0;

		//获取纹理实际尺寸
		FKInline void GetTextureSize( UINT& nWidth, UINT& nHeight );

		//填充指定纹理区域为某颜色
		void FillColor( DWORD color = 0, RECT* pFillRect = NULL, UINT nLevel = 0 );

		//锁定纹理
		virtual void LockRect( void** ppBuf, UINT* pPitch, RECT* pLockRect = NULL,
			UINT nLevel = 0, bool bDiscardLock = false ) = 0;

		//解锁纹理
		virtual void UnlockRect( UINT nLevel = 0 ) = 0;
	};
	//--------------------------------------------------------------------------------------------------------------
	typedef Stack< Texture* >	TextureList;
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
