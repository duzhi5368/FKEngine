/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Vector2
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
	//汇编优化二维矢量数学计算函数指针
	//--------------------------------------------------------------------------------------------------------------
	void	( FKFastcall *Vec2Add )				( Vector2* pOut, const Vector2* pArg1, const Vector2* pArg2 )	= _Def_Vec2Add;
	void	( FKFastcall *Vec2Sub )				( Vector2* pOut, const Vector2* pOrg, const Vector2* pSub )		= _Def_Vec2Sub;
	void	( FKFastcall *Vec2Mul )				( Vector2* pOut, const Vector2* pArg1, const Vector2* pArg2 )	= _Def_Vec2Mul;
	void	( FKFastcall *Vec2Div )				( Vector2* pOut, const Vector2* pOrg, const Vector2* pDiv )		= _Def_Vec2Div;
	void	( FKFastcall *Vec2MulScalar )		( Vector2* pOut, const Vector2* pOrg, const float fScalar )		= _Def_Vec2MulScalar;
	void	( FKFastcall *Vec2DivScalar )		( Vector2* pOut, const Vector2* pOrg, const float fScalar )		= _Def_Vec2DivScalar;

	float	( FKFastcall *Vec2Dot )				( const Vector2* pArg1, const Vector2* pArg2 )					= _Def_Vec2Dot;
	float	( FKFastcall *Vec2Length )			( const Vector2* pIn )											= _Def_Vec2Length;
	void	( FKFastcall *Vec2Normalize )		( Vector2* pOut, const Vector2* pIn )							= _Def_Vec2Normalize;
	void	( FKFastcall *Vec2NormalizeFast )	( Vector2* pOut, const Vector2* pIn, float fLength )			= _Def_Vec2NormalizeFast;
	//--------------------------------------------------------------------------------------------------------------
	//常用值声明
	//--------------------------------------------------------------------------------------------------------------
	Vector2 Vector2::Identity( 1.0f, 1.0f );
	Vector2 Vector2::Zero( 0.0f, 0.0f );
	Vector2 Vector2::UnitX( 1.0f, 0.0f );
	Vector2 Vector2::UnitY( 0.0f, 1.0f );
	//--------------------------------------------------------------------------------------------------------------
	//不同的构造函数
	//--------------------------------------------------------------------------------------------------------------
	Vector2::Vector2()
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	Vector2::Vector2( const Vector2& v2 )
		: x		(v2.x)
		, y		(v2.y)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	Vector2::Vector2( const float fx, const float fy )
		: x		(fx)
		, y		(fy)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	Vector2::Vector2( const float pVal[2] )
		: x		(pVal[0])
		, y		(pVal[1])
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	//各种运算符的重载
	//--------------------------------------------------------------------------------------------------------------
	Vector2& Vector2::operator = ( const Vector2& v2 )
	{
		x = v2.x;
		y = v2.y;

		return *this;
	}
	//--------------------------------------------------------------------------------------------------------------
	bool Vector2::operator == ( const Vector2& v2 ) const
	{
		if( x != v2.x || y != v2.y )
			return false;
		else
			return true;
	}
	//--------------------------------------------------------------------------------------------------------------
	bool Vector2::operator != ( const Vector2& v2 ) const
	{
		if( x != v2.x || y != v2.y )
			return true;
		else
			return false;
	}
	//--------------------------------------------------------------------------------------------------------------
	bool Vector2::operator > ( const Vector2& v2 ) const
	{
		if( x < v2.x || y < v2.y )
			return false;
		else
			return true;
	}
	//--------------------------------------------------------------------------------------------------------------
	bool Vector2::operator < ( const Vector2& v2 ) const
	{
		if( x > v2.x || y > v2.y )
			return false;
		else
			return true;
	}
	//--------------------------------------------------------------------------------------------------------------
	bool Vector2::operator >= ( const Vector2& v2 ) const
	{
		return ( x >= v2.x && y >= v2.y );
	}
	//--------------------------------------------------------------------------------------------------------------
	bool Vector2::operator <= ( const Vector2& v2 ) const
	{
		return ( x <= v2.x && y <= v2.y );
	}
	//--------------------------------------------------------------------------------------------------------------
	Vector2 Vector2::operator + () const
	{
		return Vector2( x, y );
	}
	//--------------------------------------------------------------------------------------------------------------
	Vector2 Vector2::operator - () const
	{
		return Vector2( -x, -y );
	}
	//--------------------------------------------------------------------------------------------------------------
	Vector2 Vector2::operator + ( const Vector2& v2 ) const
	{
		static Vector2 tmp;
		Vec2Add( &tmp, this, &v2 );
		return tmp;
	}
	//--------------------------------------------------------------------------------------------------------------
	Vector2 Vector2::operator - ( const Vector2& v2 ) const
	{
		static Vector2 tmp;
		Vec2Sub( &tmp, this, &v2 );
		return tmp;
	}
	//--------------------------------------------------------------------------------------------------------------
	Vector2 Vector2::operator * ( const Vector2& v2 ) const
	{
		static Vector2 tmp;
		Vec2Mul( &tmp, this, &v2 );
		return tmp;
	}
	//--------------------------------------------------------------------------------------------------------------
	Vector2 Vector2::operator / ( const Vector2& v2 ) const
	{
		static Vector2 tmp;
		Vec2Div( &tmp, this, &v2 );
		return tmp;
	}
	//--------------------------------------------------------------------------------------------------------------
	Vector2 Vector2::operator * ( const float fScalar ) const
	{
		static Vector2 tmp;
		Vec2MulScalar( &tmp, this, fScalar );
		return tmp;
	}
	//--------------------------------------------------------------------------------------------------------------
	Vector2 Vector2::operator / ( const float fScalar ) const
	{
		static Vector2 tmp;
		Vec2DivScalar( &tmp, this, fScalar );
		return tmp;
	}
	//--------------------------------------------------------------------------------------------------------------
	void Vector2::operator += ( const Vector2& v2 )
	{
		Vec2Add( this, this, &v2 );
	}
	//--------------------------------------------------------------------------------------------------------------
	void Vector2::operator -= ( const Vector2& v2 )
	{
		Vec2Sub( this, this, &v2 );
	}
	//--------------------------------------------------------------------------------------------------------------
	void Vector2::operator *= ( const Vector2& v2 )
	{
		Vec2Mul( this, this, &v2 );
	}
	//--------------------------------------------------------------------------------------------------------------
	void Vector2::operator /= ( const Vector2& v2 )
	{
		Vec2Div( this, this, &v2 );
	}
	//--------------------------------------------------------------------------------------------------------------
	void Vector2::operator *= ( const float fScalar )
	{
		Vec2MulScalar( this, this, fScalar );
	}
	//--------------------------------------------------------------------------------------------------------------
	void Vector2::operator /= ( const float fScalar )
	{
		Vec2DivScalar( this, this, fScalar );
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取该矢量与另一矢量的点乘积
	float Vector2::GetDot( const Vector2& v2 ) const
	{
		return Vec2Dot( this, &v2 );
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取该矢量的长度
	float Vector2::GetLength() const
	{
		return Vec2Length( this );
	}
	//--------------------------------------------------------------------------------------------------------------
	//单位化该矢量
	void Vector2::Normalize()
	{
		Vec2Normalize( this, this );
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取该矢量的单位化矢量
	Vector2 Vector2::GetNormalizedVec() const
	{
		static Vector2 tmp;
		Vec2Normalize( &tmp, this );
		return tmp;
	}
	//--------------------------------------------------------------------------------------------------------------
	//快速单位化该矢量（需要传入该矢量的单位长度）
	void Vector2::NormalizeFast( const float fLenth )
	{
		Vec2NormalizeFast( this, this, fLenth );
	}
	//--------------------------------------------------------------------------------------------------------------
	//快速获取该矢量的单位化矢量（需要传入该矢量的单位长度）
	Vector2 Vector2::GetNormalizedVecFast( const float fLength ) const
	{
		static Vector2 tmp;
		Vec2NormalizeFast( &tmp, this, fLength );
		return tmp;
	}
	//--------------------------------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------------------------------
	//默认二维矢量数学计算函数
	//--------------------------------------------------------------------------------------------------------------
	void FKFastcall _Def_Vec2Add( Vector2* pOut, const Vector2* pArg1, const Vector2* pArg2 )
	{
		pOut->x = pArg1->x + pArg2->x;
		pOut->y = pArg1->y + pArg2->y;
	}
	//--------------------------------------------------------------------------------------------------------------
	void FKFastcall _Def_Vec2Sub( Vector2* pOut, const Vector2* pOrg, const Vector2* pSub )
	{
		pOut->x = pOrg->x - pSub->x;
		pOut->y = pOrg->y - pSub->y;
	}
	//--------------------------------------------------------------------------------------------------------------
	void FKFastcall _Def_Vec2Mul( Vector2* pOut, const Vector2* pArg1, const Vector2* pArg2 )
	{
		pOut->x = pArg1->x * pArg2->x;
		pOut->y = pArg1->y * pArg2->y;
	}
	//--------------------------------------------------------------------------------------------------------------
	void FKFastcall _Def_Vec2Div( Vector2* pOut, const Vector2* pOrg, const Vector2* pDiv )
	{
		pOut->x = pOrg->x / pDiv->x;
		pOut->y = pOrg->y / pDiv->y;
	}
	//--------------------------------------------------------------------------------------------------------------
	void FKFastcall _Def_Vec2MulScalar( Vector2* pOut, const Vector2* pOrg, const float fScalar )
	{
		pOut->x = pOrg->x * fScalar;
		pOut->y = pOrg->y * fScalar;
	}
	//--------------------------------------------------------------------------------------------------------------
	void FKFastcall _Def_Vec2DivScalar( Vector2* pOut, const Vector2* pOrg, const float fScalar )
	{
		if( fScalar != 0.0f )
		{
			float fInv = 1.0f / fScalar;
			pOut->x = pOrg->x * fInv;
			pOut->y = pOrg->y * fInv;
		}
		else
		{
			pOut->x = 0.0f;
			pOut->y = 0.0f;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	float FKFastcall _Def_Vec2Dot( const Vector2* pArg1, const Vector2* pArg2 )
	{
		return ( pArg1->x * pArg2->x + pArg1->y * pArg2->y );
	}
	//--------------------------------------------------------------------------------------------------------------
	float FKFastcall _Def_Vec2Length( const Vector2* pIn )
	{
		return Sqrt( pIn->x * pIn->x + pIn->y * pIn->y );
	}
	//--------------------------------------------------------------------------------------------------------------
	void FKFastcall _Def_Vec2Normalize( Vector2* pOut, const Vector2* pIn )
	{
		float fRadius = Sqrt( pIn->x * pIn->x + pIn->y * pIn->y );
		if( fRadius != 0.0f )
		{
			fRadius = 1.0 / fRadius;

			pOut->x = pIn->x * fRadius;
			pOut->y = pIn->y * fRadius;
		}
		else
		{
			pOut->x = 0.0f;
			pOut->y = 0.0f;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	void FKFastcall _Def_Vec2NormalizeFast( Vector2* pOut, const Vector2* pIn, float fLength )
	{
		if( fLength != 0.0f )
		{
			fLength = 1.0 / fLength;

			pOut->x = pIn->x * fLength;
			pOut->y = pIn->y * fLength;
		}
		else
		{
			pOut->x = 0.0f;
			pOut->y = 0.0f;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
