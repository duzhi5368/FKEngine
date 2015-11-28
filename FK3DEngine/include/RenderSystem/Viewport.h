/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Viewport
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

#include "../RenderWrapper/Camera.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//视口类
	class FK_DLL Viewport
	{
	public:
		DWORD		mLeft;				//视口左边距
		DWORD		mTop;				//视口上边距
		DWORD		mWidth;				//视口宽
		DWORD		mHeight;			//视口高
		float		mMinZ;				//最小渲染深度值
		float		mMaxZ;				//最大渲染深度值
		float		mAspect;			//视口横纵比

		Matrix4		mViewportMat;		//视口矩阵

		Camera*		mpCamera;			//视口的摄像机

		ARGB		mdwClearColor;		//视口清空颜色
		bool		mbClearPerFrame;	//是否在渲染每一帧前清空原有图像

	public:
		Viewport();
		Viewport( DWORD dwLeft, DWORD dwTop, DWORD dwWidth, DWORD dwHeight, float fMinZ, float fMaxZ );

		//设置视口摄像机
		void SetCamera( Camera* pCamera, bool bAutoSetProj = true );

		//设置视口范围
		void SetViewportRect( DWORD dwLeft, DWORD dwTop, DWORD dwWidth, DWORD dwHeight, float fMinZ, float fMaxZ );

		//设置清空颜色
		void SetClearColor( const ColorValue& refColor );

		//设置是否在渲染每一帧前清空原有图像
		void EnableClear( bool bEnable = true );

		//获取屏幕坐标在世界空间的投影射线
		bool GetWorldRayFromScreenPoint( LONG nX, LONG nY, Ray3* pRay3 );

	protected:
		//更新视口
		void FKFastcall _UpdateViewport();
	};
	//--------------------------------------------------------------------------------------------------------------
	typedef Stack< Viewport* >		ViewportList;
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
