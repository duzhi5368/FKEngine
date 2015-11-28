/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	AnimControlManager
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
	AnimControlManager::AnimControlManager()
		: mNumAnimControl		(0)
	{
		mAnimControlList.Initialize( 20, 20 );
	}
	//--------------------------------------------------------------------------------------------------------------
	AnimControlManager::~AnimControlManager()
	{
		DeleteAllAnimControl();

		mAnimControlList.Release();
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建新动画管理器
	AnimControl* AnimControlManager::CreateAnimControl()
	{
		AnimControl* pAnimControl = new AnimControl();
		*mAnimControlList.Push() = pAnimControl;
		++mNumAnimControl;
		return pAnimControl;
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除原有动画管理器
	void AnimControlManager::DeleteAnimControl( AnimControl* pAnimControl )
	{
		if( pAnimControl == NULL )
			return;

		AnimControlList::Iterator it = mAnimControlList.Begin();
		AnimControlList::Iterator end = mAnimControlList.End();
		for(; it!=end; ++it )
		{
			if( *it == pAnimControl )
			{
				mAnimControlList.Erase( it );
				delete pAnimControl;
				--mNumAnimControl;
				return;
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除所有动画管理器
	void AnimControlManager::DeleteAllAnimControl()
	{
		AnimControlList::Iterator it = mAnimControlList.Begin();
		AnimControlList::Iterator end = mAnimControlList.End();
		for(; it!=end; ++it )
			SafeDelete( *it );

		mAnimControlList.Clear();
		mNumAnimControl = 0;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取指定动画管理器
	AnimControl* AnimControlManager::GetAnimControl( UINT nIndex )
	{
		if( nIndex >= mNumAnimControl )
			Except( Exception::ERR_INVALIDPARAMS, "指定要删除的动画管理器不存在。" );

		return *( mAnimControlList.Begin() + nIndex );
	}
	//--------------------------------------------------------------------------------------------------------------
	//更新动画帧时间
	void AnimControlManager::UpdateFrameTime( float fFrameTime )
	{
		AnimControlList::Iterator it = mAnimControlList.Begin();
		AnimControlList::Iterator end = mAnimControlList.End();

		for(; it!=end; ++it )
		{
			AnimControl* pAnimControl = *it;
			if( pAnimControl->mbCanPlay )
				pAnimControl->UpdateKeyTime( fFrameTime );
		}
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
