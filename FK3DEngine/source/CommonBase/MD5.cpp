/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	MD5
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
	namespace MD5Algorithm
	{
		//----------------------------------------------------------------------------------------------------------

		//四个基本非线性函数
		#define MD5_F( x, y, z )			( ( (x) & (y) ) | ( (~x) & (z) ) )
		#define MD5_G( x, y, z )			( ( (x) & (z) ) | ( (y) & (~z) ) )
		#define MD5_H( x, y, z )			( (x) ^ (y) ^ (z) )
		#define MD5_I( x, y, z )			( (y) ^ ( (x) | (~z) ) )

		#define MD5_S( x, n )				( ( (x) << (n) ) | ( (x) >> ( 32 - (n) ) ) )

		#define MD5_FF( a,b,c,d,m,s,t )		a += MD5_F(b,c,d) + m + t; a = MD5_S(a,s) + b;
		#define MD5_GG( a,b,c,d,m,s,t )		a += MD5_G(b,c,d) + m + t; a = MD5_S(a,s) + b;
		#define MD5_HH( a,b,c,d,m,s,t )		a += MD5_H(b,c,d) + m + t; a = MD5_S(a,s) + b;
		#define MD5_II( a,b,c,d,m,s,t )		a += MD5_I(b,c,d) + m + t; a = MD5_S(a,s) + b;

		//----------------------------------------------------------------------------------------------------------
		//将整型编码为字节
		static void _MD5_EncodeUINTtoBYTE( BYTE* pOutput, const UINT* pInput, const UINT nLen )
		{
			UINT i=0;
			UINT j=0;

			for(; j<nLen; ++i, j+=4 )
			{
				pOutput[j]		= (BYTE)( ( pInput[i] )			& 0xff );
				pOutput[j+1]	= (BYTE)( ( pInput[i] >> 8 )	& 0xff );
				pOutput[j+2]	= (BYTE)( ( pInput[i] >> 16 )	& 0xff );
				pOutput[j+3]	= (BYTE)( ( pInput[i] >> 24 )	& 0xff );
			}
		}
		//----------------------------------------------------------------------------------------------------------
		//将字节解码为整型
		static void _MD5_DecodeBYTEtoUINT( UINT* pOutput, const BYTE* pInput, const UINT nLen )
		{
			UINT i=0;
			UINT j=0;

			for(; j<nLen; ++i, j+=4 )
			{
				pOutput[i] = ( (UINT)pInput[j] ) | ( ( (UINT)pInput[j+1] ) << 8 ) |
				( ( (UINT)pInput[j+2] ) << 16 ) | ( ( (UINT)pInput[j+3] ) << 24 );
			}
		}
		//----------------------------------------------------------------------------------------------------------
		//初始化 MD5 计算数据
		static void _MD5_Init( MD5_Contex* pContex )
		{
			//零长度
			pContex->nCount[0] = pContex->nCount[1] = 0;

			//存储链接变量数值
			pContex->nState[0] = MD5_A;
			pContex->nState[1] = MD5_B;
			pContex->nState[2] = MD5_C;
			pContex->nState[3] = MD5_D;
		}
		//----------------------------------------------------------------------------------------------------------
		//更新 MD5 计算数据
		static void _MD5_Update( MD5_Contex* pContex, const BYTE* pInput, const UINT nInputLen )
		{
			UINT nIndex = ( pContex->nCount[0] >> 3 ) & 63;

			//更新位长度
			pContex->nCount[0] += nInputLen << 3;

			if( pContex->nCount[0] < ( nInputLen << 3 ) )
				++pContex->nCount[1];

			pContex->nCount[1] += nInputLen >> 29;

			//计算更新长度与 64 的模
			UINT nPartLen = 64 - nIndex;

			//处理信息
			UINT i = 0;
			if( nInputLen >= nPartLen )
			{
				//复制 64 位信息数据
				memcpy( &pContex->pBuffer[nIndex], pInput, nPartLen );

				_MD5_Process( pContex->nState, pContex->pBuffer );

				for( i=nPartLen; i+63<nInputLen; i+=64 )
					_MD5_Process( pContex->nState, &pInput[i] );

				nIndex = 0;
			}

			//缓存剩余的输入数据
			memcpy( &pContex->pBuffer[nIndex], &pInput[i], nInputLen - i );
		}
		//----------------------------------------------------------------------------------------------------------
		//结束 MD5 计算
		static void _MD5_Final( BYTE pMD5Key[16], MD5_Contex* pContex )
		{
			//存储位数量
			BYTE pBitsNum[8];
			_MD5_EncodeUINTtoBYTE( pBitsNum, pContex->nCount, 8 );

			//附加 56 与 64 的模
			UINT nIndex = ( pContex->nCount[0] >> 3 ) & 63;
			UINT nPadLen = ( nIndex < 56 ) ? ( 56 - nIndex ) : ( 120 - nIndex );

			static BYTE pPadding[64] = {
				0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

			_MD5_Update( pContex, pPadding, nPadLen );

			//附加信息长度
			_MD5_Update( pContex, pBitsNum, 8 );

			//存储 MD5 密钥
			_MD5_EncodeUINTtoBYTE( pMD5Key, pContex->nState, 16 );

			//清除加密信息
			memset( pContex, 0, sizeof(MD5_Contex) );
		}
		//----------------------------------------------------------------------------------------------------------
		//处理 MD5 信息数据
		static void _MD5_Process( UINT pState[4], const BYTE pBlock[64] )
		{
			UINT a = pState[0];
			UINT b = pState[1];
			UINT c = pState[2];
			UINT d = pState[3];

			UINT m[16];
			_MD5_DecodeBYTEtoUINT( m, pBlock, 64 );

			//第一轮
			MD5_FF( a, b, c, d, m[0],  7,  0xd76aa478 );
			MD5_FF( d, a, b, c, m[1],  12, 0xe8c7b756 );
			MD5_FF( c, d, a, b, m[2],  17, 0x242070db );
			MD5_FF( b, c, d, a, m[3],  22, 0xc1bdceee );
			MD5_FF( a, b, c, d, m[4],  7,  0xf57c0faf );
			MD5_FF( d, a, b, c, m[5],  12, 0x4787c62a );
			MD5_FF( c, d, a, b, m[6],  17, 0xa8304613 );
			MD5_FF( b, c, d, a, m[7],  22, 0xfd469501 );
			MD5_FF( a, b, c, d, m[8],  7,  0x698098d8 );
			MD5_FF( d, a, b, c, m[9],  12, 0x8b44f7af );
			MD5_FF( c, d, a, b, m[10], 17, 0xffff5bb1 );
			MD5_FF( b, c, d, a, m[11], 22, 0x895cd7be );
			MD5_FF( a, b, c, d, m[12], 7,  0x6b901122 );
			MD5_FF( d, a, b, c, m[13], 12, 0xfd987193 );
			MD5_FF( c, d, a, b, m[14], 17, 0xa679438e );
			MD5_FF( b, c, d, a, m[15], 22, 0x49b40821 );

			//第二轮
			MD5_GG( a, b, c, d, m[1],  5,  0xf61e2562 );
			MD5_GG( d, a, b, c, m[6],  9,  0xc040b340 );
			MD5_GG( c, d, a, b, m[11], 14, 0x265e5a51 );
			MD5_GG( b, c, d, a, m[0],  20, 0xe9b6c7aa );
			MD5_GG( a, b, c, d, m[5],  5,  0xd62f105d );
			MD5_GG( d, a, b, c, m[10], 9,  0x02441453 );
			MD5_GG( c, d, a, b, m[15], 14, 0xd8a1e681 );
			MD5_GG( b, c, d, a, m[4],  20, 0xe7d3fbc8 );
			MD5_GG( a, b, c, d, m[9],  5,  0x21e1cde6 );
			MD5_GG( d, a, b, c, m[14], 9,  0xc33707d6 );
			MD5_GG( c, d, a, b, m[3],  14, 0xf4d50d87 );
			MD5_GG( b, c, d, a, m[8],  20, 0x455a14ed );
			MD5_GG( a, b, c, d, m[13], 5,  0xa9e3e905 );
			MD5_GG( d, a, b, c, m[2],  9,  0xfcefa3f8 );
			MD5_GG( c, d, a, b, m[7],  14, 0x676f02d9 );
			MD5_GG( b, c, d, a, m[12], 20, 0x8d2a4c8a );

			//第三轮
			MD5_HH( a, b, c, d, m[5],  4,  0xfffa3942 );
			MD5_HH( d, a, b, c, m[8],  11, 0x8771f681 );
			MD5_HH( c, d, a, b, m[11], 16, 0x6d9d6122 );
			MD5_HH( b, c, d, a, m[14], 23, 0xfde5380c );
			MD5_HH( a, b, c, d, m[1],  4,  0xa4beea44 );
			MD5_HH( d, a, b, c, m[4],  11, 0x4bdecfa9 );
			MD5_HH( c, d, a, b, m[7],  16, 0xf6bb4b60 );
			MD5_HH( b, c, d, a, m[10], 23, 0xbebfbc70 );
			MD5_HH( a, b, c, d, m[13], 4,  0x289b7ec6 );
			MD5_HH( d, a, b, c, m[0],  11, 0xeaa127fa );
			MD5_HH( c, d, a, b, m[3],  16, 0xd4ef3085 );
			MD5_HH( b, c, d, a, m[6],  23, 0x04881d05 );
			MD5_HH( a, b, c, d, m[9],  4,  0xd9d4d039 );
			MD5_HH( d, a, b, c, m[12], 11, 0xe6db99e5 );
			MD5_HH( c, d, a, b, m[15], 16, 0x1fa27cf8 );
			MD5_HH( b, c, d, a, m[2],  23, 0xc4ac5665 );

			//第四轮
			MD5_II( a, b, c, d, m[0],  6,  0xf4292244 );
			MD5_II( d, a, b, c, m[7],  10, 0x432aff97 );
			MD5_II( c, d, a, b, m[14], 15, 0xab9423a7 );
			MD5_II( b, c, d, a, m[5],  21, 0xfc93a039 );
			MD5_II( a, b, c, d, m[12], 6,  0x655b59c3 );
			MD5_II( d, a, b, c, m[3],  10, 0x8f0ccc92 );
			MD5_II( c, d, a, b, m[10], 15, 0xffeff47d );
			MD5_II( b, c, d, a, m[1],  21, 0x85845dd1 );
			MD5_II( a, b, c, d, m[8],  6,  0x6fa87e4f );
			MD5_II( d, a, b, c, m[15], 10, 0xfe2ce6e0 );
			MD5_II( c, d, a, b, m[6],  15, 0xa3014314 );
			MD5_II( b, c, d, a, m[13], 21, 0x4e0811a1 );
			MD5_II( a, b, c, d, m[4],  6,  0xf7537e82 );
			MD5_II( d, a, b, c, m[11], 10, 0xbd3af235 );
			MD5_II( c, d, a, b, m[2],  15, 0x2ad7d2bb );
			MD5_II( b, c, d, a, m[9],  21, 0xeb86d391 );

			pState[0] += a;
			pState[1] += b;
			pState[2] += c;
			pState[3] += d;
		}
		//----------------------------------------------------------------------------------------------------------

		#undef MD5_FF
		#undef MD5_GG
		#undef MD5_HH
		#undef MD5_II

		#undef MD5_S

		#undef MD5_F
		#undef MD5_G
		#undef MD5_H
		#undef MD5_I

		//----------------------------------------------------------------------------------------------------------
	}
	//--------------------------------------------------------------------------------------------------------------

	//计算 MD5 密钥
	void Misc::GetMD5Key( BYTE pMD5Key[16], const char* pString )
	{
		//获取原始数据长度
		UINT nStrLen = (UINT)strlen( pString );

		//初始化 MD5 计算数据
		MD5Algorithm::MD5_Contex contex;
		MD5Algorithm::_MD5_Init( &contex );

		//计算 MD5 密钥
		MD5Algorithm::_MD5_Update( &contex, reinterpret_cast<const BYTE*>( pString ), nStrLen );
		MD5Algorithm::_MD5_Final( pMD5Key, &contex );
	}

	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
