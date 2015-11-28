/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	GuiTrackBar
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
	GuiTrackBar::GuiTrackBar( ZOrderType eZType, OverlaySurface* pZRefOverlay, GuiDialog* pDialog, Param* pParam )
		: GuiBase		( NULL, NULL )
		, mpSpriteSet	(pParam->pSpriteSet)
		, mProgWidth	(pParam->nProgWidth)
		, mProgHeight	(pParam->nProgHeight)
		, mBkgY			(pParam->nBkgY)
		, mSliderY		(pParam->nSliderY)
		, mSliderMinX	(pParam->nSliderMinX)
		, mSliderMaxX	(pParam->nSliderMaxX)
		, mSliderWidth	(pParam->nSliderWidth)
		, mSliderHeight	(pParam->nSliderHeight)
		, mRange		(pParam->nRange)
		, mValue		(pParam->nValue)
		, mbDragSlider	(false)
		, mbSliderFocus	(false)
		, mFnEvent		(pParam->fnEvent)
	{
		//计算滑动条客户区
		mLoca.X = pParam->nX;
		mLoca.Y = pParam->nY;
		mLoca.Width = pParam->nBkgWidth;
		mLoca.Height = max( pParam->nBkgHeight + mBkgY, mSliderHeight + mSliderY );

		mClient.SetClient( 0, 0, mLoca.Width, mLoca.Height );

		//计算滑动值百分比
		mPercent = (float)mValue / (float)mRange;
		mDragWidth = mSliderMaxX - mSliderMinX;

		//计算进度条和滑块用户区
		int nProgAbsTop = mBkgY + pParam->nProgTop;
		mSliderX = mSliderMinX + mDragWidth * mPercent;

		mProgressClient.SetClient( pParam->nProgLeft, nProgAbsTop, pParam->nProgLeft + mProgWidth,
			nProgAbsTop + mProgHeight );
		mSliderClient.SetClient( mSliderX, mSliderY, mSliderX + mSliderWidth, mSliderY + mSliderHeight );

		//计算滑动条各部分 UV
		float fTexWidth = (float)mpSpriteSet->mpTexture->mWidth;
		float fTexHeight = (float)mpSpriteSet->mpTexture->mHeight;
		float fSliderWidth = (float)pParam->nBkgWidth;
		float fProgWidth = (float)FloorToInt( (float)mProgWidth * mPercent );

		UV4 sBkgUV;
		sBkgUV.u1 = 0.0f;
		sBkgUV.u2 = (float)pParam->nBkgWidth / fTexWidth;
		sBkgUV.v1 = 0.0f;
		sBkgUV.v2 = (float)pParam->nBkgHeight / fTexHeight;

		mProgressUV.u1 = 0.0f;
		mProgressUV.u2 = fProgWidth / fTexWidth;
		mProgressUV.v1 = sBkgUV.v2;
		mProgressUV.v2 = sBkgUV.v2 + (float)mProgHeight / fTexHeight;

		mNormalUV.u1 = 0.0f;
		mNormalUV.u2 = (float)mSliderWidth / fTexWidth;
		mNormalUV.v1 = mProgressUV.v2;
		mNormalUV.v2 = mProgressUV.v2 + (float)mSliderHeight / fTexHeight;

		mHighLightUV.u1 = mNormalUV.u2;
		mHighLightUV.u2 = mNormalUV.u2 + mNormalUV.u2;
		mHighLightUV.v1 = mNormalUV.v1;
		mHighLightUV.v2 = mNormalUV.v2;

		//创建各部分精灵
		mpBackground = mpSpriteSet->CreateSprite( mLoca.X, mLoca.Y + mBkgY, pParam->nBkgWidth, pParam->nBkgHeight,
			eZType, pZRefOverlay, &sBkgUV );

		mpProgress = mpSpriteSet->CreateSprite( mLoca.X + pParam->nProgLeft, mLoca.Y + nProgAbsTop,
			fProgWidth, mProgHeight, ZOT_Upon, mpBackground, &mProgressUV );

		mpSlider = mpSpriteSet->CreateSprite( mLoca.X + mSliderX, mLoca.Y + mSliderY,
			mSliderWidth, mSliderHeight, ZOT_Upon, mpProgress, &mNormalUV );
	}
	//--------------------------------------------------------------------------------------------------------------
	GuiTrackBar::~GuiTrackBar()
	{
		mpSpriteSet->DeleteSprite( mpBackground );
		mpSpriteSet->DeleteSprite( mpProgress );
		mpSpriteSet->DeleteSprite( mpSlider );
	}
	//--------------------------------------------------------------------------------------------------------------
	//开启/关闭控件
	void FKFastcall GuiTrackBar::Enable( bool bEnable )
	{
		mbEnable = bEnable;

		DWORD dwColor = bEnable ? 0xFFFFFFFF : 0x80FFFFFF;

		mpBackground->SetColor( dwColor );
		mpProgress->SetColor( dwColor );
		mpSlider->SetColor( dwColor );
	}
	//--------------------------------------------------------------------------------------------------------------
	//显示/隐藏控件
	void FKFastcall GuiTrackBar::Show( bool bShow )
	{
		mbVisable = bShow;

		mpBackground->SetVisable( bShow );
		mpProgress->SetVisable( bShow );
		mpSlider->SetVisable( bShow );
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置位置
	void FKFastcall GuiTrackBar::SetPos( long nX, long nY )
	{
		int nOffsetX = nX - mLoca.X;
		int nOffsetY = nY - mLoca.Y;

		mpBackground->MoveLocation( nOffsetX, nOffsetY );
		mpProgress->MoveLocation( nOffsetX, nOffsetY );
		mpSlider->MoveLocation( nOffsetX, nOffsetY );

		mLoca.X = nX;
		mLoca.Y = nY;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置 Z 顺序
	void FKFastcall GuiTrackBar::SetZOrder( ZOrderType eZType, OverlaySurface* pZRefOverlay )
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取 Z 顺序
	OverlaySurface* FKFastcall GuiTrackBar::GetZOrder( ZOrderType eZType )
	{
		switch (eZType)
		{
		case ZOT_Top:		return mpSlider;
		case ZOT_Bottom:	return mpBackground;
		}

		return NULL;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获得/失去焦点
	void FKFastcall GuiTrackBar::Focus( bool bHasFocus )
	{
		mbFocus = bHasFocus;

		//如果失去焦点
		if( !bHasFocus )
		{
			mpSlider->SetSpriteUV( &mNormalUV );
			mbSliderFocus = false;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//鼠标移动
	void FKFastcall GuiTrackBar::MouseMove( InputSystem* pInputSystem )
	{
		//转换为控件坐标系
		int x = pInputSystem->mMouseX - mLoca.X;
		int y = pInputSystem->mMouseY - mLoca.Y;

		//如果滑块焦点变动
		bool bSliderFocus = mSliderClient.IsOnClient( x, y );
		if( bSliderFocus != mbSliderFocus )
		{
			mpSlider->SetSpriteUV( bSliderFocus ? &mHighLightUV : &mNormalUV );
			mbSliderFocus = bSliderFocus;
		}

		//如果正在拖动滑块
		if( mbDragSlider )
		{
			//计算鼠标移动距离
			int nRelX = pInputSystem->mMouseRelX;
			int nFixRelX = 0;
			mSliderX += nRelX;

			if( mSliderX < mSliderMinX )
			{
				nFixRelX = mSliderMinX - mSliderX;
				mSliderX = mSliderMinX;
			}
			else if( mSliderX > mSliderMaxX )
			{
				nFixRelX = mSliderMaxX - mSliderX;
				mSliderX = mSliderMaxX;
			}

			pInputSystem->mMouseRelX += nFixRelX;
			pInputSystem->mMouseX += nFixRelX;
			pInputSystem->mMouseY -= pInputSystem->mMouseRelY;
			pInputSystem->mMouseRelY = 0;

			//计算滑动值变化
			int nOldValue = mValue;
			mPercent = (float)( mSliderX - mSliderMinX ) / mDragWidth;
			mValue = FloorToInt( mPercent * (float)mRange );

			//计算滑块用户区
			mSliderClient.SetClient( mSliderX, mSliderY, mSliderX + mSliderWidth, mSliderY + mSliderHeight );

			//改变滑块位置
			mpSlider->SetLocation( mLoca.X + mSliderX, mLoca.Y + mSliderY );

			//改变进度条显示长度
			float fProgWidth = (float)FloorToInt( (float)mProgWidth * mPercent );
			mpProgress->SetSize( fProgWidth, mProgHeight );

			mProgressUV.u2 = fProgWidth / (float)mpSpriteSet->mpTexture->mWidth;
			mpProgress->SetSpriteUV( &mProgressUV );

			//如果滑动值发生变化触发帧事件
			if( nOldValue != mValue )
				_ActiveEvent();
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//左键按下
	void FKFastcall GuiTrackBar::LDown( InputSystem* pInputSystem )
	{
		//转换为控件坐标系
		int x = pInputSystem->mMouseX - mLoca.X;
		int y = pInputSystem->mMouseY - mLoca.Y;

		//如果是在滑块上按下
		if( mSliderClient.IsOnClient( x, y ) )
		{
			mbDragSlider = true;

			//锁定焦点
			GuiManager::mpSingleton->LockFocus( this );
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//左键释放
	void FKFastcall GuiTrackBar::LUp( InputSystem* pInputSystem )
	{
		mbDragSlider = false;

		//解锁焦点
		GuiManager::mpSingleton->UnlockFocus();
	}
	//--------------------------------------------------------------------------------------------------------------
	//向用户界面对象发送消息
	LONG_PTR GuiTrackBar::SendMsg( LONG_PTR nGM, LONG_PTR nGMA, LONG_PTR nGMB )
	{
		switch (nGM)
		{
			case GM_Enable:
				Enable( ( nGMA == 1 ) ? true : false );
				return TRUE;
			case GM_Show:
				Show( ( nGMA == 1 ) ? true : false );
				return TRUE;
			case GM_Focus:
				Focus( ( nGMA == 1 ) ? true : false );
				return TRUE;
			case GM_MouseMove:
				MouseMove( (InputSystem*)nGMA );
				return TRUE;
			case GM_SetPos:
				SetPos( nGMA, nGMB );
				return TRUE;
			case GM_MovePos:
				SetPos( mLoca.X + nGMA, mLoca.Y + nGMB );
				return TRUE;
			case GM_SetZOrder:
				SetZOrder( (ZOrderType)nGMA, (OverlaySurface*)nGMB );
				return TRUE;
			case GM_GetZOrder:
				return (LONG_PTR)GetZOrder( (ZOrderType)nGMA );
			case GM_LDown:
				LDown( (InputSystem*)nGMA );
				return TRUE;
			case GM_LUp:
				LUp( (InputSystem*)nGMA );
				return TRUE;
		}

		return FALSE;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置滑动条事件函数
	void FKFastcall GuiTrackBar::SetEvent( FnFrameEvent fnEvent )
	{
		mFnEvent = fnEvent;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置滑动值
	void FKFastcall GuiTrackBar::SetValue( int nValue )
	{
		//计算滑动值百分比
		int nOldValue = nValue;
		mValue = nValue;
		mPercent = (float)mValue / (float)mRange;

		//计算滑块用户区
		mSliderX = mSliderMinX + mDragWidth * mPercent;
		mSliderClient.SetClient( mSliderX, mSliderY, mSliderX + mSliderWidth, mSliderY + mSliderHeight );

		//改变滑块位置
		mpSlider->SetLocation( mLoca.X + mSliderX, mLoca.Y + mSliderY );

		//改变进度条显示长度
		float fProgWidth = (float)FloorToInt( (float)mProgWidth * mPercent );
		mpProgress->SetSize( fProgWidth, mProgHeight );

		mProgressUV.u2 = fProgWidth / (float)mpSpriteSet->mpTexture->mWidth;
		mpProgress->SetSpriteUV( &mProgressUV );

		//如果滑动值发生变化触发帧事件
		if( nOldValue != mValue )
			_ActiveEvent();
	}
	//--------------------------------------------------------------------------------------------------------------
	//触发滑动条帧事件
	void FKFastcall GuiTrackBar::_ActiveEvent()
	{
		//将滑动条事件加入帧事件列表
		if( mFnEvent != NULL && Root::mpSingleton->mpFrameListener != NULL )
			Root::mpSingleton->mpFrameListener->AddFrameEvent( mFnEvent, (LONG_PTR)this );
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================

