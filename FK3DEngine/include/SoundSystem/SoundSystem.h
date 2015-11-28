/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	SoundSystem
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
	//声音系统类 （纯虚类）
	class SoundSystem : public Singleton< SoundSystem >
	{
	public:
		SoundSystemCfg*		mpConfig;			//声音系统配置信息

		StaticSoundList		mStaticSounds;		//静态声音列表
		StreamSoundList		mStreamSounds;		//流式声音列表

	protected:

	public:
		SoundSystem();
		virtual ~SoundSystem();

		//初始化声音系统
		virtual void Initialize( SoundSystemCfg* pSoundSystemCfg, RenderWindow* pRenderWindow ) = 0;

		//关闭声音系统
		virtual void Shutdown() = 0;

		//设置声音播放格式
		virtual void SetPlayFormat( DWORD dwChannels, DWORD dwFrequency, DWORD dwBitRate ) = 0;

		//创建静态声音
		virtual StaticSound* CreateStaticSound() = 0;

		//创建流式声音
		virtual StreamSound* CreateStreamSound() = 0;

		//删除静态声音
		void DeleteStaticSound( StaticSound* pStaticSound );

		//删除流式声音
		void DeleteStreamSound( StreamSound* pStreamSound );

		//删除所有静态声音
		void DeleteAllStaticSound();

		//删除所有流式声音
		void DeleteAllStreamSound();
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
