/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	VertexDeclaration
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
	//顶点元素描述
	struct VertexElement
	{
		WORD				nStream;	//顶点流索引号
		WORD				nOffset;	//该顶点元素在顶点数据中的偏移量
		VertexElementType	Type;		//数据类型（长度）
		VertexElementMethod	Method;		//光栅化方法
		VertexElementUsage	Usage;		//顶点用途
		BYTE				UsageIndex;	//用途索引
	};
	typedef Stack< VertexElement >	VertexElementList;
	//--------------------------------------------------------------------------------------------------------------
	//顶点声明类
	class FK_DLL VertexDeclaration
	{
	protected:
		UINT				mNumElement;		//顶点元素数量
		VertexElementList	mVertexElements;	//顶点元素列表

	public:
		VertexDeclaration();
		virtual ~VertexDeclaration();

		//添加顶点元素
		void AddElement( WORD nStream, WORD nOffset, VertexElementType eType, VertexElementMethod eMethod,
			VertexElementUsage eUsage, BYTE nUsageIndex );

		//删除指定位置的顶点元素
		void DelElement( UINT nIndex );

		//删除所有顶点元素
		void DelAllElement();

		//获取指定位置的顶点元素
		VertexElement* GetElement( UINT nIndex );

		//获取顶点元素数量
		UINT GetElementNum();

		//创建顶点声明
		virtual void CreateVertexDeclaration() = 0;

		//删除顶点声明
		virtual void ReleaseVertexDeclaration() = 0;
	};
	//--------------------------------------------------------------------------------------------------------------
	typedef Stack< VertexDeclaration* >	VertexDeclarationList;
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
