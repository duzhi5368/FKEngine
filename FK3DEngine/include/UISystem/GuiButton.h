/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	GuiButton
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	按钮对象
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================
#include "GuiSprite.h"
#include "../RenderWrapper/FrameListener.h"
//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//按钮对象
	class FK_DLL GuiButton : public GuiSprite
	{
	public:
		static const int mnNormalStatus		= 0;	//按钮普通状态动画帧
		static const int mnFocusStatus		= 1;	//按钮焦点状态动画帧
		static const int mnPressStatus		= 2;	//按钮按下状态动画帧

		//按钮参数
		struct Param
		{
			SpriteSet*		pSpriteSet;		//精灵集

			GuiLoca			sLoca;			//按钮位置
			GuiClient		sClient;		//按钮客户区
			UV4*			pUV;			//按钮纹理坐标
			FnFrameEvent	fnEvent;		//按钮事件
		};

	protected:
		UV4*			mpUV;		//按钮动画帧指针
		FnFrameEvent	mFnEvent;	//按钮事件函数指针
		bool			mbPressed;	//是否已按下过按钮

	public:
		GuiButton( ZOrderType eZType, OverlaySurface* pZRefOverlay, GuiDialog* pDialog, Param* pParam );

	public:
		//获得/失去焦点
		void FKFastcall Focus( bool bHasFocus );

		//左键按下
		void FKFastcall LDown( InputSystem* pInputSystem );

		//左键释放
		void FKFastcall LUp( InputSystem* pInputSystem );

		//设置按钮事件函数
		void FKFastcall SetEvent( FnFrameEvent fnEvent );

	public:
		//向用户界面对象发送消息
		LONG_PTR SendMsg( LONG_PTR nGM, LONG_PTR nGMA = 0, LONG_PTR nGMB = 0 );
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================

