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

#include "../../include/RenderSystem/RenderSystemInsideHeader.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	VertexDeclaration::VertexDeclaration()
		: mNumElement	(0)
	{
		mVertexElements.Initialize( 8, 8 );
	}
	//--------------------------------------------------------------------------------------------------------------
	VertexDeclaration::~VertexDeclaration()
	{
		mVertexElements.Release();
	}
	//--------------------------------------------------------------------------------------------------------------
	//添加顶点元素
	void VertexDeclaration::AddElement( WORD nStream, WORD nOffset, VertexElementType eType,
		VertexElementMethod eMethod, VertexElementUsage eUsage, BYTE nUsageIndex )
	{
		//如果为调试模式
#ifdef _DEBUG
		if( mNumElement > 0 )
		{
			//检测顶点流索引、偏移量是否是按照从小到大的顺序排列的
			VertexElement* pBackElement = mVertexElements.Back();

			if( pBackElement->nStream > nStream )
				Except( Exception::ERR_INVALIDPARAMS, "顶点元素顶点流索引值没有按照从小到大的顺序排列。" );

			if( pBackElement->nStream == nStream )
			{
				if( pBackElement->nOffset > nOffset )
					Except( Exception::ERR_INVALIDPARAMS, "同顶点流中顶点元素数据偏移量没有按照从小到大的顺序排列。" );
			}
		}
#endif

		//添加顶点元素
		VertexElement* pElement = mVertexElements.Push();
		pElement->nStream		= nStream;
		pElement->nOffset		= nOffset;
		pElement->Type			= eType;
		pElement->Method		= eMethod;
		pElement->Usage			= eUsage;
		pElement->UsageIndex	= nUsageIndex;

		++mNumElement;
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除指定位置的顶点元素
	void VertexDeclaration::DelElement( UINT nIndex )
	{
		mVertexElements.Erase( nIndex );

		--mNumElement;
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除所有顶点元素
	void VertexDeclaration::DelAllElement()
	{
		mVertexElements.Clear();

		mNumElement = 0;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取指定位置的顶点元素
	VertexElement* VertexDeclaration::GetElement( UINT nIndex )
	{
		if( nIndex >= mNumElement )
			return NULL;

		return mVertexElements.Begin() + nIndex;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取顶点元素数量
	UINT VertexDeclaration::GetElementNum()
	{
		return mNumElement;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
