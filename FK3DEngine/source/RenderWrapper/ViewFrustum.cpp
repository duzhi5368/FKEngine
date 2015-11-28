/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	ViewFrustum
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
	ViewFrustum::ViewFrustum()
	{
		mOriVertex[0].x = -1.0f;	mOriVertex[0].y = -1.0f;	mOriVertex[0].z = 0.0f;		// xyz
		mOriVertex[1].x =  1.0f;	mOriVertex[1].y = -1.0f;	mOriVertex[1].z = 0.0f;		// Xyz
		mOriVertex[2].x = -1.0f;	mOriVertex[2].y =  1.0f;	mOriVertex[2].z = 0.0f;		// xYz
		mOriVertex[3].x =  1.0f;	mOriVertex[3].y =  1.0f;	mOriVertex[3].z = 0.0f;		// XYz
		mOriVertex[4].x = -1.0f;	mOriVertex[4].y = -1.0f;	mOriVertex[4].z = 1.0f;		// xyZ
		mOriVertex[5].x =  1.0f;	mOriVertex[5].y = -1.0f;	mOriVertex[5].z = 1.0f;		// XyZ
		mOriVertex[6].x = -1.0f;	mOriVertex[6].y =  1.0f;	mOriVertex[6].z = 1.0f;		// xYZ
		mOriVertex[7].x =  1.0f;	mOriVertex[7].y =  1.0f;	mOriVertex[7].z = 1.0f;		// XYZ
	}
	//--------------------------------------------------------------------------------------------------------------
	//更新视体
	void ViewFrustum::UpdateViewFrustum( const Matrix4& ViewProjMatrix )
	{
		//将观察矩阵与投影矩阵相乘得出计算视体的矩阵
		static Matrix4 FrustumMat;
		Mat4Inverse( &FrustumMat, &ViewProjMatrix );

		//转换八个顶点的坐标到世界空间
		Mat4TransVec3( &mVertex3[0], &FrustumMat, (Vector3*)&mOriVertex[0] );
		Mat4TransVec3( &mVertex3[1], &FrustumMat, (Vector3*)&mOriVertex[1] );
		Mat4TransVec3( &mVertex3[2], &FrustumMat, (Vector3*)&mOriVertex[2] );
		Mat4TransVec3( &mVertex3[3], &FrustumMat, (Vector3*)&mOriVertex[3] );
		Mat4TransVec3( &mVertex3[4], &FrustumMat, (Vector3*)&mOriVertex[4] );
		Mat4TransVec3( &mVertex3[5], &FrustumMat, (Vector3*)&mOriVertex[5] );
		Mat4TransVec3( &mVertex3[6], &FrustumMat, (Vector3*)&mOriVertex[6] );
		Mat4TransVec3( &mVertex3[7], &FrustumMat, (Vector3*)&mOriVertex[7] );

		//计算视体的六个面
		mPlane3[0].SetFromPoints( mVertex3[0], mVertex3[1], mVertex3[2] ); //近面
		mPlane3[1].SetFromPoints( mVertex3[6], mVertex3[7], mVertex3[5] ); //远面
		mPlane3[2].SetFromPoints( mVertex3[2], mVertex3[6], mVertex3[4] ); //左面
		mPlane3[3].SetFromPoints( mVertex3[7], mVertex3[3], mVertex3[5] ); //右面
		mPlane3[4].SetFromPoints( mVertex3[2], mVertex3[3], mVertex3[6] ); //上面
		mPlane3[5].SetFromPoints( mVertex3[1], mVertex3[0], mVertex3[4] ); //下面
	}
	//--------------------------------------------------------------------------------------------------------------
	//检测指定包围盒是否与视体相交
	bool ViewFrustum::IsIntersect( const Vector3 pBoxVer[8] )
	{
		//重置存储包围盒的八个角点是否在视体的六个面内的变量（每个点的每一位代表该点与某个面的关系）
		static BYTE bOutSide[8];
		memset( bOutSide, 0, sizeof(BYTE)*8 );

		//检测包围盒的八个角点是否在视体的六个面内
		for( int nPoint=0; nPoint<8; ++nPoint )
		{
			for( int nPlane3=0; nPlane3<6; ++nPlane3 )
			{
				//如果该点在视体的第 i 面之外，则将该点的 bOutSide 中的第 i 位设为一。
				if( mPlane3[nPlane3].mNormal.x * pBoxVer[nPoint].x +
					mPlane3[nPlane3].mNormal.y * pBoxVer[nPoint].y +
					mPlane3[nPlane3].mNormal.z * pBoxVer[nPoint].z +
					mPlane3[nPlane3].mDistance < 0 )
				{
					bOutSide[nPoint] |= ( 1 << nPlane3 );
				}
			}
			//如果某点的 bOutSide 的所有位都为 0，那么该点肯定在视体内，所以返回真。
			if( bOutSide[nPoint] == 0 )
				return true;
		}

		//如果所有点都在视体的某个面外则返回假
		if( (	bOutSide[0] & bOutSide[1] & bOutSide[2] & bOutSide[3] &
				bOutSide[4] & bOutSide[5] & bOutSide[6] & bOutSide[7]	) != 0 )
		{
			return false;
		}

		//其它情况则返回真渲染它
		//
		//	这里其实还可以进行更加详细的视体的 12 条边与包围盒的 12 条边之间的检测，
		//	但考虑到其运算量较大，而进行上面的检测后还无法判断出物体实际在视体外的情况又很少，
		//	如果渲染的模型不是很复杂则有些得不偿失，所以这里并没有进行这项检测。
		//	但是如果模型的渲染耗时要比进行这个计算大得多，则可以考虑进行这项检测，
		//	具体的方法和代码请参考 DirectX 9.0 SDK Cull Sample 。

		return true;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
