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
*	文件说明：	数据块类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//数据块类
	class FK_DLL DataChunk
	{
	protected:
		BYTE*		mpData;			//数据缓存指针
		BYTE*		mpPos;			//当前读取位置指针
		size_t		mActualLen;		//实际内存空间大小
		size_t		mProtectedLen;	//最大保护读写长度
		size_t		mRemainLen;		//当前读取位置到结尾数据长度

	public:
		DataChunk();
		DataChunk( DataChunk& refDataChunk );
		DataChunk( size_t nSize );
		virtual ~DataChunk();

		//分配数据块内存
		void Allocate( size_t nSize );

		//释放数据块内存
		void Free();

		//写入数据到数据块
		void Write( const void* pBuf, size_t nLen );

		//从数据块读出数据
		void Read( void* pBuf, size_t nLen );

		//获取当前读写指针（将不会受到保护）
		BYTE* GetPointer();

		//设置当前读写指针位置（不允许超过最大保护读写范围）
		void SetPosition( long Offset, BasePos eBasePos = BP_Begin );

		//获取当前读写指针位置
		size_t GetPosition();

		//设置数据块最大保护读写长度（不允许超过当前分配内存大小）
		void SetProtectedLen( size_t NewLen );

		//获取保护读写长度
		size_t GetProtectedLen();

		//重新分配数据块内存（保留内部读写指针相对位置并重置保护范围）
		BYTE* Reallocate( size_t nSize );

		//赋值操作符
		DataChunk& operator = ( const DataChunk& refDataChunk );
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
