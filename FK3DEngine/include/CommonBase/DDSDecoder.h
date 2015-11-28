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
*	文件说明：	DDS 图像解码器
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------

	// DirectDraw 像素格式结构
	struct DDPIXELFORMAT
	{
		DWORD	dwSize;				//结构大小（必须为 32）
		DWORD	dwFlags;			//格式选项标志（DDPF_RGB 未压缩格式 DDPF_FOURCC 压缩格式）
		DWORD	dwFourCC;			//压缩格式类型标志
		DWORD	dwRGBBitCount;		//未压缩像素位数
		DWORD	dwRBitMask;			//红色通道掩码
		DWORD	dwGBitMask;			//绿色通道掩码
		DWORD	dwBBitMask;			//蓝色通道掩码
		DWORD	dwRGBAlphaBitMask;	//透明通道掩码
	};

	// DirectDraw 选项
	struct DDCAPS2
	{
		DWORD	dwCaps1;			//选项 1
		DWORD	dwCaps2;			//选项 2
		DWORD	dwReserved[2];		//保留
	};

	// DirectDraw 表面描述结构
	struct DDSURFACEDESC2
	{
		DWORD			dwSize;					//结构大小（必须为 124）
		DWORD			dwFlags;				//选项标志（必须有 DDSD_CAPS, DDSD_PIXELFORMAT, DDSD_WIDTH, DDSD_HEIGHT.）
		DWORD			dwHeight;				//主图像高度
		DWORD			dwWidth;				//主图像宽度
		DWORD			dwPitchOrLinearSize;	//未压缩主图像跨度（DDSD_PITCH）或压缩主图像大小（DDSD_LINEARSIZE）
		DWORD			dwDepth;				//体积纹理深度
		DWORD			dwMipMapCount;			//图像纹理级数（DDSD_MIPMAPCOUNT）
		DWORD			dwReserved1[11];		//保留 1
		DDPIXELFORMAT	ddpfPixelFormat;		//像素格式
		DDCAPS2			ddsCaps;				//DirectDraw 选项
		DWORD			dwReserved2;			//保留 2
	};

	//纹理压缩格式标志
	const DWORD TEXC_DXT1			= FK_FOURCC( 'D', 'X', 'T', '1' );
	const DWORD TEXC_DXT2			= FK_FOURCC( 'D', 'X', 'T', '2' );
	const DWORD TEXC_DXT3			= FK_FOURCC( 'D', 'X', 'T', '3' );
	const DWORD TEXC_DXT4			= FK_FOURCC( 'D', 'X', 'T', '4' );
	const DWORD TEXC_DXT5			= FK_FOURCC( 'D', 'X', 'T', '5' );

	// DirectDraw 表面描述结构选项标志
	const DWORD DDSD_CAPS			= 0x00000001;
	const DWORD DDSD_HEIGHT			= 0x00000002;
	const DWORD DDSD_WIDTH			= 0x00000004;
	const DWORD DDSD_PITCH			= 0x00000008;
	const DWORD DDSD_PIXELFORMAT	= 0x00001000;
	const DWORD DDSD_MIPMAPCOUNT	= 0x00020000;
	const DWORD DDSD_LINEARSIZE		= 0x00080000;
	const DWORD DDSD_DEPTH			= 0x00800000;

	// DirectDraw 像素格式结构选项标志
	const DWORD DDPF_ALPHAPIXELS	= 0x00000001;
	const DWORD DDPF_FOURCC			= 0x00000004;
	const DWORD DDPF_RGB			= 0x00000040;

	// DirectDraw 选项 1
	const DWORD DDSCAPS_COMPLEX		= 0x00000008;
	const DWORD DDSCAPS_TEXTURE		= 0x00001000;
	const DWORD DDSCAPS_MIPMAP		= 0x00400000;

	// DirectDraw 选项 2
	const DWORD DDSCAPS2_CUBEMAP			= 0x00000200;
	const DWORD DDSCAPS2_CUBEMAP_POSITIVEX	= 0x00000400;
	const DWORD DDSCAPS2_CUBEMAP_NEGATIVEX	= 0x00000800;
	const DWORD DDSCAPS2_CUBEMAP_POSITIVEY	= 0x00001000;
	const DWORD DDSCAPS2_CUBEMAP_NEGATIVEY	= 0x00002000;
	const DWORD DDSCAPS2_CUBEMAP_POSITIVEZ	= 0x00004000;
	const DWORD DDSCAPS2_CUBEMAP_NEGATIVEZ	= 0x00008000;
	const DWORD DDSCAPS2_VOLUME				= 0x00200000;
	
	const DWORD DDSCAPS2_CUBEMAP_ALL = DDSCAPS2_CUBEMAP_POSITIVEX | DDSCAPS2_CUBEMAP_NEGATIVEX |
		DDSCAPS2_CUBEMAP_POSITIVEY | DDSCAPS2_CUBEMAP_NEGATIVEY | DDSCAPS2_CUBEMAP_POSITIVEZ | DDSCAPS2_CUBEMAP_NEGATIVEZ;

	// DDS 图像类型
	enum DDSImageType
	{
		DDS_Normal,		//普通纹理
		DDS_Cube,		//盒状纹理
		DDS_Volume,		//立体纹理
	};

	// DDS 图像级信息结构
	struct DDSImageLevel
	{
		DWORD			dwWidth;		//图像宽度
		DWORD			dwHeight;		//图像高度

		DWORD			dwDataLen;		//图像数据长度
		DWORD			dwDataOffset;	//图像数据偏移量
	};

	// DDS 图像文件解码信息结构
	struct DDSDecodeInfo
	{
		DWORD			dwLevelCount;	//图像级别数量
		DWORD			dwDecodeLevel;	//当前解码级别

		DDSImageType	eImageType;		//图像类型
		DDSImageLevel*	pImageLevel;	//图像级别信息
	};

	//--------------------------------------------------------------------------------------------------------------

	//解析 DDS 图像头
	extern void DDSParseHeader( ImageDecoder* pThis );

	//完成 DDS 图像解码
	extern void DDSEndDecode( ImageDecoder* pThis );

	//解码 DDS 图像数据
	extern void DDSDecode( ImageDecoder* pThis, BYTE* pDstBuf, RECT* srcRect, RECT* dstRect,
		int pitch, PixelFormat ePixelFormat );

	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
