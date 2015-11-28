/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	VertexBuffer
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
	//顶点缓存类
	class FK_DLL VertexBuffer
	{
	public:
		UINT	mnBufLen;		//顶点缓存大小
		UINT	mVerSize;		//每个顶点的大小
		bool	mbDynamic;		//是否为动态顶点缓存
		bool	mbSoftProc;		//是否为软件顶点处理

	public:
		VertexBuffer();
		virtual ~VertexBuffer() = 0;

		//创建顶点缓存
		virtual void CreateVertexBuffer( UINT nVerSize, UINT nVerNum, bool bDynamic = false, bool bSoftProc = false ) = 0;

		//释放顶点缓存
		virtual void ReleaseVertexBuffer() = 0;

		//锁定顶点缓存
		virtual void* Lock( UINT nOffsetVer, UINT nLockVer, bool bNoOverwrite = false ) = 0;

		//解锁顶点缓存
		virtual void Unlock() = 0;

		//写入数据到顶点缓存
		void WriteData( UINT nOffsetVer, void* pData, UINT nVerNum, bool bNoOverwrite = false );
	};
	//--------------------------------------------------------------------------------------------------------------
	typedef Stack< VertexBuffer* >	VertexBufferList;
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
