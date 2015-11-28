/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	GraphicsBase
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "Platform.h"
#include <limits>
//--------------------------------------------------------------------
namespace FK2DEngine
{
	//! 深度值，深度高的将覆盖深度值低的图元
	typedef double ZPos;

	//! 两个图元得Alpha颜色混合方式
	enum ENUM_AlphaMode
	{
		EAM_Default,			// 默认两个图元差值混合。255是不透明。
		EAM_Additive,			// 两个图元加法混合
		EAM_Mutiply,			// 两个图元乘法混合( 去除白底 )
		EAM_SrcAlphaOne,
		EAM_OneSrcAlpha,
	};

	//! 字体格式标识
	enum ENUM_FontFlags
	{
		EFF_None			= 0,
		EFF_Bold			= 1,
		EFF_Italic			= 2,
		EFF_Underline		= 4,
		EFF_Combinations	= 8
	};

	//! 字体偏移方式
	enum ENUM_TextAlign
	{
		ETA_Left,
		ETA_Right,
		ETA_Center,
		ETA_Justify
	};

	//! 裁剪方式
	enum ENUM_BorderFlags
	{
		EBF_Smooth				= 0,
		EBF_TileableLeft		= 1,			// 向左裁剪
		EBF_TileableTop			= 2,			// 向上裁剪
		EBF_TileableRight		= 4,			// 向右裁剪
		EBF_TileableBottom		= 8,			// 向下裁剪
		EBF_Tileable = EBF_TileableLeft | EBF_TileableTop | EBF_TileableRight | EBF_TileableBottom,
	};


	#ifndef SWIG
		FK_DEPRECATED const double zImmediate = -std::numeric_limits< double >::infinity();
	#endif
}