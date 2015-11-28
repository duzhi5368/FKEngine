/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	UVAControl
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	UV 动画控制器
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	struct UVAnimation;
	//--------------------------------------------------------------------------------------------------------------
	// UV 动画控制器
	class FK_DLL UVAControl
	{
	public:
		UVAnimation*	mpUVAnimation;	// UV 动画数据指针

		int				mnCurSegment;	//当前播放的动画段
		int				mnCurFrame;		//当前播放的动画帧

		float			mfFrameTime;	//当前帧延时

		bool			mbReverse;		//是否反转播放
		int				mnNextSegment;	//下一个播放的动画段

	public:
		UVAControl();
		UVAControl( UVAnimation* pUVAnimation );

		//设置 UV 动画数据
		void SetUVAnimation( UVAnimation* pUVAnimation );

		//播放指定动画段
		void FKFastcall PlaySegment( int nPlaySegment, int nNextSegment, bool bReverse = false );

		//设置下一个播放动画段
		void FKFastcall SetNextSegment( int nNextSegment );

		//设置为倒序播放
		void FKFastcall SetReversePlay( bool bReverse = true );

		//更新动画帧时间
		UVAFrame* FKFastcall UpdateFrameTime( float fFrameTime );

		//获取当前动画帧 UV 坐标
		const UV4* FKFastcall GetCurrentUV();
	};
	//--------------------------------------------------------------------------------------------------------------
};

//==================================================================================================================
