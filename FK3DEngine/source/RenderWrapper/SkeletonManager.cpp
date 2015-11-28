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
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../../include/RenderWrapper/RenderWrapperInsideHeader.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	SkeletonManager::SkeletonManager()
		: mNumSkeleton		(0)
	{
		mSkeletonList.Initialize( 20, 20 );
	}
	//--------------------------------------------------------------------------------------------------------------
	SkeletonManager::~SkeletonManager()
	{
		DeleteAllSkeleton();

		mSkeletonList.Release();
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建新骨骼
	Skeleton* SkeletonManager::CreateSkeleton( UINT nNumKey, float fIntervalTime )
	{
		Skeleton* pSkeleton = new Skeleton( nNumKey, fIntervalTime );
		*mSkeletonList.Push() = pSkeleton;
		++mNumSkeleton;
		return pSkeleton;
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除原有骨骼
	void SkeletonManager::DeleteSkeleton( Skeleton* pSkeleton )
	{
		if( pSkeleton == NULL )
			return;

		SkeletonList::Iterator it = mSkeletonList.Begin();
		SkeletonList::Iterator end = mSkeletonList.End();
		for(; it!=end; ++it )
		{
			if( *it == pSkeleton )
			{
				mSkeletonList.Erase( it );
				delete pSkeleton;
				--mNumSkeleton;
				return;
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除所有骨骼
	void SkeletonManager::DeleteAllSkeleton()
	{
		SkeletonList::Iterator it = mSkeletonList.Begin();
		SkeletonList::Iterator end = mSkeletonList.End();
		for(; it!=end; ++it )
			SafeDelete( *it );

		mSkeletonList.Clear();
		mNumSkeleton = 0;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取指定骨骼
	Skeleton* SkeletonManager::GetSkeleton( UINT nIndex )
	{
		if( nIndex >= mNumSkeleton )
			Except( Exception::ERR_INVALIDPARAMS, "指定要删除的骨骼不存在。" );

		return *( mSkeletonList.Begin() + nIndex );
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
