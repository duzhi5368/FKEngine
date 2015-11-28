/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	AudioAudiere
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../include/Audio.h"
#include "../include/Math.h"
#include "../include/IO.h"
#include "../include/StringConversion.h"
#include "../../depend/boost/foreach.hpp"
#include <algorithm>
#include <stdexcept>
#include <vector>
#include "../../depend/audiere/audiere.h"

//--------------------------------------------------------------------
using namespace audiere;
//--------------------------------------------------------------------
namespace FK2DEngine
{
	namespace
	{
		CSong* g_pCurSong = NULL;
		//--------------------------------------------------------------------
		class CMemoryBuffer : boost::noncopyable
		{
		private:
			FK2DEngine::CBuffer		m_Buffer;
			FilePtr					m_pFile;
		public:
			CMemoryBuffer( FK2DEngine::CReader p_Reader )
			{
				m_Buffer.Resize( p_Reader.Resource().Size() - p_Reader.Position() );
				p_Reader.Read( m_Buffer.Data(), m_Buffer.Size() );
				m_pFile = audiere::CreateMemoryFile( m_Buffer.Data(), m_Buffer.Size() );
			}

			CMemoryBuffer( const std::wstring& p_szFileName )
			{
				FK2DEngine::LoadFile( m_Buffer, p_szFileName );
				m_pFile = audiere::CreateMemoryFile( m_Buffer.Data(), m_Buffer.Size() );
			}

			operator const FilePtr& ()
			{
				return m_pFile;
			}
		};
		//--------------------------------------------------------------------
		class CStreamRegistry : public RefImplementation< StopCallback >, boost::noncopyable
		{
		private:
			struct SNumberedStream
			{
				int				m_nExtra;
				OutputStreamPtr	m_pStream;
			};

			std::vector< SNumberedStream >		m_vecStreams;

			ADR_METHOD( void ) streamStopped( StopEvent* p_pEvent )
			{
				if( p_pEvent->getReason() == StopEvent::STREAM_ENDED )
				{
					BOOST_FOREACH( SNumberedStream& stream, m_vecStreams )
					{
						if( stream.m_pStream == p_pEvent->getOutputStream() )
						{
							stream.m_pStream = 0;
							break;
						}
					}
				}
			}
		public:
			CStreamRegistry()
			{
				m_vecStreams.reserve( 100 );
			}

			OutputStreamPtr Get( int p_nHandle, int p_nExtra ) const
			{
				if(( p_nHandle < static_cast<int>(m_vecStreams.size()) ) &&
					( m_vecStreams[p_nHandle].m_nExtra == p_nExtra ))
				{
					return m_vecStreams[p_nHandle].m_pStream;
				}

				return OutputStreamPtr();
			}

			void Put( OutputStreamPtr p_pStream, int& p_nHandle, int& p_nExtra )
			{
				p_nHandle = 0;
				for( ; p_nHandle < static_cast<int>(m_vecStreams.size()); ++p_nHandle )
				{
					if( !m_vecStreams[p_nHandle].m_pStream )
					{
						m_vecStreams[p_nHandle].m_pStream = p_pStream;
						p_nExtra = ++m_vecStreams[p_nHandle].m_nExtra;
						return;
					}
				}
			
				SNumberedStream tagStream = { p_nExtra = 0, p_pStream };
				m_vecStreams.push_back( tagStream );
			}

			void Clear( int p_nHandle )
			{
				m_vecStreams.at( p_nHandle ).m_pStream = 0;
			}
		};
		//--------------------------------------------------------------------
		CStreamRegistry& g_Streams = *( new CStreamRegistry );

		bool GetDevice( AudioDevice*& p_Device )
		{
			#ifdef FK_WINDOWS
			#define FACILITY_VISUALCPP  ((LONG)0x6d)
			#define VcppException(sev,err)  ((sev) | (FACILITY_VISUALCPP<<16) | err)
			#define BAD_MOD VcppException(ERROR_SEVERITY_ERROR, ERROR_MOD_NOT_FOUND)
			#endif

			#ifdef FK_WINDOWS
			__try
			{
			#endif

				p_Device = OpenDevice();
				if( p_Device == NULL )
				{
					return false;
				}
				else
				{
					return true;
				}
			#ifdef FK_WINDOWS
			}
			__except( ( GetExceptionCode() == BAD_MOD ) ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH )
			{
				return false;
			}

			#undef BAD_MOD
			#undef VcppException
			#undef FACILITY_VISUALCPP
			#endif
		}

		AudioDevice* Device()
		{
			static AudioDevice* g_pDevice = 0;
			if( g_pDevice == 0 )
			{
				if( ! GetDevice( g_pDevice ) )
				{
					throw std::runtime_error( "初始化 audiere 失败！可能无法找到 audiere 库的原因..." );
				}

				g_pDevice->registerCallback( &g_Streams );

				g_pDevice->ref();
			}
			return g_pDevice;
		}
	}
}
//--------------------------------------------------------------------
FK2DEngine::CSampleInstance::CSampleInstance( int p_nHandle, int p_nExtra )
	: m_nHandle( p_nHandle )
	, m_nExtra( p_nExtra )
{

}
//--------------------------------------------------------------------
bool FK2DEngine::CSampleInstance::Playing() const
{
	OutputStreamPtr pStream = g_Streams.Get( m_nHandle, m_nExtra );
	return pStream && pStream->isPlaying();
}
//--------------------------------------------------------------------
bool FK2DEngine::CSampleInstance::Paused() const
{
	OutputStreamPtr pStream = g_Streams.Get( m_nHandle, m_nExtra );
	return pStream && !pStream->isPlaying();
}
//--------------------------------------------------------------------
void FK2DEngine::CSampleInstance::Pause()
{
	if( OutputStreamPtr pStream = g_Streams.Get( m_nHandle, m_nExtra ) )
	{
		pStream->stop();
	}
}
//--------------------------------------------------------------------
void FK2DEngine::CSampleInstance::Resume()
{
	if( OutputStreamPtr pStream = g_Streams.Get( m_nHandle, m_nExtra ) )
	{
		pStream->play();
	}
}
//--------------------------------------------------------------------
void FK2DEngine::CSampleInstance::Stop()
{
	if( OutputStreamPtr pStream = g_Streams.Get( m_nHandle, m_nExtra ) )
	{
		pStream->stop();
		g_Streams.Clear( m_nHandle );
	}
}
//--------------------------------------------------------------------
void FK2DEngine::CSampleInstance::ChangeVolume( double p_dVolume )
{
	if( OutputStreamPtr pStream = g_Streams.Get( m_nHandle, m_nExtra ) )
	{
		pStream->setVolume( static_cast< float >( p_dVolume ) );
	}
}
//--------------------------------------------------------------------
void FK2DEngine::CSampleInstance::ChangePan( double p_dPan )
{
	if( OutputStreamPtr pStream = g_Streams.Get( m_nHandle, m_nExtra ) )
	{
		pStream->setPan( Clamp< float >( static_cast< float >( p_dPan ), -1.0f, +1.0f ) );
	}
}
//--------------------------------------------------------------------
void FK2DEngine::CSampleInstance::ChangeSpeed( double p_dSpeed )
{
	if( OutputStreamPtr pStream = g_Streams.Get( m_nHandle, m_nExtra ) )
	{
		pStream->setPitchShift( Clamp< float >( static_cast< float >( p_dSpeed ), 0.5f, 2.0f ) );
	}
}
//--------------------------------------------------------------------
struct FK2DEngine::CFKSample::CSampleData : boost::noncopyable
{
	SampleBufferPtr		m_pBuffer; 
};
//--------------------------------------------------------------------
FK2DEngine::CFKSample::CFKSample( const std::wstring& p_szFileName )
	: m_pData( new CSampleData )
{
	Device();

	SampleSourcePtr pSource = OpenSampleSource( CMemoryBuffer( p_szFileName ), FF_AUTODETECT );
	m_pData->m_pBuffer = CreateSampleBuffer( pSource );
	if( !m_pData->m_pBuffer )
	{
		throw std::runtime_error("无法打开音频文件" + WstringToUTF8( p_szFileName ));
	}
}
//--------------------------------------------------------------------
FK2DEngine::CFKSample::CFKSample( CReader p_tagReader )
{
	Device();

	SampleSourcePtr pSource = OpenSampleSource( CMemoryBuffer( p_tagReader ), FF_AUTODETECT );
	m_pData->m_pBuffer = CreateSampleBuffer( pSource );
	if( !m_pData->m_pBuffer )
	{
		throw std::runtime_error("无法打开音频文件");
	}
}
//--------------------------------------------------------------------
FK2DEngine::CSampleInstance FK2DEngine::CFKSample::Play( double p_dVolume /* = 1.0 */,
				double p_dSpeed /* = 1.0 */, bool p_bLooping /* = false */ ) const
{
	OutputStreamPtr pStream = Device()->openStream( m_pData->m_pBuffer->openStream() );
	int nHandle = 0;
	int nExtra = 0;
	g_Streams.Put( pStream, nHandle, nExtra );

	CSampleInstance Instance( nHandle, nExtra );
	Instance.ChangeVolume( p_dVolume );
	Instance.ChangeSpeed( p_dSpeed );
	pStream->setRepeat( p_bLooping );
	pStream->play();

	return Instance;
}
//--------------------------------------------------------------------
FK2DEngine::CSampleInstance FK2DEngine::CFKSample::PlayPan( double p_dPan, 
	double p_dVolume /* = 1.0 */, double p_dSpeed /* = 1.0 */, bool p_bLooping /* = false */ ) const
{
	OutputStreamPtr pStream = Device()->openStream( m_pData->m_pBuffer->openStream() );
	int nHandle = 0;
	int nExtra = 0;
	g_Streams.Put( pStream, nHandle, nExtra );

	CSampleInstance Instance( nHandle, nExtra );
	Instance.ChangePan( p_dPan );
	Instance.ChangeVolume( p_dVolume );
	Instance.ChangeSpeed( p_dSpeed );
	pStream->setRepeat( p_bLooping );
	pStream->play();

	return Instance;
}
//--------------------------------------------------------------------
class FK2DEngine::CSong::CBaseData : boost::noncopyable
{
private:
	double		m_dVolume;
protected:
	CBaseData()
		: m_dVolume( 1.0 )
	{

	}

	virtual void ApplyVolume() = 0;
public:
	virtual ~CBaseData() {}
	virtual void Play( bool p_bLooping ) = 0;
	virtual bool Playing() const = 0;
	virtual bool Paused() const = 0;
	virtual void Pause() = 0;
	virtual void Stop() = 0;
public:
	double Volume() const
	{
		return m_dVolume;
	}

	void ChangeVolume( double p_dVolume )
	{
		m_dVolume = Clamp< double >( p_dVolume, 0.0, 1.0 );
		ApplyVolume();
	}
};
//--------------------------------------------------------------------
class FK2DEngine::CSong::CStreamData : public CBaseData
{
private:
	CMemoryBuffer		m_Buffer;
	OutputStreamPtr		m_pStream;
	CSampleInstance		m_Instance;
	bool				m_bLoop;
private:
	void Init()
	{
		int			nHandle = 0;
		int			nExtra = 0;

		m_pStream = OpenSound( Device(), m_Buffer, true );
		if( !m_pStream )
		{
			throw std::runtime_error("无法打开音频文件或音频流");
		}

		g_Streams.Put( m_pStream, nHandle, nExtra );
		m_Instance = CSampleInstance( nHandle, nExtra );
		m_bLoop = false;
	}
public:
	CStreamData( FK2DEngine::CReader p_pReader )
		: m_Buffer( p_pReader )
		, m_Instance( -1, -1 )
	{
		Init();
	}

	CStreamData( const std::wstring& p_szFileName )
		: m_Buffer( p_szFileName )
		, m_Instance( -1, -1 )
	{
		Init();
	}

	void Play( bool p_bLooping )
	{
		ApplyVolume();
		if( p_bLooping != m_bLoop )
		{
			m_pStream->setRepeat( m_bLoop = p_bLooping );
		}
		m_pStream->play();
	}

	bool Playing() const
	{
		return m_Instance.Playing();
	}

	bool Paused() const
	{
		return m_Instance.Paused();
	}

	void Pause()
	{
		m_Instance.Pause();
	}

	void Stop()
	{
		m_Instance.Stop();
		m_pStream->reset();
	}

	void ApplyVolume()
	{
		m_Instance.ChangeVolume( Volume() );
	}
};
//--------------------------------------------------------------------
FK2DEngine::CSong::CSong( const std::wstring& p_szFileName )
	: m_pData( new CStreamData( p_szFileName ) )
{

}
//--------------------------------------------------------------------
FK2DEngine::CSong::CSong( CReader p_Reader )
	: m_pData( new CStreamData( p_Reader ) )
{

}
//--------------------------------------------------------------------
FK2DEngine::CSong::~CSong()
{

}
//--------------------------------------------------------------------
FK2DEngine::CSong* FK2DEngine::CSong::CurrentSong()
{
	if( g_pCurSong && ( g_pCurSong->Playing() || g_pCurSong->Paused() ) )
	{
		return g_pCurSong;
	}
	g_pCurSong = NULL;
	return g_pCurSong;
}
//--------------------------------------------------------------------
void FK2DEngine::CSong::Play( bool p_bLooping )
{
	if( FK2DEngine::CSong* pCur = CurrentSong() )
	{
		if( pCur != this )
		{
			pCur->Stop();
			assert( CurrentSong() == 0 );
		}
	}

	m_pData->Play( p_bLooping );
	g_pCurSong = this;
}
//--------------------------------------------------------------------
void FK2DEngine::CSong::Pause()
{
	if( ( g_pCurSong == this ) && ( m_pData->Playing() ) )
	{
		m_pData->Pause();
	}
}
//--------------------------------------------------------------------
bool FK2DEngine::CSong::Paused() const
{
	return ( g_pCurSong == this ) && ( m_pData->Paused() ) ;
}
//--------------------------------------------------------------------
void FK2DEngine::CSong::Stop()
{
	if( g_pCurSong == this )
	{
		m_pData->Stop();
		g_pCurSong = NULL;
	}
}
//--------------------------------------------------------------------
bool FK2DEngine::CSong::Playing() const
{
	assert( ! ( ( g_pCurSong != this ) && ( m_pData->Playing() ) ) );
	return m_pData->Playing();
}
//--------------------------------------------------------------------
double FK2DEngine::CSong::Volume() const
{
	return m_pData->Volume();
}
//--------------------------------------------------------------------
void FK2DEngine::CSong::ChangeVolume( double p_dVolume )
{
	m_pData->ChangeVolume( p_dVolume );
}
//--------------------------------------------------------------------
void FK2DEngine::CSong::Update()
{
	Device()->update();
}
//--------------------------------------------------------------------
FK2DEngine::CFKSample::CFKSample(FK2DEngine::CAudio &p_Audio, const std::wstring &p_szFileName)
{
	CFKSample( p_szFileName ).m_pData.swap( m_pData );
}
//--------------------------------------------------------------------
FK2DEngine::CFKSample::CFKSample( CAudio& p_Audio, CReader p_Reader )
{
	CFKSample( p_Reader ).m_pData.swap( m_pData );
}
//--------------------------------------------------------------------
FK2DEngine::CSong::CSong( CAudio& p_Audio, const std::wstring& p_szFileName )
{
	CSong( p_szFileName ).m_pData.swap( m_pData );
}
//--------------------------------------------------------------------
FK2DEngine::CSong::CSong( CAudio& p_Audio, ENUM_SongType p_eType, CReader p_Reader )
{
	CSong( p_Reader ).m_pData.swap( m_pData );
}
//--------------------------------------------------------------------