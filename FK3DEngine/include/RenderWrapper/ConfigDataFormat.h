/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	ConfigDataFormat
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	配置信息数据格式
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

#include "../RenderSystem/TypeDefine.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	#pragma pack ( push )
	#pragma pack ( 4 )
	//--------------------------------------------------------------------------------------------------------------

	//配置文件头标识
	const char szConfigFileHeader[] = "FK Config File";

	//--------------------------------------------------------------------------------------------------------------

	//输入系统类型
	enum InputSystemType
	{
		IST_DirectInput8	= 0,
	};

	//声音系统类型
	enum SoundSystemType
	{
		SST_DirectSound8	= 0,
	};

	//渲染系统类型
	enum RenderSystemType
	{
		RST_Direct3D9		= 0,
		RST_OpenGL			= 1,
	};

	//--------------------------------------------------------------------------------------------------------------

	//窗口配置
	struct WindowCfg
	{
		char		szCaption[64];		//窗口标题
		bool		bCenter;			//是否窗口居中
		UINT		nLeft;				//左边距
		UINT		nTop;				//上边距
		UINT		nWidth;				//宽度
		UINT		nHeight;			//高度
		DWORD		dwStyle;			//窗口风格
	};

	//--------------------------------------------------------------------------------------------------------------

	//输入系统配置
	struct InputSystemCfg
	{
		InputSystemType	eInputSystemType;	//输入系统类型
		bool			bExclusiveKeyboard;	//是否使用独占键盘输入
		bool			bExclusiveMouse;	//是否使用独占鼠标输入
	};

	//--------------------------------------------------------------------------------------------------------------

	//扬声器类型
	enum SpeakerType
	{
		Speaker_Unknown		= 0,		//未知（忽略扬声器配置）
		Speaker_Headphone	= 1,		//手提听筒
		Speaker_Mono		= 2,		//单声道
		Speaker_Stereo		= 3,		//双声道
		Speaker_Quad		= 4,		//四声道
		Speaker_Surround	= 5,		//环绕立体声
		Speaker_5Point1		= 6,		//5.1 声道
		Speaker_7Point1		= 7,		//7.1 声道
	};
	
	//--------------------------------------------------------------------------------------------------------------

	//声音系统配置
	struct SoundSystemCfg
	{
		SoundSystemType	eSoundSystemType;	//声音系统类型
		SpeakerType		eSpeakerType;		//扬声器类型

		DWORD			dwChannels;			//声音播放声道数
		DWORD			dwFrequency;		//声音播放频率
		DWORD			dwBitRate;			//声音播放比特率
	};

	//--------------------------------------------------------------------------------------------------------------

	// D3D9 渲染系统配置
	struct D3D9Cfg
	{
		int					nDeviceType;			//设备类型
		UINT				nAdapter;				//显示适配器序号
		UINT				nWidth;					//全屏模式显示宽度
		UINT				nHeight;				//全屏模式显示高度
		bool				bVsync;					//是否开启垂直同步
		UINT				nNumBackBuffer;			//缓存页面数量
		PixelFormat			eColorFormat;			//显示颜色格式
		UINT				nRefreshRate;			//显示刷新率
		int					nMultiSampleType;		//多重取样类型
		DWORD				dwMultiSampleQuality;	//多重取样质量

		BYTE				nVS_MajorVer;			//顶点着色器主版本
		BYTE				nVS_MinorVer;			//顶点着色器次版本
		BYTE				nPS_MajorVer;			//像素着色器主版本
		BYTE				nPS_MinorVer;			//像素着色器次版本
	};

	//--------------------------------------------------------------------------------------------------------------

	// OpenGL 渲染系统配置
	struct OpenGLCfg
	{
	};

	//--------------------------------------------------------------------------------------------------------------

	//渲染系统配置
	struct RenderSystemCfg
	{
		RenderSystemType	eRenderSystemType;	//渲染系统类型
		bool				bWindowed;			//是否全屏显示

		union
		{
			D3D9Cfg		sD3D9Cfg;		// D3D9 渲染系统配置
			OpenGLCfg	sOpenGLCfg;		// OpenGL 渲染系统配置
		};
	};

	//--------------------------------------------------------------------------------------------------------------

	//配置信息数据结构
	struct ConfigData
	{
		char				szConfigName[64];	//配置名称
		WindowCfg			sWindowCfg;			//窗口配置

		InputSystemCfg		sInputSystemCfg;	//输入系统配置
		SoundSystemCfg		sSoundSystemCfg;	//声音系统配置
		RenderSystemCfg		sRenderSystemCfg;	//渲染系统配置
	};
	typedef Stack< ConfigData >		ConfigDataStack;

	//--------------------------------------------------------------------------------------------------------------
	#pragma pack ( pop )
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
