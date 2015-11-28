/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	MemPool
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

#ifndef FK_NO_TEMPLATE_IMPLEMNTATION

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//内存池成员函数
	//--------------------------------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------------------------------
	template <typename T> MemPool<T>::MemPool()
		: m_pPoolInfo		(NULL)
		, m_nNumPool		(0)
		, m_nPoolDataLen	(0)
		, m_pCurPool		(NULL)
		, m_pCurNum			(NULL)
		, m_pNextPool		(NULL)
		, m_pNextNum		(NULL)
		, m_nPoolSize		(0)
		, m_nNumData		(0)
		, m_nMaxNumData		(0)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	template <typename T> MemPool<T>::~MemPool()
	{
		Release();
	}
	//--------------------------------------------------------------------------------------------------------------
	//初始化内存池（NumPerPool 为单个内存池的元素数，PoolNum 为初始内存池数量。）
	template <typename T> void MemPool<T>::Initialize( unsigned int NumPerPool, unsigned int PoolNum )
	{
		m_nPoolSize = NumPerPool;
		m_nNumPool = PoolNum;
		m_nMaxNumData = m_nPoolSize * m_nNumPool;

		m_nPoolDataLen = ( sizeof(T)+sizeof(bool) ) * m_nPoolSize;

		//分配内存池的内存地址数组
		m_pPoolInfo = (PoolInfo*)malloc( sizeof(PoolInfo) * m_nNumPool );
		if( m_pPoolInfo == NULL )
		{
			Misc::ShowErrMsg( "初始化内存池（%s）失败。", __FUNCSIG__ );
			return;
		}

		//创建所有内存池
		PoolInfo* pPoolInfo = m_pPoolInfo;
		for( unsigned int i=0; i<m_nNumPool; ++i )
		{
			pPoolInfo->pPool = (T*)malloc( m_nPoolDataLen );
			if( pPoolInfo->pPool == NULL )
			{
				Misc::ShowErrMsg( "初始化内存池（%s）失败。", __FUNCSIG__  );
				return;
			}
			pPoolInfo->pbIsUsed = (bool*)( pPoolInfo->pPool + m_nPoolSize );
			memset( pPoolInfo->pPool, 0, m_nPoolDataLen );

			pPoolInfo->nNumEmpty = m_nPoolSize;
			++pPoolInfo;
		}

		//设置下一个可用元素
		m_pNextPool = m_pPoolInfo;
		m_pNextNum = 0;
	}
	//--------------------------------------------------------------------------------------------------------------
	//释放内存池
	template <typename T> void MemPool<T>::Release()
	{
		//如果记录信息为空则返回
		if( m_pPoolInfo == NULL )
			return;

		//释放所有内存池空间
		PoolInfo* pPoolInfo = m_pPoolInfo;
		for( unsigned int i=0; i<m_nNumPool; ++i )
		{
			free( pPoolInfo->pPool );
			++pPoolInfo;
		}

		//释放内存池信息数组
		free( m_pPoolInfo );
		m_pPoolInfo = NULL;
	}
	//--------------------------------------------------------------------------------------------------------------
	//清空内存池中所有元素
	template <typename T> void MemPool<T>::Clear()
	{
		//循环所有内存池
		PoolInfo* pPoolInfo = m_pPoolInfo;
		for( unsigned int i=0; i<m_nNumPool; ++j )
		{
			memset( pPoolInfo->pbIsUsed, 0, sizeof(bool)*m_nPoolSize );
			pPoolInfo->nNumEmpty = m_nPoolSize;
			++pPoolInfo;
		}

		m_nNumData = 0;
		m_pNextPool = m_pPoolInfo;
		m_pNextNum = 0;
	}
	//--------------------------------------------------------------------------------------------------------------
	//增加新元素
	template <typename T> T* MemPool<T>::Add()
	{
		//如果指定了下一个空闲位置
		if( m_pNextPool != NULL )
		{
			m_pCurPool = m_pNextPool;
			m_pCurNum = m_pNextNum;
		}
		//如果还有空闲位置
		else if( m_nNumData < m_nMaxNumData )
		{
			//循环所有内存池
			PoolInfo* pPoolInfo = m_pPoolInfo;
			for( unsigned int j=0; j<m_nNumPool; ++j )
			{
				//如果该内存池中还有空闲位置
				if( pPoolInfo->nNumEmpty != 0 )
				{
					bool* pbIsUsed = pPoolInfo->pbIsUsed;
					for( unsigned int i=0; i<m_nPoolSize; ++i )
					{
						//如果该位置空闲
						if( *pbIsUsed == false )
						{
							m_pCurPool = pPoolInfo;
							m_pCurNum = i;

							goto FindEmpty;
						}
						++pbIsUsed;
					}
				}

				++pPoolInfo;
			}

			Misc::ShowErrMsg( "在该内存池（%s）中已经找不到任何空元素位置，\n但是空元素位置计数却不是零。",
				__FUNCSIG__ );
			return NULL;
		}
		//如果已经没有空闲元素位置则增加内存池
		else
			AllocNewPool();

FindEmpty:
		++m_nNumData;

		m_pCurPool->nNumEmpty -= 1;
		m_pCurPool->pbIsUsed[ m_pCurNum ] = true;

		//如果该位置的下一个位置也可用则预先设置该位置为下一个元素存放空间
		unsigned int NumNext = m_pCurNum + 1;
		if( NumNext < m_nPoolSize && m_pCurPool->pbIsUsed[ NumNext ] == false )
		{
			m_pNextPool = m_pCurPool;
			m_pNextNum = NumNext;
		}
		//否则设置 m_pNextPool 为 NULL，以表示需要查找空闲位置。
		else
		{
			m_pNextPool = NULL;
		}

		return m_pCurPool->pPool + m_pCurNum;
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除旧元素
	template <typename T> void MemPool<T>::Erase( T* pData )
	{
		//判断元素所在位置
		PoolInfo* pPoolInfo = m_pPoolInfo;
		for( unsigned int i=0; i<m_nNumPool; ++i )
		{
			int pos = (int)( pData - pPoolInfo->pPool );
			//如果在该内存池内
			if( pos >= 0 && pos < (int)m_nPoolSize )
			{
				//标志此位置为空
				pPoolInfo->pbIsUsed[pos] = false;
				pPoolInfo->nNumEmpty += 1;
				--m_nNumData;
				return;
			}

			++pPoolInfo;
		}

		Misc::ShowErrMsg( "指定要删除的元素部在内存池（%s）中。", __FUNCSIG__ );
	}
	//--------------------------------------------------------------------------------------------------------------
	//分配一个新的内存池
	template <typename T> void MemPool<T>::AllocNewPool()
	{
		//扩充内存池信息数组
		unsigned int oldNumPool = m_nNumPool++;
		PoolInfo* pOldPoolInfo = m_pPoolInfo;

		m_pPoolInfo = (PoolInfo*)malloc( sizeof(PoolInfo)*m_nNumPool );
		if( m_pPoolInfo == NULL )
		{
			Misc::ShowErrMsg( "初始化内存池（%s）失败。", __FUNCSIG__ );
			return;
		}
		memcpy( m_pPoolInfo, pOldPoolInfo, sizeof(PoolInfo)*oldNumPool );
		free( pOldPoolInfo );

		//分配内存池数据内存
		PoolInfo* pNewPool = m_pPoolInfo + oldNumPool;
		pNewPool->pPool = (T*)malloc( m_nPoolDataLen );
		if( pNewPool->pPool == NULL )
		{
			Misc::ShowErrMsg( "初始化内存池（%s）失败。", __FUNCSIG__ );
			return;
		}
		pNewPool->pbIsUsed = (bool*)( pNewPool->pPool + m_nPoolSize );
		memset( pNewPool->pPool, 0, m_nPoolDataLen );

		pNewPool->nNumEmpty = m_nPoolSize;

		//增加最大元素数量
		m_nMaxNumData += m_nPoolSize;

		//记录当前空闲元素位置
		m_pCurPool = pNewPool;
		m_pCurNum = 0;
	}
	//--------------------------------------------------------------------------------------------------------------
	//返回指定位置的元素迭代器
	template <typename T> typename MemPool<T>::Iterator& MemPool<T>::GetIterator( unsigned int nPos )
	{
		//设置迭代器位置
		m_TmpIterator.pMemPool = this;
		m_TmpIterator.nPoolNum = nPos / m_nPoolSize;
		m_TmpIterator.nDataNum = nPos % m_nPoolSize;
		m_TmpIterator.pPoolInfo = m_pPoolInfo + m_TmpIterator.nPoolNum;
		m_TmpIterator.pData = m_TmpIterator.pPoolInfo->pPool + m_TmpIterator.nDataNum;

		return m_TmpIterator;
	}
	//--------------------------------------------------------------------------------------------------------------
	//返回指定位置的可用元素迭代器
	template <typename T> typename MemPool<T>::DataIterator& MemPool<T>::GetDataIterator( unsigned int nPos )
	{
		m_TmpDataIterator.pMemPool = this;

		//循环所有内存池
		unsigned int NumValidData = 0;
		unsigned int NumUsedData = 0;

		PoolInfo* pPoolInfo = m_pPoolInfo;
		for( unsigned int i=0; i<m_nNumPool; ++i )
		{
			//计算当前内存池的可用元素数
			NumUsedData = m_nPoolSize - pPoolInfo->nNumEmpty;
			NumValidData += NumUsedData;

			//如果指定的可用元素就在该内存池中
			if( NumValidData > nPos )
			{
				m_TmpDataIterator.nPoolNum = i;
				m_TmpDataIterator.pPoolInfo = pPoolInfo;
				goto FindPos;
			}

			++pPoolInfo;
		}

		goto Error;

FindPos:
		//计算指定元素在该内存池内中可用元素中的位置
		unsigned int DataNumInThisPool = nPos - ( NumValidData - NumUsedData );

		//遍历该内存池中所有可用元素
		unsigned int ValidDataCount = 0;
		bool* pbIsUsed = pPoolInfo->pbIsUsed;
		for( unsigned int i=0; i<m_nPoolSize; ++i )
		{
			//如果为可用元素
			if( *pbIsUsed )
			{
				//如果就是该元素
				if( DataNumInThisPool == ValidDataCount )
				{
					m_TmpDataIterator.nDataNum = i;
					m_TmpDataIterator.pData = pPoolInfo->pPool + i;
					return m_TmpDataIterator;
				}

				++ValidDataCount;
			}
		}

Error:
		Misc::ShowErrMsg( "指定要获取内存池（%s）可用元素迭代器的元素不可用。", __FUNCSIG__ );
		return m_TmpDataIterator;
	}
	//--------------------------------------------------------------------------------------------------------------
	//返回超前基本元素迭代器
	template <typename T> typename MemPool<T>::BaseIterator& MemPool<T>::Front()
	{
		m_TmpIterator.pMemPool = this;
		m_TmpIterator.nPoolNum = 0;
		m_TmpIterator.pPoolInfo = m_pPoolInfo;
		m_TmpIterator.nDataNum = 0;
		m_TmpIterator.pData = m_pPoolInfo->pPool - 1;
	}
	//--------------------------------------------------------------------------------------------------------------
	//返回超尾基本元素迭代器
	template <typename T> typename MemPool<T>::BaseIterator& MemPool<T>::End()
	{
		m_TmpDataIterator.pMemPool = this;
		m_TmpDataIterator.nPoolNum = m_nNumPool - 1;
		m_TmpDataIterator.pPoolInfo = m_TmpIterator.nPoolNum;
		m_TmpDataIterator.nDataNum = m_nPoolSize;
		m_TmpDataIterator.pData = m_pPoolInfo->pPool + m_nPoolSize;
	}
	//--------------------------------------------------------------------------------------------------------------


	//--------------------------------------------------------------------------------------------------------------
	//内存池基本元素迭代器成员函数
	//--------------------------------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------------------------------
	//赋值操作符
	template <typename T> FKInline typename MemPool<T>::BaseIterator&
		MemPool<T>::BaseIterator::operator = ( const BaseIterator& rBaseIterator )
	{
		pMemPool = rBaseIterator.pMemPool;
		nDataNum = rBaseIterator.nDataNum;
		nPoolNum = rBaseIterator.nPoolNum;
		pPoolInfo = rBaseIterator.pPoolInfo;
		pData = rBaseIterator.pData;

		return *this;
	}
	//--------------------------------------------------------------------------------------------------------------
	//相等比较操作符
	template <typename T> FKInline const bool
		MemPool<T>::BaseIterator::operator == ( const BaseIterator& rBaseIterator )
	{
		return ( pData == rBaseIterator.pData );
	}
	//--------------------------------------------------------------------------------------------------------------
	//不相等比较操作符
	template <typename T> FKInline const bool
		MemPool<T>::BaseIterator::operator != ( const BaseIterator& rBaseIterator )
	{
		return ( pData != rBaseIterator.pData );
	}
	//--------------------------------------------------------------------------------------------------------------


	//--------------------------------------------------------------------------------------------------------------
	//内存池元素内存迭代器成员函数
	//--------------------------------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------------------------------
	//向前移动迭代器（先计算）
	template <typename T> typename MemPool<T>::Iterator& MemPool<T>::Iterator::operator ++ ()
	{
		//如果当前迭代元素不是当前内存池中最后一个元素
		if( nDataNum < pMemPool->m_nPoolSize )
		{
			++pData;
			++nDataNum;
		}
		//如果当前迭代元素已经为当前内存池中最后一个元素
		else
		{
			//如果当前内存池为最后一个内存池
			if( nPoolNum == pMemPool->m_nNumPool - 1 )
			{
				//返回超尾元素
				return pMemPool->EndIterator();
			}

			//跳转到下一个内存池中
			++pPoolInfo;
			++nPoolNum;
			pData = pPoolInfo->pPool;
			nDataNum = 0;
		}

		return *this;
	}
	//--------------------------------------------------------------------------------------------------------------
	//向前移动迭代器（后计算）
	template <typename T> typename MemPool<T>::Iterator MemPool<T>::Iterator::operator ++ (int)
	{
		Iterator tmpIterator = *this;
		++(*this);
		return tmpIterator;
	}
	//--------------------------------------------------------------------------------------------------------------
	//向后移动迭代器（先计算）
	template <typename T> typename MemPool<T>::Iterator& MemPool<T>::Iterator::operator -- ()
	{
		//如果当前迭代元素已经为当前内存池中第一个元素
		if( nDataNum == 0 )
		{
			//如果当前内存池为第一个内存池
			if( nPoolNum == 0 )
			{
				//返回超前元素
				return pMemPool->FrontIterator();
			}

			//跳转到上一个内存池的尾部
			--pPoolInfo;
			--nPoolNum;
			nDataNum = pMemPool->m_nPoolSize - 1;
			pData = pPoolInfo->pPool + nDataNum;
		}
		else
		{
			--pData;
			--nDataNum;
		}

		return *this;
	}
	//--------------------------------------------------------------------------------------------------------------
	//向后移动迭代器（后计算）
	template <typename T> typename MemPool<T>::Iterator MemPool<T>::Iterator::operator -- (int)
	{
		Iterator tmpIterator = *this;
		--(*this);
		return tmpIterator;
	}
	//--------------------------------------------------------------------------------------------------------------


	//--------------------------------------------------------------------------------------------------------------
	//内存池可用元素迭代器成员函数
	//--------------------------------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------------------------------
	//向后移动迭代器（先计算）
	template <typename T> typename MemPool<T>::DataIterator& MemPool<T>::DataIterator::operator ++ ()
	{
		//如果当前迭代元素不是当前内存池中最后一个元素
		if( nDataNum < pMemPool->m_nPoolSize )
		{
			++pData;
			++pbIsUsed;
			++nDataNum;

FindAfterData:
			//遍历该内存池中之后所有元素
			bool* pbIsUsed = pPoolInfo->pbIsUsed + nDataNum;
			for( unsigned int i=nDataNum; i<pMemPool->m_nNumPool; ++i )
			{
				//如果当前元素不是空元素
				if( *pbIsUsed )
					return *this;

				++pData;
				++pbIsUsed;
				++nDataNum;
			}

			//如果当前元素之后没有任何可用元素则跳转到下一个内存池
			goto NextPool;
		}
		//如果当前迭代元素已经为当前内存池中最后一个元素
		else
		{
NextPool:
			//如果当前内存池为最后一个内存池
			if( nPoolNum == pMemPool->m_nNumPool - 1 )
			{
				//返回超尾元素
				return pMemPool->EndDataIterator();
			}

			//跳转到下一个内存池的起始处
			++pPoolInfo;
			++nPoolNum;
			pData = pPoolInfo->pPool;
			nDataNum = 0;

			goto FindAfterData;
		}

		return *this;
	}
	//--------------------------------------------------------------------------------------------------------------
	//向后移动迭代器（后计算）
	template <typename T> typename MemPool<T>::DataIterator MemPool<T>::DataIterator::operator ++ (int)
	{
		DataIterator tmpDataIterator = *this;
		++(*this);
		return tmpDataIterator;
	}
	//--------------------------------------------------------------------------------------------------------------
	//向前移动迭代器（先计算）
	template <typename T> typename MemPool<T>::DataIterator& MemPool<T>::DataIterator::operator -- ()
	{
		//如果当前迭代元素已经为当前内存池中第一个元素
		if( nDataNum == 0 )
		{
PrevPool:
			//如果当前内存池为第一个内存池
			if( nPoolNum == 0 )
			{
				//返回超前元素
				return pMemPool->FrontDataIterator();
			}

			//跳转到上一个内存池的尾部
			--pPoolInfo;
			--nPoolNum;
			nDataNum = pMemPool->m_nPoolSize - 1;
			pData = pPoolInfo->pPool + nDataNum;

			goto FindBeforeData;
		}
		else
		{
			--pData;
			--pbIsUsed;
			--nDataNum;

FindBeforeData:
			//遍历该内存池中之前所有元素
			bool* pbIsUsed = pPoolInfo->pbIsUsed + nDataNum;
			for( unsigned int i=0; i<nDataNum; ++i )
			{
				//如果当前元素不是空元素
				if( *pbIsUsed )
					return *this;

				--pData;
				--pbIsUsed;
				--nDataNum;
			}

			//如果当前元素之前没有任何可用元素则跳转到上一个内存池
			goto PrevPool;
		}

		return *this;
	}
	//--------------------------------------------------------------------------------------------------------------
	//向前移动迭代器（后计算）
	template <typename T> typename MemPool<T>::DataIterator MemPool<T>::DataIterator::operator -- (int)
	{
		DataIterator tmpDataIterator = *this;
		++(*this);
		return tmpDataIterator;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================


#endif	/* #ifndef FK_NO_TEMPLATE_IMPLEMNTATION */

