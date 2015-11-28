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
*	文件说明：	JPG 图像解码器
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------------------------------------------------

#include "../../depend/interJpeg/jpeglib.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------

	// IJL 错误处理数据结构
	struct IJL_ErrData
	{
		jpeg_error_mgr	IJL_ErrMgr;						// IJL 错误管理数据结构
		char			szErrStrBuf[JMSG_LENGTH_MAX];	// IJL 错误消息缓存
		bool			bError;							//是否发生错误
	};

	// IJL 解码信息数据结构
	struct IJL_DecodeInfo
	{
		jpeg_decompress_struct	DecompData;				// IJL 解码数据
		IJL_ErrData				ErrData;				// IJL 错误处理数据
	};

	// IJL 错误处理函数
	METHODDEF(void) IJL_ErrProc( j_common_ptr pInfo );

	//--------------------------------------------------------------------------------------------------------------

	//解析 JPG 图像头
	extern void JpgParseHeader( ImageDecoder* pThis );

	//完成 JPG 图像解码
	extern void JpgEndDecode( ImageDecoder* pThis );

	//解码 JPG 图像数据
	extern void JpgDecode( ImageDecoder* pThis, BYTE* pDstBuf, RECT* srcRect, RECT* dstRect,
		int pitch, PixelFormat ePixelFormat );

	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
