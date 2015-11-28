/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Vector4
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
	//汇编优化四维矢量数学计算函数指针
	//--------------------------------------------------------------------------------------------------------------
	void	( FKFastcall *Vec4Add )				( Vector4* pOut, const Vector4* pArg1, const Vector4* pArg2 )	= _Def_Vec4Add;
	void	( FKFastcall *Vec4Sub )				( Vector4* pOut, const Vector4* pOrg, const Vector4* pSub )		= _Def_Vec4Sub;
	void	( FKFastcall *Vec4Mul )				( Vector4* pOut, const Vector4* pArg1, const Vector4* pArg2 )	= _Def_Vec4Mul;
	void	( FKFastcall *Vec4Div )				( Vector4* pOut, const Vector4* pOrg, const Vector4* pDiv )		= _Def_Vec4Div;
	void	( FKFastcall *Vec4MulScalar )		( Vector4* pOut, const Vector4* pOrg, const float fScalar )		= _Def_Vec4MulScalar;
	void	( FKFastcall *Vec4DivScalar )		( Vector4* pOut, const Vector4* pOrg, const float fScalar )		= _Def_Vec4DivScalar;

	float	( FKFastcall *Vec4Dot )				( const Vector4* pArg1, const Vector4* pArg2 )					= _Def_Vec4Dot;
	float	( FKFastcall *Vec4Length )			( const Vector4* pIn )											= _Def_Vec4Length;
	void	( FKFastcall *Vec4Normalize )		( Vector4* pOut, const Vector4* pIn )							= _Def_Vec4Normalize;
	void	( FKFastcall *Vec4NormalizeFast )	( Vector4* pOut, const Vector4* pIn, float fLength )			= _Def_Vec4NormalizeFast;
	//--------------------------------------------------------------------------------------------------------------
	//常用值声明
	//--------------------------------------------------------------------------------------------------------------
	Vector4 Vector4::Identity( 1.0f, 1.0f, 1.0f, 1.0f );
	Vector4 Vector4::Zero( 0.0f, 0.0f, 0.0f, 1.0f );
	Vector4 Vector4::UnitX( 1.0f, 0.0f, 0.0f, 1.0f );
	Vector4 Vector4::UnitY( 0.0f, 1.0f, 0.0f, 1.0f );
	Vector4 Vector4::UnitZ( 0.0f, 0.0f, 1.0f, 1.0f );
	//--------------------------------------------------------------------------------------------------------------
	//不同的构造函数
	//--------------------------------------------------------------------------------------------------------------
	Vector4::Vector4()
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	Vector4::Vector4( const Vector4& v4 )
		: x		(v4.x)
		, y		(v4.y)
		, z		(v4.z)
		, w		(v4.w)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	Vector4::Vector4( const Vector3& v3 )
		: x		(v3.x)
		, y		(v3.y)
		, z		(v3.z)
		, w		(1.0f)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	Vector4::Vector4( const float fx, const float fy, const float fz, const float fw )
		: x		(fx)
		, y		(fy)
		, z		(fz)
		, w		(fw)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	Vector4::Vector4( const float pVal[4] )
		: x		(pVal[0])
		, y		(pVal[1])
		, z		(pVal[2])
		, w		(pVal[3])
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	//各种运算符的重载
	//--------------------------------------------------------------------------------------------------------------
	Vector4& Vector4::operator = ( const Vector4& v4 )
	{
		x = v4.x;
		y = v4.y;
		z = v4.z;
		w = v4.w;

		return *this;
	}
	//--------------------------------------------------------------------------------------------------------------
	Vector4& Vector4::operator = ( const Vector3& v3 )
	{
		x = v3.x;
		y = v3.y;
		z = v3.z;
		w = 1.0f;

		return *this;
	}
	//--------------------------------------------------------------------------------------------------------------
	bool Vector4::operator == ( const Vector4& v4 ) const
	{
		if( x != v4.x || y != v4.y || z != v4.z || w != v4.w )
			return false;
		else
			return true;
	}
	//--------------------------------------------------------------------------------------------------------------
	bool Vector4::operator != ( const Vector4& v4 ) const
	{
		if( x != v4.x || y != v4.y || z != v4.z || w != v4.w )
			return true;
		else
			return false;
	}
	//--------------------------------------------------------------------------------------------------------------
	bool Vector4::operator > ( const Vector4& v4 ) const
	{
		if( x < v4.x || y < v4.y || z < v4.z || w < v4.w )
			return false;
		else
			return true;
	}
	//--------------------------------------------------------------------------------------------------------------
	bool Vector4::operator < ( const Vector4& v4 ) const
	{
		if( x > v4.x || y > v4.y || z > v4.z || w > v4.w )
			return false;
		else
			return true;
	}
	//--------------------------------------------------------------------------------------------------------------
	bool Vector4::operator >= ( const Vector4& v4 ) const
	{
		return ( x >= v4.x && y >= v4.y && z >= v4.z && w >= v4.w );
	}
	//--------------------------------------------------------------------------------------------------------------
	bool Vector4::operator <= ( const Vector4& v4 ) const
	{
		return ( x <= v4.x && y <= v4.y && z <= v4.z && w <= v4.w );
	}
	//--------------------------------------------------------------------------------------------------------------
	Vector4 Vector4::operator + () const
	{
		return Vector4( x, y, z, w );
	}
	//--------------------------------------------------------------------------------------------------------------
	Vector4 Vector4::operator - () const
	{
		return Vector4( -x, -y, -z, -w );
	}
	//--------------------------------------------------------------------------------------------------------------
	Vector4 Vector4::operator + ( const Vector4& v4 ) const
	{
		static Vector4 tmp;
		Vec4Add( &tmp, this, &v4 );
		return tmp;
	}
	//--------------------------------------------------------------------------------------------------------------
	Vector4 Vector4::operator - ( const Vector4& v4 ) const
	{
		static Vector4 tmp;
		Vec4Sub( &tmp, this, &v4 );
		return tmp;
	}
	//--------------------------------------------------------------------------------------------------------------
	Vector4 Vector4::operator * ( const Vector4& v4 ) const
	{
		static Vector4 tmp;
		Vec4Mul( &tmp, this, &v4 );
		return tmp;
	}
	//--------------------------------------------------------------------------------------------------------------
	Vector4 Vector4::operator / ( const Vector4& v4 ) const
	{
		static Vector4 tmp;
		Vec4Div( &tmp, this, &v4 );
		return tmp;
	}
	//--------------------------------------------------------------------------------------------------------------
	Vector4 Vector4::operator * ( const float fScalar ) const
	{
		static Vector4 tmp;
		Vec4MulScalar( &tmp, this, fScalar );
		return tmp;
	}
	//--------------------------------------------------------------------------------------------------------------
	Vector4 Vector4::operator / ( const float fScalar ) const
	{
		static Vector4 tmp;
		Vec4DivScalar( &tmp, this, fScalar );
		return tmp;
	}
	//--------------------------------------------------------------------------------------------------------------
	Vector4 Vector4::operator * ( const Matrix4& mat4ref ) const
	{
		static Vector4 tmp;
		Mat4TransVec4( &tmp, &mat4ref, this );
		return tmp;
	}
	//--------------------------------------------------------------------------------------------------------------
	void Vector4::operator += ( const Vector4& v4 )
	{
		Vec4Add( this, this, &v4 );
	}
	//--------------------------------------------------------------------------------------------------------------
	void Vector4::operator -= ( const Vector4& v4 )
	{
		Vec4Sub( this, this, &v4 );
	}
	//--------------------------------------------------------------------------------------------------------------
	void Vector4::operator *= ( const Vector4& v4 )
	{
		Vec4Mul( this, this, &v4 );
	}
	//--------------------------------------------------------------------------------------------------------------
	void Vector4::operator /= ( const Vector4& v4 )
	{
		Vec4Div( this, this, &v4 );
	}
	//--------------------------------------------------------------------------------------------------------------
	void Vector4::operator *= ( const float fScalar )
	{
		Vec4MulScalar( this, this, fScalar );
	}
	//--------------------------------------------------------------------------------------------------------------
	void Vector4::operator /= ( const float fScalar )
	{
		Vec4DivScalar( this, this, fScalar );
	}
	//--------------------------------------------------------------------------------------------------------------
	void Vector4::operator *= ( const Matrix4& mat4ref )
	{
		static Vector4 tmp;
		Mat4TransVec4( &tmp, &mat4ref, this );
		
		x = tmp.x;
		y = tmp.y;
		z = tmp.z;
		w = tmp.w;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取该矢量与另一矢量的点乘积
	float Vector4::GetDot( const Vector4& v4 ) const
	{
		return Vec4Dot( this, &v4 );
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取该矢量的长度
	float Vector4::GetLength() const
	{
		return Vec4Length( this );
	}
	//--------------------------------------------------------------------------------------------------------------
	//单位化该矢量
	void Vector4::Normalize()
	{
		Vec4Normalize( this, this );
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取该矢量的单位化矢量
	Vector4 Vector4::GetNormalizedVec() const
	{
		static Vector4 tmp;
		Vec4Normalize( &tmp, this );
		return tmp;
	}
	//--------------------------------------------------------------------------------------------------------------
	//快速单位化该矢量（需要传入该矢量的单位长度）
	void Vector4::NormalizeFast( const float fLength )
	{
		Vec4NormalizeFast( this, this, fLength );
	}
	//--------------------------------------------------------------------------------------------------------------
	//快速获取该矢量的单位化矢量（需要传入该矢量的单位长度）
	Vector4 Vector4::GetNormalizedVecFast( const float fLength ) const
	{
		static Vector4 tmp;
		Vec4NormalizeFast( &tmp, this, fLength );
		return tmp;
	}
	//--------------------------------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------------------------------
	//默认四维矢量数学计算函数
	//--------------------------------------------------------------------------------------------------------------
	void FKFastcall _Def_Vec4Add( Vector4* pOut, const Vector4* pArg1, const Vector4* pArg2 )
	{
		pOut->x = pArg1->x + pArg2->x;
		pOut->y = pArg1->y + pArg2->y;
		pOut->z = pArg1->z + pArg2->z;
		pOut->w = pArg1->w + pArg2->w;
	}
	//--------------------------------------------------------------------------------------------------------------
	void FKFastcall _Def_Vec4Sub( Vector4* pOut, const Vector4* pOrg, const Vector4* pSub )
	{
		pOut->x = pOrg->x - pSub->x;
		pOut->y = pOrg->y - pSub->y;
		pOut->z = pOrg->z - pSub->z;
		pOut->w = pOrg->w - pSub->w;
	}
	//--------------------------------------------------------------------------------------------------------------
	void FKFastcall _Def_Vec4Mul( Vector4* pOut, const Vector4* pArg1, const Vector4* pArg2 )
	{
		pOut->x = pArg1->x * pArg2->x;
		pOut->y = pArg1->y * pArg2->y;
		pOut->z = pArg1->z * pArg2->z;
		pOut->w = pArg1->w * pArg2->w;
	}
	//--------------------------------------------------------------------------------------------------------------
	void FKFastcall _Def_Vec4Div( Vector4* pOut, const Vector4* pOrg, const Vector4* pDiv )
	{
		pOut->x = pOrg->x / pDiv->x;
		pOut->y = pOrg->y / pDiv->y;
		pOut->z = pOrg->z / pDiv->z;
		pOut->w = pOrg->w / pDiv->w;
	}
	//--------------------------------------------------------------------------------------------------------------
	void FKFastcall _Def_Vec4MulScalar( Vector4* pOut, const Vector4* pOrg, const float fScalar )
	{
		pOut->x = pOrg->x * fScalar;
		pOut->y = pOrg->y * fScalar;
		pOut->z = pOrg->z * fScalar;
		pOut->w = pOrg->w * fScalar;
	}
	//--------------------------------------------------------------------------------------------------------------
	void FKFastcall _Def_Vec4DivScalar( Vector4* pOut, const Vector4* pOrg, const float fScalar )
	{
		if( fScalar != 0.0f )
		{
			float fInv = 1.0f / fScalar;
			pOut->x = pOrg->x * fInv;
			pOut->y = pOrg->y * fInv;
			pOut->z = pOrg->z * fInv;
			pOut->w = pOrg->w * fInv;
		}
		else
		{
			pOut->x = 0.0f;
			pOut->y = 0.0f;
			pOut->z = 0.0f;
			pOut->w = 0.0f;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	float FKFastcall _Def_Vec4Dot( const Vector4* pArg1, const Vector4* pArg2 )
	{
		return ( pArg1->x * pArg2->x + pArg1->y * pArg2->y + pArg1->z * pArg2->z + pArg1->w * pArg2->w );
	}
	//--------------------------------------------------------------------------------------------------------------
	float FKFastcall _Def_Vec4Length( const Vector4* pIn )
	{
		return Sqrt( pIn->x * pIn->x + pIn->y * pIn->y + pIn->z * pIn->z + pIn->w * pIn->w );
	}
	//--------------------------------------------------------------------------------------------------------------
	void FKFastcall _Def_Vec4Normalize( Vector4* pOut, const Vector4* pIn )
	{
		float fRadius = Sqrt( pIn->x * pIn->x + pIn->y * pIn->y + pIn->z * pIn->z + pIn->w * pIn->w );
		if( fRadius != 0.0f )
		{
			fRadius = 1.0 / fRadius;

			pOut->x = pIn->x * fRadius;
			pOut->y = pIn->y * fRadius;
			pOut->z = pIn->z * fRadius;
			pOut->w = pIn->w * fRadius;
		}
		else
		{
			pOut->x = 0.0f;
			pOut->y = 0.0f;
			pOut->z = 0.0f;
			pOut->w = 0.0f;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	void FKFastcall _Def_Vec4NormalizeFast( Vector4* pOut, const Vector4* pIn, float fLength )
	{
		if( fLength != 0.0f )
		{
			fLength = 1.0 / fLength;

			pOut->x = pIn->x * fLength;
			pOut->y = pIn->y * fLength;
			pOut->z = pIn->z * fLength;
			pOut->w = pIn->w * fLength;
		}
		else
		{
			pOut->x = 0.0f;
			pOut->y = 0.0f;
			pOut->z = 0.0f;
			pOut->w = 0.0f;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
