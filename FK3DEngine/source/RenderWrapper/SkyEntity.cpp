/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	SkyEntity
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
	SkyEntity::SkyEntity()
		: mMatWorld		(Matrix4::Identity)
	{
		//初始化天空体渲染状态
		mRenderOpacity.mbLighting				= false;
		mRenderOpacity.mbFogEnable				= false;
		mRenderOpacity.mbDepthTestEnable		= false;
		mRenderOpacity.mbDepthWriteEnable		= false;

		mRenderTransparent.mbLighting			= false;
		mRenderTransparent.mbFogEnable			= false;

		mRenderTransparent.SetTransparent();
	}
	//--------------------------------------------------------------------------------------------------------------
	SkyEntity::~SkyEntity()
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置纹理状态
	void FKFastcall SkyEntity::_SetTextureState()
	{
		//使用线性过滤
		RenderSystem::mpSingleton->_SetTextureFilter( ST_MAGFILTER, TFT_LINEAR, 0 );
		RenderSystem::mpSingleton->_SetTextureFilter( ST_MINFILTER, TFT_LINEAR, 0 );
		RenderSystem::mpSingleton->_SetTextureFilter( ST_MIPFILTER, TFT_LINEAR, 0 );

		//镜向纹理寻址以避免出现接缝
		RenderSystem::mpSingleton->_SetTextureAddress( ST_ADDRESSU, TAT_MIRROR, 0 );
		RenderSystem::mpSingleton->_SetTextureAddress( ST_ADDRESSV, TAT_MIRROR, 0 );
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
