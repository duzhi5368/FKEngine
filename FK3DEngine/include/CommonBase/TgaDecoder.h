/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	TgaDecoder
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	TGA 图像解码器
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------

	#pragma pack( push )
	#pragma pack( 1 )

	// TGA 文件头
	struct TgaFileHeader
	{
		BYTE	bIDLen;			//文件描述 ID 字符串长度
		BYTE	bHavePal;		//是否存在调色板

		BYTE	bCompFlag;		//图像压缩类型

								// 0 - 无图像
								// 1 - 调色板 未压缩
								// 2 - 真彩色 未压缩
								// 3 - 黑白色 未压缩
								// 9 - 调色板 行程压缩
								// 10 - 真彩色 行程压缩
								// 11 - 黑白色 行程压缩

		WORD	wPalFirstUnit;	//图像中使用的第一个调色板项索引值
		WORD	wPalSize;		//调色板项数

		BYTE	bPalUnitBits;	//调色板项位数

								// 15 - RRRRR GGGGG BBBBB
								// 16 - A RRRRR GGGGG BBBBB
								// 24 - BBBBBBBB GGGGGGGG RRRRRRRR
								// 32 - BBBBBBBB GGGGGGGG RRRRRRRR AAAAAAAA

		WORD	wPicX;			//图像左边与屏幕左边 像素距离
		WORD	wPicY;			//图像底边在屏幕底边 像素距离
		WORD	wPicWidth;		//图像宽度
		WORD	wPicHeight;		//图像高度

		BYTE	bPixelBits;		//每像素位数

		BYTE	bPicInfoBits;	//图像描述符

								//低四位	每像素属性位数目

								//中间两位	行中像素存储顺序和行在文件中存储顺序

								//			0x00 - 左到右 下到上
								//			0x10 - 右到左 下到上
								//			0x20 - 左到右 上到下
								//			0x30 - 右到左 上到下

								//高两位	扫描行交叉类型

								//			0x00 - 无交叉
								//			0x40 - 两路奇偶交叉
								//			0x80 - 四路交叉
	};

	// TGA 文件尾
	struct TgaFileFooter
	{
		int		nExtOffset;		//扩展区域偏移量
		int		nDevListOffset;	//开发员列表偏移量
		char	szFooter[18];	// "TRUEVISION-TARGA." 字符串
	};

	#pragma pack( pop )

	//--------------------------------------------------------------------------------------------------------------

	// TGA 图像解码额外数据
	struct TgaExtData
	{
		TgaFileHeader	sTgaFileHeader;	// TGA 文件头

		BYTE*			pPalData;		//调色板数据

		bool			bUsePal;		//是否使用调色板
		bool			bUseRLE;		//是否使用行程压缩
		bool			bYReversed;		//是否行序反转
		long			nPicDataOffset;	//图像数据偏移量
	};

	//--------------------------------------------------------------------------------------------------------------

	//解析 TGA 图像头
	extern void TgaParseHeader( ImageDecoder* pThis );

	//完成 TGA 图像解码
	extern void TgaEndDecode( ImageDecoder* pThis );

	//解码 TGA 图像数据
	extern void TgaDecode( ImageDecoder* pThis, BYTE* pDstBuf, RECT* srcRect, RECT* dstRect,
		int pitch, PixelFormat ePixelFormat );

	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
