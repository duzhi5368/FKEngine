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
*	文件说明：	D3D9 顶点缓存类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//D3D9 顶点缓存类
	class FK_DLL D3D9VertexBuffer : public VertexBuffer
	{
	public:
		IDirect3DVertexBuffer9*		mpD3D9VertexBuffer;

	public:
		D3D9VertexBuffer();
		~D3D9VertexBuffer();

		//创建顶点缓存
		void CreateVertexBuffer( UINT nVerSize, UINT nVerNum, bool bDynamic = false, bool bSoftProc = false );

		//释放顶点缓存
		void ReleaseVertexBuffer();

		//锁定顶点缓存
		void* Lock( UINT nOffsetVer, UINT nLockVer, bool bNoOverwrite = false );

		//解锁顶点缓存
		void Unlock();
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
