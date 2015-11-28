/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Graphics
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "InsidePreInclude.h"
#include "Color.h"
#include "GraphicsBase.h"
#include "../../depend/boost/array.hpp"
#include "../../depend/boost/function.hpp"
#include "../../depend/boost/scoped_ptr.hpp"
#include <memory>
//--------------------------------------------------------------------
namespace FK2DEngine
{
	//! 返回屏幕当前分辨率宽度（像素）
	unsigned int	ScreenWidth();
	unsigned int	ScreenHeigth();

	//! 纹理最大尺寸
	unsigned int const MAX_TEXTURE_SIZE		= 1026;//1024; 为了支持1024大小的图

	typedef boost::array< double, 16 >		Transform;

	Transform Translate( double p_dX, double p_dY );
	Transform Rotate( double p_dAngle, double p_dAroundX = 0, double p_dAroundY = 0 );
	Transform Scale( double p_dFactor );
	Transform Scale( double p_dFactorX, double p_dFactorY, double p_dFromX = 0.0, double p_dFromY = 0.0 );

	class CGraphics
	{
	private:
		struct							SImpl;
		boost::scoped_ptr< SImpl >		m_pImpl;
	public:
		CGraphics( unsigned int p_unPhysicalWidth, unsigned int p_unPhysicalHeight, bool p_bFullScreen );
		~CGraphics();
	public:
		void	SetResolution( unsigned int p_unVirtualWidth, unsigned int p_unVirtualHeight );
	
		unsigned int	Width() const;
		unsigned int	Height() const;
		bool			FullScreen() const;

		//! 类似D3D的 BeginScene 和 EndScene。所有渲染必须在这俩函数间进行渲染。
		bool	Begin( CColor p_ClearColor = CColor::BLACK );
		void	End();

		//! 清空深度 z 缓冲
		void	Flush();

		//! 清空所有渲染对象操作，创建一个全新的渲染环境
		void	BeginGL();
		//! 重置渲染状态
		void	EndGL();

		//! 这个函数将在一个干净的FL上下文内进行对 z 的排序工作。（在BeginGL/EndGL之间）
		void	ScheduleGL( const boost::function< void() >& p_Functor, ZPos p_Z );
	
		//! 开启多边形裁减
		void	BeginClipping( double p_dX, double p_dY, double p_dWidth, double p_dHeight );
		//! 关闭裁减
		void	EndClipping();

		//! 记录
		void	BeginRecording();
		std::auto_ptr< FK2DEngine::CImageData >	EndRecording();

		//! 添加一个转换记录到转换记录栈中
		void	PushTransform( const Transform& p_Transform );
		void	PopTransform();

		//! 从一个点绘制到另外一个点的连线
		void	DrawLine( double p_dX1, double p_dY1, CColor p_Color1,
			double p_dX2, double p_dY2, CColor p_Color2,
			ZPos p_Z, ENUM_AlphaMode p_eMode = EAM_Default );

		void	DrawTriangle( double p_dX1, double p_dY1, CColor p_Color1,
			double p_dX2, double p_dY2, CColor p_Color2,
			double p_dX3, double p_dY3, CColor p_Color3,
			ZPos p_Z, ENUM_AlphaMode p_eMode = EAM_Default );

		void	DrawQuad( double p_dX1, double p_dY1, CColor p_Color1,
			double p_dX2, double p_dY2, CColor p_Color2,
			double p_dX3, double p_dY3, CColor p_Color3,
			double p_dX4, double p_dY4, CColor p_Color4,
			ZPos p_Z, ENUM_AlphaMode p_eMode = EAM_Default );

		//! 从一个图片中读取部分图片进行绘制
		std::auto_ptr< CImageData > CreateImage( const CBitmap& p_Src,
			unsigned int p_unSrcX, unsigned int p_unSrcY, unsigned int p_unSrcWidth,
			unsigned int p_unSrcHeight, unsigned int p_unBorderFlags );
	};
}