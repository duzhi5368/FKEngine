/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Label
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../Include/Label.h"

//--------------------------------------------------------------------
namespace FK2DEngine
{
	//--------------------------------------------------------------------
	void CLable::Create( IGUIControl* p_pParent, int p_nLeft, int p_nTop,const std::wstring& p_szCaption )
	{
		IGUIControl::Create();

		SetParent( p_pParent );
		SetLeft( p_nLeft );
		SetTop( p_nTop );
		SetCaption( p_szCaption );
		if( m_pColor == NULL )
		{
			SetFontColor( *(new CColor( CColor::WHITE )) );
		}
	}
	//--------------------------------------------------------------------
	void CLable::FKDraw() const
	{
		if( !m_bVisible || m_pFont == NULL )
		{
			return ;
		}

		POINT pt = { 0, 0 };
		ClientToScreen( pt );
		m_pFont->FKDraw( m_szCaption, pt.x, pt.y, GetZPos(),
			1.0, 1.0, *m_pColor );
	}
	//--------------------------------------------------------------------
}