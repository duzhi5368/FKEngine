/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	UIManager
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "UIControl.h"
#include "Graphics.h"
#include "Singleton.h"
#include "../../depend/boost/utility.hpp"
#include "Font.h"
#include <list>
//--------------------------------------------------------------------
namespace FK2DEngine
{
	//--------------------------------------------------------------------
	class CRadioButton;
	//--------------------------------------------------------------------
	class CGUIManager : boost::noncopyable
	{
	public:
		CGUIManager();
		~CGUIManager();
	public:
		//! 初始化，设定画布
		void Init( CGraphics& p_Canvas );
		//! 获取画布
		CGraphics& GetGraphics();

		//! 注册控件
		void AddCtrl( IGUIControl* p_pCtrl );
		//! 注销控件
		void DelCtrl( IGUIControl* p_pCtrl );

		//! 使控件获得焦点
		void SetFocus( IGUIControl* p_pCtrl );
		
		//! 更新GUI系统
		void Update( float p_fDelta );

		//! 绘制GUI系统
		void FKDraw();

		//! 增加单选按钮
		void AddRadio( CRadioButton* p_pRadio );
		//! 删除单选按钮
		void DelRadio( CRadioButton* p_pRadio );
		//! 选中单选按钮
		void CheckRadio( CRadioButton* p_pRadio );

		//! 获取根控件
		IGUIControl* GetRootCtrl();

	private:
		//! 处理控件的鼠标事件
		void ProcessMouseEvent( IGUIControl* p_pCtrl );
	private:
		std::list< IGUIControl* >		m_listControl;		// 控件列表
		CGraphics*						m_pGraphics;		// 渲染指针
		IGUIControl*					m_pFocusedCtrl;		// 获得焦点的控件
		IGUIControl*					m_pLockedCtrl;		// 被锁定的控件
		IGUIControl*					m_pHoveredCtrl;		// 鼠标下的控件
		std::list< CRadioButton* >		m_ListRadioButton;	// 单选按钮列表
		CFont*							m_pDefaultFont;		// 默认字体
		IGUIControl*					m_pRootCtrl;		// 根控件
	private:
		DECLARE_SINGLETON_CLASS( CGUIManager );
	};
	//--------------------------------------------------------------------
}

typedef FK2DEngine::TSingleton< FK2DEngine::CGUIManager >	SSGUIManager;