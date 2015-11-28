/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	IndexBuffer
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
	//索引缓存类
	class FK_DLL IndexBuffer
	{
	public:
		UINT		mnBufLen;		//索引缓存大小
		IndexType	mIndexType;		//索引数据类型
		UINT		mIndexSize;		//单个索引值数据长度
		bool		mbDynamic;		//是否为动态顶索引缓存

	public:
		IndexBuffer();
		virtual ~IndexBuffer() = 0;

		//创建索引缓存
		virtual void CreateIndexBuffer( UINT nIndexNum, IndexType eIndexType, bool bDynamic = false, bool bSoftProc = false ) = 0;

		//释放索引缓存
		virtual void ReleaseIndexBuffer() = 0;

		//锁定索引缓存
		virtual void* Lock( UINT nOffsetNum, UINT nLockNum, bool bNoOverwrite = false ) = 0;

		//解锁索引缓存
		virtual void Unlock() = 0;

		//写入数据到索引缓存
		void WriteData( UINT nOffsetNum, void* pData, UINT nIndexNum, bool bNoOverwrite = false );
	};
	//--------------------------------------------------------------------------------------------------------------
	typedef Stack< IndexBuffer* >	IndexBufferList;
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
