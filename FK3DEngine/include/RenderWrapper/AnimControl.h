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
*	文件说明：	动画控制器
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//动画控制器
	class FK_DLL AnimControl
	{
	public:
		bool				mbCanPlay;			//能否进行播放

		UINT				mNumKey;			//总帧数
		float				mIntervalTime;		//帧停顿时间

		UINT				mCurKey;			//当前帧
		float				mCurIntervalTime;	//当前帧停顿时间

		float				mAnimSpeed;			//动画播放速度

	public:
		AnimControl();

		//设置动画信息
		void SetAnimationInfo( UINT nNumKey, float fIntervalTime );

		//设置动画能否播放
		void SetPlayable( bool bCanPlay );

		//设置动画播放速度
		void SetPlaySpeed( float fSpeed );

		//设置当前播放时间
		void SetCurTime( float fAnimTime );

		//更新动画帧时间
		void FKFastcall UpdateKeyTime( float fKeyTime );
	};
	//--------------------------------------------------------------------------------------------------------------
	typedef Stack< AnimControl* >	AnimControlList;
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
