/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	GuiCursor
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	光标对象
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================
#include "GuiSpriteUVA.h"
//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	struct UVAnimation;
	//--------------------------------------------------------------------------------------------------------------
	//光标对象
	class FK_DLL GuiCursor : public GuiSpriteUVA
	{
	public:
		static const int mnActiveSegment	= 0;	//光标活动状态动画段
		static const int mnChangeSegment	= 1;	//光标活动状态到闲置状态改变动画段
		static const int mnIdleSegment		= 2;	//光标闲置状态动画段

	public:
		int			mTargetX;			//光标目标 X
		int			mTargetY;			//光标目标 Y

		float		mfStatChageTime;	//光标状态切换延时
		float		mfIdleTime;			//光标闲置时间

		bool		mbIdleStatus;		//光标是否为等待状态

	public:
		GuiCursor( GuiLoca* pLoca, int nTargetX, int nTargetY, SpriteSet* pSpriteSet, UVAnimation* pUVA );

	public:
		//显示/隐藏控件
		void FKFastcall Show( bool bShow );

		//设置位置
		void FKFastcall SetPos( long nX, long nY );

		//设置光标状态切换延时
		void FKFastcall SetStatChageTime( float fSecond = 6.0f );

		//更新光标闲置时间
		void FKFastcall UpdateIdleTime( float fSecond );

		//更新光标为活动状态
		void FKFastcall UpdateActivate();
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================

