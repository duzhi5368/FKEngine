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
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../../include/CommonBase/MemoryManager.h"
//#include "../../include/CommonBase/NoMemoryMarco.h"
#include <stdio.h>


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

	//内存管理器
	MemoryManager	MemoryManager::sMemoryManager;

	//--------------------------------------------------------------------------------------------------------------
	//调试模式内存管理映射函数
	//--------------------------------------------------------------------------------------------------------------
	FKInline void* FK_Malloc( size_t MemSize, const char* FileName, const char* Function, const int Line )
	{
		return MemoryManager::sMemoryManager.Malloc( MemSize, FileName, Function, Line );
	}
	//--------------------------------------------------------------------------------------------------------------
	FKInline void* FK_Realloc( void* pOldMem, size_t MemSize, const char* FileName, const char* Function, const int Line )
	{
		return MemoryManager::sMemoryManager.Realloc( pOldMem, MemSize, FileName, Function, Line );
	}
	//--------------------------------------------------------------------------------------------------------------
	FKInline void FK_Free( void* pMem, const char* FileName, const char* Function, const int Line )
	{
		MemoryManager::sMemoryManager.Free( pMem, FileName, Function, Line );
	}
	//--------------------------------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------------------------------
	//内存管理器函数
	//--------------------------------------------------------------------------------------------------------------
	//初始化内存管理器
	bool MemoryManager::_Initialize()
	{
		//获取该程序实例堆栈句柄
		mhInstHeap = GetProcessHeap();
		if( mhInstHeap == NULL )
			return false;

		//创建内存信息数据存储堆栈
		mhInfoHeap = HeapCreate( 0, FK_MEM_BLOCK_INFO_PTR_SIZE * 4, 0 );
		if( mhInfoHeap == NULL )
			return false;

		//初始化内存信息数据访问标志
		InitializeCriticalSection( &mInfoAccessFlag );

		//分配内存信息数据存储内存
		EnterCriticalSection( &mInfoAccessFlag );
		mppBlockInfo = (BlockInfo**)HeapAlloc( mhInfoHeap, HEAP_ZERO_MEMORY, FK_MEM_BLOCK_INFO_PTR_SIZE );
		LeaveCriticalSection( &mInfoAccessFlag );

		if( mppBlockInfo == NULL )
			return false;

		mnBlockCount = 0;
		mbCheckMemLeak = true;

		return true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//释放内存管理器
	void MemoryManager::_Release()
	{
		//输出内存管理日志
		SafeFree( mpTmpStr );

		//开始内存信息数据访问
		EnterCriticalSection( &mInfoAccessFlag );

		//释放所有内存块信息
		UINT CheckedCount = 0;
		BlockInfo** ppInfo = mppBlockInfo;
		for( UINT i=0; i<FK_MEM_BLOCK_INFO_NUM; ++i )
		{
			BlockInfo* pInfo = *ppInfo;

			//如果该内存块未被释放
			while( pInfo != NULL )
			{
				pInfo = pInfo->pNextInfo;
				++CheckedCount;

				HeapFree( mhInfoHeap, 0, pInfo );
			}

			if( CheckedCount > i )
				break;

			++ppInfo;
		}

		//释放内存信息数据存储内存
		HeapFree( mhInfoHeap, 0, mppBlockInfo );
		mppBlockInfo = NULL;

		//结束内存信息数据访问
		LeaveCriticalSection( &mInfoAccessFlag );

		//删除内存信息数据访问标志
		DeleteCriticalSection( &mInfoAccessFlag );

		//删除内存信息数据存储堆栈
		HeapDestroy( mhInfoHeap );
	}
	//--------------------------------------------------------------------------------------------------------------
	//C 标准内存分配
	void* MemoryManager::Malloc( size_t MemSize, const char* FileName, const char* Function, const int Line )
	{
		if( MemSize == 0 )
			return NULL;

		size_t ActSize = MemSize + 16;	//内存块实际使用内存
		void* pMem;

		//开始内存信息数据访问
		EnterCriticalSection( &mInfoAccessFlag );

		//分配内存
		pMem = HeapAlloc( mhInstHeap, 0, ActSize );
		if( pMem == NULL )
		{
			LeaveCriticalSection( &mInfoAccessFlag );
			return NULL;
		}

		//设置该内存块头尾溢出标志
		memcpy( pMem, memBegin, 9 );
		memcpy( (char*)pMem + ActSize - 7, memEnd, 7 );


		//如果允许监视内存泄漏
		if( mbCheckMemLeak )
		{
			//计算该内存块位置
			size_t BlockNum = ( (size_t)pMem >> 4 ) & ( FK_MEM_BLOCK_INFO_NUM - 1 );

			//分配内存块内存
			BlockInfo* pBlockInfo = mppBlockInfo[BlockNum];
			BlockInfo* pNewInfo = (BlockInfo*)HeapAlloc( mhInfoHeap, HEAP_ZERO_MEMORY, sizeof(BlockInfo) );

			if( pBlockInfo != NULL )
			{
				if( pBlockInfo->pNextInfo != NULL )
				{
					pNewInfo->pNextInfo = pBlockInfo->pNextInfo;
					pBlockInfo->pNextInfo->pPrevInfo = pNewInfo;
				}

				pBlockInfo->pNextInfo = pNewInfo;
				pNewInfo->pPrevInfo = pBlockInfo;
			}
			else
			{
				mppBlockInfo[BlockNum] = pNewInfo;
			}

			//设置内存块信息
			pNewInfo->pMem			= pMem;
			pNewInfo->nActSize		= ActSize;
			pNewInfo->nSize			= MemSize;
			pNewInfo->szFileName	= (char*)FileName;
			pNewInfo->szFunName		= (char*)Function;
			pNewInfo->nLine			= Line;

			++mnBlockCount;
		}

		//结束内存信息数据访问
		LeaveCriticalSection( &mInfoAccessFlag );

		return (char*)pMem + 9;
	}
	//--------------------------------------------------------------------------------------------------------------
	//C 标准内存重分配
	void* MemoryManager::Realloc( void* pOldMem, size_t MemSize, const char* FileName,
		const char* Function, const int Line )
	{
		if( MemSize == 0 || pOldMem == NULL )
			return NULL;

		pOldMem = (void*)( (char*)pOldMem - 9 );

		size_t ActSize = MemSize + 16;	//内存块实际使用内存

		//开始内存信息数据访问
		EnterCriticalSection( &mInfoAccessFlag );

		//如果允许监视内存泄漏
		if( mbCheckMemLeak )
		{
			//计算该旧内存块位置
			size_t OldBlockNum = ( (size_t)pOldMem >> 4 ) & ( FK_MEM_BLOCK_INFO_NUM - 1 );

			//寻找旧内存信息位置
			BlockInfo* pOldInfo = mppBlockInfo[OldBlockNum];

			//如果该位置已经被重复记录
			while( pOldInfo->pMem != pOldMem )
			{
				if( pOldInfo->pNextInfo == NULL )
				{
					Misc::ShowErrMsg( "找不到指定的需要再分配的内存块。" );
					LeaveCriticalSection( &mInfoAccessFlag );
					return NULL;
				}

				pOldInfo = pOldInfo->pNextInfo;
			}

			//将该内存信息从该位置的信息列表中删除
			if( pOldInfo->pNextInfo != NULL )
			{
				if( pOldInfo->pPrevInfo != NULL )
				{
					pOldInfo->pNextInfo->pPrevInfo = pOldInfo->pPrevInfo;
					pOldInfo->pPrevInfo->pNextInfo = pOldInfo->pNextInfo;
				}
				else
				{
					pOldInfo->pNextInfo->pPrevInfo = NULL;
					mppBlockInfo[OldBlockNum] = pOldInfo->pNextInfo;
				}
			}
			else if( pOldInfo->pPrevInfo == NULL )
			{
				mppBlockInfo[OldBlockNum] = NULL;
			}

			//删除该位置内存信息
			HeapFree( mhInfoHeap, 0, pOldInfo );
		}


		//分配内存
		void* pMem = HeapReAlloc( mhInstHeap, 0, pOldMem, ActSize );
		if( pMem == NULL )
		{
			LeaveCriticalSection( &mInfoAccessFlag );
			return NULL;
		}

		//设置该内存块头尾溢出标志
		memcpy( pMem, memBegin, 9 );
		memcpy( (char*)pMem + ActSize - 7, memEnd, 7 );


		//如果允许监视内存泄漏
		if( mbCheckMemLeak )
		{
			//计算该内存块位置
			size_t BlockNum = ( (size_t)pMem >> 4 ) & ( FK_MEM_BLOCK_INFO_NUM - 1 );

			//分配内存块内存
			BlockInfo* pBlockInfo = mppBlockInfo[BlockNum];
			BlockInfo* pNewInfo = (BlockInfo*)HeapAlloc( mhInfoHeap, HEAP_ZERO_MEMORY, sizeof(BlockInfo) );

			if( pBlockInfo != NULL )
			{
				pBlockInfo->pNextInfo = pNewInfo;
				pNewInfo->pPrevInfo = pBlockInfo;
			}
			else
			{
				mppBlockInfo[BlockNum] = pNewInfo;
			}

			//设置内存块信息
			pNewInfo->pMem			= pMem;
			pNewInfo->nActSize		= ActSize;
			pNewInfo->nSize			= MemSize;
			pNewInfo->szFileName	= (char*)FileName;
			pNewInfo->szFunName		= (char*)Function;
			pNewInfo->nLine			= Line;
		}


		//结束内存信息数据访问
		LeaveCriticalSection( &mInfoAccessFlag );

		return (char*)pMem + 9;
	}
	//--------------------------------------------------------------------------------------------------------------
	//C 标准内存释放
	void MemoryManager::Free( void* pMem, const char* FileName, const char* Function, const int Line )
	{
		if( pMem == NULL )
			return;

		pMem = (void*)( (char*)pMem - 9 );

		//开始内存信息数据访问
		EnterCriticalSection( &mInfoAccessFlag );

		//如果允许监视内存泄漏
		if( mbCheckMemLeak )
		{
			//计算该旧内存块位置
			size_t BlockNum = ( (size_t)pMem >> 4 ) & ( FK_MEM_BLOCK_INFO_NUM - 1 );

			//寻找旧内存信息位置
			BlockInfo* pInfo = mppBlockInfo[BlockNum];
			if( pInfo == NULL )
			{
				Misc::ShowErrMsg( "找不到指定的需要释放的内存块。" );
				goto FreeMem;
			}

			//如果该位置已经被重复记录
			while( pInfo->pMem != pMem )
			{
				if( pInfo->pNextInfo == NULL )
				{
					Misc::ShowErrMsg( "找不到指定的需要释放的内存块。" );
					goto FreeMem;
				}

				pInfo = pInfo->pNextInfo;
			}

			//检查内存访问溢出
			bool HeadOverflow = ( 0 != memcmp( pMem, memBegin, 9 ) ) ? true : false;
			bool RearOverflow = ( 0 != memcmp( (char*)pMem + pInfo->nActSize - 7, memEnd, 7 ) ) ? true : false;
			if( HeadOverflow || RearOverflow )
				_ShowOverflowMsg( pInfo, HeadOverflow, RearOverflow );

			//将该内存信息从该位置的信息列表中删除
			if( pInfo->pNextInfo != NULL )
			{
				if( pInfo->pPrevInfo != NULL )
				{
					pInfo->pNextInfo->pPrevInfo = pInfo->pPrevInfo;
					pInfo->pPrevInfo->pNextInfo = pInfo->pNextInfo;
				}
				else
				{
					pInfo->pNextInfo->pPrevInfo = NULL;
					mppBlockInfo[BlockNum] = pInfo->pNextInfo;
				}
			}
			else
			{
				if( pInfo->pPrevInfo != NULL )
					pInfo->pPrevInfo->pNextInfo = NULL;
				else
					mppBlockInfo[BlockNum] = NULL;
			}

			//删除该位置内存信息
			HeapValidate( mhInfoHeap, 0, pInfo );
			HeapFree( mhInfoHeap, 0, pInfo );

			--mnBlockCount;
		}

FreeMem:
		//删除该内存块
		HeapValidate( mhInstHeap, 0, pMem );
		HeapFree( mhInstHeap, 0, pMem );

		//结束内存信息数据访问
		LeaveCriticalSection( &mInfoAccessFlag );
	}
	//--------------------------------------------------------------------------------------------------------------
	//输出内存管理日志
	void MemoryManager::DumpMemoryLogFile( const char* szFileName )
	{
		//首先关闭内存泄漏检测
		mbCheckMemLeak = false;

		//创建日志文件
		TextFile File( szFileName, OT_Write );

		//获取日期和时间
		char szDate[20] = { NULL };
		GetDateFormat( LOCALE_SYSTEM_DEFAULT, NULL, NULL, "yyyy 年 MM 月 dd 日", szDate, sizeof(szDate) );

		char szTime[18] = { NULL };
		GetTimeFormat( LOCALE_SYSTEM_DEFAULT, NULL, NULL, "HH 时 mm 分 ss 秒", szTime, sizeof(szTime) );

		//写入日志头
		int StrLen = 0;
		char* pStr = _FormatMsg( &StrLen,
			"--------------------------------------------------------------------------------------------\n"
			"-                               自由骑士3D游戏引擎内存管理日志                             -\n"
			"--------------------------------------------------------------------------------------------\n"
			"\n日期：%s\n时间：%s\n\n", szDate, szTime );
		File.Write( pStr, StrLen );

		//如果不存在任何内存泄漏
		if( mnBlockCount == 0 )
		{
			const char szNoOverflow[] = "\n恭喜您，程序没有发生任何的内存泄漏！\n";
			File.Write( szNoOverflow, sizeof(szNoOverflow) );
		}
		else
		{
			pStr = _FormatMsg( &StrLen, "\n程序总共存在 %d 处内存泄漏。\n\n", mnBlockCount );
			File.Write( pStr, StrLen );

			//遍历所有记录信息检查内存泄漏
			UINT CheckedCount = 0;
			BlockInfo** ppInfo = mppBlockInfo;
			for( UINT i=0; i<FK_MEM_BLOCK_INFO_NUM; ++i )
			{
				BlockInfo* pInfo = *ppInfo;

				//如果该内存块未被释放
				while( pInfo != NULL )
				{
					pStr = _FormatMsg( &StrLen,
						"\n内存地址： 0x%X\n"
						"实际大小： %d\n"
						"有效大小： %d\n"
						"分配文件： %s\n"
						"分配函数： %s\n"
						"分配行号： %d\n",
						pInfo->pMem, pInfo->nActSize, pInfo->nSize, pInfo->szFileName, pInfo->szFunName, pInfo->nLine );

					File.Write( pStr, StrLen );

					pInfo = pInfo->pNextInfo;
					++CheckedCount;
				}

				if( CheckedCount > i )
					break;

				++ppInfo;
			}
		}

		File.Close();

		//开启内存泄漏检测
		mbCheckMemLeak = true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//显示内存访问溢出消息
	void MemoryManager::_ShowOverflowMsg( const BlockInfo* pInfo, bool HeadOverflow, bool RearOverflow )
	{
		char* pOverflowDesc = NULL;
		if( HeadOverflow && RearOverflow )
			pOverflowDesc = "首部和尾部";
		else if( HeadOverflow )
			pOverflowDesc = "首部";
		else if( RearOverflow )
			pOverflowDesc = "尾部";
		else
			return;

		char* pStr = _FormatMsg( NULL,
			"内存块%s访问溢出！\n\n"
			"内存地址： 0x%X\n"
			"实际大小： %d\n"
			"有效大小： %d\n"
			"分配文件： %s\n"
			"分配函数： %s\n"
			"分配行号： %d",
			pOverflowDesc,
			pInfo->pMem, pInfo->nActSize, pInfo->nSize, pInfo->szFileName, pInfo->szFunName, pInfo->nLine );
		MessageBox( GetActiveWindow(), pStr, "错误", MB_OK | MB_ICONERROR );
	}
	//--------------------------------------------------------------------------------------------------------------
	//无内存管理格式化消息
	char* MemoryManager::_FormatMsg( int* pLen, const char* msg, ... )
	{
		//获取参数列表
		va_list args;
		va_start( args, msg );

		//获取格式化显示消息所需的字符串长度（还需要一个 NULL 结尾字符空间）
		int len = _vscprintf( msg, args ) + 1;

		//释放上一次使用的字符串内存
		SafeFree( mpTmpStr );

		//分配字符串内存
		mpTmpStr = (char*)malloc( len );
		if( mpTmpStr == NULL )
		{
			MessageBox( GetActiveWindow(), "调用格式化消息函数时，分配格式化信息字符串的内存失败。", "错误", MB_OK );
			return NULL;
		}

		//格式化并显示消息
		vsprintf( mpTmpStr, msg, args );

		if( pLen != NULL )
			*pLen = len;

		return mpTmpStr;
	}
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

	//--------------------------------------------------------------------------------------------------------------
	//发布模式内存管理映射函数
	//--------------------------------------------------------------------------------------------------------------
	FKInline void* FK_Malloc( size_t MemSize )
	{
		return MemoryManager::sMemoryManager.Malloc( MemSize );
	}
	//--------------------------------------------------------------------------------------------------------------
	FKInline void* FK_Realloc( void* pOldMem, size_t MemSize )
	{
		return MemoryManager::sMemoryManager.Realloc( pOldMem, MemSize );
	}
	//--------------------------------------------------------------------------------------------------------------
	FKInline void FK_Free( void* pMem )
	{
		MemoryManager::sMemoryManager.Free( pMem );
	}
	//--------------------------------------------------------------------------------------------------------------

	
	//--------------------------------------------------------------------------------------------------------------

	MemoryManager	MemoryManager::sMemoryManager;	//内存管理器
	HANDLE			MemoryManager::mhInstHeap;		//该程序实例堆栈句柄

	//--------------------------------------------------------------------------------------------------------------
	//内存管理器函数
	//--------------------------------------------------------------------------------------------------------------
	//初始化内存管理器
	bool MemoryManager::_Initialize()
	{
		//获取该程序实例堆栈句柄
		mhInstHeap = GetProcessHeap();
		if( mhInstHeap == NULL )
			return false;

		return true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//释放内存管理器
	void MemoryManager::_Release()
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	//C 标准内存分配
	void* MemoryManager::Malloc( size_t MemSize )
	{
		if( MemSize == 0 )
			return NULL;

		//分配内存
		return HeapAlloc( mhInstHeap, 0, MemSize );
	}
	//--------------------------------------------------------------------------------------------------------------
	//C 标准内存重分配
	void* MemoryManager::Realloc( void* pOldMem, size_t MemSize )
	{
		if( MemSize == 0 || pOldMem == NULL )
			return NULL;

		//分配内存
		return HeapReAlloc( mhInstHeap, 0, pOldMem, MemSize );
	}
	//--------------------------------------------------------------------------------------------------------------
	//C 标准内存释放
	void MemoryManager::Free( void* pMem )
	{
		//删除该内存块
		HeapValidate( mhInstHeap, 0, pMem );
		HeapFree( mhInstHeap, 0, pMem );
	}
	//--------------------------------------------------------------------------------------------------------------
}
//==================================================================================================================


//==================================================================================================================
#endif	/* #if FK_MEMORY_MANAGER == FK_DEBUG_MEMORY_MANAGER */
//==================================================================================================================

//==================================================================================================================
#endif	/* #ifdef FK_MEMORY_MANAGER */
//==================================================================================================================

#include "../../include/CommonBase/MemoryMarco.h"