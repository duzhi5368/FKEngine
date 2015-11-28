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
*	文件说明：	数学库 二维矢量
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//二维矢量
	class FK_DLL Vector2
	{
	public:
		union
		{
			struct
			{
				float x;
				float y;
			};
			float v[2];
		};

	public:
		//常用值声明
		static Vector2 Identity;
		static Vector2 Zero;
		static Vector2 UnitX;
		static Vector2 UnitY;

	public:
		//不同的构造函数
		Vector2();
		Vector2( const Vector2& v2 );
		Vector2( const float fx, const float fy );
		Vector2( const float pVal[2] );

		//各种运算符的重载
		Vector2& operator = ( const Vector2& v2 );
		bool operator == ( const Vector2& v2 ) const;
		bool operator != ( const Vector2& v2 ) const;
		bool operator > ( const Vector2& v2 ) const;
		bool operator < ( const Vector2& v2 ) const;
		bool operator >= ( const Vector2& v2 ) const;
		bool operator <= ( const Vector2& v2 ) const;
		Vector2 operator + () const;
		Vector2 operator - () const;
		Vector2 operator + ( const Vector2& v2 ) const;
		Vector2 operator - ( const Vector2& v2 ) const;
		Vector2 operator * ( const Vector2& v2 ) const;
		Vector2 operator / ( const Vector2& v2 ) const;
		Vector2 operator * ( const float fScalar ) const;
		Vector2 operator / ( const float fScalar ) const;
		void operator += ( const Vector2& v2 );
		void operator -= ( const Vector2& v2 );
		void operator *= ( const Vector2& v2 );
		void operator /= ( const Vector2& v2 );
		void operator *= ( const float fScalar );
		void operator /= ( const float fScalar );

		//获取该矢量与另一矢量的点乘积
		float GetDot( const Vector2& v2 ) const;

		//获取该矢量的长度
		float GetLength() const;

		//单位化该矢量
		void Normalize();

		//获取该矢量的单位化矢量
		Vector2 GetNormalizedVec() const;

		//快速单位化该矢量（需要传入该矢量的单位长度）
		void NormalizeFast( const float fLenth );

		//快速获取该矢量的单位化矢量（需要传入该矢量的单位长度）
		Vector2 GetNormalizedVecFast( const float fLength ) const;
	};
	//--------------------------------------------------------------------------------------------------------------
	//汇编优化二维矢量数学计算函数指针
	extern void		( FKFastcall *Vec2Add )				( Vector2* pOut, const Vector2* pArg1, const Vector2* pArg2 );
	extern void		( FKFastcall *Vec2Sub )				( Vector2* pOut, const Vector2* pOrg, const Vector2* pSub );
	extern void		( FKFastcall *Vec2Mul )				( Vector2* pOut, const Vector2* pArg1, const Vector2* pArg2 );
	extern void		( FKFastcall *Vec2Div )				( Vector2* pOut, const Vector2* pOrg, const Vector2* pDiv );
	extern void		( FKFastcall *Vec2MulScalar )		( Vector2* pOut, const Vector2* pOrg, const float fScalar );
	extern void		( FKFastcall *Vec2DivScalar )		( Vector2* pOut, const Vector2* pOrg, const float fScalar );

	extern float	( FKFastcall *Vec2Dot )				( const Vector2* pArg1, const Vector2* pArg2 );
	extern float	( FKFastcall *Vec2Length )			( const Vector2* pIn );
	extern void		( FKFastcall *Vec2Normalize )		( Vector2* pOut, const Vector2* pIn );
	extern void		( FKFastcall *Vec2NormalizeFast )	( Vector2* pOut, const Vector2* pIn, float fLenth );
	//--------------------------------------------------------------------------------------------------------------
	//默认二维矢量数学计算函数
	extern void FKFastcall _Def_Vec2Add( Vector2* pOut, const Vector2* pArg1, const Vector2* pArg2 );
	extern void FKFastcall _Def_Vec2Sub( Vector2* pOut, const Vector2* pOrg, const Vector2* pSub );
	extern void FKFastcall _Def_Vec2Mul( Vector2* pOut, const Vector2* pArg1, const Vector2* pArg2 );
	extern void FKFastcall _Def_Vec2Div( Vector2* pOut, const Vector2* pOrg, const Vector2* pDiv );
	extern void FKFastcall _Def_Vec2MulScalar( Vector2* pOut, const Vector2* pOrg, const float fScalar );
	extern void FKFastcall _Def_Vec2DivScalar( Vector2* pOut, const Vector2* pOrg, const float fScalar );

	extern float FKFastcall _Def_Vec2Dot( const Vector2* pArg1, const Vector2* pArg2 );
	extern float FKFastcall _Def_Vec2Length( const Vector2* pIn );
	extern void FKFastcall _Def_Vec2Normalize( Vector2* pOut, const Vector2* pIn );
	extern void FKFastcall _Def_Vec2NormalizeFast( Vector2* pOut, const Vector2* pIn, float fLenth );
	//--------------------------------------------------------------------------------------------------------------
	typedef Stack< Vector2 >		Vector2Stack;
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
