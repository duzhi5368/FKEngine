/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	WaveDecoder
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../../include/CommonBase/CommonBaseInsideHead.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	WaveDecoder::WaveDecoder()
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	WaveDecoder::~WaveDecoder()
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	//打开声音文件
	SoundFormat* WaveDecoder::OpenSoundFile( FileObject* pFile, LPCSTR szFileName )
	{
		//打开声音文件
		pFile->Open( szFileName, OT_Read );

		//读取 WAV 文件数据头
		WaveHeader sFileHeader;
		pFile->Read( &sFileHeader, sizeof(WaveHeader) );

		//检查文件头是否正确
		if( 0 != memcmp( sFileHeader.szRIFF, "RIFF", 4 ) )
			goto ErrorFormat;

		if( 0 != memcmp( sFileHeader.szWAVE, "WAVEfmt ", 8 ) )
			goto ErrorFormat;

		//如果是压缩 WAV 文件
		if( sFileHeader.wFormatTag != 1 )
			Except( Exception::ERR_INTERNAL_ERROR, "无法解码压缩的 WAV 文件。" );

		mSoundFormat.dwChannels		= (DWORD)sFileHeader.wChannels;
		mSoundFormat.dwFrequency	= sFileHeader.dwSamplePerSec;
		mSoundFormat.dwBitRate		= (DWORD)sFileHeader.wBitsPerSample;
		mSoundFormat.dwBytesPerSec	= sFileHeader.dwBytesPerSec;
		mSoundFormat.dwLength		= sFileHeader.dwDataLen;

		//保存文件指针
		mpSoundFile = pFile;

		return &mSoundFormat;

		//文件格式错误处理
ErrorFormat:
		pFile->Close();
		Except( Exception::ERR_CANNOT_OPEN_FILE, "错误的 WAV 文件格式。" );
	}
	//--------------------------------------------------------------------------------------------------------------
	//关闭声音文件
	void WaveDecoder::CloseSoundFile()
	{
		//关闭声音文件
		if( mpSoundFile != NULL )
		{
			mpSoundFile->Close();
			mpSoundFile = NULL;
		}		
	}
	//--------------------------------------------------------------------------------------------------------------
	//重置声音读取位置到起点
	void WaveDecoder::ResetReadPos()
	{
		mpSoundFile->SetPosition( sizeof(WaveHeader), BP_Begin );
	}
	//--------------------------------------------------------------------------------------------------------------
	//读取声音数据到指定缓存
	void WaveDecoder::ReadSoundData( void* pBuf, DWORD dwLen )
	{
		mpSoundFile->Read( pBuf, dwLen );
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
