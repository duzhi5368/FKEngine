/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Segment3
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	数学库 三维线段类 
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once


//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//三维线段类
	class FK_DLL Segment3
	{
	public:
		Vector3		mOrigin;	//线段原点
		Vector3		mVector;	//线段矢量
		Vector3		mDirection;	//线段方向
		float		mLength;	//线段长度

	public:
		//不同的构造函数
		Segment3();
		Segment3( const Vector3& vOrigin, const Vector3& vDirection, const float fLength );
		Segment3( const Vector3& vOrigin, const Vector3& vVector );

		//各种运算符的重载
		Segment3& operator = ( const Segment3& seg3ref );
		Segment3 operator * ( const Matrix4& mat4ref ) const;
		void operator *= ( const Matrix4& mat4ref );
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
