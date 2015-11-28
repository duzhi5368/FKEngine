/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	MemoryManager
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	内存管理器
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

#define FK_DEBUG_MEMORY_MANAGER			0x10000
#define FK_RELEASE_MEMORY_MANAGER		0x20000

//==================================================================================================================

#include "Misc.h"
#include "FileObject.h"
#include "TextFile.h"

//==================================================================================================================

//下面的声明定义是否使用调式模式的内存管理器
#ifdef _DEBUG
	#define FK_MEMORY_MANAGER		FK_DEBUG_MEMORY_MANAGER
	#pragma message ( "调试模式内存管理器已被启用" )
#else
	#define FK_MEMORY_MANAGER		FK_RELEASE_MEMORY_MANAGER
	#pragma message ( "发布模式内存管理器已被启用" )
#endif

//==================================================================================================================
//如果定义了使用内存管理器
#ifdef FK_MEMORY_MANAGER
//==================================================================================================================


//==================================================================================================================
//如果为调试模式内存管理器
#if FK_MEMORY_MANAGER == FK_DEBUG_MEMORY_MANAGER
//==================================================================================================================


//==================================================================================================================
namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//调试模式内存管理映射函数
	//--------------------------------------------------------------------------------------------------------------

	extern FK_DLL FKInline void* FK_Malloc( size_t MemSize, const char* FileName, const char* Function, const int Line );
	extern FK_DLL FKInline void* FK_Realloc( void* pOldMem, size_t MemSize, const char* FileName, const char* Function, const int Line );
	extern FK_DLL FKInline void FK_Free( void* pMem, const char* FileName, const char* Function, const int Line );

	//--------------------------------------------------------------------------------------------------------------

	static const char memBegin[]	= "MEM_BEGIN";	//内存块起始标志
	static const char memEnd[]		= "MEM_END";	//内存块结束标志

	//--------------------------------------------------------------------------------------------------------------

	//内存块信息表大小
	#define FK_MEM_BLOCK_INFO_NUM		65535
	#define FK_MEM_BLOCK_INFO_PTR_SIZE	FK_MEM_BLOCK_INFO_NUM * sizeof(BlockInfo*)

	//--------------------------------------------------------------------------------------------------------------
	//调试模式内存管理器
	class FK_DLL MemoryManager
	{
	public:
		static MemoryManager	sMemoryManager;	//内存管理器

	protected:
		//内存块信息结构
		struct BlockInfo
		{
			void*		pMem;		//内存块指针

			size_t		nActSize;	//实际分配大小
			size_t		nSize;		//用户请求大小

			char*		szFileName;	//分配所在文件
			char*		szFunName;	//分配所在函数
			int			nLine;		//分配所在行号

			BlockInfo*	pPrevInfo;	//上一个内存块信息的指针
			BlockInfo*	pNextInfo;	//下一个内存块信息的指针
		};

		char*					mpTmpStr;		//格式化消息缓存

		HANDLE					mhInstHeap;		//该程序实例堆栈句柄

		HANDLE					mhInfoHeap;		//内存信息存储堆栈句柄
		CRITICAL_SECTION		mInfoAccessFlag;//内存信息访问标志

		UINT					mnBlockCount;	//内存块数量
		BlockInfo**				mppBlockInfo;	//内存块信息指针列表

		bool					mbCheckMemLeak;	//是否监视内存泄漏

	public:
		//初始化内存管理器
		bool _Initialize();

		//释放内存管理器
		void _Release();

		//C 标准内存分配
		void* Malloc( size_t MemSize, const char* FileName, const char* Function, const int Line );
	
		//C 标准内存重分配
		void* Realloc( void* pOldMem, size_t MemSize, const char* FileName, const char* Function, const int Line );

		//C 标准内存释放
		void Free( void* pMem, const char* FileName, const char* Function, const int Line );
	
		//输出内存管理日志
		void DumpMemoryLogFile( const char* szFileName );

	protected:
		//显示内存访问溢出消息
		void _ShowOverflowMsg( const BlockInfo* pInfo, bool HeadOverflow, bool RearOverflow );
	
		//无内存管理格式化消息
		char* _FormatMsg( int* pLen, const char* msg, ... );

	};
	//--------------------------------------------------------------------------------------------------------------
}
//==================================================================================================================


//==================================================================================================================
//如果为发布模式内存管理器
#else if FK_MEMORY_MANAGER == FK_RELEASE_MEMORY_MANAGER
//==================================================================================================================


//==================================================================================================================
namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//发布模式内存管理映射函数
	//--------------------------------------------------------------------------------------------------------------

	extern FK_DLL FKInline void* FK_Malloc( size_t MemSize );
	extern FK_DLL FKInline void* FK_Realloc( void* pOldMem, size_t MemSize );
	extern FK_DLL FKInline void FK_Free( void* pMem );

	//--------------------------------------------------------------------------------------------------------------
	//发布模式内存管理器
	class FK_DLL MemoryManager
	{
	public:
		static MemoryManager	sMemoryManager;	//内存管理器

	protected:
		static HANDLE			mhInstHeap;		//该程序实例堆栈句柄

	public:
		//初始化内存管理器
		bool _Initialize();

		//释放内存管理器
		void _Release();

		//C 标准内存分配
		void* Malloc( size_t MemSize );

		//C 标准内存重分配
		void* Realloc( void* pOldMem, size_t MemSize );

		//C 标准内存释放
		void Free( void* pMem );
	};
	//--------------------------------------------------------------------------------------------------------------
}
//==================================================================================================================


//==================================================================================================================
#endif	/* #if FK_MEMORY_MANAGER == FK_DEBUG_MEMORY_MANAGER */
//==================================================================================================================

//==================================================================================================================
#endif	/* #ifdef FK_MEMORY_MANAGER */
//==================================================================================================================
