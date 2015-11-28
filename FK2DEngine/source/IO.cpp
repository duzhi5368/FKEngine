/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	IO
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../include/IO.h"
#include <cassert>
#include <cstring>

//--------------------------------------------------------------------
void FK2DEngine::CReader::Read( void* p_pDestBuffer, std::size_t p_unLength )
{
	m_pRes->Read( m_unPos, p_unLength, p_pDestBuffer );
	Seek( p_unLength );
}
//--------------------------------------------------------------------
void FK2DEngine::CWriter::Write( const void* p_pSourceBuffer, std::size_t p_unLength )
{
	if( m_unPos + p_unLength > m_pRes->Size() )
	{
		m_pRes->Resize( m_unPos + p_unLength );
	}

	m_pRes->Write( m_unPos, p_unLength, p_pSourceBuffer );
	Seek( p_unLength );
}
//--------------------------------------------------------------------
std::size_t FK2DEngine::CBuffer::Size() const
{
	return m_vecBuf.size();
}
//--------------------------------------------------------------------
void FK2DEngine::CBuffer::Resize( std::size_t p_unNewSize )
{
	m_vecBuf.resize( p_unNewSize );
}
//--------------------------------------------------------------------
void FK2DEngine::CBuffer::Read( std::size_t p_unOffset, std::size_t p_unLength,
		  void* p_pDestBuffer ) const
{
	assert( p_unOffset + p_unLength <= Size() );
	if( p_unLength != 0 )
	{
		std::memcpy( p_pDestBuffer, &m_vecBuf[p_unOffset], p_unLength );
	}
}
//--------------------------------------------------------------------
void FK2DEngine::CBuffer::Write( std::size_t p_unOffset, std::size_t p_unLength,
		   const void* p_pSourceBuffer )
{
	assert( p_unOffset + p_unLength <= Size() );
	if( p_unLength != 0 )
	{
		std::memcpy( &m_vecBuf[p_unOffset], p_pSourceBuffer, p_unLength );
	}
}
//--------------------------------------------------------------------
void FK2DEngine::LoadFile( CBuffer& p_Buffer, const std::wstring& p_szFileName )
{
	CFile tagFile( p_szFileName );
	p_Buffer.Resize( tagFile.Size() );
	tagFile.Read( 0, p_Buffer.Size(), p_Buffer.Data() );
}
//--------------------------------------------------------------------
void FK2DEngine::SaveFile( const CBuffer& p_Buffer, const std::wstring& p_szFileName )
{
	CFile tagFile( p_szFileName, EFM_Replace );
	tagFile.Write( 0, p_Buffer.Size(), p_Buffer.Data() );
}
//--------------------------------------------------------------------