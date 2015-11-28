/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Platform
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include <algorithm>
//--------------------------------------------------------------------
#ifdef __BIG_ENDIAN__
# define FK_BIG_ENDIAN
# define IDENTITY_FUN bigToNative
# define IDENTITY_FUN2 nativeToBig
# define CONV_FUN littleToNative
# define CONV_FUN2 nativeToLittle
#else
# define FK_LITTLE_ENDIAN
# define IDENTITY_FUN littleToNative
# define IDENTITY_FUN2 nativeToLittle
# define CONV_FUN bigToNative
# define CONV_FUN2 nativeToBig
#endif

namespace FK2DEngine
{
	template< typename T >
	T IDENTITY_FUN( T t )
	{
		return t;
	}

	template< typename T >
	T IDENTITY_FUN2( T t )
	{
		return t;
	}

	template< typename T >
	T CONV_FUN( T t )
	{
		char* pBegin = reinterpret_cast< char* >(&t);
		std::reverse( pBegin, pBegin + sizeof( t ) );
		return t;
	}

	template< typename T >
	T CONV_FUN2( T t )
	{
		return CONV_FUN( t );
	}
}

#undef IDENTITY_FUN
#undef IDENTITY_FUN2
#undef CONV_FUN
#undef CONV_FUN2

// __declspec(noreturn)有两个作用。
// 1：当希望一个函数没有返回值的时候，标识之。可以免除警告。
// 2：当自定义throw抛出时，抛出函数标识之，可保证throw函数后代码不再执行。
#if defined(_MSC_VER)
# define FK_DECL_NORETURN __declspec(noreturn)
#elif defined(__GNUC__)
# define FK_DECL_NORETURN __attribute__ ((noreturn))
#endif

#if defined(WIN32)
# define FK_WINDOWS
#else
# define FK_UNIX
# if defined(__linux) || defined(__FreeBSD__)
#  define FK_X
# else
#  define FK_MAC
#  include <TargetConditionals.h>
#  if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
#   define FK_IPHONE
#  endif
# endif
#endif

//  __declspec(deprecated) 指定函数存储数据段
#if defined(FK_WINDOWS)
# define FK_DEPRECATED __declspec(deprecated)
#elif (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1))
# define FK_DEPRECATED  __attribute__((__deprecated__))
#else
# define FK_DEPRECATED 
#endif