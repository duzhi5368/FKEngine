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
*	文件说明：	字符串类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//字符串类
	class FK_DLL String
	{
	public:
		char*			mpCharString;		//字符串指针
		unsigned int	mnCharLength;		//字符串长度

	public:
		String();
		String( const char* pCharString );
		String( const char* pCharString, const unsigned int nLength );
		String( const String& refString );
		String( float value );
		String( int value );
		~String();

		//字符串类赋值
		String& operator = ( const String& refString );

		//字符串赋值
		String& operator = ( const char* pCharString );

		//浮点数赋值
		String& operator = ( float value );

		//整数赋值
		String& operator = ( int value );

		//字符串类和字符串类相等比较
		bool operator == ( const String& refString ) const;

		//字符串类和字符串相等比较
		bool operator == ( const char* pCharString ) const;

		//字符串类和字符串类不相等比较
		bool operator != ( const String& refString ) const;

		//字符串类和字符串不相等比较
		bool operator != ( const char* pCharString ) const;

		//字符串类和字符串类相连
		String operator + ( const String& refString ) const;

		//字符串类和字符串相连
		String operator + ( const char* pCharString ) const;

		//字符串和浮点数转换的字符串相连
		String operator + ( float value ) const;

		//字符串和整数转换的字符串相连
		String operator + ( int value );

		//字符串类和字符串类相连
		String& operator += ( const String& refString );

		//字符串类和字符串相连
		String& operator += ( const char* pCharString );

		//字符串和浮点数转换的字符串相连
		String& operator += ( float value );

		//字符串和整数转换的字符串相连
		String& operator += ( int value );

		//根据字符串设置字符串类
		void SetString( const char* pCharString );

		//根据字符串设置字符串类（传入字符串长度）
		void SetString( const char* pCharString, unsigned int nLength );

		//根据整数设置字符串类
		void SetString( int value );

		//根据浮点数设置字符串类
		void SetString( float value, int nPrecision = 5 );

		//获取字符串指针
		const char* GetCharPtr() const;

		//获取字符串长度
		unsigned int GetLength() const;

		//清除字符串数据
		void Clear();

		//返回第一个指定字符在字符串中的位置（找不到则返回 -1）
		int GetFirstCharIndex( char Symbol ) const;
		
		//返回最后一个指定字符在字符串中的位置（找不到则返回 -1）
		int GetLastCharIndex( char Symbol ) const;

		//根据指定字符切分字符串
		//bool Split( char, StringArray& ) const;

		//将指定位置之间的字符串
		String SubString( unsigned int Start, unsigned int End ) const;

		//从字符串尾部去除指定的字符串
		String& CutString( const String& refString );
		String& CutString( const char* pCharString );

		//在字符串尾部附加字符串
		String& AppendString( const String& refString );
		String& AppendString( const char* pCharString );

		//将字符串转换为大写
		String& ToUpperCase();

		//将字符串转换为小写
		String& ToLowerCase();

		//能否转换为整型
		bool CanToInt() const;

		//能否转换为浮点型
		bool CanToFloat() const;

		//转换为整型
		int ToInt() const;

		//转换为浮点型
		float ToFloat() const;

		//格式化字符串
		void Format( const char* szStr, ... );
	};
	//--------------------------------------------------------------------------------------------------------------
	typedef Stack< String* >	StringList;
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
