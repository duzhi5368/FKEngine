/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	FrameListener
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../../include/RenderWrapper/RenderWrapperInsideHeader.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	FrameListener::FrameListener()
	{
		mFrameEventList.Initialize( 20, 20 );
	}
	//--------------------------------------------------------------------------------------------------------------
	FrameListener::~FrameListener()
	{
		mFrameEventList.Release();
	}
	//--------------------------------------------------------------------------------------------------------------
	//增加帧事件
	void FKFastcall FrameListener::AddFrameEvent( FnFrameEvent fnEvent, LONG_PTR pParam )
	{
		FrameEvent* pEvent = mFrameEventList.Push();
		pEvent->fnEvent = fnEvent;
		pEvent->pParam = pParam;
	}
	//--------------------------------------------------------------------------------------------------------------
	//处理帧事件
	void FrameListener::ProcFrameEvent()
	{
		FrameEventList::Iterator it = mFrameEventList.Begin();
		FrameEventList::Iterator end = mFrameEventList.End();
		for(; it!=end; ++it )
		{
			FrameEvent* pEvent = it;
			pEvent->fnEvent( pEvent->pParam );
		}

		mFrameEventList.Clear();
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
