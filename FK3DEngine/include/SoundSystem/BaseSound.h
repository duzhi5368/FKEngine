/********************************************************************
*
*	���ļ���FKMMORPG��һ���֡����ļ���Ȩ����FreeKnightDuzhi(������)���С�
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	�ļ�����	BaseSound
*	���ߣ�		FreeKnightDuzhi[ ������ ]
*	�ļ�˵����	
*
*	�޸���:		
*	�޸�����:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//����������
	class BaseSound
	{
	public:
		float			mfVolume;		//��������

	public:
		virtual ~BaseSound() = 0;

		//��������
		virtual void Play() = 0;

		//ֹͣ��������
		virtual void Stop() = 0;

		//��������
		virtual void SetVolume( float fVolume ) = 0;
	};
	//--------------------------------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------------------------------
	//��̬������
	class StaticSound : virtual public BaseSound
	{
	public:
		DWORD			mdwSoundLen;	//�������ݳ���

	public:
		StaticSound();

		//�ӽ����������뾲̬����
		virtual void LoadStaticSound( SoundDecoder* pSoundDecoder ) = 0;

		//ж�ؾ�̬����
		virtual void UnloadStaticSound() = 0;
	};
	//--------------------------------------------------------------------------------------------------------------
	typedef Stack< StaticSound* >	StaticSoundList;
	//--------------------------------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------------------------------
	//��ʽ������
	class StreamSound : virtual public BaseSound
	{
	protected:
		SoundDecoder*		mpSoundDecoder;

	public:
		StreamSound();

		//������ʽ����
		virtual void LoadStreamSound( SoundDecoder* pSoundDecoder, DWORD dwCacheTime = 2000 ) = 0;

		//ж����ʽ����
		virtual void UnloadStreamSound() = 0;
	};
	//--------------------------------------------------------------------------------------------------------------
	typedef Stack< StreamSound* >	StreamSoundList;
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================