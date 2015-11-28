/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	DS8BaseSound
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
	DS8BaseSound::DS8BaseSound()
		: mpSoundBuffer		(NULL)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	//播放声音
	void DS8BaseSound::Play()
	{
		HRESULT result = mpSoundBuffer->Play( 0, 0, DSBPLAY_LOOPING );
		if( FAILED( result ) )
			Except( Exception::ERR_SOUND_API_ERROR, (String)"使用 DirectSoundBuffer8 次声音缓存播放声音失败。"
			"\nDS8 错误描述：" + DXGetErrorDescription9( result ) );
	}
	//--------------------------------------------------------------------------------------------------------------
	//停止播放声音
	void DS8BaseSound::Stop()
	{
		HRESULT result = mpSoundBuffer->Stop();
		if( FAILED( result ) )
			Except( Exception::ERR_SOUND_API_ERROR, (String)"停止播放 DirectSoundBuffer8 次声音缓存失败。"
			"\nDS8 错误描述：" + DXGetErrorDescription9( result ) );
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置音量
	void DS8BaseSound::SetVolume( float fVolume )
	{
		//根据听觉修正音量
		float fScaling = 0.0f;
		if( fVolume > 0.0f )
		{
			// log10( 当前音量比例 * 100.0f ) / log10( 最大音量比例 * 100.0f )
			fScaling = log10( fVolume * 100.0f ) / 2.0f;
		}

		LONG nVolume = FloorToInt( ( fScaling - 1.0f ) * 10000.0f );

		HRESULT result = mpSoundBuffer->SetVolume( nVolume );
		if( FAILED( result ) )
			Except( Exception::ERR_SOUND_API_ERROR, (String)"设置 DirectSoundBuffer8 次声音缓存音量失败。"
			"\nDS8 错误描述：" + DXGetErrorDescription9( result ) );

		mfVolume = fVolume;
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建声音缓存
	void FKFastcall DS8BaseSound::_CreateSoundBuffer( const SoundFormat* pSoundFmt, DWORD dwLength, DWORD dwFlags )
	{
		WAVEFORMATEX wfx;
		wfx.wFormatTag      = (WORD)WAVE_FORMAT_PCM; 
		wfx.nChannels       = (WORD)pSoundFmt->dwChannels; 
		wfx.nSamplesPerSec  = pSoundFmt->dwFrequency; 
		wfx.wBitsPerSample  = (WORD)pSoundFmt->dwBitRate; 
		wfx.nBlockAlign     = (WORD)( pSoundFmt->dwBitRate / 8 * pSoundFmt->dwChannels );
		wfx.nAvgBytesPerSec = pSoundFmt->dwBytesPerSec;
		wfx.cbSize			= 0;

		DSBUFFERDESC dsbd;
		memset( &dsbd, 0, sizeof(DSBUFFERDESC) );
		dsbd.dwSize				= sizeof(DSBUFFERDESC);
		dsbd.dwBufferBytes		= dwLength;
		dsbd.lpwfxFormat		= &wfx;
		dsbd.dwFlags			= dwFlags | DSBCAPS_CTRLVOLUME;

		//创建次声音缓存
		HRESULT result = DS8SoundSystem::mpDirectSound8->CreateSoundBuffer( &dsbd, &mpSoundBuffer, NULL );
		if( FAILED( result ) )
			Except( Exception::ERR_SOUND_API_ERROR, (String)"创建 DirectSound8 次声音缓存失败。"
			"\nDS8 错误描述：" + DXGetErrorDescription9( result ) );
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
