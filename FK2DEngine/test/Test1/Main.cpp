/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Main
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../../interface/FKEngineInclude.h"
#include "../../../depend/boost/scoped_ptr.hpp"
#include <cwctype>
#include <algorithm>

//--------------------------------------------------------------------
//#ifdef NDEBUG
//#pragma comment(lib, "FK2DEngine.lib")
//#else
//#pragma comment(lib, "FK2DEngine_D.lib")
//#endif
//--------------------------------------------------------------------
class CTextField : public FK2DEngine::CTextInput
{
private:
	FK2DEngine::CFKWindow&			m_pWindow;
	FK2DEngine::CFont&				m_pFont;
	double							m_dX;
	double							m_dY;
	DWORD							m_dLastTime;
public:
	static const unsigned long		INACTIVE_COLOR	= 0xcc666666;
	static const unsigned long		ACTIVE_COLOR    = 0xccff6666;
	static const unsigned long		SELECTION_COLOR = 0xcc0000ff;
	static const unsigned long		CARET_COLOR     = 0xffffffff;
	static const int				PADDING			= 5;
public:
	CTextField( FK2DEngine::CFKWindow& p_Window, FK2DEngine::CFont& p_Font, double p_dX, double p_dY )
		: m_pWindow( p_Window )
		, m_pFont( p_Font )
		, m_dX( p_dX )
		, m_dY( p_dY )
		, m_dLastTime( 0 )
	{
		SetText( L"请点击..." );
	}

	static wchar_t ToUpper( wchar_t p_szIn )
	{
		using namespace std;
		return towupper( (wint_t)p_szIn );
	}

	std::wstring Filter( const std::wstring& p_szString ) const
	{
		std::wstring Result;
		Result.resize( p_szString.length() );
		std::transform( p_szString.begin(), p_szString.end(), Result.begin(), ToUpper );
		return Result;
	}

	void FKDraw( DWORD p_dwElapseTime )
	{
		FK2DEngine::CColor BackgroundColor;
		if( m_pWindow.Input().TextInput() == this )
		{
			BackgroundColor = ACTIVE_COLOR;
		}
		else
		{
			BackgroundColor = INACTIVE_COLOR;
		}

		m_pWindow.Graphics().DrawQuad(
				m_dX - PADDING,				m_dY - PADDING,				BackgroundColor,
				m_dX + Width() + PADDING,	m_dY - PADDING,				BackgroundColor,
				m_dX - PADDING,				m_dY + Height() + PADDING,	BackgroundColor,
				m_dX + Width() + PADDING,	m_dY + Height() + PADDING,	BackgroundColor, 0
			);

		double dPosX = m_dX + m_pFont.TextWidth( Text().substr( 0, CarePos() ) );
		double dSelX = m_dX + m_pFont.TextWidth( Text().substr( 0, SelectionStart() ) );

		m_pWindow.Graphics().DrawQuad(
				dSelX,		m_dY,			SELECTION_COLOR,
				dPosX,		m_dY,			SELECTION_COLOR,
				dSelX,		m_dY + Height(),SELECTION_COLOR,
				dPosX,		m_dY + Height(),SELECTION_COLOR, 0
			);

		if( m_pWindow.Input().TextInput() == this )
		{
			if(( p_dwElapseTime - m_dLastTime > 1000 ) || ( p_dwElapseTime - m_dLastTime < 500 ))
			{
				if( p_dwElapseTime - m_dLastTime > 1000 )
				{
					m_dLastTime = p_dwElapseTime;
				}
				m_pWindow.Graphics().DrawLine( dPosX, m_dY,	CARET_COLOR, dPosX, m_dY + Height(), CARET_COLOR, 0 );
			}
		}

		m_pFont.FKDraw( Text(), m_dX, m_dY, 0 );
	}

	double Width() const
	{
		return m_pFont.TextWidth( Text() );
	}

	double Height() const
	{
		return m_pFont.Height();
	}

	bool IsUnderPoint( double p_dMouseX, double p_dMouseY )
	{
		return ( ( p_dMouseX > m_dX - PADDING ) 
			&& ( p_dMouseX < m_dX + Width() + PADDING )
			&& ( p_dMouseY > m_dY - PADDING )
			&& ( p_dMouseY < m_dY + Height() + PADDING ) );
	}
};
//--------------------------------------------------------------------
template< typename T, std::size_t Len >
std::size_t LengthOf( const T(&) [Len] )
{
	return Len;
}
//--------------------------------------------------------------------
class CTextInputWindow : public FK2DEngine::CFKWindow
{
private:
	boost::scoped_ptr< FK2DEngine::CFont >			m_pFont;
	boost::scoped_ptr< CTextField >					m_pTextFields[3];
	boost::scoped_ptr< FK2DEngine::CImage >			m_pCursor;
	FK2DEngine::CTimer								m_Timer;
public:
	CTextInputWindow()
		: FK2DEngine::CFKWindow( 300, 200, false )
	{
		SetCaption( L"自由骑士笃志引擎：字符输入测试" );
		SetBackgroudColor( FK2DEngine::CColor::YELLOW );
		// 设置字体
		m_pFont.reset( new FK2DEngine::CFont( Graphics(), L"Sans MT 936_S60", 20 ) );

		for( unsigned int nIndex = 0; nIndex < LengthOf( m_pTextFields ); ++nIndex )
		{
			m_pTextFields[nIndex].reset( new CTextField( *this, *m_pFont, 50, 30 + nIndex * 50 ) );
		}
		// 开动计时器
		m_Timer.Play();
		// 隐藏真实鼠标，自定虚拟鼠标
		::ShowCursor( FALSE );
		// 设置自己鼠标图片
		m_pCursor.reset( new FK2DEngine::CImage( Graphics(), FK2DEngine::ShareResourcePrefix() + L"rc/media/Cursor.png", false ) );
	}

	void FKDraw()
	{
		for( unsigned int i = 0; i < LengthOf( m_pTextFields ); ++i )
		{
			m_pTextFields[i]->FKDraw( m_Timer.GetTime() );
		}

		m_pCursor->FKDraw( Input().MouseX(), Input().MouseY(), 0 );
	}

	void KeyDown( FK2DEngine::CKey p_Btn )
	{
		if( p_Btn == FK2DEngine::EKB_Tab )
		{
			int nIndex = -1;
			for( unsigned int i = 0; i < LengthOf( m_pTextFields ); ++i )
			{
				if( Input().TextInput() == m_pTextFields[i].get() )
				{
					nIndex = i;
				}
			}

			Input().SetTextInput( m_pTextFields[(nIndex + 1) % LengthOf( m_pTextFields )].get() );
		}
		else if( p_Btn == FK2DEngine::EKB_Escape )
		{
			if( Input().TextInput() )
			{
				Input().SetTextInput( NULL );
			}
			else
			{
				Close();
			}
		}
		else if( p_Btn == FK2DEngine::MS_Left )
		{
			Input().SetTextInput( NULL );

			for( unsigned int i = 0; i < LengthOf(m_pTextFields); ++i )
			{
				if( m_pTextFields[i]->IsUnderPoint( Input().MouseX(), Input().MouseY() ) )
				{
					Input().SetTextInput( m_pTextFields[i].get() );
				}
			}
		}
	}
};
//--------------------------------------------------------------------
int main( int p_nArgc, char* p_Argv[] )
{
	CTextInputWindow win;
	win.Show();
	return 0;
}
//--------------------------------------------------------------------