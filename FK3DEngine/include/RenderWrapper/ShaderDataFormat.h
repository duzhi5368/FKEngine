/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	ShaderDataFormat
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	着色器数据格式
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------

	// Cg 语言类型
	enum CgLanguageType
	{
		Unknown		= 0,		//未知类型

		DX_VS_11	= 1,		// DirectX VS 1.1 for vertex shader
		DX_VS_20	= 2,		// DirectX VS 2.0 for vertex shader
		DX_VS_2X	= 3,		// DirectX VS 2.0 for extended vertex shader
		DX_PS_11	= 4,		// DirectX PS 1.1 for pixel shader
		DX_PS_12	= 5,		// DirectX PS 1.2 for pixel shader
		DX_PS_13	= 6,		// DirectX PS 1.3 for pixel shader
		DX_PS_20	= 7,		// DirectX PS 2.0 for pixel shader
		DX_PS_2X	= 8,		// DirectX PS 2.0 for extended pixel shader

		OGL_VP_20	= 9,		// vertex program for OpenGL NV_vertex_program 1.0
		OGL_VP_30	= 10,		// vertex program for OpenGL NV_vertex_program 2.0
		OGL_ARB_VP1	= 11,		// vertex program for OpenGL ARB Vertex Program
		OGL_FP_20	= 12,		// fragment program for OpenGL NV_texture_shader and NV_register_combiners
		OGL_FP_30	= 13,		// fragment program for OpenGL NV_fragment_program
		OGL_ARB_FP1	= 14,		// fragment program for OpenGL ARB Fragment Program
	};

	//--------------------------------------------------------------------------------------------------------------

	//配置文件头标识
	const char szScgFileHeader[] = "FK Cg Shader File";

	// Cg 着色器信息头
	struct CgShaderData
	{
		CgLanguageType	eShaderLangType;		//着色器语言类型
		UINT			nNumVaryingParam;		//可变参数数量
		UINT			nNumUniformParam;		//统一参数数量
		UINT			nNumConstantParam;		//常量参数数量
		DWORD			nShaderCodeLen;			//着色器代码长度
	};

	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
