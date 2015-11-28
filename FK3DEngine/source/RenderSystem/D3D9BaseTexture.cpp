/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	D3D9BaseTexture
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
	D3D9BaseTexture::D3D9BaseTexture()
		: mpBaseTexture		(NULL)
		, mpD3D9Texture		(NULL)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	D3D9BaseTexture::~D3D9BaseTexture()
	{
		SafeRelease( mpD3D9Texture );
	}
	//--------------------------------------------------------------------------------------------------------------
	//自动生成多级纹理映射
	void D3D9BaseTexture::_GenerateMipSubLevels( TextureFilterType eFilterType )
	{
		HRESULT result = mpD3D9Texture->SetAutoGenFilterType( D3D9TypeGet::GetTextureFilter( eFilterType ) );
		if( FAILED( result ) )
			Except( Exception::ERR_RENDER_API_ERROR, (String)"Direct3D 9 自动生成多级纹理映射错误。"
			"\n生成级别：" + mpBaseTexture->mNumLevel + "\nD3D9 错误描述：" + DXGetErrorDescription9(result) );

		mpD3D9Texture->GenerateMipSubLevels();
	}
	//--------------------------------------------------------------------------------------------------------------
	//根据纹理用途获取纹理内存管理方式和 D3D 纹理用途标识
	DWORD D3D9BaseTexture::_GetPoolModeAndUsage()
	{
		//获取纹理内存管理方式
		switch (mpBaseTexture->mType)
		{
		case TU_NORMAL:
			mpBaseTexture->mPoolMode = MPM_AGPMEM;
			break;
		case TU_DYNAMIC:
		case TU_TARGET:
			mpBaseTexture->mPoolMode = MPM_VIDEOMEM;
			break;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "错误的 Direct3D 9 纹理创建类型。" );
		}

		//如果纹理级数等于零则要求自动生成 Mipmap
		DWORD dwUsage = D3D9TypeGet::GetTextureUsage( mpBaseTexture->mType );
		if( mpBaseTexture->mNumLevel == 0 )
			dwUsage |= D3DUSAGE_AUTOGENMIPMAP;

		return dwUsage;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
