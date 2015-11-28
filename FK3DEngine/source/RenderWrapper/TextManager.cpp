/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	TextManager
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
	TextManager::TextManager()
	{
		mTextSurfaceList.Initialize( 32, 16 );
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建平面文字
	TextSurface* TextManager::CreateTextSurface( ZOrderType eZType, OverlaySurface* pZRefOverlay, FontFace* pFont,
		LPCWSTR szText, int nX, int nY, int nLineWidth, int nMaxChars, DWORD dwColor, TextureFilterType eFilterType )
	{
		////自动计算换行宽度
		if( nLineWidth == 0 )
			nLineWidth = OverlayManager::mpSingleton->mRenderWidth - nX - 10;

		TextSurface* pTextSurface = new TextSurface( eZType, pZRefOverlay, pFont, szText,
			nX, nY, nLineWidth, nMaxChars, dwColor, eFilterType );
		
		*mTextSurfaceList.Push() = pTextSurface ;

		return pTextSurface;
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除平面文字
	void TextManager::DeleteTextSurface( TextSurface* pTextSurface )
	{
		if( pTextSurface == NULL )
			return;

		Stack<TextSurface*>::Iterator it = mTextSurfaceList.Begin();
		Stack<TextSurface*>::Iterator end = mTextSurfaceList.End();
		for(; it!=end; ++it )
		{
			if( *it == pTextSurface )
			{
				mTextSurfaceList.Erase( it );
				delete pTextSurface;
				return;
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除所有平面文字
	void TextManager::DeleteAllTextSurface()
	{
		Stack<TextSurface*>::Iterator it = mTextSurfaceList.Begin();
		Stack<TextSurface*>::Iterator end = mTextSurfaceList.End();
		for(; it!=end; ++it )
		{
			if( *it != NULL )
				delete *it;
		}

		mTextSurfaceList.Clear();
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
