/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Skeleton
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
	Skeleton::Skeleton( UINT nNumKey, float fIntervalTime )
		: mNumKey			(nNumKey)
		, mIntervalTime		(fIntervalTime)
		, mNumBones			(0)
	{
		mBoneMatList.Initialize( 10, 10 );
	}
	//--------------------------------------------------------------------------------------------------------------
	Skeleton::~Skeleton()
	{
		DeleteAllBones();

		mBoneMatList.Release();
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建骨头
	void Skeleton::CreateBone( const Matrix4* pBoneKeyMatrix )
	{
		Matrix4Stack* pMatStack = new Matrix4Stack;
		*mBoneMatList.Push() = pMatStack;

		pMatStack->Initialize( mNumKey, 100 );
		memcpy( pMatStack->Begin(), pBoneKeyMatrix, sizeof(Matrix4) * mNumKey );

		++mNumBones;
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除骨头
	void Skeleton::DeleteBone( UINT nBoneIndex )
	{
		if( nBoneIndex >= mNumBones )
			Except( Exception::ERR_INVALIDPARAMS, "指定要删除的骨头不存在。" );

		Matrix4Stack* pMatStack = *( mBoneMatList.Begin() + nBoneIndex );
		mBoneMatList.Erase( nBoneIndex );

		delete pMatStack;

		--mNumBones;
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除所有骨头
	void Skeleton::DeleteAllBones()
	{
		BoneMatList::Iterator it = mBoneMatList.Begin();
		BoneMatList::Iterator end = mBoneMatList.End();
		for(; it!=end; ++it )
			SafeDelete( *it );

		mBoneMatList.Clear();

		mNumBones = 0;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置帧数量
	void Skeleton::SetKeyNumber( UINT nNumKey )
	{
		mNumKey = nNumKey;

		BoneMatList::Iterator it = mBoneMatList.Begin();
		BoneMatList::Iterator end = mBoneMatList.End();
		for(; it!=end; ++it )
			(*it)->Resize( nNumKey );
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置停顿时间
	void Skeleton::SetIntervalTime( float fIntervalTime )
	{
		mIntervalTime = fIntervalTime;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
