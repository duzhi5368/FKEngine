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
*	文件说明：	字体管理器
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//字体管理器
	class FK_DLL FontManager : public Singleton< FontManager >
	{
	public:
		friend class FontFace;

	protected:
		FT_Library		mFreeTypeLib;		// FreeType 库

		FileObject*		mpFontPackFile;		//字体打包文件指针
		FontFaceList	mFontFaceList;		//字体列表

	public:
		FontManager();
		~FontManager();

		//设置字体打包文件
		void SetFontPack( FileObject* pFile );

		//关闭字体打包文件
		void DetachFontPack();

		//创建新的字体
		FontFace* CreateFontFace( LPCSTR szFontFile, UINT nSize, UINT nTexSize = 512, UINT nGlyphWidth = 0,
			UINT nGlyphHeight = 0, UINT nFontIndex = 0, FT_Render_Mode eRenderMode = FT_RENDER_MODE_NORMAL );

		//删除字体
		void DeleteFontFace( FontFace* pFont );

		//删除所有字体
		void DeleteAllFont();
		
		//更新字体渲染数据
		void UpdateFont();

	protected:
		//载入 FreeType 字体
		FT_Face _LoadFontFace( LPCSTR szFontFile, UINT nFaceIndex );
	};
	//--------------------------------------------------------------------------------------------------------------
};

//==================================================================================================================
