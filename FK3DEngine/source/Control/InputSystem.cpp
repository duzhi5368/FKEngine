/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	InputSystem
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../../include/Control/ControlInsideHeader.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	InputSystem::InputSystem()
		: mbEnableKeyboard		(true)
		, mbEnableMouse			(true)
		, mMouseSpeed			(100)
	{
		memset( mbKeyState, 0, sizeof(bool)*( KEY_BUF_SIZE*3 + 3*4 ) );

		mMouseLastDownTime[0] = 0;
		mMouseLastDownTime[1] = 0;
		mMouseLastDownTime[2] = 0;
		mMouseDBClickTime[0] = 200;
		mMouseDBClickTime[1] = 200;
		mMouseDBClickTime[2] = 200;

		mMouseX = mMouseY = mMouseZ = 0;
		mMouseRelX = mMouseRelY = mMouseRelZ = 0;
	}
	//--------------------------------------------------------------------------------------------------------------
	InputSystem::~InputSystem()
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	//打开键盘输入
	void FKFastcall InputSystem::EnableKeyboard( bool bEnable )
	{
		mbEnableKeyboard = bEnable;
	}
	//--------------------------------------------------------------------------------------------------------------
	//打开鼠标输入
	void FKFastcall InputSystem::EnableMouse( bool bEnable )
	{
		mbEnableMouse = bEnable;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置鼠标双击判定间隔时间（毫秒）
	void FKFastcall InputSystem::SetMouseDBClickTime( DWORD dwLeftTime, DWORD dwRightTime, DWORD dwMidTime )
	{
		mMouseDBClickTime[0] = dwLeftTime;
		mMouseDBClickTime[1] = dwRightTime;
		mMouseDBClickTime[2] = dwMidTime;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置鼠标移动速度
	void FKFastcall InputSystem::SetMouseSpeed( LONG nMoveSpeed )
	{
		mMouseSpeed = nMoveSpeed;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置鼠标位置
	void FKFastcall InputSystem::SetMousePos( int x, int y, int z )
	{
		FixPosByMouseRect( &x, &y, &z );

		mMouseX = x;
		mMouseY = y;
		mMouseZ = z;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置鼠标移动区域
	void FKFastcall InputSystem::SetMouseRect( MouseRect* pMouseRect )
	{
		mMouseMoveRect = *pMouseRect;

		FixPosByMouseRect( &mMouseX, &mMouseY, &mMouseZ );
	}
	//--------------------------------------------------------------------------------------------------------------
	//根据鼠标移动区域修正坐标
	void FKFastcall InputSystem::FixPosByMouseRect( int* pX, int* pY, int* pZ )
	{
		if( *pX > mMouseMoveRect.Right )
			*pX = mMouseMoveRect.Right;
		else if( *pX < mMouseMoveRect.Left )
			*pX = mMouseMoveRect.Left;

		if( *pY > mMouseMoveRect.Bottom )
			*pY = mMouseMoveRect.Bottom;
		else if( *pY < mMouseMoveRect.Top )
			*pY = mMouseMoveRect.Top;

		if( *pZ > mMouseMoveRect.WheelMax )
			*pZ = mMouseMoveRect.WheelMax;
		else if( *pZ < mMouseMoveRect.WheelMin )
			*pZ = mMouseMoveRect.WheelMin;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
