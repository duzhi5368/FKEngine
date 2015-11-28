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
*	文件说明：	处理器信息类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	struct ProcessorExtensions
	{
		bool			FPU_FloatingPointUnit;							//浮点处理单元
		bool			VME_Virtual8086ModeEnhancements;				//虚拟 8086 模式增强
		bool			DE_DebuggingExtensions;							//扩展调试指令
		bool			PSE_PageSizeExtensions;							//页大小扩展
		bool			TSC_TimeStampCounter;							//时间计数器
		bool			MSR_ModelSpecificRegisters;						//模块细节寄存器
		bool			PAE_PhysicalAddressExtension;					//物理地址扩展
		bool			MCA_MachineCheckArchitecture;					//机器检测架构
		bool			MCE_MachineCheckException;						//机器检测异常
		bool			CX8_COMPXCHG8B_Instruction;						//COMPXCHG8B 指令
		bool			APIC_AdvancedProgrammableInterruptController;	//高级可编程中断控制器
		unsigned int	APIC_ID;										//高级可编程中断控制器编号
		bool			SEP_FastSystemCall;								//快速系统级调用
		bool			MTRR_MemoryTypeRangeRegisters;					//内存类型范围寄存器
		bool			PGE_PTE_GlobalFlag;								//PTE 全局标志
		bool			CMOV_ConditionalMoveAndCompareInstructions;		//条件数据传送和比较指令集
		bool			FGPAT_PageAttributeTable;						//页属性表
		bool			PSE36_36bitPageSizeExtension;					//36 位页大小扩展
		bool			PN_ProcessorSerialNumber;						//处理器序列号
		bool			CLFSH_CFLUSH_Instruction;						//CLFLUSH 指令支持
		unsigned int	CLFLUSH_InstructionCacheLineSize;				//CLFLUSH 指令缓存大小
		bool			DS_DebugStore;									//调试存储器
		bool			ACPI_ThermalMonitorAndClockControl;				//温度探测/时钟控制
		bool			EMMX_MultimediaExtensions;						//扩展多媒体指令集
		bool			MMX_MultimediaExtensions;						//多媒体指令集
		bool			FXSR_FastStreamingSIMD_ExtensionsSaveRestore;	//快速 SIMD 指令集存储/恢复
		bool			SSE_StreamingSIMD_Extensions;					//数据流单指令多数据扩展指令集
		bool			SSE2_StreamingSIMD2_Extensions;					//数据流单指令多数据扩展指令集 2
		bool			SS_SelfSnoop;									//自探测
		bool			HT_HyperThreading;								//超线程
		unsigned int	HT_HyterThreadingSiblings;						//超线程
		bool			TM_ThermalMonitor;								//温度探测
		bool			IA64_Intel64BitArchitecture;					//Intel 64 位架构
		bool			_3DNOW_InstructionExtensions;					//3DNow! 指令集
		bool			_E3DNOW_InstructionExtensions;					//增强 3DNow! 指令集
		bool			AA64_AMD64BitArchitecture;						//AMD 64 位架构
	};
	//--------------------------------------------------------------------------------------------------------------
	struct ProcessorCache
	{
		bool			bPresent;						//是否存在
		char			szSize[32];						//大小说明
		unsigned int	nAssociativeWays;				//相连路数
		unsigned int	nLineSize;						//线路大小
		bool			bSectored;						//是否已分区
		char			szCache[128];					//缓存说明
	};
	//--------------------------------------------------------------------------------------------------------------
	struct ProcessorL1Cache
	{
		ProcessorCache	Instruction;					//指令缓存
		ProcessorCache	Data;							//数据缓存
	};
	//--------------------------------------------------------------------------------------------------------------
	struct ProcessorTLB
	{
		bool			bPresent;						//是否存在
		char			szPageSize[32];					//页大小说明
		unsigned int	nAssociativeWays;				//相连路数
		unsigned int	nEntries;						//入口数
		char			szTLB[128];						//缓存说明
	};
	//--------------------------------------------------------------------------------------------------------------
	struct CPUInfo
	{
		char				szVendor[16];				//厂商说明
		unsigned int		nFamily;					//系列
		unsigned int		nExtendedFamily;			//扩展系列
		char				szFamily[64];				//系列说明
		unsigned int		nModel;						//型号
		unsigned int		nExtendedModel;				//扩展型号
		char				szModel[128];				//型号说明
		unsigned int		nStepping;					//次版本
		unsigned int		nType;						//类型
		char				szType[64];					//类型说明
		unsigned int		nBrandID;					//商标编号
		char				szBrandID[64];				//商标编号说明
		char				szProcessorSerial[64];		//处理器序列号
		unsigned long		MaxSupportedLevel;			//最大支持等级
		unsigned long		MaxSupportedExtendedLevel;	//扩展最大支持等级

		ProcessorExtensions	_Ext;
		ProcessorL1Cache	_L1;
		ProcessorCache		_L2;
		ProcessorCache		_L3;
		ProcessorCache		_Trace;
		ProcessorTLB		_Instruction;
		ProcessorTLB		_Data;
	};
	//--------------------------------------------------------------------------------------------------------------
	//处理器信息类
	class FK_DLL ProcessorInfo
	{
	public:
		static __int64		mqwFrequency;	// CPU 频率
		static char			mCPUName[128];	// CPU 名称
		static CPUInfo		mCPUInfo;		// CPU 信息

	public:
		//获取 CPU 频率
		static __int64 GetCPUFrequency( unsigned int nMeasureTime );

		//获取 CPU 信息
		static const CPUInfo* GetCPUInfo();

		//获取 CPU 说明信息字符串
		static String GetCPUInfoText();

		//将 CPU 说明信息写入指定文本文件
		static bool WriteCPUInfoTextFile( const char* szFileName );

	protected:
		//解析 AMD 处理器信息
		static bool _AnalyzeAMDProcessor();

		//解析 Intel 处理器信息
		static bool _AnalyzeIntelProcessor();

		//解析未知处理器信息
		static bool _AnalyzeUnknownProcessor();

		//检测 CPUID 指令是否可用
		static bool _CheckCPUIDPresence();

		//解码处理器位配置信息
		static void _DecodeProcessorConfiguration( unsigned int cfg );

		//翻译处理器配置信息
		static void _TranslateProcessorConfiguration();

		//获取标准处理器配置信息
		static void _GetStandardProcessorConfiguration();

		//获取标准处理器扩展信息
		static void _GetStandardProcessorExtensions();		
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
