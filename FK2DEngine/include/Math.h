/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Math
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#pragma warning( disable : 4244 )
//--------------------------------------------------------------------
namespace FK2DEngine
{
	//! Pi
	const double PI = 3.1415926536;

	//! 截断一个浮点数小数部分
	inline long Trunc( double p_dValue )
	{
		return static_cast< long >( p_dValue );
	}

	//! 四舍五入一个浮点数，将其变成整数
	inline long Round( double p_dValue )
	{
		if( p_dValue >= 0 )
		{
			return static_cast< long >( p_dValue + 0.5 );
		}
		else
		{
			return static_cast< long >( p_dValue - 0.5 );
		}
	}

	//! 返回一个从 min 到 max 内的一个随机浮点值。
	//! 内部使用 std::rand ，所以在调用该函数之前必须先调用 std::random
	double Random( double p_dMin, double p_dMax );

	//! 将FKAngle换转为弧度
	inline double FKAngleToRadians( double p_dAngle )
	{
		return ( p_dAngle - 90 ) * PI / 180;
	}

	//! 将弧度转换为FKAngle
	inline double RadiansToFKAngle( double p_dAngle )
	{
		return p_dAngle * 180 / PI + 90;
	}

	//! 角度转换为弧度
	inline double DegreesToRadians( double p_dAngle )
	{
		return p_dAngle * PI / 180;
	}

	//! 弧度转换为角度
	inline double RadiansToDegrees( double p_dAngle )
	{
		return p_dAngle * 180 / PI;
	}

	//! 返回一个点离原点的 X 坐标偏移
	//! \param p_dAngle 该点到原点之间的线段角度。其中 0 度将代表向上。
	//! \param p_dRadius 该点离原点之间的线段距离。
	double OffsetX( double p_dAngle, double p_dRadius );

	//! 参数同 OffsetX ,返回一个点离原点的 Y 坐标偏移
	double OffsetY( double p_dAngle, double p_dRadius );

	//! 返回两点连线的角度。其中 0 度代表垂直向上。
	//! 当两个点重合时，将返回 p_dDefaultValue 默认值。
	double Angle( double p_dFromX, double p_dFromY, double p_dToX, 
		double p_dToY, double p_dDefaultValue = 0 );

	//! 将 Angle1 转变为 Angle2 所需要旋转地最小角度
	//! \note 该值可能为负值
	double AngleDiff( double p_dAngle1, double p_dAngle2 );

	//! 单位化一个角度，将其值控制在 [0, 360] 之间
	double NormalizeAngle( double p_dAngle );

	//! 求两值平方
	template< typename T >
	T Square( T p_Value )
	{
		return p_Value * p_Value;
	}

	//! 若p_Value小于p_Min则返回p_Min,若p_Value大于p_Max,则返回p_Max,否则返回p_Value
	template< typename T >
	T Clamp( T p_Value, T p_Min, T p_Max )
	{
		if( p_Value < p_Min )
		{
			return p_Min;
		}
		if( p_Value > p_Max )
		{
			return p_Max;
		}
		return p_Value;
	}

	//! 范围检查
	template< typename T >
	T BoundBy( T p_Value, T p_Min, T p_Max )
	{
		return Clamp( p_Value, p_Min, p_Max );
	}

	//! 返回( Value - Min ) % ( Max - Min ) + Min
	int Wrap( int p_nValue, int p_nMin, int p_nMax );
	float Wrap( float p_fValue, float p_fMin, float p_fMax );
	double Wrap( double p_dValue, double p_dMin, double p_dMax );

	//! 返回两个点之间距离的平方
	double DistanceSqr( double p_dX1, double p_dY1, double p_dX2, double p_dY2 );

	//! 返回两个点之间距离
	double Distance( double p_dX1, double p_dY1, double p_dX2, double p_dY2 );

	//! 根据权重计算两值间的一个数
	//! 例如 Interpolate( 0, 10, 0.2 ) == 2;  Interpolate( 0, 10, 0.6 ) == 6
	template< typename T >
	T Interpolate( T p_A, T p_B, double p_dWeight = 0.5 )
	{
		return p_A * ( 1.0 - p_dWeight ) + p_B * p_dWeight;
	}
}