/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	CheckBox
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../Include/CheckBox.h"
#include "../Include/UIManager.h"

//--------------------------------------------------------------------
namespace FK2DEngine
{
	//--------------------------------------------------------------------
	CCheckBox::CCheckBox()
		: m_bChecked( false )
	{
		
	}
	//--------------------------------------------------------------------
	void CCheckBox::Create( IGUIControl* p_pParent, int p_nLeft, int p_nTop,
		CBitmap& p_Bitmap, const std::wstring& p_szCaption )
	{
		IGUIControl::Create();

		SetParent( p_pParent );
		SetLeft( p_nLeft );
		SetTop( p_nTop );
		SetGlygh( p_Bitmap );
		SetCaption( p_szCaption );
		if( m_pColor == NULL )
		{
			SetFontColor( *(new CColor( CColor::WHITE )) );
		}
	}
	//--------------------------------------------------------------------
	void CCheckBox::FKDraw() const
	{
		CPicButton::FKDraw();

		POINT pt = { 0, 0 };
		ClientToScreen( pt );

		if( m_bChecked && m_pBitmap != NULL )
		{
			if( m_bEnabled )
			{
				m_vecImage[4].FKDraw( pt.x, pt.y, GetZPos() );
			}
			else
			{
				m_vecImage[5].FKDraw( pt.x, pt.y, GetZPos() );
			}
		}

		if( m_pFont != NULL )
		{
			int nHeight = m_pFont->Height();
			int nOffsetY = ( m_nHeight - nHeight ) / 2;
			m_pFont->FKDraw( m_szCaption, pt.x + m_nWidth + 10, pt.y + nOffsetY, 
				GetZPos(), 1.0, 1.0, *m_pColor );
		}
	}
	//--------------------------------------------------------------------
	void CCheckBox::SetGlygh( CBitmap& p_Bitmap )
	{
		m_pBitmap = &p_Bitmap;
		m_nWidth = p_Bitmap.Width() / 6;
		m_nHeight = p_Bitmap.Height();

		for( unsigned int i = 0; i < 6; ++i )
		{
			m_vecImage.push_back( CImage( SSGUIManager::Instance()->GetGraphics().CreateImage( *m_pBitmap, 
				m_nWidth * i, 0, m_nWidth, m_nHeight, EBF_TileableLeft ) ) );
		}
	}
	//--------------------------------------------------------------------
	void CCheckBox::Check()
	{
		m_bChecked = true;
	}
	//--------------------------------------------------------------------
	void CCheckBox::UnCheck()
	{
		m_bChecked = false;
	}
	//--------------------------------------------------------------------
	bool CCheckBox::IsChecked()
	{
		return m_bChecked;
	}
	//--------------------------------------------------------------------
	void CCheckBox::MouseUp( POINT p_Point, ENUM_MouseButton p_eMB )
	{
		if(( p_eMB == eMB_Left ) && ( m_eBS == eBS_Down ))
		{
			m_bChecked = !m_bChecked;
		}

		CPicButton::MouseUp( p_Point, p_eMB );
	}
	//--------------------------------------------------------------------
}