/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	ScrollBar
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../Include/ScrollBar.h"
#include "../include/UIManager.h"

//--------------------------------------------------------------------
namespace FK2DEngine
{
	//--------------------------------------------------------------------
	CScrollBar::CScrollBar()
		: m_eType( ESBT_Horizontal )
		, m_nMin( 0 )
		, m_nMax( 100 )
		, m_bHolding( false )
		, m_pSlider( NULL )
		, m_nOffset( 0 )
		, m_nSliderPos( 0 )
	{
		m_vecImages.clear();
	}
	//--------------------------------------------------------------------
	void CScrollBar::Create(  IGUIControl* p_pParent, int p_nLeft, int p_nTop,
		ENUM_ScrollBarType p_eType, CBitmap& p_Bitmap, int p_nLength )
	{
		IGUIControl::Create();

		SetParent( p_pParent );
		SetLeft( p_nLeft );
		SetTop( p_nTop );
		SetType( p_eType );
		SetLength( p_nLength );
		SetSlider( p_Bitmap );
		if( m_pColor == NULL )
		{
			SetFontColor( *(new CColor( CColor::WHITE )) );
		}
	}
	//--------------------------------------------------------------------
	void CScrollBar::FKDraw() const
	{
		if( !m_bVisible || ( m_pSlider == NULL ) )
		{
			return;
		}

		POINT pt = { 0, 0 };
		if( m_eType == ESBT_Horizontal )
		{
			pt.x = m_nSliderPos;
		}
		else
		{
			pt.y = m_nSliderPos;
		}
		ClientToScreen( pt );

		m_vecImages[0].FKDraw( pt.x, pt.y, GetZPos() );
	}
	//--------------------------------------------------------------------
	void CScrollBar::SetSlider( CBitmap& p_SliderBitmap )
	{
		m_pSlider = &p_SliderBitmap;

		if( m_eType == ESBT_Horizontal )
		{
			SetHeight( m_pSlider->Height() );
		}
		else
		{
			SetWidth( m_pSlider->Width() );
		}

		m_vecImages.push_back( CImage( SSGUIManager::Instance()->GetGraphics().CreateImage( 
			*m_pSlider, 0, 0, m_nWidth, m_nHeight, EBF_Smooth ) ) );
	}
	//--------------------------------------------------------------------
	void CScrollBar::SetLength( int p_nLength )
	{
		if( m_eType == ESBT_Horizontal )
		{
			SetWidth( p_nLength );
		}
		else
		{
			SetHeight( p_nLength );
		}
	}
	//--------------------------------------------------------------------
	int CScrollBar::GetPosition() const
	{
		int nLen = 0;

		if( m_eType == ESBT_Horizontal )
		{
			nLen = m_nWidth - m_pSlider->Width();
		}
		else
		{
			nLen = m_nHeight - m_pSlider->Height();
		}

		return m_nSliderPos * ( m_nMax - m_nMin ) / nLen + m_nMin;
	}
	//--------------------------------------------------------------------
	void CScrollBar::SetPosition( int p_nPos )
	{
		int nLen = 0;

		if( m_eType == ESBT_Horizontal )
		{
			nLen = m_nWidth - m_pSlider->Width();
		}
		else
		{
			nLen = m_nHeight - m_pSlider->Height();
		}

		m_nSliderPos = ( p_nPos - m_nMin ) * nLen / ( m_nMax - m_nMin );
	}
	//--------------------------------------------------------------------
	void CScrollBar::SetType( ENUM_ScrollBarType p_eType )
	{
		m_eType = p_eType;
	}
	//--------------------------------------------------------------------
	void CScrollBar::SetRange( int p_nMin, int p_nMax )
	{
		m_nMax = p_nMax;
		m_nMin = p_nMin;
	}
	//--------------------------------------------------------------------
	void CScrollBar::MouseMove( POINT p_Point )
	{
		if( m_bHolding )
		{
			ScreenToClient( p_Point );

			if( m_eType == ESBT_Horizontal )
			{
				m_nSliderPos = p_Point.x - m_nOffset;

				if( m_nSliderPos < 0 )
				{
					m_nSliderPos = 0;
				}
				else if( m_nSliderPos > m_nWidth - static_cast<int>( m_pSlider->Width() ) )
				{
					m_nSliderPos = m_nWidth - static_cast<int>( m_pSlider->Width() );
				}
			}
			else
			{
				m_nSliderPos = p_Point.y - m_nOffset;

				if( m_nSliderPos < 0 )
				{
					 m_nSliderPos = 0;
				}
				else if( m_nSliderPos > m_nHeight - static_cast<int>( m_pSlider->Height() ) )
				{
					m_nSliderPos = m_nHeight - static_cast<int>( m_pSlider->Height() );
				}
			}

			if( OnScroll.function != NULL )
			{
				(OnScroll.invoker->*OnScroll.function)();
			}
		}

		IGUIControl::MouseMove(p_Point);
	}
	//--------------------------------------------------------------------
	void CScrollBar::MouseDown( POINT p_Point, ENUM_MouseButton p_eMB )
	{
		if( p_eMB == eMB_Left )
		{
			ScreenToClient( p_Point );

			if( m_eType == ESBT_Horizontal )
			{
				if( ( p_Point.x >= m_nSliderPos ) && ( p_Point.x < m_nSliderPos + static_cast<int>( m_pSlider->Width() ) ) )
				{
					m_bHolding = true;
					m_nOffset = p_Point.x - m_nSliderPos;
				}
			}
			else
			{
				if( ( p_Point.y >= m_nSliderPos ) && ( p_Point.y < m_nSliderPos + static_cast<int>( m_pSlider->Height() ) ) )
				{
					m_bHolding = true;
					m_nOffset = p_Point.y - m_nSliderPos;
				}
			}
		}

		IGUIControl::MouseDown( p_Point, p_eMB );
	}
	//--------------------------------------------------------------------
	void CScrollBar::MouseUp( POINT p_Point, ENUM_MouseButton p_eMB )
	{
		if(( p_eMB == MS_Left ) && ( m_bHolding ))
		{
			m_bHolding = false;

			if( OnChange.function != NULL )
			{
				(OnChange.invoker->*OnChange.function)();
			}
		}

		IGUIControl::MouseUp( p_Point, p_eMB );
	}
	//--------------------------------------------------------------------
}