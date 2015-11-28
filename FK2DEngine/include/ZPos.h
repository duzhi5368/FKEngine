/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	ZPos
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
namespace FK2DEngine
{
	enum ENUM_DefaultZPos
	{
		eDZP_None = 0,

		eDZP_BackGroudBegin		= 100,		// 背景层最下层
		eDZP_BackGroudEnd		= 200,		// 背景层最上层

		eDZP_TerrainBegin		= 300,		// 地形层最下层
		eDZP_TerrainEnd			= 400,		// 地形层最上层

		eDZP_TerrainBuildBegin	= 500,		// 地形建筑最下层
		eDZP_TerrainBeginEnd	= 600,		// 地形建筑最上层

		eDZP_SpritBegin			= 700,		// 地形精灵最下层
		eDZP_SpritEnd			= 800,		// 地形精灵最上层

		eDZP_SceneParticleBegin	= 900,		// 场景粒子最下层
		eDZP_SceneParticleEnd	= 1000,		// 场景粒子最上层

		eDZP_UIBegin			= 1100,		// UI最下层
		eDZP_UIEnd				= 1200,		// UI最上层

		eDZP_CursorBegin		= 1300,		// 鼠标最下层


		eDZP_Max = 0xffff,
	};
}