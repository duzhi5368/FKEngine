/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	GuiComboBox
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	class TextSurface;
	class SpriteSet;
	class FontFace;
	class Sprite;
	//--------------------------------------------------------------------------------------------------------------
	//组合框对象
	class FK_DLL GuiComboBox : public GuiBase
	{
	public:
		//组合框参数
		struct Param
		{
			SpriteSet*		pSpriteSet;		//精灵集
			FontFace*		pFontFace;		//名称字体
			UINT			nItemNum;		//初始列表项数量

			int				nX;				// X 坐标
			int				nY;				// Y 坐标
			int				nWidth;			//宽度
			int				nChoosenHeight;	//选择项高度
			int				nItemHeight;	//列表项高度
			int				nBoarderHeight;	//列表框边框高度
			int				nTextLeft;		//文字偏移 X 坐标
			int				nTextTop;		//文字偏移 Y 坐标
			GuiClient		sChoosenClient;	//选择项客户区
		};

		//列表项结构
		struct Item
		{
			LONG_PTR		pData;			//列表项数据

			Sprite*			pItemSprite;	//列表项精灵
			TextSurface*	pItemText;		//列表项文字
		};
		typedef Stack< Item >	ItemList;

	protected:
		SpriteSet*		mpSpriteSet;		//组合框精灵集指针
		FontFace*		mpTextFont;			//列表项文字字体

		UV4				mTopBoarderUV;		//上边框 UV
		UV4				mBottomBoarderUV;	//下边框 UV
		UV4				mNormalUV;			//普通选择项 UV
		UV4				mHighLightUV;		//高光选择项 UV
		UV4				mItemUV;			//列表项 UV

		Sprite*			mpChoosenItem;		//选择项精灵
		TextSurface*	mpChoosenText;		//选择项文字

		Sprite*			mpHighLightItem;	//列表项高光遮罩精灵
		Sprite*			mpBoarder;			//选择项边框精灵

		int				mX;					//组合框 X 坐标
		int				mY;					//组合框 Y 坐标
		int				mWidth;				//组合框宽度
		int				mChoosenHeight;		//选择项高度
		int				mItemHeight;		//列表项高度
		int				mBoarderHeight;		//列表框边框高度
		int				mTextLeft;			//文字偏移 X 坐标
		int				mTextTop;			//文字偏移 Y 坐标

		GuiClient		mChoosenClient;		//选择项客户区
		bool			mbChoosenFocus;		//选择项焦点

		GuiClient		mListClient;		//列表客户区
		bool			mbShowList;			//是否显示列表

		ItemList		mItemList;			//列表项
		int				mnFocusItem;		//焦点列表项索引

	public:
		GuiComboBox( ZOrderType eZType, OverlaySurface* pZRefOverlay, GuiDialog* pDialog, Param* pParam );
		~GuiComboBox();

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

		//加入列表项
		void FKFastcall AddItem( const wchar_t* szName, UINT nIndex, LONG_PTR pData = 0 );

		//删除列表项
		void FKFastcall DelItem( UINT nIndex );

		//删除所有列表项
		void FKFastcall DelAllItem();

		//选择列表项
		void FKFastcall SelItem( UINT nIndex );

	public:
		//向用户界面对象发送消息
		LONG_PTR SendMsg( LONG_PTR nGM, LONG_PTR nGMA = 0, LONG_PTR nGMB = 0 );

	protected:
		//显示下拉列表
		void FKFastcall _ShowList();

		//隐藏下拉列表
		void FKFastcall _HideList();
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================

