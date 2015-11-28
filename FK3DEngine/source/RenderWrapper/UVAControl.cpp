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
	UVAControl::UVAControl()
		: mpUVAnimation	(NULL)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	UVAControl::UVAControl( UVAnimation* pUVAnimation )
		: mpUVAnimation	(pUVAnimation)
		, mnCurSegment	(0)
		, mnCurFrame	(pUVAnimation->pSegments->Begin)
		, mfFrameTime	(0)
		, mbReverse		(false)
		, mnNextSegment	(0)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置 UV 动画数据
	void UVAControl::SetUVAnimation( UVAnimation* pUVAnimation )
	{
		mpUVAnimation	= pUVAnimation;
		mnCurSegment	= 0;
		mnCurFrame		= pUVAnimation->pSegments->Begin;
		mfFrameTime		= 0;
		mbReverse		= false;
		mnNextSegment	= 0;
	}
	//--------------------------------------------------------------------------------------------------------------
	//播放指定动画段
	void FKFastcall UVAControl::PlaySegment( int nPlaySegment, int nNextSegment, bool bReverse )
	{
		if( nPlaySegment >= mpUVAnimation->nSegment )
			Except( Exception::ERR_INVALIDPARAMS, "指定播放的 UVA 动画段无效。" );
		if( nNextSegment >= mpUVAnimation->nSegment )
			Except( Exception::ERR_INVALIDPARAMS, "指定下一个播放的 UVA 动画段无效。" );

		UVASegment* pSegment = mpUVAnimation->pSegments + nPlaySegment;

		mnCurSegment = nPlaySegment;
		mnCurFrame = bReverse ? pSegment->End : pSegment->Begin;

		mnNextSegment = nNextSegment;
		mbReverse = bReverse;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置下一个播放动画段
	void FKFastcall UVAControl::SetNextSegment( int nNextSegment )
	{
		if( nNextSegment >= mpUVAnimation->nSegment )
			Except( Exception::ERR_INVALIDPARAMS, "指定下一个播放的 UVA 动画段无效。" );

		mnNextSegment = nNextSegment;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置为倒序播放
	void FKFastcall UVAControl::SetReversePlay( bool bReverse )
	{
		mbReverse = bReverse;
	}
	//--------------------------------------------------------------------------------------------------------------
	//更新动画帧时间
	UVAFrame* FKFastcall UVAControl::UpdateFrameTime( float fFrameTime )
	{
		int nPrevFrame = mnCurFrame;
		UVASegment* pCurSegment = mpUVAnimation->pSegments + mnCurSegment;
		UVAFrame* pCurFrame = mpUVAnimation->pFrames + nPrevFrame;

		//累计帧延时
		mfFrameTime += fFrameTime;

		//如果已经大于当前帧延时
		while( mfFrameTime >= pCurFrame->fFrameTime )
		{
			mfFrameTime -= pCurFrame->fFrameTime;

			//如果为正序播放
			if( !mbReverse ) 
			{
				//如果还未到达该段动画结尾
				if( mnCurFrame < pCurSegment->End )
				{
					//播放到下一帧
					++mnCurFrame;
				}
				//如果已经到达该段动画结尾
				else
				{
					//开始播放下一个动画段
					mnCurSegment = mnNextSegment;
					pCurSegment = mpUVAnimation->pSegments + mnCurSegment;
					mnCurFrame = pCurSegment->Begin;
				}
			}
			//如果为倒序播放
			else
			{
				//如果还未到达该段动画起始
				if( mnCurFrame > pCurSegment->Begin )
				{
					//播放到下一帧
					--mnCurFrame;
				}
				//如果已经到达该段动画起始
				else
				{
					//开始播放下一个动画段
					mnCurSegment = mnNextSegment;
					pCurSegment = mpUVAnimation->pSegments + mnCurSegment;
					mnCurFrame = pCurSegment->End;

					mbReverse = false;
				}
			}

			pCurFrame = mpUVAnimation->pFrames + mnCurFrame;
		}

		//如果改变了播放动画帧
		if( nPrevFrame != mnCurFrame )
			return pCurFrame;

		return NULL;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取当前动画帧 UV 坐标
	const UV4* FKFastcall UVAControl::GetCurrentUV()
	{
		UVAFrame* pCurFrame = mpUVAnimation->pFrames + mnCurFrame;
		return (const UV4*)&pCurFrame->sUV4;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
