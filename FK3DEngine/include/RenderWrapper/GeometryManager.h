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
*	文件说明：	几何体管理器
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//几何体管理器
	class FK_DLL GeometryManager : public Singleton< GeometryManager >
	{
	public:
		GeometryList		mGeometryList;		//几何体列表

	public:
		GeometryManager();
		~GeometryManager();

		//初始化几何体管理器
		void Initialize( UINT nGeometryNum, UINT nAddNum );

		//释放几何体管理器
		void Release();

		//创建几何体
		Geometry* CreateGeometry( Geometry::FnCreator fnCreator );

		//删除指定几何体
		void DeleteGeometry( Geometry* pGeometry );

		//删除所有几何体
		void DeleteAllGeometry();
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
