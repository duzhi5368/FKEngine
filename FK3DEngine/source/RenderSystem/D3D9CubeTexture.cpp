/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	D3D9CubeTexture
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
	D3D9CubeTexture::D3D9CubeTexture( UINT nEdgeLength, PixelFormat ePixelFormat, UINT nNumLevel, TextureUsage Type )
		: CubeTexture		( nEdgeLength, ePixelFormat, nNumLevel, Type )
	{
		mBaseTexture.mpBaseTexture = reinterpret_cast< BaseTexture* >( this );

		HRESULT result = E_FAIL;

		//根据纹理用途获取纹理内存管理方式和 D3D 纹理用途标识
		DWORD dwUsage = _GetPoolModeAndUsage();

		//创建纹理
		result = D3D9RenderSystem::mpD3D9Device->CreateCubeTexture( mEdgeLength, mNumLevel,
			dwUsage, D3D9TypeGet::GetPixelFormat( mPixelFormat ),
			D3D9TypeGet::GetPoolMode( mPoolMode ), (IDirect3DCubeTexture9**)&mpD3D9Texture, NULL );
		if( FAILED( result ) )
			Except( Exception::ERR_RENDER_API_ERROR, (String)"使用 Direct3D 9 设备创建立方纹理错误。"
			"\nD3D9 错误描述" + DXGetErrorDescription9(result) );
	}
	//--------------------------------------------------------------------------------------------------------------
	//锁定纹理
	void D3D9CubeTexture::LockRect( CubeFace eFace, void** ppBuf, UINT* pPitch, RECT* pLockRect, UINT nLevel, bool bDiscardLock )
	{
		//选择锁定方法
		DWORD flag = bDiscardLock ? D3DLOCK_DISCARD : D3DLOCK_NOSYSLOCK;

		//锁定纹理
		D3DLOCKED_RECT d3dLockRect;
		HRESULT result = reinterpret_cast< IDirect3DCubeTexture9* >( mpD3D9Texture )->LockRect(
			(D3DCUBEMAP_FACES)eFace, nLevel, &d3dLockRect, pLockRect, flag );

		if( FAILED( result ) )
			Except( Exception::ERR_INVALIDPARAMS, (String)"锁定 Direct3D 9 立方纹理错误。\nD3D9 错误描述：" +
			DXGetErrorDescription9(result) );

		*ppBuf = d3dLockRect.pBits;
		*pPitch = d3dLockRect.Pitch;
	}
	//--------------------------------------------------------------------------------------------------------------
	//解锁纹理
	void D3D9CubeTexture::UnlockRect( CubeFace eFace, UINT nLevel )
	{
		HRESULT result = reinterpret_cast< IDirect3DCubeTexture9* >( mpD3D9Texture )->UnlockRect(
			(D3DCUBEMAP_FACES)eFace, nLevel );

		if( FAILED( result ) )
			Except( Exception::ERR_INVALIDPARAMS, (String)"解锁 Direct3D 9 立方纹理错误。\nD3D9 错误描述：" +
			DXGetErrorDescription9(result) );
	}
	//--------------------------------------------------------------------------------------------------------------
	//自动生成多级纹理映射
	void D3D9CubeTexture::GenerateMipSubLevels( TextureFilterType eFilterType )
	{
		_GenerateMipSubLevels( eFilterType );
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
