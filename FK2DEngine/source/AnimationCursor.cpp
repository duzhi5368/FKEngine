/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	AnimationCursor
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../include/AnimationCursor.h"
#include "../include/AnimationCursorManager.h"
#include "../include/Graphics.h"
#include "../include/Image.h"
#include "../include/UIInputManager.h"
#include "../include/ZPos.h"
#include "../Include/LogManager.h"
#include "../Include/StringConversion.h"

//--------------------------------------------------------------------
namespace FK2DEngine
{
	//--------------------------------------------------------------------
	CAnimationCursor::CAnimationCursor()
		: m_pBitmap( NULL )
		, m_nCurFrame( 0 )
		, m_nMaxFrame( 0 )
	{
		m_vecAnim.clear();
		m_szName = L"";
	}
	//--------------------------------------------------------------------
	CAnimationCursor::~CAnimationCursor()
	{

	}
	//--------------------------------------------------------------------
	void CAnimationCursor::Init( CBitmap& p_pBitmap, const std::wstring& p_szName,
		int p_nSingleWidth, int p_nSingleHeight )
	{
		m_pBitmap = &p_pBitmap;
		int nWidthLine = m_pBitmap->Width() / p_nSingleWidth;
		int nHeightLine = m_pBitmap->Height() / p_nSingleHeight;

		m_nMaxFrame = nWidthLine * nHeightLine;

		for( int i = 0; i < nWidthLine; ++i )
		{
			for( int j = 0; j < nHeightLine; ++j )
			{
				m_vecAnim.push_back( CImage( SSAnimationCursorManager::Instance()->GetGraphics().CreateImage( 
					*m_pBitmap, i * p_nSingleWidth, j * p_nSingleHeight,
					p_nSingleWidth, p_nSingleHeight, EBF_Tileable ) ) );
			}
		}
	}
	//--------------------------------------------------------------------
	void CAnimationCursor::FKDraw( bool p_bUpdate )
	{
		if( p_bUpdate )
		{
			m_nCurFrame = ( m_nCurFrame + 1 ) % m_nMaxFrame;
		}
		m_vecAnim[m_nCurFrame].FKDraw( SSGUIInputManager::Instance()->GetMousePosX(),
			SSGUIInputManager::Instance()->GetMousePosY(), eDZP_CursorBegin,
			1.0, 1.0, 0xffffffff, EAM_Mutiply );
	}
	//--------------------------------------------------------------------
	std::wstring CAnimationCursor::GetName() const
	{
		return m_szName;
	}
	//--------------------------------------------------------------------
}