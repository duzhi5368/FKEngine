/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	GuiBase
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	基本用户界面对象
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//------------------------------------------------------------------------------------------------------------------
// 用户界面消息列表

// GM_ 为主消息前缀		// GMA_ 为辅助消息 A 前缀		// GMB_ 为辅助消息 B 前缀

//基本消息
#define GM_Base					0

#define GM_Unknown				GM_Base+0
#define GM_Enable				GM_Base+1		//开启/关闭控件					GMA=1 开启		GMA=0 关闭
#define GM_Show					GM_Base+2		//显示/隐藏控件					GMA=1 显示		GMA=0 隐藏
#define GM_Focus				GM_Base+3		//获得/失去焦点					GMA=1 获得		GMB=0 失去
#define GM_MouseMove			GM_Base+4		//鼠标移动						GMA 输入系统指针
#define GM_SetPos				GM_Base+5		//设置位置						GMA X坐标		GMB Y坐标
#define GM_MovePos				GM_Base+6		//移动位置						GMA X相对坐标	GMB Y相对坐标
#define GM_SetSize				GM_Base+7		//设置尺寸						GMA 宽度		GMB 高度
#define GM_SetZOrder			GM_Base+8		//设置 Z 顺序					GMA Z 顺序类型	GMB 基准平面图像
#define GM_GetZOrder			GM_Base+9		//获取 Z 顺序基准平面图像		GMA Z 顺序类型

//输入消息
#define GM_Input				20

#define GM_LDBClick				GM_Input+0		//左键双击						GMA X坐标		GMB Y坐标
#define GM_RDBClick				GM_Input+1		//右键双击						GMA X坐标		GMB Y坐标
#define GM_LDown				GM_Input+2		//左键按下						GMA X坐标		GMB Y坐标
#define GM_RDown				GM_Input+3		//右键按下						GMA X坐标		GMB Y坐标
#define GM_LUp					GM_Input+4		//左键释放						GMA X坐标		GMB Y坐标
#define GM_RUp					GM_Input+5		//右键释放						GMA X坐标		GMB Y坐标

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//位置描述结构（相对父级左上角的位置）
	struct GuiLoca
	{
		LONG	X;
		LONG	Y;
		UINT	Width;
		UINT	Height;
	};
	//--------------------------------------------------------------------------------------------------------------
	//矩形描述结构
	struct GuiRect
	{
		int Left;
		int Top;
		int Right;
		int Bottom;		
	};
	//--------------------------------------------------------------------------------------------------------------
	//客户区遮罩图
	struct GuiMaskMap
	{
		int		nWidth;
		int		nHeight;
		UINT	nBytes;
		BYTE*	pBitMask;
	};
	//--------------------------------------------------------------------------------------------------------------
	//客户区范围
	class FK_DLL GuiClient
	{
	public:
		//客户区类型
		enum Type
		{
			NoClient,	//无客户区
			Rect,		//单矩形
			MaskMap		//遮罩图
		};

		Type			mType;

		//客户区范围
		union
		{
			GuiRect		mRect;
			GuiMaskMap	mMaskMap;
		};

	public:
		GuiClient();
		GuiClient( const GuiClient& refClient );
		GuiClient( int nLeft, int nTop, int nRight, int nBottom );

		//设置矩形客户区范围
		void FKFastcall SetClient( int nLeft, int nTop, int nRight, int nBottom );

		//检测指定坐标是否在客户区中
		bool FKFastcall IsOnClient( int x, int y );
	};
	//--------------------------------------------------------------------------------------------------------------
	class GuiDialog;
	//--------------------------------------------------------------------------------------------------------------
	//基本用户界面对象
	class FK_DLL GuiBase
	{
	public:
		bool			mbEnable;		//标识是否开启
		bool			mbVisable;		//标识是否可视
		bool			mbFocus;		//标识是否获得焦点

		GuiDialog*		mpDialog;		//从属用户界面对话框

		GuiLoca			mLoca;			//界面位置
		GuiClient		mClient;		//客户区范围

	public:
		GuiBase( GuiLoca* pLoca, GuiClient* pClient, GuiDialog* pDialog = NULL );
		virtual ~GuiBase() = 0;

		//检测指定坐标是否在客户区中
		bool FKFastcall IsOnClient( int x, int y );

	public:
		//向用户界面对象发送消息
		virtual LONG_PTR SendMsg( LONG_PTR nGM, LONG_PTR nGMA = 0, LONG_PTR nGMB = 0 ) = 0;
	};
	//--------------------------------------------------------------------------------------------------------------
	typedef Stack< GuiBase* >	GuiList;
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================

