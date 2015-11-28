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

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//渲染状态类
	class FK_DLL RenderState
	{
	public:
		FillMode			mFillMode;				//填充模式
		ShadeMode			mShadeMode;				//阴影模式
		CullMode			mCullMode;				//拣选类型

		bool				mbLighting;				//是否开启光照
		bool				mbSpecular;				//是否开启高光
		bool				mbFogEnable;			//是否开启雾化

		FogMode				mFogMode;				//雾化模式
		DWORD				mFogColor;				//雾化颜色
		float				mFogStart;				//雾化开始位置
		float				mFogEnd;				//雾化结束位置
		float				mFogDensity;			//雾化浓度

		bool				mbDepthTestEnable;		//是否开启深度测试
		bool				mbDepthWriteEnable;		//是否开启深度写入

		bool				mbAlphaEnable;			//是否开启 Alpha 混合
		bool				mbAlphaTestEnable;		//是否开启 Alpha 测试

		CompareFunction		mDepthCompareFun;		//深度比较函数
		CompareFunction		mAlphaCompareFun;		// Alpha 比较函数

		DWORD				mdwAlphaRefValue;		// Alpha 测试参照值

		BlendFactor			mSrcBlendFactor;		//来源颜色混合类型
		BlendFactor			mDestBlendFactor;		//目标颜色混合类型
		BlendFactor			mSrcAlphaBlendFactor;	//来源 Alpha 值混合类型
		BlendFactor			mDestAlphaBlendFactor;	//目标 Alpha 值混合类型

	public:
		RenderState();

		//操作符重载
		RenderState& operator = ( const RenderState& refRenderState );
		FKInline bool operator == ( const RenderState& refRenderState ) const;
		FKInline bool operator != ( const RenderState& refRenderState ) const;

		//判断是否为透明渲染状态
		bool FKFastcall IsTransparent() const;

		//设置为不透明渲染状态
		void FKFastcall SetOpacity();

		//设置为半透明渲染状态
		void FKFastcall SetTransparent( CompareFunction eCmpFun = CMP_GREATER, DWORD dwRefAlpha = 0x0 );

		//设置为透明遮罩渲染状态
		void FKFastcall SetAlphaMask( CompareFunction eCmpFun = CMP_GREATEREQUAL, DWORD dwRefAlpha = 0xAA );

		//设置为多层叠加渲染状态
		void FKFastcall SetMultiLayers( CompareFunction eCmpFun = CMP_GREATER, DWORD dwRefAlpha = 0x0 );
	};
	//--------------------------------------------------------------------------------------------------------------
};

//==================================================================================================================
