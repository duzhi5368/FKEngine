/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	GuiSpriteUVA
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	动画精灵用户界面对象
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================
#include "GuiBase.h"
#include "../RenderWrapper/OverlaySurface.h"
//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	class SpriteSet;
	class Sprite;
	struct UVAnimation;
	//--------------------------------------------------------------------------------------------------------------
	//动画精灵用户界面对象
	class FK_DLL GuiSpriteUVA : public GuiBase
	{
	public:
		SpriteSet*	mpSpriteSet;	//按钮图像精灵集
		Sprite*		mpSprite;		//按钮图像精灵

	public:
		GuiSpriteUVA( ZOrderType eZType, OverlaySurface* pZRefOverlay, GuiDialog* pDialog,
			GuiLoca* pLoca, GuiClient* pClient, SpriteSet* pSpriteSet, UVAnimation* pUVA );
		virtual ~GuiSpriteUVA();

	public:
		//显示/隐藏控件
		void FKFastcall Show( bool bShow );

		//设置位置
		void FKFastcall SetPos( long nX, long nY );

		//设置 Z 顺序
		void FKFastcall SetZOrder( ZOrderType eZType, OverlaySurface* pZRefOverlay );

		//获取 Z 顺序
		OverlaySurface* FKFastcall GetZOrder( ZOrderType eZType );

		//播放指定 UV 动画段
		void FKFastcall PlaySegment( int nPlaySegment, int nNextSegment, bool bReverse = false );

	public:
		//向用户界面对象发送消息
		LONG_PTR SendMsg( LONG_PTR nGM, LONG_PTR nGMA = 0, LONG_PTR nGMB = 0 );
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================

