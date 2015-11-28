/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	PicButton
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../Include/UIControl.h"
#include "../Include/PicButton.h"
#include "../Include/UIManager.h"
#include "../Include/Graphics.h"
#include "../Include/Bitmap.h"
#include "../Include/Image.h"
#include <vector>

//--------------------------------------------------------------------
namespace FK2DEngine
{
	//--------------------------------------------------------------------
	CPicButton::CPicButton()
		: m_pBitmap( NULL )
		, m_eBS( eBS_Up )
		, m_bHolding( false )
		, m_bHovering( false )
	{
		m_DownOffset.x = 0;
		m_DownOffset.y = 0;
		m_vecImage.clear();
	}
	//--------------------------------------------------------------------
	void CPicButton::FKDraw() const
	{
		if( !m_bVisible || m_pBitmap == NULL )
		{
			return;
		}

		POINT pt = { 0, 0 };

		if( m_eBS == eBS_Down )
		{
			pt.x += m_DownOffset.x;
			pt.y += m_DownOffset.y;
		}

		ClientToScreen( pt );

		ENUM_ButtonState bs = m_eBS;

		if( !m_bEnabled )
		{
			bs = eBS_Disabled;
		}
		
		if( m_pColor == NULL )
		{
			m_vecImage[bs].FKDraw( pt.x, pt.y, GetZPos() );
		}
		else
		{
			m_vecImage[bs].FKDraw( pt.x, pt.y, GetZPos() );
		}
		
	}
	//--------------------------------------------------------------------
	void CPicButton::SetGlygh( CBitmap& p_Bitmap )
	{
		m_pBitmap = &p_Bitmap;
		m_nWidth = p_Bitmap.Width() / 4;
		m_nHeight = p_Bitmap.Height();

		m_vecImage.push_back( CImage( SSGUIManager::Instance()->GetGraphics().CreateImage( *m_pBitmap, 
			0, 0, m_nWidth, m_nHeight, EBF_TileableLeft ) ) );
		m_vecImage.push_back( CImage( SSGUIManager::Instance()->GetGraphics().CreateImage( *m_pBitmap, 
			m_nWidth*1, 0, m_nWidth, m_nHeight, EBF_TileableLeft ) ) );
		m_vecImage.push_back( CImage( SSGUIManager::Instance()->GetGraphics().CreateImage( *m_pBitmap, 
			m_nWidth*2, 0, m_nWidth, m_nHeight, EBF_TileableLeft ) ) );
		m_vecImage.push_back( CImage( SSGUIManager::Instance()->GetGraphics().CreateImage( *m_pBitmap, 
			m_nWidth*3, 0, m_nWidth, m_nHeight, EBF_TileableLeft ) ) );
	}
	//--------------------------------------------------------------------
	void CPicButton::SetDownOffset( int p_nX, int p_nY )
	{
		m_DownOffset.x = p_nX;
		m_DownOffset.y = p_nY;
	}
	//--------------------------------------------------------------------
	void CPicButton::MouseEnter()
	{
		m_bHovering = true;

		if( m_bHolding )
		{
			m_eBS = eBS_Down;
		}
		else
		{
			m_eBS = eBS_Hot;
		}

		IGUIControl::MouseEnter();
	}
	//--------------------------------------------------------------------
	void CPicButton::MouseLeave()
	{
		m_bHovering = false;
		m_eBS = eBS_Up;

		IGUIControl::MouseLeave();
	}
	//--------------------------------------------------------------------
	void CPicButton::MouseDown( POINT p_Point, ENUM_MouseButton p_eMB )
	{
		if( p_eMB == eMB_Left )
		{
			m_eBS = eBS_Down;
			m_bHolding = true;
		}

		IGUIControl::MouseDown( p_Point, p_eMB );
	}
	//--------------------------------------------------------------------
	void CPicButton::MouseUp( POINT p_Point, ENUM_MouseButton p_eMB )
	{
		if( p_eMB == eMB_Left )
		{
			if(( m_eBS == eBS_Down ) && ( OnClick.function != NULL ))
			{
				( OnClick.invoker->*OnClick.function )();
			}

			if( m_bHovering )
			{
				m_eBS = eBS_Hot;
			}
			else
			{
				m_eBS = eBS_Up;
			}

			m_bHovering = false;
		}

		IGUIControl::MouseUp( p_Point, p_eMB );
	}
	//--------------------------------------------------------------------
}