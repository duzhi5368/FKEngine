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
*	文件说明：	字体类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once


//包含 FreeType 2.1.9 头文件
#include <ft2build.h>
#include FT_FREETYPE_H


//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//字体类
	class FK_DLL FontFace
	{
	public:
		friend class TextSurface;

		//字形信息
		struct GlyphInfo
		{
			wchar_t		wChar;		//字形字符
			int			nWidth;		//字形宽度
			int			nHeight;	//字形高度
			int			nLeft;		//字形左边距
			int			nTop;		//字形上边距
			int			nHorAdvance;//横向字距
			int			nVerAdvance;//纵向字距

			int			nTexIndex;	//字形纹理位置索引
		};

		//字形绘制数据
		struct GlyphDraw
		{
			int			nUVIndex;	//纹理坐标索引
			int			nGlyphIndex;//字形信息索引
			int			nDrawOrder;	//字形绘制序号
		};

		//字形纹理图像信息
		struct GlyphTex
		{
			wchar_t		wChar;		//字形字符
			UV4			sUV;		//字形纹理坐标
		};

	public:
		FT_Render_Mode	mRenderMode;	//字体渲染模式

		Texture*		mpFontTexture;	//字形纹理

		int				mnMaxWidth;		//最大字符宽度
		int				mnMaxHeight;	//最大字符高度

		int				mFontAscender;	//字体上边距
		int				mLineSpace;		//行间距

	protected:
		FT_Face			mFontFace;		// FreeType 主字体对象

		int				mGlyphDataLen;	//字形数据大小
		BYTEStack		mGlyphDataStack;//字形数据堆栈
		int				mNumGlyphData;	//字形数据数量

		UINT			mnTexSize;		//字形纹理尺寸
		float			mfTexSizeInv;	//字形纹理尺寸倒数

		UINT			mHorGlyphNum;	//横向字形数量
		UINT			mVerGlyphNum;	//纵向字形数量

		GlyphTex*		mpGlyphTex;		//字形纹理图像缓存
		int*			mpDrawTime;		//字形纹理绘制次数

		UINT			mCurGlyphNum;	//当前字形数量
		UINT			mMaxGlyphNum;	//最大容纳字形数量
		UINT			mOverwritePos;	//覆写字形位置

	public:
		FontFace( LPCSTR szFontFile, UINT nSize, UINT nTexSize, UINT nGlyphWidth, UINT nGlyphHeight,
			UINT nFontIndex, FT_Render_Mode eRenderMode );
		~FontFace();

		//准备指定字形
		GlyphInfo* FKFastcall PrepareGlyph( wchar_t wChar, GlyphDraw* pGlyphDraw );

		//处理字形纹理数据
		UV4* FKFastcall ProcGlyphTex( GlyphDraw* pGlyphDraw );

		//重置字形图像数据
		void FKFastcall ResetGlyphData();

	protected:
		//将字符图像复制到纹理上
		void FKFastcall _CopyGlyphToTex( GlyphDraw* pGlyphDraw );
	};
	//--------------------------------------------------------------------------------------------------------------
	typedef Stack< FontFace* >	FontFaceList;
	//--------------------------------------------------------------------------------------------------------------
};

//==================================================================================================================
