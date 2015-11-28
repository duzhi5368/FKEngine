/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	WinTiming
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../include/Timing.h"
#include <windows.h>
#include <cstdlib>

//--------------------------------------------------------------------
void FK2DEngine::FKSleep( unsigned int p_unMilliSeconds )
{
	::Sleep( p_unMilliSeconds );
}
//--------------------------------------------------------------------
namespace
{
	void ResetTGT()
	{
		::timeEndPeriod(1);
	}
}
//--------------------------------------------------------------------
unsigned long FK2DEngine::MilliSeconds()
{
	static bool bInit = false;
	if( !bInit )
	{
		if( ::timeBeginPeriod(1) != TIMERR_NOERROR )
		{
			std::atexit( ResetTGT );
		}
		bInit = true;
	}
	return ::timeGetTime();
}
//--------------------------------------------------------------------