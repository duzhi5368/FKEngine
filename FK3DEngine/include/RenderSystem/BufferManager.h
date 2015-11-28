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

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//缓存管理器
	class FK_DLL BufferManager : public Singleton< BufferManager >
	{
	protected:
		VertexBufferList		mVBList;	//顶点缓存列表
		IndexBufferList			mIBList;	//索引缓存列表
		VertexDeclarationList	mVDList;	//顶点声明列表

	public:
		BufferManager();
		virtual ~BufferManager();

		//初始化缓存管理器
		void Initialize();

		//释放缓存管理器
		void Release();

		//创建顶点缓存
		virtual VertexBuffer* AddVertexBuffer() = 0;

		//创建索引缓存
		virtual IndexBuffer* AddIndexBuffer() = 0;

		//创建顶点声明
		virtual VertexDeclaration* AddVertexDeclaration() = 0;

		//删除顶点缓存
		void DelVertexBuffer( VertexBuffer* pVB );

		//删除索引缓存
		void DelIndexBuffer( IndexBuffer* pIB );

		//删除顶点声明
		void DelVertexDeclaration( VertexDeclaration* pVD );

		//删除所有顶点缓存
		void DelAllVertexBuffer();

		//删除所有索引缓存
		void DelAllIndexBuffer();

		//删除所有顶点声明
		void DelAllVertexDeclaration();

		//获取顶点缓存数量
		UINT GetNumVertexBuffer();

		//获取索引缓存数量
		UINT GetNumIndexBuffer();

		//获取顶点声明数量
		UINT GetNumVertexDeclaration();
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
