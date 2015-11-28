/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	D3D9TypeGet
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	D3D9 API 数据类型获取类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

#include "../../../Depend/dx9/d3dx9.h"
#include "../../../Depend/dx9/dxerr9.h"
#include "../CommonBase/MemoryMarco.h"

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	// D3D9 API 数据类型获取类
	class FK_DLL D3D9TypeGet
	{
	public:
		//像素格式
		static D3DFORMAT GetPixelFormat( PixelFormat pf );

		//索引数据类型
		static D3DFORMAT GetIndexType( IndexType it );

		//纹理用途
		static DWORD GetTextureUsage( TextureUsage tu );

		//阴影模式
		static D3DSHADEMODE GetShadeMode( ShadeMode sm );

		//灯光类型
		static D3DLIGHTTYPE GetLightType( LightType lt );

		//雾模式
		static D3DFOGMODE GetFogMode( FogMode fm );

		//填充模式
		static D3DFILLMODE GetFillMode( FillMode fm );

		//拣选模式
		static D3DCULL GetCullMode( CullMode cm );

		//比较函数
		static D3DCMPFUNC GetCompareFun( CompareFunction cf );

		//混合类型
		static D3DRENDERSTATETYPE GetBlendType( BlendType bt );

		//混合因子类型
		static D3DBLEND GetBlendFactor( BlendFactor bf );

		//混合方式
		static D3DBLENDOP GetBlendOp( BlendOperation bo );

		//纹理状态类型
		static D3DTEXTURESTAGESTATETYPE GetTextureState( TextureStateType tst );

		//纹理操作类型
		static D3DTEXTUREOP GetTextureOp( TextureOperation to );

		//纹理操作参数
		static DWORD GetTextureArg( TextureArgument ta );

		//取样类型
		static D3DSAMPLERSTATETYPE GetSamplerType( SamplerType st );

		//纹理过滤类型
		static D3DTEXTUREFILTERTYPE GetTextureFilter( TextureFilterType tft );

		//纹理寻址类型
		static D3DTEXTUREADDRESS GetTextureAddress( TextureAddressType tat );

		//内存管理类型
		static D3DPOOL GetPoolMode( MemoryPoolMode mpm );

		//图元绘制类型
		static D3DPRIMITIVETYPE GetPrimitiveType( PrimitiveType pt );
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
