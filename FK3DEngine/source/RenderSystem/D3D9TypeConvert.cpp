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
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../../include/RenderSystem/RenderSystemInsideHeader.h"
#include "../../../Depend/dx9/d3dx9.h"
#include "../../../Depend/dx9/dxerr9.h"
#include "../../include/CommonBase/MemoryMarco.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//像素格式
	PixelFormat D3D9TypeConvert::GetPixelFormat( DWORD fmt )
	{
		switch (fmt)
		{
		case D3DFMT_A8R8G8B8:		return PF_A8R8G8B8;
		case D3DFMT_X8R8G8B8:		return PF_X8R8G8B8;
		case D3DFMT_R8G8B8:			return PF_R8G8B8;
		case D3DFMT_R5G6B5:			return PF_R5G6B5;
		case D3DFMT_A1R5G5B5:		return PF_A1R5G5B5;
		case D3DFMT_X1R5G5B5:		return PF_X1R5G5B5;
		case D3DFMT_A4R4G4B4:		return PF_A4R4G4B4;
		case D3DFMT_DXT1:			return PF_DXT1;
		case D3DFMT_DXT2:			return PF_DXT2;
		case D3DFMT_DXT3:			return PF_DXT3;
		case D3DFMT_DXT4:			return PF_DXT4;
		case D3DFMT_DXT5:			return PF_DXT5;
		case D3DFMT_V8U8:			return PF_V8U8;
		case D3DFMT_V16U16:			return PF_V16U16;
		case D3DFMT_L6V5U5:			return PF_L6V5U5;
		case D3DFMT_X8L8V8U8:		return PF_X8L8V8U8;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "无法转换的像素格式。" );
		}

		return PF_UNKNOWN;
	}
	//--------------------------------------------------------------------------------------------------------------
	//索引数据类型
	IndexType D3D9TypeConvert::GetIndexType( DWORD it )
	{
		switch (it)
		{
		case D3DFMT_INDEX16:		return IT_INDEX16;
		case D3DFMT_INDEX32:		return IT_INDEX32;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "无法转换的索引数据类型。" );
		}

		return IT_UNKNOWN;
	}
	//--------------------------------------------------------------------------------------------------------------
	//纹理用途
	TextureUsage D3D9TypeConvert::GetTextureUsage( DWORD tu )
	{
		switch (tu)
		{
		case 0:							return TU_NORMAL;
		case D3DUSAGE_DYNAMIC:			return TU_DYNAMIC;
		case D3DUSAGE_RENDERTARGET:		return TU_TARGET;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "无法转换的纹理用途。" );
		}

		return TU_UNKNOWN;
	}
	//--------------------------------------------------------------------------------------------------------------
	//阴影模式
	ShadeMode D3D9TypeConvert::GetShadeMode( DWORD sm )
	{
		switch (sm)
		{
		case D3DSHADE_FLAT:				return SM_FLAT;
		case D3DSHADE_GOURAUD:			return SM_GOURAUD;
		case D3DSHADE_PHONG:			return SM_PHONG;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "无法转换的阴影模式。" );
		}

		return (ShadeMode)0;
	}
	//--------------------------------------------------------------------------------------------------------------
	//灯光类型
	LightType D3D9TypeConvert::GetLightType( DWORD lt )
	{
		switch (lt)
		{
		case D3DLIGHT_POINT:			return LT_POINT;
		case D3DLIGHT_SPOT:				return LT_SPOT;
		case D3DLIGHT_DIRECTIONAL:		return LT_DIRECTIONAL;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "无法转换的阴影模式。" );
		}

		return (LightType)0;
	}
	//--------------------------------------------------------------------------------------------------------------
	//雾模式
	FogMode D3D9TypeConvert::GetFogMode( DWORD fm )
	{
		switch (fm)
		{
		case D3DFOG_NONE:			return FOG_NONE;
		case D3DFOG_EXP:			return FOG_EXP;
		case D3DFOG_EXP2:			return FOG_EXP2;
		case D3DFOG_LINEAR:			return FOG_LINEAR;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "无法转换的雾模式。" );
		}

		return FOG_NONE;
	}
	//--------------------------------------------------------------------------------------------------------------
	//填充模式
	FillMode D3D9TypeConvert::GetFillMode( DWORD fm )
	{
		switch (fm)
		{
		case D3DFILL_POINT:			return FM_POINT;
		case D3DFILL_WIREFRAME:		return FM_WIREFRAME;
		case D3DFILL_SOLID:			return FM_SOLID;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "无法转换的填充模式。" );
		}

		return (FillMode)0;
	}
	//--------------------------------------------------------------------------------------------------------------
	//拣选模式
	CullMode D3D9TypeConvert::GetCullMode( DWORD cm )
	{
		switch (cm)
		{
		case D3DCULL_NONE:			return CM_NOCULL;
		case D3DCULL_CW:			return CM_CLOCKWISE;
		case D3DCULL_CCW:			return CM_ANTICLOCKWISE;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "无法转换的拣选模式。" );
		}

		return (CullMode)0;
	}
	//--------------------------------------------------------------------------------------------------------------
	//比较函数
	CompareFunction D3D9TypeConvert::GetCompareFun( DWORD cf )
	{
		switch (cf)
		{
		case D3DCMP_GREATER:			return CMP_GREATER;
		case D3DCMP_NEVER:				return CMP_NEVER;
		case D3DCMP_GREATEREQUAL:		return CMP_GREATEREQUAL;
		case D3DCMP_ALWAYS:				return CMP_ALWAYS;
		case D3DCMP_LESS:				return CMP_LESS;
		case D3DCMP_EQUAL:				return CMP_EQUAL;
		case D3DCMP_LESSEQUAL:			return CMP_LESSEQUAL;
		case D3DCMP_NOTEQUAL:			return CMP_NOTEQUAL;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "无法转换的比较函数。" );
		}

		return (CompareFunction)0;
	}
	//--------------------------------------------------------------------------------------------------------------
	//混合类型
	BlendType D3D9TypeConvert::GetBlendType( DWORD bt )
	{
		switch (bt)
		{
		case D3DRS_SRCBLEND:			return BT_SRCBLEND;
		case D3DRS_DESTBLEND:			return BT_DESTBLEND;
		case D3DRS_SRCBLENDALPHA:		return BT_SRCBLENDALPHA;
		case D3DRS_DESTBLENDALPHA:		return BT_DESTBLENDALPHA;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "无法转换的混合类型。" );
		}

		return (BlendType)0;
	}
	//--------------------------------------------------------------------------------------------------------------
	//混合因子类型
	BlendFactor D3D9TypeConvert::GetBlendFactor( DWORD bf )
	{
		switch (bf)
		{
		case D3DBLEND_SRCALPHA:				return BF_SRCALPHA;
		case D3DBLEND_DESTALPHA:			return BF_DESTALPHA;
		case D3DBLEND_INVDESTALPHA:			return BF_INVDESTALPHA;
		case D3DBLEND_INVSRCALPHA:			return BF_INVSRCALPHA;
		case D3DBLEND_SRCCOLOR:				return BF_SRCCOLOR;
		case D3DBLEND_INVSRCCOLOR:			return BF_INVSRCCOLOR;
		case D3DBLEND_DESTCOLOR:			return BF_DESTCOLOR;
		case D3DBLEND_INVDESTCOLOR:			return BF_INVDESTCOLOR;
		case D3DBLEND_BLENDFACTOR:			return BF_BLENDFACTOR;
		case D3DBLEND_INVBLENDFACTOR:		return BF_INVBLENDFACTOR;
		case D3DBLEND_ZERO:					return BF_ZERO;
		case D3DBLEND_ONE:					return BF_ONE;
		case D3DBLEND_SRCALPHASAT:			return BF_SRCALPHASAT;
		case D3DBLEND_BOTHSRCALPHA:			return BF_BOTHSRCALPHA;
		case D3DBLEND_BOTHINVSRCALPHA:		return BF_BOTHINVSRCALPHA;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "无法转换的混合因子类型。" );
		}

		return (BlendFactor)0;
	}
	//--------------------------------------------------------------------------------------------------------------
	//混合方式
	BlendOperation D3D9TypeConvert::GetBlendOp( DWORD bo )
	{
		switch (bo)
		{
		case D3DBLENDOP_ADD:			return BO_ADD;
		case D3DBLENDOP_SUBTRACT:		return BO_SUBTRACT;
		case D3DBLENDOP_REVSUBTRACT:	return BO_REVSUBTRACT;
		case D3DBLENDOP_MIN:			return BO_MIN;
		case D3DBLENDOP_MAX:			return BO_MAX;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "无法转换的混合方式。" );
		}

		return (BlendOperation)0;
	}
	//--------------------------------------------------------------------------------------------------------------
	//纹理状态类型
	TextureStateType D3D9TypeConvert::GetTextureState( DWORD tst )
	{
		switch (tst)
		{
		case D3DTSS_COLOROP:			return TST_COLOROP;
		case D3DTSS_COLORARG1:			return TST_COLORARG1;
		case D3DTSS_COLORARG2:			return TST_COLORARG2;
		case D3DTSS_ALPHAOP:			return TST_ALPHAOP;
		case D3DTSS_ALPHAARG1:			return TST_ALPHAARG1;
		case D3DTSS_ALPHAARG2:			return TST_ALPHAARG2;
		case D3DTSS_COLORARG0:			return TST_COLORARG0;
		case D3DTSS_ALPHAARG0:			return TST_ALPHAARG0;
		case D3DTSS_BUMPENVMAT00:		return TST_BUMPENVMAT00;
		case D3DTSS_BUMPENVMAT01:		return TST_BUMPENVMAT01;
		case D3DTSS_BUMPENVMAT10:		return TST_BUMPENVMAT10;
		case D3DTSS_BUMPENVMAT11:		return TST_BUMPENVMAT11;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "无法转换的多纹理用途。" );
		}

		return (TextureStateType)0;
	}
	//--------------------------------------------------------------------------------------------------------------
	//纹理操作类型
	TextureOperation D3D9TypeConvert::GetTextureOp( DWORD to )
	{
		switch (to)
		{
		case D3DTOP_DISABLE:				return TO_DISABLE;
		case D3DTOP_SELECTARG1:				return TO_SELECTARG1;
		case D3DTOP_SELECTARG2:				return TO_SELECTARG2;
		case D3DTOP_MODULATE:				return TO_MODULATE;
		case D3DTOP_ADD:					return TO_ADD;
		case D3DTOP_SUBTRACT:				return TO_SUBTRACT;
		case D3DTOP_BUMPENVMAP:				return TO_BUMPENVMAP;
		case D3DTOP_BUMPENVMAPLUMINANCE:	return TO_BUMPENVMAPLUMINANCE;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "无法转换的纹理混合类型。" );
		}

		return (TextureOperation)0;
	}
	//--------------------------------------------------------------------------------------------------------------
	//纹理操作参数
	TextureArgument D3D9TypeConvert::GetTextureArg( DWORD ta )
	{
		switch (ta)
		{
		case D3DTA_CURRENT:			return TA_CURRENT;
		case D3DTA_TEXTURE:			return TA_TEXTURE;
		case D3DTA_DIFFUSE:			return TA_DIFFUSE;
		case D3DTA_SPECULAR:		return TA_SPECULAR;
		case D3DTA_SELECTMASK:		return TA_SELECTMASK;
		case D3DTA_TEMP:			return TA_TEMP;
		case D3DTA_TFACTOR:			return TA_TFACTOR;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "无法转换的多纹理操作参数值。" );
		}

		return (TextureArgument)0;
	}
	//--------------------------------------------------------------------------------------------------------------
	//取样类型
	SamplerType D3D9TypeConvert::GetSamplerType( DWORD st )
	{
		switch (st)
		{
		case D3DSAMP_ADDRESSU:			return ST_ADDRESSU;
		case D3DSAMP_ADDRESSV:			return ST_ADDRESSV;
		case D3DSAMP_ADDRESSW:			return ST_ADDRESSW;
		case D3DSAMP_MAGFILTER:			return ST_MAGFILTER;
		case D3DSAMP_MINFILTER:			return ST_MINFILTER;
		case D3DSAMP_MIPFILTER:			return ST_MIPFILTER;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "无法转换的取样类型。" );
		}

		return (SamplerType)0;
	}
	//--------------------------------------------------------------------------------------------------------------
	//纹理过滤类型
	TextureFilterType D3D9TypeConvert::GetTextureFilter( DWORD tft )
	{
		switch (tft)
		{
		case D3DTEXF_NONE:				return TFT_NONE;
		case D3DTEXF_LINEAR:			return TFT_LINEAR;
		case D3DTEXF_POINT:				return TFT_POINT;
		case D3DTEXF_ANISOTROPIC:		return TFT_ANISOTROPIC;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "无法转换的纹理过滤类型。" );
		}

		return (TextureFilterType)0;
	}
	//--------------------------------------------------------------------------------------------------------------
	//纹理寻址类型
	TextureAddressType D3D9TypeConvert::GetTextureAddress( DWORD tat )
	{
		switch (tat)
		{
		case D3DTADDRESS_WRAP:			return TAT_WRAP;
		case D3DTADDRESS_MIRROR:		return TAT_MIRROR;
		case D3DTADDRESS_CLAMP:			return TAT_CLAMP;
		case D3DTADDRESS_BORDER:		return TAT_BORDER;
		case D3DTADDRESS_MIRRORONCE:	return TAT_MIRRORONCE;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "无法转换的纹理寻址类型。" );
		}

		return TAT_UNKNOWN;
	}
	//--------------------------------------------------------------------------------------------------------------
	//内存管理类型
	MemoryPoolMode D3D9TypeConvert::GetPoolMode( DWORD mpm )
	{
		switch (mpm)
		{
		case D3DPOOL_MANAGED:		return MPM_AGPMEM;
		case D3DPOOL_DEFAULT:		return MPM_VIDEOMEM;
		case D3DPOOL_SYSTEMMEM:		return MPM_SYSTEMMEM;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "无法转换的内存管理类型。" );
		}

		return MPM_UNKNOWN;
	}
	//--------------------------------------------------------------------------------------------------------------
	//图元绘制类型
	PrimitiveType D3D9TypeConvert::GetPrimitiveType( DWORD pt )
	{
		switch (pt)
		{
		case D3DPT_TRIANGLELIST:		return PT_TRIANGLE_LIST;
		case D3DPT_TRIANGLEFAN:			return PT_TRIANGLE_FAN;
		case D3DPT_TRIANGLESTRIP:		return PT_TRIANGLE_STRIP;
		case D3DPT_POINTLIST:			return PT_POINT_LIST;
		case D3DPT_LINELIST:			return PT_LINE_LIST;
		case D3DPT_LINESTRIP:			return PT_LINE_STRIP;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "无法转换的图元绘制类型。" );
		}

		return (PrimitiveType)0;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
