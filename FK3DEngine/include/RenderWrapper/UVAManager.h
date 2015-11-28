/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	UVAManager
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------

	// UV 动画数据
	struct UVAnimation
	{
		int			nSegment;		//动画段数量
		int			nFrames;		//动画总帧数
		UVASegment*	pSegments;		//动画段数据指针
		UVAFrame*	pFrames;		//动画帧数据指针
	};
	typedef Stack< UVAnimation* >	UVAnimationList;

	//--------------------------------------------------------------------------------------------------------------
	// UV 动画管理器
	class FK_DLL UVAManager : public Singleton< UVAManager >
	{
	protected:
		FileObject*			mpPackFile;			// UVA 打包文件

		UVAnimationList		mUVAnimationList;	// UV 动画数据列表

	public:
		UVAManager();
		~UVAManager();

		//设置 UVA 打包文件
		void SetUVAPackFile( FileObject* pPackFile );

		//创建 UV 动画数据对象
		UVAnimation* CreateUVAnimation( LPCSTR szFileName );

		//删除 UV 动画数据对象
		void DeleteUVAnimation( UVAnimation* pUVAnimation );

		//删除所有 UV 动画数据对象
		void DeleteAllUVAnimation();
	};
	//--------------------------------------------------------------------------------------------------------------
};

//==================================================================================================================
