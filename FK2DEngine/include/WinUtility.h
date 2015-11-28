/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	WinUtility
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include <Windows.h>
#include "Platform.h"
#include "../../depend/boost/function.hpp"
#include "../../depend/boost/shared_ptr.hpp"
#include <string>
//--------------------------------------------------------------------
namespace FK2DEngine
{
	namespace Win
	{
		//! 返回这个应用程序实例
		HINSTANCE	Instance();

		void		HandleMessage();

		void		ProcessMessage();

		void		RegisterMessageHook( const boost::function< bool (MSG&) >& p_Hook );

		FK_DECL_NORETURN void ThrowLastError( const std::string& p_Action = "" );

		template< typename T >
		inline T Check( T p_ValueToCheck, const std::string& p_Action = "" )
		{
			if( !p_ValueToCheck )
			{
				ThrowLastError( p_Action );
			}
			return p_ValueToCheck;
		}

		template< typename T >
		void ReleaseComPtr( T* p_Ptr )
		{
			p_Ptr->Release();
		}

		//! 
		template< typename T >
		inline boost::shared_ptr< T > ShareComPtr( T* p_Ptr )
		{
			return boost::shared_ptr< T >( p_Ptr, ReleaseComPtr< T > );
		}

		//! 获取执行程序的文件名
		std::wstring AppFileName();

		//! 获取执行程序所在的目录
		std::wstring AppDirectory();
	}
}