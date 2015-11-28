/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	MP3Decoder
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
	static const char ERR_DECODE_MPEG_FRAME_INFO[] = "解码 MPEG 声音数据帧信息头错误。";
	//--------------------------------------------------------------------------------------------------------------
	MP3Decoder::MP3Decoder()
		: mdwID3v1Len		(0)
		, mdwID3v2Len		(0)
		, mdwFramePCMSize	(0)
		, mdwNumFrames		(0)
		, mdwPlayTime		(0)
		, mdwPrefectchLen	(0)
		, mdwReadDataLen	(0)
		, mpFramePCMBuffer	(NULL)
	{
		//初始化 MPEG 音频解码核心
		InitAudioDecoder();
	}
	//--------------------------------------------------------------------------------------------------------------
	MP3Decoder::~MP3Decoder()
	{
		CloseSoundFile();
	}
	//--------------------------------------------------------------------------------------------------------------
	//打开声音文件
	SoundFormat* MP3Decoder::OpenSoundFile( FileObject* pFile, LPCSTR szFileName )
	{
		//打开声音文件
		pFile->Open( szFileName, OT_Read, true );

		mdwID3v2Len = mdwID3v1Len = 0;


		//读取 ID3v1 数据
		DWORD dwFileLen = pFile->GetLength();

		memset( &mOverlapped, 0, sizeof(OVERLAPPED) );
		mOverlapped.Offset = dwFileLen - 128;

		if( !pFile->AsynRead( &mID3v1Data, 128, &mOverlapped ) )
		{
			if( 128 != pFile->GetAsynResult( &mOverlapped ) )
				Except( Exception::ERR_INTERNAL_ERROR, "读取 MP3 文件 ID3v1 信息失败。" );
		}

		//如果存在 ID3v1 数据
		if( 0 == memcmp( mID3v1Data.szTAG, "TAG", 3 ) )
			mdwID3v1Len = 128;


		//读取 ID3v2 数据头
		ID3v2Header sID3v2Header;
		mOverlapped.Offset = 0;

		if( !pFile->AsynRead( &sID3v2Header, sizeof(ID3v2Header), &mOverlapped ) )
		{
			if( sizeof(ID3v2Header) != pFile->GetAsynResult( &mOverlapped ) )
				Except( Exception::ERR_INTERNAL_ERROR, "读取 MP3 文件 ID3v2 信息失败。" );
		}

		//如果存在 ID3v2 数据头
		if( 0 == memcmp( sID3v2Header.szID3, "ID3", 3 ) )
		{
			//解码 ID3v2 数据长度（安全同步整数值）
			mdwID3v2Len = sID3v2Header.szSize[3] |
				( (DWORD)sID3v2Header.szSize[2] << 7 ) |
				( (DWORD)sID3v2Header.szSize[1] << 14 ) |
				( (DWORD)sID3v2Header.szSize[0] << 21 );

			//根据是否存在 ID3v2 脚注计算偏移值
			mdwID3v2Len = ( sID3v2Header.bFlag & 32 ) ? ( mdwID3v2Len + 20 ) : ( mdwID3v2Len + 10 );
		}

		//存储声音文件指针
		mpSoundFile = pFile;

		//遍历文件所有帧
		_CheckAllFrames();

		//设置声音格式信息
		DWORD dwBytesPerSec = mFrameInfo.dwFrequency * mFrameInfo.dwChannels * 2;

		mSoundFormat.dwChannels		= mFrameInfo.dwChannels;		//声音声道数
		mSoundFormat.dwFrequency	= mFrameInfo.dwFrequency;		//声音频率
		mSoundFormat.dwBitRate		= 16;							//声音比特率
		mSoundFormat.dwBytesPerSec	= dwBytesPerSec;				//播放每秒字节数

		//计算输出数据长度
		mdwFramePCMSize = 144 * 16 * mFrameInfo.dwChannels;
		mSoundFormat.dwLength = mdwNumFrames * mdwFramePCMSize;

		//总播放时间（近似值）
		mdwPlayTime = mSoundFormat.dwLength / dwBytesPerSec;

		//分配文件预读缓存和解码声音数据缓存
		mpFramePCMBuffer = (char*)malloc( 2 * mdwFileCacheLen + mdwFramePCMSize );
		mpFileReadPtr = mpFramePCMBuffer + mdwFramePCMSize;
		mpFileWorkPtr = mpFileReadPtr + mdwFileCacheLen;

		//重置声音读取位置到起点
		ResetReadPos();

		return &mSoundFormat;
	}
	//--------------------------------------------------------------------------------------------------------------
	//关闭声音文件
	void MP3Decoder::CloseSoundFile()
	{
		//关闭声音文件
		if( mpSoundFile != NULL )
		{
			//如果正在预读文件
			if( mdwPrefectchLen > 0 )
				_EndPrefectch();

			mpSoundFile->Close();
			mpSoundFile = NULL;

			free( mpFramePCMBuffer );
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//重置声音读取位置到起点
	void MP3Decoder::ResetReadPos()
	{
		//如果正在预读文件
		if( mdwPrefectchLen > 0 )
			_EndPrefectch();

		//设置起始解码位置
		mdwFramePCMLen = 0;
		mdwFrameOffset = 0;

		//读取起始文件数据
		_BeginPrefectch();
		_EndPrefectch();

		//开始预读第二段文件数据
		_BeginPrefectch();
	}
	//--------------------------------------------------------------------------------------------------------------
	//读取声音数据到指定缓存
	void MP3Decoder::ReadSoundData( void* pBuf, DWORD dwLen )
	{
		static char pFrameDataBuf[MPEG_MaxFrameSize] = { NULL };

		DWORD dwFinishLen = 0;
		char* pDstPtr = (char*)pBuf;

		//如果上次已经解码了当前帧部分声音数据
		if( mdwFramePCMLen != 0 )
		{
			char* pFramePCMPtr = mpFramePCMBuffer + mdwFramePCMSize - mdwFramePCMLen;

			//如果读取长度大于已解码长度
			if( dwLen > mdwFramePCMLen  )
			{
				memcpy( pDstPtr, pFramePCMPtr, mdwFramePCMLen );
				pDstPtr += mdwFramePCMLen;
				dwFinishLen += mdwFramePCMLen;
			}
			//如果读取长度小于已解码长度
			else
			{
				memcpy( pDstPtr, pFramePCMPtr, dwLen );
				mdwFramePCMLen -= dwLen;
				return;
			}
		}

		//循环解码每一帧
		while( dwFinishLen < dwLen )
		{
			char* pFrameDataPtr;
			DWORD dwRemainDataLen = mdwReadDataLen - mdwFrameOffset;

			//如果当前帧信息头文件数据不完全在缓存中
			if( dwRemainDataLen < 4 )
			{
				//复制剩余数据
				memcpy( pFrameDataBuf, mpFileWorkPtr + mdwFrameOffset, dwRemainDataLen );

				//结束上次预读并开始下一次预读
				_EndPrefectch();
				_BeginPrefectch();

				//复制新读取的另一部分数据
				mdwFrameOffset = 4 - dwRemainDataLen;
				memcpy( pFrameDataBuf + dwRemainDataLen, mpFileWorkPtr, mdwFrameOffset );

				//解码帧数据头
				if( !_DecodeFrameHeader( (BYTE*)pFrameDataBuf, &mFrameInfo ) )
					Except( Exception::ERR_INTERNAL_ERROR, ERR_DECODE_MPEG_FRAME_INFO );

				//复制帧声音数据
				memcpy( pFrameDataBuf + 4, mpFileWorkPtr + 4, mFrameInfo.dwFrameSize );
				mdwFrameOffset += mFrameInfo.dwFrameSize;

				pFrameDataPtr = pFrameDataBuf + 4;
			}
			//如果当前帧信息头文件数据在缓存中
			else
			{
				pFrameDataPtr = mpFileWorkPtr + mdwFrameOffset;

				//解码帧数据头
				if( !_DecodeFrameHeader( (BYTE*)pFrameDataPtr, &mFrameInfo ) )
					Except( Exception::ERR_INTERNAL_ERROR, ERR_DECODE_MPEG_FRAME_INFO );

				dwRemainDataLen -= 4;
				mdwFrameOffset += 4;

				//如果剩余数据缓存不包含全部帧声音数据
				if( dwRemainDataLen < mFrameInfo.dwFrameSize )
				{
					//复制剩余数据
					if( dwRemainDataLen > 0 )
						memcpy( pFrameDataBuf, mpFileWorkPtr + mdwFrameOffset, dwRemainDataLen );

					//结束上次预读并开始下一次预读
					_EndPrefectch();
					_BeginPrefectch();

					//复制新读取的另一部分数据
					mdwFrameOffset = mFrameInfo.dwFrameSize - dwRemainDataLen;
					memcpy( pFrameDataBuf + dwRemainDataLen, mpFileWorkPtr, mdwFrameOffset );
					pFrameDataPtr = pFrameDataBuf;
				}
				//如果剩余数据缓存包含全部帧声音数据
				else
				{
					pFrameDataPtr += 4;
					mdwFrameOffset += mFrameInfo.dwFrameSize;
				}
			}

			//解码帧声音数据
			DWORD dwDecodePCMLen = (DWORD)DecodeAudioFrame( &mFrameInfo, pFrameDataPtr, mpFramePCMBuffer );
			if( dwDecodePCMLen != mdwFramePCMSize )
				Except( Exception::ERR_INTERNAL_ERROR, "MPEG 音频解码输出数据长度错误。" );

			//计算需要复制到读取目标的数据长度
			DWORD dwReamainReadLen = dwLen - dwFinishLen;
			DWORD dwCopyLen = ( dwReamainReadLen > mdwFramePCMSize ) ? mdwFramePCMSize : dwReamainReadLen;
			memcpy( pDstPtr, mpFramePCMBuffer, dwCopyLen );
			mdwFramePCMLen = mdwFramePCMSize - dwCopyLen;
			dwFinishLen += dwCopyLen;
			pDstPtr += dwCopyLen;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//遍历文件所有帧
	void MP3Decoder::_CheckAllFrames()
	{
		//略过 ID3v2 数据段
		mOverlapped.Offset = mdwID3v2Len;

		DWORD dwFileLen = mpSoundFile->GetLength();
		BYTE szFrameHeader[4];

		for(;;)
		{
			//如果已到达文件结尾
			if( dwFileLen - mOverlapped.Offset < 4 )
				break;

			//读取帧信息头数据
			if( !mpSoundFile->AsynRead( szFrameHeader, 4, &mOverlapped ) )
			{
				if( 4 != mpSoundFile->GetAsynResult( &mOverlapped ) )
					Except( Exception::ERR_INTERNAL_ERROR, "读取 MPEG 声音数据帧信息头错误。" );
			}

			//解码帧信息头
			if( !_DecodeFrameHeader( szFrameHeader, &mFrameInfo ) )
				break;

			//如果剩余文件长度小于当前帧数据长度
			DWORD dwFrameLen = mFrameInfo.dwFrameSize + 4;
			if( dwFileLen - mOverlapped.Offset < dwFrameLen )
				break;

			//移动到下一帧
			++mdwNumFrames;

			mOverlapped.Offset += dwFrameLen;
		}

		//记录声音数据结尾位置
		mSoundDataEnd = mOverlapped.Offset;
		mOverlapped.Offset = mdwID3v2Len;
	}
	//--------------------------------------------------------------------------------------------------------------
	//解码帧信息头
	bool MP3Decoder::_DecodeFrameHeader( BYTE szFrameHeader[4], MPEGFrameInfo* pFrameInfo )
	{
		//调整头数据顺序
		DWORD dwFrameHeader;
		BYTE* pFrameHeader = (BYTE*)&dwFrameHeader;
		pFrameHeader[0] = szFrameHeader[3];
		pFrameHeader[1] = szFrameHeader[2];
		pFrameHeader[2] = szFrameHeader[1];
		pFrameHeader[3] = szFrameHeader[0];

		//检查是否为正确帧信息头（前 11 位都为真）
		if( ( dwFrameHeader >> 21 ) != 0x7FF )
			return false;

		DWORD dwBitRateIndex = ( dwFrameHeader >> 12 ) & 15;
		DWORD dwFrequencyIndex = ( dwFrameHeader >> 10 ) & 3;

		//获取 MPEG 帧信息
		pFrameInfo->bCRC			= ( ( dwFrameHeader >> 16 ) & 1 ) ? false : true;	//是否存在 CRC 校验数据
		pFrameInfo->bPadding		= ( ( dwFrameHeader >> 9 ) & 1 ) ? true : false;	//是否存在补充位
		pFrameInfo->bCopyright		= ( ( dwFrameHeader >> 3 ) & 1 ) ? true : false;	//是否享有版权
		pFrameInfo->bOriginal		= ( ( dwFrameHeader >> 2 ) & 1 ) ? true : false;	//是否为原创
		pFrameInfo->bJointMode		= (BYTE)( ( dwFrameHeader >> 4 ) & 3 );				// Joint stereo 模式
		pFrameInfo->bEmphasis		= (BYTE)( dwFrameHeader & 3 );						// Emphasis 模式

		//计算出声道数
		pFrameInfo->nChannelMode = ( dwFrameHeader >> 6 ) & 3;
		pFrameInfo->dwChannels = ( pFrameInfo->nChannelMode == 3 ) ? 1 : 2;

		//计算 MPEG 版本和层数
		int nMpegVersion = ( dwFrameHeader >> 19 ) & 3;
		switch (nMpegVersion)
		{
		case 3:
			pFrameInfo->bMpegVersion = MPEG_Version1;
			pFrameInfo->nFreqIndex = dwFrequencyIndex;
			break;
		case 2:
			pFrameInfo->bMpegVersion = MPEG_Version2;
			pFrameInfo->nFreqIndex = dwFrequencyIndex + 3;
			break;
		case 0:
			pFrameInfo->bMpegVersion = MPEG_Version25;
			pFrameInfo->nFreqIndex = dwFrequencyIndex + 6;
			break;
		default:
			Except( Exception::ERR_INTERNAL_ERROR, "错误的 MPEG 音频帧压缩版本。" );
		}

		pFrameInfo->bMpegLayer = (BYTE)( 3 - ( ( dwFrameHeader >> 17 ) & 3 ) );

		//映射出 MPEG 声音比特率和频率
		pFrameInfo->dwBitRate	= MPEG_BitRateTable[ pFrameInfo->bMpegVersion ][ pFrameInfo->bMpegLayer ][ dwBitRateIndex ];
		pFrameInfo->dwFrequency	= MPEG_FrequencyTable[ pFrameInfo->nFreqIndex ];
		pFrameInfo->nBitRateIndex = dwBitRateIndex;

		if( pFrameInfo->dwBitRate == 0 )
			Except( Exception::ERR_INTERNAL_ERROR, "错误的 MPEG 音频帧比特率。" );

		//计算帧声音数据长度
		DWORD dwFrameSize;
		switch (pFrameInfo->bMpegLayer)
		{
		case MPEG_Layer1:
			dwFrameSize  = pFrameInfo->dwBitRate * 12000;
			dwFrameSize /= pFrameInfo->dwFrequency;
			dwFrameSize  = ( ( dwFrameSize + ( pFrameInfo->bPadding ? 1 : 0 ) ) << 2 ) - 4;
			break;
		case MPEG_Layer2:
			dwFrameSize  = pFrameInfo->dwBitRate * 144000;
			dwFrameSize /= pFrameInfo->dwFrequency;
			dwFrameSize += ( pFrameInfo->bPadding ? 1 : 0 ) - 4;
			break;
		case MPEG_Layer3:
			dwFrameSize  = pFrameInfo->dwBitRate * 144000;
			dwFrameSize /= pFrameInfo->dwFrequency << ( pFrameInfo->bMpegVersion ? 1 : 0 );
			dwFrameSize += ( pFrameInfo->bPadding ? 1 : 0 ) - 4;
			break; 
		default:
			Except( Exception::ERR_INTERNAL_ERROR, "错误的 MPEG 音频帧压缩层索引。" );
		}

		pFrameInfo->dwFrameSize = dwFrameSize;

		return true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//开始预读文件数据
	void FKFastcall MP3Decoder::_BeginPrefectch()
	{
		//如果剩余数据长度小于缓存长度
		DWORD dwReamainLen = mSoundDataEnd - mOverlapped.Offset;
		if( dwReamainLen <= mdwFileCacheLen )
			mdwPrefectchLen = dwReamainLen;
		else
			mdwPrefectchLen = mdwFileCacheLen;

		//预读文件数据
		mpSoundFile->AsynRead( mpFileReadPtr, mdwPrefectchLen, &mOverlapped );

		if( dwReamainLen <= mdwFileCacheLen )
			mOverlapped.Offset = mdwID3v2Len;
		else
			mOverlapped.Offset += mdwFileCacheLen;
	}
	//--------------------------------------------------------------------------------------------------------------
	//结束预读文件数据
	void FKFastcall MP3Decoder::_EndPrefectch()
	{
		if( mdwPrefectchLen != mpSoundFile->GetAsynResult( &mOverlapped ) )
			Except( Exception::ERR_WINAPI_ERROR, "异步读取 MP3 文件数据失败。" );

		//切换文件数据解码/读取缓存
		char* pTmpPtr = mpFileWorkPtr;
		mpFileWorkPtr = mpFileReadPtr;
		mpFileReadPtr = pTmpPtr;

		mdwReadDataLen = mdwPrefectchLen;
		mdwPrefectchLen = 0;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
