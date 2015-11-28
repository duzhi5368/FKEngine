/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Matrix4
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	数学库 4x4 矩阵
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	// 4x4 矩阵
	class FK_DLL FKAlign16 Matrix4
	{
	public:
		union
		{
			struct
			{
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;
			};
			float m[4][4];
			float _m[16];
		};

	public:
		//常用值声明
		static Matrix4 Identity;
		static Matrix4 Zero;

	public:
		//不同的构造函数
		Matrix4();
		Matrix4( const Matrix4& mat4ref );
		Matrix4( const Matrix3& mat3ref );
		Matrix4(	const float f11, const float f12, const float f13, const float f14,
							const float f21, const float f22, const float f23, const float f24,
							const float f31, const float f32, const float f33, const float f34,
							const float f41, const float f42, const float f43, const float f44 );
		Matrix4( const float pVal[16] );

		//各种运算符的重载
		Matrix4& operator = ( const Matrix4& mat4ref );
		bool operator == ( const Matrix4& mat4ref ) const;
		bool operator != ( const Matrix4& mat4ref ) const;
		Matrix4 operator * ( const Matrix4& mat4ref ) const;
		void operator *= ( const Matrix4& mat4ref );
		Vector3 operator * ( const Vector3& vec3ref ) const;
		Vector4 operator * ( const Vector4& vec4ref ) const;

		//获取矩阵数值描述
		String GetDescription();

		//生成单位矩阵
		void SetIdentity();

		//生成零矩阵
		void SetZero();

		//生成转置矩阵
		void SetTranspose();

		//获取转置矩阵
		Matrix4 GetTranspose() const;

		//生成反矩阵
		void SetInverse();

		//获取反矩阵
		Matrix4 GetInverse() const;

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

		//生成平移矩阵
		void SetTranslation( const float x, const float y, const float z );

		//生成平移矩阵
		void SetTranslation( const Vector3& Pos );

		//生成缩放矩阵
		void SetScaling( const float sx, const float sy, const float sz );

		//生成缩放矩阵
		void SetScaling( const Vector3& Scl );

		//生成左手观察矩阵
		void SetLookAtLH( const Vector3& Eye, const Vector3& At, const Vector3& Up );

		//生成右手观察矩阵
		void SetLookAtRH( const Vector3& Eye, const Vector3& At, const Vector3& Up );

		//生成左手正交投影矩阵
		void SetOrthoLH( float Width, float Height, float Near, float Far );

		//生成右手正交投影矩阵
		void SetOrthoRH( float Width, float Height, float Near, float Far );

		//生成左手正交投影矩阵
		void SetOrthoOffCenterLH( float Left, float Right, float Top, float Bottom, float Near, float Far );

		//生成右手正交投影矩阵
		void SetOrthoOffCenterRH( float Left, float Right, float Top, float Bottom, float Near, float Far );

		//生成左手透视投影矩阵
		void SetPerspectiveLH( float Width, float Height, float Near, float Far );

		//生成右手透视投影矩阵
		void SetPerspectiveRH( float Width, float Height, float Near, float Far );

		//生成左手透视投影矩阵
		void SetPerspectiveFovLH( float FOV, float Aspect, float Near, float Far );

		//生成右手透视投影矩阵
		void SetPerspectiveFovRH( float FOV, float Aspect, float Near, float Far );

		//生成左手透视投影矩阵
		void SetPerspectiveOffCenterLH( float Left, float Right, float Top, float Bottom, float Near, float Far );

		//生成右手透视投影矩阵
		void SetPerspectiveOffCenterRH( float Left, float Right, float Top, float Bottom, float Near, float Far );
	};
	//--------------------------------------------------------------------------------------------------------------
	//汇编优化 4x4 矩阵数学计算函数指针
	extern void		( FKFastcall *Mat4TransMat4 )	( Matrix4* pOut, const Matrix4* pMat1, const Matrix4* pMat2 );
	extern void		( FKFastcall *Mat4TransVec3 )	( Vector3* pOutV3, const Matrix4* pMat4, const Vector3* pInV3 );
	extern void		( FKFastcall *Mat4TransVec4 )	( Vector4* pOutV4, const Matrix4* pMat4, const Vector4* pInV4 );
	extern void		( FKFastcall *Mat4Inverse )		( Matrix4* pOut, const Matrix4* pIn );
	extern void		( FKFastcall *Mat4Transpose )	( Matrix4* pOut, const Matrix4* pIn );
	//--------------------------------------------------------------------------------------------------------------
	//默认4x4 矩阵数学计算函数
	extern void FKFastcall _Def_Mat4TransMat4( Matrix4* pOut, const Matrix4* pMat1, const Matrix4* pMat2 );
	extern void FKFastcall _Def_Mat4TransVec3( Vector3* pOutV3, const Matrix4* pMat4, const Vector3* pInV3 );
	extern void FKFastcall _Def_Mat4TransVec4( Vector4* pOutV4, const Matrix4* pMat4, const Vector4* pInV4 );
	extern float _Def_Mat4Inv3x3( float a1, float a2, float a3, float b1, float b2, float b3,
		float c1, float c2, float c3 );
	extern void FKFastcall _Def_Mat4Inverse( Matrix4* pOut, const Matrix4* pIn );
	extern void FKFastcall _Def_Mat4Transpose( Matrix4* pOut, const Matrix4* pIn );
	//--------------------------------------------------------------------------------------------------------------
	typedef Stack< Matrix4 >		Matrix4Stack;
	typedef Stack< Matrix4* >		Matrix4List;
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
