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
*	文件说明：	内存池模板类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//内存池（此类可保证内存的地址固定，但不能保证内存线性分配。）
	template <typename T> class FK_DLL MemPool
	{
	public:
		//内存池信息结构
		struct PoolInfo
		{
			T*				pPool;		//内存池空间
			bool*			pbIsUsed;	//元素空间占用标志
			unsigned int	nNumEmpty;	//该内存池中空闲元素数量
		};

	public:
		//----------------------------------------------------------------------------------------------------------
		//内存池基本元素迭代器
		class BaseIterator
		{
		public:
			friend class MemPool;

		protected:
			MemPool*		pMemPool;	//迭代器所属的内存池
			PoolInfo*		pPoolInfo;	//该迭代器所在内存池
			unsigned int	nPoolNum;	//该迭代器所在内存池编号
			unsigned int	nDataNum;	//迭代器数据编号

		public:
			T*				pData;		//迭代器数据

		public:
			//赋值操作符
			FKInline BaseIterator& operator = ( const BaseIterator& rBaseIterator );

			//相等比较操作符
			FKInline const bool operator == ( const BaseIterator& rBaseIterator );

			//不相等比较操作符
			FKInline const bool operator != ( const BaseIterator& rBaseIterator );
		};
		//----------------------------------------------------------------------------------------------------------
		//内存池元素内存迭代器（此迭代器将访问池中所有元素，不论该元素是否为空。）
		class Iterator : public BaseIterator
		{
		public:
			//向后移动迭代器（先计算）
			Iterator& operator ++ ();

			//向后移动迭代器（后计算）
			Iterator operator ++ (int);

			//向前移动迭代器（先计算）
			Iterator& operator -- ();

			//向前移动迭代器（后计算）
			Iterator operator -- (int);
		};
		//----------------------------------------------------------------------------------------------------------
		//内存池可用元素迭代器（此迭代器将仅仅访问池中所有不为空的元素。）
		class DataIterator : public BaseIterator
		{
		public:
			//向后移动迭代器（先计算）
			DataIterator& operator ++ ();

			//向后移动迭代器（后计算）
			DataIterator operator ++ (int);

			//向前移动迭代器（先计算）
			DataIterator& operator -- ();

			//向前移动迭代器（后计算）
			DataIterator operator -- (int);
		};
		//----------------------------------------------------------------------------------------------------------

	protected:
		PoolInfo*		m_pPoolInfo;		//内存池信息数组
		unsigned int	m_nNumPool;			//当前内存池数量
		unsigned int	m_nPoolDataLen;		//内存池数据内存分配的大小

		PoolInfo*		m_pCurPool;			//当前可被使用的元素所在的内存池指针
		unsigned int	m_pCurNum;			//当前可被使用的元素位置

		PoolInfo*		m_pNextPool;		//下一个可被使用的元素所在的内存池指针
		unsigned int	m_pNextNum;			//下一个可被使用的元素位置

		unsigned int	m_nPoolSize;		//单个内存池容纳元素数量

		unsigned int	m_nNumData;			//当前元素数量
		unsigned int	m_nMaxNumData;		//最大元素数量

		BaseIterator	m_TmpIterator;		//复制临时基本元素迭代器

	public:
		MemPool();
		~MemPool();

		//初始化内存池（NumPerPool 为单个内存池的元素数，PoolNum 为初始内存池数量。）
		void Initialize( unsigned int NumPerPool, unsigned int PoolNum );

		//释放内存池
		void Release();

		//清空内存池中所有元素
		void Clear();

		//增加新元素
		T* Add();

		//删除旧元素
		void Erase( T* pData );

		//返回指定位置的元素内存迭代器
		Iterator& GetIterator( unsigned int nPos = 0 );

		//返回指定位置的可用元素迭代器
		DataIterator& GetDataIterator( unsigned int nPos = 0 );

		//返回超前基本元素迭代器
		BaseIterator& Front();

		//返回超尾基本元素迭代器
		BaseIterator& End();

	protected:
		//分配一个新的内存池
		void AllocNewPool();
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
