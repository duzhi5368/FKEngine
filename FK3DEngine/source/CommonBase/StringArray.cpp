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
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../../include/CommonBase/CommonBaseInsideHead.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	StringArray::StringArray()
	{
		mStringList.Initialize( 10, 10 );
	}
	//--------------------------------------------------------------------------------------------------------------
	StringArray::StringArray( const UINT nInitSize, const UINT nAddSize )
	{
		mStringList.Initialize( nInitSize, nAddSize );
	}
	//--------------------------------------------------------------------------------------------------------------
	StringArray::StringArray( const StringArray& refStringArray )
	{
		*this = refStringArray;
	}
	//--------------------------------------------------------------------------------------------------------------
	StringArray::~StringArray()
	{
		Clear();
		mStringList.Release();
	}
	//--------------------------------------------------------------------------------------------------------------
	//字符串数组类赋值
	StringArray& StringArray::operator = ( const StringArray& refStringArray )
	{
		Clear();

		UINT nInitSize = refStringArray.mStringList.Size();
		mStringList.Resize( nInitSize );

		Iterator sit = refStringArray.Begin();
		Iterator send = refStringArray.End();
		Iterator dit = mStringList.Begin();
		for(; sit!=send; ++sit, ++dit )
		{
			dit->mpCharString = NULL;
			dit->mnCharLength = 0;
			*dit = *sit;
		}

		return *this;
	}
	//--------------------------------------------------------------------------------------------------------------
	//字符串数组类和字符串数组类相等比较
	bool StringArray::operator == ( const StringArray& refStringArray ) const
	{
		Iterator sit = refStringArray.Begin();
		Iterator send = refStringArray.End();
		Iterator dit = mStringList.Begin();
		for(; sit!=send; ++sit, ++dit )
		{
			if( *dit != *sit )
				return false;
		}

		return true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//字符串数组类和字符串数组类不相等比较
	bool StringArray::operator != ( const StringArray& refStringArray ) const
	{
		Iterator sit = refStringArray.Begin();
		Iterator send = refStringArray.End();
		Iterator dit = mStringList.Begin();
		for(; sit!=send; ++sit, ++dit )
		{
			if( *dit != *sit )
				return true;
		}

		return false;
	}
	//--------------------------------------------------------------------------------------------------------------
	//字符串数组类和字符串数组类相连
	StringArray StringArray::operator + ( const StringArray& refStringArray ) const
	{
		UINT NumThis = mStringList.Size();
		UINT NumAdd = refStringArray.mStringList.Size();
		UINT NumDst = NumThis + NumAdd;

		//复制本字符串数组到返回字符串数组中
		StringArray DstStringArray( NumDst, 10 );
		DstStringArray = refStringArray;
		
		//将添加的字符串数组中的字符串添加到返回字符串数组中
		Iterator it = refStringArray.Begin();
		Iterator end = refStringArray.End();
		for(; it!=end; ++it )
		{
			DstStringArray.Push( *it );
		}

		return DstStringArray;
	}
	//--------------------------------------------------------------------------------------------------------------
	//字符串数组类和字符串数组类相连
	StringArray& StringArray::operator += ( const StringArray& refStringArray )
	{
		//将添加的字符串数组中的字符串添加本字符串数组中
		Iterator it = refStringArray.Begin();
		Iterator end = refStringArray.End();
		for(; it!=end; ++it )
			Push( *it );

		return *this;
	}
	//--------------------------------------------------------------------------------------------------------------
	//索引访问操作符“[]”
	String& StringArray::operator [] ( int index )
	{
		return mStringList[index];
	}
	//--------------------------------------------------------------------------------------------------------------
	//在尾部增加字符串
	void StringArray::Push( const char* pString )
	{
		String* pStr = mStringList.Push();

		pStr->mpCharString = NULL;
		pStr->mnCharLength = 0;
		*pStr = String( pString );
	}
	//--------------------------------------------------------------------------------------------------------------
	//在尾部增加字符串
	void StringArray::Push( const String& refString )
	{
		String* pStr = mStringList.Push();

		pStr->mpCharString = NULL;
		pStr->mnCharLength = 0;
		*pStr = refString;
	}
	//--------------------------------------------------------------------------------------------------------------
	//在尾部增加字符串
	void StringArray::Push( const char* pCharString, unsigned int nLength )
	{
		String* pStr = mStringList.Push();

		pStr->mpCharString = NULL;
		pStr->mnCharLength = 0;
		*pStr = String( pCharString, nLength );
	}
	//--------------------------------------------------------------------------------------------------------------
	//在尾部弹出字符串
	void StringArray::Pop()
	{
		String* pStr = mStringList.Pop();
		pStr->~String();
	}
	//--------------------------------------------------------------------------------------------------------------
	//在指定位置插入字符串
	void StringArray::Insert( const char* pString, String* ite )
	{
		String* pStr = mStringList.Insert( ite );

		pStr->mpCharString = NULL;
		pStr->mnCharLength = 0;
		*pStr = String( pString );
	}
	//--------------------------------------------------------------------------------------------------------------
	//在指定位置插入字符串
	void StringArray::Insert( const String& refString, String* ite )
	{
		String* pStr = mStringList.Insert( ite );
		
		pStr->mpCharString = NULL;
		pStr->mnCharLength = 0;
		*pStr = refString;
	}
	//--------------------------------------------------------------------------------------------------------------
	//在指定位置插入字符串
	void StringArray::Insert( const char* pCharString, unsigned int nLength, String* ite )
	{
		String* pStr = mStringList.Insert( ite );

		pStr->mpCharString = NULL;
		pStr->mnCharLength = 0;
		*pStr = String( pCharString, nLength );
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除指定位置的字符串
	void StringArray::Erase( String* ite )
	{
		ite->~String();

		mStringList.Erase( ite );
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除所有字符串
	void StringArray::Clear()
	{
		Iterator it = mStringList.Begin();
		Iterator end = mStringList.End();
		for(; it!=end; ++it )
		{
			it->~String();
		}
		mStringList.Clear();
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取字符串首部元素迭代器
	StringArray::Iterator StringArray::Begin() const
	{
		return mStringList.Begin();
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取字符串超尾元素迭代器
	StringArray::Iterator StringArray::End() const
	{
		return mStringList.End();
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取字符串尾部元素迭代器
	StringArray::Iterator StringArray::Back() const
	{
		return mStringList.Back();
	}
	//--------------------------------------------------------------------------------------------------------------
	//返回字符串数量
	UINT StringArray::Size() const
	{
		return mStringList.Size();
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
