/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	RenderState
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../../include/RenderSystem/RenderSystemInsideHeader.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	RenderState::RenderState()
		: mFillMode				(FM_SOLID)				//填充模式
		, mShadeMode			(SM_GOURAUD)			//阴影模式
		, mCullMode				(CM_ANTICLOCKWISE)		//拣选类型
		, mbLighting			(true)					//是否开启灯光
		, mbSpecular			(false)					//是否开启高光
		, mbFogEnable			(false)					//是否开启雾化
		, mFogMode				(FOG_LINEAR)			//雾化模式
		, mFogColor				(0xFFFFFFFF)			//雾化颜色
		, mFogStart				(1.0f)					//雾化开始位置
		, mFogEnd				(1000.0f)				//雾化结束位置
		, mFogDensity			(1.0f)					//雾化浓度
		, mbDepthTestEnable		(true)					//是否开启深度测试
		, mbDepthWriteEnable	(true)					//是否开启深度写入
		, mbAlphaEnable			(false)					//是否开启 Alpha 混合
		, mbAlphaTestEnable		(false)					//是否开启 Alpha 测试
		, mDepthCompareFun		(CMP_LESSEQUAL)			//深度比较函数
		, mAlphaCompareFun		(CMP_GREATER)			// Alpha 比较函数
		, mdwAlphaRefValue		(0x0)					// Alpha 测试参照值
		, mSrcBlendFactor		(BF_ONE)				//来源颜色混合类型
		, mDestBlendFactor		(BF_ZERO)				//目标颜色混合类型
		, mSrcAlphaBlendFactor	(BF_ONE)				//来源 Alpha 值混合类型
		, mDestAlphaBlendFactor	(BF_ZERO)				//目标 Alpha 值混合类型
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	//操作符重载
	RenderState& RenderState::operator = ( const RenderState& refRenderState )
	{
		memcpy( this, &refRenderState, sizeof(RenderState) );
		return *this;
	}
	//--------------------------------------------------------------------------------------------------------------
	FKInline bool RenderState::operator == ( const RenderState& refRenderState ) const
	{
		return ( 0 == memcmp( this, &refRenderState, sizeof(RenderState) ) );
	}
	//--------------------------------------------------------------------------------------------------------------
	FKInline bool RenderState::operator != ( const RenderState& refRenderState ) const
	{
		return !( 0 == memcmp( this, &refRenderState, sizeof(RenderState) ) );
	}
	//--------------------------------------------------------------------------------------------------------------
	//判断是否为透明渲染状态
	bool FKFastcall RenderState::IsTransparent() const
	{
		return ( mDestBlendFactor != BF_ZERO && !mbDepthTestEnable && !mbDepthWriteEnable );
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置为不透明渲染状态
	void FKFastcall RenderState::SetOpacity()
	{
		mSrcBlendFactor		= BF_ONE;
		mDestBlendFactor	= BF_ZERO;

		mbDepthTestEnable	= true;
		mbDepthWriteEnable	= true;
		mbAlphaEnable		= false;
		mbAlphaTestEnable	= false;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置为半透明渲染状态
	void FKFastcall RenderState::SetTransparent( CompareFunction eCmpFun, DWORD dwRefAlpha )
	{
		mSrcBlendFactor		= BF_SRCALPHA;
		mDestBlendFactor	= BF_INVSRCALPHA;

		mbDepthTestEnable	= false;
		mbDepthWriteEnable	= false;
		mbAlphaEnable		= true;
		mbAlphaTestEnable	= true;

		mAlphaCompareFun	= eCmpFun;
		mdwAlphaRefValue	= dwRefAlpha;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置为透明遮罩渲染状态
	void FKFastcall RenderState::SetAlphaMask( CompareFunction eCmpFun, DWORD dwRefAlpha )
	{
		mSrcBlendFactor		= BF_SRCALPHA;
		mDestBlendFactor	= BF_INVSRCALPHA;

		mbDepthTestEnable	= true;
		mbDepthWriteEnable	= true;
		mbAlphaEnable		= true;
		mbAlphaTestEnable	= true;

		mAlphaCompareFun	= eCmpFun;
		mdwAlphaRefValue	= dwRefAlpha;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置为多层叠加渲染状态
	void FKFastcall RenderState::SetMultiLayers( CompareFunction eCmpFun, DWORD dwRefAlpha )
	{
		mSrcBlendFactor		= BF_SRCALPHA;
		mDestBlendFactor	= BF_INVSRCALPHA;

		mbAlphaEnable		= true;
		mbAlphaTestEnable	= true;

		mAlphaCompareFun	= eCmpFun;
		mdwAlphaRefValue	= dwRefAlpha;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
