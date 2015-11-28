/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	AnimControl
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
	AnimControl::AnimControl()
		: mbCanPlay				(true)
		, mNumKey				(0)
		, mIntervalTime			(0.1f)
		, mCurKey				(0)
		, mCurIntervalTime		(0.0f)
		, mAnimSpeed			(1.0f)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置动画信息
	void AnimControl::SetAnimationInfo( UINT nNumKey, float fIntervalTime )
	{
		if( nNumKey == 0 || fIntervalTime == 0.0f )
			Except( Exception::ERR_INVALIDPARAMS, "错误的动画帧数或帧停顿时间。" );

		mNumKey = nNumKey;
		mIntervalTime = fIntervalTime;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置动画能否播放
	void AnimControl::SetPlayable( bool bCanPlay )
	{
		mbCanPlay = bCanPlay;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置动画播放速度
	void AnimControl::SetPlaySpeed( float fSpeed )
	{
		mAnimSpeed = fSpeed;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置当前播放时间
	void AnimControl::SetCurTime( float fAnimTime )
	{
		mCurIntervalTime = fAnimTime;
		int nPassedKey = FloorToInt( mCurIntervalTime / mIntervalTime );
		mCurIntervalTime -= mIntervalTime * (float)nPassedKey;

		mCurKey += (UINT)nPassedKey;		
		mCurKey %= mNumKey;
	}
	//--------------------------------------------------------------------------------------------------------------
	//更新动画帧时间
	void FKFastcall AnimControl::UpdateKeyTime( float fKeyTime )
	{
		mCurIntervalTime += fKeyTime * mAnimSpeed;
		int nPassedKey = FloorToInt( mCurIntervalTime / mIntervalTime );
		mCurIntervalTime -= mIntervalTime * (float)nPassedKey;

		mCurKey += (UINT)nPassedKey;
		mCurKey %= mNumKey;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
