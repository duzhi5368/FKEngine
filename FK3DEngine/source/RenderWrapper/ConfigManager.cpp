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

	//默认配置信息
	ConfigData ConfigManager::mDefConfigData;

	//--------------------------------------------------------------------------------------------------------------
	ConfigManager::ConfigManager()
	{
		//初始化配置信息堆栈
		mConfigDataStack.Initialize( 2, 2 );

		//设置默认配置信息
		strcpy( mDefConfigData.szConfigName, "Default Config" );

		//窗口配置
		strcpy( mDefConfigData.sWindowCfg.szCaption, "Free Knight Game Engine Default Window" );
		mDefConfigData.sWindowCfg.bCenter = true;
		mDefConfigData.sWindowCfg.nLeft = 0;
		mDefConfigData.sWindowCfg.nTop = 0;
		mDefConfigData.sWindowCfg.nWidth = 800;
		mDefConfigData.sWindowCfg.nHeight = 600;
		mDefConfigData.sWindowCfg.dwStyle = WS_DLGFRAME;

		//输入系统配置
		mDefConfigData.sInputSystemCfg.eInputSystemType		= IST_DirectInput8;
#ifdef _DEBUG
		mDefConfigData.sInputSystemCfg.bExclusiveKeyboard	= false;
		mDefConfigData.sInputSystemCfg.bExclusiveMouse		= false;
#else
		mDefConfigData.sInputSystemCfg.bExclusiveKeyboard	= true;
		mDefConfigData.sInputSystemCfg.bExclusiveMouse		= true;
#endif

		//声音系统配置
		mDefConfigData.sSoundSystemCfg.eSoundSystemType		= SST_DirectSound8;
		mDefConfigData.sSoundSystemCfg.eSpeakerType			= Speaker_Stereo;

		mDefConfigData.sSoundSystemCfg.dwChannels			= 2;
		mDefConfigData.sSoundSystemCfg.dwFrequency			= 44100;
		mDefConfigData.sSoundSystemCfg.dwBitRate			= 16;

		//渲染系统配置
		mDefConfigData.sRenderSystemCfg.eRenderSystemType = RST_Direct3D9;
		mDefConfigData.sRenderSystemCfg.bWindowed = false;

		//默认使用 D3D9 渲染系统
		mDefConfigData.sRenderSystemCfg.sD3D9Cfg.nDeviceType			= (int)D3DDEVTYPE_HAL;
		mDefConfigData.sRenderSystemCfg.sD3D9Cfg.nAdapter				= 0;
		mDefConfigData.sRenderSystemCfg.sD3D9Cfg.nWidth					= 1024;
		mDefConfigData.sRenderSystemCfg.sD3D9Cfg.nHeight				= 768;
		mDefConfigData.sRenderSystemCfg.sD3D9Cfg.bVsync					= true;
		mDefConfigData.sRenderSystemCfg.sD3D9Cfg.nNumBackBuffer			= 2;
		mDefConfigData.sRenderSystemCfg.sD3D9Cfg.eColorFormat			= PF_X8R8G8B8;
		mDefConfigData.sRenderSystemCfg.sD3D9Cfg.nRefreshRate			= 60;
		mDefConfigData.sRenderSystemCfg.sD3D9Cfg.nMultiSampleType		= (int)D3DMULTISAMPLE_NONE;
		mDefConfigData.sRenderSystemCfg.sD3D9Cfg.dwMultiSampleQuality	= 0;

		mDefConfigData.sRenderSystemCfg.sD3D9Cfg.nVS_MajorVer	= 0;	//顶点着色器主版本
		mDefConfigData.sRenderSystemCfg.sD3D9Cfg.nVS_MinorVer	= 0;	//顶点着色器次版本
		mDefConfigData.sRenderSystemCfg.sD3D9Cfg.nPS_MajorVer	= 0;	//像素着色器主版本
		mDefConfigData.sRenderSystemCfg.sD3D9Cfg.nPS_MinorVer	= 0;	//像素着色器次版本
	}
	//--------------------------------------------------------------------------------------------------------------
	ConfigManager::~ConfigManager()
	{
		//释放初始化配置信息堆栈
		mConfigDataStack.Release();
	}
	//--------------------------------------------------------------------------------------------------------------
	//从指定的配置文件数据中载入配置信息
	void ConfigManager::LoadConfigFileData( DataChunk* pConfigFileData )
	{
		//设置读取指针到文件头
		pConfigFileData->SetPosition( 0 );

		//验证文件头
		char szFileHeader[ sizeof(szConfigFileHeader) ];
		pConfigFileData->Read( szFileHeader, sizeof(szConfigFileHeader) );
		if( 0 != memcmp( szConfigFileHeader, szFileHeader, sizeof(szConfigFileHeader) ) )
			Except( Exception::ERR_INTERNAL_ERROR, "非法的配置文件数据。" );

		//读取配置信息数量
		UINT nNumConfig = 0;
		pConfigFileData->Read( &nNumConfig, sizeof(UINT) );

		//读取所有配置信息
		ConfigData* pConfigData = mConfigDataStack.Push( nNumConfig );
		pConfigFileData->Read( pConfigData, sizeof(ConfigData) * nNumConfig );
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建一个配置信息
	void ConfigManager::AddConfig( ConfigData* pConfigData )
	{
		ConfigData* pDst = mConfigDataStack.Push();
		memcpy( pDst, pConfigData, sizeof(ConfigData) );
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除一个配置信息
	void ConfigManager::DelConfig( UINT nIndex )
	{
		mConfigDataStack.Erase( nIndex );
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除所有配置信息
	void ConfigManager::DelAllConfig()
	{
		mConfigDataStack.Clear();
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取默认配置信息
	void ConfigManager::GetDefaultConfig( ConfigData* pConfigData )
	{
		if( pConfigData == NULL )
			Except( Exception::ERR_INVALIDPARAMS, "传入的配置信息指针为空，无法获取默认配置信息。" );

		memcpy( pConfigData, &mDefConfigData, sizeof(ConfigData) );
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取指定的配置信息
	ConfigData* ConfigManager::GetConfig( UINT nIndex )
	{
		if( nIndex >= mConfigDataStack.Size() )
			return NULL;

		return mConfigDataStack.Begin() + nIndex;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置当前配置信息
	void ConfigManager::SetCurrenConfig( UINT nIndex )
	{
		if( nIndex >= mConfigDataStack.Size() )
			Except( Exception::ERR_INVALIDPARAMS, "指定的配置信息不存在。" );

		mCurConfigIndex = nIndex;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取当前配置信息
	ConfigData* ConfigManager::GetCurrenConfig()
	{
		return GetConfig( mCurConfigIndex );
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
