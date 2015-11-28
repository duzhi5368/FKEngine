/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	UIControl
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../Include/UIControl.h"
#include "../Include/UIManager.h"
#include "../Include/Exception.h"
#include "../include/ZPos.h"

//--------------------------------------------------------------------
namespace FK2DEngine
{
	//--------------------------------------------------------------------
	IGUIControl::IGUIControl()
		: m_pParent( NULL )
		, m_nLeft(0)
		, m_nTop(0)
		, m_nWidth(0)
		, m_nHeight(0)
		, m_bEnabled(true)
		, m_bVisible(true)
		, m_bFocused(false)
		, m_pFont( NULL )
	{
		m_pColor = NULL;
		SetZPos( eDZP_UIBegin );
	}
	//--------------------------------------------------------------------
	IGUIControl::~IGUIControl()
	{
		if( m_pParent != NULL )
		{
			m_pParent->DelChild( this );
		}

		std::list< IGUIControl* >::iterator Ite = m_listChild.begin();
		for( ; Ite != m_listChild.end(); ++Ite )
		{
			(*Ite)->m_pParent = NULL;
			SSGUIManager::Instance()->DelCtrl( *Ite );
		}
		SSGUIManager::Instance()->DelCtrl( this );
	}
	//--------------------------------------------------------------------
	void IGUIControl::MouseEnter()
	{
		if( OnMouseEnter.function != NULL )
		{
			( OnMouseEnter.invoker->*OnMouseEnter.function )();
		}
	}
	//--------------------------------------------------------------------
	void IGUIControl::MouseLeave()
	{
		if( OnMouseLeave.function != NULL )
		{
			( OnMouseLeave.invoker->*OnMouseLeave.function )();
		}
	}
	//--------------------------------------------------------------------
	void IGUIControl::MouseMove( POINT p_Point )
	{
		if( OnMouseMove.function != NULL )
		{
			ScreenToClient( p_Point );
			( OnMouseMove.invoker->*OnMouseMove.function )( p_Point );
		}
	}
	//--------------------------------------------------------------------
	void IGUIControl::MouseDown( POINT p_Point, ENUM_MouseButton p_eMB )
	{
		if( OnMouseDown.function != NULL )
		{
			ScreenToClient( p_Point );
			( OnMouseDown.invoker->*OnMouseDown.function )( p_Point, p_eMB );
		}
	}
	//--------------------------------------------------------------------
	void IGUIControl::MouseUp( POINT p_Point, ENUM_MouseButton p_eMB )
	{
		if (OnMouseUp.function != NULL)
		{
			ScreenToClient( p_Point );
			(OnMouseUp.invoker->*OnMouseUp.function)( p_Point, p_eMB );
		}
	}
	//--------------------------------------------------------------------
	void IGUIControl::KeyDown( unsigned long p_ulKey, SShiftState p_State )
	{
		if (OnKeyDown.function != NULL)
		{
			(OnKeyDown.invoker->*OnKeyDown.function)( p_ulKey, p_State );
		}
	}
	//--------------------------------------------------------------------
	void IGUIControl::KeyUp( unsigned long p_ulKey, SShiftState p_State )
	{
		if (OnKeyUp.function != NULL)
		{
			(OnKeyUp.invoker->*OnKeyUp.function)( p_ulKey, p_State );
		}
	}
	//--------------------------------------------------------------------
	void IGUIControl::KeyPress( char p_cKey )
	{
		if (OnKeyPress.function != NULL)
		{
			(OnKeyPress.invoker->*OnKeyPress.function)( p_cKey );
		}
	}
	//--------------------------------------------------------------------
	void IGUIControl::Enter()
	{
		m_bFocused = true;
		if (OnEnter.function != NULL)
		{
			(OnEnter.invoker->*OnEnter.function)();
		}
	}
	//--------------------------------------------------------------------
	void IGUIControl::Leave()
	{
		m_bFocused = false;
		if (OnLeave.function != NULL)
		{
			(OnLeave.invoker->*OnLeave.function)();
		}
	}
	//--------------------------------------------------------------------
	void IGUIControl::Create()
	{
		SSGUIManager::Instance()->AddCtrl( this );
	}
	//--------------------------------------------------------------------
	void IGUIControl::SetParent( IGUIControl* p_pParent )
	{
		if( m_pParent == p_pParent )
		{
			return;
		}

		if( m_pParent != NULL )
		{
			m_pParent->DelChild( this );
		}

		m_pParent = p_pParent;

		if( m_pParent != NULL )
		{
			m_pParent->AddChild( this );
			//继承父类字体格式
			if( m_pFont == NULL )
			{
				m_pFont = m_pParent->GetFont();
			}

			SetZPos( m_pParent->GetZPos() + 1 );
		}
	}
	//--------------------------------------------------------------------
	void IGUIControl::AddChild( IGUIControl* p_pChild )
	{
		FKAssert( p_pChild != NULL , L"GUIControl不允许加载空的子控件" );

		std::list< IGUIControl* >::iterator Ite = m_listChild.begin();

		for( ; Ite != m_listChild.end(); ++Ite )
		{
			if( (*Ite) == p_pChild )
			{
				return;
			}
		}

		m_listChild.push_back( p_pChild );
	}
	//--------------------------------------------------------------------
	void IGUIControl::DelChild( IGUIControl* p_pChild )
	{
		FKAssert( p_pChild != NULL , L"GUIControl不允许删除空的子控件" );

		std::list< IGUIControl* >::iterator Ite = m_listChild.begin();

		for( ; Ite != m_listChild.end(); ++Ite )
		{
			if( (*Ite) == p_pChild )
			{
				m_listChild.erase( Ite );
				return;
			}
		}
	}
	//--------------------------------------------------------------------
	void IGUIControl::Enable()
	{
		m_bEnabled = true;
		std::list< IGUIControl* >::iterator Ite = m_listChild.begin();

		for( ; Ite != m_listChild.end(); ++Ite )
		{
			(*Ite)->Enable();
		}
	}
	//--------------------------------------------------------------------
	void IGUIControl::Disable()
	{
		m_bEnabled = false;
		std::list< IGUIControl* >::iterator Ite = m_listChild.begin();

		for( ; Ite != m_listChild.end(); ++Ite )
		{
			(*Ite)->Disable();
		}
	}
	//--------------------------------------------------------------------
	void IGUIControl::Show()
	{
		m_bVisible = true;
		std::list< IGUIControl* >::iterator Ite = m_listChild.begin();

		for( ; Ite != m_listChild.end(); ++Ite )
		{
			(*Ite)->Show();
		}
	}
	//--------------------------------------------------------------------
	void IGUIControl::Hide()
	{
		m_bVisible = false;
		std::list< IGUIControl* >::iterator Ite = m_listChild.begin();

		for( ; Ite != m_listChild.end(); ++Ite )
		{
			(*Ite)->Hide();
		}
	}
	//--------------------------------------------------------------------
	void IGUIControl::ClientToScreen( POINT &p_Point ) const
	{
		if( m_pParent != NULL )
		{
			m_pParent->ClientToScreen( p_Point );
		}
		p_Point.x += m_nLeft;
		p_Point.y += m_nTop;
	}
	//--------------------------------------------------------------------
	void IGUIControl::ScreenToClient( POINT &p_Point ) const
	{
		if( m_pParent != NULL )
		{
			m_pParent->ScreenToClient( p_Point );
		}
		p_Point.x -= m_nLeft;
		p_Point.y -= m_nTop;
	}
	//--------------------------------------------------------------------
	bool IGUIControl::IsPointIn( POINT p_Point ) const
	{
		ScreenToClient( p_Point );
		if( p_Point.x >= 0 && p_Point.x < m_nWidth && p_Point.y >= 0 && p_Point.y < m_nHeight )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	//--------------------------------------------------------------------
	void IGUIControl::SetFocus()
	{
		if( CanHaveFocus() )
		{
			SSGUIManager::Instance()->SetFocus( this );
		}
	}
	//--------------------------------------------------------------------
	bool IGUIControl::IsEnabled() const
	{
		return m_bEnabled;
	}
	//--------------------------------------------------------------------
	bool IGUIControl::IsVisiable() const
	{
		return m_bVisible;
	}
	//--------------------------------------------------------------------
	bool IGUIControl::IsFocesed() const
	{	
		return m_bFocused;
	}
	//--------------------------------------------------------------------
	void IGUIControl::SetLeft( int p_nLeft )
	{
		m_nLeft = p_nLeft;
	}
	//--------------------------------------------------------------------
	void IGUIControl::SetTop( int p_nTop )
	{
		m_nTop = p_nTop;
	}
	//--------------------------------------------------------------------
	int IGUIControl::GetLeft() const
	{
		return m_nLeft;
	}
	//--------------------------------------------------------------------
	int IGUIControl::GetTop() const
	{
		return m_nTop;
	}
	//--------------------------------------------------------------------
	void IGUIControl::SetWidth( int p_nWidth )
	{
		m_nWidth = p_nWidth;
	}
	//--------------------------------------------------------------------
	void IGUIControl::SetHeight( int p_nHeight )
	{
		m_nHeight = p_nHeight;
	}
	//--------------------------------------------------------------------
	int IGUIControl::GetWidth() const
	{
		return m_nWidth;
	}
	//--------------------------------------------------------------------
	int IGUIControl::GetHeight() const
	{
		return m_nHeight;
	}
	//--------------------------------------------------------------------
	void IGUIControl::SetFont( CFont &p_Font )
	{
		m_pFont = &p_Font;
	}
	//--------------------------------------------------------------------
	CFont* IGUIControl::GetFont() const
	{
		return m_pFont;
	}
	//--------------------------------------------------------------------
	void IGUIControl::SetFontColor( CColor &p_Color )
	{
		m_pColor = &p_Color;
	}
	//--------------------------------------------------------------------
	CColor* IGUIControl::GetColor() const
	{
		return m_pColor;
	}
	//--------------------------------------------------------------------
	void IGUIControl::SetCaption( const std::wstring& p_szCaption )
	{		
		m_szCaption = p_szCaption;
	}
	//--------------------------------------------------------------------
	std::wstring IGUIControl::GetCaption() const
	{
		return m_szCaption;
	}
	//--------------------------------------------------------------------
	void IGUIControl::SetZPos( ZPos p_nZPos )
	{
		FKAssert( ( ( p_nZPos <= eDZP_UIEnd ) && ( p_nZPos >= eDZP_UIBegin ) ), "UI控件Z深度设置不合法" );

		ZPos zPos = p_nZPos;
		if( zPos > eDZP_UIEnd )
		{
			zPos = eDZP_UIEnd;
		}
		if( zPos < eDZP_UIBegin )
		{
			zPos = eDZP_UIBegin;
		}
		m_dZpos = zPos;
	}
	//--------------------------------------------------------------------
	ZPos IGUIControl::GetZPos() const
	{
		FKAssert( ( ( m_dZpos <= eDZP_UIEnd ) && ( m_dZpos >= eDZP_UIBegin ) ), "UI控件Z深度不合法，获取失败" );
		return m_dZpos;
	}
	//--------------------------------------------------------------------
}