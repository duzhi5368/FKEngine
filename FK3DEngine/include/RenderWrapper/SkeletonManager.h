/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	SkeletonManager
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	骨骼管理类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//骨骼管理类
	class FK_DLL SkeletonManager : public Singleton< SkeletonManager >
	{
	public:
		UINT			mNumSkeleton;		//骨骼数量
		SkeletonList	mSkeletonList;		//骨骼列表

	public:
		SkeletonManager();
		~SkeletonManager();

		//创建新骨骼
		Skeleton* CreateSkeleton( UINT nNumKey, float fIntervalTime );

		//删除原有骨骼
		void DeleteSkeleton( Skeleton* pSkeleton );

		//删除所有骨骼
		void DeleteAllSkeleton();

		//获取指定骨骼
		Skeleton* GetSkeleton( UINT nIndex );
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
