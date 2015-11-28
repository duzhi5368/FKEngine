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
*	文件说明：	滑动条对象
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//滑动条对象
	class FK_DLL GuiTrackBar : public GuiBase
	{
	public:
		//滑动条参数
		struct Param
		{
			SpriteSet*		pSpriteSet;		//精灵集指针
			int				nRange;			//滑动值范围
			int				nValue;			//当前滑动值

			int				nX;				// X 坐标
			int				nY;				// Y 坐标

			int				nBkgY;			//背景 Y 坐标
			int				nBkgWidth;		//背景宽度
			int				nBkgHeight;		//背景高度

			int				nProgLeft;		//进度条偏移 X 坐标
			int				nProgTop;		//进度条偏移 Y 坐标
			int				nProgWidth;		//进度条宽度
			int				nProgHeight;	//进度条高度

			int				nSliderY;		//滑块 Y 坐标
			int				nSliderMinX;	//滑块最小 X 坐标
			int				nSliderMaxX;	//滑块最大 X 坐标
			int				nSliderWidth;	//滑块宽度
			int				nSliderHeight;	//滑块高度

			FnFrameEvent	fnEvent;		//滑块事件
		};

	public:
		float			mDragWidth;			//滑块横向移动范围
		int				mRange;				//滑动值范围
		int				mValue;				//当前滑动值
		float			mPercent;			//滑动值百分比

	protected:
		SpriteSet*		mpSpriteSet;		//滑动条精灵集指针

		UV4				mProgressUV;		//进度条 UV
		UV4				mNormalUV;			//普通滑块 UV
		UV4				mHighLightUV;		//高光滑块 UV

		Sprite*			mpBackground;		//背景精灵
		Sprite*			mpProgress;			//进度条精灵
		Sprite*			mpSlider;			//滑块精灵

		int				mBkgY;				//背景 Y 坐标
		int				mProgWidth;			//进度条宽度
		int				mProgHeight;		//进度条高度
		int				mSliderX;			//滑块 X 坐标
		int				mSliderY;			//滑块 Y 坐标
		int				mSliderMinX;		//滑块最小 X 坐标
		int				mSliderMaxX;		//滑块最大 X 坐标
		int				mSliderWidth;		//滑块宽度
		int				mSliderHeight;		//滑块高度

		GuiClient		mProgressClient;	//进度条客户区
		GuiClient		mSliderClient;		//滑块客户区

		bool			mbDragSlider;		//是否正在拖动滑块
		bool			mbSliderFocus;		//滑块是否获得焦点

		FnFrameEvent	mFnEvent;			//滑动条事件函数指针

	public:
		GuiTrackBar( ZOrderType eZType, OverlaySurface* pZRefOverlay, GuiDialog* pDialog, Param* pParam );
		~GuiTrackBar();

		//开启/关闭控件
		void FKFastcall Enable( bool bEnable );

		//显示/隐藏控件
		void FKFastcall Show( bool bShow );

		//设置位置
		void FKFastcall SetPos( long nX, long nY );

		//移动位置
		void FKFastcall MovePos( long nX, long nY );

		//设置 Z 顺序
		void FKFastcall SetZOrder( ZOrderType eZType, OverlaySurface* pZRefOverlay );

		//获取 Z 顺序
		OverlaySurface* FKFastcall GetZOrder( ZOrderType eZType );

		//获得/失去焦点
		void FKFastcall Focus( bool bHasFocus );

		//鼠标移动
		void FKFastcall MouseMove( InputSystem* pInputSystem );

		//左键按下
		void FKFastcall LDown( InputSystem* pInputSystem );

		//左键释放
		void FKFastcall LUp( InputSystem* pInputSystem );

	public:
		//向用户界面对象发送消息
		LONG_PTR SendMsg( LONG_PTR nGM, LONG_PTR nGMA = 0, LONG_PTR nGMB = 0 );

		//设置滑动条事件函数
		void FKFastcall SetEvent( FnFrameEvent fnEvent );

		//设置滑动值
		void FKFastcall SetValue( int nValue );

	protected:
		//触发滑动条帧事件
		void FKFastcall _ActiveEvent();
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================

