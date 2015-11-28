/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	BaseMath
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	数学库主程序
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	// 数学常用宏定义
	//--------------------------------------------------------------------------------------------------------------

	//圆周率定义
	#define FK_PI		3.14159265358979323846f
	#define FK_TWO_PI	6.28318530717958647692f
	#define FK_HALF_PI	1.57079632679489661923f

	//零近似值定义
	#define FK_EPSILON	0.0001f

	//将角度转换为孤度
	#define FK_DegreeToRadian( Degree ) ( (Degree) * ( FK_PI / 180.0f ) )

	//将孤度转换为角度
	#define FK_RadianToDegree( Radian ) ( (Radian) * ( 180.0f / FK_PI ) )

	#define SINCOS_TABLE_SIZE	256		//三角函数表大小

	//--------------------------------------------------------------------------------------------------------------

	//轴空间关系
	enum AxisSpaceRelation
	{
		ASR_Less		= 0,		//坐标值更大
		ASR_More		= 1,		//坐标值更小
		ASR_Overlap		= 2,		//重叠
		ASR_Invalid		= 0xFF,		//不可用
	};

	//平面与点的空间交错关系
	enum SpaceRelation
	{
		SR_POSITIVE,	//在平面正面
		SR_NEGATIVE,	//在平面背面
		SR_COINCIDING,	//与平面一致
		SR_SPANNING,	//与平面相跨
	};

	//--------------------------------------------------------------------------------------------------------------
	// 数学类预声明
	//--------------------------------------------------------------------------------------------------------------

	class Vector3;
	class Quaternion;
	class Matrix3;
	class Matrix4;
	class BoundBox;

	//--------------------------------------------------------------------------------------------------------------
	// 基本数学运算函数指针
	//--------------------------------------------------------------------------------------------------------------

	extern float	( FKFastcall *Sin )		( float radian );							//计算正弦
	extern float	( FKFastcall *Cos )		( float radian );							//计算余弦
	extern void		( FKFastcall *SinCos )	( float radian, float* pSin, float* pCos );	//计算正弦和余弦
	extern float	( FKFastcall *ASin )	( float sin );								//根据正弦计算度数
	extern float	( FKFastcall *ACos )	( float cos );								//根据余弦计算度数
	extern float	( FKFastcall *Tan )		( float radian );							//计算正切
	extern float	( FKFastcall *Cot )		( float radian );							//计算余切
	extern float	( FKFastcall *ATan )	( float x, float y );						//根据正切计算度数
	extern float	( FKFastcall *Abs )		( float val );								//求绝对值
	extern float	( FKFastcall *Sqrt )	( float val );								//计算平方根
	extern float	( FKFastcall *RSqrt )	( float val );								//计算平方根的倒数

	//检测是否为 2 次幂无符号整数
	extern FKForceInline bool IsPow2( UINT value );

	//将无符号整数转换为最接近但比它小的 2 次幂无符号整数
	extern FKForceInline UINT FloorToPow2( UINT value );

	//将无符号整数转换为最接近但比它大的 2 次幂无符号整数
	extern FKForceInline UINT CeilToPow2( UINT value );

	//有符号整数除法
	extern FKForceInline void Div( const int value, const int divisor,
		int& quotient, int& remainder );

	//无符号整数除法
	extern FKForceInline void Div( const unsigned int value, const unsigned int divisor,
		unsigned int& quotient, unsigned int& remainder );

	//将浮点数转换为最接近但比它小的整数
	extern FKForceInline int FloorToInt( float value );

	//将浮点数转换为最接近但比它大的整数
	extern FKForceInline int CeilToInt( float value );

	//--------------------------------------------------------------------------------------------------------------
	// 默认数学运算函数
	//--------------------------------------------------------------------------------------------------------------

	extern float	FKFastcall _Def_Sin( float radian );								//计算正弦
	extern float	FKFastcall _Def_Cos( float radian );								//计算余弦
	extern void		FKFastcall _Def_SinCos( float radian, float* pSin, float* pCos );	//计算正弦和余弦
	extern float	FKFastcall _Def_ASin( float sin );									//根据正弦计算度数
	extern float	FKFastcall _Def_ACos( float cos );									//根据余弦计算度数
	extern float	FKFastcall _Def_Tan( float radian );								//计算正切
	extern float	FKFastcall _Def_Cot( float radian );								//计算余切
	extern float	FKFastcall _Def_ATan( float x, float y );							//根据正切计算度数
	extern float	FKFastcall _Def_Abs( float val );									//求绝对值
	extern float	FKFastcall _Def_Sqrt( float val );									//计算平方根
	extern float	FKFastcall _Def_RSqrt( float val );									//计算平方根的倒数

	//--------------------------------------------------------------------------------------------------------------

	//初始化数学库
	extern bool InitializeMathLibrary( const CPUInfo* pCPUInfo );

	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
