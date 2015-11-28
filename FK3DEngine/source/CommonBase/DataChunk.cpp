/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	DataChunk
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
	DataChunk::DataChunk()
		: mpData		(NULL)
		, mpPos			(NULL)
		, mActualLen	(0)
		, mProtectedLen	(0)
		, mRemainLen	(0)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	DataChunk::DataChunk( DataChunk& refDataChunk )
	{
		*this = refDataChunk;
	}
	//--------------------------------------------------------------------------------------------------------------
	//自动分配内存构造函数
	//
	//nSize：	分配内存大小
	DataChunk::DataChunk( size_t nSize )
		: mpData		(NULL)
		, mActualLen	(nSize)
		, mProtectedLen	(nSize)
		, mRemainLen	(nSize)
	{
		//如果为 DEBUG 模式则检测是否释放了内存
#ifdef _DEBUG
		if( mpData != NULL )
			Except( Exception::ERR_INTERNAL_ERROR, "分配数据块（DataChunk）内存之前没有释放先前分配的内存。" );
#endif
		//分配内存
		mpData = (BYTE*)malloc(nSize);
		if( mpData == NULL )
			Except( Exception::ERR_NOT_ENOUGH_MEM, "数据块（DataChunk）无法分配到足够的内存。" );

		mpPos = mpData;
	}
	//--------------------------------------------------------------------------------------------------------------
	DataChunk::~DataChunk()
	{
		SafeFree( mpData );
	}
	//--------------------------------------------------------------------------------------------------------------
	//分配数据块内存
	void DataChunk::Allocate( size_t nSize )
	{
		//如果为 DEBUG 模式则检测是否释放了内存
#ifdef _DEBUG
		if( mpData != NULL )
			Except( Exception::ERR_INTERNAL_ERROR, "分配数据块（DataChunk）内存之前没有释放先前分配的内存。" );
#endif
		//分配内存
		mpData = (BYTE*)malloc(nSize);
		if( mpData == NULL )
			Except( Exception::ERR_NOT_ENOUGH_MEM, "数据块（DataChunk）无法分配到足够的内存。" );

		mpPos = mpData;
		mRemainLen = mProtectedLen = mActualLen = nSize;
	}
	//--------------------------------------------------------------------------------------------------------------
	//释放数据块内存
	void DataChunk::Free()
	{
		SafeFree( mpData );
		mpPos = NULL;
		mProtectedLen = mActualLen = mRemainLen = 0;
	}
	//--------------------------------------------------------------------------------------------------------------
	//写入数据到数据块
	void DataChunk::Write( const void* pBuf, size_t nLen )
	{
		//如果写入数据长度大于剩余空间长度则报错
		if( nLen > mRemainLen )
			Except( Exception::ERR_INVALIDPARAMS, "写入数据长度大于数据块剩余空间长度，写入非法。" );

		memcpy( mpPos, pBuf, nLen );
		mpPos += nLen;
		mRemainLen -= nLen;
	}
	//--------------------------------------------------------------------------------------------------------------
	//从数据块读出数据
	void DataChunk::Read( void* pBuf, size_t nLen )
	{
		//如果读取数据长度大于剩余空间长度则报错
		if( nLen > mRemainLen )
			Except( Exception::ERR_INVALIDPARAMS, "读取数据长度大于数据块剩余空间长度，读取非法。" );

		memcpy( pBuf, mpPos, nLen );
		mpPos += nLen;
		mRemainLen -= nLen;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取当前读写指针（将不会受到保护）
	BYTE* DataChunk::GetPointer()
	{
		return mpPos;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置当前读写指针位置（不允许超过最大保护读写范围）
	void DataChunk::SetPosition( long Offset, BasePos eBasePos )
	{
		long TmpRemainLen = (long)mRemainLen;

		//判断移动偏移量基准类型
		switch (eBasePos)
		{
			//起始位置
		case BP_Begin:
			{
				TmpRemainLen = (long)mProtectedLen - Offset;
				break;
			}
			//结尾位置
		case BP_End:
			{
				TmpRemainLen = -Offset;
				break;
			}
			//当前读写位置
		case BP_Curpos:
			{
				TmpRemainLen -= Offset;
				break;
			}
		default:
			Except( Exception::ERR_INVALIDPARAMS, "非法的数据块读写指针移动基准位置类型。" );
		}

		if( TmpRemainLen > (long)mProtectedLen || TmpRemainLen < 0 )
			Except( Exception::ERR_INVALIDPARAMS, "不允许将数据块读写指针移动到保护读写范围之外。" );

		//移动读写指针位置
		mRemainLen = (size_t)TmpRemainLen;
		mpPos = mpData + mProtectedLen - mRemainLen;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取当前读写指针位置
	size_t DataChunk::GetPosition()
	{
		return mProtectedLen - mRemainLen;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置数据块最大保护读写长度（不允许超过当前分配内存大小）
	void DataChunk::SetProtectedLen( size_t NewLen )
	{
		if( NewLen > mActualLen )
			Except( Exception::ERR_INTERNAL_ERROR, "不允许设置的数据块最大保护读写长度大于实际分配的内存大小。" );

		//计算相对 增加/减少 的剩余读写长度
		long TmpRemainLen = (long)mRemainLen;
		TmpRemainLen += (long)NewLen - (long)mProtectedLen;

		//如果由于保护长度的减小造成读写指针在保护范围之外则将它移动到结尾
		mRemainLen = ( TmpRemainLen < 0 ) ? 0 : (size_t)TmpRemainLen;

		mProtectedLen = NewLen;
		mpPos = mpData + mProtectedLen - mRemainLen;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取保护读写长度
	size_t DataChunk::GetProtectedLen()
	{
		return mProtectedLen;
	}
	//--------------------------------------------------------------------------------------------------------------
	//重新分配数据块内存（保留内部读写指针相对位置并重置保护范围）
	//
	//nSize：	新的数据块大小
	//返回值：	新的相对读写指针（相对位置与原指针相同）
	BYTE* DataChunk::Reallocate( size_t nSize )
	{
		//分配内存
		mpData = (BYTE*)realloc( mpData, nSize );
		if( mpData == NULL )
			Except( Exception::ERR_NOT_ENOUGH_MEM, "数据块（DataChunk）无法重新分配到足够的内存。" );

		//重置保护范围为实际内存大小
		mActualLen = nSize;

		//计算相对 增加/减少 的剩余读写长度
		long TmpRemainLen = (long)mRemainLen;
		TmpRemainLen += (long)mActualLen - (long)mProtectedLen;

		//如果由于保护长度的减小造成读写指针在保护范围之外则将它移动到结尾
		mRemainLen = ( TmpRemainLen < 0 ) ? 0 : (size_t)TmpRemainLen;

		//保留内部读写指针相对位置
		mProtectedLen = mActualLen;
		mpPos = mpData + mProtectedLen - mRemainLen;

		return mpPos;
	}
	//--------------------------------------------------------------------------------------------------------------
	//赋值操作符
	DataChunk& DataChunk::operator = ( const DataChunk& refDataChunk )
	{
		SafeFree( mpData );

		mActualLen		= refDataChunk.mActualLen;
		mProtectedLen	= refDataChunk.mProtectedLen;
		mRemainLen		= refDataChunk.mRemainLen;

		mpData = (BYTE*)malloc( mActualLen );
		if( mpData == NULL )
			Except( Exception::ERR_NOT_ENOUGH_MEM, "数据块（DataChunk）无法分配到足够的内存。" );

		memcpy( mpData, refDataChunk.mpData, mActualLen );

		mpPos = mpData + mProtectedLen - mRemainLen;

		return *this;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
