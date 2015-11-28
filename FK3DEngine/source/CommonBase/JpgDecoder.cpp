/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	JpgDecoder
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../../include/CommonBase/CommonBaseInsideHead.h"

//==================================================================================================================

#ifdef __cplusplus 
	extern "C" 
	{
#endif

#include "../../depend/interJpeg/jpeglib.h"

#ifdef __cplusplus 
	}
#endif 

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	// IJL 错误处理函数
	METHODDEF(void) IJL_ErrProc( j_common_ptr pInfo )
	{
		//获取错误数据
		IJL_ErrData* pErrData = (IJL_ErrData*)pInfo->err;

		//格式化错误消息
		( *pInfo->err->format_message )( pInfo, pErrData->szErrStrBuf );

		pErrData->bError = true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//解析 JPG 图像头
	void JpgParseHeader( ImageDecoder* pThis )
	{
		//解码 JPG 时必需要求将整个文件预先读入内存
		if( !pThis->mbPreLoadInMem )
			Except( Exception::ERR_INVALIDPARAMS, "解码 JPG 时必需要求将整个文件预先读入内存。" );


		//IJL 解码信息
		IJL_DecodeInfo IJLDecodeInfo;
		memset( &IJLDecodeInfo, 0, sizeof(IJL_DecodeInfo) );

		jpeg_decompress_struct* pDecompData = &IJLDecodeInfo.DecompData;
		IJL_ErrData* pErrData = &IJLDecodeInfo.ErrData;

		//初始化 IJL 错误处理数据
		pDecompData->err = jpeg_std_error( &pErrData->IJL_ErrMgr );
		pErrData->IJL_ErrMgr.error_exit = IJL_ErrProc;

		//创建解码器
		jpeg_create_decompress( pDecompData );
		if( pErrData->bError )
		{
			Except( Exception::ERR_INTERNAL_ERROR, (String)"创建 JPG 解码器错误。\n"
				"IJL 错误消息：" + pErrData->szErrStrBuf );
		}

		static MemStream sMemStream;
		sMemStream.pData = (char*)pThis->mpImageData->GetPointer();
		sMemStream.nRemLen = pThis->mpImageData->GetProtectedLen();
		sMemStream.pPos = sMemStream.pData;

		//设置数据输入源
		jpeg_stdio_src( pDecompData, &sMemStream );
		if( pErrData->bError )
		{
			Except( Exception::ERR_INTERNAL_ERROR, (String)"设置 JPG 图像数据输入源错误。\n"
				"IJL 错误消息：" + pErrData->szErrStrBuf );
		}

		//读取 JPG 图像头
		jpeg_read_header( pDecompData, TRUE );
		if( pErrData->bError )
		{
			Except( Exception::ERR_INTERNAL_ERROR, (String)"读取 JPG 图像数据头错误。\n"
				"IJL 错误消息：" + pErrData->szErrStrBuf );
		}

		//开始进行解码
		jpeg_start_decompress( pDecompData );
		if( pErrData->bError )
		{
			Except( Exception::ERR_INTERNAL_ERROR, (String)"开始进行 JPG 图像数据解码错误。\n"
				"IJL 错误消息：" + pErrData->szErrStrBuf );
		}

		if( pDecompData->output_components != 3 )
			Except( Exception::ERR_INTERNAL_ERROR, "错误的 JPG 图像解码数据颜色格式。" );

		pThis->mWidth		= pDecompData->output_width;
		pThis->mHeight		= pDecompData->output_height;
		pThis->mPixelFormat	= PF_R8G8B8;


		//预先将整个图像数据解码到缓存中
		BYTE* pImageBuf = (BYTE*)malloc( pThis->mWidth * pThis->mHeight * 3 );
		BYTE* pDecodePtr = pImageBuf;

		int nDecodeWidthLen = pDecompData->output_width * 3;

		for( UINT i=0; i<pThis->mHeight; ++i )
		{
			//解码一行
			if( 1 != jpeg_read_scanlines( pDecompData, (JSAMPARRAY)&pDecodePtr, 1 ) )
			{
				free( pImageBuf );
				Except( Exception::ERR_INTERNAL_ERROR, "解码 JPG 图像数据失败。" );
			}

			pDecodePtr += nDecodeWidthLen;
		}

		//结束解码
		jpeg_finish_decompress( &IJLDecodeInfo.DecompData );
		if( pErrData->bError )
		{
			Except( Exception::ERR_INTERNAL_ERROR, (String)"结束 JPG 图像数据解码错误。\n"
				"IJL 错误消息：" + pErrData->szErrStrBuf );
		}

		//摧毁解码器
		jpeg_destroy_decompress( &IJLDecodeInfo.DecompData );
		if( pErrData->bError )
		{
			Except( Exception::ERR_INTERNAL_ERROR, (String)"摧毁 JPG 图像解码器错误。\n"
				"IJL 错误消息：" + pErrData->szErrStrBuf );
		}

		pThis->mpExtData = pImageBuf;
	}
	//--------------------------------------------------------------------------------------------------------------
	//完成 JPG 图像解码
	void JpgEndDecode( ImageDecoder* pThis )
	{
		SafeFree( pThis->mpExtData );
	}
	//--------------------------------------------------------------------------------------------------------------
	//解码 JPG 图像数据
	void JpgDecode( ImageDecoder* pThis, BYTE* pDstBuf, RECT* srcRect, RECT* dstRect,
		int pitch, PixelFormat ePixelFormat )
	{
		//求出来源矩形和填充矩形的交集
		UINT srcX = 0;
		UINT srcY = 0;
		UINT dstX = 0;
		UINT dstY = 0;
		UINT srcWidth = pThis->mWidth;
		UINT srcHeight = pThis->mHeight;
		UINT dstWidth = pThis->mWidth;
		UINT dstHeight = pThis->mHeight;

		if( dstRect != NULL )
		{
			dstX = dstRect->left;
			dstY = dstRect->top;
			dstWidth = dstRect->right - dstRect->left;
			dstHeight = dstRect->bottom - dstRect->top;
		}

		if( srcRect != NULL )
		{
			srcX = srcRect->left;
			srcY = srcRect->top;
			srcWidth = srcRect->right - srcRect->left;
			srcHeight = srcRect->bottom - srcRect->top;
		}

		srcWidth = ( srcWidth<dstWidth ) ? srcWidth : dstWidth;
		srcHeight = ( srcHeight<dstHeight ) ? srcHeight : dstHeight;

		//复制并转换指定区域图像数据

		//将输出指针移动到起始写入位置
		BYTE* pDst = pDstBuf + pitch * dstY + dstX;

		//跳到图像起始行读取位置
		UINT nImageWidthLen = 3 * pThis->mWidth;
		BYTE* pSrc = (BYTE*)pThis->mpExtData + nImageWidthLen * srcY + srcX * 3;

		switch(ePixelFormat)
		{
		case PF_A8R8G8B8:
		case PF_X8R8G8B8:
			{
				for( UINT col=0; col<srcHeight; ++col )
				{
					//行起始读取位置
					BYTE* pSrcColBeg = pSrc;
					BYTE* pDstColBeg = pDst;
					for( UINT row=0; row<srcWidth; ++row )
					{
						*pDst++ = *(pSrc+2);//读取 B
						*pDst++ = *(pSrc+1);//读取 G
						*pDst++ = *pSrc;	//读取 R
						*pDst++ = 0xFF;		//略过 A

						pSrc += 3;
					}
					//跳到下一行写入位置
					pSrc = pSrcColBeg + nImageWidthLen;
					pDst = pDstColBeg + pitch;
				}
			}
			break;
		case PF_R5G6B5:
			{
				for( UINT col=0; col<srcHeight; ++col )
				{
					//行起始读取位置
					BYTE* pSrcColBeg = pSrc;
					BYTE* pDstColBeg = pDst;
					for( UINT row=0; row<srcWidth; ++row )
					{
						BYTE r = (*pSrc++) >> 3;	//读取 R
						BYTE g = (*pSrc++) >> 2;	//读取 G
						BYTE b = (*pSrc++) >> 3;	//读取 B

						*(WORD*)pDst = b | (g<<5) | (r<<11);
						pDst += 2;
					}
					//跳到下一行写入位置
					pSrc = pSrcColBeg + nImageWidthLen;
					pDst = pDstColBeg + pitch;
				}
			}
			break;
		case PF_A1R5G5B5:
			{
				for( UINT col=0; col<srcHeight; ++col )
				{
					//行起始读取位置
					BYTE* pSrcColBeg = pSrc;
					BYTE* pDstColBeg = pDst;
					for( UINT row=0; row<srcWidth; ++row )
					{
						BYTE r = (*pSrc++) >> 3;	//读取 R
						BYTE g = (*pSrc++) >> 3;	//读取 G
						BYTE b = (*pSrc++) >> 3;	//读取 B

						*(WORD*)pDst = b | (g<<5) | (r<<10) | (1<<11);
						pDst += 2;
					}
					//跳到下一行写入位置
					pSrc = pSrcColBeg + nImageWidthLen;
					pDst = pDstColBeg + pitch;
				}
			}
			break;
		case PF_A4R4G4B4:
			{
				for( UINT col=0; col<srcHeight; ++col )
				{
					//行起始读取位置
					BYTE* pSrcColBeg = pSrc;
					BYTE* pDstColBeg = pDst;
					for( UINT row=0; row<srcWidth; ++row )
					{
						BYTE r = (*pSrc++) >> 4;	//读取 R
						BYTE g = (*pSrc++) >> 4;	//读取 G
						BYTE b = (*pSrc++) >> 4;	//读取 B

						*(WORD*)pDst = b | (g<<4) | (r<<8) | (16<<12);
						pDst += 2;
					}
					//跳到下一行写入位置
					pSrc = pSrcColBeg + nImageWidthLen;
					pDst = pDstColBeg + pitch;
				}
			}
			break;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "错误的 JPG 图像解码输出像素格式。" );
		}
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
