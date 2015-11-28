/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	BufferManager
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
	BufferManager::BufferManager()
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	BufferManager::~BufferManager()
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	//初始化缓存管理器
	void BufferManager::Initialize()
	{
		mVBList.Initialize( 300, 200 );
		mIBList.Initialize( 150, 100 );
		mVDList.Initialize( 150, 100 );
	}
	//--------------------------------------------------------------------------------------------------------------
	//释放缓存管理器
	void BufferManager::Release()
	{
		DelAllVertexBuffer();
		DelAllIndexBuffer();
		DelAllVertexDeclaration();

		mVBList.Release();
		mIBList.Release();
		mVDList.Release();
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除顶点缓存
	void BufferManager::DelVertexBuffer( VertexBuffer* pVB )
	{
		VertexBufferList::Iterator it = mVBList.Begin();
		VertexBufferList::Iterator end = mVBList.End();
		for(; it!=end; ++it )
		{
			if( *it == pVB )
			{
				mVBList.Erase( it );
				delete pVB;
				return;
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除索引缓存
	void BufferManager::DelIndexBuffer( IndexBuffer* pIB )
	{
		IndexBufferList::Iterator it = mIBList.Begin();
		IndexBufferList::Iterator end = mIBList.End();
		for(; it!=end; ++it )
		{
			if( *it == pIB )
			{
				mIBList.Erase( it );
				delete pIB;
				return;
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除顶点声明
	void BufferManager::DelVertexDeclaration( VertexDeclaration* pVD )
	{
		VertexDeclarationList::Iterator it = mVDList.Begin();
		VertexDeclarationList::Iterator end = mVDList.End();
		for(; it!=end; ++it )
		{
			if( *it == pVD )
			{
				mVDList.Erase( it );
				delete pVD;
				return;
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除所有顶点缓存
	void BufferManager::DelAllVertexBuffer()
	{
		UINT NumVB = mVBList.Size();
		VertexBuffer** ppVB = mVBList.Begin();
		for( UINT i=0; i<NumVB; ++i )
		{
			SafeDelete( *ppVB++ );
		}

		mVBList.Clear();
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除所有索引缓存
	void BufferManager::DelAllIndexBuffer()
	{
		UINT NumIB = mIBList.Size();
		IndexBuffer** ppIB = mIBList.Begin();
		for( UINT i=0; i<NumIB; ++i )
		{
			SafeDelete( *ppIB++ );
		}

		mIBList.Clear();
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除所有顶点声明
	void BufferManager::DelAllVertexDeclaration()
	{
		UINT NumVD = mVDList.Size();
		VertexDeclaration** ppVD = mVDList.Begin();
		for( UINT i=0; i<NumVD; ++i )
		{
			SafeDelete( *ppVD++ );
		}

		mVDList.Clear();
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取顶点缓存数量
	UINT BufferManager::GetNumVertexBuffer()
	{
		return mVBList.Size();
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取索引缓存数量
	UINT BufferManager::GetNumIndexBuffer()
	{
		return mIBList.Size();
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取顶点声明数量
	UINT BufferManager::GetNumVertexDeclaration()
	{
		return mVDList.Size();
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
