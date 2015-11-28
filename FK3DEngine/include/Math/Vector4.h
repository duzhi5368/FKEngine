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
*	文件说明：	数学库 四维矢量
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//四维矢量
	class FK_DLL Vector4
	{
	public:
		union
		{
			struct
			{
				float x;
				float y;
				float z;
				float w;
			};
			float v[4];
		};

	public:
		//常用值声明
		static Vector4 Identity;
		static Vector4 Zero;
		static Vector4 UnitX;
		static Vector4 UnitY;
		static Vector4 UnitZ;

	public:
		//不同的构造函数
		Vector4();
		Vector4( const Vector4& v4 );
		Vector4( const Vector3& v3 );
		Vector4( const float fx, const float fy, const float fz, const float fw );
		Vector4( const float pVal[4] );

		//各种运算符的重载
		Vector4& operator = ( const Vector4& v4 );
		Vector4& operator = ( const Vector3& v3 );
		bool operator == ( const Vector4& v4 ) const;
		bool operator != ( const Vector4& v4 ) const;
		bool operator > ( const Vector4& v4 ) const;
		bool operator < ( const Vector4& v4 ) const;
		bool operator >= ( const Vector4& v4 ) const;
		bool operator <= ( const Vector4& v4 ) const;
		Vector4 operator + () const;
		Vector4 operator - () const;
		Vector4 operator + ( const Vector4& v4 ) const;
		Vector4 operator - ( const Vector4& v4 ) const;
		Vector4 operator * ( const Vector4& v4 ) const;
		Vector4 operator / ( const Vector4& v4 ) const;
		Vector4 operator * ( const float fScalar ) const;
		Vector4 operator / ( const float fScalar ) const;
		Vector4 operator * ( const Matrix4& mat4ref ) const;
		void operator += ( const Vector4& v4 );
		void operator -= ( const Vector4& v4 );
		void operator *= ( const Vector4& v4 );
		void operator /= ( const Vector4& v4 );
		void operator *= ( const float fScalar );
		void operator /= ( const float fScalar );
		void operator *= ( const Matrix4& mat4ref );

		//获取该矢量与另一矢量的点乘积
		float GetDot( const Vector4& v4 ) const;

		//获取该矢量的长度
		float GetLength() const;

		//单位化该矢量
		void Normalize();

		//获取该矢量的单位化矢量
		Vector4 GetNormalizedVec() const;

		//快速单位化该矢量（需要传入该矢量的单位长度）
		void NormalizeFast( const float fLength );

		//快速获取该矢量的单位化矢量（需要传入该矢量的单位长度）
		Vector4 GetNormalizedVecFast( const float fLength ) const;
	};
	//--------------------------------------------------------------------------------------------------------------
	//汇编优化四维矢量数学计算函数指针
	extern void		( FKFastcall *Vec4Add )				( Vector4* pOut, const Vector4* pArg1, const Vector4* pArg2 );
	extern void		( FKFastcall *Vec4Sub )				( Vector4* pOut, const Vector4* pOrg, const Vector4* pSub );
	extern void		( FKFastcall *Vec4Mul )				( Vector4* pOut, const Vector4* pArg1, const Vector4* pArg2 );
	extern void		( FKFastcall *Vec4Div )				( Vector4* pOut, const Vector4* pOrg, const Vector4* pDiv );
	extern void		( FKFastcall *Vec4MulScalar )		( Vector4* pOut, const Vector4* pOrg, const float fScalar );
	extern void		( FKFastcall *Vec4DivScalar )		( Vector4* pOut, const Vector4* pOrg, const float fScalar );

	extern float	( FKFastcall *Vec4Dot )				( const Vector4* pArg1, const Vector4* pArg2 );
	extern float	( FKFastcall *Vec4Length )			( const Vector4* pIn );
	extern void		( FKFastcall *Vec4Normalize )		( Vector4* pOut, const Vector4* pIn );
	extern void		( FKFastcall *Vec4NormalizeFast )	( Vector4* pOut, const Vector4* pIn, float fLength );
	//--------------------------------------------------------------------------------------------------------------
	//默认四维矢量数学计算函数
	extern void FKFastcall _Def_Vec4Add( Vector4* pOut, const Vector4* pArg1, const Vector4* pArg2 );
	extern void FKFastcall _Def_Vec4Sub( Vector4* pOut, const Vector4* pOrg, const Vector4* pSub );
	extern void FKFastcall _Def_Vec4Mul( Vector4* pOut, const Vector4* pArg1, const Vector4* pArg2 );
	extern void FKFastcall _Def_Vec4Div( Vector4* pOut, const Vector4* pOrg, const Vector4* pDiv );
	extern void FKFastcall _Def_Vec4MulScalar( Vector4* pOut, const Vector4* pOrg, const float fScalar );
	extern void FKFastcall _Def_Vec4DivScalar( Vector4* pOut, const Vector4* pOrg, const float fScalar );

	extern float FKFastcall _Def_Vec4Dot( const Vector4* pArg1, const Vector4* pArg2 );
	extern float FKFastcall _Def_Vec4Length( const Vector4* pIn );
	extern void FKFastcall _Def_Vec4Normalize( Vector4* pOut, const Vector4* pIn );
	extern void FKFastcall _Def_Vec4NormalizeFast( Vector4* pOut, const Vector4* pIn, float fLength );
	//--------------------------------------------------------------------------------------------------------------
	typedef Stack< Vector4 >		Vector4Stack;
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
