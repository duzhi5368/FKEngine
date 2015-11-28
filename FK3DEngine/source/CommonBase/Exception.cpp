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

#include "../../include/CommonBase/CommonBaseInsideHead.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------

	//最后一个异常
	Exception Exception::mLastException;

	//--------------------------------------------------------------------------------------------------------------
	Exception::Exception()
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	//异常构造函数
	//
	// eLevel：	异常类型
	// sDesc：	异常描述
	// sFunc：	抛出异常函数
	// sFile：	抛出异常文件
	// nLine：	抛出异常行号
	Exception::Exception( ExceptionType eType, const String& sDesc, LPCSTR szFunc, LPCSTR szFile, UINT nLine )
		: mType		(eType)
		, mDesc		(sDesc)
		, mFunc		(szFunc)
		, mFile		(szFile)
		, mLine		(nLine)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取异常描述信息
	String Exception::GetDescription()
	{
		char* pszDescription[] =
		{
			"未知的错误类型",			//ERR_UNKNOWN
			"内存不足",					//ERR_NOT_ENOUGH_MEM
			"无法打开指定文件",			//ERR_CANNOT_OPEN_FILE
			"无法读取文件数据",			//ERR_CANNOT_READ_FILE
			"无法写入文件数据",			//ERR_CANNOT_WRITE_FILE
			"非法的传入参数",			//ERR_INVALIDPARAMS
			"名称过长",					//ERR_NAME_TOO_LONG
			"渲染系统 API 错误",		//ERR_RENDER_API_ERROR
			"声音系统 API 错误",		//ERR_SOUND_API_ERROR
			"Windows 操作系统 API 错误",//ERR_WINAPI_ERROR
			"发现重复的项目",			//ERR_DUPLICATE_ITEM
			"找不到指定项目",			//ERR_ITEM_NOT_FOUND
			"致命的内部错误",			//ERR_INTERNAL_ERROR
			"程序请求中断",				//ERR_RT_ASSERTION_FAILED
		};

		//格式化描述信息
		String FullDesc = Misc::FormatString( NULL,	"程序异常终止！\n"
													"\n异常描述： %s"
													"\n错误类型： %s"
													"\n错误文件： %s"
													"\n错误函数： %s"
													"\n错误行号： %d",
													mDesc.GetCharPtr(),
													pszDescription[ mType ],
													mFile.GetCharPtr(),
													mFunc.GetCharPtr(),
													mLine );

		return FullDesc;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置异常数据
	void Exception::SetException( ExceptionType eType, const String& sDesc, LPCSTR szFunc, LPCSTR szFile, UINT nLine )
	{
		mLastException.mType = eType;
		mLastException.mDesc = sDesc;
		mLastException.mFunc = szFunc;
		mLastException.mFile = szFile;
		mLastException.mLine = nLine;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
