/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Form
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../Include/Form.h"

//--------------------------------------------------------------------
namespace FK2DEngine
{
	//--------------------------------------------------------------------
	CForm::CForm()
		: m_bHolding ( false )
		, m_bMoveable( false )
	{
		
	}
	//--------------------------------------------------------------------
	void CForm::Create( IGUIControl* p_pParent, int p_nLeft, int p_nTop,
		int p_nWidth, int p_nHeight )
	{
		IGUIControl::Create();
		SetParent( p_pParent );
		SetLeft( p_nLeft );
		SetTop( p_nTop );
		SetWidth( p_nWidth );
		SetHeight( p_nHeight );
		if( m_pColor == NULL )
		{
			SetFontColor( *(new CColor( CColor::WHITE )) );
		}
	}
	//--------------------------------------------------------------------
	void CForm::Create( IGUIControl* p_pParent, int p_nLeft, int p_nTop,
		CBitmap& p_Bitmap )
	{
		IGUIControl::Create();
		SetParent( p_pParent );
		SetLeft( p_nLeft );
		SetTop( p_nTop );
		SetBG( p_Bitmap );
		SetWidth( p_Bitmap.Width() );
		SetHeight( p_Bitmap.Height() );
		if( m_pColor == NULL )
		{
			SetFontColor( *(new CColor( CColor::WHITE )) );
		}
	}
	//--------------------------------------------------------------------
	void CForm::SetHotRect( RECT& p_RC )
	{
		m_HotRC = p_RC;
		m_bMoveable = true;
	}
	//--------------------------------------------------------------------
	void CForm::MouseMove( POINT p_Point )
	{
		if( !m_bMoveable )
		{
			return;
		}

		if( m_bHolding )
		{
			SetLeft( p_Point.x - m_OffsetPT.x );
			SetTop( p_Point.y - m_OffsetPT.y );
		}

		IGUIControl::MouseMove( p_Point );
	}
	//--------------------------------------------------------------------
	void CForm::MouseDown( POINT p_Point, ENUM_MouseButton p_eMB )
	{
		if( !m_bMoveable )
		{
			return;
		}

		RECT rcReal;
		rcReal.left	= m_HotRC.left + m_nLeft;
		rcReal.top	= m_HotRC.top + m_nTop;
		rcReal.right= rcReal.left + m_HotRC.right - m_HotRC.left;
		rcReal.bottom = rcReal.top + m_HotRC.bottom - m_HotRC.top;

		if( p_eMB == eMB_Left && PtInRect( &rcReal, p_Point ) )
		{
			m_bHolding = true;
			m_OffsetPT.x = p_Point.x - rcReal.left;
			m_OffsetPT.y = p_Point.y - rcReal.top;
		}

		IGUIControl::MouseDown( p_Point, p_eMB );
	}
	//--------------------------------------------------------------------
	void CForm::MouseUp( POINT p_Point, ENUM_MouseButton p_eMB )
	{
		if( !m_bMoveable )
		{
			return;
		}

		if( p_eMB == eMB_Left )
		{
			m_bHolding = false;
		}

		IGUIControl::MouseUp( p_Point, p_eMB );
	}
	//--------------------------------------------------------------------
}