/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	NumConvert
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	引擎辅助功能程序
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//数字转换类
	class FK_DLL NumConvert
	{
	public:
		static const int	mMaxIntTextLen = 65;				//最大整型转换字符串长度
		static const int	mMaxFloatTextLen = _CVTBUFSIZE;		//最大浮点型转换字符串长度

	public:
		//是否可将多字节字符串转换为整数
		static bool CanMultiByteToInt( LPCSTR szStr, UINT nLen = 0 );

		//是否可将多字节字符串转换为浮点数
		static bool CanMultiByteToFloat( LPCSTR szStr, UINT nLen = 0 );

		//是否可将 Unicode 字符串转换为整数
		static bool CanUnicodeToInt( LPCWSTR szStr, UINT nLen = 0 );

		//是否可将 Unicode 字符串转换为浮点数
		static bool CanUnicodeToFloat( LPCWSTR szStr, UINT nLen = 0 );

		//将多字节字符串转换为整数
		static int MultiByteToInt( LPCSTR szStr );

		//将多字节字符串转换为浮点数
		static float MultiByteToFloat( LPCSTR szStr );

		//将 Unicode 字符串转换为整数
		static int UnicodeToInt( LPCWSTR szStr );

		//将 Unicode 字符串转换为浮点数
		static float UnicodeToFloat( LPCWSTR szStr );

		//将整数转换为多字节字符串
		static int IntToMultiByte( int nValue, char* pBuf );

		//将整数转换为 Unicode 字符串
		static int IntToUnicode( int nValue, wchar_t* pBuf );

		//将浮点数转换为多字节字符串
		static int FloatToMultiByte( float fValue, char* pBuf, int nPrecision = 2 );

		//将浮点数转换为 Unicode 字符串
		static int FloatToUnicode( float fValue, wchar_t* pBuf, int nPrecision = 2 );
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
