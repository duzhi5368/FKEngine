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
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../../include/UISystem/UISystemInsideHeader.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	GuiCursor::GuiCursor( GuiLoca* pLoca, int nTargetX, int nTargetY, SpriteSet* pSpriteSet, UVAnimation* pUVA )
		: GuiSpriteUVA		( ZOT_Top, NULL, NULL, pLoca, NULL, pSpriteSet, pUVA )
		, mTargetX			(nTargetX)
		, mTargetY			(nTargetY)
		, mfStatChageTime	(6.0f)
		, mfIdleTime		(0.0f)
		, mbIdleStatus		(false)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	//显示/隐藏控件
	void FKFastcall GuiCursor::Show( bool bShow )
	{
		mpSprite->SetVisable( bShow );
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置位置
	void FKFastcall GuiCursor::SetPos( long nX, long nY )
	{
		//改变精灵显示位置
		mpSprite->SetLocation( nX - mTargetX, nY - mTargetY );
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置光标状态切换延时
	void FKFastcall GuiCursor::SetStatChageTime( float fSecond )
	{
		mfStatChageTime = fSecond;
	}
	//--------------------------------------------------------------------------------------------------------------
	//更新光标闲置时间
	void FKFastcall GuiCursor::UpdateIdleTime( float fSecond )
	{
		//如果已经为等待状态则直接返回
		if( mbIdleStatus )
			return;

		mfIdleTime += fSecond;

		//如果到达光标状态切换延时
		if( mfIdleTime >= mfStatChageTime )
		{
			//开始播放等待状态切换动画
			mpSprite->mpUVASprite->PlaySegment( mnChangeSegment, mnIdleSegment );

			mbIdleStatus = true;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//更新光标为活动状态
	void FKFastcall GuiCursor::UpdateActivate()
	{
		mfIdleTime = 0.0f;

		//如果光标不是等待状态则直接返回
		if( !mbIdleStatus )
			return;

		mbIdleStatus = false;

		//如果当前正在播放等待状态切换动画
		if( mpSprite->mpUVASprite->mnCurSegment == mnChangeSegment )
		{
			//开始倒序播放等待状态切换动画
			mpSprite->mpUVASprite->SetReversePlay();

			//设定在其之后播放活动状态动画
			mpSprite->mpUVASprite->SetNextSegment( mnActiveSegment );
		}
		else
		{
			//开始倒序播放等待状态切换动画
			mpSprite->mpUVASprite->PlaySegment( mnChangeSegment, mnActiveSegment, true );
		}
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================

