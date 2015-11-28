/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	DS8StaticSound
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../../include/SoundSystem/SoundSystemInsideHeader.h"
#include <MMReg.h>
#include "../../../Depend/dx9/dsound.h"
#include "../../../Depend/dx9/dxerr9.h"
#include "../../include/CommonBase/MemoryMarco.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	DS8StaticSound::DS8StaticSound()
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	DS8StaticSound::~DS8StaticSound()
	{
		FKTry
		{
			UnloadStaticSound();
		}
		FKCatch
		{
			ShowExceptionMsg;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//载入静态声音
	void DS8StaticSound::LoadStaticSound( SoundDecoder* pSoundDecoder )
	{
		//设置次声音缓存格式
		SoundFormat* pSoundFormat = &pSoundDecoder->mSoundFormat;

		_CreateSoundBuffer( pSoundFormat, pSoundFormat->dwLength, 0 );

		//复制声音数据到缓存中
		mdwSoundLen = pSoundFormat->dwLength;
		void* pBufPtr = NULL;
		DWORD dwBufLen = 0;

		HRESULT result = mpSoundBuffer->Lock( 0, 0, &pBufPtr, &dwBufLen, NULL, NULL, DSBLOCK_ENTIREBUFFER  );
		if( FAILED( result ) )
			Except( Exception::ERR_SOUND_API_ERROR, (String)"锁定 DirectSoundBuffer8 次声音缓存失败。"
			"\nDS8 错误描述：" + DXGetErrorDescription9( result ) );

		pSoundDecoder->ReadSoundData( pBufPtr, mdwSoundLen );

		result = mpSoundBuffer->Unlock( pBufPtr, dwBufLen, NULL, 0 );
		if( FAILED( result ) )
			Except( Exception::ERR_SOUND_API_ERROR, (String)"解锁 DirectSoundBuffer8 次声音缓存失败。"
			"\nDS8 错误描述：" + DXGetErrorDescription9( result ) );
	}
	//--------------------------------------------------------------------------------------------------------------
	//卸载静态声音
	void DS8StaticSound::UnloadStaticSound()
	{
		if( mpSoundBuffer != NULL )
		{
			Stop();
			mpSoundBuffer->Release();
			mpSoundBuffer = NULL;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
