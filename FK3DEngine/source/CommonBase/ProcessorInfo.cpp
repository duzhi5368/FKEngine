/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	ProcessorInfo
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

	__int64		ProcessorInfo::mqwFrequency		= 0;		// CPU 频率
	char		ProcessorInfo::mCPUName[128]	= { NULL };	// CPU 名称
	CPUInfo		ProcessorInfo::mCPUInfo			= { NULL };	// CPU 信息

	//--------------------------------------------------------------------------------------------------------------

	//内联函数声明
	FKForceInline static char* TranslateAssociativeWays( unsigned int nWays, char* buf );
	FKForceInline static void TranslateTLB( ProcessorTLB* tlb );
	FKForceInline static void TranslateCache( ProcessorCache* cache );

	//位检测宏
	#define CheckBit( var, bit )	( ( var & (1<<bit) ) ? true : false )

	//--------------------------------------------------------------------------------------------------------------
	//获取 CPU 频率
	__int64 ProcessorInfo::GetCPUFrequency( unsigned int nMeasureTime )
	{
		//如果 CPUID 指令不可用则返回
		if( !_CheckCPUIDPresence() )
			return 0;

		//如果以获取 CPU 频率则直接返回
		if( mqwFrequency > 0 )
			return mqwFrequency;

		//调用 CPUID 指令
		unsigned long reg;
		__asm
		{
			mov eax, 1;
			cpuid
			mov reg, edx;
		}

		//检测是否支持（Real Date Time Stamp Counter）实时时间计数器功能
		if( !( reg & (1<<4) ) )
			return 0;

		//使用高精度计时器以检测 CPU 频率
		__int64 starttime, endtime, timedif, freq, start, end, dif;
		if( !QueryPerformanceFrequency( (LARGE_INTEGER*)&freq ) )
			return 0;

		//设置当前线程为最高优先权（实时）并且在多处理器系统中使用第一块处理器
		HANDLE hProcess = GetCurrentProcess();
		HANDLE hThread = GetCurrentThread();
		unsigned long dwCurPriorityClass = GetPriorityClass( hProcess );
		int iCurThreadPriority = GetThreadPriority( hThread );
		unsigned long dwProcessMask, dwSystemMask, dwNewMask = 1;
		GetProcessAffinityMask( hProcess, &dwProcessMask, &dwSystemMask );

		SetPriorityClass( hProcess, REALTIME_PRIORITY_CLASS );
		SetThreadPriority( hThread, THREAD_PRIORITY_TIME_CRITICAL );
		SetProcessAffinityMask( hProcess, dwNewMask );

		//结束调用 CPUID 指令
		__asm cpuid

		//开始高精度计时
		QueryPerformanceCounter( (LARGE_INTEGER*)&starttime );

		//获取当前 CPU 时钟
		__asm 
		{
			rdtsc;
			mov dword ptr [start+4], edx;
			mov dword ptr [start], eax;
		}

		//暂停线程指定的测量时间
		Sleep( nMeasureTime );

		//获取计时长度
		QueryPerformanceCounter( (LARGE_INTEGER*)&endtime );

		//在此获取 CPU 时钟
		__asm 
		{
			rdtsc;
			mov dword ptr [end+4], edx;
			mov dword ptr [end], eax;
		}

		//恢复默认线程优先级及 CPU 状态
		SetProcessAffinityMask( hProcess, dwProcessMask );
		SetThreadPriority( hThread, iCurThreadPriority );
		SetPriorityClass( hProcess, dwCurPriorityClass );

		//计算时间和时钟差值
		dif = end - start;
		timedif = endtime - starttime;

		//计算并返回 CPU 频率
		mqwFrequency = (__int64)( (float)dif / ( (float)timedif / freq ) );

		return mqwFrequency;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取 CPU 信息
	const CPUInfo* ProcessorInfo::GetCPUInfo()
	{
		unsigned long eaxreg, ebxreg, ecxreg, edxreg;

		//如果 CPUID 指令不可用则返回
		if( !_CheckCPUIDPresence() )
			return NULL;

		//获取调用 CPUID 指令
		__asm
		{
			mov eax, 0;
			cpuid
			mov eaxreg, eax;
			mov ebxreg, ebx;
			mov edxreg, edx;
			mov ecxreg, ecx;
		}

		//获取 CPU 厂商字符串
		*( (unsigned long*)mCPUInfo.szVendor ) = ebxreg;
		*( (unsigned long*)(mCPUInfo.szVendor+4) ) = edxreg;
		*( (unsigned long*)(mCPUInfo.szVendor+8) ) = ecxreg;

		//获取 CPUID 最大支持等级
		mCPUInfo.MaxSupportedLevel = eaxreg & 0xFFFF;

		//获取扩展 CPUID 最大支持等级
		__asm
		{
			mov eax, 0x80000000;
			cpuid
			mov eaxreg, eax;
		}
		mCPUInfo.MaxSupportedExtendedLevel = eaxreg;

		//根据 CPU 厂商类型调用不同的解析函数
		switch (ebxreg)
		{
		case 0x756E6547:	//GennneIntel
			_AnalyzeIntelProcessor();
			break;
		case 0x68747541:	//AuthenticAMD
			_AnalyzeAMDProcessor();
			break;
		case 0x69727943:	//CyrixInstead
		default:			//未知 CPU 厂商
			_AnalyzeUnknownProcessor();
			break;
		}

		//返回 CPU 信息变量指针
		return &mCPUInfo;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取 CPU 说明信息字符串
	String ProcessorInfo::GetCPUInfoText()
	{
		//测量 CPU 频率
		GetCPUFrequency( 50 );

		//获取 CPU 信息
		GetCPUInfo();

		//信息存储字符串
		String strInfo;
		strInfo.SetString(
			"--------------------------------------------------------------------------------------------\n"
			"-                              自由骑士3D游戏引擎 CPU 统计信息                             -\n"
			"--------------------------------------------------------------------------------------------\n"
			"\nCPU 标准信息：\n" );

		strInfo += "\n名称             ";		strInfo += mCPUName;
		strInfo += "\n频率             ";		strInfo += (int)( (float)mqwFrequency / 1000000.0f );
		strInfo += "\n厂商             ";		strInfo += mCPUInfo.szVendor;
		strInfo += "\n系列             ";		strInfo += mCPUInfo.szFamily;
		strInfo += "\n扩展系列         ";		strInfo += (int)mCPUInfo.nExtendedFamily;
		strInfo += "\n型号             ";		strInfo += mCPUInfo.szModel;
		strInfo += "\n扩展型号         ";		strInfo += (int)mCPUInfo.nExtendedModel;
		strInfo += "\n类型             ";		strInfo += mCPUInfo.szType;
		strInfo += "\n商标             ";		strInfo += mCPUInfo.szBrandID;

		if( mCPUInfo._Ext.PN_ProcessorSerialNumber )
		{
			strInfo += "\n处理器序列号     ";		strInfo += mCPUInfo.szProcessorSerial;
		}
		else
		{
			strInfo += "\n处理器序列号     关闭";
		}


		strInfo += "\n\n\nCPU 配置信息：\n";

		strInfo += "\n一级指令缓存            ";		strInfo += mCPUInfo._L1.Instruction.szCache;
		strInfo += "\n一级数据缓存            ";		strInfo += mCPUInfo._L1.Data.szCache;
		strInfo += "\n二级缓存                ";		strInfo += mCPUInfo._L2.szCache;
		strInfo += "\n三级缓存                ";		strInfo += mCPUInfo._L3.szCache;
		strInfo += "\n调试缓存                ";		strInfo += mCPUInfo._Trace.szCache;
		strInfo += "\n指令TLB                 ";		strInfo += mCPUInfo._Instruction.szTLB;
		strInfo += "\n数据TLB                 ";		strInfo += mCPUInfo._Data.szTLB;


		char szCPUIDLevelNum [128] = { NULL };

		itoa( mCPUInfo.MaxSupportedLevel, szCPUIDLevelNum, 16 );
		strInfo += "\n\n最大支持 CPUID 指令等级           0x";		strInfo += szCPUIDLevelNum;

		itoa( mCPUInfo.MaxSupportedExtendedLevel, szCPUIDLevelNum, 16 );
		strInfo += "\n最大支持扩展 CPUID 指令等级       0x";		strInfo += szCPUIDLevelNum;


		strInfo += "\n\n\nCPU 扩展信息：\n";

		const char szYes[]	= "是";
		const char szNo[]	= "否";

		strInfo += "\nAA64   AMD 64 位架构                  ";		strInfo += mCPUInfo._Ext.AA64_AMD64BitArchitecture							? szYes : szNo;
		strInfo += "\nACPI   温度探测/时钟控制              ";		strInfo += mCPUInfo._Ext.ACPI_ThermalMonitorAndClockControl					? szYes : szNo;
		strInfo += "\nAPIC   高级可编程中断控制器           ";		strInfo += mCPUInfo._Ext.APIC_AdvancedProgrammableInterruptController		? szYes : szNo;
		strInfo += "\n       高级可编程中断控制器编号       ";		strInfo += (int)mCPUInfo._Ext.APIC_ID;
		strInfo += "\nCLFSH  CLFLUSH 指令支持               ";		strInfo += mCPUInfo._Ext.CLFSH_CFLUSH_Instruction							? szYes : szNo;
		strInfo += "\n       CLFLUSH 指令缓存大小           ";		strInfo += (int)mCPUInfo._Ext.CLFLUSH_InstructionCacheLineSize;
		strInfo += "\nCMOV   条件数据传送和比较指令集       ";		strInfo += mCPUInfo._Ext.CMOV_ConditionalMoveAndCompareInstructions			? szYes : szNo;
		strInfo += "\nCX8    COMPXCHG8B 指令                ";		strInfo += mCPUInfo._Ext.CX8_COMPXCHG8B_Instruction							? szYes : szNo;
		strInfo += "\nDE     扩展调试指令                   ";		strInfo += mCPUInfo._Ext.DE_DebuggingExtensions								? szYes : szNo;
		strInfo += "\nDS     调试存储器                     ";		strInfo += mCPUInfo._Ext.DS_DebugStore										? szYes : szNo;
		strInfo += "\nFGPAT  页属性表                       ";		strInfo += mCPUInfo._Ext.FGPAT_PageAttributeTable							? szYes : szNo;


		strInfo += "\nFPU    浮点处理单元                   ";		strInfo += mCPUInfo._Ext.FPU_FloatingPointUnit								? szYes : szNo;
		strInfo += "\nFXSR   快速 SIMD 指令集存储/恢复      ";		strInfo += mCPUInfo._Ext.FXSR_FastStreamingSIMD_ExtensionsSaveRestore		? szYes : szNo;
		strInfo += "\nHT     超线程                         ";		strInfo += mCPUInfo._Ext.HT_HyperThreading									? szYes : szNo;
		strInfo += "\nIA64   Intel 64 位架构                ";		strInfo += mCPUInfo._Ext.IA64_Intel64BitArchitecture						? szYes : szNo;
		strInfo += "\nMCA    机器检测架构                   ";		strInfo += mCPUInfo._Ext.MCA_MachineCheckArchitecture						? szYes : szNo;
		strInfo += "\nMCE    机器检测异常                   ";		strInfo += mCPUInfo._Ext.MCE_MachineCheckException							? szYes : szNo;
		strInfo += "\nMMX    多媒体指令集                   ";		strInfo += mCPUInfo._Ext.MMX_MultimediaExtensions							? szYes : szNo;
		strInfo += "\nMMX+   扩展多媒体指令集               ";		strInfo += mCPUInfo._Ext.EMMX_MultimediaExtensions							? szYes : szNo;
		strInfo += "\nMSR    模块细节寄存器                 ";		strInfo += mCPUInfo._Ext.MSR_ModelSpecificRegisters							? szYes : szNo;
		strInfo += "\nMTRR   内存类型范围寄存器             ";		strInfo += mCPUInfo._Ext.MTRR_MemoryTypeRangeRegisters						? szYes : szNo;

		strInfo += "\nPAE    物理地址扩展                   ";		strInfo += mCPUInfo._Ext.PAE_PhysicalAddressExtension						? szYes : szNo;
		strInfo += "\nPGE    PTE 全局标志                   ";		strInfo += mCPUInfo._Ext.PGE_PTE_GlobalFlag									? szYes : szNo;
		strInfo += "\nPSE    页大小扩展                     ";		strInfo += mCPUInfo._Ext.PSE_PageSizeExtensions								? szYes : szNo;
		strInfo += "\nPSE36  36 位页大小扩展                ";		strInfo += mCPUInfo._Ext.PSE36_36bitPageSizeExtension						? szYes : szNo;

		strInfo += "\nSEP    快速系统级调用                 ";		strInfo += mCPUInfo._Ext.SEP_FastSystemCall									? szYes : szNo;
		strInfo += "\nSS     自探测                         ";		strInfo += mCPUInfo._Ext.SS_SelfSnoop										? szYes : szNo;
		strInfo += "\nSSE    数据流单指令多数据扩展指令集   ";		strInfo += mCPUInfo._Ext.SSE_StreamingSIMD_Extensions						? szYes : szNo;
		strInfo += "\nSSE2   数据流单指令多数据扩展指令集 2 ";		strInfo += mCPUInfo._Ext.SSE2_StreamingSIMD2_Extensions						? szYes : szNo;

		strInfo += "\nTM     温度探测                       ";		strInfo += mCPUInfo._Ext.TM_ThermalMonitor									? szYes : szNo;
		strInfo += "\nTSC    时间计数器                     ";		strInfo += mCPUInfo._Ext.TSC_TimeStampCounter								? szYes : szNo;
		strInfo += "\nVME    虚拟 8086 模式增强             ";		strInfo += mCPUInfo._Ext.VME_Virtual8086ModeEnhancements					? szYes : szNo;
		strInfo += "\n       3DNow! 指令集                  ";		strInfo += mCPUInfo._Ext._3DNOW_InstructionExtensions						? szYes : szNo;
		strInfo += "\n       增强 3DNow! 指令集             ";		strInfo += mCPUInfo._Ext._E3DNOW_InstructionExtensions						? szYes : szNo;

		return strInfo;
	}
	//--------------------------------------------------------------------------------------------------------------
	//将 CPU 说明信息写入指定文本文件
	bool ProcessorInfo::WriteCPUInfoTextFile( const char* szFileName )
	{
		//获取 CPU 说明信息
		String& szInfo = GetCPUInfoText();

		//将 CPU 说明信息写入指定文件中
		FILE* pFile = fopen( szFileName, "wt" );
		if( pFile == NULL )
			return false;

		fwrite( szInfo.GetCharPtr(), 1, szInfo.mnCharLength, pFile );

		fclose( pFile );

		return true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//解析 AMD 处理器信息
	bool ProcessorInfo::_AnalyzeAMDProcessor()
	{
		unsigned long eaxreg, ebxreg, ecxreg, edxreg;

		//如果 CPUID 指令不可用则返回
		if( !_CheckCPUIDPresence() )
			return false;

		//调用 CPUID 指令
		__asm
		{
			mov eax, 1;
			cpuid
			mov eaxreg, eax;
			mov ebxreg, ebx;
			mov edxreg, edx;
		}

		//获取型号（model）、系列（family）、次版本（stepping）和类型（type）。
		mCPUInfo.nStepping	= eaxreg & 0xF;
		mCPUInfo.nModel		= (eaxreg >> 4) & 0xF;
		mCPUInfo.nFamily	= (eaxreg >> 8) & 0xF;
		mCPUInfo.nType		= (eaxreg >> 12) & 0x3;

		//解释 CPU 类型
		switch (mCPUInfo.nType)
		{
		case 0:
			strcpy( mCPUInfo.szType, "原始 OEM" );
			strcpy( mCPUName, mCPUInfo.szType );
			strcat( mCPUName, " " );
			break;
		case 1:
			strcpy( mCPUInfo.szType, "超频" );
			strcpy( mCPUName, mCPUInfo.szType);
			strcat( mCPUName, " " );
			break;
		case 2:
			strcpy( mCPUInfo.szType, "降频" );
			strcpy( mCPUName, mCPUInfo.szType );
			strcat( mCPUName, " " );
			break;
		case 3:
			strcpy( mCPUInfo.szType, "保留" );
			break;
		default:
			strcpy( mCPUInfo.szType, "未知" );
			break;
		}

		//检测处理器是否支持商标字符串 CPUID 指令级别
		if (mCPUInfo.MaxSupportedExtendedLevel >= 0x80000004)
		{
			//如果支持则读取数据
			char tmp[52];
			memset( tmp, 0, sizeof(tmp) );

			__asm
			{
				mov eax, 0x80000002;
				cpuid
				mov dword ptr [tmp], eax;
				mov dword ptr [tmp+4], ebx;
				mov dword ptr [tmp+8], ecx;
				mov dword ptr [tmp+12], edx;
				mov eax, 0x80000003;
				cpuid
				mov dword ptr [tmp+16], eax;
				mov dword ptr [tmp+20], ebx;
				mov dword ptr [tmp+24], ecx;
				mov dword ptr [tmp+28], edx;
				mov eax, 0x80000004;
				cpuid
				mov dword ptr [tmp+32], eax;
				mov dword ptr [tmp+36], ebx;
				mov dword ptr [tmp+40], ecx;
				mov dword ptr [tmp+44], edx;
			}

			strcpy( mCPUInfo.szBrandID, tmp );
		}
		else
		{
			strcpy( mCPUInfo.szBrandID, "不支持" );
		}

		//解释 CPU 系列
		switch (mCPUInfo.nFamily)
		{
		case 4:			// Family = 4:  486 (80486) 或 5x86 (80486) 系列
			switch (mCPUInfo.nModel)
			{
			case 3:
			case 7:
			case 8:
			case 9:
				strcpy( mCPUInfo.szFamily, "AMD 80486" );
				break;
			case 0xE:
			case 0xF:
				strcpy( mCPUInfo.szFamily, "AMD 5x86" );
				break;
			default:
				strcpy( mCPUInfo.szFamily, "未知系列" );
				break;
			}
			break;
		case 5:			// Family = 5:  K5 或 K6 系列
			switch (mCPUInfo.nModel)
			{
			case 0:
			case 1:
			case 2:
			case 3:
				strcpy( mCPUInfo.szFamily, "AMD K5" );
				break;
			case 6:
			case 7:
			case 8:
			case 9:
				strcpy( mCPUInfo.szFamily, "AMD K6" );
				break;
			default:
				strcpy( mCPUInfo.szFamily, "未知系列" );
				break;
			}
			break;
		case 6:			// Family = 6:  K7 Athlon 系列
			strcpy( mCPUInfo.szFamily, "AMD K7" );
			break;
		default:
			strcpy( mCPUInfo.szFamily, "未知系列" );
			break;
		}

		//解释 CPU 型号
		switch (mCPUInfo.nFamily)
		{
		case 4:			// Family = 4:  486 (80486) 或 5x85 (80486) 系列
			switch (mCPUInfo.nModel)
			{
			case 3:			// Model = 3:  80486 DX2
				strcpy( mCPUInfo.szModel, "AMD 80486 DX2" );
				strcat( mCPUName, "AMD 80486 DX2" );
				break;
			case 7:			// Model = 7:  80486 回写增强 DX2
				strcpy( mCPUInfo.szModel, "AMD 80486 回写增强 DX2" );
				strcat( mCPUName, "AMD 80486 回写增强 DX2" );
				break;
			case 8:			// Model = 8:  80486 DX4
				strcpy( mCPUInfo.szModel, "AMD 80486 DX4" );
				strcat( mCPUName, "AMD 80486 DX4" );
				break;
			case 9:			// Model = 9:  80486 回写增强 DX4
				strcpy( mCPUInfo.szModel, "AMD 80486 回写增强 DX4" );
				strcat( mCPUName, "AMD 80486 回写增强 DX4" );
				break;
			case 0xE:		// Model = 0xE:  5x86
				strcpy( mCPUInfo.szModel, "AMD 5x86" );
				strcat( mCPUName, "AMD 5x86" );
				break;
			case 0xF:		// Model = 0xF:  5x86 回写增强
				strcpy( mCPUInfo.szModel, "AMD 5x86 回写增强" );
				strcat( mCPUName, "AMD 5x86 回写增强" );
				break;
			default:		// ...
				strcpy( mCPUInfo.szModel, "未知 AMD 80486 或 5x86 型号" );
				strcat( mCPUName, "AMD 80486 或 5x86 (未知型号)" );
				break;
			}
			break;
		case 5:			// Family = 5:  K5 或 K6 系列
			switch (mCPUInfo.nModel)
			{
			case 0:			// Model = 0:  K5 SSA 5 (Pentium Rating 75, 90 and 100 Mhz)
				strcpy( mCPUInfo.szModel, "AMD K5 SSA5 (PR75, PR90, PR100)" );
				strcat( mCPUName, "AMD K5 SSA5 (PR75, PR90, PR100)" );
				break;
			case 1:			// Model = 1:  K5 5k86 (PR 120 and 133 MHz)
				strcpy( mCPUInfo.szModel, "AMD K5 5k86 (PR120, PR133)" );
				strcat( mCPUName, "AMD K5 5k86 (PR120, PR133)" );
				break;
			case 2:			// Model = 2:  K5 5k86 (PR 166 MHz)
				strcpy( mCPUInfo.szModel, "AMD K5 5k86 (PR166)" );
				strcat( mCPUName, "AMD K5 5k86 (PR166)" );
				break;
			case 3:			// Model = 3:  K5 5k86 (PR 200 MHz)
				strcpy( mCPUInfo.szModel, "AMD K5 5k86 (PR200)" );
				strcat( mCPUName, "AMD K5 5k86 (PR200)" );
				break;
			case 6:			// Model = 6:  K6
				strcpy( mCPUInfo.szModel, "AMD K6 (0.30 微米)" );
				strcat( mCPUName, "AMD K6 (0.30 微米)" );
				break;
			case 7:			// Model = 7:  K6 (0.25 微米)
				strcpy( mCPUInfo.szModel, "AMD K6 (0.25 微米)" );
				strcat( mCPUName, "AMD K6 (0.25 微米)" );
				break;
			case 8:			// Model = 8:  K6-2
				strcpy( mCPUInfo.szModel, "AMD K6-2" );
				strcat( mCPUName, "AMD K6-2" );
				break;
			case 9:			// Model = 9:  K6-III
				strcpy( mCPUInfo.szModel, "AMD K6-III" );
				strcat( mCPUName, "AMD K6-III" );
				break;
			case 0xD:		// Model = 0xD:  K6-2+ / K6-III+
				strcpy( mCPUInfo.szModel, "AMD K6-2+ 或 K6-III+ (0.18 微米)" );
				strcat( mCPUName, "AMD K6-2+ 或 K6-III+ (0.18 微米)" );
				break;
			default:		// ...
				strcpy( mCPUInfo.szModel, "未知 AMD K5 或 K6 未知" );
				strcat( mCPUName, "AMD K5 或 K6 (未知型号)" );
				break;
			}
			break;
		case 6:			// Family = 6:  K7 系列
			switch (mCPUInfo.nModel)
			{
			case 1:			// Athlon
				strcpy( mCPUInfo.szModel, "AMD Athlon (0.25 微米)" );
				strcat( mCPUName, "AMD Athlon (0.25 微米)" );
				break;
			case 2:			// Athlon (0.18 微米)
				strcpy( mCPUInfo.szModel, "AMD Athlon (0.18 微米)" );
				strcat( mCPUName, "AMD Athlon (0.18 微米)" );
				break;
			case 3:			// Duron (烈火核心)
				strcpy( mCPUInfo.szModel, "AMD Duron (烈火核心)" );
				strcat( mCPUName, "AMD Duron (烈火核心)" );
				break;
			case 4:			// Athlon (雷鸟核心)
				strcpy( mCPUInfo.szModel, "AMD Athlon (雷鸟核心)" );
				strcat( mCPUName, "AMD Athlon (雷鸟核心)" );
				break;
			case 6:			// Athlon MP / Mobile Athlon (Palomino 核心)
				strcpy( mCPUInfo.szModel, "AMD Athlon XP (Palomino 核心)" );
				strcat( mCPUName, "AMD Athlon XP (Palomino 核心)" );
				break;
			case 7:			// Mobile Duron (摩根核心)
				strcpy( mCPUInfo.szModel, "AMD Mobile Duron (摩根核心)" );
				strcat( mCPUName, "AMD Mobile Duron (摩根核心)" );
				break;
			default:
				strcpy( mCPUInfo.szModel, "未知 AMD K7 型号" );
				strcat( mCPUName, "AMD K7 (未知型号)" );
				break;
			}
			break;
		default:
			strcpy( mCPUInfo.szModel, "未知 AMD 型号" );
			strcat( mCPUName, "AMD (未知型号)" );
			break;
		}

		_GetStandardProcessorExtensions();

		if( mCPUInfo.MaxSupportedExtendedLevel >= 0x80000001 )
		{
			__asm
			{
				mov eax, 0x80000001;
				cpuid
				mov edxreg, edx;
			}

			mCPUInfo._Ext.EMMX_MultimediaExtensions			= CheckBit(edxreg, 22);
			mCPUInfo._Ext.AA64_AMD64BitArchitecture			= CheckBit(edxreg, 29);
			mCPUInfo._Ext._E3DNOW_InstructionExtensions		= CheckBit(edxreg, 30);
			mCPUInfo._Ext._3DNOW_InstructionExtensions		= CheckBit(edxreg, 31);
		}

		if( mCPUInfo.MaxSupportedExtendedLevel >= 0x80000006 )
		{
			__asm
			{
				mov eax, 0x80000005;
				cpuid
				mov eaxreg, eax;
				mov ebxreg, ebx;
				mov ecxreg, ecx;
				mov edxreg, edx;
			}

			if( (ebxreg >> 16) && (eaxreg >> 16) )
			{
				mCPUInfo._Data.bPresent = true;
				strcpy( mCPUInfo._Data.szPageSize, "4 KB / 2 MB / 4MB" );
				mCPUInfo._Data.nAssociativeWays = (eaxreg >> 24) & 0xFF;
				mCPUInfo._Data.nEntries = (eaxreg >> 16) & 0xFF;
			}
			else if( eaxreg >> 16 )
			{
				mCPUInfo._Data.bPresent = true;
				strcpy( mCPUInfo._Data.szPageSize, "2 MB / 4MB" );
				mCPUInfo._Data.nAssociativeWays = (eaxreg >> 24) & 0xFF;
				mCPUInfo._Data.nEntries = (eaxreg >> 16) & 0xFF;
			}
			else if( ebxreg >> 16 )
			{
				mCPUInfo._Data.bPresent = true;
				strcpy( mCPUInfo._Data.szPageSize, "4 KB" );
				mCPUInfo._Data.nAssociativeWays = (ebxreg >> 24) & 0xFF;
				mCPUInfo._Data.nEntries = (ebxreg >> 16) & 0xFF;
			}
			if( mCPUInfo._Data.nAssociativeWays == 0xFF )
				mCPUInfo._Data.nAssociativeWays = (unsigned int) -1;

			//一级指令/代码缓存信息
			if( (ebxreg & 0xFFFF) && (eaxreg & 0xFFFF) )
			{
				mCPUInfo._Instruction.bPresent = true;
				strcpy( mCPUInfo._Instruction.szPageSize, "4 KB / 2 MB / 4MB" );
				mCPUInfo._Instruction.nAssociativeWays = (eaxreg >> 8) & 0xFF;
				mCPUInfo._Instruction.nEntries = eaxreg & 0xFF;
			}
			else if( eaxreg & 0xFFFF )
			{
				mCPUInfo._Instruction.bPresent = true;
				strcpy( mCPUInfo._Instruction.szPageSize, "2 MB / 4MB" );
				mCPUInfo._Instruction.nAssociativeWays = (eaxreg >> 8) & 0xFF;
				mCPUInfo._Instruction.nEntries = eaxreg & 0xFF;
			}
			else if( ebxreg & 0xFFFF )
			{
				mCPUInfo._Instruction.bPresent = true;
				strcpy( mCPUInfo._Instruction.szPageSize, "4 KB" );
				mCPUInfo._Instruction.nAssociativeWays = (ebxreg >> 8) & 0xFF;
				mCPUInfo._Instruction.nEntries = ebxreg & 0xFF;
			}
			if( mCPUInfo._Instruction.nAssociativeWays == 0xFF )
				mCPUInfo._Instruction.nAssociativeWays = (unsigned int) -1;

			//一级数据缓存信息
			if( (ecxreg >> 24) > 0 )
			{
				mCPUInfo._L1.Data.bPresent = true;
				sprintf( mCPUInfo._L1.Data.szSize, "%d KB", ecxreg >> 24 );
				mCPUInfo._L1.Data.nAssociativeWays = (ecxreg >> 15) & 0xFF;
				mCPUInfo._L1.Data.nLineSize = ecxreg & 0xFF;
			}

			//二级指令/代码缓存信息
			if( (edxreg >> 24) > 0 )
			{
				mCPUInfo._L1.Instruction.bPresent = true;
				sprintf( mCPUInfo._L1.Instruction.szSize, "%d KB", edxreg >> 24 );
				mCPUInfo._L1.Instruction.nAssociativeWays = (edxreg >> 15) & 0xFF;
				mCPUInfo._L1.Instruction.nLineSize = edxreg & 0xFF;
			}

			__asm
			{
				mov eax, 0x80000006;
				cpuid
				mov eaxreg, eax;
				mov ebxreg, ebx;
				mov ecxreg, ecx;
			}

			if( ( (ecxreg >> 12) & 0xF ) > 0 )
			{
				mCPUInfo._L2.bPresent = true;
				sprintf( mCPUInfo._L2.szSize, "%d KB", ecxreg >> 16 );
				switch ( (ecxreg >> 12) & 0xF )
				{
				case 1:
					mCPUInfo._L2.nAssociativeWays = 1;
					break;
				case 2:
					mCPUInfo._L2.nAssociativeWays = 2;
					break;
				case 4:
					mCPUInfo._L2.nAssociativeWays = 4;
					break;
				case 6:
					mCPUInfo._L2.nAssociativeWays = 8;
					break;
				case 8:
					mCPUInfo._L2.nAssociativeWays = 16;
					break;
				case 0xF:
					mCPUInfo._L2.nAssociativeWays = (unsigned int) -1;
					break;
				default:
					mCPUInfo._L2.nAssociativeWays = 0;
					break;
				}
				mCPUInfo._L2.nLineSize = ecxreg & 0xFF;
			}
		}
		else
		{
			_GetStandardProcessorConfiguration();
		}

		_TranslateProcessorConfiguration();

		return true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//解析 Intel 处理器信息
	bool ProcessorInfo::_AnalyzeIntelProcessor()
	{
		unsigned long eaxreg, ebxreg, edxreg;

		//如果 CPUID 指令不可用则返回
		if( !_CheckCPUIDPresence() )
			return false;

		//获取调用 CPUID 指令
		__asm
		{
			mov eax, 1;
			cpuid
			mov eaxreg, eax;
			mov ebxreg, ebx;
			mov edxreg, edx;
		}

		//获取型号（model）、系列（family）、次版本（stepping）、类型（type）和商标编号（brand id）。
		mCPUInfo.nStepping = eaxreg & 0xF;
		mCPUInfo.nModel    = (eaxreg >> 4) & 0xF;
		mCPUInfo.nFamily   = (eaxreg >> 8) & 0xF;
		mCPUInfo.nType     = (eaxreg >> 12) & 0x3;
		mCPUInfo.nBrandID  = ebxreg & 0xF;

		//解释 CPU 类型字符串
		switch (mCPUInfo.nType)
		{
		case 0:
			strcpy( mCPUInfo.szType, "原始 OEM" );
			strcpy( mCPUName, mCPUInfo.szType);
			strcat( mCPUName, " " );
			break;
		case 1:			// Type = 1:  超频
			strcpy( mCPUInfo.szType, "超频" );
			strcpy( mCPUName, mCPUInfo.szType);
			strcat( mCPUName, " " );
			break;
		case 2:			// Type = 2:  降频
			strcpy( mCPUInfo.szType, "降频" );
			strcpy( mCPUName, mCPUInfo.szType);
			strcat( mCPUName, " " );
			break;
		case 3:			// Type = 3:  保留
			strcpy( mCPUInfo.szType, "保留" );
			break;
		default:
			strcpy( mCPUInfo.szType, "未知" );
			break;
		}

		//解释商标
		switch (mCPUInfo.nBrandID)
		{
		case 0:			// Brand id = 0:  不支持
			strcpy( mCPUInfo.szBrandID, "Not supported" );
			break;
		case 1:			// Brand id = 1:  Intel Celeron (0.18 微米) 处理器
			strcpy( mCPUInfo.szBrandID, "0.18 微米 Intel Celeron" );
			break;
		case 2:			// Brand id = 2:  Intel Pentium III (0.18 微米) 处理器
			strcpy( mCPUInfo.szBrandID, "0.18 微米 Intel Pentium III" );
			break;
		case 3:			// Brand id = 3:  Model dependent
			if (mCPUInfo.nModel == 6)
				strcpy( mCPUInfo.szBrandID, "0.13 微米 Intel Celeron" );
			else
				strcpy( mCPUInfo.szBrandID, "0.18 微米 Intel Pentium III Xeon" );
			break;
		case 4:			// Brand id = 4:  Intel Pentium III Tualatin (0.13 微米) 处理器
			strcpy( mCPUInfo.szBrandID, "0.13 微米 Intel Pentium III" );
			break;
		case 6:			// Brand id = 6:  Intel Pentium III mobile (0.13 微米) 处理器
			strcpy( mCPUInfo.szBrandID, "0.13 微米 Intel Pentium III mobile" );
			break;
		case 7:			// Brand id = 7:  Intel Celeron mobile (0.13 微米) 处理器
			strcpy( mCPUInfo.szBrandID, "0.13 微米 Intel Celeron mobile" );
			break;
		case 8:			// Brand id = 8:  Intel Pentium 4 Willamette (0.18 微米) 处理器
			strcpy( mCPUInfo.szBrandID, "0.18 微米 Intel Pentium 4" );
			break;
		case 9:			// Brand id = 9:  Intel Pentium 4 Northwood (0.13 微米) 处理器
			strcpy( mCPUInfo.szBrandID, "0.13 微米 Intel Pentium 4" );
			break;
		case 0xA:		// Brand id = 0xA:  Intel Pentium 4 Northwood (0.13 微米 处理器) 
			strcpy( mCPUInfo.szBrandID, "0.13 微米 Intel Pentium 4" );
			break;
		case 0xB:		// Brand id = 0xB:  Intel Pentium 4 Northwood Xeon (0.13 微米 处理器)
			strcpy( mCPUInfo.szBrandID, "0.13 微米 Intel Pentium 4 Xeon" );
			break;
		case 0xE:		// Brand id = 0xE:  Intel Pentium 4 Willamette Xeon (0.18 微米 处理器)
			strcpy( mCPUInfo.szBrandID, "0.18 微米 Intel Pentium 4 Xeon" );
			break;
		default:		// Should be never called, but sure is sure
			strcpy( mCPUInfo.szBrandID, "未知" );
			break;
		}

		//解释 CPU 系列
		switch (mCPUInfo.nFamily)
		{
		case 3:			// Family = 3:  i386 (80386) 系列
			strcpy( mCPUInfo.szFamily, "Intel i386" );
			break;
		case 4:			// Family = 4:  i486 (80486) 系列
			strcpy( mCPUInfo.szFamily, "Intel i486" );
			break;
		case 5:			// Family = 5:  Pentium (80586) 系列
			strcpy( mCPUInfo.szFamily, "Intel Pentium" );
			break;
		case 6:			// Family = 6:  Pentium Pro (80686) 系列
			strcpy( mCPUInfo.szFamily, "Intel Pentium Pro" );
			break;
		case 15:		// Family = 15:  扩展系列

			mCPUInfo.nExtendedFamily = (eaxreg >> 20) & 0xFF;
			switch (mCPUInfo.nExtendedFamily)
			{
			case 0:			// Family = 15, Ext. Family = 0:  Pentium 4 系列
				strcpy( mCPUInfo.szFamily, "Intel Pentium 4" );
				break;
			case 1:			// Family = 15, Ext. Family = 1:  McKinley (64-bit) 系列
				strcpy( mCPUInfo.szFamily, "Intel McKinley (IA-64)" );
				break;
			default:
				strcpy( mCPUInfo.szFamily, "未知 Intel Pentium 4+" );
				break;
			}
			break;
		default:
			strcpy( mCPUInfo.szFamily, "未知" );
			break;
		}

		//解释 CPU 型号
		switch (mCPUInfo.nFamily)
		{
		case 3:			// i386 (80386) 系列
			strcpy( mCPUInfo.szModel, "未知 Intel i386" );
			strcat( mCPUName, "Intel i386" );
			break;
		case 4:			// i486 (80486) 系列
			switch (mCPUInfo.nModel)
			{
			case 0:			// Model = 0:  i486 DX-25/33 型号
				strcpy( mCPUInfo.szModel, "Intel i486 DX-25/33" );
				strcat( mCPUName, "Intel i486 DX-25/33" );
				break;
			case 1:			// Model = 1:  i486 DX-50 型号
				strcpy( mCPUInfo.szModel, "Intel i486 DX-50" );
				strcat( mCPUName, "Intel i486 DX-50" );
				break;
			case 2:			// Model = 2:  i486 SX 型号
				strcpy( mCPUInfo.szModel, "Intel i486 SX" );
				strcat( mCPUName, "Intel i486 SX" );
				break;
			case 3:			// Model = 3:  i486 DX2 （使用 i487 数字协处理器） 型号
				strcpy( mCPUInfo.szModel, "Intel i486 487/DX2" );
				strcat( mCPUName, "Intel i486 DX2 使用 i487 数字协处理器" );
				break;
			case 4:			// Model = 4:  i486 SL 型号
				strcpy( mCPUInfo.szModel, "Intel i486 SL" );
				strcat( mCPUName, "Intel i486 SL" );
				break;
			case 5:			// Model = 5:  i486 SX2 型号
				strcpy( mCPUInfo.szModel, "Intel i486 SX2" );
				strcat( mCPUName, "Intel i486 SX2" );
				break;
			case 7:			// Model = 7:  i486 回写增强 DX2 型号
				strcpy( mCPUInfo.szModel, "Intel i486 回写增强 DX2" );
				strcat( mCPUName, "Intel i486 回写增强 DX2" );
				break;
			case 8:			// Model = 8:  i486 DX4 型号
				strcpy( mCPUInfo.szModel, "Intel i486 DX4" );
				strcat( mCPUName, "Intel i486 DX4" );
				break;
			case 9:			// Model = 9:  i486 回写增强 DX4 型号
				strcpy( mCPUInfo.szModel, "Intel i486 回写增强 DX4" );
				strcat( mCPUName, "Intel i486 DX4" );
				break;
			default:		// ...
				strcpy( mCPUInfo.szModel, "未知 Intel i486" );
				strcat( mCPUName, "Intel i486 (未知型号)" );
				break;
			}
			break;
		case 5:			// Pentium (80586) 系列
			switch (mCPUInfo.nModel)
			{
			case 0:			// Model = 0:  Pentium (P5 A-Step) 型号
				strcpy( mCPUInfo.szModel, "Intel Pentium (P5 A-Step)" );
				strcat( mCPUName, "Intel Pentium (P5 A-Step 核心)" );
				break;
			case 1:			// Model = 1:  Pentium 60/66 型号
				strcpy( mCPUInfo.szModel, "Intel Pentium 60/66 (P5)" );
				strcat( mCPUName, "Intel Pentium 60/66 (P5 核心)" );
				break;
			case 2:			// Model = 2:  Pentium 75-200 (P54C) 型号
				strcpy( mCPUInfo.szModel, "Intel Pentium 75-200 (P54C)" );
				strcat( mCPUName, "Intel Pentium 75-200 (P54C 核心)" );
				break;
			case 3:			// Model = 3:  486 系统使用的 Pentium 超频处理器型号
				strcpy( mCPUInfo.szModel, "Intel Pentium 486 系统 (P24T 超频)" );
				strcat( mCPUName, "486 系统使用的 Intel Pentium (P24T 核心超频)" );
				break;
			case 4:			// Model = 4:  Pentium MMX 型号
				strcpy( mCPUInfo.szModel, "Intel Pentium MMX (P55C)" );
				strcat( mCPUName, "Intel Pentium MMX (P55C 核心)" );
				break;
			case 7:			// Model = 7:  Pentium 型号
				strcpy( mCPUInfo.szModel, "Intel Pentium (P54C)" );
				strcat( mCPUName, "Intel Pentium (P54C 核心)" );
				break;
			case 8:			// Model = 8:  Pentium MMX (0.25 微米) 型号
				strcpy( mCPUInfo.szModel, "Intel Pentium MMX (P55C), 0.25 微米" );
				strcat( mCPUName, "Intel Pentium MMX (P55C 核心), 0.25 微米" );
				break;
			default:		// ...
				strcpy( mCPUInfo.szModel, "未知 Intel Pentium" );
				strcat( mCPUName, "Intel Pentium (未知 P5 型号)" );
				break;
			}
			break;
		case 6:			// Pentium Pro (80686) 系列
			switch (mCPUInfo.nModel)
			{
			case 0:			// Model = 0:  Pentium Pro (P6 A-Step) 型号
				strcpy( mCPUInfo.szModel, "Intel Pentium Pro (P6 A-Step)" );
				strcat( mCPUName, "Intel Pentium Pro (P6 A-Step 核心)" );
				break;
			case 1:			// Model = 1:  Pentium Pro
				strcpy( mCPUInfo.szModel, "Intel Pentium Pro (P6)" );
				strcat( mCPUName, "Intel Pentium Pro (P6 核心)" );
				break;
			case 3:			// Model = 3:  Pentium II (66 MHz FSB ) 型号
				strcpy( mCPUInfo.szModel, "Intel Pentium II Model 3, 0.28 微米" );
				strcat( mCPUName, "Intel Pentium II (Model 3 核心, 0.28 微米)" );
				break;
			case 5:			// Model = 5:  Pentium II/Xeon/Celeron (0.25 微米) 型号
				strcpy( mCPUInfo.szModel, "Intel Pentium II Model 5/Xeon/Celeron, 0.25 微米" );
				strcat( mCPUName, "Intel Pentium II/Xeon/Celeron (Model 5 核心, 0.25 微米)" );
				break;
			case 6:			// Model = 6:  Celoron (内置二级缓存) 型号
				strcpy( mCPUInfo.szModel, "Intel Celeron - 内置二级缓存" );
				strcat( mCPUName, "Intel Celeron - 内置二级缓存" );
				break;
			case 7:			// Model = 7:  Pentium III/Xeon (外置二级缓存) 型号
				strcpy( mCPUInfo.szModel, "Intel Pentium III/Pentium III Xeon - 外置二级缓存, 0.25 微米" );
				strcat( mCPUName, "Intel Pentium III/Pentium III Xeon (0.25 微米) 外置二级缓存" );
				break;
			case 8:			// Model = 8:  Pentium III/Xeon/Celeron (256 KB 内置二级缓存) 型号
				strcpy( mCPUInfo.szModel, "Intel Pentium III/Celeron/Pentium III Xeon - 内置二级缓存, 0.18 微米" );
				// We want to know it exactly:
				switch (mCPUInfo.nBrandID)
				{
				case 1:			// Model = 8, Brand id = 1:  Celeron (内置二级缓存) 型号
					strcat( mCPUName, "Intel Celeron (0.18 微米) 内置二级缓存" );
					break;
				case 2:			// Model = 8, Brand id = 2:  Pentium III (内置二级缓存) 型号
					strcat( mCPUName, "Intel Pentium III (0.18 微米) 内置二级缓存" );
					break;
				case 3:			// Model = 8, Brand id = 3:  Pentium III Xeon (内置二级缓存) 型号
					strcat( mCPUName, "Intel Pentium III Xeon (0.18 微米) 内置二级缓存" );
					break;
				default:		// ...?
					strcat( mCPUName, "Intel Pentium III core (unknown model, 0.18 微米) 内置二级缓存" );
					break;
				}
				break;
			case 0xA:		// Model = 0xA:  Pentium III/Xeon/Celeron (1 or 2 MB 内置二级缓存) 型号
				strcpy( mCPUInfo.szModel, "Intel Pentium III/Celeron/Pentium III Xeon - 内置二级缓存, 0.18 微米" );
				// Exact detection:
				switch (mCPUInfo.nBrandID)
				{
				case 1:			// Model = 0xA, Brand id = 1:  Celeron (1 or 2 MB 内置二级缓存) 型号
					strcat( mCPUName, "Intel Celeron (0.18 微米) 内置二级缓存" );
					break;
				case 2:			// Model = 0xA, Brand id = 2:  Pentium III (1 or 2 MB 内置二级缓存) 型号
					strcat( mCPUName, "Intel Pentium III (0.18 微米) 内置二级缓存" );
					break;
				case 3:			// Model = 0xA, Brand id = 3:  Pentium III Xeon (1 or 2 MB 内置二级缓存) 型号
					strcat( mCPUName, "Intel Pentium III Xeon (0.18 微米) 内置二级缓存" );
					break;
				default:
					strcat( mCPUName, "Intel Pentium III 核心 (未知型号, 0.18 微米) 内置二级缓存" );
					break;
				}
				break;
			case 0xB:		// Model = 0xB: Pentium III/Xeon/Celeron (图拉腾核心, 内置二级缓存) 型号
				strcpy( mCPUInfo.szModel, "Intel Pentium III/Celeron/Pentium III Xeon - 内置二级缓存, 0.13 微米" );
				// Omniscient: ;-)
				switch (mCPUInfo.nBrandID)
				{
				case 3:			// Model = 0xB, Brand id = 3:  Celeron (图拉腾核心) 型号
					strcat( mCPUName, "Intel Celeron (图拉腾核心, 0.13 微米) 内置二级缓存" );
					break;
				case 4:			// Model = 0xB, Brand id = 4:  Pentium III (图拉腾核心) 型号
					strcat( mCPUName, "Intel Pentium III (图拉腾核心, 0.13 微米) 内置二级缓存" );
					break;
				case 7:			// Model = 0xB, Brand id = 7:  Celeron mobile (图拉腾核心) 型号
					strcat( mCPUName, "Intel Celeron mobile (图拉腾核心, 0.13 微米) 内置二级缓存" );
					break;
				default:		// *bored*
					strcat( mCPUName, "Intel Pentium III 图拉腾核心 (未知型号, 0.13 微米) 内置二级缓存" );
					break;
				}
				break;
			default:
				strcpy( mCPUInfo.szModel, "未知 Intel Pentium Pro" );
				strcat( mCPUName, "Intel Pentium Pro (未知型号)" );
				break;
			}
			break;
		case 15:		//扩展系列
			mCPUInfo.nExtendedModel = (eaxreg >> 16) & 0xFF;
			switch (mCPUInfo.nModel)
			{
			case 0:			// Model = 0:  Pentium 4 Willamette (A-Step) 核心
				if ((mCPUInfo.nBrandID) == 8)	// Brand id = 8:  P4 Willamette
				{
					strcpy( mCPUInfo.szModel, "Intel Pentium 4 Willamette (A-Step)" );
					strcat( mCPUName, "Intel Pentium 4 Willamette 核心 (A-Step)" );
				}
				else							// else Xeon
				{
					strcpy( mCPUInfo.szModel, "Intel Pentium 4 Willamette 核心 Xeon (A-Step)" );
					strcat( mCPUName, "Intel Pentium 4 Willamette 核心 Xeon (A-Step)" );
				}
				break;
			case 1:			// Model = 1:  Pentium 4 Willamette 核心
				if ((mCPUInfo.nBrandID) == 8)	// Brand id = 8:  P4 Willamette
				{
					strcpy( mCPUInfo.szModel, "Intel Pentium 4 Willamette 核心" );
					strcat( mCPUName, "Intel Pentium 4 Willamette 核心" );
				}
				else							// else Xeon
				{
					strcpy( mCPUInfo.szModel, "Intel Pentium 4 Willamette 核心 Xeon" );
					strcat( mCPUName, "Intel Pentium 4 Willamette 核心 Xeon" );
				}
				break;
			case 2:			// Model = 2:  Pentium 4 Northwood 核心
				if (((mCPUInfo.nBrandID) == 9) || ((mCPUInfo.nBrandID) == 0xA))		// P4 Willamette
				{
					strcpy( mCPUInfo.szModel, "Intel Pentium 4 Northwood 核心" );
					strcat( mCPUName, "Intel Pentium 4 Northwood 核心" );
				}
				else							// Xeon
				{
					strcpy( mCPUInfo.szModel, "Intel Pentium 4 Northwood 核心 Xeon" );
					strcat( mCPUName, "Intel Pentium 4 Northwood 核心 Xeon" );
				}
				break;
			default:
				strcpy( mCPUInfo.szModel, "未知 Intel Pentium 4" );
				strcat( mCPUName, "Intel Pentium 4 (未知型号)" );
				break;
			}
			break;
		default:
			strcpy( mCPUInfo.szModel, "未知 Intel model" );
			strcat( mCPUName, "Intel (未知型号)" );
			break;
		}


		if (mCPUInfo.MaxSupportedLevel >= 3)
		{
			unsigned long sig1, sig2, sig3;
			__asm
			{
				mov eax, 1;
				cpuid
				mov sig1, eax;
				mov eax, 3;
				cpuid
				mov sig2, ecx;
				mov sig3, edx;
			}

			sprintf( mCPUInfo.szProcessorSerial, "%04lX-%04lX-%04lX-%04lX-%04lX-%04lX",
				sig1 >> 16, sig1 & 0xFFFF, sig3 >> 16, sig3 & 0xFFFF, sig2 >> 16, sig2 & 0xFFFF );
		}
		else
		{
			strcpy( mCPUInfo.szProcessorSerial, "无处理器序列号" );
		}

		_GetStandardProcessorExtensions();

		_GetStandardProcessorConfiguration();
		_TranslateProcessorConfiguration();

		return true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//解析未知处理器信息
	bool ProcessorInfo::_AnalyzeUnknownProcessor()
	{
		unsigned long eaxreg, ebxreg;

		//如果 CPUID 指令不可用则返回
		if( !_CheckCPUIDPresence() )
			return false;

		//获取调用 CPUID 指令
		__asm
		{
			mov eax, 1;
			cpuid
			mov eaxreg, eax;
			mov ebxreg, ebx;
		}

		//获取型号（model）、系列（family）、次版本（stepping）和类型（type）。
		mCPUInfo.nStepping = eaxreg & 0xF;
		mCPUInfo.nModel    = (eaxreg >> 4) & 0xF;
		mCPUInfo.nFamily   = (eaxreg >> 8) & 0xF;
		mCPUInfo.nType     = (eaxreg >> 12) & 0x3;

		mCPUInfo.nBrandID  = ebxreg & 0xF;

		_GetStandardProcessorExtensions();

		strcpy( mCPUName, "未知" );

		strcpy( mCPUInfo._Data.szTLB, "未知" );
		strcpy( mCPUInfo._Instruction.szTLB, "未知" );

		strcpy( mCPUInfo._Trace.szCache, "未知" );
		strcpy( mCPUInfo._L1.Data.szCache, "未知" );
		strcpy( mCPUInfo._L1.Instruction.szCache, "未知" );
		strcpy( mCPUInfo._L2.szCache, "未知" );
		strcpy( mCPUInfo._L3.szCache, "未知" );

		strcpy( mCPUInfo.szProcessorSerial, "未知/不支持" );

		sprintf( mCPUInfo.szBrandID, "商标编号 %d", mCPUInfo.nBrandID );
		sprintf( mCPUInfo.szFamily, "系列编号 %d", mCPUInfo.nFamily );
		sprintf( mCPUInfo.szModel, "型号编号 %d", mCPUInfo.nModel );

		//检测 CPU 类型
		switch (mCPUInfo.nType)
		{
		case 0:
			strcpy( mCPUInfo.szType, "原始 OEM" );
			break;
		case 1:
			strcpy( mCPUInfo.szType, "超频" );
			break;
		case 2:
			strcpy( mCPUInfo.szType, "降频" );
			break;
		case 3:
			strcpy( mCPUInfo.szType, "保留" );
			break;
		default:
			strcpy( mCPUInfo.szType, "未知" );
			break;
		}

		return true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//检测 CPUID 指令是否可用
	bool ProcessorInfo::_CheckCPUIDPresence()
	{
		unsigned long BitChanged;

		__asm
		{
			pushfd;
			pop eax;
			mov ebx, eax;
			xor eax, 0x00200000;
			push eax;
			popfd;
			pushfd;
			pop eax;
			xor eax,ebx;
			mov BitChanged, eax;
		}

		return ( (BitChanged) ? true : false );
	}
	//--------------------------------------------------------------------------------------------------------------
	//解码处理器位配置信息
	void ProcessorInfo::_DecodeProcessorConfiguration( unsigned int cfg )
	{
		cfg &= 0xFF;

		switch(cfg)
		{
		case 0:			// cfg = 0:  未使用
			break;
		case 0x1:		// cfg = 0x1:  代码 TLB ,4 KB 页, 4 路, 32 入口
			mCPUInfo._Instruction.bPresent = true;
			strcpy( mCPUInfo._Instruction.szPageSize, "4 KB" );
			mCPUInfo._Instruction.nAssociativeWays = 4;
			mCPUInfo._Instruction.nEntries = 32;
			break;
		case 0x2:		// cfg = 0x2:  代码 TLB ,4 MB 页, 完全相连, 2 入口
			mCPUInfo._Instruction.bPresent = true;
			strcpy( mCPUInfo._Instruction.szPageSize, "4 MB" );
			mCPUInfo._Instruction.nAssociativeWays = 4;
			mCPUInfo._Instruction.nEntries = 2;
			break;
		case 0x3:		// cfg = 0x3:  数据 TLB ,4 KB 页, 4 路, 64 入口
			mCPUInfo._Data.bPresent = true;
			strcpy( mCPUInfo._Data.szPageSize, "4 KB" );
			mCPUInfo._Data.nAssociativeWays = 4;
			mCPUInfo._Data.nEntries = 64;
			break;
		case 0x4:		// cfg = 0x4:  数据 TLB ,4 MB 页, 4 路, 8 入口
			mCPUInfo._Data.bPresent = true;
			strcpy( mCPUInfo._Data.szPageSize, "4 MB" );
			mCPUInfo._Data.nAssociativeWays = 4;
			mCPUInfo._Data.nEntries = 8;
			break;
		case 0x6:		// cfg = 0x6:  代码一级缓存, 8 KB, 4 路, 32 字节线路
			mCPUInfo._L1.Instruction.bPresent = true;
			strcpy( mCPUInfo._L1.Instruction.szSize, "8 KB" );
			mCPUInfo._L1.Instruction.nAssociativeWays = 4;
			mCPUInfo._L1.Instruction.nLineSize = 32;
			break;
		case 0x8:		// cfg = 0x8:  代码一级缓存, 16 KB, 4 路, 32 字节线路
			mCPUInfo._L1.Instruction.bPresent = true;
			strcpy( mCPUInfo._L1.Instruction.szSize, "16 KB" );
			mCPUInfo._L1.Instruction.nAssociativeWays = 4;
			mCPUInfo._L1.Instruction.nLineSize = 32;
			break;
		case 0xA:		// cfg = 0xA:  数据一级缓存, 8 KB, 2 路, 32 字节线路
			mCPUInfo._L1.Data.bPresent = true;
			strcpy( mCPUInfo._L1.Data.szSize, "8 KB" );
			mCPUInfo._L1.Data.nAssociativeWays = 2;
			mCPUInfo._L1.Data.nLineSize = 32;
			break;
		case 0xC:		// cfg = 0xC:  数据一级缓存, 16 KB, 4 路, 32 字节线路
			mCPUInfo._L1.Data.bPresent = true;
			strcpy( mCPUInfo._L1.Data.szSize, "16 KB" );
			mCPUInfo._L1.Data.nAssociativeWays = 4;
			mCPUInfo._L1.Data.nLineSize = 32;
			break;
		case 0x22:		// cfg = 0x22:  代码和数据三级缓存, 512 KB, 4 路, 64 字节线路, 已分区
			mCPUInfo._L3.bPresent = true;
			strcpy( mCPUInfo._L3.szSize, "512 KB" );
			mCPUInfo._L3.nAssociativeWays = 4;
			mCPUInfo._L3.nLineSize = 64;
			mCPUInfo._L3.bSectored = true;
			break;
		case 0x23:		// cfg = 0x23:  代码和数据三级缓存, 1024 KB, 8 路, 64 字节线路, 已分区
			mCPUInfo._L3.bPresent = true;
			strcpy( mCPUInfo._L3.szSize, "1024 KB" );
			mCPUInfo._L3.nAssociativeWays = 8;
			mCPUInfo._L3.nLineSize = 64;
			mCPUInfo._L3.bSectored = true;
			break;
		case 0x25:		// cfg = 0x25:  代码和数据三级缓存, 2048 KB, 8 路, 64 字节线路, 已分区
			mCPUInfo._L3.bPresent = true;
			strcpy( mCPUInfo._L3.szSize, "2048 KB" );
			mCPUInfo._L3.nAssociativeWays = 8;
			mCPUInfo._L3.nLineSize = 64;
			mCPUInfo._L3.bSectored = true;
			break;
		case 0x29:		// cfg = 0x29:  代码和数据三级缓存, 4096 KB, 8 路, 64 字节线路, 已分区
			mCPUInfo._L3.bPresent = true;
			strcpy( mCPUInfo._L3.szSize, "4096 KB" );
			mCPUInfo._L3.nAssociativeWays = 8;
			mCPUInfo._L3.nLineSize = 64;
			mCPUInfo._L3.bSectored = true;
			break;
		case 0x40:		// cfg = 0x40:  未集成二级缓存(P6 核心)或三级缓存(P4 核心)
			break;
		case 0x41:		// cfg = 0x41:  代码和数据二级缓存, 128 KB, 4 路, 32 字节线路
			mCPUInfo._L2.bPresent = true;
			strcpy( mCPUInfo._L2.szSize, "128 KB" );
			mCPUInfo._L2.nAssociativeWays = 4;
			mCPUInfo._L2.nLineSize = 32;
			break;
		case 0x42:		// cfg = 0x42:  代码和数据二级缓存, 256 KB, 4 路, 32 字节线路
			mCPUInfo._L2.bPresent = true;
			strcpy( mCPUInfo._L2.szSize, "256 KB" );
			mCPUInfo._L2.nAssociativeWays = 4;
			mCPUInfo._L2.nLineSize = 32;
			break;
		case 0x43:		// cfg = 0x43:  代码和数据二级缓存, 512 KB, 4 路, 32 字节线路
			mCPUInfo._L2.bPresent = true;
			strcpy( mCPUInfo._L2.szSize, "512 KB" );
			mCPUInfo._L2.nAssociativeWays = 4;
			mCPUInfo._L2.nLineSize = 32;
			break;
		case 0x44:		// cfg = 0x44:  代码和数据二级缓存, 1024 KB, 4 路, 32 字节线路
			mCPUInfo._L2.bPresent = true;
			strcpy( mCPUInfo._L2.szSize, "1 MB" );
			mCPUInfo._L2.nAssociativeWays = 4;
			mCPUInfo._L2.nLineSize = 32;
			break;
		case 0x45:		// cfg = 0x45:  代码和数据二级缓存, 2048 KB, 4 路, 32 字节线路
			mCPUInfo._L2.bPresent = true;
			strcpy( mCPUInfo._L2.szSize, "2 MB" );
			mCPUInfo._L2.nAssociativeWays = 4;
			mCPUInfo._L2.nLineSize = 32;
			break;
		case 0x50:		// cfg = 0x50:  代码 TLB ,4 KB / 4 MB / 2 MB 页, 完全相连, 64 入口
			mCPUInfo._Instruction.bPresent = true;
			strcpy( mCPUInfo._Instruction.szPageSize, "4 KB / 2 MB / 4 MB" );
			mCPUInfo._Instruction.nAssociativeWays = (unsigned int) -1;
			mCPUInfo._Instruction.nEntries = 64;
			break;
		case 0x51:		// cfg = 0x51:  代码 TLB ,4 KB / 4 MB / 2 MB 页, 完全相连, 128 入口
			mCPUInfo._Instruction.bPresent = true;
			strcpy( mCPUInfo._Instruction.szPageSize, "4 KB / 2 MB / 4 MB" );
			mCPUInfo._Instruction.nAssociativeWays = (unsigned int) -1;
			mCPUInfo._Instruction.nEntries = 128;
			break;
		case 0x52:		// cfg = 0x52:  代码 TLB ,4 KB / 4 MB / 2 MB 页, 完全相连, 256 入口
			mCPUInfo._Instruction.bPresent = true;
			strcpy( mCPUInfo._Instruction.szPageSize, "4 KB / 2 MB / 4 MB" );
			mCPUInfo._Instruction.nAssociativeWays = (unsigned int) -1;
			mCPUInfo._Instruction.nEntries = 256;
			break;
		case 0x5B:		// cfg = 0x5B:  数据 TLB ,4 KB / 4 MB 页, 完全相连, 64 入口
			mCPUInfo._Data.bPresent = true;
			strcpy( mCPUInfo._Data.szPageSize, "4 KB / 4 MB" );
			mCPUInfo._Data.nAssociativeWays = (unsigned int) -1;
			mCPUInfo._Data.nEntries = 64;
			break;
		case 0x5C:		// cfg = 0x5C:  数据 TLB ,4 KB / 4 MB 页, 完全相连, 128 入口
			mCPUInfo._Data.bPresent = true;
			strcpy( mCPUInfo._Data.szPageSize, "4 KB / 4 MB" );
			mCPUInfo._Data.nAssociativeWays = (unsigned int) -1;
			mCPUInfo._Data.nEntries = 128;
			break;
		case 0x5d:		// cfg = 0x5D:  数据 TLB ,4 KB / 4 MB 页, 完全相连, 256 入口
			mCPUInfo._Data.bPresent = true;
			strcpy( mCPUInfo._Data.szPageSize, "4 KB / 4 MB" );
			mCPUInfo._Data.nAssociativeWays = (unsigned int) -1;
			mCPUInfo._Data.nEntries = 256;
			break;
		case 0x66:		// cfg = 0x66:  数据一级缓存, 8 KB, 4 路, 64 字节线路, 已分区
			mCPUInfo._L1.Data.bPresent = true;
			strcpy( mCPUInfo._L1.Data.szSize, "8 KB" );
			mCPUInfo._L1.Data.nAssociativeWays = 4;
			mCPUInfo._L1.Data.nLineSize = 64;
			break;
		case 0x67:		// cfg = 0x67:  数据一级缓存, 16 KB, 4 路, 64 字节线路, 已分区
			mCPUInfo._L1.Data.bPresent = true;
			strcpy( mCPUInfo._L1.Data.szSize, "16 KB" );
			mCPUInfo._L1.Data.nAssociativeWays = 4;
			mCPUInfo._L1.Data.nLineSize = 64;
			break;
		case 0x68:		// cfg = 0x68:  数据一级缓存, 32 KB, 4 路, 64 字节线路, 已分区
			mCPUInfo._L1.Data.bPresent = true;
			strcpy( mCPUInfo._L1.Data.szSize, "32 KB" );
			mCPUInfo._L1.Data.nAssociativeWays = 4;
			mCPUInfo._L1.Data.nLineSize = 64;
			break;
		case 0x70:		// cfg = 0x70:  trace一级缓存, 12 K-micro-ops, 4 路
			mCPUInfo._Trace.bPresent = true;
			strcpy( mCPUInfo._Trace.szSize, "12 K-micro-ops" );
			mCPUInfo._Trace.nAssociativeWays = 4;
			break;
		case 0x71:		// cfg = 0x71:  trace一级缓存, 16 K-micro-ops, 4 路
			mCPUInfo._Trace.bPresent = true;
			strcpy( mCPUInfo._Trace.szSize, "16 K-micro-ops" );
			mCPUInfo._Trace.nAssociativeWays = 4;
			break;
		case 0x72:		// cfg = 0x72:  trace一级缓存, 32 K-micro-ops, 4 路
			mCPUInfo._Trace.bPresent = true;
			strcpy( mCPUInfo._Trace.szSize, "32 K-micro-ops" );
			mCPUInfo._Trace.nAssociativeWays = 4;
			break;
		case 0x79:		// cfg = 0x79:  代码和数据二级缓存, 128 KB, 8 路, 64 字节线路, 已分区
			mCPUInfo._L2.bPresent = true;
			strcpy( mCPUInfo._L2.szSize, "128 KB" );
			mCPUInfo._L2.nAssociativeWays = 8;
			mCPUInfo._L2.nLineSize = 64;
			mCPUInfo._L2.bSectored = true;
			break;
		case 0x7A:		// cfg = 0x7A:  代码和数据二级缓存, 256 KB, 8 路, 64 字节线路, 已分区
			mCPUInfo._L2.bPresent = true;
			strcpy( mCPUInfo._L2.szSize, "256 KB" );
			mCPUInfo._L2.nAssociativeWays = 8;
			mCPUInfo._L2.nLineSize = 64;
			mCPUInfo._L2.bSectored = true;
			break;
		case 0x7B:		// cfg = 0x7B:  代码和数据二级缓存, 512 KB, 8 路, 64 字节线路, 已分区
			mCPUInfo._L2.bPresent = true;
			strcpy( mCPUInfo._L2.szSize, "512 KB" );
			mCPUInfo._L2.nAssociativeWays = 8;
			mCPUInfo._L2.nLineSize = 64;
			mCPUInfo._L2.bSectored = true;
			break;
		case 0x7C:		// cfg = 0x7C:  代码和数据二级缓存, 1024 KB, 8 路, 64 字节线路, 已分区
			mCPUInfo._L2.bPresent = true;
			strcpy( mCPUInfo._L2.szSize, "1 MB" );
			mCPUInfo._L2.nAssociativeWays = 8;
			mCPUInfo._L2.nLineSize = 64;
			mCPUInfo._L2.bSectored = true;
			break;
		case 0x81:		// cfg = 0x81:  代码和数据二级缓存, 128 KB, 8 路, 32 字节线路
			mCPUInfo._L2.bPresent = true;
			strcpy( mCPUInfo._L2.szSize, "128 KB" );
			mCPUInfo._L2.nAssociativeWays = 8;
			mCPUInfo._L2.nLineSize = 32;
			break;
		case 0x82:		// cfg = 0x82:  代码和数据二级缓存, 256 KB, 8 路, 32 字节线路
			mCPUInfo._L2.bPresent = true;
			strcpy( mCPUInfo._L2.szSize, "256 KB" );
			mCPUInfo._L2.nAssociativeWays = 8;
			mCPUInfo._L2.nLineSize = 32;
			break;
		case 0x83:		// cfg = 0x83:  代码和数据二级缓存, 512 KB, 8 路, 32 字节线路
			mCPUInfo._L2.bPresent = true;
			strcpy( mCPUInfo._L2.szSize, "512 KB" );
			mCPUInfo._L2.nAssociativeWays = 8;
			mCPUInfo._L2.nLineSize = 32;
			break;
		case 0x84:		// cfg = 0x84:  代码和数据二级缓存, 1024 KB, 8 路, 32 字节线路
			mCPUInfo._L2.bPresent = true;
			strcpy( mCPUInfo._L2.szSize, "1 MB" );
			mCPUInfo._L2.nAssociativeWays = 8;
			mCPUInfo._L2.nLineSize = 32;
			break;
		case 0x85:		// cfg = 0x85:  代码和数据二级缓存, 2048 KB, 8 路, 32 字节线路
			mCPUInfo._L2.bPresent = true;
			strcpy( mCPUInfo._L2.szSize, "2 MB" );
			mCPUInfo._L2.nAssociativeWays = 8;
			mCPUInfo._L2.nLineSize = 32;
			break;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//翻译处理器配置信息
	void ProcessorInfo::_TranslateProcessorConfiguration()
	{
		TranslateTLB( &mCPUInfo._Data );
		TranslateTLB( &mCPUInfo._Instruction );

		TranslateCache( &mCPUInfo._Trace );

		TranslateCache( &mCPUInfo._L1.Instruction );
		TranslateCache( &mCPUInfo._L1.Data );
		TranslateCache( &mCPUInfo._L2 );
		TranslateCache( &mCPUInfo._L3 );
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取标准处理器配置信息
	void ProcessorInfo::_GetStandardProcessorConfiguration()
	{
		unsigned long eaxreg, ebxreg, ecxreg, edxreg;

		//如果 CPUID 指令不可用则返回
		if( !_CheckCPUIDPresence() )
			return;

		if (mCPUInfo.MaxSupportedLevel >= 2)
		{
			unsigned long count, num = 255;
			for (count = 0; count < num; count++)
			{
				__asm
				{
					mov eax, 2;
					cpuid
					mov eaxreg, eax;
					mov ebxreg, ebx;
					mov ecxreg, ecx;
					mov edxreg, edx;
				}

				num = eaxreg & 0xFF;

				_DecodeProcessorConfiguration(eaxreg >> 8);
				_DecodeProcessorConfiguration(eaxreg >> 16);
				_DecodeProcessorConfiguration(eaxreg >> 24);

				if ((ebxreg & 0x80000000) == 0)
				{
					_DecodeProcessorConfiguration(ebxreg);
					_DecodeProcessorConfiguration(ebxreg >> 8);
					_DecodeProcessorConfiguration(ebxreg >> 16);
					_DecodeProcessorConfiguration(ebxreg >> 24);
				}
				if ((ecxreg & 0x80000000) == 0)
				{
					_DecodeProcessorConfiguration(ecxreg);
					_DecodeProcessorConfiguration(ecxreg >> 8);
					_DecodeProcessorConfiguration(ecxreg >> 16);
					_DecodeProcessorConfiguration(ecxreg >> 24);
				}
				if ((edxreg & 0x80000000) == 0)
				{
					_DecodeProcessorConfiguration(edxreg);
					_DecodeProcessorConfiguration(edxreg >> 8);
					_DecodeProcessorConfiguration(edxreg >> 16);
					_DecodeProcessorConfiguration(edxreg >> 24);
				}
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取标准处理器扩展信息
	void ProcessorInfo::_GetStandardProcessorExtensions()
	{
		unsigned long ebxreg, edxreg;

		//如果 CPUID 指令不可用则返回
		if( !_CheckCPUIDPresence() )
			return;

		//调用 CPUID 指令
		__asm
		{
			mov eax, 1;
			cpuid
			mov ebxreg, ebx;
			mov edxreg, edx;
		}

		//检测标准处理器扩展信息位
		mCPUInfo._Ext.FPU_FloatingPointUnit							= CheckBit(edxreg, 0);
		mCPUInfo._Ext.VME_Virtual8086ModeEnhancements				= CheckBit(edxreg, 1);
		mCPUInfo._Ext.DE_DebuggingExtensions						= CheckBit(edxreg, 2);
		mCPUInfo._Ext.PSE_PageSizeExtensions						= CheckBit(edxreg, 3);
		mCPUInfo._Ext.TSC_TimeStampCounter							= CheckBit(edxreg, 4);
		mCPUInfo._Ext.MSR_ModelSpecificRegisters					= CheckBit(edxreg, 5);
		mCPUInfo._Ext.PAE_PhysicalAddressExtension					= CheckBit(edxreg, 6);
		mCPUInfo._Ext.MCE_MachineCheckException						= CheckBit(edxreg, 7);
		mCPUInfo._Ext.CX8_COMPXCHG8B_Instruction						= CheckBit(edxreg, 8);
		mCPUInfo._Ext.APIC_AdvancedProgrammableInterruptController	= CheckBit(edxreg, 9);
		mCPUInfo._Ext.APIC_ID = (ebxreg >> 24) & 0xFF;
		mCPUInfo._Ext.SEP_FastSystemCall							= CheckBit(edxreg, 11);
		mCPUInfo._Ext.MTRR_MemoryTypeRangeRegisters					= CheckBit(edxreg, 12);
		mCPUInfo._Ext.PGE_PTE_GlobalFlag							= CheckBit(edxreg, 13);
		mCPUInfo._Ext.MCA_MachineCheckArchitecture					= CheckBit(edxreg, 14);
		mCPUInfo._Ext.CMOV_ConditionalMoveAndCompareInstructions		= CheckBit(edxreg, 15);
		mCPUInfo._Ext.FGPAT_PageAttributeTable						= CheckBit(edxreg, 16);
		mCPUInfo._Ext.PSE36_36bitPageSizeExtension					= CheckBit(edxreg, 17);
		mCPUInfo._Ext.PN_ProcessorSerialNumber						= CheckBit(edxreg, 18);
		mCPUInfo._Ext.CLFSH_CFLUSH_Instruction						= CheckBit(edxreg, 19);
		mCPUInfo._Ext.CLFLUSH_InstructionCacheLineSize = (ebxreg >> 8) & 0xFF;
		mCPUInfo._Ext.DS_DebugStore									= CheckBit(edxreg, 21);
		mCPUInfo._Ext.ACPI_ThermalMonitorAndClockControl			= CheckBit(edxreg, 22);
		mCPUInfo._Ext.MMX_MultimediaExtensions						= CheckBit(edxreg, 23);
		mCPUInfo._Ext.FXSR_FastStreamingSIMD_ExtensionsSaveRestore	= CheckBit(edxreg, 24);
		mCPUInfo._Ext.SSE_StreamingSIMD_Extensions					= CheckBit(edxreg, 25);
		mCPUInfo._Ext.SSE2_StreamingSIMD2_Extensions				= CheckBit(edxreg, 26);
		mCPUInfo._Ext.SS_SelfSnoop									= CheckBit(edxreg, 27);
		mCPUInfo._Ext.HT_HyperThreading								= CheckBit(edxreg, 28);
		mCPUInfo._Ext.HT_HyterThreadingSiblings = (ebxreg >> 16) & 0xFF;
		mCPUInfo._Ext.TM_ThermalMonitor								= CheckBit(edxreg, 29);
		mCPUInfo._Ext.IA64_Intel64BitArchitecture					= CheckBit(edxreg, 30);
	}
	//--------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------
	FKForceInline static char* TranslateAssociativeWays( unsigned int nWays, char* buf )
	{
		if( nWays == 0xFFFFFFFF )
		{
			strcpy( buf, "完全相连" );
		}
		else
		{
			if( nWays == 1 )
				strcpy( buf, "单路直接映射" );
			else if( nWays == 0 )
				strcpy( buf, "未知相连路数" );
			else
				sprintf( buf, "%d 路相连", nWays );
		}

		return buf;
	}
	//--------------------------------------------------------------------------------------------------------------
	FKForceInline static void TranslateTLB( ProcessorTLB* tlb )
	{
		char buf[64];

		//检测缓存是否存在
		if(tlb->bPresent)
		{
			sprintf( tlb->szTLB, "%s 页大小, %s, %d 入口", tlb->szPageSize,
				TranslateAssociativeWays( tlb->nAssociativeWays, buf ), tlb->nEntries );
		}
		else
		{
			strcpy( tlb->szTLB, "无" );
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	FKForceInline static void TranslateCache( ProcessorCache* cache )
	{
		char buf[64];

		//检测缓存是否存在
		if (cache->bPresent)
		{
			//如果存在则生成说明文字
			sprintf( cache->szCache, "%s 缓存大小, %s, %d 字节线路", cache->szSize,
				TranslateAssociativeWays( cache->nAssociativeWays, buf ), cache->nLineSize );
			if( cache->bSectored )
				strcat( cache->szCache, ", 已分区" );
		}
		else
		{
			strcpy( cache->szCache, "无" );
		}
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
