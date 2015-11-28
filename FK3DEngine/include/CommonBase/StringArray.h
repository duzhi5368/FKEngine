/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	StringArray
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	字符串数组类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================
#include <windows.h>
//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//字符串数组类
	class FK_DLL StringArray
	{
	public:
		typedef String*		Iterator;

	protected:
		Stack< String >		mStringList;	//字符串列表

	public:
		StringArray();
		StringArray( const UINT nInitSize, const UINT nAddSize );
		StringArray( const StringArray& refStringArray );
		~StringArray();

		//字符串数组类赋值
		StringArray& operator = ( const StringArray& refStringArray );

		//字符串数组类和字符串数组类相等比较
		bool operator == ( const StringArray& refStringArray ) const;

		//字符串数组类和字符串数组类不相等比较
		bool operator != ( const StringArray& refStringArray ) const;

		//字符串数组类和字符串数组类相连
		StringArray operator + ( const StringArray& refStringArray ) const;

		//字符串数组类和字符串数组类相连
		StringArray& operator += ( const StringArray& refStringArray );

		//索引访问操作符“[]”
		String& operator [] ( int index );

		//在尾部增加字符串
		void Push( const char* pString );

		//在尾部增加字符串
		void Push( const String& refString );

		//在尾部增加字符串
		void Push( const char* pCharString, unsigned int nLength );

		//在尾部弹出字符串
		void Pop();

		//在指定位置插入字符串
		void Insert( const char* pString, String* ite );

		//在指定位置插入字符串
		void Insert( const String& refString, String* ite );

		//在指定位置插入字符串
		void Insert( const char* pCharString, unsigned int nLength, String* ite );

		//删除指定位置的字符串
		void Erase( String* ite );

		//删除所有字符串
		void Clear();

		//获取字符串首部元素迭代器
		Iterator Begin() const;

		//获取字符串超尾元素迭代器
		Iterator End() const;

		//获取字符串尾部元素迭代器
		Iterator Back() const;

		//返回字符串数量
		UINT Size() const;
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
