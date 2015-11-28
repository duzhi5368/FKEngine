/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Audio
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "Platform.h"
#include "InsidePreInclude.h"
#include "IO.h"
#include "../../depend/boost/scoped_ptr.hpp"
#include "../../depend/boost/shared_ptr.hpp"
#include <string>
//--------------------------------------------------------------------
#ifdef FK_WINDOWS
	#ifndef NOMINMAX
		#define NOMINMAX
	#endif

	#include <Windows.h>
#endif
//--------------------------------------------------------------------
namespace FK2DEngine
{
	#ifndef SWIG
		FK_DEPRECATED class CAudio;
	#endif
	
	//--------------------------------------------------------------------
	//--------------------------------------------------------------------
	class CSampleInstance
	{
	private:
		int			m_nHandle;
		int			m_nExtra;
		bool		Alive() const;
	public:
		CSampleInstance( int p_nHandle, int p_nExtra );
	public:
		bool		Playing() const;
		bool		Paused() const;
		void		Pause();
		void		Resume();
		void		Stop();
		void		ChangeVolume( double p_dVolume );
		void		ChangePan( double p_dPan );
		void		ChangeSpeed( double p_dSpeed );
	};
	//--------------------------------------------------------------------
	//--------------------------------------------------------------------
	class CFKSample
	{
	private:
		struct CSampleData;
		boost::shared_ptr< CSampleData >		m_pData;
	public:
		explicit CFKSample( const std::wstring& p_szFileName );
		explicit CFKSample( CReader p_tagReader );
	public:
		CSampleInstance Play( double p_dVolume = 1.0, double p_dSpeed = 1.0, bool p_bLooping = false ) const;
		CSampleInstance PlayPan( double p_dPan, double p_dVolume = 1.0, double p_dSpeed = 1.0, bool p_bLooping = false ) const;

		#ifndef SWIG
			FK_DEPRECATED CFKSample( CAudio& p_Audio, const std::wstring& p_szFileName );
			FK_DEPRECATED CFKSample( CAudio& p_Audio, CReader p_Reader );
		#endif
	};
	//--------------------------------------------------------------------
	//--------------------------------------------------------------------
	class CSong
	{
	private:
		class CBaseData;
		class CModuleData;
		class CStreamData;
		boost::scoped_ptr< CBaseData >			m_pData;

	public:
		explicit CSong( const std::wstring& p_szFileName );
		explicit CSong( CReader p_Reader );
		~CSong();
	public:
		static CSong*	CurrentSong();
		static void		Update();
	public:
		bool		Paused() const;
		bool		Playing() const;
		void		Play( bool p_bLooping = false );
		void		Pause();
		void		Stop();
		//! 音量大小 0.0f - 1.0f
		double		Volume() const;
		void		ChangeVolume( double p_dVolume );

#ifndef SWIG
		enum ENUM_SongType
		{
			EST_Stream,
			EST_Module,
		};

		FK_DEPRECATED CSong( CAudio&, const std::wstring& p_szFileName );
		FK_DEPRECATED CSong( CAudio&, ENUM_SongType p_eType, CReader p_Reader );
#endif
	};
}