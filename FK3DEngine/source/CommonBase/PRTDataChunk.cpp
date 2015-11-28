/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	PRTDataChunk
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
	PRTDataChunk::PRTDataChunk()
		: DataChunk()
	{
		ClearPassword();
	}
	//--------------------------------------------------------------------------------------------------------------
	PRTDataChunk::PRTDataChunk( PRTDataChunk& refDataChunk )
	{
		*this = refDataChunk;
	}
	//--------------------------------------------------------------------------------------------------------------
	PRTDataChunk::PRTDataChunk( size_t nSize )
		: DataChunk( nSize )
	{
		ClearPassword();
	}
	//--------------------------------------------------------------------------------------------------------------
	PRTDataChunk::~PRTDataChunk()
	{
		ClearPassword();
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置读写保护密码
	void PRTDataChunk::SetPassword( const char* szPassword )
	{
		//使用 MD5 算法将字符串转为 128 位密钥
		Misc::GetMD5Key( mpPassword, szPassword );

		//复制为 256 位双长度重复密钥
		memcpy( mpPassword + 16, mpPassword, 16 );
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置读写保护密码
	void PRTDataChunk::SetPassword( const BYTE pPassword[16] )
	{
		memcpy( mpPassword, pPassword, 16 );
		memcpy( mpPassword + 16, pPassword, 16 );
	}
	//--------------------------------------------------------------------------------------------------------------
	//清除读写保护密码
	void PRTDataChunk::ClearPassword()
	{
		memset( mpPassword, 0, sizeof(mpPassword) );
	}
	//--------------------------------------------------------------------------------------------------------------
	//写入数据到保护数据块
	void PRTDataChunk::Code( const void* pBuf, size_t nLen )
	{
		//如果写入数据长度大于剩余空间长度则报错
		if( nLen > mRemainLen )
			Except( Exception::ERR_INVALIDPARAMS, "写入数据长度大于保护数据块剩余空间长度，写入非法。" );

		//获取当前读取位置的第一个密码处理字节位置
		BYTE* pPassword = mpPassword + ( (int)( mpPos - mpData ) % 16 );

		//将数据复制到缓存中
		memcpy( mpPos, pBuf, nLen );

		//保护数据
		BYTE* pData = mpPos;
		UINT nRound = (UINT)nLen >> 4;
		UINT nRem = (UINT)nLen % 16;

		//处理 16 字节对齐数据
		for( UINT i=0; i<nRound; ++i )
		{
			*(UINT*)pData += *(UINT*)(pPassword);
			pData += 4;
			*(UINT*)pData += *(UINT*)(pPassword+4);
			pData += 4;
			*(UINT*)pData += *(UINT*)(pPassword+8);
			pData += 4;
			*(UINT*)pData += *(UINT*)(pPassword+12);
			pData += 4;
		}

		//进行剩余未对齐数据处理
		for( UINT i=0; i<nRem; ++i, ++pData )
		{
			*pData += pPassword[i];
			++pData;
		}

		mpPos += nLen;
		mRemainLen -= nLen;
	}
	//--------------------------------------------------------------------------------------------------------------
	//从保护数据块读出数据
	void PRTDataChunk::Decode( void* pBuf, size_t nLen )
	{
		//如果读取数据长度大于剩余空间长度则报错
		if( nLen > mRemainLen )
			Except( Exception::ERR_INVALIDPARAMS, "读取数据长度大于保护数据块剩余空间长度，读取非法。" );

		//获取当前读取位置的第一个密码处理字节位置
		BYTE* pPassword = mpPassword + ( (int)( mpPos - mpData ) % 16 );

		//将数据复制到缓存中
		memcpy( pBuf, mpPos, nLen );

		//保护数据
        BYTE* pData = (BYTE*)pBuf;	
		UINT nRound = (UINT)nLen >> 4;
		UINT nRem = (UINT)nLen % 16;

		//处理 16 字节对齐数据
		for( UINT i=0; i<nRound; ++i )
		{
			*(UINT*)pData -= *(UINT*)(pPassword);
			pData += 4;
			*(UINT*)pData -= *(UINT*)(pPassword+4);
			pData += 4;
			*(UINT*)pData -= *(UINT*)(pPassword+8);
			pData += 4;
			*(UINT*)pData -= *(UINT*)(pPassword+12);
			pData += 4;
		}

		//进行剩余未对齐数据处理
		for( UINT i=0; i<nRem; ++i, ++pData )
		{
			*pData -= pPassword[i];
			++pData;
		}

		mpPos += nLen;
		mRemainLen -= nLen;
	}
	//--------------------------------------------------------------------------------------------------------------
	//赋值操作符
	PRTDataChunk& PRTDataChunk::operator = ( const PRTDataChunk& refDataChunk )
	{
		//调用基类赋值操作符
		DataChunk::operator =( refDataChunk );

		//复制密码
		memcpy( mpPassword, refDataChunk.mpPassword, sizeof(mpPassword) );

		return *this;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
