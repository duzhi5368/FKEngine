/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Light
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
	Light::Light()
		: mLightType		(LT_DIRECTIONAL)
		, mRange			(1000.0f)
		, mAttenConst		(1.0f)
		, mAttenLinear		(0.0f)
		, mAttenQuad		(0.0f)
		, mFallOff			(1.0f)
		, mTheta			( FK_DegreeToRadian( 20.0f ) )
		, mPhi				( FK_DegreeToRadian( 45.0f ) )
		, mIndex			( 0.0f )
		, mbActived			(false)
		, mbEnable			(false)
		, mpAttachedNode	(NULL)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	Light::~Light()
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	//将该灯光从它所连接的节点上断开
	void Light::DetachFromNode()
	{
		mpAttachedNode->DetachLight( this );
	}
	//--------------------------------------------------------------------------------------------------------------
	//激活灯光
	void Light::ActiveLight( const bool bActive )
	{
		if( bActive == mbActived )
			Except( Exception::ERR_INVALIDPARAMS, "无法重复激活或反激活灯光。" );

		if( !RenderSystem::mpSingleton->_ActiveLight( this, bActive ) )
			Except( Exception::ERR_INVALIDPARAMS, "已经达到硬件支持的最大灯光数量，激活失败。" );
	}
	//--------------------------------------------------------------------------------------------------------------
	//开启/关闭灯光
	void Light::EnableLight( const bool bEnable )
	{
		if( !mbActived )
			Except( Exception::ERR_INVALIDPARAMS, "无法开启未设置的灯光。" );

		mbEnable = bEnable;

		RenderSystem::mpSingleton->_SetLightState( this );
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
