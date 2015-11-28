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
*	文件说明：	堆栈模板类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================
#include "Macros.h"
//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//指针堆栈类（此类可保证内存的线性分配，但内存地址有可能改变。）
	//
	//注意：
	//		此类对堆栈的元素存取操作都是通过指针完成的。例如，要推入一个元素则应该写： *Stack.Push() = Value;
	template <typename T> class FK_DLL Stack
	{
	public:
		typedef T*		Iterator;		//T 类型

	public:
		T*				m_pStackBegin;	//栈底指针
		T*				m_pStackTop;	//栈顶指针
		T*				m_pStackEnd;	//栈尾指针
		unsigned int	m_nStackSize;	//堆栈当前大小
		unsigned int	m_nAddSize;		//每次增加的内存容量
		unsigned int	m_nNumData;		//当前栈内元素数

	public:
		Stack();
		~Stack();

		//初始化堆栈内存（InitNum 为初始分配的元素数量，AddNum 为每次扩大的元素数量。）
		void Initialize( unsigned int InitNum, unsigned int AddNum );

		//释放堆栈内存
		void Release();

		//重新设置内存扩容时扩充大小
		FKInline void ResetAddNum( unsigned int AddNum );

		//清空所有元素
		void Clear();

		//检测堆栈是否为空
		FKInline bool IsEmpty() const;

		//返回当前元素数
		FKInline unsigned int Size() const;

		//改变元素数量（如果内存不足将自动扩充）
		void Resize( unsigned int size );

		//推入新元素并返回其指针
		T* Push();

		//推入指定数量的新元素并返回第一个元素的指针
		T* Push( unsigned int num );

		//弹出栈顶元素并返回其指针
		T* Pop();

		//弹出指定数量的栈顶元素并返回其指针
		T* Pop( unsigned int num );

		//获取栈底的第一个元素的指针
		FKInline T* Begin() const;

		//获取栈顶的最后一个元素之后的超尾指针
		FKInline T* End() const;

		//获取栈顶的最后一个元素的指针
		FKInline T* Back() const;

		//在指定元素之前插入一个元素并将之后的元素后移（0 为第一个元素）
		T* Insert( unsigned int index );

		//在指定元素之前插入一个元素并将之后的元素后移（使用迭代器代表元素）
		T* Insert( T* ite );

		//删除指定位置的元素并将之后的元素前移（0 为第一个元素）
		void Erase( unsigned int index, unsigned int num = 1 );
		
		//删除指定位置的元素并将之后的元素前移（使用迭代器代表元素）
		void Erase( T* ite );

		//弹出栈底元素
		void PopFront( unsigned int num = 1 );

		//重载索引访问操作符“[]”使用时注意不要越界
		FKInline T& operator [] ( int index );

		//重载复制操作符（仅复制内容，内存扩容大小不变。）
		void operator = ( const Stack<T>& refStack );

	protected:
		//扩充内存空间
		bool _Expand();
	};
	//--------------------------------------------------------------------------------------------------------------

	//基本堆栈类型声明
	typedef Stack< void* >				PTRStack;		//无类型指针堆栈

	typedef Stack< bool >				BOOLStack;		//布尔型堆栈
	typedef Stack< wchar_t >			WCHARStack;		//宽字符型堆栈

	typedef Stack< char >				CHARStack;		//有符号字符型堆栈
	typedef Stack< short >				SHORTStack;		//有符号短整型堆栈
	typedef Stack< int >				INTStack;		//有符号整型堆栈
	typedef Stack< long >				LONGStack;		//有符号长整型堆栈

	typedef Stack< unsigned char >		BYTEStack;		//无符号字符型堆栈
	typedef Stack< unsigned short >		WORDStack;		//无符号短整型堆栈
	typedef Stack< unsigned int >		UINTStack;		//无符号整型堆栈
	typedef Stack< unsigned long >		ULONGStack;		//无符号长整型堆栈

	typedef Stack< float >				FLOATStack;		//单精度浮点型堆栈
	typedef Stack< float >				DOUBLEStack;	//双精度浮点型堆栈

	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
