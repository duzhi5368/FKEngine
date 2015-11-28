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

#pragma once

//--------------------------------------------------------------------
namespace FK2DEngine
{
	//--------------------------------------------------------------------
	//! 计时器虚接口
	class ITimer
	{
	public:
		virtual unsigned int GetTime() = 0;
		virtual void Play() = 0;
		virtual void Pause() = 0;
		virtual void Stop() = 0;
		virtual bool IsStop() const = 0;
		virtual bool IsPause() const = 0;
	};
	//--------------------------------------------------------------------
	//! 计时器
	class CTimer : public ITimer
	{
	public:
		CTimer();
		~CTimer();
	public:
		//! 获取从Play到当前流逝的时间（单位：毫秒）
		//! note: 不计算暂停时流逝的时间。最长时间50天左右
		unsigned int GetTime();

		//! 获取流逝的时间（单位：毫秒）
		//! note: 计算暂停时流逝的时间。最长时间50天左右
		unsigned int GetElapseTime();

		//! 开始计时 或 继续计时
		void		Play();

		//! 暂停计时
		void		Pause();

		//! 停止计时
		void		Stop();

		//! 是否暂停计时状态
		bool		IsPause() const;

		//! 是否停止计时状态
		bool		IsStop() const;
	private:
		//! 时间状态
		enum ENUM_TimerState
		{
			eTS_Running,					// 计时中
			eTS_Pause,						// 暂停计时
			eTS_Stop,						// 停止计时
		};
		unsigned int		m_dwStartTime;	// 开始时间
		unsigned int		m_dwElapse;		// 流逝的时间
		unsigned int		m_dwPauseTime;	// 暂停时间
		ENUM_TimerState		m_eState;		// 时钟状态
	};
	//--------------------------------------------------------------------
	//! 高精度计时器
	class CPreciseTimer : public ITimer
	{
	public:
		CPreciseTimer( int p_nPrecosion = 10000, bool p_nPlay = false );
		~CPreciseTimer();
	public:
		//! 设置计时器精度
		//@params [IN] p_nPrecosion 此值为1000时，GeTime()返回单位为毫秒
		void SetPrecosion( int p_nPrecosion = 10000 );

		//! 获取计时器精
		int GetPrecosion() const;

		//! 获取当前计时器读数
		unsigned int GeTime();

		//! 开始计时
		void Play();

		//! 停止计时
		void Stop();

		//! 暂停计时
		void Pause();

		//! 是否暂停计时
		bool IsPause() const;

		//! 是否停止计时
		bool IsStop() const;

	private:
		//! 获取系统计时器的值
		__int64 GetCurrentCount();
	private:
		//! 时间状态
		enum ENUM_TimerState
		{
			eTS_Running,					// 计时中
			eTS_Pause,						// 暂停计时
			eTS_Stop,						// 停止计时
		};
		static __int64	m_n64Freq;		// 计时器频率
		__int64			m_n64TimeBegin;	// 开始时刻
		__int64			m_n64TimeEnd;	// 停止时刻
		int				m_nPrecision;	// 计时器精度
		ENUM_TimerState	m_eTimerState;	// 计时器状态
	};
	//--------------------------------------------------------------------
}