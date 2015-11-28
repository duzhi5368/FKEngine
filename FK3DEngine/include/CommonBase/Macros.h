/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Macros
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	常规宏定义
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================
//常用宏定义
#define FKExport					__declspec(dllexport)		// DLL 导出
#define FKImport					__declspec(dllimport)		// DLL 导入
#define FKFastcall					__fastcall					//快速函数调用规则
#define FKInline					__inline					//请求内联函数
#define FKForceInline				__forceinline				//强制内联函数
#define FKAlign16					__declspec(align(16))		//数据 16 字节对齐
#define FKNaked						__declspec(naked)			//不允许编译器增加函数初始化和结束代码


//如果在包含该头文件前声明 FK_DLL_EXPORT 即为 DLL 导出，声明 FK_DLL_IMPORT 即为 DLL 导入，不声明则不进行 DLL 导出、导入。
#ifdef FK_DLL_EXPORT
	#define FK_DLL		FKExport
#else
	#ifdef FK_DLL_IMPORT
		#define FK_DLL		FKImport
	#else
		#define FK_DLL
	#endif
#endif


//内存管理宏
#define SafeFree( Pointer )			{ if( Pointer != NULL )		{ free( Pointer ); Pointer = NULL; } }
#define SafeDelete( Pointer )		{ if( Pointer != NULL )		{ delete Pointer; Pointer = NULL; } }
#define SafeDeleteArray( Array )	{ if( Array != NULL )		{ delete [] Array; Array = NULL; } }
#define SafeRelease( Interface )	{ if( Interface != NULL )	{ Interface->Release(); Interface = NULL; } }


//根据是否为调试模式将一些内存释放宏映射到不同的宏
#ifdef _DEBUG
	#define FKFree( Pointer )			SafeFree( Pointer )
	#define FKDelete( Pointer )			SafeDelete( Pointer )
	#define FKDeleteArray( Array )		SafeDeleteArray( Array )
	#define FKRelease( Interface )		SafeRelease( Interface )
#else
	#define FKFree( Pointer )			{ free( Pointer ); }
	#define FKDelete( Pointer )			{ delete Pointer; }
	#define FKDeleteArray( Array )		{ delete [] Array; }
	#define FKRelease( Interface )		{ Interface->Release(); }
#endif


#define FK_MAX( a, b )						( ( (a) > (b) ) ? (a) : (b) )	//最大值宏
#define FK_MIN( a, b )						( ( (a) < (b) ) ? (a) : (b) )	//最小值宏

// FOURCC 生成宏
#define FK_FOURCC( ch1, ch2, ch3, ch4 )		((DWORD)(BYTE)(ch1) | ((DWORD)(BYTE)(ch2) << 8) | \
											((DWORD)(BYTE)(ch3) << 16) | ((DWORD)(BYTE)(ch4) << 24 ))
