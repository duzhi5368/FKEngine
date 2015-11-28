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
*	文件说明：	异常类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

//异常处理宏
#define FKTry							try
#define FKCatch							catch( Exception& sException )
#define FKCatchElse						catch( ... )

#define ShowExceptionMsg				MessageBox( GetActiveWindow(), sException.GetDescription().GetCharPtr(), \
										"错误", MB_OK | MB_ICONERROR | MB_TASKMODAL )
#define ShowUnknownExceptionMsg			MessageBox( GetActiveWindow(), "发生未知的程序错误。", \
										"错误", MB_OK | MB_ICONERROR | MB_TASKMODAL )
#define ShowLastException				MessageBox( GetActiveWindow(), Exception::mLastException.GetDescription(). \
										GetCharPtr(), "错误", MB_OK | MB_ICONERROR | MB_TASKMODAL )

#define Except( Type, Desc )			throw( Exception( Type, Desc, __FUNCTION__, __FILE__, __LINE__ ) )
#define ExceptToUpLevel					throw sException

#define SetLastException( Type, Desc )	Exception::mLastException.SetException( Type, Desc, \
										__FUNCTION__, __FILE__, __LINE__ )
#define GetLastException				Exception::mLastException

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//异常类
	class FK_DLL Exception
	{
	public:
		//异常类型
		enum ExceptionType
		{
			ERR_UNKNOWN,
			ERR_NOT_ENOUGH_MEM,
			ERR_CANNOT_OPEN_FILE,
			ERR_CANNOT_READ_FILE,
			ERR_CANNOT_WRITE_FILE,
			ERR_INVALIDPARAMS,
			ERR_NAME_TOO_LONG,
			ERR_RENDER_API_ERROR,
			ERR_SOUND_API_ERROR,
			ERR_WINAPI_ERROR,
			ERR_DUPLICATE_ITEM,
			ERR_ITEM_NOT_FOUND,
			ERR_INTERNAL_ERROR,
			ERR_RT_ASSERTION_FAILED,
		};

		//最后一个异常
		static Exception	mLastException;

	public:
		ExceptionType	mType;	//错误类型
		String			mDesc;	//错误描述
		String			mFunc;	//错误函数
		String			mFile;	//错误文件名
		UINT			mLine;	//错误行号

	public:
		Exception();
		Exception( ExceptionType eType, const String& sDesc, LPCSTR szFunc, LPCSTR szFile, UINT nLine );

		//获取异常描述信息
		String GetDescription();

		//设置异常数据
		void SetException( ExceptionType eType, const String& sDesc, LPCSTR szFunc, LPCSTR szFile, UINT nLine );
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
