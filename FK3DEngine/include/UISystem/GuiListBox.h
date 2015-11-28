/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	GuiListBox
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	列表框对象
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//列表框对象
	class FK_DLL GuiListBox : public GuiBase
	{
	public:
		//列表框参数
		struct Param
		{
			SpriteSet*		pSpriteSet;		//精灵集
			FontFace*		pFontFace;		//名称字体
			UINT			nItemNum;		//初始列表项数量

			int				nX;				// X 坐标
			int				nY;				// Y 坐标
			int				nBkgWidth;		//背景宽度
			int				nBkgHeight;		//背景高度
		};

	protected:
		SpriteSet*		mpSpriteSet;		//列表框精灵集指针
		FontFace*		mpTextFont;			//列表项文字字体

		int				mX;					// X 坐标
		int				mY;					// Y 坐标

	public:
		GuiListBox( GuiDialog* pDialog, Param* pParam );
		~GuiListBox();

	public:
		//开启/关闭控件
		void FKFastcall Enable( bool bEnable );

		//显示/隐藏控件
		void FKFastcall Show( bool bShow );

		//设置位置
		void FKFastcall SetPos( long nX, long nY );

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

	protected:

	public:
		//向用户界面对象发送消息
		LONG_PTR SendMsg( LONG_PTR nGM, LONG_PTR nGMA = 0, LONG_PTR nGMB = 0 );
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================

