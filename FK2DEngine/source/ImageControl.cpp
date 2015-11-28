/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	ImageControl
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../Include/ImageControl.h"
#include "../Include/UIManager.h"

//--------------------------------------------------------------------
namespace FK2DEngine
{
	//--------------------------------------------------------------------
	CImageControl::CImageControl()
		: m_pBitmap( NULL )
	{
		m_vecImages.clear();
	}
	//--------------------------------------------------------------------
	void CImageControl::Create( IGUIControl* p_pParent, int p_nLeft, int p_nTop,
		CBitmap& p_Bitmap )
	{
		IGUIControl::Create();

		SetParent( p_pParent );
		SetLeft( p_nLeft );
		SetTop( p_nTop );
		SetImage( p_Bitmap );

		if( m_pColor == NULL )
		{
			SetFontColor( *(new CColor( CColor::WHITE )) );
		}
	}
	//--------------------------------------------------------------------
	void CImageControl::FKDraw() const
	{
		if( !m_bVisible || m_pBitmap == NULL )
		{
			return;
		}

		m_vecImages[0].FKDraw( m_nLeft, m_nTop, GetZPos() );
	}
	//--------------------------------------------------------------------
	void CImageControl::SetImage( CBitmap& p_Bitmap )
	{
		m_pBitmap = &p_Bitmap;
		m_nWidth = p_Bitmap.Width();
		m_nHeight = p_Bitmap.Height();

		m_vecImages.push_back( CImage( SSGUIManager::Instance()->GetGraphics().CreateImage( 
			*m_pBitmap, 0, 0, m_pBitmap->Width(), m_pBitmap->Height(), EBF_Smooth ) ) );
	}
	//--------------------------------------------------------------------
}