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
*	文件说明：	三维轴对齐包围盒类 
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//三维轴对齐包围盒类
	class FK_DLL AxisBox3
	{
	public:
		union
		{
			struct
			{
				Vector3	mMaxVer;		//包围盒最大点
				Vector3	mMinVer;		//包围盒最小点
			};
			float m[6];
		};

		bool			mbEmpty;		//标志包围盒是否为空

	public:
		AxisBox3();
		AxisBox3( const AxisBox3& refBox );
		AxisBox3( const Vector3& vMax, const Vector3& vMin );
		AxisBox3( const float pValue[6] );
		AxisBox3( float fMaxX, float fMaxY, float fMaxZ, float fMinX, float fMinY, float fMinZ );

		//赋值运算符的重载
		AxisBox3& operator = ( const AxisBox3& refBox );

		//设置包围盒最大点、最小点。
		void SetMaxMin( const Vector3& vMax, const Vector3& vMin );
		void SetMaxMin( const float pValue[6] );
		void SetMaxMin( float fMaxX, float fMaxY, float fMaxZ, float fMinX, float fMinY, float fMinZ );

		//获取包围盒最大点、最小点。
		void GetMaxMin( Vector3* pvMax, Vector3* pvMin ) const;
		void GetMaxMin( float pValue[6] ) const;
		void GetMaxMin( float& fMaxX, float& fMaxY, float& fMaxZ, float& fMinX, float& fMinY, float& fMinZ ) const;

		//获取包围盒角点
		void GetCorner( Vector3 pCorner[8] ) const;

		//获取矩阵转换过的角点
		void GetCorner( Vector3 pCorner[8], const Matrix4& mat4 ) const;

		//设置/获取是否为空
		void SetEmpty( const bool bEmpty );
		bool IsEmpty() const;

		//合并点集
		void ContainVertex( const Vector3* pVertex, const UINT nNumVer );

		//合并包围盒
		void Combine( const AxisBox3& refBox );

		//获取合并后的包围盒
		void GetCombine( AxisBox3* pOutAxisBox3, const AxisBox3& refBox ) const;

		//使用矩阵转换包围盒
		void Transform( const Matrix4& mat4, Vector3 pCorner[8] = NULL );

		//获取使用矩阵转换后的包围盒
		void GetTransform( AxisBox3* pOutAxisBox3, const Matrix4& mat4, Vector3 pCorner[8] = NULL ) const;

		//检测指定包围盒是否与该包围盒相交
		bool IsIntersect( const AxisBox3& refBox ) const;
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
