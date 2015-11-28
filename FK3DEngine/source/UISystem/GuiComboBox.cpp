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

#include "../../include/UISystem/UISystemInsideHeader.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	GuiComboBox::GuiComboBox( ZOrderType eZType, OverlaySurface* pZRefOverlay, GuiDialog* pDialog, Param* pParam )
		: GuiBase		( NULL, NULL, pDialog )
		, mpSpriteSet	(pParam->pSpriteSet)
		, mpTextFont	(pParam->pFontFace)
		, mX			(pParam->nX)
		, mY			(pParam->nY)
		, mWidth		(pParam->nWidth)
		, mChoosenHeight(pParam->nChoosenHeight)
		, mItemHeight	(pParam->nItemHeight)
		, mBoarderHeight(pParam->nBoarderHeight)
		, mTextLeft		(pParam->nTextLeft)
		, mTextTop		(pParam->nTextTop)
		, mChoosenClient(pParam->sChoosenClient)
		, mListClient	(pParam->sChoosenClient)
		, mbChoosenFocus(false)
		, mbShowList	(false)
		, mnFocusItem	(0)
	{
		//计算组合框用户区
		mLoca.X = pParam->nX;
		mLoca.Y = pParam->nY;
		mLoca.Width = pParam->nWidth;
		mLoca.Height = pParam->nChoosenHeight;

		mClient.SetClient( 0, 0, mLoca.Width, mLoca.Height );

		//计算组合框各部分精灵 UV
		float fTexHeight = (float)mpSpriteSet->mpTexture->mHeight;
		float fComboWidth = (float)mWidth / (float)mpSpriteSet->mpTexture->mWidth;
		float fChoosenHeight = (float)mChoosenHeight / fTexHeight;
		float fItemHeight = (float)mItemHeight / fTexHeight;
		float fBoarderHeight = (float)mBoarderHeight / fTexHeight;

		mNormalUV.u1 = 0.0f;
		mNormalUV.u2 = fComboWidth;
		mNormalUV.v1 = 0.0f;
		mNormalUV.v2 = fChoosenHeight;

		mHighLightUV.u1 = 0.0f;
		mHighLightUV.u2 = fComboWidth;
		mHighLightUV.v1 = mNormalUV.v2;
		mHighLightUV.v2 = mNormalUV.v2 + fChoosenHeight;

		mItemUV.u1 = 0.0f;
		mItemUV.u2 = fComboWidth;
		mItemUV.v1 = mHighLightUV.v2;
		mItemUV.v2 = mHighLightUV.v2 + fItemHeight;

		UV4 sHighLightItemUV;
		sHighLightItemUV.u1 = 0.0f;
		sHighLightItemUV.u2 = fComboWidth;
		sHighLightItemUV.v1 = mItemUV.v2;
		sHighLightItemUV.v2 = mItemUV.v2 + fItemHeight;

		mTopBoarderUV.u1 = 0.0f;
		mTopBoarderUV.u2 = fComboWidth;
		mTopBoarderUV.v1 = sHighLightItemUV.v2 + fBoarderHeight;
		mTopBoarderUV.v2 = sHighLightItemUV.v2;

		mBottomBoarderUV.u1 = 0.0f;
		mBottomBoarderUV.u2 = fComboWidth;
		mBottomBoarderUV.v1 = mTopBoarderUV.v2;
		mBottomBoarderUV.v2 = mTopBoarderUV.v1;

		//创建选择框各部分精灵
		mpChoosenItem = mpSpriteSet->CreateSprite( mX, mY, mWidth, mChoosenHeight,
			eZType, pZRefOverlay, &mNormalUV );

		mpHighLightItem = mpSpriteSet->CreateSprite( mX, mY, mWidth, mItemHeight,
			ZOT_Upon, mpChoosenItem, &sHighLightItemUV );

		mpBoarder = mpSpriteSet->CreateSprite( mX, mY, mWidth, mBoarderHeight,
			ZOT_Upon, mpChoosenItem, &mBottomBoarderUV );

		mpHighLightItem->SetVisable( false );
		mpBoarder->SetVisable( false );

		//创建选择项文字
		mpChoosenText = TextManager::mpSingleton->CreateTextSurface( ZOT_Upon, mpChoosenItem, mpTextFont, L" ",
			mLoca.X + mChoosenClient.mRect.Left + mTextLeft, mLoca.Y + mChoosenClient.mRect.Top + mTextTop,
			0, 128 );

		//初始化列表项
		mItemList.Initialize( pParam->nItemNum, 10 );
	}
	//--------------------------------------------------------------------------------------------------------------
	GuiComboBox::~GuiComboBox()
	{
		//删除所有列表项
		DelAllItem();

		mpSpriteSet->DeleteSprite( mpChoosenItem );
		mpSpriteSet->DeleteSprite( mpHighLightItem );
		mpSpriteSet->DeleteSprite( mpBoarder );

		TextManager::mpSingleton->DeleteTextSurface( mpChoosenText );
	}
	//--------------------------------------------------------------------------------------------------------------
	//开启/关闭控件
	void FKFastcall GuiComboBox::Enable( bool bEnable )
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	//显示/隐藏控件
	void FKFastcall GuiComboBox::Show( bool bShow )
	{
		mbVisable = bShow;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置位置
	void FKFastcall GuiComboBox::SetPos( long nX, long nY )
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置 Z 顺序
	void FKFastcall GuiComboBox::SetZOrder( ZOrderType eZType, OverlaySurface* pZRefOverlay )
	{
	}
	//--------------------------------------------------------------------------------------------------------------
//	获取 Z 顺序
	OverlaySurface* FKFastcall GuiComboBox::GetZOrder( ZOrderType eZType )
	{
		//switch (eZType)
		//{
		//case ZOT_Top:		return mpChoosenText;
		//case ZOT_Bottom:	return mpChoosenItem;
		//}

		return NULL;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获得/失去焦点
	void FKFastcall GuiComboBox::Focus( bool bHasFocus )
	{
		mbFocus = bHasFocus;

		//如果失去焦点
		if( !bHasFocus )
		{
			mpChoosenItem->SetSpriteUV( &mNormalUV );
			mbChoosenFocus = false;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//鼠标移动
	void FKFastcall GuiComboBox::MouseMove( InputSystem* pInputSystem )
	{
		//转换为控件坐标系
		int x = pInputSystem->mMouseX - mLoca.X;
		int y = pInputSystem->mMouseY - mLoca.Y;

		//如果选择项焦点变动
		bool bChoosenFocus = mChoosenClient.IsOnClient( x, y );
		if( bChoosenFocus != mbChoosenFocus )
		{
			mpChoosenItem->SetSpriteUV( bChoosenFocus ? &mHighLightUV : &mNormalUV );
			mbChoosenFocus = bChoosenFocus;
		}

		//如果正在显示下拉列表并且鼠标在列表项上
		if( mbShowList && mListClient.IsOnClient( x, y ) )
		{
			//判断焦点列表项
			int nItem = ( y - mListClient.mRect.Top ) / mItemHeight;

			//如果列表项焦点改变
			if( nItem != mnFocusItem )
			{
				//改变列表项高光遮罩精灵位置
				float fItemY = (float)( nItem * mItemHeight + mLoca.Y + mListClient.mRect.Top );
				mpHighLightItem->SetLocation( mX, fItemY );

				mnFocusItem = nItem;
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//左键按下
	void FKFastcall GuiComboBox::LDown( InputSystem* pInputSystem )
	{
		//转换为控件坐标系
		int nX = pInputSystem->mMouseX - mLoca.X;
		int nY = pInputSystem->mMouseY - mLoca.Y;

		//在选择项上按键
		if( mChoosenClient.IsOnClient( nX, nY ) )
		{
			//显示或隐藏下拉列表
			if( mbShowList )
				_HideList();
			else
				_ShowList();
		}
		//如果在下拉列表上按键
		else if( mbShowList && mListClient.IsOnClient( nX, nY ) )
		{
			//判断焦点列表项
			int nItem = ( nY - mListClient.mRect.Top ) / mItemHeight;
			Item* pItem = mItemList.Begin() + nItem;

			//改变当前选择文字
			mpChoosenText->SetText( pItem->pItemText->mpText );

			//隐藏下拉列表
			_HideList();
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//加入列表项
	void FKFastcall GuiComboBox::AddItem( const wchar_t* szName, UINT nIndex, LONG_PTR pData )
	{
		//检查指定插入位置是否正确
		UINT nNumItems = mItemList.Size();
		if( nIndex > nNumItems )
			Except( Exception::ERR_INVALIDPARAMS, "不正确的列表项插入位置。" );

		//创建列表项
		Item* pItem = mItemList.Insert( nIndex );

		pItem->pData = pData;

		pItem->pItemSprite = mpSpriteSet->CreateSprite( mX, mY, mWidth, mItemHeight,
			ZOT_Below, mpHighLightItem, &mItemUV );

		pItem->pItemText = TextManager::mpSingleton->CreateTextSurface( ZOT_Upon, mpHighLightItem,
			mpTextFont, szName, mX, mY );

		pItem->pItemSprite->SetVisable( false );
		pItem->pItemText->SetVisable( false );
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除列表项
	void FKFastcall GuiComboBox::DelItem( UINT nIndex )
	{
		//检查要求删除的列表项是否存在
		UINT nNumItems = mItemList.Size();
		if( nIndex >= nNumItems )
			Except( Exception::ERR_INVALIDPARAMS, "不正确的列表项删除位置。" );

		Item* pItem = mItemList.Begin() + nIndex;

		mpSpriteSet->DeleteSprite( pItem->pItemSprite );
		TextManager::mpSingleton->DeleteTextSurface( pItem->pItemText );

		mItemList.Erase( pItem );
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除所有列表项
	void FKFastcall GuiComboBox::DelAllItem()
	{
		ItemList::Iterator it = mItemList.Begin();
		ItemList::Iterator end = mItemList.End();
		for(; it!=end; ++it )
		{
			Item* pItem = it;

			TextManager::mpSingleton->DeleteTextSurface( pItem->pItemText );

			mpSpriteSet->DeleteSprite( pItem->pItemSprite );
		}

		mItemList.Clear();
	}
	//--------------------------------------------------------------------------------------------------------------
	//选择列表项
	void FKFastcall GuiComboBox::SelItem( UINT nIndex )
	{
		//检查要求删除的列表项是否存在
		UINT nNumItems = mItemList.Size();
		if( nIndex >= nNumItems )
			Except( Exception::ERR_INVALIDPARAMS, "不正确的列表项选择位置。" );

		Item* pItem = mItemList.Begin() + nIndex;
	}
	//--------------------------------------------------------------------------------------------------------------
	//向用户界面对象发送消息
	LONG_PTR GuiComboBox::SendMsg( LONG_PTR nGM, LONG_PTR nGMA, LONG_PTR nGMB )
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
		}

		return FALSE;
	}
	//--------------------------------------------------------------------------------------------------------------
	//显示下拉列表
	void FKFastcall GuiComboBox::_ShowList()
	{
		//计算下拉列表显示位置
		UINT nNumItems = mItemList.Size();

		//显示列表需要高度
		int nListHeight = mItemHeight * nNumItems + mBoarderHeight;

		//选择框下可用高度
		int nChoosenBottom = mLoca.Y + mLoca.Height;
		int nRemainHeight =  OverlayManager::mpSingleton->mRenderHeight - nChoosenBottom;
		int nListTop;

		//如果选择框下空间足够显示
		if( nRemainHeight >= nListHeight )
		{
			nListTop = nChoosenBottom;

			//显示下边框
			mpBoarder->SetSpriteUV( &mBottomBoarderUV );
			mpBoarder->SetLocation( (float)mX, (float)( nListTop + nListHeight - mBoarderHeight ) );
		}
		//否则将列表框显示在其上
		else
		{
			mLoca.Y = mLoca.Y - nListHeight;
			nListTop = mLoca.Y + mBoarderHeight;

			mChoosenClient.mRect.Top += nListHeight;
			mChoosenClient.mRect.Bottom += nListHeight;

			//显示上边框
			mpBoarder->SetSpriteUV( &mTopBoarderUV );
			mpBoarder->SetLocation( (float)mX, (float)mLoca.Y );
		}

		mpBoarder->SetVisable( true );

		//更新控件用户区
		mLoca.Height += nListHeight;
		mClient.mRect.Bottom += nListHeight;

		mListClient.mRect.Top = nListTop - mLoca.Y;
		mListClient.mRect.Bottom = mListClient.mRect.Top + mItemHeight * nNumItems;

		int nTextLeft = mX + mChoosenClient.mRect.Left + mTextLeft;

		//显示选择列表项的高光遮罩精灵
		float fItemY = (float)( mnFocusItem * mItemHeight + mLoca.Y + mListClient.mRect.Top );

		//保证其显示在对话框最上层
		ZOrderType eZType;
		OverlaySurface* pZRefOverlay;
		mpDialog->GetTopZOrder( &eZType, &pZRefOverlay );
		mpHighLightItem->SetZOrder( eZType, pZRefOverlay );

		mpHighLightItem->SetLocation( mX, fItemY );
		mpHighLightItem->SetVisable( true );

		//显示列表项
		ItemList::Iterator it = mItemList.Begin();
		ItemList::Iterator end = mItemList.End();
		for(; it!=end; ++it )
		{
			Item* pItem = it;

			Sprite* pSprite = pItem->pItemSprite;
			TextSurface* pText = pItem->pItemText;

//			pSprite->SetZOrder( ZOT_Below, mpHighLightItem );
//			pText->SetZOrder( ZOT_Upon, mpHighLightItem );

			pSprite->SetLocation( mX, (float)nListTop );
			pText->SetTextPos( nTextLeft, nListTop + mTextTop );

			nListTop += mItemHeight;

			pSprite->SetVisable( true );
			pText->SetVisable( true );
		}

		mbShowList = true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//隐藏下拉列表
	void FKFastcall GuiComboBox::_HideList()
	{
		//更新控件用户区
		mLoca.Y = mY;
		mLoca.Height = mChoosenHeight;
		mClient.mRect.Bottom = mChoosenHeight;

		mChoosenClient.mRect.Top = mTextTop;
		mChoosenClient.mRect.Bottom = mTextTop + mItemHeight;

		//隐藏下拉列表
		mpBoarder->SetVisable( false );

		ItemList::Iterator it = mItemList.Begin();
		ItemList::Iterator end = mItemList.End();
		for(; it!=end; ++it )
		{
			Item* pItem = it;
			pItem->pItemSprite->SetVisable( false );
			pItem->pItemText->SetVisable( false );
		}

		//隐藏选择列表项的高光遮罩精灵
		mpHighLightItem->SetVisable( false );

		mbShowList = false;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================

