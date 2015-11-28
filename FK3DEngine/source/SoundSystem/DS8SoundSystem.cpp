/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	DS8SoundSystem
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../../include/SoundSystem/SoundSystemInsideHeader.h"
//#include "../../include/CommonBase/NoMemoryMarco.h"
#include <MMReg.h>
#include "../../../Depend/dx9/dsound.h"
#include "../../../Depend/dx9/dxerr9.h"
#include "../../include/CommonBase/MemoryMarco.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------

	IDirectSound8*		DS8SoundSystem::mpDirectSound8	= NULL;
	IDirectSoundBuffer*	DS8SoundSystem::mpPrimaryBuffer	= NULL;

	//--------------------------------------------------------------------------------------------------------------
	DS8SoundSystem::DS8SoundSystem()
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	DS8SoundSystem::~DS8SoundSystem()
	{
		Shutdown();
	}
	//--------------------------------------------------------------------------------------------------------------
	//初始化声音系统
	void DS8SoundSystem::Initialize( SoundSystemCfg* pSoundSystemCfg, RenderWindow* pRenderWindow )
	{
		//创建 DirectSound8 对象
		HRESULT result = DirectSoundCreate8( NULL, &mpDirectSound8, NULL );
		if( FAILED( result ) )
			Except( Exception::ERR_SOUND_API_ERROR, (String)"创建 DirectSound8 对象失败。"
			"\nDS8 错误描述：" + DXGetErrorDescription9( result ) );

		//设置 DirectSound8 协作级
		result = mpDirectSound8->SetCooperativeLevel( pRenderWindow->mhWnd, DSSCL_PRIORITY );
		if( FAILED( result ) )
			Except( Exception::ERR_SOUND_API_ERROR, (String)"设置 DirectSound8 协作级失败。"
			"\nDS8 错误描述：" + DXGetErrorDescription9( result ) );

		//设置 DirectSound8 扬声器配置
		DWORD pSpeakerType[] =
		{
			DSSPEAKER_DIRECTOUT,		// Speaker_Unknown		= 0	未知（忽略扬声器配置）
			DSSPEAKER_HEADPHONE,		// Speaker_Headphone	= 1	手提听筒
			DSSPEAKER_MONO,				// Speaker_Mono			= 2	单声道
			DSSPEAKER_STEREO,			// Speaker_Stereo		= 3	双声道
			DSSPEAKER_QUAD,				// Speaker_Quad			= 4	四声道
			DSSPEAKER_SURROUND,			// Speaker_Surround		= 5	环绕立体声
			DSSPEAKER_5POINT1,			// Speaker_5Point1		= 6	5.1 声道
			DSSPEAKER_7POINT1,			// Speaker_7Point1		= 7	7.1 声道
		};

		result = mpDirectSound8->SetSpeakerConfig( pSpeakerType[ pSoundSystemCfg->eSpeakerType ] );
		if( FAILED( result ) )
			Except( Exception::ERR_SOUND_API_ERROR, (String)"设置 DirectSound8 协作级失败。"
			"\nDS8 错误描述：" + DXGetErrorDescription9( result ) );


		//创建主声音缓存
		DSBUFFERDESC dsbd;
		memset( &dsbd, 0, sizeof(DSBUFFERDESC) );
		dsbd.dwSize		= sizeof(DSBUFFERDESC);
		dsbd.dwFlags	= DSBCAPS_PRIMARYBUFFER;

		result = mpDirectSound8->CreateSoundBuffer( &dsbd, &mpPrimaryBuffer, NULL );
		if( FAILED( result ) )
			Except( Exception::ERR_SOUND_API_ERROR, (String)"创建 DirectSound8 主声音缓存失败。"
			"\nDS8 错误描述：" + DXGetErrorDescription9( result ) );

		//设置主声音缓存格式
		SetPlayFormat( pSoundSystemCfg->dwChannels, pSoundSystemCfg->dwFrequency, pSoundSystemCfg->dwBitRate );
	}
	//--------------------------------------------------------------------------------------------------------------
	//关闭声音系统
	void DS8SoundSystem::Shutdown()
	{
		DeleteAllStaticSound();
		DeleteAllStreamSound();

		SafeRelease( mpPrimaryBuffer );

		SafeRelease( mpDirectSound8 );
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置声音播放格式
	void DS8SoundSystem::SetPlayFormat( DWORD dwChannels, DWORD dwFrequency, DWORD dwBitRate )
	{
		if( mpPrimaryBuffer == NULL )
			Except( Exception::ERR_INVALIDPARAMS, "在创建主声音缓存之前无法设置播放格式。" );

		//设置主声音缓存格式
		WAVEFORMATEX wfx;
		wfx.wFormatTag      = (WORD)WAVE_FORMAT_PCM; 
		wfx.nChannels       = (WORD)dwChannels; 
		wfx.nSamplesPerSec  = dwFrequency; 
		wfx.wBitsPerSample  = (WORD)dwBitRate; 
		wfx.nBlockAlign     = (WORD)( dwBitRate / 8 * dwChannels );
		wfx.nAvgBytesPerSec = (DWORD)( dwFrequency * wfx.nBlockAlign );
		wfx.cbSize			= 0;

		HRESULT result = mpPrimaryBuffer->SetFormat( &wfx );
		if( FAILED( result ) )
			Except( Exception::ERR_SOUND_API_ERROR, (String)"设置 DirectSound8 主声音缓存播放格式。"
			"\nDS8 错误描述：" + DXGetErrorDescription9( result ) );
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建静态声音
	StaticSound* DS8SoundSystem::CreateStaticSound()
	{
		DS8StaticSound* pStaticSound = new DS8StaticSound;
		*mStaticSounds.Push() = pStaticSound;
		return pStaticSound;
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建流式声音
	StreamSound* DS8SoundSystem::CreateStreamSound()
	{
		DS8StreamSound* pStreamSound = new DS8StreamSound;
		*mStreamSounds.Push() = pStreamSound;
		return pStreamSound;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
