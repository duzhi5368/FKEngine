/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Exception
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include <string>
//--------------------------------------------------------------------
//! 异常处理宏
#define FK_EXCEPTION( num, desc, src ) throw( FK2DEngine::CException( num, desc, src, __FILE__, __LINE__ ) )
//--------------------------------------------------------------------
//! 堆栈展开选项
//@remarks 不推荐使用 FKUnguard 和 FKUnguardRet
#if FK_STACK_UNWINDING == 1
	#define FKGuard( a )	FK2DEngine::CAutomaticGuardUnguard _auto_guard_object( (a) )
	#define FKUnguard()
	#define FKUnguardRet( a )	return a
#else
	#define FKGuard( a )
	#define FKUnguard()
	#define FKUnguardRet( a )	return a
#endif
//--------------------------------------------------------------------
//! 断言处理模式
#if FK_ASSERT_MODE == 1
	#ifdef _DEBUG
		#define FKAssert( a, b )		assert( (a) && (b) )
	#else
		#define FKAssert( a, b )		if( !(a) ) FK_EXCEPTION( FK2DEngine::eEC_RuntimeAssertionFailed, (b), L"没有函数信息" )
	#endif
#elif FK_ASSERT_MODE == 2
	#define FKAssert( a, b )			if( !(a) ) FK_EXCEPTION( FK2DEngine::eEC_RuntimeAssertionFailed, (b), L"没有函数信息" )
#else 
	#define FKAssert( a, b )			assert( (a) && (b) )  
#endif
//--------------------------------------------------------------------
namespace FK2DEngine
{
	//! 堆栈深度
	const static unsigned int FK_CALL_STACK_DEPTH = 512;

	//! 异常处理类
	//@remarks	应用程序必须保证所有异常被捕获，也就是说，所有函数都必须被
	//			try{} catch( FK2DEngine::CException& e ){} 包含
	class CException
	{
	public:
		//! 错误类型码
		enum ENUM_ExceptionCode
		{
			eEC_None,

			eEC_InvalidParams,						// 无效参数
			eEC_RuntimeAssertionFailed,				// 断言未通过
			eEC_ErrorInternal,						// 内部数据流错误
			eEC_ItemCannotFind,						// 元素没有找到
			eEC_FileCannotFind,						// 文件无法找到
			eEC_HardwareCannotSupport,				// 硬件无法支持
			eEC_IniLoadError,						// INI文件读取失败
			//TODO：替代std的Exception

			eEC_Max,
		};
	protected:
		long					m_nLine;
		int						m_nNumber;
		std::wstring			m_szDescription;
		std::wstring			m_szSource;
		std::wstring			m_szFile;
		unsigned short			m_usStackDepth;
	protected:
		static CException*		s_pLastException;
		static std::wstring		s_FunctionStack[FK_CALL_STACK_DEPTH];
		static unsigned short	s_usStackDepth;
	public:
		CException( int p_nNumber, const std::wstring& p_szDesc, const std::wstring& p_szSrc );
		CException( int p_nNumber, const std::wstring& p_szDesc, const std::wstring& p_szSrc,
			const char* p_szFil, long p_lLine );
		CException( const CException& p_Other );
		inline void operator = ( const CException& p_Other );
	public:
		//! 得到详细的错误描述
		std::wstring GetFullDescription() const;

		//! 得到错误码
		int	GetNumber() const throw();

		//! 得到异常源
		const std::wstring& GetSource() const
		{
			return m_szSource;
		}

		//! 得到文件名
		const std::wstring& GetFile() const
		{
			return m_szFile;
		}

		//! 得到行号
		long GetLine() const
		{
			return m_nLine;
		}

		//! 得到简单的描述信息
		const std::wstring& GetDescription() const
		{
			return m_szDescription;
		}

		//! 得到指向最后一个异常对象的指针
		static CException* GetLastException() throw();

		//! 函数入栈
		static void PushFunction( const std::wstring& p_szFuncName ) throw();

		//! 函数出栈
		static void PopFunction() throw();
	};

	class CAutomaticGuardUnguard
	{
	public:
		CAutomaticGuardUnguard( const std::wstring& p_FuncName ) throw()
		{
			CException::PushFunction( p_FuncName );
		}
		~CAutomaticGuardUnguard() throw()
		{
			CException::PopFunction();
		}
	};
}