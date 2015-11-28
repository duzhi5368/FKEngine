/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	FKEngineInclude
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "../include/System.h"
#include "../include/Audio.h"
#include "../include/Bitmap.h"
#include "../include/Color.h"
#include "../include/Directories.h"
#include "../include/Font.h"
#include "../include/Graphics.h"
#include "../include/Image.h"
#include "../include/ImageData.h"
#include "../include/Input.h"
#include "../include/Inspection.h"
#include "../include/IO.h"
#include "../include/Math.h"
#include "../include/Platform.h"
#include "../include/Text.h"
#include "../include/TextInput.h"
#include "../include/Timing.h"
#include "../include/StringConversion.h"
#include "../include/FKWindow.h"
#include "../include/FKVersion.h"
#include "../include/Timer.h"
#include "../include/MsgListenerManager.h"
#include "../include/LogManager.h"
#include "../include/Exception.h"
#include "../include/FKSharedPtr.h"
#include "../include/ZPos.h"
#include "../include/Singleton.h"
#include "../include/Archive.h"
#include "../include/ArchiveFactory.h"
#include "../include/ArchiveManager.h"
#include "../include/DataStream.h"
#include "../include/ZipArchive.h"
#include "../include/ZipArchiveFactory.h"
#include "../include/ZipDataStream.h"
#include "../include/FileDataStream.h"
#include "../include/FileSystemArchive.h"
#include "../include/FileSystemArchiveFactory.h"
#include "../Include/UIInputManager.h"
#include "../Include/UIControl.h"
#include "../Include/UserMsgNotify.h"
#include "../Include/UIManager.h"
#include "../Include/WinControl.h"
#include "../Include/Panel.h"
#include "../Include/Form.h"
#include "../Include/PicButton.h"
#include "../Include/Button.h"
#include "../Include/CheckBox.h"
#include "../Include/RadioButton.h"
#include "../Include/Label.h"
#include "../Include/ImageControl.h"
#include "../include/ScrollBar.h"
#include "../include/Edit.h"
#include "../include/AnimationCursorManager.h"
#include "../include/IniFile.h"

//--------------------------------------------------------------------

//! 修改控制台类型窗口入口函数
#ifdef USE_FK_CONSOLE

#else
	#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
#endif
//--------------------------------------------------------------------
//! 设置为1，堆栈展开代码将被编译到库中，在异常抛出时会显示调用堆栈
#ifndef FK_STACK_UNWINDING
#define FK_STACK_UNWINDING 1
#endif
//--------------------------------------------------------------------
//! 断言模式
/*
0 - Debug模式使用标准断言，Release模式没有任何作用
1 - Debug模式使用标准断言，Release模式使用异常机制
2 - Debug模式使用异常机制，Release模式使用异常机制
*/
#ifndef FK_ASSERT_MODE
#define FK_ASSERT_MODE 1 
#endif
//--------------------------------------------------------------------

using namespace FK2DEngine;