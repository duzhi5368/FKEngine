/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Matrix3
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	数学库 3x3 矩阵
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	// 3x3 矩阵
	class FK_DLL FKAlign16 Matrix3
	{
	public:
		union
		{
			struct
			{
				float _11, _12, _13;
				float _21, _22, _23;
				float _31, _32, _33;
			};
			float m[3][3];
			float _m[9];
		};

	public:
		//常用值声明
		static Matrix3 Identity;
		static Matrix3 Zero;

	public:
		//不同的构造函数
		Matrix3();
		Matrix3( const Matrix3& mat3ref );
		Matrix3( const Matrix4& mat4ref );
		Matrix3(	const float f11, const float f12, const float f13,
							const float f21, const float f22, const float f23,
							const float f31, const float f32, const float f33 );
		Matrix3( const float pVal[9] );

		//各种运算符的重载
		Matrix3& operator = ( const Matrix3& mat3ref );
		bool operator == ( const Matrix3& mat3ref ) const;
		bool operator != ( const Matrix3& mat3ref ) const;
		Matrix3 operator * ( const Matrix3& mat3ref ) const;
		void operator *= ( const Matrix3& mat3ref );
		Vector3 operator * ( const Vector3& vec3ref ) const;

		//获取矩阵数值描述
		String GetDescription();

		//生成单位矩阵
		void SetIdentity();

		//生成零矩阵
		void SetZero();

		//生成转置矩阵
		void SetTranspose();

		//获取转置矩阵
		Matrix3 GetTranspose() const;

		//生成反矩阵
		void SetInverse();

		//获取反矩阵
		Matrix3 GetInverse() const;

		//根据自定义轴生成旋转矩阵
		void SetRotationAxis( const float Degree, const Vector3& Axis );

		//根据四元数生成旋转矩阵
		void SetRotationQuaternion( const Quaternion& Quat );

		//根据欧拉角生成旋转矩阵
		void SetRotationPitchYawRoll( const float Pitch, const float Yaw, const float Roll );

		//生成绕 X 轴旋转矩阵
		void SetRotationX( const float Degree );

		//生成绕 Y 轴旋转矩阵
		void SetRotationY( const float Degree );

		//生成绕 Z 轴旋转矩阵
		void SetRotationZ( const float Degree );

		//生成缩放矩阵
		void SetScaling( const float sx, const float sy, const float sz );

		//生成缩放矩阵
		void SetScaling( const Vector3& Scl );
	};
	//--------------------------------------------------------------------------------------------------------------
	//汇编优化 3x3 矩阵数学计算函数指针
	extern void		( FKFastcall *Mat3TransMat3 )	( Matrix3* pOut, const Matrix3* pMat1, const Matrix3* pMat2 );
	extern void		( FKFastcall *Mat3TransVec3 )	( Vector3* pOutV3, const Matrix3* pMat3, const Vector3* pInV3 );
	extern void		( FKFastcall *Mat3Inverse )		( Matrix3* pOut, const Matrix3* pIn );
	extern void		( FKFastcall *Mat3Transpose )	( Matrix3* pOut, const Matrix3* pIn );
	//--------------------------------------------------------------------------------------------------------------
	//默认3x3 矩阵数学计算函数
	extern void FKFastcall _Def_Mat3TransMat3( Matrix3* pOut, const Matrix3* pMat1, const Matrix3* pMat2 );
	extern void FKFastcall _Def_Mat3TransVec3( Vector3* pOutV3, const Matrix3* pMat3, const Vector3* pInV3 );
	extern void FKFastcall _Def_Mat3Inverse( Matrix3* pOut, const Matrix3* pIn );
	extern void	FKFastcall _Def_Mat3Transpose( Matrix3* pOut, const Matrix3* pIn );
	//--------------------------------------------------------------------------------------------------------------
	typedef Stack< Matrix3 >		Matrix3Stack;
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
