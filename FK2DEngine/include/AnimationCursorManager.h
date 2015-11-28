/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	AnimationCursorManager
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "AnimationCursor.h"
#include "Singleton.h"
#include <map>
//--------------------------------------------------------------------
namespace FK2DEngine
{
	//--------------------------------------------------------------------
	typedef std::map< std::wstring, CAnimationCursor > CursorMap;
	//--------------------------------------------------------------------
	//! 动态光标管理类
	class CAnimationCursorManager
	{
	public:
		CAnimationCursorManager();
		~CAnimationCursorManager();
	public:
		//! 初始化
		//@remarks [IN] p_nFPS	每秒显示帧数
		//@remarks [IN] p_nHotX	光标的焦点X偏移
		//@remarks [IN]	P_nHotY 光标的焦点Y偏移
		void Init( CGraphics& p_Graphics, int p_nFPS = 30, int p_nHotX = 0, int p_nHotY = 0 );
		//! 开启/关闭光标管理类
		void Enable( bool p_bEnable = true );
		//! 是否开启光标管理类
		bool Enabled();
		//! 增加一个动态光标
		bool AddCursor( CBitmap& p_Bitmap, const std::wstring& p_szCursorName,
			int p_nSingleWidth, int p_nSingleHeight );
		//! 设置当前显示的动态光标
		void SetAnimCursor( const std::wstring& p_szCursorName );
		//! 返回渲染器
		CGraphics& GetGraphics();

		//! 逻辑帧更新
		void Update( float p_fDetla );
		//! 显示帧更新
		void FKDraw();
	private:
		bool					m_bEnable;			// 是否采用动态光标
		CAnimationCursor*		m_pCurCursor;		// 当前使用的动态光标
		CursorMap				m_CursorMap;		// 动态光标组
		float					m_fDelta;			// 图片更新间隔
		float					m_fTime;			// 当前已过时间
		int						m_nHotX;			// 焦点x偏移
		int						m_nHotY;			// 焦点y偏移
		CGraphics*				m_pGraphics;		// 渲染器指针
		bool					m_bNeedUpdate;		// 是否需要更新光标帧
	private:
		DECLARE_SINGLETON_CLASS( CAnimationCursorManager );
	};
	//--------------------------------------------------------------------
}

typedef FK2DEngine::TSingleton< FK2DEngine::CAnimationCursorManager >	SSAnimationCursorManager;