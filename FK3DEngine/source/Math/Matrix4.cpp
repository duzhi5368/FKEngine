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
	//汇编优化 4x4 矩阵数学计算函数指针
	void ( FKFastcall *Mat4TransMat4 )		( Matrix4* pOut, const Matrix4* pMat1, const Matrix4* pMat2 )	= _Def_Mat4TransMat4;
	void ( FKFastcall *Mat4TransVec3 )		( Vector3* pOutV3, const Matrix4* pMat4, const Vector3* pInV3 )	= _Def_Mat4TransVec3;
	void ( FKFastcall *Mat4TransVec4 )		( Vector4* pOutV4, const Matrix4* pMat4, const Vector4* pInV4 )	= _Def_Mat4TransVec4;
	void ( FKFastcall *Mat4Inverse )		( Matrix4* pOut, const Matrix4* pIn )							= _Def_Mat4Inverse;
	void ( FKFastcall *Mat4Transpose )		( Matrix4* pOut, const Matrix4* pIn )							= _Def_Mat4Transpose;
	//--------------------------------------------------------------------------------------------------------------
	//常用值声明
	//--------------------------------------------------------------------------------------------------------------
	Matrix4 Matrix4::Identity(	1.0f, 0.0f, 0.0f, 0.0f,
								0.0f, 1.0f, 0.0f, 0.0f,
								0.0f, 0.0f, 1.0f, 0.0f,
								0.0f, 0.0f, 0.0f, 1.0f );
	Matrix4 Matrix4::Zero(	0.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f, 0.0f );
	//--------------------------------------------------------------------------------------------------------------
	//不同的构造函数
	//--------------------------------------------------------------------------------------------------------------
	Matrix4::Matrix4()
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	Matrix4::Matrix4( const Matrix4& mat4ref )
	{
		memcpy( _m, mat4ref._m, sizeof(float)*16 );
	}
	//--------------------------------------------------------------------------------------------------------------
	Matrix4::Matrix4( const Matrix3& mat3ref )
	{
		_11 = mat3ref._11;		_12 = mat3ref._12;		_13 = mat3ref._13;		_14 = 0.0f;
		_21 = mat3ref._21;		_22 = mat3ref._22;		_23 = mat3ref._23;		_24 = 0.0f;
		_31 = mat3ref._31;		_32 = mat3ref._32;		_33 = mat3ref._33;		_34 = 0.0f;
		_41 = 0.0f;				_42 = 0.0f;				_43 = 0.0f;				_44 = 1.0f;
	}
	//--------------------------------------------------------------------------------------------------------------
	Matrix4::Matrix4(	const float f11, const float f12, const float f13, const float f14,
								const float f21, const float f22, const float f23, const float f24,
								const float f31, const float f32, const float f33, const float f34,
								const float f41, const float f42, const float f43, const float f44 )
		: _11(f11), _12(f12), _13(f13), _14(f14)
		, _21(f21), _22(f22), _23(f23), _24(f24)
		, _31(f31), _32(f32), _33(f33), _34(f34)
		, _41(f41), _42(f42), _43(f43), _44(f44)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	Matrix4::Matrix4( const float pVal[16] )
	{
		memcpy( _m, pVal, sizeof(float)*16 );
	}
	//--------------------------------------------------------------------------------------------------------------
	//各种运算符的重载
	//--------------------------------------------------------------------------------------------------------------
	Matrix4& Matrix4::operator = ( const Matrix4& mat4ref )
	{
		memcpy( this->_m, mat4ref._m, sizeof(float)*16 );
		return *this;
	}
	//--------------------------------------------------------------------------------------------------------------
	bool Matrix4::operator == ( const Matrix4& mat4ref ) const
	{
		if( 0 == memcmp( this->_m, mat4ref._m, sizeof(float)*16 ) )
			return true;
		else
			return false;
	}
	//--------------------------------------------------------------------------------------------------------------
	bool Matrix4::operator != ( const Matrix4& mat4ref ) const
	{
		if( 0 != memcmp( this->_m, mat4ref._m, sizeof(float)*16 ) )
			return true;
		else
			return false;
	}
	//--------------------------------------------------------------------------------------------------------------
	Matrix4 Matrix4::operator * ( const Matrix4& mat4ref ) const
	{
		static Matrix4 tmp;
		Mat4TransMat4( &tmp, this, &mat4ref );
		return tmp;
	}
	//--------------------------------------------------------------------------------------------------------------
	void Matrix4::operator *= ( const Matrix4& mat4ref )
	{
		static Matrix4 tmp;
		Mat4TransMat4( &tmp, this, &mat4ref );
		*this = tmp;
	}
	//--------------------------------------------------------------------------------------------------------------
	Vector3 Matrix4::operator * ( const Vector3& vec3ref ) const
	{
		static Vector3 tmp;
		Mat4TransVec3( &tmp, this, &vec3ref );
		return tmp;
	}
	//--------------------------------------------------------------------------------------------------------------
	Vector4 Matrix4::operator * ( const Vector4& vec4ref ) const
	{
		static Vector4 tmp;
		Mat4TransVec4( &tmp, this, &vec4ref );
		return tmp;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取矩阵数值描述
	String Matrix4::GetDescription()
	{
		int nLen = 0;
		char* pStr = Misc::FormatString( &nLen,
			"\nMatrix4:\n"
			"11 %f\t12 %f\t13 %f\t14 %f\n"
			"21 %f\t22 %f\t23 %f\t24 %f\n"
			"31 %f\t32 %f\t33 %f\t34 %f\n"
			"41 %f\t42 %f\t43 %f\t44 %f\n",
			_11, _12, _13, _14, _21, _22, _23, _24, _31, _32, _33, _34, _41, _42, _43, _44 );

		return String( pStr, (UINT)nLen );
	}
	//--------------------------------------------------------------------------------------------------------------
	//生成单位矩阵
	void Matrix4::SetIdentity()
	{
		*this = Matrix4::Identity;
	}
	//--------------------------------------------------------------------------------------------------------------
	//生成零矩阵
	void Matrix4::SetZero()
	{
		*this = Matrix4::Zero;
	}
	//--------------------------------------------------------------------------------------------------------------
	//生成转置矩阵
	void Matrix4::SetTranspose()
	{
		static Matrix4 tmp;
		Mat4Transpose( &tmp, this );
		*this = tmp;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取转置矩阵
	Matrix4 Matrix4::GetTranspose() const
	{
		static Matrix4 tmp;
		Mat4Transpose( &tmp, this );
		return tmp;
	}
	//--------------------------------------------------------------------------------------------------------------
	//生成反矩阵
	void Matrix4::SetInverse()
	{
		static Matrix4 tmp;
		Mat4Inverse( &tmp, this );
		*this = tmp;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取反矩阵
	Matrix4 Matrix4::GetInverse() const
	{
		static Matrix4 tmp;
		Mat4Inverse( &tmp, this );
		return tmp;
	}
	//--------------------------------------------------------------------------------------------------------------
	//根据自定义轴生成旋转矩阵
	void Matrix4::SetRotationAxis( const float Degree, const Vector3& Axis )
	{
		float w, sin;
		SinCos( FK_DegreeToRadian(Degree) * 0.5, &sin, &w );

		float x = sin * Axis.x;
		float y = sin * Axis.y;
		float z = sin * Axis.z;

		float RMagn = RSqrt( w * w + x * x + y * y + z * z );
		x *= RMagn;
		y *= RMagn;
		z *= RMagn;
		w *= RMagn;

		float x2 = x * 2.0f;
		float y2 = y * 2.0f;
		float z2 = z * 2.0f;
		float wx = x2 * w, wy = y2 * w, wz = z2 * w;
		float xx = x2 * x, xy = y2 * x, xz = z2 * x;
		float yy = y2 * y, yz = y2 * z, zz = z2 * z;

		_11 = 1.0f - yy - zz;
		_12 = xy - wz;
		_13 = xz + wy;
		_14 = 0.0f;

		_21 = xy + wz;
		_22 = 1.0f - xx - zz;
		_23 = yz - wx;
		_24 = 0.0f;

		_31 = xz - wy;
		_32 = yz + wx;
		_33 = 1.0f - xx - yy;
		_34 = 0.0f;

		_41 = 0.0f;
		_42 = 0.0f;
		_43 = 0.0f;
		_44 = 1.0f;
	}
	//--------------------------------------------------------------------------------------------------------------
	//根据四元数生成旋转矩阵
	void Matrix4::SetRotationQuaternion( const Quaternion& Quat )
	{
		float x2 = Quat.x * 2.0f;
		float y2 = Quat.y * 2.0f;
		float z2 = Quat.z * 2.0f;
		float wx = x2 * Quat.w, wy = y2 * Quat.w, wz = z2 * Quat.w;
		float xx = x2 * Quat.x, xy = y2 * Quat.x, xz = z2 * Quat.x;
		float yy = y2 * Quat.y, yz = y2 * Quat.z, zz = z2 * Quat.z;

		_11 = 1.0f - yy - zz;
		_12 = xy - wz;
		_13 = xz + wy;
		_14 = 0.0f;

		_21 = xy + wz;
		_22 = 1.0f - xx - zz;
		_23 = yz - wx;
		_24 = 0.0f;

		_31 = xz - wy;
		_32 = yz + wx;
		_33 = 1.0f - xx - yy;
		_34 = 0.0f;

		_41 = 0.0f;
		_42 = 0.0f;
		_43 = 0.0f;
		_44 = 1.0f;
	}
	//--------------------------------------------------------------------------------------------------------------
	//根据欧拉角生成旋转矩阵
	void Matrix4::SetRotationPitchYawRoll( const float Pitch, const float Yaw, const float Roll )
	{
		float A, B, C, D, E, F;
		SinCos( FK_DegreeToRadian(Pitch),	&B, &A );
		SinCos( FK_DegreeToRadian(Yaw),		&D, &C );
		SinCos( FK_DegreeToRadian(Roll),	&F, &E );

		float AD = A * D;
		float BD = B * D;

		_11 =  C * E;
		_12 = -C * F;
		_13 = -D;
		_14 = 0.0f;

		_21 = -BD * E + A * F;
		_22 =  BD * F + A * E;
		_23 = -B * C;
		_24 = 0.0f;

		_31 =  AD * E + B * F;
		_32 = -AD * F + B * E;
		_33 =  A * C;
		_34 = 0.0f;

		_41 = 0.0f;
		_42 = 0.0f;
		_43 = 0.0f;
		_44 = 1.0f;
	}
	//--------------------------------------------------------------------------------------------------------------
	//生成绕 X 轴旋转矩阵
	void Matrix4::SetRotationX( const float Degree )
	{
		float sin, cos;
		SinCos( FK_DegreeToRadian(Degree), &sin, &cos );

		_11 = 1.0f;		_12 = 0.0f;		_13 = 0.0f;		_14 = 0.0f;
		_21 = 0.0f;		_22 = cos;		_23 = sin;		_24 = 0.0f;
		_31 = 0.0f;		_32 = -sin;		_33 = cos;		_34 = 0.0f;
		_41 = 0.0f;		_42 = 0.0f;		_43 = 0.0f;		_44 = 1.0f;
	}
	//--------------------------------------------------------------------------------------------------------------
	//生成绕 Y 轴旋转矩阵
	void Matrix4::SetRotationY( const float Degree )
	{
		float sin, cos;
		SinCos( FK_DegreeToRadian(Degree), &sin, &cos );

		_11 = cos;		_12 = 0.0f;		_13 = -sin;		_14 = 0.0f;
		_21 = 0.0f;		_22 = 1.0f;		_23 = 0.0f;		_24 = 0.0f;
		_31 = sin;		_32 = 0.0f;		_33 = cos;		_34 = 0.0f;
		_41 = 0.0f;		_42 = 0.0f;		_43 = 0.0f;		_44 = 1.0f;
	}
	//--------------------------------------------------------------------------------------------------------------
	//生成绕 Z 轴旋转矩阵
	void Matrix4::SetRotationZ( const float Degree )
	{
		float sin, cos;
		SinCos( FK_DegreeToRadian(Degree), &sin, &cos );

		_11 = cos;		_12 = sin;		_13 = 0.0f;		_14 = 0.0f;
		_21 = -sin;		_22 = cos;		_23 = 0.0f;		_24 = 0.0f;
		_31 = 0.0f;		_32 = 0.0f;		_33 = 1.0f;		_34 = 0.0f;
		_41 = 0.0f;		_42 = 0.0f;		_43 = 0.0f;		_44 = 1.0f;
	}
	//--------------------------------------------------------------------------------------------------------------
	//生成平移矩阵
	void Matrix4::SetTranslation( const float x, const float y, const float z )
	{
		_11 = 1.0f;		_12 = 0.0f;		_13 = 0.0f;		_14 = 0.0f;
		_21 = 0.0f;		_22 = 1.0f;		_23 = 0.0f;		_24 = 0.0f;
		_31 = 0.0f;		_32 = 0.0f;		_33 = 1.0f;		_34 = 0.0f;
		_41 = x;		_42 = y;		_43 = z;		_44 = 1.0f;
	}
	//--------------------------------------------------------------------------------------------------------------
	//生成平移矩阵
	void Matrix4::SetTranslation( const Vector3& Pos )
	{
		_11 = 1.0f;		_12 = 0.0f;		_13 = 0.0f;		_14 = 0.0f;
		_21 = 0.0f;		_22 = 1.0f;		_23 = 0.0f;		_24 = 0.0f;
		_31 = 0.0f;		_32 = 0.0f;		_33 = 1.0f;		_34 = 0.0f;
		_41 = Pos.x;	_42 = Pos.y;	_43 = Pos.z;	_44 = 1.0f;
	}
	//--------------------------------------------------------------------------------------------------------------
	//生成缩放矩阵
	void Matrix4::SetScaling( const float sx, const float sy, const float sz )
	{
		_11 = sx;		_12 = 0.0f;		_13 = 0.0f;		_14 = 0.0f;
		_21 = 0.0f;		_22 = sy;		_23 = 0.0f;		_24 = 0.0f;
		_31 = 0.0f;		_32 = 0.0f;		_33 = sz;		_34 = 0.0f;
		_41 = 0.0f;		_42 = 0.0f;		_43 = 0.0f;		_44 = 1.0f;
	}
	//--------------------------------------------------------------------------------------------------------------
	//生成缩放矩阵
	void Matrix4::SetScaling( const Vector3& Scl )
	{
		_11 = Scl.x;	_12 = 0.0f;		_13 = 0.0f;		_14 = 0.0f;
		_21 = 0.0f;		_22 = Scl.y;	_23 = 0.0f;		_24 = 0.0f;
		_31 = 0.0f;		_32 = 0.0f;		_33 = Scl.z;	_34 = 0.0f;
		_41 = 0.0f;		_42 = 0.0f;		_43 = 0.0f;		_44 = 1.0f;
	}
	//--------------------------------------------------------------------------------------------------------------
	//生成左手观察矩阵
	void Matrix4::SetLookAtLH( const Vector3& Eye, const Vector3& At, const Vector3& Up )
	{
		Vector3 zAxis = At - Eye;
		zAxis.Normalize();

		Vector3 xAxis = Up.GetCross( zAxis );
		xAxis.Normalize();

		Vector3 yAxis = zAxis.GetCross( xAxis );

		_11 = xAxis.x;		_12 = yAxis.x;		_13 = zAxis.x;		_14 = 0.0f;
		_21 = xAxis.y;		_22 = yAxis.y;		_23 = zAxis.y;		_24 = 0.0f;
		_31 = xAxis.z;		_32 = yAxis.z;		_33 = zAxis.z;		_34 = 0.0f;

		_41 = -xAxis.GetDot( Eye );
		_42 = -yAxis.GetDot( Eye );
		_43 = -zAxis.GetDot( Eye );
		_44 = 1.0f;
	}
	//--------------------------------------------------------------------------------------------------------------
	//生成右手观察矩阵
	void Matrix4::SetLookAtRH( const Vector3& Eye, const Vector3& At, const Vector3& Up )
	{
		Vector3 zAxis = Eye - At;
		zAxis.Normalize();

		Vector3 xAxis = Up.GetCross( zAxis );
		xAxis.Normalize();

		Vector3 yAxis = zAxis.GetCross( xAxis );

		_11 = xAxis.x;		_12 = yAxis.x;		_13 = zAxis.x;		_14 = 0.0f;
		_21 = xAxis.y;		_22 = yAxis.y;		_23 = zAxis.y;		_24 = 0.0f;
		_31 = xAxis.z;		_32 = yAxis.z;		_33 = zAxis.z;		_34 = 0.0f;

		_41 = -xAxis.GetDot( Eye );
		_42 = -yAxis.GetDot( Eye );
		_43 = -zAxis.GetDot( Eye );
		_44 = 1.0f;
	}
	//--------------------------------------------------------------------------------------------------------------
	//生成左手正交投影矩阵
	void Matrix4::SetOrthoLH( float Width, float Height, float Near, float Far )
	{
		_11 = 2.0f / Width;		_12 = 0.0f;					_13 = 0.0f;					_14 = 0.0f;
		_21 = 0.0f;				_22 = 2.0f / Height;		_23 = 0.0f;					_24 = 0.0f;
		_31 = 0.0f;				_32 = 0.0f;					_33 = 1.0f / (Far-Near);	_34 = 0.0f;
		_41 = 0.0f;				_42 = 0.0f;					_43 = Near / (Near-Far);	_44 = 1.0f;
	}
	//--------------------------------------------------------------------------------------------------------------
	//生成右手正交投影矩阵
	void Matrix4::SetOrthoRH( float Width, float Height, float Near, float Far )
	{
		float Depth = Near - Far;

		_11 = 2.0f / Width;		_12 = 0.0f;					_13 = 0.0f;				_14 = 0.0f;
		_21 = 0.0f;				_22 = 2.0f / Height;		_23 = 0.0f;				_24 = 0.0f;
		_31 = 0.0f;				_32 = 0.0f;					_33 = 1.0f / Depth;		_34 = 0.0f;
		_41 = 0.0f;				_42 = 0.0f;					_43 = Near / Depth;		_44 = 1.0f;
	}
	//--------------------------------------------------------------------------------------------------------------
	//生成左手正交投影矩阵
	void Matrix4::SetOrthoOffCenterLH( float Left, float Right, float Top, float Bottom, float Near, float Far )
	{
		_11 = 2.0f / (Right-Left);			_12 = 0.0f;							_13 = 0.0f;					_14 = 0.0f;
		_21 = 0.0f;							_22 = 2.0f / (Top-Bottom);			_23 = 0.0f;					_24 = 0.0f;
		_31 = 0.0f;							_32 = 0.0f;							_33 = 1.0f / (Far-Near);	_34 = 0.0f;
		_41 = (Left+Right) / (Left-Right);	_42 = (Top+Bottom) / (Bottom-Top);	_43 = Near / (Near-Far);	_44 = 1.0f;
	}
	//--------------------------------------------------------------------------------------------------------------
	//生成右手正交投影矩阵
	void Matrix4::SetOrthoOffCenterRH( float Left, float Right, float Top, float Bottom, float Near, float Far )
	{
		float Depth = Near - Far;

		_11 = 2.0f / (Right-Left);			_12 = 0.0f;							_13 = 0.0f;				_14 = 0.0f;
		_21 = 0.0f;							_22 = 2.0f / (Top-Bottom);			_23 = 0.0f;				_24 = 0.0f;
		_31 = 0.0f;							_32 = 0.0f;							_33 = 1.0f / Depth;		_34 = 0.0f;
		_41 = (Left+Right) / (Left-Right);	_42 = (Top+Bottom) / (Bottom-Top);	_43 = Near / Depth;		_44 = 1.0f;
	}
	//--------------------------------------------------------------------------------------------------------------
	//生成左手透视投影矩阵
	void Matrix4::SetPerspectiveLH( float Width, float Height, float Near, float Far )
	{
		_11 = 2.0f * Near / Width;	_12 = 0.0f;						_13 = 0.0f;						_14 = 0.0f;
		_21 = 0.0f;					_22 = 2.0f * Near / Height;		_23 = 0.0f;						_24 = 0.0f;
		_31 = 0.0f;					_32 = 0.0f;						_33 = Far / (Far-Near);			_34 = 1.0f;
		_41 = 0.0f;					_42 = 0.0f;						_43 = Near * Far / (Near-Far);	_44 = 0.0f;
	}
	//--------------------------------------------------------------------------------------------------------------
	//生成右手透视投影矩阵
	void Matrix4::SetPerspectiveRH( float Width, float Height, float Near, float Far )
	{
		float Depth = Near - Far;

		_11 = 2.0f * Near / Width;	_12 = 0.0f;						_13 = 0.0f;					_14 = 0.0f;
		_21 = 0.0f;					_22 = 2.0f * Near / Height;		_23 = 0.0f;					_24 = 0.0f;
		_31 = 0.0f;					_32 = 0.0f;						_33 = Far / Depth;			_34 = 1.0f;
		_41 = 0.0f;					_42 = 0.0f;						_43 = Near * Far / Depth;	_44 = 0.0f;
	}
	//--------------------------------------------------------------------------------------------------------------
	//生成左手透视投影矩阵
	void Matrix4::SetPerspectiveFovLH( float FOV, float Aspect, float Near, float Far )
	{
		float Width = Cot( FK_DegreeToRadian(FOV) * 0.5f );
		float Height = Width / Aspect;
		float Depth = Far - Near;

		_11 = Height;	_12 = 0.0f;		_13 = 0.0f;						_14 = 0.0f;
		_21 = 0.0f;		_22 = Width;	_23 = 0.0f;						_24 = 0.0f;
		_31 = 0.0f;		_32 = 0.0f;		_33 = Far / Depth;				_34 = 1.0f;
		_41 = 0.0f;		_42 = 0.0f;		_43 = -Near * Far / Depth;		_44 = 0.0f;
	}
	//--------------------------------------------------------------------------------------------------------------
	//生成右手透视投影矩阵
	void Matrix4::SetPerspectiveFovRH( float FOV, float Aspect, float Near, float Far )
	{
		float Width = Cot( FK_DegreeToRadian(FOV) * 0.5f );
		float Height = Width / Aspect;
		float Depth = Near - Far;

		_11 = Height;	_12 = 0.0f;		_13 = 0.0f;					_14 = 0.0f;
		_21 = 0.0f;		_22 = Width;	_23 = 0.0f;					_24 = 0.0f;
		_31 = 0.0f;		_32 = 0.0f;		_33 = Far / Depth;			_34 = -1.0f;
		_41 = 0.0f;		_42 = 0.0f;		_43 = Near * Far / Depth;	_44 = 0.0f;
	}
	//--------------------------------------------------------------------------------------------------------------
	//生成左手透视投影矩阵
	void Matrix4::SetPerspectiveOffCenterLH( float Left, float Right, float Top, float Bottom, float Near, float Far )
	{
		_11 = 2.0f * Near / (Right-Left);	_12 = 0.0f;							_13 = 0.0f;						_14 = 0.0f;
		_21 = 0.0f;							_22 = 2.0f * Near / (Top-Bottom);	_23 = 0.0f;						_24 = 0.0f;
		_31 = (Left+Right) / (Left-Right);	_32 = (Top+Bottom) / (Bottom-Top);	_33 = Far / (Far-Near);			_34 = 1.0f;
		_41 = 0.0f;							_42 = 0.0f;							_43 = Near * Far / (Near-Far);	_44 = 0.0f;
	}
	//--------------------------------------------------------------------------------------------------------------
	//生成右手透视投影矩阵
	void Matrix4::SetPerspectiveOffCenterRH( float Left, float Right, float Top, float Bottom, float Near, float Far )
	{
		float Depth = Near - Far;

		_11 = 2.0f * Near / (Right-Left);	_12 = 0.0f;							_13 = 0.0f;					_14 = 0.0f;
		_21 = 0.0f;							_22 = 2.0f * Near / (Top-Bottom);	_23 = 0.0f;					_24 = 0.0f;
		_31 = (Left+Right) / (Left-Right);	_32 = (Top+Bottom) / (Bottom-Top);	_33 = Far / Depth;			_34 = -1.0f;
		_41 = 0.0f;							_42 = 0.0f;							_43 = Near * Far / Depth;	_44 = 0.0f;
	}
	//--------------------------------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------------------------------
	//默认 4x4 矩阵数学计算函数
	//--------------------------------------------------------------------------------------------------------------
	void FKFastcall _Def_Mat4TransMat4( Matrix4* pOut, const Matrix4* pMat1, const Matrix4* pMat2 )
	{
		pOut->_11 = pMat1->_11 * pMat2->_11 + pMat1->_12 * pMat2->_21 + pMat1->_13 * pMat2->_31 + pMat1->_14 * pMat2->_41;
		pOut->_12 = pMat1->_11 * pMat2->_12 + pMat1->_12 * pMat2->_22 + pMat1->_13 * pMat2->_32 + pMat1->_14 * pMat2->_42;
		pOut->_13 = pMat1->_11 * pMat2->_13 + pMat1->_12 * pMat2->_23 + pMat1->_13 * pMat2->_33 + pMat1->_14 * pMat2->_43;
		pOut->_14 = pMat1->_11 * pMat2->_14 + pMat1->_12 * pMat2->_24 + pMat1->_13 * pMat2->_34 + pMat1->_14 * pMat2->_44;

		pOut->_21 = pMat1->_21 * pMat2->_11 + pMat1->_22 * pMat2->_21 + pMat1->_23 * pMat2->_31 + pMat1->_24 * pMat2->_41;
		pOut->_22 = pMat1->_21 * pMat2->_12 + pMat1->_22 * pMat2->_22 + pMat1->_23 * pMat2->_32 + pMat1->_24 * pMat2->_42;
		pOut->_23 = pMat1->_21 * pMat2->_13 + pMat1->_22 * pMat2->_23 + pMat1->_23 * pMat2->_33 + pMat1->_24 * pMat2->_43;
		pOut->_24 = pMat1->_21 * pMat2->_14 + pMat1->_22 * pMat2->_24 + pMat1->_23 * pMat2->_34 + pMat1->_24 * pMat2->_44;

		pOut->_31 = pMat1->_31 * pMat2->_11 + pMat1->_32 * pMat2->_21 + pMat1->_33 * pMat2->_31 + pMat1->_34 * pMat2->_41;
		pOut->_32 = pMat1->_31 * pMat2->_12 + pMat1->_32 * pMat2->_22 + pMat1->_33 * pMat2->_32 + pMat1->_34 * pMat2->_42;
		pOut->_33 = pMat1->_31 * pMat2->_13 + pMat1->_32 * pMat2->_23 + pMat1->_33 * pMat2->_33 + pMat1->_34 * pMat2->_43;
		pOut->_34 = pMat1->_31 * pMat2->_14 + pMat1->_32 * pMat2->_24 + pMat1->_33 * pMat2->_34 + pMat1->_34 * pMat2->_44;

		pOut->_41 = pMat1->_41 * pMat2->_11 + pMat1->_42 * pMat2->_21 + pMat1->_43 * pMat2->_31 + pMat1->_44 * pMat2->_41;
		pOut->_42 = pMat1->_41 * pMat2->_12 + pMat1->_42 * pMat2->_22 + pMat1->_43 * pMat2->_32 + pMat1->_44 * pMat2->_42;
		pOut->_43 = pMat1->_41 * pMat2->_13 + pMat1->_42 * pMat2->_23 + pMat1->_43 * pMat2->_33 + pMat1->_44 * pMat2->_43;
		pOut->_44 = pMat1->_41 * pMat2->_14 + pMat1->_42 * pMat2->_24 + pMat1->_43 * pMat2->_34 + pMat1->_44 * pMat2->_44;
	}
	//--------------------------------------------------------------------------------------------------------------
	void FKFastcall _Def_Mat4TransVec3( Vector3* pOutV3, const Matrix4* pMat4, const Vector3* pInV3 )
	{
		float w = 1.0f;

		pOutV3->x = pInV3->x * pMat4->_11 + pInV3->y * pMat4->_21 + pInV3->z * pMat4->_31 + pMat4->_41;
		pOutV3->y = pInV3->x * pMat4->_12 + pInV3->y * pMat4->_22 + pInV3->z * pMat4->_32 + pMat4->_42;
		pOutV3->z = pInV3->x * pMat4->_13 + pInV3->y * pMat4->_23 + pInV3->z * pMat4->_33 + pMat4->_43;
		w         = pInV3->x * pMat4->_14 + pInV3->y * pMat4->_24 + pInV3->z * pMat4->_34 + pMat4->_44;

		float fInvW = 1.0f / w;

		pOutV3->x *= fInvW;
		pOutV3->y *= fInvW;
		pOutV3->z *= fInvW;
	}
	//--------------------------------------------------------------------------------------------------------------
	void FKFastcall _Def_Mat4TransVec4( Vector4* pOutV4, const Matrix4* pMat4, const Vector4* pInV4 )
	{
		pOutV4->x = pInV4->x * pMat4->_11 + pInV4->y * pMat4->_21 + pInV4->z * pMat4->_31 + pInV4->w * pMat4->_41;
		pOutV4->y = pInV4->x * pMat4->_12 + pInV4->y * pMat4->_22 + pInV4->z * pMat4->_32 + pInV4->w * pMat4->_42;
		pOutV4->z = pInV4->x * pMat4->_13 + pInV4->y * pMat4->_23 + pInV4->z * pMat4->_33 + pInV4->w * pMat4->_43;
		pOutV4->w = pInV4->x * pMat4->_14 + pInV4->y * pMat4->_24 + pInV4->z * pMat4->_34 + pInV4->w * pMat4->_44;
	}
	//--------------------------------------------------------------------------------------------------------------
	float _Def_Mat4Inv3x3( float a1, float a2, float a3, float b1, float b2, float b3,
		float c1, float c2, float c3 )
	{
		float d1 = b2 * c3 - c2 * b3;
		float d2 = a2 * c3 - c2 * a3;
		float d3 = a2 * b3 - b2 * a3;
		return a1 * d1 - b1 * d2 + c1 * d3;
	}
	//--------------------------------------------------------------------------------------------------------------
	void FKFastcall _Def_Mat4Inverse( Matrix4* pOut, const Matrix4* pIn )
	{
		pOut->_11 =  _Def_Mat4Inv3x3( pIn->_22, pIn->_32, pIn->_42, pIn->_23, pIn->_33, pIn->_43, pIn->_24, pIn->_34, pIn->_44 );
		pOut->_21 = -_Def_Mat4Inv3x3( pIn->_21, pIn->_31, pIn->_41, pIn->_23, pIn->_33, pIn->_43, pIn->_24, pIn->_34, pIn->_44 );
		pOut->_31 =  _Def_Mat4Inv3x3( pIn->_21, pIn->_31, pIn->_41, pIn->_22, pIn->_32, pIn->_42, pIn->_24, pIn->_34, pIn->_44 );
		pOut->_41 = -_Def_Mat4Inv3x3( pIn->_21, pIn->_31, pIn->_41, pIn->_22, pIn->_32, pIn->_42, pIn->_23, pIn->_33, pIn->_43 );

		pOut->_12 = -_Def_Mat4Inv3x3( pIn->_12, pIn->_32, pIn->_43, pIn->_13, pIn->_33, pIn->_43, pIn->_14, pIn->_34, pIn->_44 );
		pOut->_22 =  _Def_Mat4Inv3x3( pIn->_11, pIn->_31, pIn->_41, pIn->_13, pIn->_33, pIn->_43, pIn->_14, pIn->_34, pIn->_44 );
		pOut->_32 = -_Def_Mat4Inv3x3( pIn->_11, pIn->_31, pIn->_41, pIn->_12, pIn->_32, pIn->_42, pIn->_14, pIn->_34, pIn->_44 );
		pOut->_42 =  _Def_Mat4Inv3x3( pIn->_11, pIn->_31, pIn->_41, pIn->_12, pIn->_32, pIn->_42, pIn->_13, pIn->_33, pIn->_43 );

		pOut->_13 =  _Def_Mat4Inv3x3( pIn->_12, pIn->_22, pIn->_42, pIn->_13, pIn->_23, pIn->_43, pIn->_14, pIn->_24, pIn->_44 );
		pOut->_23 = -_Def_Mat4Inv3x3( pIn->_11, pIn->_21, pIn->_41, pIn->_13, pIn->_23, pIn->_43, pIn->_14, pIn->_24, pIn->_44 );
		pOut->_33 =  _Def_Mat4Inv3x3( pIn->_11, pIn->_21, pIn->_41, pIn->_12, pIn->_22, pIn->_42, pIn->_14, pIn->_24, pIn->_44 );
		pOut->_43 = -_Def_Mat4Inv3x3( pIn->_11, pIn->_21, pIn->_41, pIn->_12, pIn->_22, pIn->_42, pIn->_13, pIn->_23, pIn->_43 );

		pOut->_14 = -_Def_Mat4Inv3x3( pIn->_12, pIn->_22, pIn->_32, pIn->_13, pIn->_23, pIn->_33, pIn->_14, pIn->_24, pIn->_34 );
		pOut->_24 =  _Def_Mat4Inv3x3( pIn->_11, pIn->_21, pIn->_31, pIn->_13, pIn->_23, pIn->_33, pIn->_14, pIn->_24, pIn->_34 );
		pOut->_34 = -_Def_Mat4Inv3x3( pIn->_11, pIn->_21, pIn->_31, pIn->_12, pIn->_22, pIn->_32, pIn->_14, pIn->_24, pIn->_34 );
		pOut->_44 =  _Def_Mat4Inv3x3( pIn->_11, pIn->_21, pIn->_31, pIn->_12, pIn->_22, pIn->_32, pIn->_13, pIn->_23, pIn->_33 );

		float Product = pIn->_11 * pOut->_11 + pIn->_12 * pOut->_21 + pIn->_13 * pOut->_31 + pIn->_14 * pOut->_41;
		float InvProduct = 1.0f / Product;

		pOut->_11 *= InvProduct;	pOut->_21 *= InvProduct;	pOut->_31 *= InvProduct;	pOut->_41 *= InvProduct;
		pOut->_12 *= InvProduct;	pOut->_22 *= InvProduct;	pOut->_32 *= InvProduct;	pOut->_42 *= InvProduct;
		pOut->_13 *= InvProduct;	pOut->_23 *= InvProduct;	pOut->_33 *= InvProduct;	pOut->_43 *= InvProduct;
		pOut->_14 *= InvProduct;	pOut->_24 *= InvProduct;	pOut->_34 *= InvProduct;	pOut->_44 *= InvProduct;
	}
	//--------------------------------------------------------------------------------------------------------------
	void FKFastcall _Def_Mat4Transpose( Matrix4* pOut, const Matrix4* pIn )
	{
		pOut->_11 = pIn->_11;	pOut->_12 = pIn->_21;	pOut->_13 = pIn->_31;	pOut->_14 = pIn->_41;
		pOut->_21 = pIn->_12;	pOut->_22 = pIn->_22;	pOut->_23 = pIn->_32;	pOut->_24 = pIn->_42;
		pOut->_31 = pIn->_13;	pOut->_32 = pIn->_23;	pOut->_33 = pIn->_33;	pOut->_34 = pIn->_43;
		pOut->_41 = pIn->_14;	pOut->_42 = pIn->_24;	pOut->_43 = pIn->_34;	pOut->_44 = pIn->_44;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
