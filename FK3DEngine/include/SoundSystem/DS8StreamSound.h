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

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	#pragma warning ( disable : 4250 )		//虚基类通过控制域继承方法
	//--------------------------------------------------------------------------------------------------------------
	// DirectSound 8 流式声音类
	class DS8StreamSound : public StreamSound, public DS8BaseSound
	{
	protected:
		bool			mbStopFill;			//是否终止数据填充线程
		HANDLE			mhFillThread;		//声音数据填充线程句柄
		HANDLE			mhFillEvent;		//声音数据填充事件句柄

		DWORD			mdwBufSize;			//播放缓存大小
		DWORD			mdwFillPos;			//数据填充位置
		DWORD			mdwFillLen;			//数据填充长度

		BYTE*			mpFillCache;		//数据填充缓存

	public:
		DS8StreamSound();
		~DS8StreamSound();

		//载入流式声音
		void LoadStreamSound( SoundDecoder* pSoundDecoder, DWORD dwCacheTime );

		//卸载流式声音
		void UnloadStreamSound();

	public:
		//流式声音数据填充线程
		static DWORD WINAPI _FillSoundThread( LPVOID pVoid );

		//填充声音数据
		static void _FillSoundData( DS8StreamSound* pThis );
	};
	//--------------------------------------------------------------------------------------------------------------
	#pragma warning ( default : 4250 )
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
