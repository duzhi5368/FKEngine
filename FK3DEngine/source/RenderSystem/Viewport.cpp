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

#include "../../include/RenderSystem/RenderSystemInsideHeader.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	Viewport::Viewport()
		: mLeft				(0)
		, mTop				(0)
		, mWidth			(800)
		, mHeight			(600)
		, mAspect			(800.0f/600.0f)
		, mMinZ				(0.0f)
		, mMaxZ				(1.0f)
		, mViewportMat		(Matrix4::Identity)
		, mpCamera			(NULL)
		, mdwClearColor		(0)
		, mbClearPerFrame	(false)
	{
		_UpdateViewport();
	}
	//--------------------------------------------------------------------------------------------------------------
	Viewport::Viewport( DWORD dwLeft, DWORD dwTop, DWORD dwWidth, DWORD dwHeight, float fMinZ, float fMaxZ )
		: mLeft				(dwLeft)
		, mTop				(dwTop)
		, mWidth			(dwWidth)
		, mHeight			(dwHeight)
		, mMinZ				(fMinZ)
		, mMaxZ				(fMaxZ)
		, mViewportMat		(Matrix4::Identity)
		, mpCamera			(NULL)
		, mdwClearColor		(0)
		, mbClearPerFrame	(false)
	{
		mAspect = (float)dwWidth / (float)dwHeight;

		_UpdateViewport();
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置视口摄像机
	void Viewport::SetCamera( Camera* pCamera, bool bAutoSetProj )
	{
		//存储摄像机指针
		mpCamera = pCamera;

		//设置摄像机投影矩阵
		if( bAutoSetProj )
		{
			pCamera->mAspect = mAspect;
			pCamera->mbNeedUpdateProj;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置视口范围
	void Viewport::SetViewportRect( DWORD dwLeft, DWORD dwTop, DWORD dwWidth, DWORD dwHeight, float fMinZ, float fMaxZ )
	{
		mLeft	= dwLeft;
		mTop	= dwTop;
		mWidth	= dwWidth;
		mHeight	= dwHeight;
		mMinZ	= fMinZ;
		mMaxZ	= fMaxZ;

		mAspect = (float)dwWidth / (float)dwHeight;

		_UpdateViewport();
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置清空颜色
	void Viewport::SetClearColor( const ColorValue& refColor )
	{
		mdwClearColor = refColor.GetARGB();
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置是否在渲染每一帧前清空原有图像
	void Viewport::EnableClear( bool bEnable )
	{
		mbClearPerFrame = bEnable;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取屏幕坐标在世界空间的投影射线
	bool Viewport::GetWorldRayFromScreenPoint( LONG nX, LONG nY, Ray3* pRay3 )
	{
		if( nX < (LONG)mLeft || nY < (LONG)mTop )
			return false;
		
		nX -= mLeft;
		nY -= mTop;

		if( nX > (LONG)mWidth || nY > (LONG)mHeight )
			return false;

		//将屏幕坐标转换为以中心为原点的单位化坐标（投影转换）
		float fTanFOV2 = Tan( FK_PI * 0.125 );		// Tan( FK_DegreeToRadian( 45.0f / 2.0f )
		float fX = fTanFOV2 * ( (float)nX / ( (float)mWidth * 0.5f ) - 1.0f ) * mAspect;
		float fY = fTanFOV2 * ( 1.0f - (float)nY / ( (float)mHeight * 0.5f ) );

		//将经过投影转换的坐标值转换到世界空间坐标
		static Matrix4 MatInvView;
		static Vector3 VectorEnd;

		MatInvView = mpCamera->mViewMatrix.GetInverse();

		pRay3->mOrigin.x = fX * mpCamera->mViewNear;
		pRay3->mOrigin.y = fY * mpCamera->mViewNear;
		pRay3->mOrigin.z = mpCamera->mViewNear;

		VectorEnd.x = fX * mpCamera->mViewFar;
		VectorEnd.y = fY * mpCamera->mViewFar;
		VectorEnd.z = mpCamera->mViewFar;

		pRay3->mOrigin *= MatInvView;
		VectorEnd *= MatInvView;

		pRay3->mDirection = VectorEnd - pRay3->mOrigin;
		pRay3->mDirection.Normalize();

		return true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//更新视口
	void FKFastcall Viewport::_UpdateViewport()
	{
		//更新视口摄像机观察横纵比
		if( mpCamera != NULL )
			mpCamera->SetAspect( mAspect );

		//更新视口矩阵
		float fHalfWidth = (float)mWidth / 2.0f;
		float fHalfHeight = (float)mHeight / 2.0f;

		mViewportMat._11 = fHalfWidth;
		mViewportMat._22 = -fHalfHeight;
		mViewportMat._33 = mMaxZ - mMinZ;
		mViewportMat._41 = (float)mLeft + fHalfWidth;
		mViewportMat._42 = (float)mTop + fHalfHeight;
		mViewportMat._43 = mMinZ;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
