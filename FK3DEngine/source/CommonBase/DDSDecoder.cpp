/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	DDSDecoder
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../../include/CommonBase/CommonBaseInsideHead.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//解析 DDS 图像头
	void DDSParseHeader( ImageDecoder* pThis )
	{
		//读取 DDS 文件头
		char szHeader[4];

		if( pThis->mbPreLoadInMem )
			pThis->mpImageData->Read( szHeader, 4 );
		else
			pThis->mpImageFile->Read( szHeader, 4 );

		if( 0 != memcmp( szHeader, "DDS ", 4 ) )
			Except( Exception::ERR_CANNOT_READ_FILE, "错误的 DDS 文件格式。" );

		//读取表面头
		DDSURFACEDESC2 sSurfaceDesc;
		if( pThis->mbPreLoadInMem )
			pThis->mpImageData->Read( &sSurfaceDesc, sizeof(sSurfaceDesc) );
		else
			pThis->mpImageFile->Read( &sSurfaceDesc, sizeof(sSurfaceDesc) );
			
		if( sSurfaceDesc.dwSize != 124 )
			Except( Exception::ERR_INTERNAL_ERROR, "错误的 DDS 文件表面描述结构大小。" );
		if( sSurfaceDesc.ddpfPixelFormat.dwSize != 32 )
			Except( Exception::ERR_INTERNAL_ERROR, "错误的 DDS 文件像素格式结构大小。" );

		//识别图像格式
		if( sSurfaceDesc.ddpfPixelFormat.dwFlags & DDPF_RGB )
		{
			switch (sSurfaceDesc.ddpfPixelFormat.dwRGBBitCount)
			{
			case 16:	pThis->mPixelFormat = PF_R5G6B5;	break;
			case 24:	pThis->mPixelFormat = PF_R8G8B8;	break;
			case 32:	pThis->mPixelFormat = PF_A8R8G8B8;	break;
			default:
				Except( Exception::ERR_INTERNAL_ERROR, "错误的 DDS 文件 RGB 像素格式位数。" );
			}
		}
		else if( sSurfaceDesc.ddpfPixelFormat.dwFlags & DDPF_FOURCC )
		{
			switch (sSurfaceDesc.ddpfPixelFormat.dwFourCC)
			{
			case TEXC_DXT1:		pThis->mPixelFormat = PF_DXT1;	break;
			case TEXC_DXT2:		pThis->mPixelFormat = PF_DXT2;	break;
			case TEXC_DXT3:		pThis->mPixelFormat = PF_DXT3;	break;
			case TEXC_DXT4:		pThis->mPixelFormat = PF_DXT4;	break;
			case TEXC_DXT5:		pThis->mPixelFormat = PF_DXT5;	break;
			default:
				Except( Exception::ERR_INTERNAL_ERROR, "不支持的 DDS 文件纹理压缩格式。" );
			}
		}
		else
			Except( Exception::ERR_INTERNAL_ERROR, "不支持的 DDS 文件图像格式。" );

		//存储图像尺寸
		pThis->mWidth = sSurfaceDesc.dwWidth;
		pThis->mHeight = sSurfaceDesc.dwHeight;

		//获取图像级数
		DWORD dwCaps1 = sSurfaceDesc.ddsCaps.dwCaps1;
		DWORD dwCaps2 = sSurfaceDesc.ddsCaps.dwCaps2;

		DWORD			dwLevelCount	= 1;
		DDSImageType	eDDSType		= DDS_Normal;

		//如果为多级纹理
		if( dwCaps1 & DDSCAPS_COMPLEX )
		{
			//如果为普通多级映射纹理
			if( dwCaps1 & DDSCAPS_MIPMAP )
			{
				eDDSType = DDS_Normal;
				dwLevelCount = sSurfaceDesc.dwMipMapCount;
			}
			//如果为盒状纹理
			else if( dwCaps2 & DDSCAPS2_CUBEMAP )
			{
				eDDSType = DDS_Cube;

				if( dwCaps2 ^ DDSCAPS2_CUBEMAP_ALL )
					Except( Exception::ERR_INTERNAL_ERROR, "不允许载入却少面 DDS 文件盒状纹理。" );

				dwLevelCount = 6;
			}
			//如果为立体纹理
			else if( dwCaps2 & DDSCAPS2_VOLUME )
			{
				eDDSType = DDS_Volume;
			}
		}

		//创建 DDS 解码信息数据
		DDSDecodeInfo* pDDSInfo = (DDSDecodeInfo*)malloc( sizeof(DDSDecodeInfo) + sizeof(DDSImageLevel) * dwLevelCount );
		DDSImageLevel* pDDSLevel = (DDSImageLevel*)pDDSInfo + 1;

		pDDSInfo->dwLevelCount	= dwLevelCount;
		pDDSInfo->dwDecodeLevel = 0;
		pDDSInfo->eImageType	= eDDSType;
		pDDSInfo->pImageLevel	= pDDSLevel;

		pThis->mpDDSInfo = pDDSInfo;

		//设置 DDS 图像级信息
		DWORD dwDataOffset = sizeof(DDSURFACEDESC2) + 4;
		DWORD dwWidth = sSurfaceDesc.dwWidth;
		DWORD dwHeight = sSurfaceDesc.dwHeight;

		//计算未压缩数据长度
		DWORD dwImageSize = 0;
		if( sSurfaceDesc.ddpfPixelFormat.dwFlags & DDPF_RGB )
			dwImageSize = sSurfaceDesc.dwPitchOrLinearSize * dwHeight;

		switch (eDDSType)
		{
		case DDS_Normal:
			{
				//设置所有映射级图像信息
				for( DWORD i=0; i<dwLevelCount; ++i, ++pDDSLevel )
				{
					pDDSLevel->dwWidth = dwWidth;
					pDDSLevel->dwHeight = dwHeight;

					//计算数据长度

					//如果为压缩数据
					if( dwImageSize == 0 )
					{
						DWORD dwSize = max( 1, dwWidth >> 2 ) * max( 1, dwHeight >> 2 );
						if( pThis->mPixelFormat == PF_DXT1 )
							dwSize <<= 3;
						else
							dwSize <<= 4;

						pDDSLevel->dwDataLen = dwSize;
					}
					//如果为未压缩数据
					else
					{
						pDDSLevel->dwDataLen = dwImageSize;
						dwImageSize >>= 2;
					}

					pDDSLevel->dwDataOffset = dwDataOffset;
					dwDataOffset += pDDSLevel->dwDataLen;

					dwWidth >>= 1;
					dwHeight >>= 1;
				}
				break;
			}
		/*case DDS_Cube:
			{
				break;
			}
		case DDS_Volume:
			{
				break;
			}*/
		default:
			Except( Exception::ERR_INTERNAL_ERROR, "错误的 DDS 纹理类型。" );
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//完成 DDS 图像解码
	void DDSEndDecode( ImageDecoder* pThis )
	{
		//释放 DDS 解码信息数据
		SafeFree( pThis->mpDDSInfo );
	}
	//--------------------------------------------------------------------------------------------------------------
	//解码 DDS 图像数据
	void DDSDecode( ImageDecoder* pThis, BYTE* pDstBuf, RECT* srcRect, RECT* dstRect,
		int pitch, PixelFormat ePixelFormat )
	{
		//获取当前解码图像级数据
		DWORD dwDecodeLevel = pThis->mpDDSInfo->dwDecodeLevel;
		if( dwDecodeLevel >= pThis->mpDDSInfo->dwLevelCount )
			Except( Exception::ERR_INVALIDPARAMS, "指定解码的 DDS 图像级别不存在。" );

		DDSImageLevel* pDDSLevel = pThis->mpDDSInfo->pImageLevel + dwDecodeLevel;

		//如果为压缩纹理格式
		if( pThis->mPixelFormat & PF_COMPRESS_MASK )
		{
			if( srcRect != NULL || dstRect != NULL )
				Except( Exception::ERR_INVALIDPARAMS, "无法指定纹理压缩格式 DDS 图像的解码区域。" );

			if( ePixelFormat != pThis->mPixelFormat )
				Except( Exception::ERR_INVALIDPARAMS, "必须使用当前 DDS 图像的纹理压缩格式进行解码。" );

			//分配数据缓存
			BYTE* pSrcBuf = (BYTE*)malloc( pDDSLevel->dwDataLen );
			if( pSrcBuf == NULL )
				Except( Exception::ERR_NOT_ENOUGH_MEM, "分配 DDS 图像解码缓存失败。" );

			//将压缩数据读入缓存
			if( pThis->mbPreLoadInMem )
			{
				pThis->mpImageData->SetPosition( pDDSLevel->dwDataOffset, BP_Begin );
				pThis->mpImageData->Read( pSrcBuf, pDDSLevel->dwDataLen );
			}
			else
			{
				pThis->mpImageFile->SetPosition( pDDSLevel->dwDataOffset, BP_Begin );
				pThis->mpImageFile->Read( pSrcBuf, pDDSLevel->dwDataLen );
			}

			//复制压缩数据
			memcpy( pDstBuf, pSrcBuf, pDDSLevel->dwDataLen );

			free( pSrcBuf );
		}
		else
		{
			Except( Exception::ERR_INVALIDPARAMS, "无法解码非纹理压缩格式的 DDS 图像。" );
		}
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
