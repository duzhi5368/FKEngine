/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	AppEntry
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	程序入口代码 头文件
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================
#include <iostream>
#include "MemoryManager.h"
//==================================================================================================================

#ifndef _WINDOWS
#error 只有 Windows 程序才允许编译该入口代码
#endif

//==================================================================================================================

// C 运行时库函数声明
extern "C" int __cdecl _heap_init( int );
extern "C" int __cdecl _mtinit();
extern "C" int __cdecl _ioinit();
extern "C" int __cdecl _cinit( int );
extern "C" _CRTIMP void __cdecl _cexit();
extern "C" void __cdecl __crtExitProcess( int );

//应用程序入口函数
extern int WinMainEntry();

//==================================================================================================================

#ifndef _FK_WIDECHAR_
	extern "C" int __cdecl main( int, char**, char** )
	{
		return 0;
	}
#else
	extern "C" int __cdecl wmain( int, wchar_t**, wchar_t** )
	{
		return 0;
	}
#endif

//==================================================================================================================

//显示入口程序错误消息
void ShowEntryErrMsg( const char* szErrMsg )
{
	MessageBox( GetActiveWindow(), szErrMsg, "入口程序错误", MB_OK | MB_ICONERROR | MB_TASKMODAL );
}

//==================================================================================================================

//系统入口函数
#ifdef _FK_WIDECHAR_
	extern "C" int wWinMainCRTStartup()
#else
	extern "C" int WinMainCRTStartup()
#endif

{
	int nResult;

	//如果需要输出内存管理日志，则首先记录程序运行路径。
#if FK_MEMORY_MANAGER == FK_DEBUG_MEMORY_MANAGER
	char szMemLogPath[ MAX_PATH+1 ] = { NULL };
	GetCurrentDirectory( MAX_PATH+1, szMemLogPath );
	strcat_s( szMemLogPath, "\\FKMemLog.log" );
#endif

	//初始化程序堆内存
#ifdef _MT
	if( !_heap_init(1) )
#else  /* _MT */
	if( !_heap_init(0) )
#endif /* _MT */
	{
		ShowEntryErrMsg( "初始化程序堆内存（Heap）错误。" );
		return -1;
	}

	//初始化多线程管理程序
#ifdef _MT
	if( !_mtinit() )
	{
		ShowEntryErrMsg( "初始化多线程管理程序错误。" );
		return -1;
	}
#endif  /* _MT */

	if( _ioinit() < 0 )
	{
		ShowEntryErrMsg( "初始化 IO 管理程序错误。" );
		return -1;
	}

	//初始化内存管理器
	if( !MemoryManager::sMemoryManager._Initialize() )
	{
		ShowEntryErrMsg( "初始化内存管理器错误。" );
		return -1;
	}

	//初始化全局数据
	if( 0 != _cinit( TRUE ) )
	{
		ShowEntryErrMsg( "初始化全局数据错误。" );
		return -1;
	}

	//调用应用程序入口函数
	nResult = WinMainEntry();

	//释放全局数据
	_cexit();

	//释放内存管理器
	//写入内存管理日志
#if FK_MEMORY_MANAGER == FK_DEBUG_MEMORY_MANAGER
	MemoryManager::sMemoryManager.DumpMemoryLogFile( szMemLogPath );
#endif

	MemoryManager::sMemoryManager._Release();

	//终止进程
	__crtExitProcess( nResult );

	return nResult;
}

//==================================================================================================================
