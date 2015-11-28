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

#pragma once

//==================================================================================================================

struct IDirectSound8;
struct IDirectSoundBuffer;

//--------------------------------------------------------------------------------------------------------------

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	// DirectSound 8 声音系统类
	class DS8SoundSystem : public SoundSystem
	{
	protected:
		friend class DS8BaseSound;

		static IDirectSound8*		mpDirectSound8;
		static IDirectSoundBuffer*	mpPrimaryBuffer;

	public:
		DS8SoundSystem();
		~DS8SoundSystem();

		//初始化声音系统
		void Initialize( SoundSystemCfg* pSoundSystemCfg, RenderWindow* pRenderWindow );

		//关闭声音系统
		void Shutdown();

		//设置声音播放格式
		void SetPlayFormat( DWORD dwChannels, DWORD dwFrequency, DWORD dwBitRate );

		//创建静态声音
		StaticSound* CreateStaticSound();

		//创建流式声音
		StreamSound* CreateStreamSound();
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
