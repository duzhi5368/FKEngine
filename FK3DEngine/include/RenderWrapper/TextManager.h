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
*	文件说明：	文字管理器
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

#include <list>

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//文字管理器
	class FK_DLL TextManager : public Singleton< TextManager >
	{
	public:
		Stack<TextSurface*> mTextSurfaceList;//平面文字列表
	public:
		TextManager();

		//创建平面文字
		TextSurface* CreateTextSurface( ZOrderType eZType, OverlaySurface* pZRefOverlay, FontFace* pFont,
			LPCWSTR szText, int nX, int nY, int nLineWidth = 0, int nMaxChars = 0, DWORD dwColor = 0xFFFFFFFF,
			TextureFilterType eFilterType = TFT_POINT );

		//删除平面文字
		void DeleteTextSurface( TextSurface* pTextSurface );

		//删除所有平面文字
		void DeleteAllTextSurface();
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
