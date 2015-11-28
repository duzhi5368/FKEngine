/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	DS8StreamSound
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
	DS8StreamSound::DS8StreamSound()
		: mbStopFill		(false)
		, mhFillThread		(NULL)
		, mhFillEvent		(NULL)
		, mpFillCache		(NULL)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	DS8StreamSound::~DS8StreamSound()
	{
		FKTry
		{
			UnloadStreamSound();
		}
		FKCatch
		{
			ShowExceptionMsg;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//载入流式声音
	void DS8StreamSound::LoadStreamSound( SoundDecoder* pSoundDecoder, DWORD dwCacheTime )
	{
		//存储声音解码器指针
		mpSoundDecoder = pSoundDecoder;

		//创建声音播放通知事件
		mhFillEvent = CreateEvent( NULL, FALSE, FALSE, NULL );
		if( mhFillEvent == NULL )
			Except( Exception::ERR_WINAPI_ERROR, "创建流式声音数据填充事件失败。" );

		//创建声音缓存
		DWORD dwNumNotifys = 32;
		mdwFillLen = pSoundDecoder->mSoundFormat.dwBytesPerSec * dwCacheTime / ( 1000 * dwNumNotifys );
		mdwBufSize = mdwFillLen * dwNumNotifys;
		mdwFillPos = 0;

		_CreateSoundBuffer( &pSoundDecoder->mSoundFormat, mdwBufSize,
			DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_GETCURRENTPOSITION2 );

		//分配声音数据填充缓存
		mpFillCache = (BYTE*)malloc( mdwBufSize * 2 );

		//获取声音播放通知设置接口
		IDirectSoundNotify* pDSNotify = NULL;
		HRESULT result = mpSoundBuffer->QueryInterface( IID_IDirectSoundNotify, (void**)&pDSNotify );
		if( FAILED( result ) )
			Except( Exception::ERR_INTERNAL_ERROR, (String)"获取 DirectSound8 声音播放通知设置接口失败。"
			"\nDS8 错误描述：" + DXGetErrorDescription9( result ) );

		//设置声音播放通知位置
		DSBPOSITIONNOTIFY* pDSPosNotify = (DSBPOSITIONNOTIFY*)malloc( sizeof(DSBPOSITIONNOTIFY) * dwNumNotifys );
		DSBPOSITIONNOTIFY* pDSPosNotifyPtr = pDSPosNotify;
		for( DWORD i=0; i<dwNumNotifys; ++i, ++pDSPosNotifyPtr )
		{
			pDSPosNotifyPtr->dwOffset = mdwFillLen * i;
			pDSPosNotifyPtr->hEventNotify = mhFillEvent;
		}

		result = pDSNotify->SetNotificationPositions( dwNumNotifys, pDSPosNotify );

		free( pDSPosNotify );
		pDSNotify->Release();

		if( FAILED( result ) )
			Except( Exception::ERR_INTERNAL_ERROR, (String)"设置 DirectSound8 声音播放通知失败。"
			"\nDS8 错误描述：" + DXGetErrorDescription9( result ) );

		//填充起始声音数据
		_FillSoundData( this );

		//创建声音数据填充线程
		mbStopFill = false;
		mhFillThread = CreateThread( NULL, 0, _FillSoundThread, (LPVOID)this, 0, NULL );
		if( mhFillThread == NULL )
			Except( Exception::ERR_WINAPI_ERROR, "创建流式声音数据填充线程失败。" );
	}
	//--------------------------------------------------------------------------------------------------------------
	//卸载流式声音
	void DS8StreamSound::UnloadStreamSound()
	{
		//停止声音播放
		if( mpSoundBuffer != NULL )
			Stop();

		//终止数据填充线程
		if( mhFillThread != NULL )
		{
			//激活声音数据填充终止事件
			mbStopFill = true;
			if( FALSE == SetEvent( mhFillEvent ) )
				Except( Exception::ERR_WINAPI_ERROR, "激活声音数据填充终止事件失败。" );

			//等待数据填充线程终止
			DWORD dwResult = WaitForSingleObject( mhFillThread, 10000 );
			CloseHandle( mhFillThread );
			mhFillThread = NULL;
			if( dwResult != WAIT_OBJECT_0 )
				Except( Exception::ERR_WINAPI_ERROR, "等待声音数据填充线程终止错误。" );
		}

		//删除数据填充事件
		if( mhFillEvent != NULL )
		{
			CloseHandle( mhFillEvent );
			mhFillEvent = NULL;
		}

		//释放声音缓存
		SafeRelease( mpSoundBuffer );
		SafeFree( mpFillCache );
	}
	//--------------------------------------------------------------------------------------------------------------
	//流式声音数据填充线程
	DWORD WINAPI DS8StreamSound::_FillSoundThread( LPVOID pVoid )
	{
		DS8StreamSound* pThis = (DS8StreamSound*)pVoid;

		FKTry
		{
			for(;;)
			{
				//等待数据填充数据事件
				DWORD dwResult = WaitForSingleObject( pThis->mhFillEvent, INFINITE );
				if( dwResult != WAIT_OBJECT_0 )
					Except( Exception::ERR_WINAPI_ERROR, "等待声音数据填充线程事件错误。" );

				//如果要求线程终止
				if( pThis->mbStopFill )
					break;

				//填充声音数据
				_FillSoundData( pThis );
			}
		}
		FKCatch
		{
			ShowExceptionMsg;
		}

		return 0;
	}
	//--------------------------------------------------------------------------------------------------------------
	//填充声音数据
	void DS8StreamSound::_FillSoundData( DS8StreamSound* pThis )
	{
		//解码声音数据
		DWORD dwFillLen = pThis->mdwFillLen;
		BYTE* pSrcPtr = pThis->mpFillCache;
		
		pThis->mpSoundDecoder->ReadSoundData( pSrcPtr, dwFillLen );

		//锁定声音缓存
		BYTE* pDstPtr1 = NULL;
		BYTE* pDstPtr2 = NULL;
		DWORD dwLockSize1;
		DWORD dwLockSize2;

		HRESULT result = pThis->mpSoundBuffer->Lock( pThis->mdwFillPos, dwFillLen,
			(void**)&pDstPtr1, &dwLockSize1, (void**)pDstPtr2, &dwLockSize2, 0 );
		if( FAILED( result ) )
			Except( Exception::ERR_INTERNAL_ERROR, "锁定 DirectSoundBuffer8 次声音缓存失败。" );

		//复制声音数据
		memcpy( pDstPtr1, pSrcPtr, dwLockSize1 );
		if( pDstPtr2 != NULL )
			memcpy( pDstPtr2, pSrcPtr + dwLockSize1, dwLockSize2 );

		//解锁声音缓存
		result = pThis->mpSoundBuffer->Unlock( pDstPtr1, dwLockSize1, pDstPtr2, dwLockSize2 );
		if( FAILED( result ) )
			Except( Exception::ERR_INTERNAL_ERROR, "解锁 DirectSoundBuffer8 次声音缓存失败。" );


		pThis->mdwFillPos += dwFillLen;

		//如果已到达声音结尾
		if( pThis->mdwFillPos >= pThis->mdwBufSize )
			pThis->mdwFillPos -= pThis->mdwBufSize;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
