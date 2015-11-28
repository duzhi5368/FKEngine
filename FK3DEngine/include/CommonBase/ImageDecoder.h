/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	ImageDecoder
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	图像解码器
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------

	class FileObject;
	class ImageDecoder;
	struct DDSDecodeInfo;

	//解析图像头函数
	typedef void ( *FnImageParseHeader )( ImageDecoder* pThis );

	//完成图像解码函数
	typedef void ( *FnImageEndDecode )( ImageDecoder* pThis );

	//解码图象数据函数
	typedef void ( *FnImageDecode )( ImageDecoder* pThis, BYTE* pDstBuf, RECT* srcRect, RECT* dstRect,
		int pitch, PixelFormat ePixelFormat );

	//--------------------------------------------------------------------------------------------------------------
	//图像解码器
	class FK_DLL ImageDecoder
	{
	public:
		FnImageParseHeader		mFnParseHeader;	//解析图像头函数
		FnImageEndDecode		mFnEndDecode;	//完成图像解码函数
		FnImageDecode			mFnDecode;		//解码图象数据函数

		FileObject*				mpImageFile;	//图像文件
		DataChunk				mImageData;		//解码器图像数据缓存
		DataChunk*				mpImageData;	//图像解码读取数据缓存

		bool					mbIsLoadImage;	//是否加载了图像
		bool					mbPreLoadInMem;	//是否预先将文件读入内存

		UINT					mWidth;			//图像宽
		UINT					mHeight;		//图像高
		PixelFormat				mPixelFormat;	//图像格式

		DDSDecodeInfo*			mpDDSInfo;		// DDS 解码信息数据
		void*					mpExtData;		//解码额外数据指针

	public:
		ImageDecoder();
		~ImageDecoder();

		//从文件载入图像
		void LoadImageFromFile( FileObject* pImageFile, const char* szFileName, const bool bPreLoadInMem = true );

		//从内存载入图像
		void LoadImageFromMem( DataChunk* pImageData, ImageType eType );

		//卸载图像
		bool UnloadImage();

		//获取图像宽、高。
		void GetDimension( UINT& nWidth, UINT& nHeight );

		//获取图像格式
		PixelFormat GetFormat();

		//解析图像头
		void ParseHeader();

		//解码图像数据
		void Decode( BYTE* pDstBuf, RECT* srcRect, RECT* dstRect, int pitch, PixelFormat ePixelFormat );
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
