/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Panel
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../include/Panel.h"

//--------------------------------------------------------------------
namespace FK2DEngine
{
	//--------------------------------------------------------------------
	void CPanel::Create( IGUIControl* p_pParent, int p_nLeft, int p_nTop,
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
	void CPanel::Create( IGUIControl* p_pParent, int p_nLeft, int p_nTop,
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
}