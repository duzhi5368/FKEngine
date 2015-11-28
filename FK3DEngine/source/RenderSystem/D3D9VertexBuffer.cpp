/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	D3D9VertexBuffer
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
	D3D9VertexBuffer::D3D9VertexBuffer()
		: mpD3D9VertexBuffer	(NULL)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	D3D9VertexBuffer::~D3D9VertexBuffer()
	{
		ReleaseVertexBuffer();
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建顶点缓存
	void D3D9VertexBuffer::CreateVertexBuffer( UINT nVerSize, UINT nVerNum, bool bDynamic, bool bSoftProc )
	{
		//存储缓存信息
		mnBufLen = nVerSize * nVerNum;
		mVerSize = nVerSize;
		mbDynamic = bDynamic;

		D3DPOOL D3DPool = D3DPOOL_MANAGED;
		DWORD dwUsage = D3DUSAGE_WRITEONLY;

		//如果要求使用动态缓存
		if( bDynamic )
		{
			D3DPool = D3DPOOL_DEFAULT;
			dwUsage |= D3DUSAGE_DYNAMIC;
		}

		//如果要求软件顶点处理
		if( bSoftProc )
		{
			D3DPool = D3DPOOL_MANAGED;
			dwUsage |= D3DUSAGE_SOFTWAREPROCESSING;
		}

		//创建顶点缓存
		HRESULT result = D3D9RenderSystem::mpD3D9Device->CreateVertexBuffer( mnBufLen, dwUsage, 0, D3DPool,
			&mpD3D9VertexBuffer, NULL );
		if( FAILED( result ) )
			Except( Exception::ERR_RENDER_API_ERROR, (String)"创建 Direct3D 9 顶点缓存错误！"
			"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
	}
	//--------------------------------------------------------------------------------------------------------------
	//释放顶点缓存
	void D3D9VertexBuffer::ReleaseVertexBuffer()
	{
		SafeRelease( mpD3D9VertexBuffer );
	}
	//--------------------------------------------------------------------------------------------------------------
	//锁定顶点缓存
	void* D3D9VertexBuffer::Lock( UINT nOffsetVer, UINT nLockVer, bool bNoOverwrite )
	{
		UINT nOffset = nOffsetVer * mVerSize;
		UINT nLockSize = nLockVer * mVerSize;

		//如果为调试模式则检测锁定长度
#ifdef _DEBUG
		if( nOffset != 0 && nLockSize == 0 )
		{
			Except( Exception::ERR_INVALIDPARAMS, "锁定顶点缓存长度为零。如果需要锁定整个缓存，请将顶点偏移量也设为零。" );
			return NULL;
		}

		if( nOffset + nLockSize > mnBufLen )
		{
			Except( Exception::ERR_INVALIDPARAMS, "锁定 D3D9 顶点缓存溢出。" );
			return NULL;
		}
#endif

		//设置锁定模式
		DWORD dwFlags = mbDynamic ? D3DLOCK_DISCARD : D3DLOCK_NOSYSLOCK;

		if( bNoOverwrite )
		{
			if( mbDynamic )
				dwFlags = D3DLOCK_NOOVERWRITE;
			else
				dwFlags |= D3DLOCK_NOOVERWRITE;
		}

		//锁定
		void* pBuffer = NULL;
		HRESULT result = mpD3D9VertexBuffer->Lock( nOffset, nLockSize, &pBuffer, dwFlags );
		if( FAILED( result ) )
		{
			Except( Exception::ERR_RENDER_API_ERROR, (String)"锁定 D3D9 顶点缓存失败。"
				"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
			return NULL;
		}

		return pBuffer;
	}
	//--------------------------------------------------------------------------------------------------------------
	//解锁顶点缓存
	void D3D9VertexBuffer::Unlock()
	{
		HRESULT result = mpD3D9VertexBuffer->Unlock();
		if( FAILED( result ) )
		{
			Except( Exception::ERR_RENDER_API_ERROR, (String)"解除锁定 D3D9 顶点缓存失败。"
				"\nD3D9 错误描述：" + DXGetErrorDescription9( result ) );
		}
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
