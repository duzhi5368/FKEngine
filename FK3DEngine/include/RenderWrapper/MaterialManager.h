/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	MaterialManager
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	材质管理器
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//材质管理器
	class FK_DLL MaterialManager : public Singleton< MaterialManager >
	{
	protected:
		MaterialMap			mMaterialMap;		//材质数据索引哈希表
		MaterialList		mMaterialList;		//材质数据索引堆栈

	public:
		MaterialManager();
		~MaterialManager();

		//初始化材质管理器
		void Initialize( UINT nNumMaterial );

		//释放材质管理器
		void Release();

		//创建新的材质
		Material* CreateMaterial( const char* szMaterialName );

		//根据纹理创建纹理材质
		Material* CreateMaterial( const char* szTexFileName, MaterialType eType );

		//根据材质数据创建新的材质
		Material* CreateMaterial( const MaterialData* pMaterialData );

		//删除原有材质
		void DeleteMaterial( Material* pMaterial );

		//删除所有材质
		void DeleteAllMaterial();

		//根据名称获取材质
		FKInline Material* GetMaterial( const char* szMaterialName );

		//获取材质数量
		FKInline UINT GetMaterialNum();

		//根据材质类型选择最适当的纹理格式
		PixelFormat GetBestTextureFormat( MaterialType eMatType, PixelFormat eOrgPixelFormat );
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
