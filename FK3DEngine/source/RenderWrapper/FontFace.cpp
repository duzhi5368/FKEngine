/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	FontFace
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../../include/RenderWrapper/RenderWrapperInsideHeader.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	FontFace::FontFace( LPCSTR szFontFile, UINT nSize, UINT nTexSize, UINT nGlyphWidth, UINT nGlyphHeight,
		UINT nFontIndex, FT_Render_Mode eRenderMode )
		: mRenderMode		(eRenderMode)
		, mpFontTexture		(NULL)
		, mFontFace			(NULL)
		, mNumGlyphData		(0)
		, mCurGlyphNum		(0)
		, mOverwritePos		(0)
	{
		//检查字体大小
		if( nSize == 0 )
			Except( Exception::ERR_INVALIDPARAMS, "不允许创建字体大小为零。" );

		//创建字体
		mFontFace = FontManager::mpSingleton->_LoadFontFace( szFontFile, nFontIndex );

		//设置字体大小
		FT_Error error = FT_Set_Pixel_Sizes( mFontFace, 0, nSize );
		if( error != 0 )
			Except( Exception::ERR_INTERNAL_ERROR, "设置字体大小失败。" );

		//计算该大小字形尺寸
		if( mFontFace->units_per_EM == 0 )
			Except( Exception::ERR_INVALIDPARAMS, "不支持的字体格式。" );

		if( nGlyphWidth == 0 )
		{
			float BoxW = (float)( ( mFontFace->bbox.xMax - mFontFace->bbox.xMin ) * nSize ) / (float)mFontFace->units_per_EM;
			mnMaxWidth = CeilToInt( BoxW );
		}
		else
			mnMaxWidth = nGlyphWidth;

		if( nGlyphHeight == 0 )
		{
			float BoxH = (float)( ( mFontFace->bbox.yMax - mFontFace->bbox.yMin ) * nSize ) / (float)mFontFace->units_per_EM;
			mnMaxHeight	= CeilToInt( BoxH );
		}
		else
			mnMaxHeight = nGlyphHeight;

		mFontAscender = CeilToInt( (float)( mFontFace->ascender * nSize ) / (float)mFontFace->units_per_EM );
		mLineSpace = CeilToInt( (float)( mFontFace->height * nSize ) / (float)mFontFace->units_per_EM );

		//设置字体编码
		if( 0 != FT_Select_Charmap( mFontFace, FT_ENCODING_UNICODE ) )
			Except( Exception::ERR_INTERNAL_ERROR, "设置字体编码失败。" );

		//计算字形纹理数据
		if( (int)nTexSize < mnMaxWidth || (int)nTexSize < mnMaxHeight )
			Except( Exception::ERR_INVALIDPARAMS, "指定的字形纹理尺寸小于字形尺寸。" );

		mnTexSize = nTexSize;
		mfTexSizeInv = 1.0f / (float)nTexSize;

		mHorGlyphNum = nTexSize / mnMaxWidth;
		mVerGlyphNum = nTexSize / mnMaxHeight;
		mMaxGlyphNum = mHorGlyphNum * mVerGlyphNum;

		//分配字形纹理图像缓存
		int nBufSize = mMaxGlyphNum * ( sizeof(GlyphTex) + sizeof(int) );
		mpGlyphTex = (GlyphTex*)malloc( nBufSize );
		memset( mpGlyphTex, 0, nBufSize );

		mpDrawTime = (int*)( mpGlyphTex + mMaxGlyphNum );

		//初始化字形数据缓存
		mGlyphDataLen = mnMaxWidth * mnMaxHeight * sizeof(DWORD) + sizeof(GlyphInfo);
		mGlyphDataStack.Initialize( 256 * mGlyphDataLen, 128 * mGlyphDataLen );

		//创建字形纹理
		mpFontTexture = TextureManager::mpSingleton->CreateTexture( nTexSize, nTexSize, PF_A8R8G8B8, TU_DYNAMIC, 1 );
		mpFontTexture->FillColor();
	}
	//--------------------------------------------------------------------------------------------------------------
	FontFace::~FontFace()
	{
		//释放字体
		if( mFontFace != NULL )
			FT_Done_Face( mFontFace );

		SafeFree( mpGlyphTex );

		TextureManager::mpSingleton->DeleteTexture( mpFontTexture );
	}
	//--------------------------------------------------------------------------------------------------------------
	//准备指定字形
	FontFace::GlyphInfo* FKFastcall FontFace::PrepareGlyph( wchar_t wChar, GlyphDraw* pGlyphDraw )
	{
		pGlyphDraw->nUVIndex = -1;

		BYTE* pGlyphData = mGlyphDataStack.Begin();

		//查找指定字形是否已存在
		for( int i=0; i<mNumGlyphData; ++i, pGlyphData+=mGlyphDataLen )
		{
			GlyphInfo* pGlyphInfo = (GlyphInfo*)pGlyphData;

			//如果指定字形已存在则返回它
			if( pGlyphInfo->wChar == wChar )
			{
				pGlyphDraw->nGlyphIndex = i;
				return pGlyphInfo;
			}
		}

		//如果字形尚未创建则创建它
		GlyphInfo* pGlyphInfo = (GlyphInfo*)mGlyphDataStack.Push( mGlyphDataLen );

		pGlyphInfo->wChar = wChar;
		pGlyphDraw->nGlyphIndex = mNumGlyphData;
		pGlyphInfo->nTexIndex = -1;

		++mNumGlyphData;

		DWORD* pImageBuf = (DWORD*)( pGlyphInfo + 1 );

		//载入该字形
		if( 0 != FT_Load_Char( mFontFace, wChar, FT_LOAD_DEFAULT ) )
			Except( Exception::ERR_INTERNAL_ERROR, "选择字符字形失败。" );

		FT_GlyphSlot pGlyph = mFontFace->glyph;
		FT_Bitmap* pBitmap = &pGlyph->bitmap;

		//如果该字形不是字体内嵌位图则进行渲染
		if( pGlyph->format != FT_GLYPH_FORMAT_BITMAP )
		{
			if( 0 != FT_Render_Glyph( pGlyph, mRenderMode ) )
				Except( Exception::ERR_INTERNAL_ERROR, "渲染字形位	图失败。" );
		}

		//存储字形排版数据
		pGlyphInfo->nLeft		= pGlyph->bitmap_left;
		pGlyphInfo->nTop		= pGlyph->bitmap_top;
		pGlyphInfo->nWidth		= mnMaxWidth;
		pGlyphInfo->nHeight		= mnMaxHeight;
		pGlyphInfo->nHorAdvance	= pGlyph->metrics.horiAdvance >> 6;
		pGlyphInfo->nVerAdvance = pGlyph->metrics.vertAdvance >> 6;

		//复制字形图像到缓存
		memset( pImageBuf, 0, sizeof(DWORD) * mnMaxWidth * mnMaxHeight );
		DWORD* pDstBuf = pImageBuf;
		DWORD* pDst = pDstBuf;

		//如果像素模式为单色（一位代表一像素）
		if( pBitmap->pixel_mode == FT_PIXEL_MODE_MONO )
		{
			BYTE* pSrc = (BYTE*)pBitmap->buffer;
			BYTE* pSrcRow = pSrc;

			for( int y=0; y<pBitmap->rows; ++y )
			{
				BYTE nBitMask = 128;
				for( int x=0; x<pBitmap->width; ++x )
				{
					*pDst = ( *pSrc & nBitMask ) ? 0xFFFFFFFF : 0x0;
					++pDst;

					nBitMask >>= 1;
					if( nBitMask == 0 )
					{
						nBitMask = 128;
						++pSrc;
					}
				}

				pSrcRow += pBitmap->pitch;
				pSrc = pSrcRow;
				pDstBuf += pGlyphInfo->nWidth;
				pDst = pDstBuf;
			}
		}
		//如果像素模式为 256 位灰度（一字节代表一像素）
		else
		{
			BYTE* pSrc = (BYTE*)pBitmap->buffer;
			BYTE* pSrcRow = pSrc;

			for( int y=0; y<pBitmap->rows; ++y )
			{
				for( int x=0; x<pBitmap->width; ++x )
				{
					DWORD dwGray = *pSrc;
					*pDst = dwGray | (dwGray<<8) | (dwGray<<16) | (dwGray<<24);

					++pDst;
					++pSrc;
				}

				pSrcRow += pBitmap->pitch;
				pSrc = pSrcRow;
				pDstBuf += pGlyphInfo->nWidth;
				pDst = pDstBuf;
			}
		}

		//返回字形信息
		return pGlyphInfo;
	}
	//--------------------------------------------------------------------------------------------------------------
	//处理字形纹理数据
	UV4* FKFastcall FontFace::ProcGlyphTex( GlyphDraw* pGlyphDraw )
	{
		//获取字形信息
		GlyphInfo* pGlyphInfo = (GlyphInfo*)( mGlyphDataStack.Begin() + pGlyphDraw->nGlyphIndex * mGlyphDataLen );

		GlyphTex* pGlyphTex;
		int nTexIndex = pGlyphInfo->nTexIndex;

		//如果指定了原有字形纹理编号
		if( nTexIndex != -1 )
		{
			pGlyphTex = mpGlyphTex + nTexIndex;

			//如果原有位置字符可用则无须更新纹理坐标
			if( pGlyphTex->wChar == pGlyphInfo->wChar )
			{
				//如果该字形纹理在本帧渲染还未使用
				int* pDrawTime = mpDrawTime + nTexIndex;
				if( *pDrawTime == 0 )
					*pDrawTime += 1;

				pGlyphDraw->nDrawOrder = *pDrawTime;

				//如果已建立顶点 UV 数据则直接返回
				if( pGlyphDraw->nUVIndex == nTexIndex )
					return NULL;

				//否则返回纹理坐标数据
				pGlyphDraw->nUVIndex = nTexIndex;
				return &pGlyphTex->sUV;
			}
		}

		//指定新的字形纹理位置

		//如果纹理位置已满
		if( mCurGlyphNum == mMaxGlyphNum )
		{
			nTexIndex = mOverwritePos;
			pGlyphTex = mpGlyphTex + nTexIndex;

			if( mpDrawTime[ nTexIndex ] > 0 )
			{
				++mOverwritePos;

				if( mOverwritePos == mMaxGlyphNum )
					mOverwritePos = 0;
			}
		}
		else
		{
			nTexIndex = mCurGlyphNum;
			pGlyphTex = mpGlyphTex + nTexIndex;

			++mCurGlyphNum;
		}

		pGlyphTex->wChar = pGlyphInfo->wChar;
		pGlyphInfo->nTexIndex = nTexIndex;
		pGlyphDraw->nUVIndex = nTexIndex;

		//计算字形在纹理上的位置
		UV4* pUV = &pGlyphTex->sUV;
		pUV->u1 = (float)( ( nTexIndex % mHorGlyphNum ) * mnMaxWidth ) * mfTexSizeInv;
		pUV->v1 = (float)( ( nTexIndex / mHorGlyphNum ) * mnMaxHeight ) * mfTexSizeInv;
		pUV->u2 = pUV->u1 + (float)mnMaxWidth * mfTexSizeInv;
		pUV->v2 = pUV->v1 + (float)mnMaxHeight * mfTexSizeInv;

		//如果是首个绘制字符则将其图像复制到纹理上
		int* pDrawTime = mpDrawTime + nTexIndex;
		if( *pDrawTime == 0 )
			_CopyGlyphToTex( pGlyphDraw );

		*pDrawTime += 1;
		pGlyphDraw->nDrawOrder = *pDrawTime;

		//需要更新顶点 UV 数据
		return &pGlyphTex->sUV;
	}
	//--------------------------------------------------------------------------------------------------------------
	//重置字形图像数据
	void FKFastcall FontFace::ResetGlyphData()
	{
		memset( mpDrawTime, 0, sizeof(int) * mMaxGlyphNum );

		mOverwritePos = 0;
	}
	//--------------------------------------------------------------------------------------------------------------
	//将字符图像复制到纹理上
	void FKFastcall FontFace::_CopyGlyphToTex( GlyphDraw* pGlyphDraw )
	{
		GlyphInfo* pGlyphInfo = (GlyphInfo*)( mGlyphDataStack.Begin() + pGlyphDraw->nGlyphIndex * mGlyphDataLen );

		//如果没有任何图像则直接返回
		if( pGlyphInfo->nWidth == 0 || pGlyphInfo->nHeight == 0 )
			return;

		DWORD* pImageBuf = (DWORD*)( pGlyphInfo + 1 );
		int nTexIndex = pGlyphDraw->nUVIndex;

		//计算字形图像复制位置
		RECT LockRect;
		LockRect.left = ( nTexIndex % mHorGlyphNum ) * mnMaxWidth;
		LockRect.top = nTexIndex / mHorGlyphNum * mnMaxHeight;
		LockRect.right = LockRect.left + pGlyphInfo->nWidth;
		LockRect.bottom = LockRect.top + pGlyphInfo->nHeight;

		//锁定纹理
		BYTE* pTexBuf;
		UINT nTexPitch;
		int nCopyLen = pGlyphInfo->nWidth * sizeof(DWORD);
		mpFontTexture->LockRect( (void**)&pTexBuf, &nTexPitch, &LockRect );

		for( int y=0; y<pGlyphInfo->nHeight; ++y )
		{
			memcpy( pTexBuf, pImageBuf, nCopyLen );
			pTexBuf += nTexPitch;
			pImageBuf += pGlyphInfo->nWidth;
		}

		//解锁纹理
		mpFontTexture->UnlockRect();
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
