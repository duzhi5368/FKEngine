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
	//汇编优化 3x3 矩阵数学计算函数指针
	void ( FKFastcall *Mat3TransMat3 )		( Matrix3* pOut, const Matrix3* pMat1, const Matrix3* pMat2 )	= _Def_Mat3TransMat3;
	void ( FKFastcall *Mat3TransVec3 )		( Vector3* pOutV3, const Matrix3* pMat3, const Vector3* pInV3 )	= _Def_Mat3TransVec3;
	void ( FKFastcall *Mat3Inverse )		( Matrix3* pOut, const Matrix3* pIn )							= _Def_Mat3Inverse;
	void ( FKFastcall *Mat3Transpose )		( Matrix3* pOut, const Matrix3* pIn )							= _Def_Mat3Transpose;
	//--------------------------------------------------------------------------------------------------------------
	//常用值声明
	//--------------------------------------------------------------------------------------------------------------
	Matrix3 Matrix3::Identity(	1.0f, 0.0f, 0.0f,
								0.0f, 1.0f, 0.0f,
								0.0f, 0.0f, 1.0f );
	Matrix3 Matrix3::Zero(	0.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 0.0f );
	//--------------------------------------------------------------------------------------------------------------
	//不同的构造函数
	//--------------------------------------------------------------------------------------------------------------
	Matrix3::Matrix3()
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	Matrix3::Matrix3( const Matrix3& mat3ref )
	{
		memcpy( _m, mat3ref._m, sizeof(float)*9 );
	}
	//--------------------------------------------------------------------------------------------------------------
	Matrix3::Matrix3( const Matrix4& mat4ref )
	{
		_11 = mat4ref._11;		_12 = mat4ref._12;		_13 = mat4ref._13;
		_21 = mat4ref._21;		_22 = mat4ref._22;		_23 = mat4ref._23;
		_31 = mat4ref._31;		_32 = mat4ref._32;		_33 = mat4ref._33;
	}
	//--------------------------------------------------------------------------------------------------------------
	Matrix3::Matrix3(	const float f11, const float f12, const float f13,
								const float f21, const float f22, const float f23,
								const float f31, const float f32, const float f33 )
		: _11(f11), _12(f12), _13(f13)
		, _21(f21), _22(f22), _23(f23)
		, _31(f31), _32(f32), _33(f33)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	Matrix3::Matrix3( const float pVal[9] )
	{
		memcpy( _m, pVal, sizeof(float)*9 );
	}
	//--------------------------------------------------------------------------------------------------------------
	//各种运算符的重载
	//--------------------------------------------------------------------------------------------------------------
	Matrix3& Matrix3::operator = ( const Matrix3& mat3ref )
	{
		memcpy( this->_m, mat3ref._m, sizeof(float)*9 );
		return *this;
	}
	//--------------------------------------------------------------------------------------------------------------
	bool Matrix3::operator == ( const Matrix3& mat3ref ) const
	{
		if( 0 == memcmp( this->_m, mat3ref._m, sizeof(float)*9 ) )
			return true;
		else
			return false;
	}
	//--------------------------------------------------------------------------------------------------------------
	bool Matrix3::operator != ( const Matrix3& mat3ref ) const
	{
		if( 0 != memcmp( this->_m, mat3ref._m, sizeof(float)*9 ) )
			return true;
		else
			return false;
	}
	//--------------------------------------------------------------------------------------------------------------
	Matrix3 Matrix3::operator * ( const Matrix3& mat3ref ) const
	{
		static Matrix3 tmp;
		Mat3TransMat3( &tmp, this, &mat3ref );
		return tmp;
	}
	//--------------------------------------------------------------------------------------------------------------
	void Matrix3::operator *= ( const Matrix3& mat3ref )
	{
		static Matrix3 tmp;
		Mat3TransMat3( &tmp, this, &mat3ref );
		*this = tmp;
	}
	//--------------------------------------------------------------------------------------------------------------
	Vector3 Matrix3::operator * ( const Vector3& vec3ref ) const
	{
		static Vector3 tmp;
		Mat3TransVec3( &tmp, this, &vec3ref );
		return tmp;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取矩阵数值描述
	String Matrix3::GetDescription()
	{
		int nLen = 0;
		char* pStr = Misc::FormatString( &nLen,
			"\nMatrix3:\n"
			"11 %f\t12 %f\t13 %f\n"
			"21 %f\t22 %f\t23 %f\n"
			"31 %f\t32 %f\t33 %f\n",
			_11, _12, _13, _21, _22, _23, _31, _32, _33 );

		return String( pStr, (UINT)nLen );
	}
	//--------------------------------------------------------------------------------------------------------------
	//生成单位矩阵
	void Matrix3::SetIdentity()
	{
		*this = Matrix3::Identity;
	}
	//--------------------------------------------------------------------------------------------------------------
	//生成零矩阵
	void Matrix3::SetZero()
	{
		*this = Matrix3::Zero;
	}
	//--------------------------------------------------------------------------------------------------------------
	//生成转置矩阵
	void Matrix3::SetTranspose()
	{
		static Matrix3 tmp;
		Mat3Transpose( &tmp, this );
		*this = tmp;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取转置矩阵
	Matrix3 Matrix3::GetTranspose() const
	{
		static Matrix3 tmp;
		Mat3Transpose( &tmp, this );
		return tmp;
	}
	//--------------------------------------------------------------------------------------------------------------
	//生成反矩阵
	void Matrix3::SetInverse()
	{
		static Matrix3 tmp;
		Mat3Inverse( &tmp, this );
		*this = tmp;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取反矩阵
	Matrix3 Matrix3::GetInverse() const
	{
		static Matrix3 tmp;
		Mat3Inverse( &tmp, this );
		return tmp;
	}
	//--------------------------------------------------------------------------------------------------------------
	//根据自定义轴生成旋转矩阵
	void Matrix3::SetRotationAxis( const float Degree, const Vector3& Axis )
	{
		float fCos = 0.0f;
		float fSin = 0.0f;
		SinCos( FK_DegreeToRadian(Degree), &fSin, &fCos );

		float fOneMinusCos = 1.0 - fCos;
		float fX2 = Axis.x * Axis.x;
		float fY2 = Axis.y * Axis.y;
		float fZ2 = Axis.z * Axis.z;
		float fXYM = Axis.x * Axis.y * fOneMinusCos;
		float fXZM = Axis.x * Axis.z * fOneMinusCos;
		float fYZM = Axis.y * Axis.z * fOneMinusCos;
		float fXSin = Axis.x * fSin;
		float fYSin = Axis.y * fSin;
		float fZSin = Axis.z * fSin;

		m[0][0] = fX2 * fOneMinusCos + fCos;
		m[0][1] = fXYM - fZSin;
		m[0][2] = fXZM + fYSin;
		m[1][0] = fXYM + fZSin;
		m[1][1] = fY2 * fOneMinusCos + fCos;
		m[1][2] = fYZM - fXSin;
		m[2][0] = fXZM - fYSin;
		m[2][1] = fYZM + fXSin;
		m[2][2] = fZ2 * fOneMinusCos + fCos;
	}
	//--------------------------------------------------------------------------------------------------------------
	//根据四元数生成旋转矩阵
	void Matrix3::SetRotationQuaternion( const Quaternion& Quat )
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

		_21 = xy + wz;
		_22 = 1.0f - xx - zz;
		_23 = yz - wx;

		_31 = xz - wy;
		_32 = yz + wx;
		_33 = 1.0f - xx - yy;
	}
	//--------------------------------------------------------------------------------------------------------------
	//根据欧拉角生成旋转矩阵
	void Matrix3::SetRotationPitchYawRoll( const float Pitch, const float Yaw, const float Roll )
	{
		//生成分别绕 X、Y、Z 轴旋转的四元数
		static Quaternion quat;
		quat.SetRotationPitchYawRoll( Pitch, Yaw, Roll );

		//生成旋转矩阵
		SetRotationQuaternion( quat );
	}
	//--------------------------------------------------------------------------------------------------------------
	//生成绕 X 轴旋转矩阵
	void Matrix3::SetRotationX( const float Degree )
	{
		float sin, cos;
		SinCos( FK_DegreeToRadian(Degree), &sin, &cos );

		_11 = 1.0f;		_12 = 0.0f;		_13 = 0.0f;
		_21 = 0.0f;		_22 = cos;		_23 = sin;
		_31 = 0.0f;		_32 = -sin;		_33 = cos;
	}
	//--------------------------------------------------------------------------------------------------------------
	//生成绕 Y 轴旋转矩阵
	void Matrix3::SetRotationY( const float Degree )
	{
		float sin, cos;
		SinCos( FK_DegreeToRadian(Degree), &sin, &cos );

		_11 = cos;		_12 = 0.0f;		_13 = -sin;
		_21 = 0.0f;		_22 = 1.0f;		_23 = 0.0f;
		_31 = sin;		_32 = 0.0f;		_33 = cos;
	}
	//--------------------------------------------------------------------------------------------------------------
	//生成绕 Z 轴旋转矩阵
	void Matrix3::SetRotationZ( const float Degree )
	{
		float sin, cos;
		SinCos( FK_DegreeToRadian(Degree), &sin, &cos );

		_11 = cos;		_12 = sin;		_13 = 0.0f;
		_21 = -sin;		_22 = cos;		_23 = 0.0f;
		_31 = 0.0f;		_32 = 0.0f;		_33 = 1.0f;
	}
	//--------------------------------------------------------------------------------------------------------------
	//生成缩放矩阵
	void Matrix3::SetScaling( const float sx, const float sy, const float sz )
	{
		_11 = sx;		_12 = 0.0f;		_13 = 0.0f;
		_21 = 0.0f;		_22 = sy;		_23 = 0.0f;
		_31 = 0.0f;		_32 = 0.0f;		_33 = sz;
	}
	//--------------------------------------------------------------------------------------------------------------
	//生成缩放矩阵
	void Matrix3::SetScaling( const Vector3& Scl )
	{
		_11 = Scl.x;	_12 = 0.0f;		_13 = 0.0f;
		_21 = 0.0f;		_22 = Scl.y;	_23 = 0.0f;
		_31 = 0.0f;		_32 = 0.0f;		_33 = Scl.z;
	}
	//--------------------------------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------------------------------
	//默认三维矢量数学计算函数
	//--------------------------------------------------------------------------------------------------------------
	void FKFastcall _Def_Mat3TransMat3( Matrix3* pOut, const Matrix3* pMat1, const Matrix3* pMat2 )
	{
		pOut->_11 = pMat1->_11 * pMat2->_11 + pMat1->_12 * pMat2->_21 + pMat1->_13 * pMat2->_31;
		pOut->_12 = pMat1->_11 * pMat2->_12 + pMat1->_12 * pMat2->_22 + pMat1->_13 * pMat2->_32;
		pOut->_13 = pMat1->_11 * pMat2->_13 + pMat1->_12 * pMat2->_23 + pMat1->_13 * pMat2->_33;

		pOut->_21 = pMat1->_21 * pMat2->_11 + pMat1->_22 * pMat2->_21 + pMat1->_23 * pMat2->_31;
		pOut->_22 = pMat1->_21 * pMat2->_12 + pMat1->_22 * pMat2->_22 + pMat1->_23 * pMat2->_32;
		pOut->_23 = pMat1->_21 * pMat2->_13 + pMat1->_22 * pMat2->_23 + pMat1->_23 * pMat2->_33;

		pOut->_31 = pMat1->_31 * pMat2->_11 + pMat1->_32 * pMat2->_21 + pMat1->_33 * pMat2->_31;
		pOut->_32 = pMat1->_31 * pMat2->_12 + pMat1->_32 * pMat2->_22 + pMat1->_33 * pMat2->_32;
		pOut->_33 = pMat1->_31 * pMat2->_13 + pMat1->_32 * pMat2->_23 + pMat1->_33 * pMat2->_33;
	}
	//--------------------------------------------------------------------------------------------------------------
	void FKFastcall _Def_Mat3TransVec3( Vector3* pOutV3, const Matrix3* pMat3, const Vector3* pInV3 )
	{
		pOutV3->x = pInV3->x * pMat3->_11 + pInV3->y * pMat3->_21 + pInV3->z * pMat3->_31;
		pOutV3->y = pInV3->x * pMat3->_12 + pInV3->y * pMat3->_22 + pInV3->z * pMat3->_32;
		pOutV3->z = pInV3->x * pMat3->_13 + pInV3->y * pMat3->_23 + pInV3->z * pMat3->_33;
	}
	//--------------------------------------------------------------------------------------------------------------
	void FKFastcall _Def_Mat3Inverse( Matrix3* pOut, const Matrix3* pIn )
	{
		float det	= pIn->_11 * ( pIn->_22 * pIn->_33 - pIn->_32 * pIn->_23 )
					- pIn->_12 * ( pIn->_21 * pIn->_33 - pIn->_31 * pIn->_23 )
					+ pIn->_13 * ( pIn->_21 * pIn->_32 - pIn->_31 * pIn->_22 );

		if( det < 0.0005f && det > -0.0005f )
		{
			pOut->_11 = 1.0f;	pOut->_12 = 0.0f;	pOut->_13 = 0.0f;
			pOut->_21 = 0.0f;	pOut->_22 = 1.0f;	pOut->_23 = 0.0f;
			pOut->_31 = 0.0f;	pOut->_32 = 0.0f;	pOut->_33 = 1.0f;
			return;
		}

		float InvDet = 1.0f / det;

		pOut->_11 =		pIn->_22 * pIn->_33 - pIn->_23 * pIn->_32	* InvDet;
		pOut->_12 =	- (	pIn->_12 * pIn->_33 - pIn->_32 * pIn->_13 )	* InvDet;
		pOut->_13 =		pIn->_12 * pIn->_23 - pIn->_22 * pIn->_13	* InvDet;

		pOut->_21 =	- (	pIn->_21 * pIn->_33 - pIn->_23 * pIn->_31 )	* InvDet;
		pOut->_22 =		pIn->_11 * pIn->_33 - pIn->_31 * pIn->_13	* InvDet;
		pOut->_23 =	- (	pIn->_11 * pIn->_23 - pIn->_21 * pIn->_13 )	* InvDet;

		pOut->_31 =		pIn->_21 * pIn->_32 - pIn->_31 * pIn->_22	* InvDet;
		pOut->_32 =	- (	pIn->_11 * pIn->_32 - pIn->_31 * pIn->_12 )	* InvDet;
		pOut->_33 =		pIn->_11 * pIn->_22 - pIn->_12 * pIn->_21	* InvDet;		
	}
	//--------------------------------------------------------------------------------------------------------------
	void FKFastcall _Def_Mat3Transpose( Matrix3* pOut, const Matrix3* pIn )
	{
		pOut->_11 = pIn->_11;	pOut->_12 = pIn->_21;	pOut->_13 = pIn->_31;
		pOut->_21 = pIn->_12;	pOut->_22 = pIn->_22;	pOut->_23 = pIn->_32;
		pOut->_31 = pIn->_13;	pOut->_32 = pIn->_23;	pOut->_33 = pIn->_33;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
