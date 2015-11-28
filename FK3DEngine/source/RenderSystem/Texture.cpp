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

#include "../../include/RenderSystem/RenderSystemInsideHeader.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	Texture::Texture( UINT nWidth, UINT nHeight, PixelFormat ePixelFormat, UINT nNumLevel, TextureUsage Type )
		: BaseTexture		( ePixelFormat, nNumLevel, Type )
		, mWidth			(nWidth)
		, mHeight			(nHeight)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	Texture::~Texture()
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取纹理实际尺寸
	FKInline void Texture::GetTextureSize( UINT& nWidth, UINT& nHeight )
	{
		nWidth = mWidth;
		nHeight = mHeight;
	}
	//--------------------------------------------------------------------------------------------------------------
	//填充指定纹理区域为某颜色
	void Texture::FillColor( DWORD color, RECT* pFillRect, UINT nLevel )
	{
		BYTE* pDstBuf = NULL;
		UINT pitch = 0;

		//计算填充宽高
		UINT width = ( pFillRect != NULL ) ? pFillRect->right - pFillRect->left : mWidth;
		UINT height = ( pFillRect != NULL ) ? pFillRect->bottom - pFillRect->top : mHeight;

		//锁定纹理
		LockRect( (void**)&pDstBuf, &pitch, pFillRect, nLevel );

		switch (mPixelFormat)
		{
		case PF_X8R8G8B8:
		case PF_A8R8G8B8:
			{
				//行循环
				for( UINT row=0; row<height; ++row )
				{
					DWORD* pDst = (DWORD*)pDstBuf;

					//列循环
					for( UINT col=0; col<width; ++col )
						*pDst++ = color;

					pDstBuf += pitch;
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

				//行循环
				for( UINT row=0; row<height; ++row )
				{
					WORD* pDst = (WORD*)pDstBuf;

					//列循环
					for( UINT col=0; col<width; ++col )
						*pDst++ = newColor;

					pDstBuf += pitch;
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

				//行循环
				for( UINT row=0; row<height; ++row )
				{
					WORD* pDst = (WORD*)pDstBuf;

					//列循环
					for( UINT col=0; col<width; ++col )
						*pDst++ = newColor;

					pDstBuf += pitch;
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

				//行循环
				for( UINT row=0; row<height; ++row )
				{
					WORD* pDst = (WORD*)pDstBuf;

					//列循环
					for( UINT col=0; col<width; ++col )
						*pDst++ = newColor;

					pDstBuf += pitch;
				}
				break;
			}
		default:
			Except( Exception::ERR_INTERNAL_ERROR, "错误的纹理像素格式。" );
		}

		//解锁纹理
		UnlockRect();
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
