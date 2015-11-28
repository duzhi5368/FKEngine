/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	AxisBox3
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../../include/Math/MathInsideHead.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	AxisBox3::AxisBox3()
		: mbEmpty	(true)
	{
		memset( m, 0, sizeof(float)*6 );
	}
	//--------------------------------------------------------------------------------------------------------------
	AxisBox3::AxisBox3( const AxisBox3& refBox )
		: mbEmpty	(refBox.mbEmpty)
	{
		memcpy( m, refBox.m, sizeof(float)*6 );
	}
	//--------------------------------------------------------------------------------------------------------------
	AxisBox3::AxisBox3( const Vector3& vMax, const Vector3& vMin )
		: mbEmpty	(false)
		, mMaxVer	(vMax)
		, mMinVer	(vMin)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	AxisBox3::AxisBox3( const float pValue[6] )
		: mbEmpty	(false)
	{
		m[0] = pValue[0];		m[1] = pValue[1];		m[2] = pValue[2];
		m[3] = pValue[3];		m[4] = pValue[4];		m[5] = pValue[5];
	}
	//--------------------------------------------------------------------------------------------------------------
	AxisBox3::AxisBox3( float fMaxX, float fMaxY, float fMaxZ, float fMinX, float fMinY, float fMinZ )
		: mbEmpty	(false)
	{
		m[0] = fMaxX;		m[1] = fMaxY;		m[2] = fMaxZ;
		m[3] = fMinX;		m[4] = fMinY;		m[5] = fMinZ;
	}
	//--------------------------------------------------------------------------------------------------------------
	//赋值运算符的重载
	AxisBox3& AxisBox3::operator = ( const AxisBox3& refBox )
	{
		memcpy( this, &refBox, sizeof(AxisBox3) );
		return *this;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置包围盒最大点、最小点。
	void AxisBox3::SetMaxMin( const Vector3& vMax, const Vector3& vMin )
	{
		mMaxVer = vMax;
		mMinVer = vMin;

		mbEmpty = false;
	}
	//--------------------------------------------------------------------------------------------------------------
	void AxisBox3::SetMaxMin( const float pValue[6] )
	{
		memcpy( m, pValue, sizeof(float)*6 );

		mbEmpty = false;
	}
	//--------------------------------------------------------------------------------------------------------------
	void AxisBox3::SetMaxMin( float fMaxX, float fMaxY, float fMaxZ, float fMinX, float fMinY, float fMinZ )
	{
		m[0] = fMaxX;		m[1] = fMaxY;		m[2] = fMaxZ;
		m[3] = fMinX;		m[4] = fMinY;		m[5] = fMinZ;

		mbEmpty = false;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取包围盒最大点、最小点。
	void AxisBox3::GetMaxMin( Vector3* pvMax, Vector3* pvMin ) const
	{
		*pvMax = mMaxVer;
		*pvMin = mMinVer;
	}
	//--------------------------------------------------------------------------------------------------------------
	void AxisBox3::GetMaxMin( float pValue[6] ) const
	{
		memcpy( pValue, m, sizeof(float)*6 );
	}
	//--------------------------------------------------------------------------------------------------------------
	void AxisBox3::GetMaxMin( float& fMaxX, float& fMaxY, float& fMaxZ, float& fMinX, float& fMinY, float& fMinZ ) const
	{
		fMaxX = m[0];		fMaxY = m[1];		fMaxZ = m[2];
		fMinX = m[3];		fMinY = m[4];		fMinZ = m[5];
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取包围盒角点
	void AxisBox3::GetCorner( Vector3 pCorner[8] ) const
	{
		pCorner[0].x = mMinVer.x;		pCorner[0].y = mMinVer.y;		pCorner[0].z = mMinVer.z;	//左下前
		pCorner[1].x = mMaxVer.x;		pCorner[1].y = mMinVer.y;		pCorner[1].z = mMinVer.z;	//右下前
		pCorner[2].x = mMinVer.x;		pCorner[2].y = mMinVer.y;		pCorner[2].z = mMaxVer.z;	//左下后
		pCorner[3].x = mMaxVer.x;		pCorner[3].y = mMinVer.y;		pCorner[3].z = mMaxVer.z;	//右下后

		pCorner[4].x = mMinVer.x;		pCorner[4].y = mMaxVer.y;		pCorner[4].z = mMinVer.z;	//左上前
		pCorner[5].x = mMaxVer.x;		pCorner[5].y = mMaxVer.y;		pCorner[5].z = mMinVer.z;	//右上前
		pCorner[6].x = mMinVer.x;		pCorner[6].y = mMaxVer.y;		pCorner[6].z = mMaxVer.z;	//左上后
		pCorner[7].x = mMaxVer.x;		pCorner[7].y = mMaxVer.y;		pCorner[7].z = mMaxVer.z;	//右上后
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取矩阵转换过的角点
	void AxisBox3::GetCorner( Vector3 pCorner[8], const Matrix4& mat4 ) const
	{
		//获取角点
		static Vector3 vCorner[8];
		GetCorner( vCorner );

		//转换所有角点坐标
		for( UINT i=0; i<8; ++i )
			Mat4TransVec3( pCorner + i, &mat4, vCorner + i );
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置是否为空
	void AxisBox3::SetEmpty( const bool bEmpty )
	{
		mbEmpty = bEmpty;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取是否为空
	bool AxisBox3::IsEmpty() const
	{
		return mbEmpty;
	}
	//--------------------------------------------------------------------------------------------------------------
	//合并点集
	void AxisBox3::ContainVertex( const Vector3* pVertex, const UINT nNumVer )
	{
		if( nNumVer == 0 )
			return;

		mbEmpty = false;

		//重新计算最大值与最小值
		for( UINT i=0; i<nNumVer; ++i )
		{
			const Vector3* pVerPtr = pVertex + i;

			if( pVerPtr->x < mMinVer.x )
				mMinVer.x = pVerPtr->x;
			else if( pVerPtr->x > mMaxVer.x )
				mMaxVer.x = pVerPtr->x;

			if( pVerPtr->y < mMinVer.y )
				mMinVer.y = pVerPtr->y;
			else if( pVerPtr->y > mMaxVer.y )
				mMaxVer.y = pVerPtr->y;

			if( pVerPtr->z < mMinVer.z )
				mMinVer.z = pVerPtr->z;
			else if( pVerPtr->z > mMaxVer.z )
				mMaxVer.z = pVerPtr->z;

			++pVerPtr;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//合并包围盒
	void AxisBox3::Combine( const AxisBox3& refBox )
	{
		GetCombine( this, refBox );
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取合并后的包围盒
	void AxisBox3::GetCombine( AxisBox3* pOutAxisBox3, const AxisBox3& refBox ) const
	{
		if( refBox.mbEmpty )
		{
			*pOutAxisBox3 = *this;
			return;
		}
		else if( mbEmpty && refBox.mbEmpty )
		{
			pOutAxisBox3->mbEmpty = true;
			return;
		}
		else if( mbEmpty && !refBox.mbEmpty )
		{
			*pOutAxisBox3 = refBox;
			return;
		}

		if( mMinVer.x > refBox.mMinVer.x )
			pOutAxisBox3->mMinVer.x = refBox.mMinVer.x;
		if( mMinVer.y > refBox.mMinVer.y )
			pOutAxisBox3->mMinVer.y = refBox.mMinVer.y;
		if( mMinVer.z > refBox.mMinVer.z )
			pOutAxisBox3->mMinVer.z = refBox.mMinVer.z;

		if( mMaxVer.x < refBox.mMaxVer.x )
			pOutAxisBox3->mMaxVer.x = refBox.mMaxVer.x;
		if( mMaxVer.y < refBox.mMaxVer.y )
			pOutAxisBox3->mMaxVer.y = refBox.mMaxVer.y;
		if( mMaxVer.z < refBox.mMaxVer.z )
			pOutAxisBox3->mMaxVer.z = refBox.mMaxVer.z;

		pOutAxisBox3->mbEmpty = false;
	}
	//--------------------------------------------------------------------------------------------------------------
	//使用矩阵转换包围盒
	void AxisBox3::Transform( const Matrix4& mat4, Vector3 pCorner[8] )
	{
		GetTransform( this, mat4, pCorner );
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取使用矩阵转换后的包围盒
	void AxisBox3::GetTransform( AxisBox3* pOutAxisBox3, const Matrix4& mat4, Vector3 pCorner[8] ) const
	{
		if( mbEmpty )
		{
			pOutAxisBox3->mbEmpty = true;
			return;
		}

		pOutAxisBox3->mbEmpty = false;


		//将最大点最小点转换为实际的八个角点
		Vector3* pTransCorner = NULL;
		if( pCorner == NULL )
		{
			static Vector3 vTransCorner[8];
			GetCorner( vTransCorner, mat4 );
			pTransCorner = vTransCorner;
		}
		else
		{
			GetCorner( pCorner, mat4 );
			pTransCorner = pCorner;
		}
		

		//重新获取最大值与最小值
		pOutAxisBox3->mMinVer = *pTransCorner;
		pOutAxisBox3->mMaxVer = pOutAxisBox3->mMinVer;

		for( UINT i=1; i<8; ++i )
		{
			const Vector3* pCornerVer = pTransCorner + i;

			if( pCornerVer->x < pOutAxisBox3->mMinVer.x )
				pOutAxisBox3->mMinVer.x = pCornerVer->x;
			else if( pCornerVer->x > pOutAxisBox3->mMaxVer.x )
				pOutAxisBox3->mMaxVer.x = pCornerVer->x;

			if( pCornerVer->y < pOutAxisBox3->mMinVer.y )
				pOutAxisBox3->mMinVer.y = pCornerVer->y;
			else if( pCornerVer->y > pOutAxisBox3->mMaxVer.y )
				pOutAxisBox3->mMaxVer.y = pCornerVer->y;

			if( pCornerVer->z < pOutAxisBox3->mMinVer.z )
				pOutAxisBox3->mMinVer.z = pCornerVer->z;
			else if( pCornerVer->z > pOutAxisBox3->mMaxVer.z )
				pOutAxisBox3->mMaxVer.z = pCornerVer->z;

			++pCornerVer;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//检测指定包围盒是否与该包围盒相交
	bool AxisBox3::IsIntersect( const AxisBox3& refBox ) const
	{
		if( mbEmpty || refBox.mbEmpty )
			return false;

		if( mMaxVer.x < refBox.mMinVer.x || refBox.mMaxVer.x < mMinVer.x )
			return false;

		if( mMaxVer.y < refBox.mMinVer.y || refBox.mMaxVer.y < mMinVer.y )
			return false;

		if( mMaxVer.z < refBox.mMinVer.z || refBox.mMaxVer.z < mMinVer.z )
			return false;

		return true;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
