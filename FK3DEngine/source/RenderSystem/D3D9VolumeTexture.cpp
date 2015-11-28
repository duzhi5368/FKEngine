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
	D3D9VolumeTexture::D3D9VolumeTexture( UINT nWidth, UINT nHeight, UINT nDepth,
		PixelFormat ePixelFormat, UINT nNumLevel, TextureUsage Type )
		: VolumeTexture		( nWidth, nHeight, nDepth, ePixelFormat, nNumLevel, Type )
	{
		mBaseTexture.mpBaseTexture = reinterpret_cast< BaseTexture* >( this );

		HRESULT result = E_FAIL;

		//根据纹理用途获取纹理内存管理方式和 D3D 纹理用途标识
		DWORD dwUsage = _GetPoolModeAndUsage();

		//创建纹理
		result = D3D9RenderSystem::mpD3D9Device->CreateVolumeTexture( mWidth, mHeight, mDepth, mNumLevel,
			dwUsage, D3D9TypeGet::GetPixelFormat( mPixelFormat ),
			D3D9TypeGet::GetPoolMode( mPoolMode ), (IDirect3DVolumeTexture9**)&mpD3D9Texture, NULL );
		if( FAILED( result ) )
			Except( Exception::ERR_RENDER_API_ERROR, (String)"使用 Direct3D 9 设备创建三维纹理错误。"
			"\nD3D9 错误描述" + DXGetErrorDescription9(result) );
	}
	//--------------------------------------------------------------------------------------------------------------
	//锁定纹理
	void D3D9VolumeTexture::LockBox( LockBox3* pLocked, Box3* pLockBox, UINT nLevel, bool bDiscardLock )
	{
		//选择锁定方法
		DWORD flag = bDiscardLock ? D3DLOCK_DISCARD : D3DLOCK_NOSYSLOCK;

		//锁定纹理
		HRESULT result = reinterpret_cast< IDirect3DVolumeTexture9* >( mpD3D9Texture )->LockBox(
			nLevel, (D3DLOCKED_BOX*)pLocked, (D3DBOX*)pLockBox, flag );

		if( FAILED( result ) )
			Except( Exception::ERR_INVALIDPARAMS, (String)"锁定 Direct3D 9 三维纹理错误。\nD3D9 错误描述：" +
			DXGetErrorDescription9(result) );
	}
	//--------------------------------------------------------------------------------------------------------------
	//解锁纹理
	void D3D9VolumeTexture::UnlockBox( UINT nLevel )
	{
		HRESULT result = reinterpret_cast< IDirect3DVolumeTexture9* >( mpD3D9Texture )->UnlockBox( nLevel );

		if( FAILED( result ) )
			Except( Exception::ERR_INVALIDPARAMS, (String)"解锁 Direct3D 9 三维纹理错误。\nD3D9 错误描述：" +
			DXGetErrorDescription9(result) );
	}
	//--------------------------------------------------------------------------------------------------------------
	//自动生成多级纹理映射
	void D3D9VolumeTexture::GenerateMipSubLevels( TextureFilterType eFilterType )
	{
		_GenerateMipSubLevels( eFilterType );
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
