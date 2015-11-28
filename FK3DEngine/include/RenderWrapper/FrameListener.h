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
*	文件说明：	帧监听器
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------

	//帧事件处理函数
	typedef void (*FnFrameEvent)( LONG_PTR pParam );

	//帧事件数据
	struct FrameEvent
	{
		FnFrameEvent	fnEvent;	//帧事件处理函数指针
		LONG_PTR		pParam;		//帧事件函数参数
	};

	typedef Stack< FrameEvent >		FrameEventList;

	//帧监听数据
	struct FrameListenerData
	{
		float	fTimeSinceLastFrame;		//上一帧到当前帧的时间
	};

	//--------------------------------------------------------------------------------------------------------------
	//帧监听器（应用程序继承此类以实现自己的帧监听器）
	class FK_DLL FrameListener
	{
	protected:
		FrameEventList		mFrameEventList;	//帧事件列表

	public:
		FrameListener();
		virtual ~FrameListener();

		//增加帧事件
		void FKFastcall AddFrameEvent( FnFrameEvent fnEvent, LONG_PTR pParam = NULL );

		//处理帧事件
		void ProcFrameEvent();

		//帧起始事件
		virtual void FrameStart( FrameListenerData* pFrameData ) = 0;

		//帧结束时间
		virtual void FrameEnd( FrameListenerData* pFrameData ) = 0;
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
