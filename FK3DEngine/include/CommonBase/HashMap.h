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
*	文件说明：	哈希表模板类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//哈希表模板类
	template <typename T> class FK_DLL HashMap
	{
	public:
		//哈希字符串最大长度
		static const int		mMaxHashStrLen = 32;

		//哈希元素结构
		struct HashData
		{
			T				Data;						//存储数据

			char			szKeyName[mMaxHashStrLen];	//键名
			unsigned short	nKeyNameLen;				//键名长度

			HashData*		pNext;						//下一个元素的指针
			HashData*		pPrev;						//上一个元素的指针

			unsigned int	nStackIndex;				//该元素在堆栈中的索引号
		};

	public:
		unsigned int			mnTableSize;	//哈希元素索引表大小
		HashData**				mpHashTable;	//哈希元素索引表

		UINT					mNumData;		//哈希元素数量
		Stack< HashData* >		mHashDataStack;	//哈希元素指针堆栈（用来进行快速轮循）

	public:
		HashMap();
		~HashMap();

		//初始化哈希表（nTableSize 为哈希表大小，如果添加的元素超过了此数值将造成重复率极高。）
		void Initialize( unsigned int nTableSize );

		//清空哈希表
		void Clear();

		//释放哈希表
		void Release();

		//加入元素
		bool Add( const T& refData, const char* szKeyName );

		//删除元素
		void Del( const char* szKeyName );

		//寻找元素
		T* Find( const char* szKeyName );

		//返回当前元素数
		FKInline unsigned int Size() const;
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
