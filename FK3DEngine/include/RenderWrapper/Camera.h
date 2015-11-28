/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Camera
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	摄像机类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

#include "ViewFrustum.h"


//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//摄像机类
	class FK_DLL Camera
	{
	public:
		float		mFOVy;					//观察视角
		float		mAspect;				//观察横纵比
		float		mViewNear;				//近裁剪面
		float		mViewFar;				//远裁剪面

		Vector3		mViewPos;				//观察点
		float		mViewPitch;				//观察俯仰角
		float		mViewYaw;				//观察偏转角
		float		mViewRoll;				//观察倾斜角

		Matrix4		mRotaMatrix;			//观察旋转矩阵
		Matrix4		mViewMatrix;			//观察矩阵
		Matrix4		mProjMatrix;			//投影矩阵
		Matrix4		mViewProjMatrix;		//观察投影合并矩阵

		ViewFrustum	mViewFrustum;			//视体

		bool		mbNeedUpdateViewPos;	//是否需要更新观察位移矩阵
		bool		mbNeedUpdateViewRot;	//是否需要更新观察旋转矩阵
		bool		mbNeedUpdateView;		//是否需要更新观察矩阵
		bool		mbNeedUpdateProj;		//是否需要更新投影矩阵
		bool		mbNeedUpdateFrum;		//是否需要更新视体

	public:
		Camera();

		//设置观察状态
		void SetView( const Vector3& vPos, float fPitch, float fYaw, float fRoll );

		//设置观察位置
		void SetViewPos( const Vector3& vPos );

		//平移观察位置
		void Translate( const Vector3& vTrans );

		//平移观察位置
		void Translate( float fX, float fY, float fZ );

		//旋转观察方向
		void Rotate( float fPitch, float fYaw, float fRoll );

		//设置俯仰角
		void SetPitch( float fDegree );

		//设置偏转角
		void SetYaw( float fDegree );

		//设置倾斜角
		void SetRoll( float fDegree );

		//设置投影状态
		void SetProjection( float fFOVy, float fAspect, float fNear, float fFar );

		//设置观察视角
		void SetVisualAngle( float fFOVy );

		//设置观察横纵比
		void SetAspect( float fAspect );

		//设置近裁剪面
		void SetViewNear( float fNear );

		//设置远裁剪面
		void SetViewFar( float fFar );

		//获取观察矩阵
		Matrix4& GetViewMatrix();

		//获取投影矩阵
		Matrix4& GetProjMatrix();

		//获取观察投影矩阵
		Matrix4& GetViewProjMatrix();

		//更新观察位移矩阵
		void _UpdateViewPosMatrix();

		//更新观察旋转矩阵
		void _UpdateViewRotaMatrix();

		//更新摄像机
		void _UpdateCamera();

		//更新观察矩阵
		void _UpdateViewMatrix();

		//更新投影矩阵
		void _UpdateProjMatrix();

		//更新视体
		void _UpdateViewFrustum();
	};
	//--------------------------------------------------------------------------------------------------------------
	typedef Stack< Camera* >	CameraList;
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
