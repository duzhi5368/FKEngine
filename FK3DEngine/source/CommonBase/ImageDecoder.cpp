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
	ImageDecoder::ImageDecoder()
		: mFnParseHeader	(NULL)
		, mFnDecode			(NULL)
		, mFnEndDecode		(NULL)
		, mbIsLoadImage		(false)
		, mpImageFile		(NULL)
		, mpImageData		(NULL)
		, mbPreLoadInMem	(false)
		, mWidth			(0)
		, mHeight			(0)
		, mPixelFormat		(PF_UNKNOWN)
		, mpDDSInfo			(NULL)
		, mpExtData			(NULL)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	ImageDecoder::~ImageDecoder()
	{
		if( mbIsLoadImage )
			UnloadImage();
	}
	//--------------------------------------------------------------------------------------------------------------
	//从文件载入图像
	void ImageDecoder::LoadImageFromFile( FileObject* pImageFile, const char* szFileName, const bool bPreLoadInMem )
	{
		mpImageFile = pImageFile;
		mbPreLoadInMem = bPreLoadInMem;

		//打开文件
		pImageFile->Open( szFileName, OT_Read );

		//如果指定预先将图像数据载入内存
		if( mbPreLoadInMem )
		{
			DWORD dwFileLen = pImageFile->GetLength();
			mImageData.Allocate( dwFileLen );
			pImageFile->Read( mImageData.GetPointer(), dwFileLen );
			mpImageData = &mImageData;
		}

		//根据文件扩展名判断文件类型
		static char szExtName[32] = { NULL };
		UINT nExtPos = Misc::GetLastCharIndex( (char*)szFileName, '.' ) + 1;
		memcpy( szExtName, szFileName + nExtPos, 32 );
		strlwr( szExtName );

		UINT nExtHash = Misc::HashString( szExtName );

		//将函数指针指向相应的解码函数
		switch (nExtHash)
		{
			//BMP 图像
		case BmpExtNameID:
			{
				mFnParseHeader	= BmpParseHeader;
				mFnEndDecode	= BmpEndDecode;
				mFnDecode		= BmpDecode;
				break;
			}
			//TGA 图像
		case TgaExtNameID:
			{
				mFnParseHeader	= TgaParseHeader;
				mFnEndDecode	= TgaEndDecode;
				mFnDecode		= TgaDecode;
				break;
			}
			//JPG 图像
		case JpgExtNameID:
			{
				mFnParseHeader	= JpgParseHeader;
				mFnEndDecode	= JpgEndDecode;
				mFnDecode		= JpgDecode;
				break;
			}
			//DDS 图像
		case DDSExtNameID:
			{
				mFnParseHeader	= DDSParseHeader;
				mFnEndDecode	= DDSEndDecode;
				mFnDecode		= DDSDecode;
				break;
			}
		default:
			Except( Exception::ERR_INTERNAL_ERROR, (String)"无法根据图像文件 '" + szFileName +
				"' 的扩展名识别图像类型。");
		}

		mbIsLoadImage = true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//从内存载入图像
	void ImageDecoder::LoadImageFromMem( DataChunk* pImageData, ImageType eType )
	{
		mpImageData = pImageData;
		mbPreLoadInMem = true;

		//根据文件类型将函数指针指向相应的解码函数
		switch (eType)
		{
			//BMP 图像
		case eBMP:
			{
				mFnParseHeader	= BmpParseHeader;
				mFnEndDecode	= BmpEndDecode;
				mFnDecode		= BmpDecode;
				break;
			}
			//TGA 图像
		case eTGA:
			{
				mFnParseHeader	= TgaParseHeader;
				mFnEndDecode	= TgaEndDecode;
				mFnDecode		= TgaDecode;
				break;
			}
			//JPG 图像
		case eJPG:
			{
				mFnParseHeader	= JpgParseHeader;
				mFnEndDecode	= JpgEndDecode;
				mFnDecode		= JpgDecode;
				break;
			}
			//DDS 图像
		case eDDS:
			{
				mFnParseHeader	= DDSParseHeader;
				mFnEndDecode	= DDSEndDecode;
				mFnDecode		= DDSDecode;
				break;
			}
		default:
			Except( Exception::ERR_INVALIDPARAMS, "错误的图像类型。");
		}

		mbIsLoadImage = true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//卸载图像文件
	bool ImageDecoder::UnloadImage()
	{
		bool bResult = true;

		mFnEndDecode( this );

		//关闭文件
		if( mpImageFile != NULL )
			bResult = mpImageFile->Close();

		mFnParseHeader	= NULL;
		mFnDecode		= NULL;
		mpImageFile		= NULL;
		mpImageData		= NULL;
		mbPreLoadInMem	= false;
		mWidth			= 0;
		mHeight			= 0;
		mPixelFormat	= PF_UNKNOWN;

		//清空图像数据
		mImageData.Free();

		mbIsLoadImage = false;

		return bResult;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取图像宽、高。
	void ImageDecoder::GetDimension( UINT& nWidth, UINT& nHeight )
	{
		nWidth = mWidth;
		nHeight = mHeight;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取图像格式
	PixelFormat ImageDecoder::GetFormat()
	{
		return mPixelFormat;
	}
	//--------------------------------------------------------------------------------------------------------------
	//解析图像头
	void ImageDecoder::ParseHeader()
	{
		mFnParseHeader( this );
	}
	//--------------------------------------------------------------------------------------------------------------
	//解码图像数据
	void ImageDecoder::Decode( BYTE* pDstBuf, RECT* srcRect, RECT* dstRect, int pitch, PixelFormat ePixelFormat )
	{
		mFnDecode( this, pDstBuf, srcRect, dstRect, pitch, ePixelFormat );
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
