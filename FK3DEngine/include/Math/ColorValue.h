/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	ColorValue
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	数学库 颜色值
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------

	//四字节种数颜色值定义
	typedef unsigned long	ARGB;
	typedef unsigned long	RGBA;
	typedef unsigned long	ABGR;

	//--------------------------------------------------------------------------------------------------------------
	//颜色值
	class FK_DLL ColorValue
	{
	public:
		float R;	//红色值
		float G;	//绿色值
		float B;	//蓝色值
		float A;	//透明值

	public:
		static ColorValue White;	//白色
		static ColorValue Black;	//黑色
		static ColorValue Red;		//红色
		static ColorValue Green;	//绿色
		static ColorValue Blue;		//蓝色
		static ColorValue Yellow;	//黄色
		static ColorValue Purple;	//紫色
		static ColorValue Azury;	//青色

	public:
		ColorValue();
		ColorValue( const float a, const float r, const float g, const float b );

		//操作符重载
		ColorValue& operator = ( const ColorValue& color );
		FKInline bool operator == ( const ColorValue& color ) const;
		FKInline bool operator != ( const ColorValue& color ) const;

		//设置整数颜色值
		void SetByteColor( const BYTE a, const BYTE r, const BYTE g, const BYTE b );
		void SetARGB( ARGB color );
		void SetRGBA( RGBA color );
		void SetABGR( ABGR color );

		//获取整数颜色值
		void GetByteColor( BYTE& a, BYTE& r, BYTE& g, BYTE& b ) const;
		ARGB GetARGB() const;
		RGBA GetRGBA() const;
		ABGR GetABGR() const;
	};
	//--------------------------------------------------------------------------------------------------------------
	typedef Stack< ColorValue >		ColorValueStack;
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
