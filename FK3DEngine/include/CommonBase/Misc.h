/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Misc
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	引擎辅助功能程序
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================
#include "Stack.h"
#include <windows.h>
#include "BaseTypeDefine.h"
//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//辅助功能类
	class FK_DLL Misc
	{
	protected:
		static CHARStack	mFormatBuf;		//字符串格式化缓存
		static WCHARStack	mUnicodeBuf;	// Unicode 字符串缓存

	public:
		//格式化字符串
		static char* FormatString( int* pLen, const char* szStr, ... );

		//显示消息
		static int ShowMsg( const char* szStr, const char* szCaption, int nFlag, ... );

		//显示错误消息
		static void ShowErrMsg( const char* szStr, ... );

		//显示 WIN32 系统错误消息
		static void ShowWin32ErrMsg();

		//返回第一个指定字符在字符串中的位置（找不到则返回 -1）
		static int GetFirstCharIndex( char* pStr, char cSymbol );

		//返回最后一个指定字符在字符串中的位置（找不到则返回 -1）
		static int GetLastCharIndex( char* pStr, char cSymbol );

		//判断字符串是否为正确的全角字符结尾
		static bool IsSBCTerminater( char* pStr, size_t nStrLen );

		//将多字节字符串转换为 Unicode 字符串
		static wchar_t* AnsiToUnicode( char* pStr );

		//获取字符串哈希值（可输出字符串长度）
		static unsigned int HashString( const char* pString, size_t* pStrLenOutput = NULL );

		//计算 MD5 密钥
		static void GetMD5Key( BYTE pMD5Key[16], const char* pString );

		//计算 CRC32 校验值
		static DWORD GetCRC32( BYTE* pData, DWORD dwLen );

		//根据像素格式获取色深字节数
		static unsigned int GetColorDepth( PixelFormat ePixelFormat );

		//根据像素格式获取 Alpha 值位数
		static unsigned int GetColorAlphaBits( PixelFormat ePixelFormat );

		//重置随机种子
		static void ResetRandomSeed();

		//生成指定范围内的整型随机数
		static int GetIntRandom( const int nMin, const int nMax );

		//生成指定范围内的浮点随机数
		static float GetFloatRandom( const float fMin, const float fMax );
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
