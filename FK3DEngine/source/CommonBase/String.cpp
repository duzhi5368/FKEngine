/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	String
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
	String::String()
		: mpCharString	(NULL)
		, mnCharLength	(0)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	String::String( const char* pCharString )
		: mpCharString	(NULL)
		, mnCharLength	(0)
	{
		*this = pCharString;
	}
	//--------------------------------------------------------------------------------------------------------------
	String::String( const char* pCharString, const unsigned int nLength )
		: mnCharLength	(nLength)
	{
		mpCharString = (char*)malloc( mnCharLength + 1 );
		if( mpCharString == NULL )
			Except( Exception::ERR_NOT_ENOUGH_MEM, "初始化字符串时分配字符串内存失败。" );

		memcpy( mpCharString, pCharString, mnCharLength );

		mpCharString[mnCharLength] = '\0';
	}
	//--------------------------------------------------------------------------------------------------------------
	String::String( const String& refString )
		: mpCharString	(NULL)
		, mnCharLength	(0)
	{
		*this = refString;
	}
	//--------------------------------------------------------------------------------------------------------------
	String::String( float value )
		: mpCharString	(NULL)
		, mnCharLength	(0)
	{
		*this = value;
	}
	//--------------------------------------------------------------------------------------------------------------
	String::String( int value )
		: mpCharString	(NULL)
		, mnCharLength	(0)
	{
		*this = value;
	}
	//--------------------------------------------------------------------------------------------------------------
	String::~String()
	{
		SafeFree( mpCharString );
	}
	//--------------------------------------------------------------------------------------------------------------
	//字符串类赋值
	String& String::operator = ( const String& refString )
	{
		SafeFree( mpCharString );

		mnCharLength = refString.mnCharLength;
		mpCharString = (char*)malloc( mnCharLength + 1 );
		if( mpCharString == NULL )
			Except( Exception::ERR_NOT_ENOUGH_MEM, "初始化字符串时分配字符串内存失败。" );

		memcpy( mpCharString, refString.mpCharString, mnCharLength + 1 );

		return *this;
	}
	//--------------------------------------------------------------------------------------------------------------
	//字符串赋值
	String& String::operator = ( const char* pCharString )
	{
		SafeFree( mpCharString );

		mnCharLength = (unsigned int)strlen( pCharString );

		mpCharString = (char*)malloc( mnCharLength + 1 );
		if( mpCharString == NULL )
			Except( Exception::ERR_NOT_ENOUGH_MEM, "初始化字符串时分配字符串内存失败。" );

		memcpy( mpCharString, pCharString, mnCharLength );

		mpCharString[mnCharLength] = '\0';

		return *this;
	}
	//--------------------------------------------------------------------------------------------------------------
	//浮点数赋值
	String& String::operator = ( float value )
	{
		SetString( value );
		return *this;
	}
	//--------------------------------------------------------------------------------------------------------------
	//整数赋值
	String& String::operator = ( int value )
	{
		SetString( value );
		return *this;
	}
	//--------------------------------------------------------------------------------------------------------------
	//字符串类和字符串类相等比较
	bool String::operator == ( const String& refString ) const
	{
		if( mnCharLength == refString.mnCharLength )
		{
			if( 0 == memcmp( mpCharString, refString.mpCharString, mnCharLength ) )
				return true;
		}

		return false;
	}
	//--------------------------------------------------------------------------------------------------------------
	//字符串类和字符串相等比较
	bool String::operator == ( const char* pCharString ) const
	{
		unsigned int nLength = (unsigned int)strlen( pCharString );

		if( mnCharLength == nLength )
		{
			if( 0 == memcmp( mpCharString, pCharString, mnCharLength ) )
				return true;
		}

		return false;
	}
	//--------------------------------------------------------------------------------------------------------------
	//字符串类和字符串类不相等比较
	bool String::operator != ( const String& refString ) const
	{
		if( mnCharLength == refString.mnCharLength )
		{
			if( 0 == memcmp( mpCharString, refString.mpCharString, mnCharLength ) )
				return false;
		}

		return true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//字符串类和字符串不相等比较
	bool String::operator != ( const char* pCharString ) const
	{
		unsigned int nLength = (unsigned int)strlen( pCharString );

		if( mnCharLength == nLength )
		{
			if( 0 == memcmp( mpCharString, pCharString, mnCharLength ) )
				return false;
		}

		return true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//字符串类和字符串类相连
	String String::operator + ( const String& refString ) const
	{
		String tmpStr;
		tmpStr.mnCharLength = mnCharLength + refString.mnCharLength;

		tmpStr.mpCharString = (char*)malloc( tmpStr.mnCharLength + 1 );
		if( mpCharString == NULL )
			Except( Exception::ERR_NOT_ENOUGH_MEM, "连接字符串时分配字符串内存失败。" );

		memcpy( tmpStr.mpCharString, this->mpCharString, this->mnCharLength );
		memcpy( tmpStr.mpCharString + mnCharLength, refString.mpCharString, refString.mnCharLength );
		tmpStr.mpCharString[ tmpStr.mnCharLength ] = '\0';

		return tmpStr;
	}
	//--------------------------------------------------------------------------------------------------------------
	//字符串类和字符串相连
	String String::operator + ( const char* pCharString ) const
	{
		unsigned int nLength = (unsigned int)strlen( pCharString );

		String tmpStr;
		tmpStr.mnCharLength = mnCharLength + nLength;

		tmpStr.mpCharString = (char*)malloc( tmpStr.mnCharLength + 1 );
		if( mpCharString == NULL )
			Except( Exception::ERR_NOT_ENOUGH_MEM, "连接字符串时分配字符串内存失败。" );

		memcpy( tmpStr.mpCharString, this->mpCharString, this->mnCharLength );
		memcpy( tmpStr.mpCharString + mnCharLength, pCharString, nLength );
		tmpStr.mpCharString[ tmpStr.mnCharLength ] = '\0';

		return tmpStr;
	}
	//--------------------------------------------------------------------------------------------------------------
	//字符串和浮点数转换的字符串相连
	String String::operator + ( float value ) const
	{
		return *this + String( value );
	}
	//--------------------------------------------------------------------------------------------------------------
	//字符串和整数转换的字符串相连
	String String::operator + ( int value )
	{
		return *this + String( value );
	}
	//--------------------------------------------------------------------------------------------------------------
	//字符串类和字符串类相连
	String& String::operator += ( const String& refString )
	{
		unsigned int newCharLength = mnCharLength + refString.mnCharLength;

		char* newCharString = (char*)malloc( newCharLength + 1 );
		if( newCharString == NULL )
			Except( Exception::ERR_NOT_ENOUGH_MEM, "连接字符串时分配字符串内存失败。" );

		memcpy( newCharString, this->mpCharString, this->mnCharLength );
		memcpy( newCharString + mnCharLength, refString.mpCharString, refString.mnCharLength );
		newCharString[ newCharLength ] = '\0';

		SafeFree( mpCharString );
		mpCharString = newCharString;
		mnCharLength = newCharLength;

		return *this;
	}
	//--------------------------------------------------------------------------------------------------------------
	//字符串类和字符串相连
	String& String::operator += ( const char* pCharString )
	{
		unsigned int nLength = (unsigned int)strlen( pCharString );

		unsigned int newCharLength = mnCharLength + nLength;

		char* newCharString = (char*)malloc( newCharLength + 1 );
		if( newCharString == NULL )
			Except( Exception::ERR_NOT_ENOUGH_MEM, "连接字符串时分配字符串内存失败。" );

		memcpy( newCharString, this->mpCharString, this->mnCharLength );
		memcpy( newCharString + mnCharLength, pCharString, nLength );
		newCharString[ newCharLength ] = '\0';

		SafeFree( mpCharString );
		mpCharString = newCharString;
		mnCharLength = newCharLength;

		return *this;
	}
	//--------------------------------------------------------------------------------------------------------------
	//字符串和浮点数转换的字符串相连
	String& String::operator += ( float value )
	{
		*this += String( value );
		return *this; 
	}
	//--------------------------------------------------------------------------------------------------------------
	//字符串和整数转换的字符串相连
	String& String::operator += ( int value )
	{
		*this += String( value );
		return *this;
	}
	//--------------------------------------------------------------------------------------------------------------
	//根据字符串设置字符串类
	void String::SetString( const char* pCharString )
	{
		*this = pCharString;
	}
	//--------------------------------------------------------------------------------------------------------------
	//根据字符串设置字符串类（传入字符串长度）
	void String::SetString( const char* pCharString, unsigned int nLength )
	{
		SafeFree( mpCharString );

		mnCharLength = nLength;

		mpCharString = (char*)malloc( mnCharLength + 1 );
		if( mpCharString == NULL )
			Except( Exception::ERR_NOT_ENOUGH_MEM, "初始化字符串时分配字符串内存失败。" );

		memcpy( mpCharString, pCharString, mnCharLength );

		mpCharString[mnCharLength] = '\0';
	}
	//--------------------------------------------------------------------------------------------------------------
	//根据整数设置字符串类
	void String::SetString( int value )
	{
		char szBuf[ NumConvert::mMaxIntTextLen ] = { NULL };
		unsigned int nLen = NumConvert::IntToMultiByte( value, szBuf );
		SetString( szBuf, nLen );
	}
	//--------------------------------------------------------------------------------------------------------------
	//根据浮点数设置字符串类
	void String::SetString( float value, int nPrecision )
	{
		char szBuf[ NumConvert::mMaxFloatTextLen ] = { NULL };
		unsigned int nLen = NumConvert::FloatToMultiByte( value, szBuf, nPrecision );
		SetString( szBuf, nLen );
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取字符串指针
	const char* String::GetCharPtr() const
	{
		return mpCharString;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取字符串长度
	unsigned int String::GetLength() const
	{
		return mnCharLength;
	}
	//--------------------------------------------------------------------------------------------------------------
	//清除字符串数据
	void String::Clear()
	{
		SafeFree( mpCharString );
		mnCharLength = 0;
	}
	//--------------------------------------------------------------------------------------------------------------
	//返回第一个指定字符在字符串中的位置
	int String::GetFirstCharIndex( char Symbol ) const
	{
		char* pStr = mpCharString;
		for( unsigned int i=0; i<mnCharLength; ++i )
		{
			if( *pStr++ == Symbol )
				return (int)i;
		}

		return -1;
	}
	//--------------------------------------------------------------------------------------------------------------
	//返回最后一个指定字符在字符串中的位置
	int String::GetLastCharIndex( char Symbol ) const
	{
		char* pStr = mpCharString + mnCharLength;
		for( unsigned int i=mnCharLength; i>0; --i )
		{
			if( *(--pStr) == Symbol )
				return (int)i;
		}

		return -1;
	}
	//--------------------------------------------------------------------------------------------------------------
	//根据指定字符切分字符串
	//bool String::Split( char, StringArray& ) const
	//{
	//}
	//--------------------------------------------------------------------------------------------------------------
	//将指定位置之间的字符串
	String String::SubString( unsigned int Start, unsigned int End ) const
	{
		return String( mpCharString + Start, End - Start );
	}
	//--------------------------------------------------------------------------------------------------------------
	//从字符串尾部去除指定的字符串
	String& String::CutString( const String& refString )
	{
		char* pChar = strstr( mpCharString, refString.mpCharString );
		if( pChar != NULL )
			*pChar = '\0';

		return *this;
	}
	//--------------------------------------------------------------------------------------------------------------
	String& String::CutString( const char* pCharString )
	{
		char* pChar = strstr( mpCharString, pCharString );
		if( pChar != NULL )
			*pChar = '\0';

		return *this;
	}
	//--------------------------------------------------------------------------------------------------------------
	//在字符串尾部附加字符串
	String& String::AppendString( const String& refString )
	{
		unsigned int newLen = mnCharLength + refString.mnCharLength;
		char* newStr = (char*)malloc( newLen + 1 );

		memcpy( newStr, mpCharString, mnCharLength );
		memcpy( newStr + mnCharLength, refString.mpCharString, refString.mnCharLength + 1 );

		free( mpCharString );

		mpCharString = newStr;
		mnCharLength = newLen;

		return *this;
	}
	//--------------------------------------------------------------------------------------------------------------
	String& String::AppendString( const char* pCharString )
	{
		unsigned int AppendLen = (unsigned int)strlen( pCharString );

		unsigned int newLen = mnCharLength + AppendLen;
		char* newStr = (char*)malloc( newLen + 1 );

		memcpy( newStr, mpCharString, mnCharLength );
		memcpy( newStr + mnCharLength, pCharString, AppendLen + 1 );

		free( mpCharString );
		mpCharString = newStr;
		mnCharLength = newLen;

		return *this;
	}
	//--------------------------------------------------------------------------------------------------------------
	//将字符串转换为大写
	String& String::ToUpperCase()
	{
		strupr( mpCharString );
		return *this;
	}
	//--------------------------------------------------------------------------------------------------------------
	//将字符串转换为小写
	String& String::ToLowerCase()
	{
		strlwr( mpCharString );
		return *this;
	}
	//--------------------------------------------------------------------------------------------------------------
	//能否转换为整型
	bool String::CanToInt() const
	{
		return NumConvert::CanMultiByteToInt( mpCharString );
	}
	//--------------------------------------------------------------------------------------------------------------
	//能否转换为浮点型
	bool String::CanToFloat() const
	{
		return NumConvert::CanMultiByteToFloat( mpCharString );
	}
	//--------------------------------------------------------------------------------------------------------------
	//转换为整型
	int String::ToInt() const
	{
		return NumConvert::MultiByteToInt( mpCharString );
	}
	//--------------------------------------------------------------------------------------------------------------
	//转换为双精度浮点型
	float String::ToFloat() const
	{
		return NumConvert::MultiByteToFloat( mpCharString );
	}
	//--------------------------------------------------------------------------------------------------------------
	//格式化字符串
	void String::Format( const char* szStr, ... )
	{
		//获取参数列表
		va_list args;
		va_start( args, szStr );

		//获取格式化所需的字符串长度
		mnCharLength = _vscprintf( szStr, args );

		//调整字符串缓存大小
		SafeFree( mpCharString );

		mpCharString = (char*)malloc( mnCharLength + 1 );
		if( mpCharString == NULL )
			Except( Exception::ERR_NOT_ENOUGH_MEM, "格式化字符串时分配字符串内存失败。" );

		//格式化字符串
		vsprintf( mpCharString, szStr, args );
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
