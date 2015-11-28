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
*	文件说明：	平面文字类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//平面文字类
	class FK_DLL TextSurface : public OverlaySurface
	{
	public:
		friend class TextManager;

	public:
		FontFace*		mpFont;			//文字字体
		wchar_t*		mpText;			//文字内容字符串
		int				mNumChars;		//文字字符数量
		int				mMaxChars;		//最大字符数量
		int				mX;				//文字显示 X 坐标
		int				mY;				//文字显示 Y 坐标（文字基线）
		int				mLineWidth;		//文字行宽
		DWORD			mTextColor;		//文字颜色

	protected:
		FontFace::GlyphDraw*	mpGlyphDraw;	//文字字形绘制数据
		OverlayVertex*			mpVertexData;	//文字顶点数据

		UINT					mNumDrawGroup;	//字形渲染分组数量
		UINT					mpDrawGroup[8];	//字形渲染分组数据

	public:
		TextSurface( ZOrderType eZType, OverlaySurface* pZRefOverlay, FontFace* pFont, LPCWSTR szText, int nX, int nY,
			int nLineWidth, int nMaxChars = 0, DWORD dwColor = 0xFFFFFFFF, TextureFilterType eFilterType = TFT_POINT );
		~TextSurface();
	

		//设置最大字符数量
		void SetMaxChars( int nMaxChars );

		//设置文字内容
		void SetText( LPCWSTR szText );

		//设置文字位置
		void SetTextPos( int nX, int nY );

		//设置文字颜色
		void SetTextColor( DWORD dwColor );

	protected:
		//更新文字数据
		void FKFastcall _UpdateText();

		//处理平面图像顶点
		void FKFastcall _ProcessVertex();

		//渲染平面图像
		void FKFastcall _RenderOverlay();
	};
	//--------------------------------------------------------------------------------------------------------------
	typedef Stack< TextSurface* >	TextSurfaceList;
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
