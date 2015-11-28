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
	Camera::Camera()
		: mFOVy					(45.0f)
		, mAspect				(4.0f/3.0f)
		, mViewNear				(1.0f)
		, mViewFar				(10000.0f)
		, mViewPos				( Vector3( 0.0f, 0.0f, 0.0f ) )
		, mViewPitch			(0.0f)
		, mViewYaw				(0.0f)
		, mViewRoll				(0.0f)
		, mbNeedUpdateViewPos	(true)
		, mbNeedUpdateViewRot	(true)
		, mbNeedUpdateView		(true)
		, mbNeedUpdateProj		(true)
		, mbNeedUpdateFrum		(true)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置观察状态
	void Camera::SetView( const Vector3& vPos, float fPitch, float fYaw, float fRoll )
	{
		mViewPos = vPos;
		mViewPitch = fPitch;
		mViewYaw = fYaw;
		mViewRoll = fRoll;
		mbNeedUpdateView = true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置观察位置
	void Camera::SetViewPos( const Vector3& vPos )
	{
		mViewPos = vPos;
		mbNeedUpdateView = true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//平移观察位置
	void Camera::Translate( const Vector3& vTrans )
	{
		mViewPos += vTrans;
		mbNeedUpdateView = true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//平移观察位置
	void Camera::Translate( float fX, float fY, float fZ )
	{
		mViewPos.x += fX;
		mViewPos.y += fY;
		mViewPos.z += fZ;
		mbNeedUpdateView = true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//旋转观察方向
	void Camera::Rotate( float fPitch, float fYaw, float fRoll )
	{
		mViewPitch	+= fPitch;
		mViewYaw	+= fYaw;
		mViewRoll	+= fRoll;

		mbNeedUpdateView = true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置俯仰角
	void Camera::SetPitch( float fDegree )
	{
		mViewPitch = fDegree;
		mbNeedUpdateView = true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置偏转角
	void Camera::SetYaw( float fDegree )
	{
		mViewYaw = fDegree;
		mbNeedUpdateView = true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置倾斜角
	void Camera::SetRoll( float fDegree )
	{
		mViewRoll = fDegree;
		mbNeedUpdateView = true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置投影状态
	void Camera::SetProjection( float fFOVy, float fAspect, float fNear, float fFar )
	{
		mFOVy = fFOVy;
		mAspect = fAspect;
		mViewNear = fNear;
		mViewFar = fFar;

		mbNeedUpdateProj = true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置观察视角
	void Camera::SetVisualAngle( float fFOVy )
	{
		mFOVy = fFOVy;
		mbNeedUpdateProj = true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置观察横纵比
	void Camera::SetAspect( float fAspect )
	{
		mAspect = fAspect;
		mbNeedUpdateProj = true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置近裁剪面
	void Camera::SetViewNear( float fNear )
	{
		mViewNear = fNear;
		mbNeedUpdateProj = true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置远裁剪面
	void Camera::SetViewFar( float fFar )
	{
		mViewFar = fFar;
		mbNeedUpdateProj = true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取观察矩阵
	Matrix4& Camera::GetViewMatrix()
	{
		return mViewMatrix;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取投影矩阵
	Matrix4& Camera::GetProjMatrix()
	{
		return mProjMatrix;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取观察投影矩阵
	Matrix4& Camera::GetViewProjMatrix()
	{
		return mViewProjMatrix;
	}
	//--------------------------------------------------------------------------------------------------------------
	//更新观察位移矩阵
	void Camera::_UpdateViewPosMatrix()
	{
		//设置位移
		static Vector3 vViewFront;
		vViewFront.x = mViewPos.x;
		vViewFront.y = mViewPos.y;
		vViewFront.z = mViewPos.z + 1.0f;

		mViewMatrix.SetLookAtLH( mViewPos, vViewFront, Vector3::UnitY );

		mbNeedUpdateView = true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//更新观察旋转矩阵
	void Camera::_UpdateViewRotaMatrix()
	{
		//设置旋转
		static Matrix4 MatA;
		static Matrix4 MatB;
		static Matrix4 MatC;

		//绕 Y 轴旋转 * 绕 X 轴旋转
		MatA.SetRotationY( mViewYaw );
		MatB.SetRotationX( mViewPitch );

		Mat4TransMat4( &MatC, &MatA, &MatB );

		//绕 Z 轴旋转
		MatA.SetRotationZ( mViewRoll );

		Mat4TransMat4( &mRotaMatrix, &MatC, &MatA );

		mbNeedUpdateView = true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//更新摄像机
	void Camera::_UpdateCamera()
	{
		if( mbNeedUpdateViewPos )
			_UpdateViewPosMatrix();

		if( mbNeedUpdateViewRot )
			_UpdateViewRotaMatrix();

		if( mbNeedUpdateView )
			_UpdateViewMatrix();

		if( mbNeedUpdateProj )
			_UpdateProjMatrix();

		if( mbNeedUpdateFrum )
			_UpdateViewFrustum();
	}
	//--------------------------------------------------------------------------------------------------------------
	//更新观察矩阵
	void Camera::_UpdateViewMatrix()
	{
		//级联位移和旋转矩阵
		mViewMatrix *= mRotaMatrix;

		mbNeedUpdateView = false;
		mbNeedUpdateFrum = true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//更新投影矩阵
	void Camera::_UpdateProjMatrix()
	{
		mProjMatrix.SetPerspectiveFovLH( mFOVy, mAspect, mViewNear, mViewFar );

		mbNeedUpdateProj = false;
		mbNeedUpdateFrum = true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//更新视体
	void Camera::_UpdateViewFrustum()
	{
		//更新观察投影矩阵
		Mat4TransMat4( &mViewProjMatrix, &mViewMatrix, &mProjMatrix );

		mViewFrustum.UpdateViewFrustum( mViewProjMatrix );

		mbNeedUpdateFrum = false;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
