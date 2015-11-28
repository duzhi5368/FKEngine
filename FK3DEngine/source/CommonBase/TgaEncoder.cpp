/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	TgaEncoder
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
	//编码 TGA 图像数据
	FK_DLL void EncodeTgaImage( FileObject* pDstFile, BYTE* pSrcBuf, int nPitch, PixelFormat ePixelFormat,
		UINT nWidth, UINT nHeight )
	{
		//计算每像素字节数
		UINT nPixelBytes = Misc::GetColorDepth( ePixelFormat );

		//设置 TGA 图像文件头数据
		TgaFileHeader sTgaHeader = { NULL };

		sTgaHeader.bCompFlag	= 2;			// 2 - 真彩色 未压缩
		sTgaHeader.wPicWidth	= (WORD)nWidth;	//图像宽度
		sTgaHeader.wPicHeight	= (WORD)nHeight;//图像高度

		//每像素位数
		sTgaHeader.bPixelBits	= (BYTE)( nPixelBytes << 3 );

		//图像描述符
		UINT nAlphaBits = Misc::GetColorAlphaBits( ePixelFormat ) & 15;

		//从左到右 从上到下
		sTgaHeader.bPicInfoBits	= nAlphaBits | 0x20;

		//写入图像头
		pDstFile->Write( &sTgaHeader, sizeof(TgaFileHeader) );

		//写入图像数据
		int nRowLen = nWidth * nPixelBytes;
		for( UINT i=0; i<nHeight; ++i )
		{
			pDstFile->Write( pSrcBuf, nRowLen );
			pSrcBuf += nPitch;
		}

		//写入文件尾
		TgaFileFooter sTgaFooter = { NULL };
		memcpy( sTgaFooter.szFooter, "TRUEVISION-TARGA.", 18 );

		pDstFile->Write( &sTgaFooter, sizeof(TgaFileFooter) );
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
