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
*	文件说明：	动画控制器管理类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//动画控制器管理类
	class FK_DLL AnimControlManager : public Singleton< AnimControlManager >
	{
	public:
		UINT				mNumAnimControl;	//动画管理器数量
		AnimControlList		mAnimControlList;	//动画管理器列表

	public:
		AnimControlManager();
		~AnimControlManager();

		//创建新动画管理器
		AnimControl* CreateAnimControl();

		//删除原有动画管理器
		void DeleteAnimControl( AnimControl* pAnimControl );

		//删除所有动画管理器
		void DeleteAllAnimControl();

		//获取指定动画管理器
		AnimControl* GetAnimControl( UINT nIndex );

		//更新动画帧时间
		void UpdateFrameTime( float fFrameTime );
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
