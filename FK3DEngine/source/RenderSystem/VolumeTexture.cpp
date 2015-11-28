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

#include "../../include/RenderSystem/RenderSystemInsideHeader.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	VolumeTexture::VolumeTexture( UINT nWidth, UINT nHeight, UINT nDepth,
		PixelFormat ePixelFormat, UINT nNumLevel, TextureUsage Type )
		: BaseTexture		( ePixelFormat, nNumLevel, Type )
		, mWidth			(nWidth)
		, mHeight			(nHeight)
		, mDepth			(nDepth)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	VolumeTexture::~VolumeTexture()
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取纹理实际尺寸
	FKInline void VolumeTexture::GetTextureSize( UINT& nWidth, UINT& nHeight, UINT& nDepth )
	{
		nWidth	= mWidth;
		nHeight	= mHeight;
		nDepth	= nDepth;
	}
	//--------------------------------------------------------------------------------------------------------------
	//填充指定纹理区域为某颜色
	void VolumeTexture::FillColor( DWORD color, Box3* pFillBox, UINT nLevel )
	{
		//计算填充宽高深
		UINT width	= ( pFillBox != NULL ) ? pFillBox->nRight - pFillBox->nLeft : mWidth;
		UINT height	= ( pFillBox != NULL ) ? pFillBox->nFront - pFillBox->nBack : mHeight;
		UINT depth	= ( pFillBox != NULL ) ? pFillBox->nBottom - pFillBox->nTop : mDepth;

		//锁定纹理
		LockBox3 sLockBox;
		LockBox( &sLockBox, pFillBox, nLevel );

		BYTE* pDstBuf = (BYTE*)sLockBox.pBits;

		switch (mPixelFormat)
		{
		case PF_X8R8G8B8:
		case PF_A8R8G8B8:
			{
				//深循环
				for( UINT i=0; i<depth; ++i )
				{
					//高循环
					for( UINT j=0; j<height; ++j )
					{
						DWORD* pDst = (DWORD*)pDstBuf;

						//宽循环
						for( UINT k=0; k<width; ++k )
							*pDst++ = color;

						pDstBuf += sLockBox.nRowPitch;
					}

					pDstBuf += sLockBox.nSlicePitch;
				}
				break;
			}
		case PF_R5G6B5:
			{
				//计算颜色数据
				BYTE* pByteColor = (BYTE*)&color;
				BYTE b = (*pByteColor++) >>	3;	//读取 B
				BYTE g = (*pByteColor++) >>	2;	//读取 G
				BYTE r = (*pByteColor++) >>	3;	//读取 R
				WORD newColor = b | (g<<5) | (r<<11);

				//深循环
				for( UINT i=0; i<depth; ++i )
				{
					//高循环
					for( UINT j=0; j<height; ++j )
					{
						WORD* pDst = (WORD*)pDstBuf;

						//宽循环
						for( UINT k=0; k<width; ++k )
							*pDst++ = newColor;

						pDstBuf += sLockBox.nRowPitch;
					}

					pDstBuf += sLockBox.nSlicePitch;
				}
				break;
			}
		case PF_A1R5G5B5:
			{
				//计算颜色数据
				BYTE* pByteColor = (BYTE*)&color;
				BYTE b = (*pByteColor++) >>	3;	//读取 B
				BYTE g = (*pByteColor++) >>	2;	//读取 G
				BYTE r = (*pByteColor++) >>	3;	//读取 R
				BYTE a = (*pByteColor++) >>	7;	//读取 A

				WORD newColor = b | (g<<5) | (r<<10) | (a<<11);

				//深循环
				for( UINT i=0; i<depth; ++i )
				{
					//高循环
					for( UINT j=0; j<height; ++j )
					{
						WORD* pDst = (WORD*)pDstBuf;

						//宽循环
						for( UINT k=0; k<width; ++k )
							*pDst++ = color;

						pDstBuf += sLockBox.nRowPitch;
					}

					pDstBuf += sLockBox.nSlicePitch;
				}
				break;
			}
		case PF_A4R4G4B4:
			{
				//计算颜色数据
				BYTE* pByteColor = (BYTE*)&color;
				BYTE b = (*pByteColor++) >>	4;	//读取 B
				BYTE g = (*pByteColor++) >>	4;	//读取 G
				BYTE r = (*pByteColor++) >>	4;	//读取 R
				BYTE a = (*pByteColor++) >>	4;	//读取 A

				WORD newColor = b | (g<<4) | (r<<8) | (a<<12);

				//深循环
				for( UINT i=0; i<depth; ++i )
				{
					//高循环
					for( UINT j=0; j<height; ++j )
					{
						WORD* pDst = (WORD*)pDstBuf;

						//宽循环
						for( UINT k=0; k<width; ++k )
							*pDst++ = color;

						pDstBuf += sLockBox.nRowPitch;
					}

					pDstBuf += sLockBox.nSlicePitch;
				}
				break;
			}
		default:
			Except( Exception::ERR_INTERNAL_ERROR, "错误的纹理像素格式。" );
		}

		//解锁纹理
		UnlockBox();
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
