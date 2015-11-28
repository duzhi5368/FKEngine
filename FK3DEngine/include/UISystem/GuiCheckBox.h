/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	GuiCheckBox
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	复选框对象
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================
#include "../RenderWrapper/FrameListener.h"
//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//复选框对象
	class FK_DLL GuiCheckBox : public GuiSprite
	{
	public:
		//复选框参数
		struct Param
		{
			SpriteSet*		pSpriteSet;		//精灵集

			GuiLoca			sLoca;			//复选框位置
			GuiClient		sClient;		//复选框客户区

			FnFrameEvent	fnEvent;		//复选框事件
		};

	public:
		bool		mbChecked;			//是否为复选状态

	protected:
		UV4				mNormalUV;			//普通纹理坐标
		UV4				mCheckedUV;			//复选纹理坐标
		UV4				mNormalLightUV;		//普通高光纹理坐标
		UV4				mCheckedLightUV;	//复选高光纹理坐标

		FnFrameEvent	mFnEvent;			//复选框事件函数指针

	public:
		GuiCheckBox( ZOrderType eZType, OverlaySurface* pZRefOverlay, GuiDialog* pDialog, Param* pParam );

	public:
		//获得/失去焦点
		void FKFastcall Focus( bool bHasFocus );

		//左键按下
		void FKFastcall LDown( InputSystem* pInputSystem );

		//设置复选框事件函数
		void FKFastcall SetEvent( FnFrameEvent fnEvent );

	public:
		//向用户界面对象发送消息
		LONG_PTR SendMsg( LONG_PTR nGM, LONG_PTR nGMA = 0, LONG_PTR nGMB = 0 );
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================

