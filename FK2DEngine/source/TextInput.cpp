/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	TextInput
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../include/TextInput.h"
#include "../include/WinKey.h"
#include <algorithm>
#include <vector>
#include <wctype.h>

//--------------------------------------------------------------------
#ifndef NOMINMAX
	#define NOMINMAX
#endif
#include <Windows.h>
//--------------------------------------------------------------------
struct FK2DEngine::CTextInput::SImpl
{
	std::wstring	m_szText;
	unsigned int	m_unCarePos;
	unsigned int	m_unSelectionStart;

	SImpl()
		: m_unCarePos( 0 )
		, m_unSelectionStart( 0 )
	{

	}
};
//--------------------------------------------------------------------
FK2DEngine::CTextInput::CTextInput()
	: m_pImpl( new SImpl )
{

}
//--------------------------------------------------------------------
FK2DEngine::CTextInput::~CTextInput()
{

}
//--------------------------------------------------------------------
std::wstring FK2DEngine::CTextInput::Text() const
{
	return m_pImpl->m_szText;
}
//--------------------------------------------------------------------
void FK2DEngine::CTextInput::SetText(const std::wstring &p_szText)
{
	m_pImpl->m_szText		= p_szText;
	m_pImpl->m_unCarePos	= m_pImpl->m_unSelectionStart	= p_szText.length();
}
//--------------------------------------------------------------------
unsigned FK2DEngine::CTextInput::CarePos() const
{
	return m_pImpl->m_unCarePos;
}
//--------------------------------------------------------------------
void FK2DEngine::CTextInput::SetCarePos(unsigned int p_unPos)
{
	m_pImpl->m_unCarePos = p_unPos;
}
//--------------------------------------------------------------------
unsigned int FK2DEngine::CTextInput::SelectionStart() const
{
	return m_pImpl->m_unSelectionStart;
}
//--------------------------------------------------------------------
void FK2DEngine::CTextInput::SetSelectionStart( unsigned int p_unPos )
{
	m_pImpl->m_unSelectionStart = p_unPos;
}
//--------------------------------------------------------------------
bool FK2DEngine::CTextInput::FeedMessage(unsigned long p_ulMessage, unsigned long p_wParam, unsigned long p_lParam)
{
	if( p_ulMessage == WM_CHAR && p_wParam >= 32 && p_lParam != 127 )
	{
		if( m_pImpl->m_unCarePos != m_pImpl->m_unSelectionStart )
		{
			unsigned int unMin = std::min( m_pImpl->m_unCarePos, m_pImpl->m_unSelectionStart );
			unsigned int unMax = std::max( m_pImpl->m_unCarePos, m_pImpl->m_unSelectionStart );
			m_pImpl->m_szText.erase( m_pImpl->m_szText.begin() + unMin, m_pImpl->m_szText.begin() + unMax );
			m_pImpl->m_unCarePos = m_pImpl->m_unSelectionStart = unMin;
		}

		wchar_t szText[] = { static_cast< wchar_t >( p_wParam ), 0 };

		std::wstring szFilteredText = szText;	// Filter( szText );
		m_pImpl->m_szText.insert( m_pImpl->m_szText.begin() + m_pImpl->m_unCarePos,
			szFilteredText.begin(), szFilteredText.end() );

		m_pImpl->m_unCarePos += szFilteredText.length();
		m_pImpl->m_unSelectionStart = m_pImpl->m_unCarePos;
		return true;
	}

	bool bCtrlDown =		( GetKeyState( VK_CONTROL ) < 0 );
	bool bShiftDown =		( GetKeyState( VK_SHIFT ) < 0 );

	if( p_ulMessage == WM_KEYDOWN && p_wParam == VK_LEFT && !bCtrlDown )
	{
		if( m_pImpl->m_unCarePos > 0 )
		{
			m_pImpl->m_unCarePos -= 1;
		}

		if( !bShiftDown )
		{
			m_pImpl->m_unSelectionStart = m_pImpl->m_unCarePos;
		}

		return true;
	}

	if( p_ulMessage == WM_KEYDOWN && p_wParam == VK_RIGHT && !bCtrlDown )
	{
		if( m_pImpl->m_unCarePos < m_pImpl->m_szText.length() )
		{
			m_pImpl->m_unCarePos += 1;
		}

		if( !bShiftDown )
		{
			m_pImpl->m_unSelectionStart = m_pImpl->m_unCarePos;
		}

		return true;
	}

	if( p_ulMessage == WM_KEYDOWN && p_wParam == VK_HOME )
	{
		m_pImpl->m_unCarePos = 0;

		if( !bShiftDown )
		{
			m_pImpl->m_unSelectionStart = m_pImpl->m_unCarePos;
		}

		return true;
	}

	if( p_ulMessage == WM_KEYDOWN && p_wParam == VK_END )
	{
		m_pImpl->m_unCarePos = m_pImpl->m_szText.length();

		if( !bShiftDown )
		{
			m_pImpl->m_unSelectionStart = m_pImpl->m_unCarePos;
		}

		return true;
	}

	if( p_ulMessage == WM_KEYDOWN && p_wParam == VK_LEFT && bCtrlDown )
	{
		if( m_pImpl->m_unCarePos == m_pImpl->m_szText.length() )
		{
			--m_pImpl->m_unCarePos;
		}

		while( m_pImpl->m_unCarePos > 0 && iswspace( m_pImpl->m_szText.at( m_pImpl->m_unCarePos - 1 ) ) )
		{
			--m_pImpl->m_unCarePos;
		}

		while( m_pImpl->m_unCarePos > 0 && !iswspace( m_pImpl->m_szText.at( m_pImpl->m_unCarePos - 1 ) ) )
		{
			--m_pImpl->m_unCarePos;
		}

		if( !bShiftDown )
		{
			m_pImpl->m_unSelectionStart = m_pImpl->m_unCarePos;
		}

		return true;
	}

	if( p_ulMessage == WM_KEYDOWN && p_wParam == VK_BACK )
	{
		if( m_pImpl->m_unSelectionStart != m_pImpl->m_unCarePos )
		{
			unsigned int unMin = std::min( m_pImpl->m_unCarePos, m_pImpl->m_unSelectionStart );
			unsigned int unMax = std::max( m_pImpl->m_unCarePos, m_pImpl->m_unSelectionStart );

			m_pImpl->m_szText.erase( m_pImpl->m_szText.begin() + unMin, m_pImpl->m_szText.begin() + unMax );
			m_pImpl->m_unSelectionStart = m_pImpl->m_unCarePos = unMin;
		}
		else
		{
			unsigned int unOldCaret =  m_pImpl->m_unCarePos;
			FeedMessage( WM_KEYDOWN, VK_LEFT, p_lParam );
			m_pImpl->m_szText.erase( m_pImpl->m_szText.begin() + m_pImpl->m_unCarePos, m_pImpl->m_szText.begin() + unOldCaret );
			m_pImpl->m_unSelectionStart = m_pImpl->m_unCarePos;
		}

		return true;
	}

	if( p_ulMessage == WM_KEYDOWN && p_wParam == VK_DELETE )
	{
		if( m_pImpl->m_unSelectionStart != m_pImpl->m_unCarePos )
		{
			unsigned int unMin = std::min( m_pImpl->m_unCarePos, m_pImpl->m_unSelectionStart );
			unsigned int unMax = std::max( m_pImpl->m_unCarePos, m_pImpl->m_unSelectionStart );

			m_pImpl->m_szText.erase( m_pImpl->m_szText.begin() + unMin, m_pImpl->m_szText.begin() + unMax );
			m_pImpl->m_unSelectionStart = m_pImpl->m_unCarePos = unMin;
		}
		else
		{
			unsigned int unOldCaret =  m_pImpl->m_unCarePos;
			FeedMessage( WM_KEYDOWN, VK_RIGHT, p_lParam );
			m_pImpl->m_szText.erase( m_pImpl->m_szText.begin() + unOldCaret, m_pImpl->m_szText.begin() + m_pImpl->m_unCarePos );
			m_pImpl->m_unSelectionStart = m_pImpl->m_unCarePos = unOldCaret;
		}

		return true;
	}

	return false;
}
//--------------------------------------------------------------------