/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	GeometryManager
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../../include/RenderWrapper/RenderWrapperInsideHeader.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	GeometryManager::GeometryManager()
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	GeometryManager::~GeometryManager()
	{
		Release();
	}
	//--------------------------------------------------------------------------------------------------------------
	//初始化几何体管理器
	void GeometryManager::Initialize( UINT nGeometryNum, UINT nAddNum )
	{
		mGeometryList.Initialize( nGeometryNum, nAddNum );
	}
	//--------------------------------------------------------------------------------------------------------------
	//释放几何体管理器
	void GeometryManager::Release()
	{
		DeleteAllGeometry();
		mGeometryList.Release();
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建几何体
	Geometry* GeometryManager::CreateGeometry( Geometry::FnCreator fnCreator )
	{
		//创建几何体
		Geometry* pGeometry = fnCreator();
		*mGeometryList.Push() = pGeometry;
		return pGeometry;
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除指定几何体
	void GeometryManager::DeleteGeometry( Geometry* pGeometry )
	{
		GeometryList::Iterator it = mGeometryList.Begin();
		GeometryList::Iterator end = mGeometryList.End();
		for(; it!=end; ++it )
		{
			if( *it == pGeometry )
			{
				mGeometryList.Erase( it );
				delete pGeometry;
				return;
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除所有几何体
	void GeometryManager::DeleteAllGeometry()
	{
		GeometryList::Iterator it = mGeometryList.Begin();
		GeometryList::Iterator end = mGeometryList.End();
		for(; it!=end; ++it )
			SafeDelete( *it );

		mGeometryList.Clear();
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
