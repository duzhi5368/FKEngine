/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	AnimationCursor
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "Bitmap.h"
#include "Image.h"
#include <vector>
//--------------------------------------------------------------------
namespace FK2DEngine
{
	//--------------------------------------------------------------------
	//! 动态光标类
	class CAnimationCursor
	{
	public:
		CAnimationCursor();
		~CAnimationCursor();
	public:
		//! 初始化
		//@params [IN] p_nSingleWidth	单副图片的宽度
		void Init( CBitmap& p_pBitmap, const std::wstring& p_szName,
			int p_nSingleWidth, int p_nSingleHeight );
		
		//! 绘制
		//@params [IN] p_bUpdate 是否更新一帧
		void FKDraw( bool p_bUpdate = false );

		//! 获取光标名称
		std::wstring GetName() const;
	private:
		CBitmap*				m_pBitmap;		// 光标组图片
		std::vector< CImage >	m_vecAnim;		// 光标组动画
		std::wstring			m_szName;		// 光标组名称
		int						m_nCurFrame;	// 当前帧数
		int						m_nMaxFrame;	// 最大帧数
	};
	//--------------------------------------------------------------------
}