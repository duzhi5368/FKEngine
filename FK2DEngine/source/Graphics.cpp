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

#include "../include/Graphics.h"
#include "../include/GraphicsCommon.h"
#include "../include/DrawOp.h"
#include "../include/Texture.h"
#include "../include/TexChunk.h"
#include "../include/LargeImageData.h"
#include "../include/Macro.h"
#include "../include/Bitmap.h"
#include "../include/Image.h"
#include "../include/Platform.h"
#include "../../depend/boost/foreach.hpp"
#include <cmath>
#include <algorithm>
#include <limits>

//#ifdef FK_IPHONE
//#import <UIKit/UIKit.h>
//#endif
//--------------------------------------------------------------------
#pragma warning( disable : 4244 )
//--------------------------------------------------------------------
struct FK2DEngine::CGraphics::SImpl
{
	typedef std::vector< boost::shared_ptr< CTexture > >	Textures;
public:
	unsigned int		m_unVirtWidth;
	unsigned int		m_unVirtHeight;
	unsigned int		m_unPhysWidth;
	unsigned int		m_unPhysHeight;
	bool				m_nFullScreen;
	DrawOpQueueStack	m_Queues;
	Textures			m_vecTextures;
	Transforms			m_CurTransforms;
	Transforms			m_AbsoluteTransforms;

#ifdef FK_IPHONE
#pragma message("Graphics暂时对IPHONE不能支持")
#endif
};
//--------------------------------------------------------------------
FK2DEngine::CGraphics::CGraphics(unsigned int p_unPhysicalWidth, unsigned int p_unPhysicalHeight, bool p_bFullScreen)
	: m_pImpl( new SImpl )
{
	m_pImpl->m_unPhysWidth	= p_unPhysicalWidth;
	m_pImpl->m_unPhysHeight	= p_unPhysicalHeight;
	m_pImpl->m_unVirtWidth	= p_unPhysicalWidth;
	m_pImpl->m_unVirtHeight = p_unPhysicalHeight;
	m_pImpl->m_nFullScreen	= p_bFullScreen;

#ifdef FK_IPHONE
	std::swap( m_pImpl->m_unVirtWidth, m_pImpl->m_unVirtHeight );
#endif

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glViewport( 0, 0, p_unPhysicalWidth, p_unPhysicalHeight );

#ifdef FK_IPHONE
	glOrthof( 0, p_unPhysicalWidth, p_unPhysicalHeight, 0, -1, 1 );
#else
	glOrtho( 0, p_unPhysicalWidth, p_unPhysicalHeight, 0, -1, 1 );
#endif

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glEnable( GL_BLEND );

	m_pImpl->m_Queues.resize(1);

	m_pImpl->m_CurTransforms.push_back( FK2DEngine::Scale( 1 ) );
	m_pImpl->m_AbsoluteTransforms.push_back( FK2DEngine::Scale( 1 ) );
}
//--------------------------------------------------------------------
FK2DEngine::CGraphics::~CGraphics()
{

}
//--------------------------------------------------------------------
unsigned int FK2DEngine::CGraphics::Width() const
{
	double dSize[2] = { m_pImpl->m_unVirtWidth, m_pImpl->m_unVirtHeight };
	return static_cast< unsigned int >( dSize[0] );
}
//--------------------------------------------------------------------
unsigned int FK2DEngine::CGraphics::Height() const
{
	double dSize[2] = { m_pImpl->m_unVirtWidth, m_pImpl->m_unVirtHeight };
	return static_cast< unsigned int >( dSize[1] );
}
//--------------------------------------------------------------------
bool FK2DEngine::CGraphics::FullScreen() const
{
	return m_pImpl->m_nFullScreen;
}
//--------------------------------------------------------------------
void FK2DEngine::CGraphics::SetResolution(unsigned int p_unVirtualWidth, unsigned int p_unVirtualHeight)
{
	if( p_unVirtualWidth == 0 || p_unVirtualHeight == 0 )
	{
		throw std::invalid_argument("无效的虚拟决议");
	}

	m_pImpl->m_unVirtWidth	= p_unVirtualWidth;
	m_pImpl->m_unVirtHeight	= p_unVirtualHeight;

	Transform baseTransform;
	baseTransform = Scale( 1.0 / p_unVirtualWidth * m_pImpl->m_unPhysWidth,
		1.0 / p_unVirtualHeight * m_pImpl->m_unPhysHeight );
	m_pImpl->m_CurTransforms.front() = m_pImpl->m_AbsoluteTransforms.front() = baseTransform;
}
//--------------------------------------------------------------------
bool FK2DEngine::CGraphics::Begin(FK2DEngine::CColor p_ClearColor /*= CColor::BLACK*/)
{
	m_pImpl->m_Queues.resize( 1 );
	m_pImpl->m_Queues.front().Clear();
	m_pImpl->m_CurTransforms.resize( 1 );

	m_pImpl->m_AbsoluteTransforms = m_pImpl->m_CurTransforms;

	glClearColor( p_ClearColor.Red() / 255.0, p_ClearColor.Green() / 255.0,
		p_ClearColor.Blue() / 255.0, p_ClearColor.Alpha() / 255.0 );
	glClearDepth( 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );


	// 重新绘制
	// glScissor( 0, 0, m_pImpl->m_unPhysWidth, m_pImpl->m_unPhysHeight );

	// glClearDepth( 1.0f );

	// 清除模板值，色值，深度值
	// glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

	// 开启模板测试
	// glEnable(GL_STENCIL_TEST );  

	// 开启深度测试
	// glEnable(GL_DEPTH_TEST);
	// glDepthMask( GL_FALSE );
	// glDepthFunc( GL_GREATER );

	// 开启Aplha测试
	// glEnable( GL_ALPHA_TEST );

	return true;
}
//--------------------------------------------------------------------
void FK2DEngine::CGraphics::End()
{
	// 执行绘图单元
	Flush();

	// 关闭Aplha测试
	// glDisable( GL_ALPHA_TEST );

	// 强制刷新缓冲
	glFlush();
	// 结束，进行同步
	glFinish();
}
//--------------------------------------------------------------------
void FK2DEngine::CGraphics::Flush()
{
	m_pImpl->m_Queues.resize( 1 );
	m_pImpl->m_Queues.at( 0 ).PerformDrawOpsAndCode();
	m_pImpl->m_Queues.at( 0 ).Clear();

	// 避免内存泄露
	m_pImpl->m_Queues.clear();	
	m_pImpl->m_AbsoluteTransforms.clear();
}
//--------------------------------------------------------------------
void FK2DEngine::CGraphics::BeginGL()
{
	if( m_pImpl->m_Queues.size() > 1 )
	{
		throw std::logic_error( "自定义OpenGL不支持创建CMacro" );
	}

#ifdef FK_IPHONE
	throw std::logic_error( "自定义OpenGL不支持IPHONE" );
#else
	Flush();
	glPushAttrib( GL_ALL_ATTRIB_BITS );
	glDisable( GL_BLEND );
	while( glGetError() != GL_NO_ERROR );
#endif
}
//--------------------------------------------------------------------
void FK2DEngine::CGraphics::EndGL()
{
#ifdef FK_IPHONE
	throw std::logic_error( "自定义OpenGL不支持IPHONE" );
#else
	glPopAttrib();

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glViewport( 0, 0, m_pImpl->m_unPhysWidth, m_pImpl->m_unPhysHeight );
	glOrtho( 0, m_pImpl->m_unPhysWidth, m_pImpl->m_unPhysHeight, 0, -1, 1 );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glEnable( GL_BLEND );
#endif
}
//--------------------------------------------------------------------
#ifdef FK_IPHONE
	void FK2DEngine::CGraphics::ScheduleGL(const boost::function<void()> &p_Functor, FK2DEngine::ZPos p_Z)
	{
		throw std::logic_error( "自定义OpenGL不支持IPHONE" );
	}
#else
	namespace
	{
		struct SRunGLFunctor
		{
			FK2DEngine::CGraphics&		m_Graphics;
			boost::function< void() >	m_Functor;

			SRunGLFunctor( FK2DEngine::CGraphics& p_Graphics, const boost::function< void() >& p_Functor )
				: m_Graphics( p_Graphics )
				, m_Functor( p_Functor )
			{

			}

			void operator()() const
			{
				glPushAttrib( GL_ALL_ATTRIB_BITS );
				glDisable( GL_BLEND );
				while( glGetError() != GL_NO_ERROR );

				m_Functor();

				m_Graphics.EndGL();
			}
		};
	}

	void FK2DEngine::CGraphics::ScheduleGL(const boost::function<void()> &p_Functor, FK2DEngine::ZPos p_Z)
	{
		m_pImpl->m_Queues.back().ScheduleGL( SRunGLFunctor( *this, p_Functor ), p_Z );
	}
#endif
//--------------------------------------------------------------------
void FK2DEngine::CGraphics::BeginClipping(double p_dX, double p_dY, double p_dWidth, double p_dHeight)
{
	if( m_pImpl->m_Queues.size() > 1 )
	{
		throw std::logic_error("剪裁不支持对一个创建的CMaro类型进行处理");
	}

	double dLeft	= p_dX;
	double dRight	= p_dX + p_dWidth;
	double dTop		= p_dY;
	double dBottom	= p_dY + p_dHeight;

	ApplyTransform( m_pImpl->m_AbsoluteTransforms.back(), dLeft, dTop );
	ApplyTransform( m_pImpl->m_AbsoluteTransforms.back(), dRight, dBottom );

	int nPhyX = static_cast< int >( min( dLeft, dRight ));
	int nPhyY = static_cast< int >( max( dTop, dBottom ));
	int nPhyWidth = static_cast< int >( std::abs( dLeft - dRight ));
	int nPhyHeight = static_cast< int >( std::abs( dTop - dBottom ));

	nPhyY = m_pImpl->m_unPhysHeight - nPhyY - nPhyHeight;

	m_pImpl->m_Queues.back().BeginClipping( nPhyX, nPhyY, nPhyWidth, nPhyHeight );
}
//--------------------------------------------------------------------
void FK2DEngine::CGraphics::EndClipping()
{
	if( m_pImpl->m_Queues.size() > 1 )
	{
		throw std::logic_error("剪裁不支持对一个创建的CMaro类型进行处理");
	}

	m_pImpl->m_Queues.back().EndClipping();
}
//--------------------------------------------------------------------
void FK2DEngine::CGraphics::BeginRecording()
{
	m_pImpl->m_Queues.resize( m_pImpl->m_Queues.size() + 1 );
}
//--------------------------------------------------------------------
std::auto_ptr< FK2DEngine::CImageData >	FK2DEngine::CGraphics::EndRecording()
{
	if( m_pImpl->m_Queues.size() == 1 )
	{
		throw std::logic_error( "CMacro类型对象无法进行记录捕获" );
	}

	std::auto_ptr< CImageData >	Result( new CMacro( *this, m_pImpl->m_Queues.back() ) );
	m_pImpl->m_Queues.pop_back();
	return Result;
}
//--------------------------------------------------------------------
namespace
{
	void EnsureBackOfList( FK2DEngine::Transforms& p_list, const FK2DEngine::Transform& p_Transform )
	{
		FK2DEngine::Transforms::iterator OldPos = std::find( p_list.begin(), p_list.end(), p_Transform );
		if( OldPos == p_list.end() )
		{
			p_list.push_back( p_Transform );
		}
		else
		{
			p_list.splice( p_list.end(), p_list, OldPos );
		}
	}
}
//--------------------------------------------------------------------
void FK2DEngine::CGraphics::PushTransform( const FK2DEngine:: Transform& p_Transform )
{
	m_pImpl->m_CurTransforms.push_back( p_Transform );
	Transform Result = MutliplyTransform( p_Transform, m_pImpl->m_AbsoluteTransforms.back() );
	EnsureBackOfList( m_pImpl->m_AbsoluteTransforms, Result );
}
//--------------------------------------------------------------------
void FK2DEngine::CGraphics::PopTransform()
{
	m_pImpl->m_CurTransforms.pop_back();
	Transform Result = Scale( 1 );
	BOOST_REVERSE_FOREACH( const Transform& tf, m_pImpl->m_CurTransforms )
	{
		Result = MutliplyTransform( Result, tf );
	}
	EnsureBackOfList( m_pImpl->m_AbsoluteTransforms, Result );
}
//--------------------------------------------------------------------
void FK2DEngine::CGraphics::DrawLine(double p_dX1, double p_dY1,
	FK2DEngine::CColor p_Color1, double p_dX2, double p_dY2,
	FK2DEngine::CColor p_Color2, FK2DEngine::ZPos p_Z,
	FK2DEngine::ENUM_AlphaMode p_eMode /*= EAM_Default*/)
{
	SDrawOp op( m_pImpl->m_AbsoluteTransforms.back() );

	op.m_eMode				= p_eMode;
	op.m_unUsedVertices		= 2;
	op.m_Vertices[0]		= SDrawOp::SVertex( p_dX1, p_dY1, p_Color1 );
	op.m_Vertices[1]		= SDrawOp::SVertex( p_dX2, p_dY2, p_Color2 );

	m_pImpl->m_Queues.back().ScheduleDrawOp( op, p_Z );
}
//--------------------------------------------------------------------
void FK2DEngine::CGraphics::DrawTriangle(double p_dX1, double p_dY1, FK2DEngine::CColor p_Color1,
	double p_dX2, double p_dY2, FK2DEngine::CColor p_Color2, double p_dX3,
	double p_dY3, FK2DEngine::CColor p_Color3, FK2DEngine::ZPos p_Z,
	FK2DEngine::ENUM_AlphaMode p_eMode /*= EAM_Default*/)
{
	SDrawOp op( m_pImpl->m_AbsoluteTransforms.back() );

	op.m_eMode				= p_eMode;
	op.m_unUsedVertices		= 3;
	op.m_Vertices[0]		= SDrawOp::SVertex( p_dX1, p_dY1, p_Color1 );
	op.m_Vertices[1]		= SDrawOp::SVertex( p_dX2, p_dY2, p_Color2 );
	op.m_Vertices[2]		= SDrawOp::SVertex( p_dX3, p_dY3, p_Color3 );

#ifdef FK_IPHONE
	op.m_unUsedVertices		= 4;
	op.m_Vertices[3]		= op.m_Vertices[2];
#endif

	m_pImpl->m_Queues.back().ScheduleDrawOp( op, p_Z );
}
//--------------------------------------------------------------------
void FK2DEngine::CGraphics::DrawQuad(double p_dX1, double p_dY1, FK2DEngine::CColor p_Color1,
	double p_dX2, double p_dY2, FK2DEngine::CColor p_Color2, double p_dX3, double p_dY3,
	FK2DEngine::CColor p_Color3, double p_dX4, double p_dY4, FK2DEngine::CColor p_Color4,
	FK2DEngine::ZPos p_Z, FK2DEngine::ENUM_AlphaMode p_eMode /*= EAM_Default*/)
{
	ReorderCoordinatesIfNecessary( p_dX1, p_dY1, p_dX2, p_dY2, p_dX3, p_dY3, p_Color3, p_dX4, p_dY4, p_Color4 );

	SDrawOp op( m_pImpl->m_AbsoluteTransforms.back() );

	op.m_eMode				= p_eMode;
	op.m_unUsedVertices		= 4;
	op.m_Vertices[0]		= SDrawOp::SVertex( p_dX1, p_dY1, p_Color1 );
	op.m_Vertices[1]		= SDrawOp::SVertex( p_dX2, p_dY2, p_Color2 );

#ifdef FK_IPHONE
	op.m_Vertices[2]		= SDrawOp::SVertex( p_dX3, p_dY3, p_Color3 );
	op.m_Vertices[3]		= SDrawOp::SVertex( p_dX4, p_dY4, p_Color4 );
#else
	op.m_Vertices[3]		= SDrawOp::SVertex( p_dX3, p_dY3, p_Color3 );
	op.m_Vertices[2]		= SDrawOp::SVertex( p_dX4, p_dY4, p_Color4 );
#endif

	m_pImpl->m_Queues.back().ScheduleDrawOp( op, p_Z );
}
//--------------------------------------------------------------------
std::auto_ptr< FK2DEngine::CImageData >	FK2DEngine::CGraphics::CreateImage(const FK2DEngine::CBitmap &p_Src,
		unsigned int p_unSrcX, unsigned int p_unSrcY, unsigned int p_unSrcWidth,
		unsigned int p_unSrcHeight, unsigned int p_unBorderFlags)
{
	static const unsigned int s_unMaxSize = MAX_TEXTURE_SIZE;

	if(( p_unBorderFlags & EBF_Tileable ) == EBF_Tileable &&
		p_unSrcWidth == p_unSrcHeight &&
		( p_unSrcWidth & ( p_unSrcWidth - 1 ) ) == 0 &&
		p_unSrcWidth >= 64 )
	{
		boost::shared_ptr< CTexture > Texture( new CTexture( p_unSrcWidth ) );
		std::auto_ptr< CImageData >	Data;

		if( p_unSrcX == 0 && p_unSrcWidth == p_Src.Width() &&
			p_unSrcY == 0 && p_unSrcHeight == p_Src.Height() )
		{
			Data = Texture->TryAlloc( *this, m_pImpl->m_AbsoluteTransforms, 
				m_pImpl->m_Queues, Texture, p_Src, 0 );
		}
		else
		{
			CBitmap TrimmedSrc;
			TrimmedSrc.Resize( p_unSrcWidth, p_unSrcHeight );
			TrimmedSrc.Insert( p_Src, 0, 0, p_unSrcX, p_unSrcY, p_unSrcWidth, p_unSrcHeight );
			Data = Texture->TryAlloc( *this, m_pImpl->m_AbsoluteTransforms, m_pImpl->m_Queues, Texture, TrimmedSrc, 0 );
		}

		if( !Data.get() )
		{
			throw std::logic_error( "分配内部纹理块失败" );
		}
		return Data;
	}

	if(( p_unSrcWidth > s_unMaxSize - 2 ) || ( p_unSrcHeight > s_unMaxSize - 2 ))
	{
		CBitmap Bitmap( p_unSrcWidth, p_unSrcHeight );
		Bitmap.Insert( p_Src, 0, 0, p_unSrcX, p_unSrcY, p_unSrcWidth, p_unSrcHeight );
		std::auto_ptr< CImageData >		pImageData;
		pImageData.reset( new CLargeImageData( *this, Bitmap, s_unMaxSize - 2, s_unMaxSize -2, p_unBorderFlags ));
		return pImageData;
	}

	CBitmap bmp;
	ApplyBorderFlags( bmp, p_Src, p_unSrcX, p_unSrcY, p_unSrcWidth, p_unSrcHeight, p_unBorderFlags );

	for( SImpl::Textures::iterator i = m_pImpl->m_vecTextures.begin();
		i != m_pImpl->m_vecTextures.end(); ++i )
	{
		boost::shared_ptr< CTexture >		pTexture( *i );
		std::auto_ptr< CImageData >			pData;

		pData = pTexture->TryAlloc( *this, m_pImpl->m_AbsoluteTransforms, m_pImpl->m_Queues, pTexture, bmp, 1 );
		if( pData.get() )
		{
			return pData;
		}
	}	

	boost::shared_ptr< CTexture >	pTexture;
	pTexture.reset( new CTexture( s_unMaxSize ));
	m_pImpl->m_vecTextures.push_back( pTexture );

	std::auto_ptr< CImageData >		pData;
	pData = pTexture->TryAlloc( *this, m_pImpl->m_AbsoluteTransforms, m_pImpl->m_Queues, pTexture, bmp, 1 );
	if( !pData.get() )
	{
		throw std::logic_error( "分配内部纹理块失败" );
	}
	return pData;
}
//--------------------------------------------------------------------