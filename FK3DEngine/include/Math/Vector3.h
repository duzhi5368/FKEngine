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
*	文件说明：	数学库 三维矢量
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//三维矢量
	class FK_DLL Vector3
	{
	public:
		union
		{
			struct
			{
				float x;
				float y;
				float z;
			};
			float v[3];
		};

	public:
		//常用值声明
		static Vector3 Identity;
		static Vector3 Zero;
		static Vector3 UnitX;
		static Vector3 UnitY;
		static Vector3 UnitZ;

	public:
		//不同的构造函数
		Vector3();
		Vector3( const Vector3& v3 );
		Vector3( const float fx, const float fy, const float fz );
		Vector3( const float pVal[3] );

		//各种运算符的重载
		Vector3& operator = ( const Vector3& v3 );
		bool operator == ( const Vector3& v3 ) const;
		bool operator != ( const Vector3& v3 ) const;
		bool operator > ( const Vector3& v3 ) const;
		bool operator < ( const Vector3& v3 ) const;
		bool operator >= ( const Vector3& v3 ) const;
		bool operator <= ( const Vector3& v3 ) const;
		Vector3 operator + () const;
		Vector3 operator - () const;
		Vector3 operator + ( const Vector3& v3 ) const;
		Vector3 operator - ( const Vector3& v3 ) const;
		Vector3 operator * ( const Vector3& v3 ) const;
		Vector3 operator / ( const Vector3& v3 ) const;
		Vector3 operator * ( const float fScalar ) const;
		Vector3 operator / ( const float fScalar ) const;
		Vector3 operator * ( const Matrix4& mat4ref ) const;
		void operator += ( const Vector3& v3 );
		void operator -= ( const Vector3& v3 );
		void operator *= ( const Vector3& v3 );
		void operator /= ( const Vector3& v3 );
		void operator *= ( const float fScalar );
		void operator /= ( const float fScalar );
		void operator *= ( const Matrix4& mat4ref );

		//将矢量与另一矢量叉乘
		void Cross( const Vector3& v3 );

		//获取该矢量与另一矢量的点乘积
		float GetDot( const Vector3& v3 ) const;

		//获取该矢量与另一矢量的叉乘积
		Vector3 GetCross( const Vector3& v3 ) const;

		//获取该矢量的长度
		float GetLength() const;

		//单位化该矢量
		void Normalize();

		//获取该矢量的单位化矢量
		Vector3 GetNormalizedVec() const;

		//快速单位化该矢量（需要传入该矢量的单位长度）
		void NormalizeFast( const float fLenth );

		//快速获取该矢量的单位化矢量（需要传入该矢量的单位长度）
		Vector3 GetNormalizedVecFast( const float fLength ) const;
	};
	//--------------------------------------------------------------------------------------------------------------
	//汇编优化三维矢量数学计算函数指针
	extern void		( FKFastcall *Vec3Add )				( Vector3* pOut, const Vector3* pArg1, const Vector3* pArg2 );
	extern void		( FKFastcall *Vec3Sub )				( Vector3* pOut, const Vector3* pOrg, const Vector3* pSub );
	extern void		( FKFastcall *Vec3Mul )				( Vector3* pOut, const Vector3* pArg1, const Vector3* pArg2 );
	extern void		( FKFastcall *Vec3Div )				( Vector3* pOut, const Vector3* pOrg, const Vector3* pDiv );
	extern void		( FKFastcall *Vec3MulScalar )		( Vector3* pOut, const Vector3* pOrg, const float fScalar );
	extern void		( FKFastcall *Vec3DivScalar )		( Vector3* pOut, const Vector3* pOrg, const float fScalar );

	extern float	( FKFastcall *Vec3Dot )				( const Vector3* pArg1, const Vector3* pArg2 );
	extern void		( FKFastcall *Vec3Cross )			( Vector3* pOut, const Vector3* pArg1, const Vector3* pArg2 );
	extern float	( FKFastcall *Vec3Length )			( const Vector3* pIn );
	extern void		( FKFastcall *Vec3Normalize )		( Vector3* pOut, const Vector3* pIn );
	extern void		( FKFastcall *Vec3NormalizeFast )	( Vector3* pOut, const Vector3* pIn, float fLenth );
	//--------------------------------------------------------------------------------------------------------------
	//默认三维矢量数学计算函数
	extern void FKFastcall _Def_Vec3Add( Vector3* pOut, const Vector3* pArg1, const Vector3* pArg2 );
	extern void FKFastcall _Def_Vec3Sub( Vector3* pOut, const Vector3* pOrg, const Vector3* pSub );
	extern void FKFastcall _Def_Vec3Mul( Vector3* pOut, const Vector3* pArg1, const Vector3* pArg2 );
	extern void FKFastcall _Def_Vec3Div( Vector3* pOut, const Vector3* pOrg, const Vector3* pDiv );
	extern void FKFastcall _Def_Vec3MulScalar( Vector3* pOut, const Vector3* pOrg, const float fScalar );
	extern void FKFastcall _Def_Vec3DivScalar( Vector3* pOut, const Vector3* pOrg, const float fScalar );

	extern float FKFastcall _Def_Vec3Dot( const Vector3* pArg1, const Vector3* pArg2 );
	extern void FKFastcall _Def_Vec3Cross( Vector3* pOut, const Vector3* pArg1, const Vector3* pArg2 );
	extern float FKFastcall _Def_Vec3Length( const Vector3* pIn );
	extern void FKFastcall _Def_Vec3Normalize( Vector3* pOut, const Vector3* pIn );
	extern void FKFastcall _Def_Vec3NormalizeFast( Vector3* pOut, const Vector3* pIn, float fLenth );
	//--------------------------------------------------------------------------------------------------------------
	typedef Stack< Vector3 >		Vector3Stack;
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
