/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	D3D9VertexShader
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
	D3D9VertexShader::D3D9VertexShader()
		: mpD3D9VS		(NULL)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	D3D9VertexShader::~D3D9VertexShader()
	{
		ReleaseVertexShader();
	}
	//--------------------------------------------------------------------------------------------------------------
	//构建顶点着色器
	void D3D9VertexShader::BuildVertexShader()
	{
		HRESULT result = D3D9RenderSystem::mpD3D9Device->CreateVertexShader( mpShaderCode, &mpD3D9VS );
		if( FAILED( result ) )
		{
			Except( Exception::ERR_RENDER_API_ERROR, (String)"创建 D3D9 顶点着色器失败。"
			"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置顶点着色器
	void D3D9VertexShader::SetVertexShader()
	{
		//如果和上次设置的指针相同则无须重复设置
		if( mpLastSetVS == this )
			return;

		//设置着色器
		HRESULT result = D3D9RenderSystem::mpD3D9Device->SetVertexShader( mpD3D9VS );
		if( FAILED( result ) )
		{
			Except( Exception::ERR_RENDER_API_ERROR, (String)"设置 D3D9 顶点着色器失败。"
				"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置顶点着色器参数
	void D3D9VertexShader::SetVertexShaderParam()
	{
		HRESULT result;

		//循环设置所有着色器参数
		ShaderParam* pParam = mShaderParams.Begin();
		for( UINT i=0; i<mNumUniformParam; ++i )
		{
			switch ( pParam->eParamType )
			{
			case SPT_FLOAT:
				{
					result = D3D9RenderSystem::mpD3D9Device->SetVertexShaderConstantF(
						pParam->nRegIndex, (float*)pParam->pData, pParam->nNumUnits );
					break;
				}
			case SPT_INT:
				{
					result = D3D9RenderSystem::mpD3D9Device->SetVertexShaderConstantI(
						pParam->nRegIndex, (int*)pParam->pData, pParam->nNumUnits );
					break;
				}
			case SPT_BOOL:
				{
					result = D3D9RenderSystem::mpD3D9Device->SetVertexShaderConstantB(
						pParam->nRegIndex, (BOOL*)pParam->pData, pParam->nNumUnits );
					break;
				}
			default:
				Except( Exception::ERR_INTERNAL_ERROR, "错误的顶点着色器参数类型。" );
			}

			if( FAILED( result ) )
			{
				Except( Exception::ERR_RENDER_API_ERROR, (String)"设置 D3D9 顶点着色器参数失败。"
					"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
			}

			++pParam;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//释放顶点着色器
	void D3D9VertexShader::ReleaseVertexShader()
	{
		SafeRelease( mpD3D9VS );
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
