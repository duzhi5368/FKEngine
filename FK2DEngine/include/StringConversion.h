/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	StringConversion
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include <string>
#include <vector>
#include "FKSharedPtr.h"
//--------------------------------------------------------------------

namespace FK2DEngine
{
	typedef std::vector< std::wstring >		StringVector;		// 字符串向量类型
	typedef TSharedPtr< StringVector >		StringVectorPtr;	// 字符串向量智能指针类型
	typedef std::wostringstream				StrStreamType;		// 字符流

	//! 转换 std::string 为 std::wstring
	std::wstring UTF8ToWstring( const std::string& p_UTF8 );

	//! 转换 std::wstring 为 std::string
	std::string WstringToUTF8( const std::wstring& p_WString );

	//! 本地解码方式 转换 std::string 为 std::wstring
	std::wstring Widen( const std::string& p_UTF8 );

	//! 本地解码方式 转换 std::wstring 为 std::string
	std::string Narrow( const std::wstring& p_WString );

	//! 删除字符串头尾空格，回车，Tab
	//@param [IN] p_bLeft	是否删除字符串头的空格，回车，Tab
	//@param [IN] p_bRight	是否删除字符串尾的空格，回车，Tab
	void Trim( std::wstring& p_Str, bool p_bLeft = true, bool p_bRight = true );

	//! 将一个字符串分割为多个字符串单元，保存在字符串向量中
	//@param [IN] p_Delims	进行分割的字符组成的字符串,默认为("\t\n\r ")
	//@param [IN] p_unMaxSplits 最大分割次数
	StringVector Split( const std::wstring& p_Str, std::wstring& p_Delims,
		unsigned int p_unMaxSplits = 0 );

	//! 将字符串中所有大写字母该为小写字母
	void ToLowerCase( std::wstring& p_Str );

	//! 将字符串中所有小写字母该为大写字母
	void ToUpperCase( std::wstring& p_Str );

	//! 判断字符串是否以指定字符串模式开头
	//@param [IN] p_Pattern	参考的指定字符串
	//@param [IN] p_bCaseSensitive 是否大小写敏感
	bool IsStartWith( const std::wstring& p_Str, const std::wstring& p_Pattern, bool p_bCaseSensitive = false );

	//! 判断字符串是否意指定字符串模式结尾
	bool IsEndWith( const std::wstring& p_Str, const std::wstring& p_Pattern, bool p_bCaseSensitive = false );

	//! 标准化路径
	//@note 将路径中的'\\'转换为'/'，并在最后补上'/'为结尾
	//@par	例如：C:\\aaa\\bbb 转化为 C:/aaa/bbb/
	std::wstring StandardisePath( const std::wstring& p_Init );

	//! 分割文件路径和文件名
	void SplitFilename( const std::wstring& p_QualifiedName,
		std::wstring& p_szOutBasename, std::wstring& p_szOutPath );

	//! 检查原字符是否与模式字符匹配
	//@remarks	支持'*'通配符。当两个或者两个以上通配符连续出现在一起时，则每个通配符代表一个字符。
	//			否则，每个通配符代表N个字符.
	//@Ppar		例如: "FreeKnight" 与'F*t'匹配，却不和'F**t'匹配。
	bool Match( const std::wstring& p_Str, const std::wstring& p_Pattern, bool p_bCaseSensitive = true );

	//! 转换float为 std;:wstring
	//@param [IN] p_usPrecision 显示的位数
	std::wstring ToString( float p_fValue, unsigned short p_usPrecision = 6,
		unsigned short p_usWidth = 0, char p_Fill = ' ', 
		std::ios::fmtflags p_Flags = std::ios::fmtflags( 0 ) );
	std::wstring ToString( double p_dValue, unsigned short p_usPrecision = 6,
		unsigned short p_usWidth = 0, char p_Fill = ' ', 
		std::ios::fmtflags p_Flags = std::ios::fmtflags( 0 ) );

	//! 转换wchar_t为std::wstring
	std::wstring ToString( wchar_t p_Val );

	//! 转换 int 为 std::wstring
	std::wstring ToString( int p_nVal, unsigned short p_usWidth = 0, char p_Fill = ' ',
		std::ios::fmtflags p_Flags = std::ios::fmtflags( 0 ) );
	std::wstring ToString( size_t p_unVal, unsigned short p_usWidth = 0, char p_Fill = ' ',
		std::ios::fmtflags p_Flags = std::ios::fmtflags( 0 ) );
	std::wstring ToString( unsigned long p_ulVal, unsigned short p_usWidth = 0, char p_Fill = ' ',
		std::ios::fmtflags p_Flags = std::ios::fmtflags( 0 ) );
	std::wstring ToString( long p_lVal, unsigned short p_usWidth = 0, char p_Fill = ' ',
		std::ios::fmtflags p_Flags = std::ios::fmtflags( 0 ) );

	//! 转换 bool 为 std::wstring
	//@param [IN] bYesNo 若该参数为true，则返回字符串为"yes"或"no"。否则返回"true"或"false"
	std::wstring ToString( bool p_bValue, bool p_bYesNo = false );

	//! 转换字符串向量为 std::wstring
	std::wstring ToString( const StringVector& p_Value );

	//! 转换 std::wstring 为 float
	float			ParseFloat( const std::wstring& p_Value );
	double			ParseDouble( const std::wstring& p_Value );
	int				ParseInt( const std::wstring& p_Value );
	unsigned int	ParseUnsignedInt( const std::wstring& p_Value );
	long			ParseLong( const std::wstring& p_Value );
	unsigned long	ParseUnsignedLong( const std::wstring& p_Value );
	bool			ParseBool( const std::wstring& p_Value );
	StringVector	ParseStringVector( const std::wstring& p_Value );

	//! 判断一个 std::wstring 是否是数字
	bool IsNumber( const std::wstring& p_Value );

	//! 单字节转双字节
	void Ansi2Wide( wchar_t* p_pDest, const char* p_pSrc, size_t p_unLen );
	//! 双字节转单字节
	void Wide2Ansi( char* p_pDest, const wchar_t* p_pSrc, size_t p_unLen );
	//! 单字节字符串转为单字符数组
	void AnsiString2Array( char* p_pDest, const std::string& p_Str );
	//! 双字符串转为双字符数组
	void WideString2Array( wchar_t* p_pDest, const std::wstring& p_Str );
}