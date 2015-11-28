/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	IConvert
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "Platform.h"
#include <errno.h>
//--------------------------------------------------------------------
#ifdef __APPLE__
#include </usr/include/iconv.h>
#else
#include <iconv.h>
#endif
//--------------------------------------------------------------------
namespace FK2DEngine
{
	template< typename Out, const char* p_pTo, const char* p_pFrom, typename In >
	Out Iconvert( const In& p_In )
	{
		if( p_In.empty() )
		{
			return Out();
		}

		const size_t ulBufferLength = 128;
		typedef typename In::value_type		InElem;
		typedef typename Out::value_type	OutElem;

		static iconv_t cd = iconv_open( p_pTo, p_pFrom );

		Out result;
		OutElem buffer[ulBufferLength];

		char* pInBuf = const_cast< char* >( reinterpret_cast< const char* >(&p_In[0]) );
		size_t ulInBytesLeft = p_In.size() * sizeof( InElem );
		char* pOutBuf = reinterpret_cast< char* >( buffer );
		size_t ulOutBytesLeft = sizeof( buffer );

		for( ; ; )
		{
			size_t ulRet = ::iconv( cd, &pInBuf, &ulInBytesLeft, &pOutBuf, &ulOutBytesLeft );
			if( ulRet == static_cast< size_t >( -1 ) && errno == EILSEQ )
			{
				++pInBuf;
				--ulInBytesLeft;
			}
			else if( ulRet == static_cast< size_t >( -1 ) && errno == E2BIG )
			{
				result.insert( result.end(), buffer, buffer + ulBufferLength );
				pOutBuf = reinterpret_cast< char* >( buffer );
				ulOutBytesLeft = sizeof( buffer );
			}
			else
			{
				result.insert( result.end(), buffer, buffer + ulBufferLength - ulOutBytesLeft / sizeof( OutElem ) );
				return result;
			}
		}
	}
}