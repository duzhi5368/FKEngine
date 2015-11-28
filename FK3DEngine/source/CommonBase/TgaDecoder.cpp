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
	//解析 TGA 图像头
	void TgaParseHeader( ImageDecoder* pThis )
	{
		//注意： 由于 TGA 文件没有特有的文件类型标志字段，所以无法判断是否为 TGA 文件！

		//分配 TGA 解码额外信息数据内存
		TgaExtData* pExtData = (TgaExtData*)malloc( sizeof(TgaExtData) );
		if( pExtData == NULL )
			Except( Exception::ERR_NOT_ENOUGH_MEM, "分配 TGA 解码额外信息数据内存失败。" );

		pThis->mpExtData = pExtData;

		//读取 TGA 文件头数据
		TgaFileHeader* pHeaderData = &pExtData->sTgaFileHeader;

		//如果已将文件预先读入内存
		if( pThis->mbPreLoadInMem )
			pThis->mpImageData->Read( pHeaderData, sizeof(TgaFileHeader) );
		//如果未将文件预先读入内存则仅将头信息读入
		else
			pThis->mpImageFile->Read( pHeaderData, sizeof(TgaFileHeader) );

		//存储图像宽度、高度、每像素位数。
		pThis->mWidth = (UINT)pHeaderData->wPicWidth;
		pThis->mHeight = (UINT)pHeaderData->wPicHeight;

		BYTE bColorDepth = pHeaderData->bPixelBits;

		//判断 TGA 文件压缩类型
		switch ( pHeaderData->bCompFlag )
		{
		case 1:
			{
				//如果使用的是调色板则按调色板中的颜色位数为图像颜色位数
				bColorDepth = pHeaderData->bPalUnitBits;
				pExtData->bUsePal = true;

				//是否使用行程压缩
				pExtData->bUseRLE = false;
				break;
			}
		case 2:
			{
				pExtData->bUsePal = false;
				pExtData->bUseRLE = false;
				break;
			}
		case 9:
			{
				bColorDepth = pHeaderData->bPalUnitBits;
				pExtData->bUsePal = true;

				pExtData->bUseRLE = true;
				break;
			}
		case 10:
			{
				pExtData->bUsePal = false;
				pExtData->bUseRLE = true;
				break;
			}
		default:
			{
				Except( Exception::ERR_CANNOT_READ_FILE, "不支持的 TGA 压缩格式。" );
			}
		}

		//识别像素格式
		switch (bColorDepth)
		{
		case 16:	pThis->mPixelFormat = PF_R5G6B5;	break;
		case 24:	pThis->mPixelFormat = PF_R8G8B8;	break;
		case 32:	pThis->mPixelFormat = PF_A8R8G8B8;	break;
		default:
			Except(Exception::ERR_INTERNAL_ERROR, "不支持的 TGA 像素颜色位数。" );
		}

		//判断图像数据存储顺序
		BYTE bSaveOrder = pHeaderData->bPicInfoBits & 0x30;	//取第 5、6 位
		switch (bSaveOrder)
		{
		case 0x00:
			{
				pExtData->bYReversed = true;
				break;
			}
		case 0x20:
			{
				pExtData->bYReversed = false;
				break;
			}
		default:
			{
				Except( Exception::ERR_CANNOT_READ_FILE, "不支持的图像数据存储顺序。" );
			}
		}

		//计算图像数据偏移位置
		long nIDLen = (long)pHeaderData->bIDLen;
		pExtData->nPicDataOffset = 18 + nIDLen;

		//如果使用调色板则先载入它
		if( pExtData->bUsePal )
		{
			BYTE* pSrcPalData;

			//分配已转换调色板数据存储内存（分配为每像素 32 位）
			pExtData->pPalData = (BYTE*)malloc( (size_t)( pHeaderData->wPalSize * 4 ) );
			if( pExtData->pPalData == NULL )
				Except( Exception::ERR_NOT_ENOUGH_MEM, "分配 TGA 解码调色板数据内存失败。" );

			//读取调色板数据
			long nSrcPalSize = (long)( pHeaderData->wPalSize * ( pHeaderData->bPalUnitBits >> 3 ) );
			pExtData->nPicDataOffset += nSrcPalSize;

			if( pThis->mbPreLoadInMem )
			{
				//获取调色板起始位置内存指针
				pThis->mpImageData->SetPosition( nIDLen, BP_Curpos );
				pSrcPalData = pThis->mpImageData->GetPointer();
				pThis->mpImageData->SetPosition( nSrcPalSize );
			}
			else
			{
				//分配未转换调色板数据存储内存
				pSrcPalData = (BYTE*)malloc( nSrcPalSize );
				if( pSrcPalData == NULL )
					Except( Exception::ERR_NOT_ENOUGH_MEM, "分配 TGA 解码原始调色板数据内存失败。" );

				//将读取指针移动到调色板起始位置
				pThis->mpImageFile->SetPosition( nIDLen, BP_Curpos );

				//读取数据
				pThis->mpImageFile->Read( pSrcPalData, nSrcPalSize );
			}

			//转换调色板数据为 32 位色
			BYTE* pSrcPtr = pSrcPalData;
			BYTE* pPalPtr = pExtData->pPalData;

			/*---------------------------------------------------------

			2 字节表项中，两个字节分解成如下形式：
			arrrrrgg gggbbbbb

			但是，由于低位在前，高位在后的存储顺序，从文件中读出表项
			时，将先读入 gggbbbbb 而后读入 arrrrrgg ；a 表示属性位。

			3 字节表项中各字节依次对应 BLUE，GREEN，RED。

			4 字节表项中，字节 1 表示 BLUE，字节 2 表示 GREEN，
			字节 3 表示 RED，字节 4 表示属性。

			
			---------------------------------------------------------*/
			UINT nPalIndexNum = (UINT)pHeaderData->wPalSize;
			switch ( pHeaderData->bPalUnitBits )
			{
			case 16:
				{
					for( UINT i=0; i<nPalIndexNum; i++ )
					{
						*pPalPtr++ = *pSrcPtr & 0x1F;									//B
						*pPalPtr++ = ( *pSrcPtr>>5 ) | ( ( *(pSrcPtr+1) & 0x3 )<<3 );	//G
						*pPalPtr++ = ( *(pSrcPtr+1) & 0x7C)>>2;							//R
						*pPalPtr++ = 255;												//A
						pSrcPtr += 2;
					}
					break;
				}
			case 24:
				{
					for( UINT i=0; i<nPalIndexNum; i++ )
					{
						*pPalPtr++ = *pSrcPtr++;	//B
						*pPalPtr++ = *pSrcPtr++;	//G
						*pPalPtr++ = *pSrcPtr++;	//R
						*pPalPtr++ = 255;			//A
					}
					break;
				}
			case 32:
				{
					for( UINT i=0; i<nPalIndexNum; i++ )
					{
						*pPalPtr++ = *pSrcPtr++;	//B
						*pPalPtr++ = *pSrcPtr++;	//G
						*pPalPtr++ = *pSrcPtr++;	//R
						*pPalPtr++ = *pSrcPtr++;	//A
					}
					break;
				}
			default:
				if( !pThis->mbPreLoadInMem )
					FKFree( pSrcPalData );
				Except( Exception::ERR_CANNOT_READ_FILE, "不支持的 TGA 图像调色板位数。" );
			}

			if( !pThis->mbPreLoadInMem )
				FKFree( pSrcPalData );
		}
		else
		{
			pExtData->pPalData = NULL;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//完成 TGA 图像解码
	void TgaEndDecode( ImageDecoder* pThis )
	{
		TgaExtData* pExtData = (TgaExtData*)pThis->mpExtData;

		//释放调色板内存空间
		if( pExtData != NULL )
		{
			if( pExtData->bUsePal )
				SafeFree( pExtData->pPalData );

			free( pExtData );
			pExtData = NULL;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//解码 TGA 图像数据
	void TgaDecode( ImageDecoder* pThis, BYTE* pDstBuf, RECT* srcRect, RECT* dstRect,
		int pitch, PixelFormat ePixelFormat )
	{
		//附加信息记录结构指针
		TgaExtData* pExtData = (TgaExtData*)pThis->mpExtData;

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

		//计算读取来源和目标矩形交集中的所有像素所需要的信息

		UINT nByte = (UINT)pExtData->sTgaFileHeader.bPixelBits >> 3;	//每个像素的字节数
		UINT nBltPixel = srcWidth * srcHeight;							//需要读取的像素个数

		//根据行序是否倒转决定起始读写位置和跨度

		UINT nReadOffset = 0;	//开始进行图像数据读取的偏移字节数
		UINT nProcOffset = 0;	//开始进行图像解码的偏移字节数
		int nPitch = 0;			//行与行之间的跳转值

		if( pExtData->bYReversed )
		{
			//最后一行第一个元素
			nReadOffset = ( pThis->mWidth * ( pThis->mHeight-srcY-srcHeight ) + srcX ) * nByte;
			nProcOffset = ( pThis->mWidth * ( srcHeight - 1 ) ) * nByte;
			nPitch = -(int)( (pThis->mWidth+srcWidth) * nByte );	//跳转到上一行起点的跨度
			
		}
		else
		{
			nReadOffset = ( pThis->mWidth * srcY + srcX ) * nByte;	//第一行第一个元素
			nProcOffset = 0;
			nPitch = (pThis->mWidth-srcWidth) * nByte;				//跳转到下一行起点的跨度
		}

		//计算需要读取的图像解码数据总字节数
		UINT nPicDataLen = pThis->mWidth * srcHeight * nByte;

		//读取图像数据
		BYTE* pSrcPicData = NULL;

		//如果图像未使用行程压缩
		if( !pExtData->bUseRLE )
		{
			if( pThis->mbPreLoadInMem )
			{
				//存储源数据读取位置指针
				pThis->mpImageData->SetPosition( pExtData->nPicDataOffset + nReadOffset, BP_Begin );
				pSrcPicData = pThis->mpImageData->GetPointer();
			}
			else
			{
				//分配源图象数据存储内存块
				pSrcPicData = (BYTE*)malloc( nPicDataLen );
				if( pSrcPicData == NULL )
				{
					Except( Exception::ERR_NOT_ENOUGH_MEM, "分配源图象数据存储内存块失败。" );
				}

				//从文件中读取源图象数据
				pThis->mpImageFile->SetPosition( pExtData->nPicDataOffset + nReadOffset, BP_Begin );
				pThis->mpImageFile->Read( pSrcPicData, nPicDataLen );
			}
		}
		//如果图像使用行程压缩则必须要求将整个文件预先读入内存
		else
		{
			if( !pThis->mbPreLoadInMem )
			{
				Except( Exception::ERR_INTERNAL_ERROR,
					"在解码使用行程压缩的 TGA 图像时必须预先将整个文件数据读入内存。" );
			}

			pThis->mpImageData->SetPosition( pExtData->nPicDataOffset, BP_Begin );
			pSrcPicData = pThis->mpImageData->GetPointer();
		}


		BYTE* pSrcPtr = pSrcPicData;

		//分配需要传送的 32 位临时图像数据块内存
		BYTE* pBitPic = (BYTE*)malloc( nBltPixel*4 );
		if( pBitPic == NULL )
		{
			Except( Exception::ERR_NOT_ENOUGH_MEM, "解码 TGA 图像时分配 32 位临时图像数据块内存失败。" );
		}
		memset( pBitPic, 0, nBltPixel*4 );

		//传送目标内存块写入指针
		BYTE* pBuf = pBitPic;


		//下面根据不同的格式进行图像解码
		switch ( pExtData->sTgaFileHeader.bCompFlag )
		{
			//未压缩的，调色板图像
		case 1:
			{
				//读取交集中每像素的颜色索引值并将其映射为相应的颜色
				BYTE* pPal = pExtData->pPalData;

				//移动读取到解码起始位置
				pSrcPtr += nProcOffset;

				//根据调色板索引值位数采用不同读取方法
				switch (nByte)
				{
				case 1:
					{
						for( UINT row=0; row<srcHeight; row++ )
						{
							for( UINT col=0; col<srcWidth; col++ )
							{
								UINT nPalItem = (*pSrcPtr++) * 4;

								*pBuf++ = pPal[nPalItem];	//B
								*pBuf++ = pPal[++nPalItem];	//G
								*pBuf++ = pPal[++nPalItem];	//R
								*pBuf++ = pPal[++nPalItem];	//A
							}

							//如果已经完成当前行读取则跳转到上一行或下一行
							pSrcPtr += nPitch;
						}
						break;
					}
				case 2:
					{
						for( UINT row=0; row<srcHeight; row++ )
						{
							for( UINT col=0; col<srcWidth; col++ )
							{
								UINT nPalItem = (*(WORD*)pSrcPtr) * 4;
								pSrcPtr += 2;

								*pBuf++ = pPal[nPalItem];	//B
								*pBuf++ = pPal[++nPalItem];	//G
								*pBuf++ = pPal[++nPalItem];	//R
								*pBuf++ = pPal[++nPalItem];	//A
							}

							//如果已经完成当前行读取则跳转到上一行或下一行
							pSrcPtr += nPitch;
						}
						break;
					}
				case 4:
					{
						for( UINT row=0; row<srcHeight; row++ )
						{
							for( UINT col=0; col<srcWidth; col++ )
							{
								UINT nPalItem = (*(DWORD*)pSrcPtr) * 4;
								pSrcPtr += 4;

								*pBuf++ = pPal[nPalItem];	//B
								*pBuf++ = pPal[++nPalItem];	//G
								*pBuf++ = pPal[++nPalItem];	//R
								*pBuf++ = pPal[++nPalItem];	//A
							}

							//如果已经完成当前行读取则跳转到上一行或下一行
							pSrcPtr += nPitch;
						}
						break;
					}
				default:
					Except( Exception::ERR_CANNOT_READ_FILE, "错误的 TGA 图像调色板索引项位数。" );
				}

				break;
			}
			//未压缩的，位图图像
		case 2:
			{
				//移动读取到解码起始位置
				pSrcPtr += nProcOffset;

				//根据每像素位数（色深）选择不同赋值方式（统一转换为 BGRA）
				switch (nByte)
				{
					//16 位
				case 2:
					{
						for( UINT row=0; row<srcHeight; row++ )
						{
							for( UINT col=0; col<srcWidth; col++ )
							{
								*pBuf++ = ( *pSrcPtr & 0x1F ) * 8;									//B
								*pBuf++ = ( ( *pSrcPtr>>5 ) | ( ( *(pSrcPtr+1) & 0x3 )<<3 ) ) * 8;	//G
								*pBuf++ = ( *(pSrcPtr+1) & 0x7C) * 2;								//R
								*pBuf++ = 255;														//A

								pSrcPtr += 2;
							}

							//如果已经完成当前行读取则跳转到上一行或下一行
							pSrcPtr += nPitch;
						}
						break;
					}
					//24 位
				case 3:
					{
						for( UINT row=0; row<srcHeight; row++ )
						{
							for( UINT col=0; col<srcWidth; col++ )
							{
								*pBuf++ = *pSrcPtr++;	//B
								*pBuf++ = *pSrcPtr++;	//G
								*pBuf++ = *pSrcPtr++;	//R
								*pBuf++ = 255;			//A
							}

							//如果已经完成当前行读取则跳转到上一行或下一行
							pSrcPtr += nPitch;
						}
						break;
					}
					//32 位
				case 4:
					{
						for( UINT row=0; row<srcHeight; row++ )
						{
							for( UINT col=0; col<srcWidth; col++ )
							{
								*pBuf++ = *pSrcPtr++;	//B
								*pBuf++ = *pSrcPtr++;	//G
								*pBuf++ = *pSrcPtr++;	//R
								*pBuf++ = *pSrcPtr++;	//A
							}

							//如果已经完成当前行读取则跳转到上一行或下一行
							pSrcPtr += nPitch;
						}
						break;
					}
				default:
					Except( Exception::ERR_CANNOT_READ_FILE, "错误的 TGA 图像色深。" );
				}

				break;
			}

			//--------------------------------------------------------------------------------------------
			//压缩的图像数据，为性能考虑必须先读取所有的像素到一个中间缓存，然后再传送临时块中交集的部分。
			//--------------------------------------------------------------------------------------------

			//行程编码的调色板图像
		case 9:
			{
				//读取所有像素
				UINT nNumPixel = pThis->mWidth * pThis->mHeight;

				//分配图像数据内存块
				UINT nTmpBitLen = nNumPixel * nByte;
				BYTE* pTmpBitPic = (BYTE*)malloc( nTmpBitLen );
				if( pTmpBitPic == NULL )
				{
					Except( Exception::ERR_NOT_ENOUGH_MEM, "分配 TGA 图像解码行程压缩解压临时缓存失败。" );
				}
				memset( pTmpBitPic, 0, nTmpBitLen );

				//临时内存块写入指针
				BYTE* pTmpBuf = pTmpBitPic;
				BYTE* pPal = pExtData->pPalData;

				//根据根据调色板位数选择不同解码方式
				switch (nByte)
				{
				case 1:
					{
						//循环读取直到所有像素索引都读取完毕
						for( UINT i=0; i<nNumPixel;)
						{
							//识别数据包头信息类型

							//行程数据包（头信息低七位记录了下面的颜色值重复的次数）
							if( ( *pSrcPtr & 0x80 ) == 128 )
							{
								//计算重复次数并累加像素计数
								UINT repeat = ( *pSrcPtr++ & 0x7F ) + 1;
								i += repeat;

								//记录重复的像素颜色索引值
								BYTE Index = *pSrcPtr++;

								//填充重复的像素
								for( UINT c=0; c<repeat; c++ )
								{
									*pTmpBuf++ = Index;
								}
							}
							//非行程数据包（头信息低七位记录了下面包含的颜色值数量）
							else
							{
								//计算下面包含的像素数并累加像素计数
								UINT nPixel = ( *pSrcPtr++ & 0x7F ) + 1;
								i += nPixel;

								//填充该包包含的像素
								for( UINT c=0; c<nPixel; c++ )
								{
									*pTmpBuf++ = *pSrcPtr++;
								}
							}
						}

						//将解码完毕的索引数据中需要的交集部分传送到 pBitPic 内存块中（颜色值映射）

						//设置起始读取位置
						pTmpBuf = pTmpBitPic + nReadOffset + nProcOffset;

						for( UINT row=0; row<srcHeight; row++ )
						{
							for( UINT col=0; col<srcWidth; col++ )
							{
								UINT nPalItem = (*pTmpBuf++) * 4;

								*pBuf++ = pPal[nPalItem];	//B
								*pBuf++ = pPal[++nPalItem];	//G
								*pBuf++ = pPal[++nPalItem];	//R
								*pBuf++ = pPal[++nPalItem];	//A
							}

							//如果已经完成当前行读取则跳转到上一行或下一行
							pTmpBuf += nPitch;
						}

						break;
					}
				case 2:
					{
						//循环读取直到所有像素都读取完毕
						for( UINT i=0; i<nNumPixel;)
						{
							//识别数据包头信息类型

							//行程数据包（头信息低七位记录了下面的颜色值重复的次数）
							if( ( *pSrcPtr & 0x80 ) == 128 )
							{
								//计算重复次数并累加像素计数
								UINT repeat = ( *pSrcPtr++ & 0x7F ) + 1;
								i += repeat;

								//记录重复的像素颜色索引值
								BYTE HiBits = *pSrcPtr++;
								BYTE LoBits = *pSrcPtr++;

								//填充重复的像素
								for( UINT c=0; c<repeat; c++ )
								{
									*pTmpBuf++ = HiBits;
									*pTmpBuf++ = LoBits;
								}
							}
							//非行程数据包（头信息低七位记录了下面包含的颜色值数量）
							else
							{
								//计算下面包含的像素数并累加像素计数
								UINT nPixel = ( *pSrcPtr++ & 0x7F ) + 1;
								i += nPixel;

								//填充该包包含的像素
								for( UINT c=0; c<nPixel; c++ )
								{
									*pTmpBuf++ = *pSrcPtr++;	//B
									*pTmpBuf++ = *pSrcPtr++;	//G
								}
							}
						}

						//将解码完毕的索引数据中需要的交集部分传送到 pBitPic 内存块中（颜色值映射）

						//设置起始读取位置
						pTmpBuf = pTmpBitPic + nReadOffset + nProcOffset;

						for( UINT row=0; row<srcHeight; row++ )
						{
							for( UINT col=0; col<srcWidth; col++ )
							{
								UINT nPalItem = (*(WORD*)pTmpBuf) * 4;
								pTmpBuf += 2;

								*pBuf++ = pPal[nPalItem];	//B
								*pBuf++ = pPal[++nPalItem];	//G
								*pBuf++ = pPal[++nPalItem];	//R
								*pBuf++ = pPal[++nPalItem];	//A
							}

							//如果已经完成当前行读取则跳转到上一行或下一行
							pTmpBuf += nPitch;
						}

						break;
					}
					//32 位
				case 4:
					{
						//循环读取直到所有像素都读取完毕
						for( UINT i=0; i<nNumPixel;)
						{
							//行程数据包（头信息低七位记录了下面的颜色值重复的次数）
							if( ( *pSrcPtr & 0x80 ) == 128 )
							{
								//计算重复次数并累加像素计数
								UINT repeat = ( *pSrcPtr++ & 0x7F ) + 1;
								i += repeat;

								//记录重复的像素颜色索引值
								BYTE Index1 = *pSrcPtr++;
								BYTE Index2 = *pSrcPtr++;
								BYTE Index3 = *pSrcPtr++;
								BYTE Index4 = *pSrcPtr++;

								//填充重复的像素
								for( UINT c=0; c<repeat; c++ )
								{
									*pTmpBuf++ = Index1;
									*pTmpBuf++ = Index2;
									*pTmpBuf++ = Index3;
									*pTmpBuf++ = Index4;
								}
							}
							//非行程数据包（头信息低七位记录了下面包含的颜色值数量）
							else
							{
								//计算下面包含的像素数并累加像素计数
								UINT nPixel = ( *pSrcPtr++ & 0x7F ) + 1;
								i += nPixel;

								//填充该包包含的像素
								for( UINT c=0; c<nPixel; c++ )
								{
									*pTmpBuf++ = *pSrcPtr++;	//B
									*pTmpBuf++ = *pSrcPtr++;	//G
									*pTmpBuf++ = *pSrcPtr++;	//R
									*pTmpBuf++ = *pSrcPtr++;	//A
								}
							}
						}

						//将解码完毕的索引数据中需要的交集部分传送到 pBitPic 内存块中（颜色值映射）

						//设置起始读取位置
						pTmpBuf = pTmpBitPic + nReadOffset + nProcOffset;

						for( UINT row=0; row<srcHeight; row++ )
						{
							for( UINT col=0; col<srcWidth; col++ )
							{
								UINT nPalItem = (*(DWORD*)pTmpBuf) * 4;
								pTmpBuf += 4;

								*pBuf++ = pPal[nPalItem];	//B
								*pBuf++ = pPal[++nPalItem];	//G
								*pBuf++ = pPal[++nPalItem];	//R
								*pBuf++ = pPal[++nPalItem];	//A
							}

							//如果已经完成当前行读取则跳转到上一行或下一行
							pTmpBuf += nPitch;
						}

						break;
					}
				default:
					Except( Exception::ERR_CANNOT_READ_FILE, "错误的 TGA 图像调色板索引项位数。" );
				}

				FKFree( pTmpBitPic );
				break;
			}
			//行程编码的位图图像
		case 10:
			{
				//读取所有像素
				UINT nNumPixel = pThis->mWidth * pThis->mHeight;

				//分配图像数据内存块
				UINT nTmpBitLen = nNumPixel * nByte;
				BYTE* pTmpBitPic = (BYTE*)malloc( nTmpBitLen );
				if( pTmpBitPic == NULL )
				{
					Except( Exception::ERR_NOT_ENOUGH_MEM, "分配 TGA 图像解码行程压缩解压临时缓存失败。" );
				}
				memset( pTmpBitPic, 0, nTmpBitLen );

				//临时内存块写入指针
				BYTE* pTmpBuf = pTmpBitPic;

				//根据每像素位数（色深）选择不同赋值方式
				switch (nByte)
				{
					//16 位
				case 2:
					{
						//循环读取直到所有像素都读取完毕
						for( UINT i=0; i<nNumPixel;)
						{
							//识别数据包头信息类型

							//行程数据包（头信息低七位记录了下面的颜色值重复的次数）
							if( ( *pSrcPtr & 0x80 ) == 128 )
							{
								//计算重复次数并累加像素计数
								UINT repeat = ( *pSrcPtr++ & 0x7F ) + 1;
								i += repeat;

								//记录重复的像素颜色值
								BYTE HiBits = *pSrcPtr++;
								BYTE LoBits = *pSrcPtr++;

								//填充重复的像素
								for( UINT c=0; c<repeat; c++ )
								{
									*pTmpBuf++ = HiBits;
									*pTmpBuf++ = LoBits;
								}
							}
							//非行程数据包（头信息低七位记录了下面包含的颜色值数量）
							else
							{
								//计算下面包含的像素数并累加像素计数
								UINT nPixel = ( *pSrcPtr++ & 0x7F ) + 1;
								i += nPixel;

								//填充该包包含的像素
								for( UINT c=0; c<nPixel; c++ )
								{
									*pTmpBuf++ = *pSrcPtr++;
									*pTmpBuf++ = *pSrcPtr++;
								}
							}
						}

						//将解码完毕的位图数据中需要的交集部分传送到 pBitPic 内存块中（并同时进行 32 位转换）

						//设置起始读取位置
						pTmpBuf = pTmpBitPic + nReadOffset + nProcOffset;

						for( UINT row=0; row<srcHeight; row++ )
						{
							for( UINT col=0; col<srcWidth; col++ )
							{
								*pBuf++ = ( *pTmpBuf & 0x1F ) * 8;									//B
								*pBuf++ = ( ( *pTmpBuf>>5 ) | ( ( *(pTmpBuf+1) & 0x3 )<<3 ) ) * 8;	//G
								*pBuf++ = ( ( *(pTmpBuf+1) & 0x7C)>>2 ) * 8;						//R
								*pBuf++ = 255;														//A
								pTmpBuf += 2;
							}

							//如果已经完成当前行读取则跳转到上一行或下一行
							pTmpBuf += nPitch;
						}

						break;
					}
					//24 位
				case 3:
					{
						//循环读取直到所有像素都读取完毕
						for( UINT i=0; i<nNumPixel;)
						{
							//识别数据包头信息类型

							//行程数据包（头信息低七位记录了下面的颜色值重复的次数）
							if( ( *pSrcPtr & 0x80 ) == 128 )
							{
								//计算重复次数并累加像素计数
								UINT repeat = ( *pSrcPtr++ & 0x7F ) + 1;
								i += repeat;

								//记录重复的像素颜色值
								BYTE b = *pSrcPtr++;		//B
								BYTE g = *pSrcPtr++;		//G
								BYTE r = *pSrcPtr++;		//R

								//填充重复的像素
								for( UINT c=0; c<repeat; c++ )
								{
									*pTmpBuf++ = b;
									*pTmpBuf++ = g;
									*pTmpBuf++ = r;
								}
							}
							//非行程数据包（头信息低七位记录了下面包含的颜色值数量）
							else
							{
								//计算下面包含的像素数并累加像素计数
								UINT nPixel = ( *pSrcPtr++ & 0x7F ) + 1;
								i += nPixel;

								//填充该包包含的像素
								for( UINT c=0; c<nPixel; c++ )
								{
									*pTmpBuf++ = *pSrcPtr++;	//B
									*pTmpBuf++ = *pSrcPtr++;	//G
									*pTmpBuf++ = *pSrcPtr++;	//R
								}
							}
						}

						//将解码完毕的位图数据中需要的交集部分传送到 pBitPic 内存块中（并同时进行 32 位转换）

						//设置起始读取位置
						pTmpBuf = pTmpBitPic + nReadOffset + nProcOffset;

						for( UINT row=0; row<srcHeight; row++ )
						{
							for( UINT col=0; col<srcWidth; col++ )
							{
								*pBuf++ = *pTmpBuf++;	//B
								*pBuf++ = *pTmpBuf++;	//G
								*pBuf++ = *pTmpBuf++;	//R
								*pBuf++ = 255;			//A
							}

							//如果已经完成当前行读取则跳转到上一行或下一行
							pTmpBuf += nPitch;
						}

						break;
					}
					//32 位
				case 4:
					{
						//循环读取直到所有像素都读取完毕
						for( UINT i=0; i<nNumPixel;)
						{
							//行程数据包（头信息低七位记录了下面的颜色值重复的次数）
							if( ( *pSrcPtr & 0x80 ) == 128 )
							{
								//计算重复次数并累加像素计数
								UINT repeat = ( *pSrcPtr++ & 0x7F ) + 1;
								i += repeat;

								//记录重复的像素颜色值
								BYTE b = *pSrcPtr++;		//B
								BYTE g = *pSrcPtr++;		//G
								BYTE r = *pSrcPtr++;		//R
								BYTE a = *pSrcPtr++;		//A

								//填充重复的像素
								for( UINT c=0; c<repeat; c++ )
								{
									*pTmpBuf++ = b;
									*pTmpBuf++ = g;
									*pTmpBuf++ = r;
									*pTmpBuf++ = a;
								}
							}
							//非行程数据包（头信息低七位记录了下面包含的颜色值数量）
							else
							{
								//计算下面包含的像素数并累加像素计数
								UINT nPixel = ( *pSrcPtr++ & 0x7F ) + 1;
								i += nPixel;

								//填充该包包含的像素
								for( UINT c=0; c<nPixel; c++ )
								{
									*pTmpBuf++ = *pSrcPtr++;	//B
									*pTmpBuf++ = *pSrcPtr++;	//G
									*pTmpBuf++ = *pSrcPtr++;	//R
									*pTmpBuf++ = *pSrcPtr++;	//A
								}
							}
						}

						//将解码完毕的位图数据中需要的交集部分传送到 pBitPic 内存块中（并同时进行 32 位转换）

						//设置起始读取位置
						pTmpBuf = pTmpBitPic + nReadOffset + nProcOffset;

						for( UINT row=0; row<srcHeight; row++ )
						{
							for( UINT col=0; col<srcWidth; col++ )
							{
								*pBuf++ = *pTmpBuf++;	//B
								*pBuf++ = *pTmpBuf++;	//G
								*pBuf++ = *pTmpBuf++;	//R
								*pBuf++ = *pTmpBuf++;	//A
							}

							//如果已经完成当前行读取则跳转到上一行或下一行
							pTmpBuf += nPitch;
						}

						break;
					}
				default:
					Except( Exception::ERR_CANNOT_READ_FILE, "错误的 TGA 图像色深。" );
				}

				FKFree( pTmpBitPic );
				break;
			}

			//其它不支持的类型
		default:
			Except( Exception::ERR_CANNOT_READ_FILE, "不支持的 TGA 压缩格式。" );
		}

		//解码完成，将已经解为 32 位位图模式的图像传送到目标缓存

		//将所有需要的图像数据读取完毕后调用相应的函数进行颜色模式的转换和传送

		pBuf = pBitPic;				//读取指针
		BYTE*	pDst = pDstBuf;		//填充指针
		UINT	nBegOffset = pitch/4 * dstY + dstX;	//填充起始偏移像素数

		switch (ePixelFormat)
		{
		case PF_A8R8G8B8:
		case PF_X8R8G8B8:
			{
				pDst += nBegOffset * 4;
				BYTE* pColBeg = pDst;

				//循环填充所有像素
				for( UINT row=0; row<srcHeight; row++ )
				{
					for( UINT col=0; col<srcWidth; col++ )
					{
						*pDst++ = *pBuf++;	//读取 B
						*pDst++ = *pBuf++;	//读取 G
						*pDst++ = *pBuf++;	//读取 R
						*pDst++ = *pBuf++;	//读取 A
					}

					//如果已经完成当前行传送则跳转行
					pColBeg += pitch;
					pDst = pColBeg;
				}
				break;
			}
		case PF_R5G6B5:
			{
				pDst += nBegOffset * 2;
				BYTE* pColBeg = pDst;

				//循环填充所有像素
				for( UINT row=0; row<srcHeight; row++ )
				{
					for( UINT col=0; col<srcWidth; col++ )
					{
						BYTE b = (*pBuf++) >> 3;	//读取 B
						BYTE g = (*pBuf++) >> 2;	//读取 G
						BYTE r = (*pBuf++) >> 3;	//读取 R
						pBuf++;						//略过 A

						*(WORD*)pDst = b | (g<<5) | (r<<11);
						pDst += 2;
					}

					//如果已经完成当前行传送则跳转行
					pColBeg += pitch;
					pDst = pColBeg;
				}
				break;
			}
		case PF_A1R5G5B5:
			{
				pDst += nBegOffset * 2;
				BYTE* pColBeg = pDst;

				//循环填充所有像素
				for( UINT row=0; row<srcHeight; row++ )
				{
					for( UINT col=0; col<srcWidth; col++ )
					{
						BYTE b = (*pBuf++) >> 3;	//读取 B
						BYTE g = (*pBuf++) >> 3;	//读取 G
						BYTE r = (*pBuf++) >> 3;	//读取 R
						BYTE a = (*pBuf++) >> 7;	//读取 A

						*(WORD*)pDst = b | (g<<5) | (r<<10) | (a<<11);
						pDst += 2;
					}

					//如果已经完成当前行传送则跳转行
					pColBeg += pitch;
					pDst = pColBeg;
				}
				break;
			}
		case PF_A4R4G4B4:
			{
				pDst += nBegOffset * 2;
				BYTE* pColBeg = pDst;

				//循环填充所有像素
				for( UINT row=0; row<srcHeight; row++ )
				{
					for( UINT col=0; col<srcWidth; col++ )
					{
						BYTE b = (*pBuf++) >> 4;	//读取 B
						BYTE g = (*pBuf++) >> 4;	//读取 G
						BYTE r = (*pBuf++) >> 4;	//读取 R
						BYTE a = (*pBuf++) >> 4;	//读取 A

						*(WORD*)pDst = b | (g<<4) | (r<<8) | (a<<12);
						pDst += 2;
					}

					//如果已经完成当前行传送则跳转行
					pColBeg += pitch;
					pDst = pColBeg;
				}
				break;
			}
		default:
			FKFree( pBitPic );
			Except( Exception::ERR_INVALIDPARAMS, "错误的 TGA 图像解码输出像素格式。" );
		}

		//释放解码后的像素级临时存储空间
		FKFree( pBitPic );
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
