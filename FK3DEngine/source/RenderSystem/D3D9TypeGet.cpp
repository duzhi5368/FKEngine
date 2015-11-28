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
	//像素格式
	D3DFORMAT D3D9TypeGet::GetPixelFormat( PixelFormat pf )
	{
		switch (pf)
		{
		case PF_A8R8G8B8:		return D3DFMT_A8R8G8B8;
		case PF_X8R8G8B8:		return D3DFMT_X8R8G8B8;
		case PF_R8G8B8:			return D3DFMT_R8G8B8;
		case PF_R5G6B5:			return D3DFMT_R5G6B5;
		case PF_A1R5G5B5:		return D3DFMT_A1R5G5B5;
		case PF_X1R5G5B5:		return D3DFMT_X1R5G5B5;
		case PF_A4R4G4B4:		return D3DFMT_A4R4G4B4;
		case PF_DXT1:			return D3DFMT_DXT1;
		case PF_DXT2:			return D3DFMT_DXT2;
		case PF_DXT3:			return D3DFMT_DXT3;
		case PF_DXT4:			return D3DFMT_DXT4;
		case PF_DXT5:			return D3DFMT_DXT5;
		case PF_V8U8:			return D3DFMT_V8U8;
		case PF_V16U16:			return D3DFMT_V16U16;
		case PF_L6V5U5:			return D3DFMT_L6V5U5;
		case PF_X8L8V8U8:		return D3DFMT_X8L8V8U8;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "无法转换的像素格式。" );
		}

		return D3DFMT_UNKNOWN;
	}
	//--------------------------------------------------------------------------------------------------------------
	//索引数据类型
	D3DFORMAT D3D9TypeGet::GetIndexType( IndexType it )
	{
		switch (it)
		{
		case IT_INDEX16:		return D3DFMT_INDEX16;
		case IT_INDEX32:		return D3DFMT_INDEX32;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "无法转换的索引数据类型。" );
		}

		return D3DFMT_UNKNOWN;
	}
	//--------------------------------------------------------------------------------------------------------------
	//纹理用途
	DWORD D3D9TypeGet::GetTextureUsage( TextureUsage tu )
	{
		switch (tu)
		{
		case TU_NORMAL:			return 0;
		case TU_DYNAMIC:		return D3DUSAGE_DYNAMIC;
		case TU_TARGET:			return D3DUSAGE_RENDERTARGET;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "无法转换的纹理用途。" );
		}

		return 0;
	}
	//--------------------------------------------------------------------------------------------------------------
	//阴影模式
	D3DSHADEMODE D3D9TypeGet::GetShadeMode( ShadeMode sm )
	{
		switch (sm)
		{
		case SM_FLAT:			return D3DSHADE_FLAT;
		case SM_GOURAUD:		return D3DSHADE_GOURAUD;
		case SM_PHONG:			return D3DSHADE_PHONG;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "无法转换的阴影模式。" );
		}

		return (D3DSHADEMODE)0;
	}
	//--------------------------------------------------------------------------------------------------------------
	//灯光类型
	D3DLIGHTTYPE D3D9TypeGet::GetLightType( LightType lt )
	{
		switch (lt)
		{
		case LT_POINT:			return D3DLIGHT_POINT;
		case LT_SPOT:			return D3DLIGHT_SPOT;
		case LT_DIRECTIONAL:	return D3DLIGHT_DIRECTIONAL;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "无法转换的阴影模式。" );
		}

		return (D3DLIGHTTYPE)0;
	}
	//--------------------------------------------------------------------------------------------------------------
	//雾模式
	D3DFOGMODE D3D9TypeGet::GetFogMode( FogMode fm )
	{
		switch (fm)
		{
		case FOG_NONE:			return D3DFOG_NONE;
		case FOG_EXP:			return D3DFOG_EXP;
		case FOG_EXP2:			return D3DFOG_EXP2;
		case FOG_LINEAR:		return D3DFOG_LINEAR;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "无法转换的雾模式。" );
		}

		return D3DFOG_NONE;
	}
	//--------------------------------------------------------------------------------------------------------------
	//填充模式
	D3DFILLMODE D3D9TypeGet::GetFillMode( FillMode fm )
	{
		switch (fm)
		{
		case FM_POINT:			return D3DFILL_POINT;
		case FM_WIREFRAME:		return D3DFILL_WIREFRAME;
		case FM_SOLID:			return D3DFILL_SOLID;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "无法转换的填充模式。" );
		}

		return (D3DFILLMODE)0;
	}
	//--------------------------------------------------------------------------------------------------------------
	//拣选模式
	D3DCULL D3D9TypeGet::GetCullMode( CullMode cm )
	{
		switch (cm)
		{
		case CM_NOCULL:			return D3DCULL_NONE;
		case CM_CLOCKWISE:		return D3DCULL_CW;
		case CM_ANTICLOCKWISE:	return D3DCULL_CCW;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "无法转换的拣选模式。" );
		}

		return (D3DCULL)0;
	}
	//--------------------------------------------------------------------------------------------------------------
	//比较函数
	D3DCMPFUNC D3D9TypeGet::GetCompareFun( CompareFunction cf )
	{
		switch (cf)
		{
		case CMP_GREATER:			return D3DCMP_GREATER;
		case CMP_NEVER:				return D3DCMP_NEVER;
		case CMP_GREATEREQUAL:		return D3DCMP_GREATEREQUAL;
		case CMP_ALWAYS:			return D3DCMP_ALWAYS;
		case CMP_LESS:				return D3DCMP_LESS;
		case CMP_EQUAL:				return D3DCMP_EQUAL;
		case CMP_LESSEQUAL:			return D3DCMP_LESSEQUAL;
		case CMP_NOTEQUAL:			return D3DCMP_NOTEQUAL;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "无法转换的比较函数。" );
		}

		return (D3DCMPFUNC)0;
	}
	//--------------------------------------------------------------------------------------------------------------
	//混合类型
	D3DRENDERSTATETYPE D3D9TypeGet::GetBlendType( BlendType bt )
	{
		switch (bt)
		{
		case BT_SRCBLEND:			return D3DRS_SRCBLEND;
		case BT_DESTBLEND:			return D3DRS_DESTBLEND;
		case BT_SRCBLENDALPHA:		return D3DRS_SRCBLENDALPHA;
		case BT_DESTBLENDALPHA:		return D3DRS_DESTBLENDALPHA;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "无法转换的混合类型。" );
		}

		return (D3DRENDERSTATETYPE)0;
	}
	//--------------------------------------------------------------------------------------------------------------
	//混合因子类型
	D3DBLEND D3D9TypeGet::GetBlendFactor( BlendFactor bf )
	{
		switch (bf)
		{
		case BF_SRCALPHA:			return D3DBLEND_SRCALPHA;
		case BF_DESTALPHA:			return D3DBLEND_DESTALPHA;
		case BF_INVDESTALPHA:		return D3DBLEND_INVDESTALPHA;
		case BF_INVSRCALPHA:		return D3DBLEND_INVSRCALPHA;
		case BF_SRCCOLOR:			return D3DBLEND_SRCCOLOR;
		case BF_INVSRCCOLOR:		return D3DBLEND_INVSRCCOLOR;
		case BF_DESTCOLOR:			return D3DBLEND_DESTCOLOR;
		case BF_INVDESTCOLOR:		return D3DBLEND_INVDESTCOLOR;
		case BF_BLENDFACTOR:		return D3DBLEND_BLENDFACTOR;
		case BF_INVBLENDFACTOR:		return D3DBLEND_INVBLENDFACTOR;
		case BF_ZERO:				return D3DBLEND_ZERO;
		case BF_ONE:				return D3DBLEND_ONE;
		case BF_SRCALPHASAT:		return D3DBLEND_SRCALPHASAT;
		case BF_BOTHSRCALPHA:		return D3DBLEND_BOTHSRCALPHA;
		case BF_BOTHINVSRCALPHA:	return D3DBLEND_BOTHINVSRCALPHA;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "无法转换的混合因子类型。" );
		}

		return (D3DBLEND)0;
	}
	//--------------------------------------------------------------------------------------------------------------
	//混合方式
	D3DBLENDOP D3D9TypeGet::GetBlendOp( BlendOperation bo )
	{
		switch (bo)
		{
		case BO_ADD:			return D3DBLENDOP_ADD;
		case BO_SUBTRACT:		return D3DBLENDOP_SUBTRACT;
		case BO_REVSUBTRACT:	return D3DBLENDOP_REVSUBTRACT;
		case BO_MIN:			return D3DBLENDOP_MIN;
		case BO_MAX:			return D3DBLENDOP_MAX;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "无法转换的混合方式。" );
		}

		return (D3DBLENDOP)0;
	}
	//--------------------------------------------------------------------------------------------------------------
	//纹理状态类型
	D3DTEXTURESTAGESTATETYPE D3D9TypeGet::GetTextureState( TextureStateType tst )
	{
		switch (tst)
		{
		case TST_COLOROP:			return D3DTSS_COLOROP;
		case TST_COLORARG1:			return D3DTSS_COLORARG1;
		case TST_COLORARG2:			return D3DTSS_COLORARG2;
		case TST_ALPHAOP:			return D3DTSS_ALPHAOP;
		case TST_ALPHAARG1:			return D3DTSS_ALPHAARG1;
		case TST_ALPHAARG2:			return D3DTSS_ALPHAARG2;
		case TST_COLORARG0:			return D3DTSS_COLORARG0;
		case TST_ALPHAARG0:			return D3DTSS_ALPHAARG0;
		case TST_BUMPENVMAT00:		return D3DTSS_BUMPENVMAT00;
		case TST_BUMPENVMAT01:		return D3DTSS_BUMPENVMAT01;
		case TST_BUMPENVMAT10:		return D3DTSS_BUMPENVMAT10;
		case TST_BUMPENVMAT11:		return D3DTSS_BUMPENVMAT11;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "无法转换的多纹理用途。" );
		}

		return (D3DTEXTURESTAGESTATETYPE)0;
	}
	//--------------------------------------------------------------------------------------------------------------
	//纹理状态参数
	D3DTEXTUREOP D3D9TypeGet::GetTextureOp( TextureOperation to )
	{
		switch (to)
		{
		case TO_DISABLE:				return D3DTOP_DISABLE;
		case TO_SELECTARG1:				return D3DTOP_SELECTARG1;
		case TO_SELECTARG2:				return D3DTOP_SELECTARG2;
		case TO_MODULATE:				return D3DTOP_MODULATE;
		case TO_ADD:					return D3DTOP_ADD;
		case TO_SUBTRACT:				return D3DTOP_SUBTRACT;
		case TO_BUMPENVMAP:				return D3DTOP_BUMPENVMAP;
		case TO_BUMPENVMAPLUMINANCE:	return D3DTOP_BUMPENVMAPLUMINANCE;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "无法转换的纹理混合类型。" );
		}

		return (D3DTEXTUREOP)0;
	}
	//--------------------------------------------------------------------------------------------------------------
	//纹理操作参数
	DWORD D3D9TypeGet::GetTextureArg( TextureArgument ta )
	{
		switch (ta)
		{
		case TA_CURRENT:		return D3DTA_CURRENT;
		case TA_TEXTURE:		return D3DTA_TEXTURE;
		case TA_DIFFUSE:		return D3DTA_DIFFUSE;
		case TA_SPECULAR:		return D3DTA_SPECULAR;
		case TA_SELECTMASK:		return D3DTA_SELECTMASK;
		case TA_TEMP:			return D3DTA_TEMP;
		case TA_TFACTOR:		return D3DTA_TFACTOR;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "无法转换的多纹理操作参数值。" );
		}

		return 0;
	}
	//--------------------------------------------------------------------------------------------------------------
	//取样类型
	D3DSAMPLERSTATETYPE D3D9TypeGet::GetSamplerType( SamplerType st )
	{
		switch (st)
		{
		case ST_ADDRESSU:			return D3DSAMP_ADDRESSU;
		case ST_ADDRESSV:			return D3DSAMP_ADDRESSV;
		case ST_ADDRESSW:			return D3DSAMP_ADDRESSW;
		case ST_MAGFILTER:			return D3DSAMP_MAGFILTER;
		case ST_MINFILTER:			return D3DSAMP_MINFILTER;
		case ST_MIPFILTER:			return D3DSAMP_MIPFILTER;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "无法转换的取样类型。" );
		}

		return (D3DSAMPLERSTATETYPE)0;
	}
	//--------------------------------------------------------------------------------------------------------------
	//纹理过滤类型
	D3DTEXTUREFILTERTYPE D3D9TypeGet::GetTextureFilter( TextureFilterType tft )
	{
		switch (tft)
		{
		case TFT_NONE:				return D3DTEXF_NONE;
		case TFT_LINEAR:			return D3DTEXF_LINEAR;
		case TFT_POINT:				return D3DTEXF_POINT;
		case TFT_ANISOTROPIC:		return D3DTEXF_ANISOTROPIC;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "无法转换的纹理过滤类型。" );
		}

		return (D3DTEXTUREFILTERTYPE)0;
	}
	//--------------------------------------------------------------------------------------------------------------
	//纹理寻址类型
	D3DTEXTUREADDRESS D3D9TypeGet::GetTextureAddress( TextureAddressType tat )
	{
		switch (tat)
		{
		case TAT_WRAP:			return D3DTADDRESS_WRAP;
		case TAT_MIRROR:		return D3DTADDRESS_MIRROR;
		case TAT_CLAMP:			return D3DTADDRESS_CLAMP;
		case TAT_BORDER:		return D3DTADDRESS_BORDER;
		case TAT_MIRRORONCE:	return D3DTADDRESS_MIRRORONCE;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "无法转换的纹理寻址类型。" );
		}

		return (D3DTEXTUREADDRESS)0;
	}
	//--------------------------------------------------------------------------------------------------------------
	//内存管理类型
	D3DPOOL D3D9TypeGet::GetPoolMode( MemoryPoolMode mpm )
	{
		switch (mpm)
		{
		case MPM_AGPMEM:		return D3DPOOL_MANAGED;
		case MPM_VIDEOMEM:		return D3DPOOL_DEFAULT;
		case MPM_SYSTEMMEM:		return D3DPOOL_SYSTEMMEM;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "无法转换的内存管理类型。" );
		}

		return D3DPOOL_DEFAULT;
	}
	//--------------------------------------------------------------------------------------------------------------
	//图元绘制类型
	D3DPRIMITIVETYPE D3D9TypeGet::GetPrimitiveType( PrimitiveType pt )
	{
		switch (pt)
		{
		case PT_TRIANGLE_LIST:		return D3DPT_TRIANGLELIST;
		case PT_TRIANGLE_FAN:		return D3DPT_TRIANGLEFAN;
		case PT_TRIANGLE_STRIP:		return D3DPT_TRIANGLESTRIP;
		case PT_POINT_LIST:			return D3DPT_POINTLIST;
		case PT_LINE_LIST:			return D3DPT_LINELIST;
		case PT_LINE_STRIP:			return D3DPT_LINESTRIP;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "无法转换的图元绘制类型。" );
		}

		return (D3DPRIMITIVETYPE)0;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
