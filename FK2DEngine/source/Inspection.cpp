/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Inspection
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../include/Inspection.h"
#include "../include/Timing.h"

//--------------------------------------------------------------------

namespace FK2DEngine
{
	namespace FPS
	{
		int g_nFps;
		int g_nAccum;
		int g_nSec;

		void RegisterFrame()
		{
			++g_nAccum;
			int nNewSec = FK2DEngine::MilliSeconds() / 1000;
			if( nNewSec != g_nSec )
			{
				g_nSec = nNewSec;
				g_nFps = g_nAccum;
				g_nAccum = 0;
			}
		}
	}
	//--------------------------------------------------------------------
	int GetFPS()
	{
		return FPS::g_nFps;
	}
}