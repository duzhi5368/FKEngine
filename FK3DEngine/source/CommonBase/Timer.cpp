/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Timer
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../../include/CommonBase/CommonBaseInsideHead.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//声明一秒钟的时间内的时钟频率（震荡次数）变量
	__int64 Timer::m_sTimerFrequency = 0;

	//--------------------------------------------------------------------------------------------------------------
	Timer::Timer()
		: m_bIsPause	(false)
		, m_BeginTime	(0)
		, m_PauseTime	(0)
		, m_dwPrecision	(1000)
	{
		if( m_sTimerFrequency == 0 )
		{
			//获得一秒钟内的时钟频率（震荡次数）
			LARGE_INTEGER LargeInt;
			if( QueryPerformanceFrequency( &LargeInt ) == TRUE )
			{
				m_sTimerFrequency = LargeInt.QuadPart;
			}
			else
			{
				Misc::ShowErrMsg( "Initialise timer failed." );
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	Timer::~Timer()
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	//重新开始计时
	void FKFastcall Timer::Start()
	{
		//获得当前时间（计时开始时间）
		LARGE_INTEGER LargeInt;
		QueryPerformanceCounter( &LargeInt );
		m_BeginTime = LargeInt.QuadPart;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取计时时间
	//
	//返回值：	从计时开始到调用此函数的时间（unsigned long 值）
	DWORD FKFastcall Timer::GetTime()
	{
		//计时开始时间减去当前时间得出计时时长
		LARGE_INTEGER LargeInt;
		QueryPerformanceCounter( &LargeInt );
		return (DWORD)( ( LargeInt.QuadPart - m_BeginTime ) * m_dwPrecision / m_sTimerFrequency );
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取计时时间
	//
	//计时单位时钟为秒，精度始终为微秒（百万分之一秒）。
	//
	//返回值：	从计时开始到调用此函数的时间（float 值）
	float FKFastcall Timer::GetTimeDouble()
	{
		LARGE_INTEGER LargeInt;
		QueryPerformanceCounter( &LargeInt );
		return (float)( ( LargeInt.QuadPart - m_BeginTime ) * 1000000 / m_sTimerFrequency ) / 1000000;
	}
	//--------------------------------------------------------------------------------------------------------------
	//暂停计时
	void FKFastcall Timer::Pause()
	{
		LARGE_INTEGER LargeInt;

	#ifdef _DEBUG	//调试模式时用暂停计时的标志避免双重暂停
		if( !m_bIsPause )
		{
			//获取暂停时的时间
			QueryPerformanceCounter( &LargeInt );
			m_PauseTime = LargeInt.QuadPart;
			m_bIsPause = true;
		}
		else
		{
			Misc::ShowErrMsg( "不能对同一个计时器进行两次暂停计时！" );
		}
	#else
		//获取暂停时的时间
		QueryPerformanceCounter( &LargeInt );
		m_PauseTime = LargeInt.QuadPart;
		m_bIsPause = true;
	#endif
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取是否暂停了计时
	bool FKFastcall Timer::IsPaused()
	{
		return m_bIsPause;
	}
	//--------------------------------------------------------------------------------------------------------------
	//撤消暂停计时
	void FKFastcall Timer::Resume()
	{
		LARGE_INTEGER LargeInt;

	#ifdef _DEBUG	//调试模式时用暂停计时的标志避免双重撤消暂停
		if( m_bIsPause )
		{
			//计时开始时间加上从暂停到撤消暂停之间的时长
			QueryPerformanceCounter( &LargeInt );
			m_BeginTime += LargeInt.QuadPart - m_PauseTime;
			m_bIsPause = false;
		}
		else
		{
			Misc::ShowErrMsg( "不能对同一个计时器进行两次撤消暂停计时！" );
		}
	#else
		//计时开始时间加上从暂停到撤消暂停之间的时长
		QueryPerformanceCounter( &LargeInt );
		m_BeginTime += LargeInt.QuadPart - m_PauseTime;
		m_bIsPause = false;
	#endif
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置时间格式
	//
	//dwPrecision：	时间格式值（默认值1000）如此值为 1000 则返回时间值单位为毫秒。
	void __fastcall Timer::SetFormat( const DWORD dwPrecision )
	{
		m_dwPrecision = dwPrecision;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
