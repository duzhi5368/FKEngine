/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	RenderSystemInsideHeader
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

#include "../CommonBase/CommonBaseInsideHead.h"
#include "../Math/MathInsideHead.h"

#include "../RenderWrapper/ConfigDataFormat.h"
#include "../RenderWrapper/ShaderDataFormat.h"
#include "../RenderWrapper/UVAFileFormat.h"

#ifdef _DEBUG
	#define D3D_DEBUG_INFO
#endif

#pragma comment ( lib, "d3d9.lib" )
#pragma comment ( lib, "d3dx9.lib" )


#include "TypeDefine.h"
#include "Viewport.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexDeclaration.h"
#include "BaseTexture.h"
#include "Texture.h"
#include "CubeTexture.h"
#include "VolumeTexture.h"
#include "TextureState.h"
#include "RenderState.h"
#include "BaseShader.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "RenderSystem.h"
#include "BufferManager.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include "D3D9TypeGet.h"
#include "D3D9TypeConvert.h"
#include "D3D9RenderSystem.h"
#include "D3D9VertexBuffer.h"
#include "D3D9IndexBuffer.h"
#include "D3D9VertexDeclaration.h"
#include "D3D9BaseTexture.h"
#include "D3D9Texture.h"
#include "D3D9CubeTexture.h"
#include "D3D9VolumeTexture.h"
#include "D3D9VertexShader.h"
#include "D3D9PixelShader.h"
#include "D3D9BufferManager.h"
#include "D3D9TextureManager.h"
#include "D3D9ShaderManager.h"

//==================================================================================================================