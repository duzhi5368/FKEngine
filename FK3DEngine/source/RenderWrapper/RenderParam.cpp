/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	RenderParam
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../../include/RenderWrapper/RenderWrapperInsideHeader.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//根据参数进行渲染
	void FKFastcall RenderParam::RenderByParam() const
	{
		//判断绘制类型
		switch (eDrawType)
		{
		case RenderParam::DT_Basic:
			{
				RenderSystem::mpSingleton->_RenderVertex( sBasicParam.ePrimType, sBasicParam.nVerOffset,
					sBasicParam.nMinIndex, sBasicParam.nVerCount, sBasicParam.nIndexOffset, sBasicParam.nPrimCount );
				return;
			}
		case RenderParam::DT_TPatch:
			{
				return;
			}
		case RenderParam::DT_RPatch:
			{
				return;
			}
		default:
			Except( Exception::ERR_INTERNAL_ERROR, "错误的渲染参数绘制类型。" );
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//是否需要进行进行渲染
	bool FKFastcall RenderParam::NeedRender() const
	{
		//判断绘制类型
		switch (eDrawType)
		{
		case RenderParam::DT_Basic:		return ( sBasicParam.nPrimCount != 0 );
		case RenderParam::DT_TPatch:	return ( sTPatchParam.nPrimCount != 0 );
		case RenderParam::DT_RPatch:	return ( sRPatchParam.nPrimCount != 0 );
		default:
			Except( Exception::ERR_INTERNAL_ERROR, "错误的渲染参数绘制类型。" );
		}

		return false;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
