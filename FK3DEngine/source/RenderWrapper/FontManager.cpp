/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	FontManager
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
	FontManager::FontManager()
	{
		mFontFaceList.Initialize( 10, 5 );

		//初始化 FreeType 库
		FT_Error error = FT_Init_FreeType( &mFreeTypeLib );
		if( error != 0 )
			Except( Exception::ERR_INTERNAL_ERROR, "初始化 FreeType 失败。" );
	}
	//--------------------------------------------------------------------------------------------------------------
	FontManager::~FontManager()
	{
		//删除所有字体
		DeleteAllFont();

		//释放 FreeType 库
		FT_Done_FreeType( mFreeTypeLib );
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置字体打包文件
	void FontManager::SetFontPack( FileObject* pFile )
	{
		mpFontPackFile = pFile;
	}
	//--------------------------------------------------------------------------------------------------------------
	//关闭字体打包文件
	void FontManager::DetachFontPack()
	{
		mpFontPackFile = NULL;
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建新的字体
	FontFace* FontManager::CreateFontFace( LPCSTR szFontFile, UINT nSize, UINT nTexSize, UINT nGlyphWidth,
		UINT nGlyphHeight, UINT nFontIndex, FT_Render_Mode eRenderMode )
	{
		FontFace* pFont = new FontFace( szFontFile, nSize, nTexSize, nGlyphWidth, nGlyphHeight, nFontIndex, eRenderMode );
		*mFontFaceList.Push() = pFont;
		return pFont;
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除字体
	void FontManager::DeleteFontFace( FontFace* pFont )
	{
		if( pFont == NULL )
			return;

		FontFaceList::Iterator it = mFontFaceList.Begin();
		FontFaceList::Iterator end = mFontFaceList.End();
		for(; it!=end; ++it )
		{
			if( *it == pFont )
			{
				mFontFaceList.Erase( it );
				delete pFont;
				return;
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除所有字体
	void FontManager::DeleteAllFont()
	{
		FontFaceList::Iterator it = mFontFaceList.Begin();
		FontFaceList::Iterator end = mFontFaceList.End();
		for(; it!=end; ++it )
			SafeDelete( *it );
	}
	//--------------------------------------------------------------------------------------------------------------
	//更新字体渲染数据
	void FontManager::UpdateFont()
	{
		FontFaceList::Iterator it = mFontFaceList.Begin();
		FontFaceList::Iterator end = mFontFaceList.End();
		for(; it!=end; ++it )
		{
			FontFace* pFont = *it;
			pFont->ResetGlyphData();
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//载入 FreeType 字体
	FT_Face FontManager::_LoadFontFace( LPCSTR szFontFile, UINT nFaceIndex )
	{
		//创建字体
		FT_Face pFontFace = NULL;
		FT_Error error = FT_New_Face( mFreeTypeLib, szFontFile, nFaceIndex, &pFontFace );
		if( error != 0 )
		{
			if( error == FT_Err_Cannot_Open_Resource )
				Except( Exception::ERR_INVALIDPARAMS, (String)"找不到指定的字体文件 '" + szFontFile + "'。" );
			else if( error == FT_Err_Unknown_File_Format )
				Except( Exception::ERR_INVALIDPARAMS, "字体文件格式错误，无法创建字体。" );
			else
				Except( Exception::ERR_INTERNAL_ERROR, "创建字体失败。" );
		}

		return pFontFace;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
