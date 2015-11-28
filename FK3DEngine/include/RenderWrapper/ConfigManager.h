/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	ConfigManager
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	配置信息管理类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//配置信息管理类
	class FK_DLL ConfigManager : public Singleton< ConfigManager >
	{
	public:
		static ConfigData	mDefConfigData;		//默认配置信息

	protected:
		ConfigDataStack		mConfigDataStack;	//配置信息堆栈
		UINT				mCurConfigIndex;	//当前配置信息索引

	public:
		ConfigManager();
		~ConfigManager();

		//从指定的配置文件数据中载入配置信息
		void LoadConfigFileData( DataChunk* pConfigFileData );

		//创建一个配置信息
		void AddConfig( ConfigData* pConfigData );

		//删除一个配置信息
		void DelConfig( UINT nIndex );

		//删除所有配置信息
		void DelAllConfig();

		//获取默认配置信息
		void GetDefaultConfig( ConfigData* pConfigData );

		//获取指定的配置信息
		ConfigData* GetConfig( UINT nIndex );

		//设置当前配置信息
		void SetCurrenConfig( UINT nIndex );

		//获取当前配置信息
		ConfigData* GetCurrenConfig();
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
