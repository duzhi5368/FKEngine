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

#include "../../include/SoundSystem/SoundSystemInsideHeader.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	SoundSystem::SoundSystem()
		: mpConfig		(NULL)
	{
		mStaticSounds.Initialize( 20, 20 );
		mStreamSounds.Initialize( 20, 10 );
	}
	//--------------------------------------------------------------------------------------------------------------
	SoundSystem::~SoundSystem()
	{
		mStaticSounds.Release();
		mStreamSounds.Release();
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除静态声音
	void SoundSystem::DeleteStaticSound( StaticSound* pStaticSound )
	{
		StaticSoundList::Iterator it = mStaticSounds.Begin();
		StaticSoundList::Iterator end = mStaticSounds.End();
		for(; it!=end; ++it )
		{
			if( *it == pStaticSound )
			{
				mStaticSounds.Erase( it );
				delete pStaticSound;
				return;
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除流式声音
	void SoundSystem::DeleteStreamSound( StreamSound* pStreamSound )
	{
		StreamSoundList::Iterator it = mStreamSounds.Begin();
		StreamSoundList::Iterator end = mStreamSounds.End();
		for(; it!=end; ++it )
		{
			if( *it == pStreamSound )
			{
				mStreamSounds.Erase( it );
				delete pStreamSound;
				return;
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除所有静态声音
	void SoundSystem::DeleteAllStaticSound()
	{
		StaticSoundList::Iterator it = mStaticSounds.Begin();
		StaticSoundList::Iterator end = mStaticSounds.End();
		for(; it!=end; ++it )
			SafeDelete( *it );

		mStaticSounds.Clear();
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除所有流式声音
	void SoundSystem::DeleteAllStreamSound()
	{
		StreamSoundList::Iterator it = mStreamSounds.Begin();
		StreamSoundList::Iterator end = mStreamSounds.End();
		for(; it!=end; ++it )
			SafeDelete( *it );

		mStreamSounds.Clear();
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
