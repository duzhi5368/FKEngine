/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Vector3
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
	//汇编优化三维矢量数学计算函数指针
	//--------------------------------------------------------------------------------------------------------------
	void	( FKFastcall *Vec3Add )				( Vector3* pOut, const Vector3* pArg1, const Vector3* pArg2 )	= _Def_Vec3Add;
	void	( FKFastcall *Vec3Sub )				( Vector3* pOut, const Vector3* pOrg, const Vector3* pSub )		= _Def_Vec3Sub;
	void	( FKFastcall *Vec3Mul )				( Vector3* pOut, const Vector3* pArg1, const Vector3* pArg2 )	= _Def_Vec3Mul;
	void	( FKFastcall *Vec3Div )				( Vector3* pOut, const Vector3* pOrg, const Vector3* pDiv )		= _Def_Vec3Div;
	void	( FKFastcall *Vec3MulScalar )		( Vector3* pOut, const Vector3* pOrg, const float fScalar )		= _Def_Vec3MulScalar;
	void	( FKFastcall *Vec3DivScalar )		( Vector3* pOut, const Vector3* pOrg, const float fScalar )		= _Def_Vec3DivScalar;

	float	( FKFastcall *Vec3Dot )				( const Vector3* pArg1, const Vector3* pArg2 )					= _Def_Vec3Dot;
	void	( FKFastcall *Vec3Cross )			( Vector3* pOut, const Vector3* pArg1, const Vector3* pArg2 )	= _Def_Vec3Cross;
	float	( FKFastcall *Vec3Length )			( const Vector3* pIn )											= _Def_Vec3Length;
	void	( FKFastcall *Vec3Normalize )		( Vector3* pOut, const Vector3* pIn )							= _Def_Vec3Normalize;
	void	( FKFastcall *Vec3NormalizeFast )	( Vector3* pOut, const Vector3* pIn, float fLength )			= _Def_Vec3NormalizeFast;
	//--------------------------------------------------------------------------------------------------------------
	//常用值声明
	//--------------------------------------------------------------------------------------------------------------
	Vector3 Vector3::Identity( 1.0f, 1.0f, 1.0f );
	Vector3 Vector3::Zero( 0.0f, 0.0f, 0.0f );
	Vector3 Vector3::UnitX( 1.0f, 0.0f, 0.0f );
	Vector3 Vector3::UnitY( 0.0f, 1.0f, 0.0f );
	Vector3 Vector3::UnitZ( 0.0f, 0.0f, 1.0f );
	//--------------------------------------------------------------------------------------------------------------
	//不同的构造函数
	//--------------------------------------------------------------------------------------------------------------
	Vector3::Vector3()
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	Vector3::Vector3( const Vector3& v3 )
		: x		(v3.x)
		, y		(v3.y)
		, z		(v3.z)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	Vector3::Vector3( const float fx, const float fy, const float fz )
		: x		(fx)
		, y		(fy)
		, z		(fz)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	Vector3::Vector3( const float pVal[3] )
		: x		(pVal[0])
		, y		(pVal[1])
		, z		(pVal[2])
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	//各种运算符的重载
	//--------------------------------------------------------------------------------------------------------------
	Vector3& Vector3::operator = ( const Vector3& v3 )
	{
		x = v3.x;
		y = v3.y;
		z = v3.z;

		return *this;
	}
	//--------------------------------------------------------------------------------------------------------------
	bool Vector3::operator == ( const Vector3& v3 ) const
	{
		if( x != v3.x || y != v3.y || z != v3.z )
			return false;
		else
			return true;
	}
	//--------------------------------------------------------------------------------------------------------------
	bool Vector3::operator != ( const Vector3& v3 ) const
	{
		if( x != v3.x || y != v3.y || z != v3.z )
			return true;
		else
			return false;
	}
	//--------------------------------------------------------------------------------------------------------------
	bool Vector3::operator > ( const Vector3& v3 ) const
	{
		if( x < v3.x || y < v3.y || z < v3.z )
			return false;
		else
			return true;
	}
	//--------------------------------------------------------------------------------------------------------------
	bool Vector3::operator < ( const Vector3& v3 ) const
	{
		if( x > v3.x || y > v3.y || z > v3.z )
			return false;
		else
			return true;
	}
	//--------------------------------------------------------------------------------------------------------------
	bool Vector3::operator >= ( const Vector3& v3 ) const
	{
		return ( x >= v3.x && y >= v3.y && z >= v3.z );
	}
	//--------------------------------------------------------------------------------------------------------------
	bool Vector3::operator <= ( const Vector3& v3 ) const
	{
		return ( x <= v3.x && y <= v3.y && z <= v3.z );
	}
	//--------------------------------------------------------------------------------------------------------------
	Vector3 Vector3::operator + () const
	{
		return Vector3( x, y, z );
	}
	//--------------------------------------------------------------------------------------------------------------
	Vector3 Vector3::operator - () const
	{
		return Vector3( -x, -y, -z );
	}
	//--------------------------------------------------------------------------------------------------------------
	Vector3 Vector3::operator + ( const Vector3& v3 ) const
	{
		static Vector3 tmp;
		Vec3Add( &tmp, this, &v3 );
		return tmp;
	}
	//--------------------------------------------------------------------------------------------------------------
	Vector3 Vector3::operator - ( const Vector3& v3 ) const
	{
		static Vector3 tmp;
		Vec3Sub( &tmp, this, &v3 );
		return tmp;
	}
	//--------------------------------------------------------------------------------------------------------------
	Vector3 Vector3::operator * ( const Vector3& v3 ) const
	{
		static Vector3 tmp;
		Vec3Mul( &tmp, this, &v3 );
		return tmp;
	}
	//--------------------------------------------------------------------------------------------------------------
	Vector3 Vector3::operator / ( const Vector3& v3 ) const
	{
		static Vector3 tmp;
		Vec3Div( &tmp, this, &v3 );
		return tmp;
	}
	//--------------------------------------------------------------------------------------------------------------
	Vector3 Vector3::operator * ( const float fScalar ) const
	{
		static Vector3 tmp;
		Vec3MulScalar( &tmp, this, fScalar );
		return tmp;
	}
	//--------------------------------------------------------------------------------------------------------------
	Vector3 Vector3::operator / ( const float fScalar ) const
	{
		static Vector3 tmp;
		Vec3DivScalar( &tmp, this, fScalar );
		return tmp;
	}
	//--------------------------------------------------------------------------------------------------------------
	Vector3 Vector3::operator * ( const Matrix4& mat4ref ) const
	{
		static Vector3 tmp;
		Mat4TransVec3( &tmp, &mat4ref, this );
		return tmp;
	}
	//--------------------------------------------------------------------------------------------------------------
	void Vector3::operator += ( const Vector3& v3 )
	{
		Vec3Add( this, this, &v3 );
	}
	//--------------------------------------------------------------------------------------------------------------
	void Vector3::operator -= ( const Vector3& v3 )
	{
		Vec3Sub( this, this, &v3 );
	}
	//--------------------------------------------------------------------------------------------------------------
	void Vector3::operator *= ( const Vector3& v3 )
	{
		Vec3Mul( this, this, &v3 );
	}
	//--------------------------------------------------------------------------------------------------------------
	void Vector3::operator /= ( const Vector3& v3 )
	{
		Vec3Div( this, this, &v3 );
	}
	//--------------------------------------------------------------------------------------------------------------
	void Vector3::operator *= ( const float fScalar )
	{
		Vec3MulScalar( this, this, fScalar );
	}
	//--------------------------------------------------------------------------------------------------------------
	void Vector3::operator /= ( const float fScalar )
	{
		Vec3DivScalar( this, this, fScalar );
	}
	//--------------------------------------------------------------------------------------------------------------
	void Vector3::operator *= ( const Matrix4& mat4ref )
	{
		static Vector3 tmp;
		Mat4TransVec3( &tmp, &mat4ref, this );
		
		x = tmp.x;
		y = tmp.y;
		z = tmp.z;
	}
	//--------------------------------------------------------------------------------------------------------------
	//将矢量与另一矢量叉乘
	void Vector3::Cross( const Vector3& v3 )
	{
		static Vector3 tmp;
		Vec3Cross( &tmp, this, &v3 );
		tmp = *this;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取该矢量与另一矢量的点乘积
	float Vector3::GetDot( const Vector3& v3 ) const
	{
		return Vec3Dot( this, &v3 );
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取该矢量与另一矢量的叉乘积
	Vector3 Vector3::GetCross( const Vector3& v3 ) const
	{
		static Vector3 tmp;
		Vec3Cross( &tmp, this, &v3 );
		return tmp;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取该矢量的长度
	float Vector3::GetLength() const
	{
		return Vec3Length( this );
	}
	//--------------------------------------------------------------------------------------------------------------
	//单位化该矢量
	void Vector3::Normalize()
	{
		Vec3Normalize( this, this );
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取该矢量的单位化矢量
	Vector3 Vector3::GetNormalizedVec() const
	{
		static Vector3 tmp;
		Vec3Normalize( &tmp, this );
		return tmp;
	}
	//--------------------------------------------------------------------------------------------------------------
	//快速单位化该矢量（需要传入该矢量的单位长度）
	void Vector3::NormalizeFast( const float fLenth )
	{
		Vec3NormalizeFast( this, this, fLenth );
	}
	//--------------------------------------------------------------------------------------------------------------
	//快速获取该矢量的单位化矢量（需要传入该矢量的单位长度）
	Vector3 Vector3::GetNormalizedVecFast( const float fLength ) const
	{
		static Vector3 tmp;
		Vec3NormalizeFast( &tmp, this, fLength );
		return tmp;
	}
	//--------------------------------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------------------------------
	//默认三维矢量数学计算函数
	//--------------------------------------------------------------------------------------------------------------
	void FKFastcall _Def_Vec3Add( Vector3* pOut, const Vector3* pArg1, const Vector3* pArg2 )
	{
		pOut->x = pArg1->x + pArg2->x;
		pOut->y = pArg1->y + pArg2->y;
		pOut->z = pArg1->z + pArg2->z;
	}
	//--------------------------------------------------------------------------------------------------------------
	void FKFastcall _Def_Vec3Sub( Vector3* pOut, const Vector3* pOrg, const Vector3* pSub )
	{
		pOut->x = pOrg->x - pSub->x;
		pOut->y = pOrg->y - pSub->y;
		pOut->z = pOrg->z - pSub->z;
	}
	//--------------------------------------------------------------------------------------------------------------
	void FKFastcall _Def_Vec3Mul( Vector3* pOut, const Vector3* pArg1, const Vector3* pArg2 )
	{
		pOut->x = pArg1->x * pArg2->x;
		pOut->y = pArg1->y * pArg2->y;
		pOut->z = pArg1->z * pArg2->z;
	}
	//--------------------------------------------------------------------------------------------------------------
	void FKFastcall _Def_Vec3Div( Vector3* pOut, const Vector3* pOrg, const Vector3* pDiv )
	{
		pOut->x = pOrg->x / pDiv->x;
		pOut->y = pOrg->y / pDiv->y;
		pOut->z = pOrg->z / pDiv->z;
	}
	//--------------------------------------------------------------------------------------------------------------
	void FKFastcall _Def_Vec3MulScalar( Vector3* pOut, const Vector3* pOrg, const float fScalar )
	{
		pOut->x = pOrg->x * fScalar;
		pOut->y = pOrg->y * fScalar;
		pOut->z = pOrg->z * fScalar;
	}
	//--------------------------------------------------------------------------------------------------------------
	void FKFastcall _Def_Vec3DivScalar( Vector3* pOut, const Vector3* pOrg, const float fScalar )
	{
		if( fScalar != 0.0f )
		{
			float fInv = 1.0f / fScalar;
			pOut->x = pOrg->x * fInv;
			pOut->y = pOrg->y * fInv;
			pOut->z = pOrg->z * fInv;
		}
		else
		{
			pOut->x = 0.0f;
			pOut->y = 0.0f;
			pOut->z = 0.0f;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	float FKFastcall _Def_Vec3Dot( const Vector3* pArg1, const Vector3* pArg2 )
	{
		return ( pArg1->x * pArg2->x + pArg1->y * pArg2->y + pArg1->z * pArg2->z );
	}
	//--------------------------------------------------------------------------------------------------------------
	void FKFastcall _Def_Vec3Cross( Vector3* pOut, const Vector3* pArg1, const Vector3* pArg2 )
	{
		pOut->x = pArg1->y * pArg2->z - pArg1->z * pArg2->y;
		pOut->y = pArg1->z * pArg2->x - pArg1->x * pArg2->z;
		pOut->z = pArg1->x * pArg2->y - pArg1->y * pArg2->x;
	}
	//--------------------------------------------------------------------------------------------------------------
	float FKFastcall _Def_Vec3Length( const Vector3* pIn )
	{
		return Sqrt( pIn->x * pIn->x + pIn->y * pIn->y + pIn->z * pIn->z );
	}
	//--------------------------------------------------------------------------------------------------------------
	void FKFastcall _Def_Vec3Normalize( Vector3* pOut, const Vector3* pIn )
	{
		float fRadius = Sqrt( pIn->x * pIn->x + pIn->y * pIn->y + pIn->z * pIn->z );
		if( fRadius != 0.0f )
		{
			fRadius = 1.0 / fRadius;

			pOut->x = pIn->x * fRadius;
			pOut->y = pIn->y * fRadius;
			pOut->z = pIn->z * fRadius;
		}
		else
		{
			pOut->x = 0.0f;
			pOut->y = 0.0f;
			pOut->z = 0.0f;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	void FKFastcall _Def_Vec3NormalizeFast( Vector3* pOut, const Vector3* pIn, float fLength )
	{
		if( fLength != 0.0f )
		{
			fLength = 1.0 / fLength;

			pOut->x = pIn->x * fLength;
			pOut->y = pIn->y * fLength;
			pOut->z = pIn->z * fLength;
		}
		else
		{
			pOut->x = 0.0f;
			pOut->y = 0.0f;
			pOut->z = 0.0f;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
