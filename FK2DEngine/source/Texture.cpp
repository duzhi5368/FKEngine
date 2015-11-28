/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Texture
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../include/Graphics.h"
#include "../include/Texture.h"
#include "../include/TexChunk.h"
#include "../include/Bitmap.h"
#include "../include/Platform.h"
#include <stdexcept>

//--------------------------------------------------------------------
namespace FK2DEngine
{
	bool UndocumentedRetrofication = false;
}
//--------------------------------------------------------------------
FK2DEngine::CTexture::CTexture(unsigned int p_unSize)
	: m_Allocator( p_unSize, p_unSize )
	, m_unNum ( 0 )
{
	glGenTextures( 1, &m_Name );

	if( m_Name == static_cast< GLuint >( -1 ) )
	{
		throw std::runtime_error( "创建OpenGL纹理失败" );
	}

	glBindTexture( GL_TEXTURE_2D, m_Name );

#ifdef FK_IPHONE
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, m_Allocator.Width(), m_Allocator.Height(),
		0, GL_RGBA, GL_UNSIGNED_BYTE, 0 );
#else
	glTexImage2D( GL_TEXTURE_2D, 0, 4, m_Allocator.Width(), m_Allocator.Height(),
		0, GL_RGBA, GL_UNSIGNED_BYTE, 0 );
#endif

	if( UndocumentedRetrofication )
	{
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	}
	else
	{
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	}

#ifdef FK_WINDOWS
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
#else
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
#endif
}
//--------------------------------------------------------------------
FK2DEngine::CTexture::~CTexture()
{
	glDeleteTextures( 1, &m_Name );
}
//--------------------------------------------------------------------
unsigned int FK2DEngine::CTexture::Size() const
{
	return m_Allocator.Width();
}
//--------------------------------------------------------------------
GLuint FK2DEngine::CTexture::TexName() const
{
	return m_Name;
}
//--------------------------------------------------------------------
std::auto_ptr< FK2DEngine::CTexChunk > FK2DEngine::CTexture::TryAlloc( CGraphics& pGraphics, Transforms& p_Transforms,
		DrawOpQueueStack& p_Queues, boost::shared_ptr< CTexture > p_Ptr,
		const CBitmap& p_Bitmap, unsigned int p_unPadding )
{
	std::auto_ptr< FK2DEngine::CTexChunk >		Result;
	boost::optional< CBlockAllocator::SBlock >	Block = m_Allocator.Alloc( p_Bitmap.Width(), p_Bitmap.Height() );
	if( !Block )
	{
		return Result;
	}

	Result.reset( new CTexChunk( pGraphics, p_Transforms, p_Queues, p_Ptr, 
		Block->m_unLeft + p_unPadding, Block->m_unTop + p_unPadding,
		Block->m_unWidth - 2 * p_unPadding, Block->m_unHeight - 2 * p_unPadding, p_unPadding ) );

	glBindTexture( GL_TEXTURE_2D, m_Name );
	glTexSubImage2D( GL_TEXTURE_2D, 0, Block->m_unLeft, Block->m_unTop, Block->m_unWidth,
		Block->m_unHeight, CColor::GL_FORMAT, GL_UNSIGNED_BYTE, p_Bitmap.Data() );

	m_unNum += 1;
	return Result;
}
//--------------------------------------------------------------------
void FK2DEngine::CTexture::Free( unsigned int p_unX, unsigned int p_unY )
{
	m_Allocator.Free( p_unX, p_unY );
	m_unNum -= 1;
}
//--------------------------------------------------------------------
FK2DEngine::CBitmap FK2DEngine::CTexture::ToBitmap(unsigned int p_unX, 
		unsigned int p_unY, unsigned int p_unWidth, unsigned int p_unHeight) const
{
#ifdef FK_IPHONE
	throw std::logic_error( "CTexture::ToBitmap 暂不支持 iOS" );
#else
	FK2DEngine::CBitmap	FullTexture( Size(), Size());
	glBindTexture( GL_TEXTURE_2D, m_Name );
	glGetTexImage( GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, FullTexture.Data() );
	FK2DEngine::CBitmap	Bitmap( p_unWidth, p_unHeight );
	Bitmap.Insert( FullTexture, -int(p_unX), -int(p_unY) );

	return Bitmap;
#endif
}