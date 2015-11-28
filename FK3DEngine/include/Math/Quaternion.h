/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Quaternion
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	数学库 四元数
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//四元数
	class FK_DLL FKAlign16 Quaternion
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
		static Quaternion Identity;
		static Quaternion Zero;

	public:
		//不同的构造函数
		Quaternion();
		Quaternion( const Quaternion& quat );
		Quaternion( const float fx, const float fy, const float fz, const float fw );
		Quaternion( const float* pVal );
		Quaternion( const float Pitch, const float Yaw, const float Roll );
		Quaternion( const float Degree, const Vector3& Axis );
		Quaternion( const Matrix3& mat3 );
		Quaternion( const Matrix4& mat4 );

		//各种运算符的重载
		Quaternion& operator = ( const Quaternion& quat );
		bool operator == ( const Quaternion& quat ) const;
		bool operator != ( const Quaternion& quat ) const;
		Quaternion operator + ( const Quaternion& quat ) const;
		Quaternion operator - ( const Quaternion& quat ) const;
		Quaternion operator * ( const Quaternion& quat ) const;
		Quaternion operator * ( const float fScalar ) const;
		Vector3 operator * ( const Vector3 v3 ) const;
		void operator += ( const Quaternion& quat );
		void operator -= ( const Quaternion& quat );
		void operator *= ( const Quaternion& quat );
		void operator *= ( const float fScalar );

		//根据欧拉角生成四元数
		void SetRotationPitchYawRoll( const float Pitch, const float Yaw, const float Roll );

		//根据自定义轴生成四元数
		void SetRotationAxis( const float Degree, const Vector3& Axis );

		//根据旋转矩阵生成四元数
		void SetRotationMatrix3( const Matrix3& mat3 );

		//根据旋转矩阵生成四元数
		void SetRotationMatrix4( const Matrix4& mat4 );

		//获取四元数绕自定义轴旋转数据
		void GetRotationAxis( float* pDegree, Vector3* pAxis );

		//获取四元数旋转矩阵数据
		void GetRotationMatrix3( Matrix3* pMat3 );

		//获取四元数旋转矩阵数据
		void GetRotationMatrix4( Matrix4* pMat4 );

		//获取四元数点乘积
		float GetDot( const Quaternion& quat );

		//获取四元数长度
		float GetLength();

		//四元数单位化
		void Normalize();

		//获取四元数的逆
		Quaternion GetInverse();

		//四元数求逆
		void SetInverse();
	};
	//--------------------------------------------------------------------------------------------------------------
	typedef Stack< Quaternion >		QuaternionStack;
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
