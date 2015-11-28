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

#pragma once

//--------------------------------------------------------------------
#include "../../depend/boost/optional.hpp"
#include "../../depend/boost/scoped_ptr.hpp"
//--------------------------------------------------------------------
namespace FK2DEngine
{
	class CBlockAllocator
	{
	private:
		struct							SImpl;
		boost::scoped_ptr< SImpl >		m_pImpl;
	public:
		struct SBlock
		{
			unsigned int		m_unLeft;
			unsigned int		m_unTop;
			unsigned int		m_unWidth;
			unsigned int		m_unHeight;

			SBlock()
			{

			}
			SBlock( unsigned int p_unLeft, unsigned int p_unTop,
				unsigned int p_unWidth, unsigned int p_unHeight )
				: m_unLeft ( p_unLeft )
				, m_unTop ( p_unTop )
				, m_unWidth ( p_unWidth )
				, m_unHeight ( p_unHeight )
			{

			}
		};
	public:
		CBlockAllocator( unsigned int p_unWidth, unsigned int p_unHeight );
		~CBlockAllocator();
	public:
		unsigned int Width() const;
		unsigned int Height() const;

		boost::optional< SBlock >	Alloc( unsigned int p_unWidth, unsigned int p_unHeight );
		void						Free( unsigned int p_unLeft, unsigned int p_unTop );
	};
}