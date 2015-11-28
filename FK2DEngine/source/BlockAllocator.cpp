/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	BlockAllocator
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../include/BlockAllocator.h"
#include <stdexcept>
#include <vector>

//--------------------------------------------------------------------
struct FK2DEngine::CBlockAllocator::SImpl
{
	typedef std::vector< SBlock >		Blocks;
public:
	unsigned int	m_unWidth;
	unsigned int	m_unHeight;
	Blocks			m_vecBlocks;
	unsigned int	m_unFirstX;
	unsigned int	m_unFirstY;
	unsigned int	m_unMaxW;
	unsigned int	m_unMaxH;
public:
	void MarkBlockUsed( const SBlock& p_Block, unsigned int p_unWidth, unsigned int p_unHeight )
	{
		m_unFirstX += p_unWidth;
		if( m_unFirstX + p_unWidth >= m_unWidth )
		{
			m_unFirstX = 0;
			m_unFirstY += p_unHeight;
		}
		m_vecBlocks.push_back( p_Block );
	}

	bool IsBlockFree( const SBlock& p_Block )const
	{
		unsigned int unRight = p_Block.m_unLeft + p_Block.m_unWidth;
		unsigned int unBottom = p_Block.m_unTop + p_Block.m_unHeight;

		if( unRight > m_unWidth || unBottom > m_unHeight )
		{
			return false;
		}

		Blocks::const_iterator IteEnd = m_vecBlocks.end();
		for( Blocks::const_iterator Ite = m_vecBlocks.begin();
			Ite != IteEnd; ++Ite )
		{
			if( Ite->m_unLeft < unRight && p_Block.m_unLeft < Ite->m_unLeft + Ite->m_unWidth 
				&& Ite->m_unTop < unBottom && p_Block.m_unTop < Ite->m_unTop + Ite->m_unHeight )
			{
				return false;
			}
		}
		return true;
	}
};
//--------------------------------------------------------------------
FK2DEngine::CBlockAllocator::CBlockAllocator( unsigned int p_unWidth, unsigned int p_unHeight )
	: m_pImpl( new SImpl )
{
	m_pImpl->m_unWidth	= p_unWidth;
	m_pImpl->m_unHeight	= p_unHeight;
	m_pImpl->m_unFirstX	= 0;
	m_pImpl->m_unFirstY	= 0;
	m_pImpl->m_unMaxW	= p_unWidth;
	m_pImpl->m_unMaxH	= p_unHeight;
}
//--------------------------------------------------------------------
FK2DEngine::CBlockAllocator::~CBlockAllocator()
{

}
//--------------------------------------------------------------------
unsigned int FK2DEngine::CBlockAllocator::Width() const
{
	return m_pImpl->m_unWidth;
}
//--------------------------------------------------------------------
unsigned int FK2DEngine::CBlockAllocator::Height() const
{
	return m_pImpl->m_unHeight;
}
//--------------------------------------------------------------------
boost::optional< FK2DEngine::CBlockAllocator::SBlock >
	FK2DEngine::CBlockAllocator::Alloc( unsigned int p_unWidth, unsigned int p_unHeight )
{
	if( p_unWidth > Width() || p_unHeight > Height() )
	{
		return boost::optional< SBlock >();
	}
	if( p_unWidth > m_pImpl->m_unMaxW && p_unHeight > m_pImpl->m_unMaxH )
	{
		return boost::optional< SBlock >();
	}

	SBlock b = SBlock( m_pImpl->m_unFirstX, m_pImpl->m_unFirstY, p_unWidth, p_unHeight );
	if( m_pImpl->IsBlockFree(b) )
	{
		m_pImpl->MarkBlockUsed( b, p_unWidth, p_unHeight );
		return b;
	}

	unsigned int& x = b.m_unLeft;
	unsigned int& y = b.m_unTop;
	for( y = 0; y <= Height() - p_unHeight; y += 16 )
	{
		for( x = 0; x <= Width() - p_unWidth; x += 8 )
		{
			if( ! m_pImpl->IsBlockFree(b) )
			{
				continue;
			}

			while( y > 0 && m_pImpl->IsBlockFree( SBlock( x, y - 1, p_unWidth, p_unHeight ) ) )
			{
				--y;
			}
			while( x > 0 && m_pImpl->IsBlockFree( SBlock( x - 1, y, p_unWidth, p_unHeight ) ) )
			{
				--x;
			}

			m_pImpl->MarkBlockUsed( b, p_unWidth, p_unHeight );
			return b;
		}
	}

	m_pImpl->m_unMaxW = p_unWidth - 1;
	m_pImpl->m_unMaxH = p_unHeight - 1;
	return boost::optional< SBlock >( );
}
//--------------------------------------------------------------------
void FK2DEngine::CBlockAllocator::Free( unsigned int p_unLeft, unsigned int p_unTop )
{
	for( SImpl::Blocks::iterator Ite = m_pImpl->m_vecBlocks.begin();
		Ite != m_pImpl->m_vecBlocks.end(); ++Ite )
	{
		if( Ite->m_unLeft == p_unLeft && Ite->m_unTop == p_unTop )
		{
			m_pImpl->m_vecBlocks.erase( Ite );
			m_pImpl->m_unMaxW = m_pImpl->m_unWidth - 1;
			m_pImpl->m_unMaxH = m_pImpl->m_unHeight - 1;
			return;
		}
	}

	throw std::logic_error( "尝试释放一个无效块" );
}
//--------------------------------------------------------------------