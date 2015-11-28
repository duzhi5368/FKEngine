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
	Segment3::Segment3()
		: mOrigin		(Vector3::Zero)
		, mVector		(Vector3::Zero)
		, mDirection	(Vector3::Zero)
		, mLength		(0.0f)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	Segment3::Segment3( const Vector3& vOrigin, const Vector3& vDirection, const float fLength )
		: mOrigin		(vOrigin)
		, mVector		(vDirection*fLength)
		, mDirection	(vDirection)
		, mLength		(fLength)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	Segment3::Segment3( const Vector3& vOrigin, const Vector3& vVector )
		: mOrigin		(vOrigin)
		, mVector		(vVector)
	{
		mLength = vVector.GetLength();
		mDirection = vVector.GetNormalizedVecFast( mLength );
	}
	//--------------------------------------------------------------------------------------------------------------
	Segment3& Segment3::operator = ( const Segment3& seg3ref )
	{
		memcpy( this, &seg3ref, sizeof(Segment3) );
		return *this;
	}
	//--------------------------------------------------------------------------------------------------------------
	Segment3 Segment3::operator * ( const Matrix4& mat4ref ) const
	{
		static Segment3 tmp;

		tmp.mOrigin = mOrigin * mat4ref;
		tmp.mVector = mVector * mat4ref;
		tmp.mLength = tmp.mVector.GetLength();
		tmp.mDirection = tmp.mVector.GetNormalizedVecFast( tmp.mLength );

		return tmp;
	}
	//--------------------------------------------------------------------------------------------------------------
	void Segment3::operator *= ( const Matrix4& mat4ref )
	{

		mOrigin *= mat4ref;
		mVector *= mat4ref;
		mLength = mVector.GetLength();
		mDirection = mVector.GetNormalizedVecFast( mLength );
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
