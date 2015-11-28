/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	FactoryObject
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include <xstring>
//--------------------------------------------------------------------
namespace FK2DEngine
{
	//! 抽象工厂类
	template< typename T >
	class TFactoryObject
	{
	public:
		virtual ~TFactoryObject()
		{

		}

		//! 返回工厂类型
		virtual const std::wstring& GetType() const = 0;

		//! 创建一个对象
		virtual T* CreateInstance( const std::wstring& p_szName ) = 0;

		//! 销毁本工厂创建的一个对象
		virtual void DestroyInstance( T* p_pPtr ) = 0;
	};
}