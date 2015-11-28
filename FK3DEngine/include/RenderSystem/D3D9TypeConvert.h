/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	D3D9TypeConvert
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	D3D9 API 数据类型转换类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	// D3D9 API 数据类型转换类
	class FK_DLL D3D9TypeConvert
	{
	public:
		//像素格式
		static PixelFormat GetPixelFormat( DWORD fmt );

		//索引数据类型
		static IndexType GetIndexType( DWORD it );

		//纹理用途
		static TextureUsage GetTextureUsage( DWORD tu );

		//阴影模式
		static ShadeMode GetShadeMode( DWORD sm );

		//灯光类型
		static LightType GetLightType( DWORD lt );

		//雾模式
		static FogMode GetFogMode( DWORD fm );

		//填充模式
		static FillMode GetFillMode( DWORD fm );

		//拣选模式
		static CullMode GetCullMode( DWORD cm );

		//比较函数
		static CompareFunction GetCompareFun( DWORD cf );

		//混合类型
		static BlendType GetBlendType( DWORD bt );

		//混合因子类型
		static BlendFactor GetBlendFactor( DWORD bf );

		//混合方式
		static BlendOperation GetBlendOp( DWORD bo );

		//纹理状态类型
		static TextureStateType GetTextureState( DWORD tst );

		//纹理操作类型
		static TextureOperation GetTextureOp( DWORD to );

		//纹理操作参数
		static TextureArgument GetTextureArg( DWORD ta );

		//取样类型
		static SamplerType GetSamplerType( DWORD st );

		//纹理过滤类型
		static TextureFilterType GetTextureFilter( DWORD tft );

		//纹理寻址类型
		static TextureAddressType GetTextureAddress( DWORD tat );

		//内存管理类型
		static MemoryPoolMode GetPoolMode( DWORD mpm );

		//图元绘制类型
		static PrimitiveType GetPrimitiveType( DWORD pt );
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
