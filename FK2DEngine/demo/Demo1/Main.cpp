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

//--------------------------------------------------------------------
//#ifdef NDEBUG
//#pragma comment(lib, "FK2DEngine.lib")
//#else
//#pragma comment(lib, "FK2DEngine_D.lib")
//#endif
//--------------------------------------------------------------------
#include "../../../depend/boost/scoped_ptr.hpp"
#include "../../../depend/boost/shared_ptr.hpp"
#include "../../../depend/boost/lexical_cast.hpp"
#include <cmath>
#include <cstdlib>
#include <list>
#include <vector>
//--------------------------------------------------------------------
enum ENUM_ZOrder
{
	eZ_Backgroud,
	eZ_Stars,
	eZ_Player,
	eZ_UI,
};
//--------------------------------------------------------------------
typedef std::vector< boost::shared_ptr< FK2DEngine::CImage > >		Animation;
//--------------------------------------------------------------------
class CStar
{
private:
	Animation*			m_pAnimation;
	FK2DEngine::CColor	m_pColor;
	double				m_dPosX;
	double				m_dPosY;
public:
	explicit CStar( Animation& p_Anim )
	{
		m_pAnimation = &p_Anim;

		m_pColor.SetAlpha( 255 );
		double dRed = FK2DEngine::Random( 40, 255 );
		m_pColor.SetRed( static_cast< FK2DEngine::CColor::Channel >( dRed ) );
		double dGreen = FK2DEngine::Random( 40, 255 );
		m_pColor.SetGreen( static_cast< FK2DEngine::CColor::Channel >( dGreen ) );
		double dBlue = FK2DEngine::Random( 40, 255 );
		m_pColor.SetBlue( static_cast< FK2DEngine::CColor::Channel >( dBlue ) );

		m_dPosX = FK2DEngine::Random( 0, 640 );
		m_dPosY = FK2DEngine::Random( 0, 640 );
	}

	double X() const
	{
		return m_dPosX;
	}

	double Y() const
	{
		return m_dPosY;
	}

	void FKDraw() const
	{
		FK2DEngine::CImage& img = *m_pAnimation->at( FK2DEngine::MilliSeconds() / 100 % m_pAnimation->size() );

		img.FKDraw( m_dPosX - img.Width() / 2.0, m_dPosY - img.Height() / 2.0,
			eZ_Stars, 1, 1, m_pColor, FK2DEngine::EAM_Additive );
	}
};
//--------------------------------------------------------------------
class CPlayer
{
private:
	boost::scoped_ptr< FK2DEngine::CImage >		m_pImg;
	boost::scoped_ptr< FK2DEngine::CFKSample >	m_pBeep;
	double										m_dPosX;
	double										m_dPosY;
	double										m_dVelX;
	double										m_dVelY;
	double										m_dAngel;
	unsigned int								m_unScore;
public:
	CPlayer( FK2DEngine::CGraphics& p_Graphics, FK2DEngine::CAudio& p_Audio )
	{
		std::wstring szFileName = FK2DEngine::ShareResourcePrefix() + L"rc/media/Starfighter.bmp";
		m_pImg.reset( new FK2DEngine::CImage( p_Graphics, szFileName ) );

		szFileName = FK2DEngine::ShareResourcePrefix() + L"rc/media/Beep.wav";
		m_pBeep.reset( new FK2DEngine::CFKSample( p_Audio, szFileName ) );

		m_dPosX = m_dPosY = m_dVelX = m_dVelY = m_dAngel = 0;
		m_unScore = 0;
	}

	unsigned int GetScore() const
	{
		return m_unScore;
	}

	void Warp( double p_dX, double p_dY )
	{
		m_dPosX = p_dX;
		m_dPosY = p_dY;
	}

	void TurnLeft()
	{
		m_dAngel -= 4.5;
	}

	void TurnRight()
	{
		m_dAngel += 4.5;
	}

	void Accelerate()
	{
		m_dVelX += FK2DEngine::OffsetX( m_dAngel, 0.5 );
		m_dVelY += FK2DEngine::OffsetY( m_dAngel, 0.5 );
	}

	void Move()
	{
		m_dPosX += m_dVelX;
		while( m_dPosX < 0 )
		{
			m_dPosX += 640;
		}
		while( m_dPosX > 640 )
		{
			m_dPosX -= 640;
		}

		m_dPosY += m_dVelY;
		while( m_dPosY < 0 )
		{
			m_dPosY += 480;
		}
		while( m_dPosY > 480 )
		{
			m_dPosY -= 480;
		}

		m_dVelX *= 0.95;
		m_dVelY *= 0.95;
	}

	void FKDraw() const
	{
		m_pImg->DrawRot( m_dPosX, m_dPosY, eZ_Player, m_dAngel );
	}

	void CollectStars( std::list<CStar>& p_Stars )
	{
		std::list<CStar>::iterator IteCur = p_Stars.begin();

		while( IteCur != p_Stars.end() )
		{
			if( FK2DEngine::Distance( m_dPosX, m_dPosY, IteCur->X(), IteCur->Y() ) < 35 )
			{
				IteCur = p_Stars.erase( IteCur );
				m_unScore += 10;
				m_pBeep->Play();
			}
			else
			{
				++IteCur;
			}
		}
	}
};
//--------------------------------------------------------------------
class CGameWindow : public FK2DEngine::CFKWindow
{
private:
	boost::scoped_ptr< FK2DEngine::CImage >		m_pBackgroundImage;
	Animation									m_StarAnim;
	CPlayer										m_Player;
	std::list<CStar>							m_Stars;
	FK2DEngine::CFont							m_Font;
	CBitmap										m_pCursorBitmap;
	int											m_nFps;
	int											m_nLastFps;
public:
	CGameWindow()
		: CFKWindow( 640,480,false )
		, m_Player( Graphics(), Audio() )
		, m_Font( Graphics(), FK2DEngine::DefaultFontName(), 24 )
		, m_nFps( 0 )
		, m_nLastFps( 0 )
	{
		SetCaption( L"自由骑士笃志引擎：DEMO1" );

		std::wstring szFileName = FK2DEngine::ShareResourcePrefix() + L"rc/media/Space.png";
		m_pBackgroundImage.reset( new FK2DEngine::CImage( Graphics(), szFileName, false ) );

		szFileName = FK2DEngine::ShareResourcePrefix() + L"rc/media/Star.png";
		FK2DEngine::ImagesFromTiledBitmap( Graphics(), szFileName, 25, 25, false, m_StarAnim );

		m_Player.Warp( 320, 240 );

		//szFileName = FK2DEngine::ShareResourcePrefix() + L"avgRc\\Attack.cur";
		//SetNewCursor( szFileName );

		szFileName = FK2DEngine::ShareResourcePrefix() + L"avgRc\\AnimCursor.bmp";
		m_pCursorBitmap = LoadImageFile( szFileName );
		SSAnimationCursorManager::Instance()->AddCursor( m_pCursorBitmap, L"默认鼠标", 32, 32 );
		SSAnimationCursorManager::Instance()->SetAnimCursor( L"默认鼠标" );
		SSAnimationCursorManager::Instance()->Enable( true );
	}
	
	void Update( float p_fDelta )
	{
		if( Input().Down( FK2DEngine::EKB_Left ) || Input().Down( FK2DEngine::GP_Left ) )
		{
			m_Player.TurnLeft();
		}
		if( Input().Down( FK2DEngine::EKB_Right ) || Input().Down( FK2DEngine::GP_Right ) )
		{
			m_Player.TurnRight();
		}
		if( Input().Down( FK2DEngine::EKB_Up ) || Input().Down( FK2DEngine::GP_Button0 ) )
		{
			m_Player.Accelerate();
		}

		m_Player.Move();
		m_Player.CollectStars( m_Stars );

		if( std::rand() % 25 == 0 && m_Stars.size() < 25 )
		{
			m_Stars.push_back( CStar(m_StarAnim) );
		}

		m_nFps = FK2DEngine::GetFPS();
	}

	void FKDraw()
	{
		m_Player.FKDraw();
		m_pBackgroundImage->FKDraw( 0, 0, eZ_Backgroud );

		for( std::list<CStar>::const_iterator i = m_Stars.begin(); 
			i != m_Stars.end(); ++i )
		{
			i->FKDraw();
		}

		if( m_nLastFps != m_nFps )
		{
			m_nLastFps = m_nFps;
		}
		m_Font.FKDraw(L"FPS:" + boost::lexical_cast< std::wstring >( m_nLastFps ), 510, 10, eZ_UI, 1, 1, FK2DEngine::CColor::BLUE );

		m_Font.FKDraw( L"得分：" + boost::lexical_cast< std::wstring >( m_Player.GetScore()),
			10, 10, eZ_UI, 1, 1, FK2DEngine::CColor::YELLOW );
	}

	void KeyDown( FK2DEngine::CKey p_Key )
	{
		if( p_Key == FK2DEngine::EKB_Escape )
		{
			Close();
		}
		else if( p_Key == FK2DEngine::EKB_PrintScreen )
		{
			PrintScreen( FK2DEngine::ShareResourcePrefix() + L"Screen\\" + ToString( GetTime() ) + L".bmp" );
		}
	}

	bool OnClose()
	{
		if( MessageBox( NULL, L"确定退出吗？", L"提示", MB_OKCANCEL | MB_ICONINFORMATION ) == IDOK )
		{
			return true;
		}
		return false;
	}
};
//--------------------------------------------------------------------
// 开启CRT检查
#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC 
	#include<stdlib.h> 
	#include<crtdbg.h>
	#define new new( _NORMAL_BLOCK, __FILE__, __LINE__ ) 
#endif
//--------------------------------------------------------------------
int main( int p_nArgc, char* p_Argv[] )
{
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetBreakAlloc( 717 );
	try
	{
		CGameWindow window;
		window.Show();
	}
	catch( ... )
	{
		throw;
	}

	_CrtDumpMemoryLeaks();
}
//--------------------------------------------------------------------