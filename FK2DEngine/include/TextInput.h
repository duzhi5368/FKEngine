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

#pragma once

//--------------------------------------------------------------------
#include "InsidePreInclude.h"
#include "Platform.h"
#include "../../depend/boost/scoped_ptr.hpp"
#include <string>
//--------------------------------------------------------------------
namespace FK2DEngine
{
	class CTextInput
	{
		struct		SImpl;
	private:
		boost::scoped_ptr< SImpl >		m_pImpl;
	public:
		CTextInput();
		virtual ~CTextInput();
	public:
		std::wstring Text() const;
		void SetText( const std::wstring& p_szText );

		unsigned int CarePos() const;
		void SetCarePos( unsigned int p_unPos );

		unsigned int SelectionStart() const;
		void SetSelectionStart( unsigned int p_unPos );

		#if defined( FK_MAC )
			bool FeedNSEvent( void* p_pEvent );
		#elif defined ( FK_WINDOWS )
			bool FeedMessage( unsigned long p_ulMessage, unsigned long p_wParam, unsigned long p_lParam );
		#elif defined ( FK_X )
			bool FeedXEvent( void* p_pDisplay, void* p_pEvent );
		#endif

		virtual std::wstring Filter( const std::wstring& p_TextIn ) const
		{
			return p_TextIn;
		}
	};
}