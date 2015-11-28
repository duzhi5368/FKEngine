/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	DrawOpQueue
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "GraphicsCommon.h"
#include "DrawOp.h"
#include "Timing.h"
#include "StringConversion.h"
#include "LogManager.h"
#include "../../depend/boost/foreach.hpp"
#include "../../depend/boost/function.hpp"
#include "../../depend/boost/optional.hpp"
#include <algorithm>
#include <map>
#include <vector>
//--------------------------------------------------------------------
class FK2DEngine::CDrawOpQueue
{
private:
	typedef std::vector< SDrawOp >								DrawOps;
	typedef std::multimap< ZPos, boost::function< void() > >	CodeMap;
	struct SClipRect
	{
		int				m_nX;
		int				m_nY;
		unsigned int	m_unWidth;
		unsigned int	m_unHeight;
	};
private:
	DrawOps							m_DrawOps;
	CodeMap							m_CodeMap;
	std::vector< SClipRect >		m_ClipRectStack;
	boost::optional< SClipRect >	m_EffectiveRect;
private:
	void UpdateEffectiveRect()
	{
		if( m_ClipRectStack.empty() )
		{
			return m_EffectiveRect.reset();
		}

		SClipRect Result = { 0, 0, 0x7fffffff, 0x7fffffff };

		BOOST_FOREACH( const SClipRect& Rect, m_ClipRectStack )
		{
			int nRight = std::min< int >( Result.m_nX + Result.m_unWidth, Rect.m_nX + Rect.m_unWidth );
			int nBottom = std::min< int >( Result.m_nY + Result.m_unHeight, Rect.m_nY + Rect.m_unHeight );
			Result.m_nX = std::min< int >( Result.m_nX, Rect.m_nX );
			Result.m_nY = std::min< int >( Result.m_nY, Rect.m_nY );

			if( Result.m_nX >= nRight || Result.m_nY >= nBottom )
			{
				m_EffectiveRect.reset();
				return ;
			}

			Result.m_unWidth	= nRight - Result.m_nX;
			Result.m_unHeight	= nBottom - Result.m_nY;
		}

		int nFac = ClipRectBaseFactor();
		Result.m_nX *= nFac;
		Result.m_nY *= nFac;
		Result.m_unWidth *= nFac;
		Result.m_unHeight *= nFac;

		m_EffectiveRect = Result;
	}

public:
	void Swap( CDrawOpQueue& p_Other )
	{
		m_ClipRectStack.swap( p_Other.m_ClipRectStack );
		std::swap( m_EffectiveRect, p_Other.m_EffectiveRect );
		m_DrawOps.swap( p_Other.m_DrawOps );
		m_CodeMap.swap( p_Other.m_CodeMap );
	}

	void ScheduleDrawOp( SDrawOp p_Op, ZPos p_Z )
	{
	#ifdef FK_IPHONE
			assert( p_Op.m_unUsedVertices == 4 );	// 不支持绘制三角形和线
	#endif

		if( m_EffectiveRect )
		{
			const SClipRect& Rect	= *m_EffectiveRect;
			p_Op.m_nClipX			= Rect.m_nX;
			p_Op.m_nClipY			= Rect.m_nY;
			p_Op.m_unClipWidth		= Rect.m_unWidth;
			p_Op.m_unClipHeight		= Rect.m_unHeight;
		}
		else if( !m_ClipRectStack.empty() )
		{
			return;
		}

		p_Op.m_Z	= p_Z;
		m_DrawOps.push_back( p_Op );
	}

	void ScheduleGL( boost::function< void() > p_CustomCode, ZPos p_Z )
	{
		m_CodeMap.insert( std::make_pair( p_Z, p_CustomCode ) );
	}

	void BeginClipping( int p_nX, int p_nY, unsigned int p_unWidth, unsigned int p_unHeight )
	{
		SClipRect Rect = { p_nX, p_nY, p_unWidth, p_unHeight };
		m_ClipRectStack.push_back( Rect );
		UpdateEffectiveRect();
	}

	void EndClipping()
	{
		m_ClipRectStack.pop_back();
		UpdateEffectiveRect();
	}

	void PerformDrawOpsAndCode()
	{
		if( m_DrawOps.empty() )
		{
			BOOST_FOREACH( const CodeMap::value_type& fn, m_CodeMap )
			{
				fn.second();
			}
			return;
		}

		std::stable_sort( m_DrawOps.begin(), m_DrawOps.end() );

		// 循环对DrawOps进行自定义操作，若没有自定义行为，则我们仅进行一次绘制，无额外性能消耗
		DrawOps::const_iterator IteCur = m_DrawOps.begin();
		DrawOps::const_iterator IteLast = m_DrawOps.begin();
		CodeMap::const_iterator Ite = m_CodeMap.begin();

		while( true )
		{
			if( Ite == m_CodeMap.end() )
			{
				IteLast = m_DrawOps.end() - 1;
			}
			else
			{
				while( IteLast != m_DrawOps.end() - 1 && ( IteLast + 1 )->m_Z < Ite->first )
				{
					++IteLast;
				}
			}

			if( IteCur <= IteLast )
			{
				CRenderState rs;
				while( IteCur < IteLast )
				{
					DrawOps::const_iterator IteNext = IteCur + 1;
					IteCur->Perform( rs, &*IteNext );
					IteCur = IteNext;
				}

				IteLast->Perform( rs, 0 );
				++IteCur;
			}

			if( Ite == m_CodeMap.end() )
			{
				break;
			}
			else
			{
				Ite->second();
				++Ite;
			}
		}
	}

	void Clear()
	{
		m_CodeMap.clear();
		m_DrawOps.clear();
	}

	void CompileTo( VectexArray& p_Va )
	{
		if( !m_CodeMap.empty() )
		{
			throw std::logic_error("自定义码不允许记录在一个宏内");
		}

		p_Va.reserve( m_DrawOps.size() );
		std::stable_sort( m_DrawOps.begin(), m_DrawOps.end() );
		BOOST_FOREACH( const SDrawOp& Op, m_DrawOps )
		{
			Op.CompileTo( p_Va );
		}
	}
};