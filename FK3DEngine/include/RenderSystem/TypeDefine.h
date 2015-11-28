/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	TypeDefine
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
	//立方盒
	struct Box3
	{
		UINT	nLeft;
		UINT	nTop;
		UINT	nRight;
		UINT	nBottom;
		UINT	nFront;
		UINT	nBack;
	};
	//--------------------------------------------------------------------------------------------------------------
	//锁定立方盒
	struct LockBox3
	{
		int		nRowPitch;
		int		nSlicePitch;
		void*	pBits;
	};
	//--------------------------------------------------------------------------------------------------------------
	//立方体面类型
	enum CubeFace
	{
		CF_POSITIVE_X		= 0,
		CF_NEGATIVE_X		= 1,
		CF_POSITIVE_Y		= 2,
		CF_NEGATIVE_Y		= 3,
		CF_POSITIVE_Z		= 4,
		CF_NEGATIVE_Z		= 5,
	};
	//--------------------------------------------------------------------------------------------------------------
	//灯光类型
	enum LightType
	{
		LT_POINT,			//点光
		LT_SPOT,			//聚光灯
		LT_DIRECTIONAL,		//直射光
	};
	//--------------------------------------------------------------------------------------------------------------
	//材质颜色来源
	struct MaterialSource
	{
		//来源类型
		enum SourceType
		{
			MST_Material		= 0,
			MST_VertexDiffuse	= 1,
			MST_VertexSpecular	= 2,
		};

		SourceType	DiffuseSource;	//漫射色来源
		SourceType	SpecularSource;	//高光色来源
		SourceType	AmbientSource;	//环境色来源
		SourceType	EmissiveSource;	//放射色来源
	};
	//--------------------------------------------------------------------------------------------------------------
	//索引数据类型
	enum IndexType
	{
		IT_UNKNOWN	= 0,
		IT_INDEX16	= 2,
		IT_INDEX32	= 4,
	};
	//--------------------------------------------------------------------------------------------------------------
	//纹理类型
	enum TextureUsage
	{
		TU_UNKNOWN	= 0x0,
		TU_NORMAL	= 0x1,
		TU_DYNAMIC	= 0x2,
		TU_TARGET	= 0x4,
	};
	//--------------------------------------------------------------------------------------------------------------
	//雾模式
	enum FogMode
	{
		FOG_NONE	= 0,	//无雾
		FOG_EXP		= 1,	//指数雾
		FOG_EXP2	= 2,	//二次方指数雾
		FOG_LINEAR	= 3,	//线性雾
	};
	//--------------------------------------------------------------------------------------------------------------
	//填充模式
	enum FillMode
	{
		FM_POINT		= 1,
		FM_WIREFRAME	= 2,
		FM_SOLID		= 3,
	};
	//--------------------------------------------------------------------------------------------------------------
	//阴影模式
	enum ShadeMode
	{
		SM_FLAT		= 1,
		SM_GOURAUD	= 2,
		SM_PHONG	= 3,
	};
	//--------------------------------------------------------------------------------------------------------------
	//比较函数
	enum CompareFunction
	{
		CMP_NEVER,			//总是失败
		CMP_LESS,			//小于时通过
		CMP_EQUAL,			//等于时通过
		CMP_LESSEQUAL,		//小于等于时通过
		CMP_GREATER,		//大于时通过
		CMP_NOTEQUAL,		//不等于时通过
		CMP_GREATEREQUAL,	//大于等于时通过
		CMP_ALWAYS,			//总是通过
	};
	//--------------------------------------------------------------------------------------------------------------
	//混合操作类型
	enum BlendType
	{
		BT_SRCBLEND,		//来源颜色混合类型
		BT_DESTBLEND,		//目标颜色混合类型
		BT_SRCBLENDALPHA,	//来源 ALPHA 值混合类型（要求开启分开混合 ALPHA 值选项）
		BT_DESTBLENDALPHA,	//目标 ALPHA 值混合类型（要求开启分开混合 ALPHA 值选项）
	};
	//--------------------------------------------------------------------------------------------------------------
	//混合操作因子
	enum BlendFactor
	{
		BF_ZERO,
		BF_ONE,
		BF_SRCCOLOR,
		BF_INVSRCCOLOR,
		BF_DESTCOLOR,
		BF_INVDESTCOLOR,
		BF_SRCALPHA,
		BF_INVSRCALPHA,
		BF_DESTALPHA,
		BF_INVDESTALPHA,
		BF_SRCALPHASAT,
		BF_BOTHSRCALPHA,
		BF_BOTHINVSRCALPHA,
		BF_BLENDFACTOR,
		BF_INVBLENDFACTOR,
	};
	//--------------------------------------------------------------------------------------------------------------
	//混合操作方式
	enum BlendOperation
	{
		BO_ADD,
		BO_SUBTRACT,
		BO_REVSUBTRACT,
		BO_MIN,
		BO_MAX,
	};
	//--------------------------------------------------------------------------------------------------------------
	//拣选模式
	enum CullMode
	{
		CM_NOCULL,			//不拣选
		CM_CLOCKWISE,		//顺时针方向拣选
		CM_ANTICLOCKWISE,	//逆时针方向拣选
	};
	//--------------------------------------------------------------------------------------------------------------
	//纹理状态类型
	enum TextureStateType
	{
		TST_COLOROP,
		TST_COLORARG1,
		TST_COLORARG2,
		TST_ALPHAOP,
		TST_ALPHAARG1,
		TST_ALPHAARG2,
		TST_BUMPENVMAT00,
		TST_BUMPENVMAT01,
		TST_BUMPENVMAT10,
		TST_BUMPENVMAT11,
		TST_COLORARG0,
		TST_ALPHAARG0,
	};
	//--------------------------------------------------------------------------------------------------------------
	//纹理操作类型
	enum TextureOperation
	{
		TO_DISABLE,
		TO_SELECTARG1,
		TO_SELECTARG2,
		TO_MODULATE,
		TO_ADD,
		TO_SUBTRACT,
		TO_BUMPENVMAP,
		TO_BUMPENVMAPLUMINANCE,
	};
	//--------------------------------------------------------------------------------------------------------------
	//纹理操作参数
	enum TextureArgument
	{
		TA_CURRENT,
		TA_DIFFUSE,
		TA_SELECTMASK,
		TA_SPECULAR,
		TA_TEMP,
		TA_TEXTURE,
		TA_TFACTOR,
	};
	//--------------------------------------------------------------------------------------------------------------
	//取样类型
	enum SamplerType
	{
		ST_ADDRESSU			= 0x1,
		ST_ADDRESSV			= 0x2,
		ST_ADDRESSW			= 0x4,
		ST_MAGFILTER		= 0x10,
		ST_MINFILTER		= 0x20,
		ST_MIPFILTER		= 0x40,
	};
	//--------------------------------------------------------------------------------------------------------------
	//纹理过滤类型
	enum TextureFilterType
	{
		TFT_NONE,
		TFT_POINT,
		TFT_LINEAR,
		TFT_ANISOTROPIC,
	};
	//--------------------------------------------------------------------------------------------------------------
	//纹理寻址类型
	enum TextureAddressType
	{
		TAT_UNKNOWN		= 0,
		TAT_WRAP		= 1,
		TAT_MIRROR		= 2,
		TAT_CLAMP		= 3,
		TAT_BORDER		= 4,
		TAT_MIRRORONCE	= 5,
	};
	//--------------------------------------------------------------------------------------------------------------
	//内存管理方式
	enum MemoryPoolMode
	{
		MPM_UNKNOWN		= 0,		//未知内存模式
		MPM_VIDEOMEM	= 1,		//显示内存
		MPM_SYSTEMMEM	= 2,		//系统内存
		MPM_AGPMEM		= 3,		//AGP 内存和显示内存
	};
	//--------------------------------------------------------------------------------------------------------------
	//图元绘制类型
	enum PrimitiveType
	{
		PT_POINT_LIST		= 1,
		PT_LINE_LIST		= 2,
		PT_LINE_STRIP		= 3,
		PT_TRIANGLE_LIST	= 4,
		PT_TRIANGLE_STRIP	= 5,
		PT_TRIANGLE_FAN		= 6,
	};
	//--------------------------------------------------------------------------------------------------------------
	//多流数据类型
	enum VertexElementType
	{
		VET_FLOAT1		= 0,	// 0	D3DDECLTYPE_FLOAT1
		VET_FLOAT2,				// 1	D3DDECLTYPE_FLOAT2
		VET_FLOAT3,				// 2	D3DDECLTYPE_FLOAT3
		VET_FLOAT4,				// 3	D3DDECLTYPE_FLOAT4
		VET_ARGBCOLOR,			// 4	D3DDECLTYPE_D3DCOLOR
		VET_UBYTE4,				// 5	D3DDECLTYPE_UBYTE4
		VET_SHORT2,				// 6	D3DDECLTYPE_SHORT2
		VET_SHORT4,				// 7	D3DDECLTYPE_SHORT4
		VET_UBYTE4N,			// 8	D3DDECLTYPE_UBYTE4N
		VET_SHORT2N,			// 9	D3DDECLTYPE_SHORT2N
		VET_SHORT4N,			// 10	D3DDECLTYPE_SHORT4N
		VET_USHORT2N,			// 11	D3DDECLTYPE_USHORT2N
		VET_USHORT4N,			// 12	D3DDECLTYPE_USHORT4N
		VET_UDEC3,				// 13	D3DDECLTYPE_UDEC3
		VET_DEC3N,				// 14	D3DDECLTYPE_DEC3N
		VET_FLOAT16_2,			// 15	D3DDECLTYPE_FLOAT16_2
		VET_FLOAT16_4,			// 16	D3DDECLTYPE_FLOAT16_4
		VET_UNUSED,				// 17	D3DDECLTYPE_UNUSED
	};
	//--------------------------------------------------------------------------------------------------------------
	enum VertexElementMethod
	{
		VEM_DEFAULT		= 0,	// 0	D3DDECLMETHOD_DEFAULT
		VEM_PARTIALU,			// 1	D3DDECLMETHOD_PARTIALU
		VEM_PARTIALV,			// 2	D3DDECLMETHOD_PARTIALV
		VEM_CROSSUV,			// 3	D3DDECLMETHOD_CROSSUV
		VEM_UV,					// 4	D3DDECLMETHOD_UV
		VEM_LOOKUP,				// 5	D3DDECLMETHOD_LOOKUP
		VEM_LOOKUPPRESAMPLED,	// 6	D3DDECLMETHOD_LOOKUPPRESAMPLED
	};
	//--------------------------------------------------------------------------------------------------------------
	//多流数据用途
	enum VertexElementUsage
	{
		VEU_POSITION		= 0,// 0	D3DDECLUSAGE_POSITION
		VEU_BLENDWEIGHT,		// 1	D3DDECLUSAGE_BLENDWEIGHT
		VEU_BLENDINDICES,		// 2	D3DDECLUSAGE_BLENDINDICES
		VEU_NORMAL,				// 3	D3DDECLUSAGE_NORMAL
		VEU_PSIZE,				// 4	D3DDECLUSAGE_PSIZE
		VEU_TEXCOORD,			// 5	D3DDECLUSAGE_TEXCOORD
		VEU_TANGENT,			// 6	D3DDECLUSAGE_TANGENT
		VEU_BINORMAL,			// 7	D3DDECLUSAGE_BINORMAL
		VEU_TESSFACTOR,			// 8	D3DDECLUSAGE_TESSFACTOR
		VEU_POSITIONT,			// 9	D3DDECLUSAGE_POSITIONT
		VEU_COLOR,				// 10	D3DDECLUSAGE_COLOR
		VEU_FOG,				// 11	D3DDECLUSAGE_FOG
		VEU_DEPTH,				// 12	D3DDECLUSAGE_DEPTH
		VEU_SAMPLE,				// 13	D3DDECLUSAGE_SAMPLE
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
