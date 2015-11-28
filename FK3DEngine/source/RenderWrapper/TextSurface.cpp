/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	TextSurface
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
	TextSurface::TextSurface( ZOrderType eZType, OverlaySurface* pZRefOverlay, FontFace* pFont, LPCWSTR szText,
		int nX, int nY, int nLineWidth, int nMaxChars, DWORD dwColor, TextureFilterType eFilterType )
		:OverlaySurface	( eZType, pZRefOverlay, eFilterType )
		 ,mpFont			(pFont)
		, mpText			(NULL)
		, mNumChars			(0)
		, mX				(nX)
		, mY				(nY)
		, mLineWidth		(nLineWidth)
		, mTextColor		(dwColor)
	{
		if( nX + mpFont->mnMaxWidth > nLineWidth )
			Except( Exception::ERR_INVALIDPARAMS, "指定平面文字行宽度过小。" );

		//计算字符串长度
		int nNumChars = 0;
		wchar_t* pChar = (wchar_t*)szText;
		while( *pChar != 0 )
		{
			++pChar;
			++nNumChars;
		}

		//如果未指定最大字符数量则使用当前字符串字符数量
		if( nMaxChars == 0 )
			mMaxChars = nNumChars;
		else if( nMaxChars < nNumChars )
			Except( Exception::ERR_INVALIDPARAMS, "指定的最大字符数量小于当前文字字符数量。" );
		else
			mMaxChars = nMaxChars;

		mNumChars = nNumChars;

		//分配内存
		size_t nStrLen = sizeof(wchar_t) * ( mMaxChars + 1 );
		size_t nDrawLen = sizeof(FontFace::GlyphDraw) * mMaxChars;

		BYTE* pMem = (BYTE*)malloc( nStrLen + nDrawLen + sizeof(OverlayVertex)*6*mMaxChars );

		//复制字符串
		mpText = (wchar_t*)pMem;
		memcpy( mpText, szText, sizeof(wchar_t) * ( mNumChars + 1 ) );

		//分配字形数据缓存
		mpGlyphDraw = (FontFace::GlyphDraw*)( pMem + nStrLen );
		memset( mpGlyphDraw, 0, nDrawLen );

		//分配顶点数据缓存
		mpVertexData = (OverlayVertex*)( pMem + nStrLen + nDrawLen );
		mpVertexPtr = mpVertexData;
		mNumVertex = 0;

		//更新文字数据
		_UpdateText();
	}
	//--------------------------------------------------------------------------------------------------------------
	TextSurface::~TextSurface()
	{
		SafeFree( mpText );
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置最大字符数量
	void TextSurface::SetMaxChars( int nMaxChars )
	{
		free( mpText );

		//分配内存
		size_t nStrLen = sizeof(wchar_t) * ( nMaxChars + 1 );
		size_t nDrawLen = sizeof(FontFace::GlyphDraw) * nMaxChars;

		BYTE* pMem = (BYTE*)malloc( nStrLen + nDrawLen + sizeof(OverlayVertex)*6*nMaxChars );

		//分配新字符串缓存
		mMaxChars = nMaxChars;
		mNumChars = 0;

		mpText = (wchar_t*)pMem;
		memset( mpText, 0, nStrLen + nDrawLen );

		//分配字形数据缓存
		mpGlyphDraw = (FontFace::GlyphDraw*)( pMem + nStrLen );

		//分配顶点数据缓存
		mpVertexData = (OverlayVertex*)( pMem + nStrLen + nDrawLen );
		mpVertexPtr = mpVertexData;

		//更新文字数据
		_UpdateText();
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置文字内容
	void TextSurface::SetText( LPCWSTR szText )
	{
		//计算字符串长度
		int mNumChars = 0;
		wchar_t* pChar = (wchar_t*)szText;
		while( *pChar != 0 )
		{
			++pChar;
			++mNumChars;
		}

		if( mMaxChars < mNumChars )
			Except( Exception::ERR_INVALIDPARAMS, "指定的文字字符数量超过了最大字符数量。" );

		//复制字符串
		memcpy( mpText, szText, sizeof(wchar_t) * ( mNumChars + 1 ) );

		//重置字形数据缓存
		memset( mpGlyphDraw, 0, sizeof(FontFace::GlyphDraw) * mMaxChars );

		//更新文字数据
		_UpdateText();
	}
	//--------------------------------------------------------------------------------------------------------------
	//改变文字位置
	void TextSurface::SetTextPos( int nX, int nY )
	{
		//计算文字位置差值
		float fOffsetX = (float)( nX - mX );
		float fOffsetY = (float)( nY - mY );

		//计算所有字形顶点位置
		wchar_t* pChar = mpText;
		OverlayVertex* pVertex = mpVertexData;

		while( *pChar != 0 )
		{
			++pChar;

			pVertex->x += fOffsetX;	pVertex->y += fOffsetY;	++pVertex;
			pVertex->x += fOffsetX;	pVertex->y += fOffsetY;	++pVertex;
			pVertex->x += fOffsetX;	pVertex->y += fOffsetY;	++pVertex;
			pVertex->x += fOffsetX;	pVertex->y += fOffsetY;	++pVertex;
			pVertex->x += fOffsetX;	pVertex->y += fOffsetY;	++pVertex;
			pVertex->x += fOffsetX;	pVertex->y += fOffsetY;	++pVertex;
		}

		mX = nX;
		mY = nY;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置文字颜色
	void TextSurface::SetTextColor( DWORD dwColor )
	{
		//设置所有字形顶点颜色
		wchar_t* pChar = mpText;
		OverlayVertex* pVertex = mpVertexData;

		while( *pChar != 0 )
		{
			++pChar;

			pVertex->color = dwColor;	++pVertex;
			pVertex->color = dwColor;	++pVertex;
			pVertex->color = dwColor;	++pVertex;
			pVertex->color = dwColor;	++pVertex;
			pVertex->color = dwColor;	++pVertex;
			pVertex->color = dwColor;	++pVertex;
		}

		mTextColor = dwColor;
	}
	//--------------------------------------------------------------------------------------------------------------
	//更新文字数据
	void FKFastcall TextSurface::_UpdateText()
	{
		//减去顶点计数
		if( mbVisable )
			OverlayManager::mpSingleton->mNumRenderVer -= mNumVertex;

		//当前文字显示基准点
		int nPenX = mX;
		int nPenY = mY + mpFont->mFontAscender;

		//计算所有字形顶点数据
		wchar_t* pChar = mpText;
		FontFace::GlyphDraw* pGlyphDraw = mpGlyphDraw;
		OverlayVertex* pVertex = mpVertexData;
		mNumChars = 0;
		mNumVertex = 0;

		while( *pChar != 0 )
		{
			//获取字形信息
			FontFace::GlyphInfo* pGlyphInfo = mpFont->PrepareGlyph( *pChar, pGlyphDraw );

			//如果超过行宽度则自动换行
			if( nPenX + pGlyphInfo->nWidth - mX > mLineWidth )
			{
				nPenX = mX;
				nPenY += mpFont->mLineSpace;
			}

			//计算字形显示数据
			float fX1 = (float)( nPenX + pGlyphInfo->nLeft );
			float fY1 = (float)( nPenY - pGlyphInfo->nTop );
			float fX2 = fX1 + (float)pGlyphInfo->nWidth;
			float fY2 = fY1 + (float)pGlyphInfo->nHeight;

			nPenX += pGlyphInfo->nHorAdvance;

			//计算字形顶点数据
			pVertex->x = fX1;	pVertex->y = fY2;	pVertex->z = 1.0f;		//左下
			pVertex->color = mTextColor;	++pVertex;

			pVertex->x = fX1;	pVertex->y = fY1;	pVertex->z = 1.0f;		//左上
			pVertex->color = mTextColor;	++pVertex;

			pVertex->x = fX2;	pVertex->y = fY2;	pVertex->z = 1.0f;		//右下
			pVertex->color = mTextColor;	++pVertex;

			pVertex->x = fX2;	pVertex->y = fY1;	pVertex->z = 1.0f;		//右上
			pVertex->color = mTextColor;	++pVertex;

			pVertex->x = fX2;	pVertex->y = fY2;	pVertex->z = 1.0f;		//右下
			pVertex->color = mTextColor;	++pVertex;

			pVertex->x = fX1;	pVertex->y = fY1;	pVertex->z = 1.0f;		//左上
			pVertex->color = mTextColor;	++pVertex;

			++mNumChars;
			mNumVertex += 6;

			++pGlyphDraw;
			++pChar;
		}

		//加上新的顶点计数
		if( mbVisable )
			OverlayManager::mpSingleton->mNumRenderVer += mNumVertex;
	}
	//--------------------------------------------------------------------------------------------------------------
	//处理平面图像顶点
	void FKFastcall TextSurface::_ProcessVertex()
	{
		//计算文字顶点纹理坐标数据
		FontFace::GlyphDraw* pGlyphDraw = mpGlyphDraw;
		OverlayVertex* pVertex = mpVertexData;

		for( int i=0; i<mNumChars; ++i, ++pGlyphDraw )
		{
			//处理字形纹理
			UV4* pUV = mpFont->ProcGlyphTex( pGlyphDraw );
			if( pUV != NULL )
			{
				pVertex->u = pUV->u1;	pVertex->v = pUV->v2;	++pVertex;
				pVertex->u = pUV->u1;	pVertex->v = pUV->v1;	++pVertex;
				pVertex->u = pUV->u2;	pVertex->v = pUV->v2;	++pVertex;
				pVertex->u = pUV->u2;	pVertex->v = pUV->v1;	++pVertex;
				pVertex->u = pUV->u2;	pVertex->v = pUV->v2;	++pVertex;
				pVertex->u = pUV->u1;	pVertex->v = pUV->v1;	++pVertex;
			}
		}

		//根据字形的绘制顺序重新排列顶点数据
		static FontFace::GlyphDraw sTmpGlphyDraw;
		static OverlayVertex pTmpVertex[6];

		for( int i=0; i<mNumChars-1; i++ )
		{
			for( int j=i+1; j<mNumChars; j++ )
			{
				FontFace::GlyphDraw* pGlyphDrawA = mpGlyphDraw + i;
				FontFace::GlyphDraw* pGlyphDrawB = mpGlyphDraw + j;

				if( pGlyphDrawA->nDrawOrder > pGlyphDrawB->nDrawOrder )
				{
					OverlayVertex* pVertexPtrA = mpVertexData + i * 6;
					OverlayVertex* pVertexPtrB = mpVertexData + j * 6;

					memcpy( &sTmpGlphyDraw, pGlyphDrawA, sizeof(FontFace::GlyphDraw) );
					memcpy( pTmpVertex, pVertexPtrA, sizeof(OverlayVertex) * 6 );

					memcpy( pGlyphDrawA, pGlyphDrawB, sizeof(FontFace::GlyphDraw) );
					memcpy( pVertexPtrA, pVertexPtrB, sizeof(OverlayVertex) * 6 );

					memcpy( pGlyphDrawB, &sTmpGlphyDraw, sizeof(FontFace::GlyphDraw) );
					memcpy( pVertexPtrB, pTmpVertex, sizeof(OverlayVertex) * 6 );
				}
			}
		}

		//将同一渲染顺序的字形分组
		mNumDrawGroup = 0;

		pGlyphDraw = mpGlyphDraw;
		int nLastDrawOrder = pGlyphDraw->nDrawOrder;
		UINT nNumGlyphInGroup = 0;

		for( int i=0; i<mNumChars; ++i, ++pGlyphDraw, ++nNumGlyphInGroup )
		{
			if( pGlyphDraw->nDrawOrder != nLastDrawOrder )
			{
				if( mNumDrawGroup == 8 )
					goto ErrorNumDrawGroup;

				mpDrawGroup[ mNumDrawGroup ] = nNumGlyphInGroup;
				nLastDrawOrder = pGlyphDraw->nDrawOrder;
				nNumGlyphInGroup = 0;
				++mNumDrawGroup;
			}
		}

		mpDrawGroup[ mNumDrawGroup ] = nNumGlyphInGroup;
		++mNumDrawGroup;

		if( mNumDrawGroup == 8 )
			goto ErrorNumDrawGroup;

		return;

ErrorNumDrawGroup:
		Except( Exception::ERR_INTERNAL_ERROR, "文字平面字形渲染分组超过了最大数量。" );
	}
	//--------------------------------------------------------------------------------------------------------------
	//渲染平面图像
	void FKFastcall TextSurface::_RenderOverlay()
	{
		//设置纹理过滤类型
		RenderSystem::mpSingleton->_SetTextureFilter( ST_MAGFILTER, mFilterType, 0 );
		RenderSystem::mpSingleton->_SetTextureFilter( ST_MINFILTER, mFilterType, 0 );

		//设置纹理
		RenderSystem::mpSingleton->_SetTexture( mpFont->mpFontTexture, 0 );

		//更新字形纹理
		UINT nVertexOffset = mVertexOffset;
		FontFace::GlyphDraw* pGlyphDraw = mpGlyphDraw;

		//循环渲染所有字形渲染分组
		for( UINT i=0; i<mNumDrawGroup; ++i )
		{
			UINT nNumGlyphInGroup = mpDrawGroup[i];

			//遍历该渲染分组内所有字形
			for( UINT j=0; j<nNumGlyphInGroup; ++j, ++pGlyphDraw )
			{
				//如果不是首个绘制字符则将其图像复制到纹理上
				if( pGlyphDraw->nDrawOrder > 1 )
					mpFont->_CopyGlyphToTex( pGlyphDraw );
			}

			//渲染该分组内所有字形
			RenderSystem::mpSingleton->_RenderVertex( PT_TRIANGLE_LIST, nVertexOffset, nNumGlyphInGroup << 1 );
			nVertexOffset += nNumGlyphInGroup * 6;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
