/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Stack
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
	template <typename T> Stack<T>::Stack()
		: m_pStackBegin	(NULL)
		, m_pStackTop	(NULL)
		, m_pStackEnd	(NULL)
		, m_nStackSize	(0)
		, m_nAddSize	(0)
		, m_nNumData	(0)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	template <typename T> Stack<T>::~Stack()
	{
		Release();
	}
	//--------------------------------------------------------------------------------------------------------------
	//初始化堆栈内存（InitNum 为初始分配的元素数量，AddNum 为每次扩大的元素数量。）
	template <typename T> void Stack<T>::Initialize( unsigned int InitNum, unsigned int AddNum )
	{
		if( m_pStackBegin != NULL )
		{
			Misc::ShowErrMsg( "不允许重复初始化堆栈（%s）。", __FUNCSIG__ );
			return;
		}

		if( InitNum == 0 )
		{
			Misc::ShowErrMsg( "无法初始化堆栈（%s）为零元素。", __FUNCSIG__ );
			return;
		}

		m_nNumData = 0;

		//分配初始内存
		m_nStackSize = InitNum * sizeof(T);
		m_pStackBegin = (T*)malloc( m_nStackSize );
		if( m_pStackBegin == NULL )
		{
			Misc::ShowErrMsg( "初始化堆栈（%s）失败。", __FUNCSIG__ );
			return;
		}

		//记录每次增加的内存容量
		m_nAddSize = AddNum * sizeof(T);

		m_pStackTop = m_pStackBegin;
		m_pStackEnd = (T*)( (unsigned char*)m_pStackBegin + m_nStackSize );
	}
	//--------------------------------------------------------------------------------------------------------------
	//释放堆栈内存
	template <typename T> void Stack<T>::Release()
	{
		if( m_pStackBegin != NULL )
		{
			free( m_pStackBegin );
			m_pStackBegin = NULL;
			m_nNumData = m_nStackSize = 0;
			m_pStackTop = m_pStackEnd = NULL;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//重新设置内存扩容时扩充大小
	template <typename T> FKInline void Stack<T>::ResetAddNum( unsigned int AddNum )
	{
		m_nAddSize = AddNum * sizeof(T);
	}
	//--------------------------------------------------------------------------------------------------------------
	//清空所有元素
	template <typename T> void Stack<T>::Clear()
	{
		m_nNumData = 0;
		m_pStackTop = m_pStackBegin;
	}
	//--------------------------------------------------------------------------------------------------------------
	//检测堆栈是否为空
	template <typename T> bool Stack<T>::IsEmpty() const
	{
		if( m_pStackTop == m_pStackBegin )
			return true;
		return false;
	}
	//--------------------------------------------------------------------------------------------------------------
	//返回当前元素数
	template <typename T> FKInline unsigned int Stack<T>::Size() const
	{
		return m_nNumData;
	}
	//--------------------------------------------------------------------------------------------------------------
	//改变元素数量（如果内存不足将自动扩充）
	template <typename T> void Stack<T>::Resize( unsigned int size )
	{
		//计算需要内存空间
		unsigned int nNewSize = sizeof(T) * size;

		//如果内存不足则进行扩充
		if( m_nStackSize < nNewSize )
		{
			T* pOldStackBegin = m_pStackBegin;
			unsigned int OldDataSize = (unsigned int)( (unsigned char*)m_pStackTop - (unsigned char*)m_pStackBegin );
			m_nStackSize = nNewSize;

			//分配新的内存块
			m_pStackBegin = (T*)malloc( m_nStackSize );
			if( m_pStackBegin == NULL )
			{
				Misc::ShowErrMsg( "扩充堆栈模板类（%s）失败！", __FUNCSIG__ );
				return;
			}

			memset( m_pStackBegin, 0, m_nStackSize );
			memcpy( m_pStackBegin, pOldStackBegin, OldDataSize );

			//释放原有内存块
			free( pOldStackBegin );

			m_pStackEnd = (T*)( (unsigned char*)m_pStackBegin + m_nStackSize );
		}

		m_pStackTop = (T*)( (unsigned char*)m_pStackBegin + nNewSize );
		m_nNumData = size;
	}
	//--------------------------------------------------------------------------------------------------------------
	//推入新元素并返回其指针
	template <typename T> T* Stack<T>::Push()
	{
		//如果尚未初始化则报错
#ifdef _DEBUG
		if( m_pStackBegin == NULL )
		{
			Misc::ShowErrMsg( "无法在初始化堆栈（%s）前调用 Push() 函数。", __FUNCSIG__ );
			return NULL;
		}
#endif

		//如果堆栈已分配内存不足则扩充它
		if( m_pStackTop == m_pStackEnd )
		{
			if( !_Expand() )
				return NULL;
		}

		T* pData = m_pStackTop;
		++m_pStackTop;
		++m_nNumData;

		return pData;
	}
	//--------------------------------------------------------------------------------------------------------------
	//推入指定数量的新元素并返回第一个元素的指针
	template <typename T> T* Stack<T>::Push( unsigned int num )
	{
		//如果尚未初始化则报错
#ifdef _DEBUG
		if( m_pStackBegin == NULL )
		{
			Misc::ShowErrMsg( "无法在初始化堆栈（%s）前调用 Push() 函数。", __FUNCSIG__ );
			return NULL;
		}
#endif
		//重新设置堆栈元素数量
		m_nNumData += num;

		//计算需要内存空间
		unsigned int nNewSize = sizeof(T) * m_nNumData;

		T* pFirstPushData = m_pStackTop;

		//如果内存不足则进行扩充
		if( m_nStackSize < nNewSize )
		{
			T* pOldStackBegin = m_pStackBegin;
			unsigned int OldDataSize = (unsigned int)( (unsigned char*)m_pStackTop - (unsigned char*)m_pStackBegin );
			m_nStackSize = nNewSize;

			//分配新的内存块
			m_pStackBegin = (T*)malloc( m_nStackSize );
			if( m_pStackBegin == NULL )
			{
				Misc::ShowErrMsg( "扩充堆栈模板类（%s）失败！", __FUNCSIG__ );
				return NULL;
			}

			memset( m_pStackBegin, 0, m_nStackSize );
			memcpy( m_pStackBegin, pOldStackBegin, OldDataSize );

			//释放原有内存块
			free( pOldStackBegin );

			m_pStackEnd = (T*)( (unsigned char*)m_pStackBegin + m_nStackSize );
			pFirstPushData = (T*)( (unsigned char*)m_pStackBegin + OldDataSize );
		}

		m_pStackTop = (T*)( (unsigned char*)m_pStackBegin + nNewSize );

		return pFirstPushData;
	}
	//--------------------------------------------------------------------------------------------------------------
	//弹出栈顶元素并返回其指针
	template <typename T> T* Stack<T>::Pop()
	{
		if( m_pStackTop == m_pStackBegin )
			return NULL;

		--m_nNumData;
		--m_pStackTop;

		return m_pStackTop;
	}
	//--------------------------------------------------------------------------------------------------------------
	//弹出指定数量的栈顶元素并返回其指针
	template <typename T> T* Stack<T>::Pop( unsigned int num )
	{
		if( m_pStackTop == m_pStackBegin )
			return NULL;

		m_pStackTop -= num;
		m_nNumData -= num;

		return m_pStackTop;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取栈底的第一个元素的指针
	template <typename T> FKInline T* Stack<T>::Begin() const
	{
		return m_pStackBegin;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取栈顶的最后一个元素之后的超尾指针
	template <typename T> FKInline T* Stack<T>::End() const
	{
		return m_pStackTop;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取栈顶的最后一个元素的指针
	template <typename T> FKInline T* Stack<T>::Back() const
	{
		return m_pStackTop - 1;
	}
	//--------------------------------------------------------------------------------------------------------------
	//在指定元素之前插入一个元素并将之后的元素后移（0 为第一个元素）
	template <typename T> T* Stack<T>::Insert( unsigned int index )
	{
		//如果堆栈已分配内存不足则扩充它
		if( m_pStackTop == m_pStackEnd )
		{
			if( !_Expand() )
				return NULL;
		}

		//将之后的元素后移
		T* pInsPos = m_pStackBegin + index;
		memmove( pInsPos + 1, pInsPos, sizeof(T)*( m_nNumData - index ) );

		++m_pStackTop;
		++m_nNumData;

		return pInsPos;
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除指定位置的元素并将之后的元素前移（使用迭代器代表元素）
	template <typename T> T* Stack<T>::Insert( T* ite )
	{
		return Insert( (UINT)( ite - m_pStackBegin ) );
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除指定位置的元素并将之前的元素前移（0 为第一个元素）
	template <typename T> void Stack<T>::Erase( unsigned int index, unsigned int num )
	{
		//如果堆栈为空则返回
		if( m_pStackTop == m_pStackBegin )
			return;

		//如果删除元素之后不存在任何元素则不将之后元素前移
		unsigned int MoveNum = m_nNumData - index - num;
		if( MoveNum > 0 )
			memcpy( m_pStackBegin + index, m_pStackBegin + index + num, sizeof(T)*MoveNum );

		m_nNumData -= num;
		m_pStackTop -= num;
	}
	//--------------------------------------------------------------------------------------------------------------
	template <typename T> void Stack<T>::Erase( T* ite )
	{
		Erase( (UINT)( ite - m_pStackBegin ) );
	}
	//--------------------------------------------------------------------------------------------------------------
	//弹出栈底元素
	template <typename T> void Stack<T>::PopFront( unsigned int num )
	{
		//如果堆栈为空则返回
		if( m_pStackTop == m_pStackBegin )
			return;

		//如果删除元素数量大于当前数量则返回
		if( num > m_nNumData )
		{
			Misc::ShowErrMsg( "指定弹出的堆栈模板类（%s）元素数量超过当前元素数量！", __FUNCSIG__ );
			return;
		}

		//将之后元素前移
		memcpy( m_pStackBegin, m_pStackBegin + num, sizeof(T) * ( m_nNumData - num ) );

		m_nNumData -= num;
		m_pStackTop -= num;
	}
	//--------------------------------------------------------------------------------------------------------------
	//重载索引访问操作符“[]”使用时注意不要越界
	template <typename T> FKInline T& Stack<T>::operator [] ( int index )
	{
		return *(m_pStackBegin + index);
	}
	//--------------------------------------------------------------------------------------------------------------
	//重载复制操作符（仅复制内容，内存扩容大小不变。）
	template <typename T> void Stack<T>::operator = ( const Stack<T>& refStack )
	{
		//如果被赋值内存空间不足则重新分配它
		unsigned int copySize = (unsigned int)( refStack.m_pStackTop - refStack.m_pStackBegin );
		if( m_nStackSize < copySize )
		{
			free( m_pStackBegin );
			m_pStackBegin = (T*)malloc( copySize );
			if( m_pStackBegin == NULL )
			{
				Misc::ShowErrMsg( "扩充堆栈模板类（%s）失败！", __FUNCSIG__ );
				m_nStackSize = 0;
				m_nNumData = 0;
				return;
			}

			m_nStackSize = copySize;
			m_pStackEnd = m_pStackTop = (T*)( (unsigned char*)m_pStackBegin + copySize );
		}
		else
		{
			m_pStackTop = (T*)( (unsigned char*)m_pStackBegin + copySize );
		}

		m_nNumData = refStack.m_nNumData;

		memcpy( m_pStackBegin, refStack.m_pStackBegin, copySize );
	}
	//--------------------------------------------------------------------------------------------------------------
	//扩充内存空间
	template <typename T> bool Stack<T>::_Expand()
	{
		T* pOldStackBegin = m_pStackBegin;
		unsigned int OldStackSize = m_nStackSize;
		m_nStackSize += m_nAddSize;

		//分配新的内存块
		m_pStackBegin = (T*)malloc( m_nStackSize );
		if( m_pStackBegin == NULL )
		{
			Misc::ShowErrMsg( "扩充堆栈模板类（%s）失败！", __FUNCSIG__ );
			return false;
		}

		memset( m_pStackBegin, 0, m_nStackSize );
		memcpy( m_pStackBegin, pOldStackBegin, OldStackSize );

		//释放原有内存块
		free( pOldStackBegin );

		m_pStackTop = (T*)( (unsigned char*)m_pStackBegin + OldStackSize );
		m_pStackEnd = (T*)( (unsigned char*)m_pStackBegin + m_nStackSize );

		return true;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================


#endif	/* #ifndef FK_NO_TEMPLATE_IMPLEMNTATION */

