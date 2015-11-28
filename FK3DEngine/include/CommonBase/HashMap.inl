/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	HashMap
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
	template <typename T> HashMap<T>::HashMap()
		: mnTableSize	(0)
		, mpHashTable	(NULL)
		, mNumData		(0)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	template <typename T> HashMap<T>::~HashMap()
	{
		Release();
	}
	//--------------------------------------------------------------------------------------------------------------
	//初始化哈希表（nTableSize 为哈希表大小，如果添加的元素超过了此数值将造成重复率极高。）
	template <typename T> void HashMap<T>::Initialize( unsigned int nTableSize )
	{
		//初始化堆栈
		mHashDataStack.Initialize( nTableSize, nTableSize >> 2 );

		//初始化哈希元素索引表内存
		mnTableSize = nTableSize;

		size_t nTableLen = sizeof(HashData*) * nTableSize;
		mpHashTable = (HashData**)malloc( nTableLen );
		if( mpHashTable == NULL )
		{
			Misc::ShowErrMsg( "初始化哈希表（%s）内存失败。", __FUNCSIG__ );
			return;
		}

		memset( mpHashTable, 0, nTableLen );
	}
	//--------------------------------------------------------------------------------------------------------------
	//清空哈希表
	template <typename T> void HashMap<T>::Clear()
	{
		//释放所有哈希元素
		unsigned int nStackSize = mHashDataStack.Size();
		HashData** ppHashData = mHashDataStack.Begin();
		for( unsigned int i=0; i<nStackSize; ++i )
		{
			SafeFree( *ppHashData );
			++ppHashData;
		}

		mHashDataStack.Clear();
		memset( mpHashTable, 0, sizeof(HashData*) * mnTableSize );
	}
	//--------------------------------------------------------------------------------------------------------------
	//释放哈希表
	template <typename T> void HashMap<T>::Release()
	{
		//释放所有哈希元素
		unsigned int nStackSize = mHashDataStack.Size();
		HashData** ppHashData = mHashDataStack.Begin();
		for( unsigned int i=0; i<nStackSize; ++i )
		{
			if( *ppHashData != NULL )
				free( *ppHashData );

			ppHashData++;
		}

		mHashDataStack.Release();
		SafeFree( mpHashTable );
	}
	//--------------------------------------------------------------------------------------------------------------
	//加入元素
	template <typename T> bool HashMap<T>::Add( const T& refData, const char* szKeyName )
	{
		//计算键名哈希值作为该元素在索引表中的位置
		size_t nStrLen = 0;
		unsigned int nPos = Misc::HashString( szKeyName, &nStrLen ) % mnTableSize;

		//检查键名字符串长度
		if( nStrLen >= 32 )
		{
			Misc::ShowErrMsg( "哈希表（%s）键名不允许超过 %d 个字符。", __FUNCSIG__, mMaxHashStrLen );
			return false;
		}

		//创建新的哈希元素
		HashData* pHashData = (HashData*)malloc( sizeof(HashData) );
		if( pHashData == NULL )
		{
			Misc::ShowErrMsg( "分配哈希表（%s）内存失败。", __FUNCSIG__ );
			return false;
		}
		
		//复制元素数据
		pHashData->nKeyNameLen = (unsigned short)nStrLen;
		memcpy( pHashData->szKeyName, szKeyName, nStrLen + 1 );

		pHashData->Data = refData;
		pHashData->pPrev = NULL;
		pHashData->pNext = NULL;
		pHashData->nStackIndex = mHashDataStack.Size();

		//如果该位置已经存在哈希元素
		if( mpHashTable[nPos] != NULL )
		{
			HashData* pExistData = mpHashTable[nPos];

CheckNextKey:
			//如果键名长度相同
			if( pExistData->nKeyNameLen == (unsigned short)nStrLen )
			{
				//且键名相同
				if( 0 == memcmp( pExistData->szKeyName, szKeyName, nStrLen ) )
				{
					return false;
				}
			}
			
			if( pExistData->pNext != NULL )
			{
				pExistData = pExistData->pNext;
				goto CheckNextKey;
			}

			pExistData->pNext = pHashData;
			pHashData->pPrev = pExistData;
		}
		//如果该位置为空
		else
		{
			mpHashTable[nPos] = pHashData;
		}

		//将此元素指针加入堆栈索引表
		*mHashDataStack.Push() = pHashData;

		++mNumData;

		return true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除元素
	template <typename T> void HashMap<T>::Del( const char* szKeyName )
	{
		//计算键名哈希值作为该元素在索引表中的位置
		size_t nStrLen = 0;
		unsigned int nPos = Misc::HashString( szKeyName, &nStrLen ) % mnTableSize;

		//检查键名字符串长度
		if( nStrLen >= 32 )
		{
			Misc::ShowErrMsg( "哈希表（%s）键名不允许超过 %d 个字符。", __FUNCSIG__, mMaxHashStrLen );
			return;
		}

		//如果在哈希表中找不到该元素
		if( mpHashTable[nPos] == NULL )
		{
			Misc::ShowErrMsg( "在哈希表（%s）中找不到键名为 '%s' 的元素。", __FUNCSIG__, szKeyName );
			return;
		}

		//如果找到
		HashData* pHashData = mpHashTable[nPos];

		//如果该位置不只有一个元素
		if( pHashData->pNext != NULL )
		{
CheckNextKey:
			//如果键名长度相同
			if( pHashData->nKeyNameLen == (unsigned short)nStrLen )
			{
				//且键名相同
				if( 0 == memcmp( pHashData->szKeyName, szKeyName, nStrLen ) )
					goto FoundKey;
			}

			//如果该位置没有其它元素了
			if( pHashData->pNext == NULL )
			{
				Misc::ShowErrMsg( "在哈希表（%s）中找不到键名为 '%s' 的元素。", __FUNCSIG__, szKeyName );
				return;
			}
			//否则继续查询下一个键
			else
			{
				pHashData = pHashData->pNext;
				goto CheckNextKey;
			}
		}

FoundKey:
		//如果该元素之前存在其它元素
		if( pHashData->pPrev != NULL )
		{
			HashData* pParent = pHashData->pPrev;

			//如果在其之后也有元素则将它的前一个元素和它的后一个元素连接起来
			if( pHashData->pNext != NULL )
			{
				pParent->pNext = pHashData->pNext;
				pHashData->pNext->pPrev = pParent;
			}
			//否则只清空它的前面的元素指向该元素的指针
			else
			{
				pParent->pNext = NULL;
			}
		}
		//如果该元素之前没有其它元素
		else
		{
			//如果在其之后有元素则将之后的元素作为该位置第一个元素
			if( pHashData->pNext != NULL )
			{
				mpHashTable[nPos] = pHashData->pNext;
				mpHashTable[nPos]->pPrev = NULL;
			}
			//如果在其之后没有元素则将该位置清空
			else
				mpHashTable[nPos] = NULL;
		}

		//清空该哈希元素在索引堆栈中的指针
		*( mHashDataStack.Begin() + pHashData->nStackIndex ) = NULL;

		//释放该哈希元素内存
		free( pHashData );

		--mNumData;
	}
	//--------------------------------------------------------------------------------------------------------------
	//寻找元素
	template <typename T> T* HashMap<T>::Find( const char* szKeyName )
	{
		//计算键名哈希值作为该元素在索引表中的位置
		size_t nStrLen = 0;
		int nPos = Misc::HashString( szKeyName, &nStrLen ) % mnTableSize;

		//检查键名字符串长度
		if( nStrLen >= 32 )
		{
			Misc::ShowErrMsg( "哈希表（%s）键名不允许超过 %d 个字符。", __FUNCSIG__, mMaxHashStrLen );
			return NULL;
		}

		//如果在哈希表中找不到该元素
		if( mpHashTable[nPos] == NULL )
		{
			Misc::ShowErrMsg( "在哈希表（%s）中找不到键名为 '%s' 的元素。", __FUNCSIG__, szKeyName );
			return NULL;
		}

		//如果找到
		HashData* pHashData = mpHashTable[nPos];

		//如果该位置不只有一个元素
		if( pHashData->pNext != NULL )
		{
CheckNextKey:
			//如果键名长度相同
			if( pHashData->nKeyNameLen == (unsigned short)nStrLen )
			{
				//且键名相同
				if( 0 == memcmp( pHashData->szKeyName, szKeyName, nStrLen ) )
				{
					goto FoundKey;
				}
			}

			//如果该位置没有其它元素了
			if( pHashData->pNext == NULL )
			{
				Misc::ShowErrMsg( "在哈希表（%s）中找不到键名为 '%s' 的元素。", __FUNCSIG__, szKeyName );
				return NULL;
			}
			//否则继续查询下一个键
			else
			{
				pHashData = pHashData->pNext;
				goto CheckNextKey;
			}
		}

FoundKey:
		return &pHashData->Data;
	}
	//--------------------------------------------------------------------------------------------------------------
	//返回当前元素数
	template <typename T> FKInline unsigned int HashMap<T>::Size() const
	{
		return mNumData;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================


#endif	/* #ifndef FK_NO_TEMPLATE_IMPLEMNTATION */

