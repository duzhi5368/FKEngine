/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	AxisBox2
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	二维轴对齐包围盒类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//二维轴对齐包围盒类
	class FK_DLL AxisBox2
	{
	public:
		union
		{
			struct
			{
				Vector2	mMaxVer;		//包围盒最大点
				Vector2	mMinVer;		//包围盒最小点
			};
			float m[4];
		};

		bool			mbEmpty;		//标志包围盒是否为空

	public:
		AxisBox2();
		AxisBox2( const AxisBox2& refBox );
		AxisBox2( const Vector2& vMax, const Vector2& vMin );
		AxisBox2( const float pValue[4] );
		AxisBox2( float fMaxX, float fMaxY, float fMinX, float fMinY );

		//赋值运算符的重载
		AxisBox2& operator = ( const AxisBox2& refBox );

		//设置包围盒最大点、最小点。
		void SetMaxMin( const Vector2& vMax, const Vector2& vMin );
		void SetMaxMin( const float pValue[4] );
		void SetMaxMin( float fMaxX, float fMaxY, float fMinX, float fMinY );

		//获取包围盒最大点、最小点。
		void GetMaxMin( Vector2* pvMax, Vector2* pvMin ) const;
		void GetMaxMin( float pValue[4] ) const;
		void GetMaxMin( float& fMaxX, float& fMaxY, float& fMinX, float& fMinY ) const;

		//获取包围盒角点
		void GetCorner( Vector2 pCorner[4] ) const;

		//设置/获取是否为空
		void SetEmpty( const bool bEmpty );
		bool IsEmpty() const;

		//合并点集
		void ContainVertex( const Vector2* pVertex, const UINT nNumVer );

		//合并包围盒
		void Combine( const AxisBox2& refBox );

		//获取合并后的包围盒
		void GetCombine( AxisBox2* pOutAxisBox2, const AxisBox2& refBox ) const;

		//检测指定包围盒是否与该包围盒相交
		bool IsIntersect( const AxisBox2& refBox ) const;
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
