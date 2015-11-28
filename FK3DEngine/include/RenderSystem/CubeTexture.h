/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	CubeTexture
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
	//立方体纹理
	class FK_DLL CubeTexture : public BaseTexture
	{
	public:
		UINT		mEdgeLength;	//立方体边长

	public:
		CubeTexture( UINT nEdgeLength, PixelFormat ePixelFormat, UINT nNumLevel, TextureUsage Type );
		virtual ~CubeTexture() = 0;

		//获取立方体纹理边长
		UINT GetEdgeLength();

		//填充指定纹理区域为某颜色
		void FillColor( CubeFace eFace, DWORD color = 0, RECT* pFillRect = NULL, UINT nLevel = 0 );

		//锁定纹理
		virtual void LockRect( CubeFace eFace, void** ppBuf, UINT* pPitch, RECT* pLockRect = NULL,
			UINT nLevel = 0, bool bDiscardLock = false ) = 0;

		//解锁纹理
		virtual void UnlockRect( CubeFace eFace, UINT nLevel = 0 ) = 0;
	};
	//--------------------------------------------------------------------------------------------------------------
	typedef Stack< CubeTexture* >	CubeTextureList;
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
