/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	D3D9VertexDeclaration
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
	D3D9VertexDeclaration::D3D9VertexDeclaration()
		: mpD3D9VertexDeclaration		(NULL)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	D3D9VertexDeclaration::~D3D9VertexDeclaration()
	{
		ReleaseVertexDeclaration();
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建顶点声明
	void D3D9VertexDeclaration::CreateVertexDeclaration()
	{
		static D3DVERTEXELEMENT9 D3DElement[nMaxElementNum];
		D3DVERTEXELEMENT9* pD3DElement = D3DElement;

		if( mNumElement > nMaxElementNum )
			Except( Exception::ERR_INVALIDPARAMS, "顶点声明所包含的顶点元素数量超过了最大值。" );

		//将顶点元素复制到 D3D9 顶点元素结构中
		VertexElement* pElement = mVertexElements.Begin();
		for( UINT i=0; i<mNumElement; ++i )
		{
			pD3DElement->Stream		= pElement->nStream;
			pD3DElement->Offset		= pElement->nOffset;
			pD3DElement->Type		= (BYTE)pElement->Type;
			pD3DElement->Method		= (BYTE)pElement->Method;
			pD3DElement->Usage		= (BYTE)pElement->Usage;
			pD3DElement->UsageIndex	= pElement->UsageIndex;

			++pD3DElement;
			++pElement;
		}

		//设置结尾顶点元素
		pD3DElement->Stream		= 0xFF;
		pD3DElement->Offset		= 0;
		pD3DElement->Type		= D3DDECLTYPE_UNUSED;
		pD3DElement->Method		= 0;
		pD3DElement->Usage		= 0;
		pD3DElement->UsageIndex	= 0;

		HRESULT result = D3D9RenderSystem::mpD3D9Device->CreateVertexDeclaration( D3DElement,
			&mpD3D9VertexDeclaration );
		if( FAILED( result ) )
			Except( Exception::ERR_RENDER_API_ERROR, (String)"创建 D3D9 顶点声明对象失败。"
			"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除顶点声明
	void D3D9VertexDeclaration::ReleaseVertexDeclaration()
	{
		SafeRelease( mpD3D9VertexDeclaration );
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
