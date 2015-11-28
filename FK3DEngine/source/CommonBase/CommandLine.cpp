/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	CommandLine
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
	CommandLine::CommandLine()
	{
		//获取名令行字符串指针
		char* pCommandLine = GetCommandLineA();

		//获取程序运行路径字符串
		if( *pCommandLine == '\0' || *pCommandLine != '"' )
			return;
		++pCommandLine;

		char* pAppPath = pCommandLine;
		UINT nPathLen = 0;
		while( *pCommandLine != '"' )
		{
			++pCommandLine;
			++nPathLen;
		}

		mAppPath.SetString( pAppPath, nPathLen );

		pCommandLine += 2;

		//获取命令参数字符串
		while( *pCommandLine != '\0' )
		{
			char* pArgString = pCommandLine;

			UINT nArgLen = 0;
			while( *pCommandLine != ' ' )
			{
				++pCommandLine;
				++nArgLen;
			}

			if( nArgLen != 0 )
				mArgStrings.Push( pArgString, nArgLen );

			++pCommandLine;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
