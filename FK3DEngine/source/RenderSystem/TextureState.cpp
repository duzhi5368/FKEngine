/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	TextureState
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../../include/RenderSystem/RenderSystemInsideHeader.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	TextureState::TextureState()
		: mpTexture			(NULL)				//纹理
		, mFilterMag		(TFT_LINEAR)		//放大纹理过滤类型
		, mFilterMin		(TFT_LINEAR)		//缩小纹理过滤类型
		, mFilterMip		(TFT_LINEAR)		//多级纹理过滤类型
		, mAddressU			(TAT_WRAP)			//纹理 U 坐标寻址类型
		, mAddressV			(TAT_WRAP)			//纹理 V 坐标寻址类型
		, mAddressW			(TAT_WRAP)			//纹理 W 坐标寻址类型
		, mColorOp			(TO_MODULATE)		//纹理颜色操作类型
		, mColorArg1		(TA_TEXTURE)		//纹理颜色操作参数 2
		, mColorArg2		(TA_CURRENT)		//纹理颜色操作参数 2
		, mAlphaOp			(TO_SELECTARG1)		//纹理 Alpha 操作类型
		, mAlphaArg1		(TA_TEXTURE)		//纹理 Alpha 操作参数 1
		, mAlphaArg2		(TA_CURRENT)		//纹理 Alpha 操作参数 2
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	TextureState::~TextureState()
	{
		if( mpTexture != NULL )
			TextureManager::mpSingleton->DeleteTexture( mpTexture );
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置纹理
	void TextureState::SetTexture( Texture* pTexture )
	{
		mpTexture = pTexture;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取纹理
	Texture* TextureState::GetTexture()
	{
		return mpTexture;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置纹理过滤类型
	void TextureState::SetTextureFilter( SamplerType eSamplerType, TextureFilterType eFilterType )
	{
		switch (eSamplerType)
		{
		case ST_MAGFILTER:		mFilterMag = eFilterType;		return;
		case ST_MINFILTER:		mFilterMin = eFilterType;		return;
		case ST_MIPFILTER:		mFilterMip = eFilterType;		return;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "取样类型错误，无法设置纹理过滤类型渲染状态。" );
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取纹理过滤类型
	TextureFilterType TextureState::GetTextureFilter( SamplerType eSamplerType )
	{
		switch (eSamplerType)
		{
		case ST_MAGFILTER:		return mFilterMag;
		case ST_MINFILTER:		return mFilterMin;
		case ST_MIPFILTER:		return mFilterMip;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "取样类型错误，无法获取纹理过滤类型渲染状态。" );
		}

		return (TextureFilterType)0;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置纹理寻址类型
	void TextureState::SetTextureAddress( SamplerType eSamplerType, TextureAddressType eAddressType )
	{
		switch (eSamplerType)
		{
		case ST_MAGFILTER:		mAddressU = eAddressType;		return;
		case ST_MINFILTER:		mAddressV = eAddressType;		return;
		case ST_MIPFILTER:		mAddressW = eAddressType;		return;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "取样类型错误，无法设置纹理寻址类型渲染状态。" );
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取纹理寻址类型
	TextureAddressType TextureState::GetTextureAddress( SamplerType eSamplerType )
	{
		switch (eSamplerType)
		{
		case ST_MAGFILTER:		return mAddressU;
		case ST_MINFILTER:		return mAddressV;
		case ST_MIPFILTER:		return mAddressW;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "取样类型错误，无法获取纹理寻址类型渲染状态。" );
		}

		return (TextureAddressType)0;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置纹理操作类型状态
	void TextureState::SetTextureOperation( TextureStateType eStateType, TextureOperation eOperationType )
	{
		switch (eStateType)
		{
		case TST_COLOROP:		mColorOp = eOperationType;		return;
		case TST_ALPHAOP:		mAlphaOp = eOperationType;		return;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "纹理状态类型错误，无法设置纹理操作类型状态。" );
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取纹理操作类型状态
	TextureOperation TextureState::GetTextureOperation( TextureStateType eStateType )
	{
		switch (eStateType)
		{
		case TST_COLOROP:		return mColorOp;
		case TST_ALPHAOP:		return mAlphaOp;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "纹理状态类型错误，无法获取纹理操作类型状态。" );
		}

		return (TextureOperation)0;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置纹理操作参数状态
	void TextureState::SetTextureArgument( TextureStateType eStateType, TextureArgument eArgumentType )
	{
		switch (eStateType)
		{
		case TST_COLORARG1:		mColorArg1 = eArgumentType;		return;
		case TST_COLORARG2:		mColorArg2 = eArgumentType;		return;
		case TST_ALPHAARG1:		mAlphaArg1 = eArgumentType;		return;
		case TST_ALPHAARG2:		mAlphaArg2 = eArgumentType;		return;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "纹理状态类型错误，无法设置纹理操作参数状态。" );
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取纹理操作类型状态
	TextureArgument TextureState::GetTextureArgument( TextureStateType eStateType )
	{
		switch (eStateType)
		{
		case TST_COLORARG1:		return mColorArg1;
		case TST_COLORARG2:		return mColorArg2;
		case TST_ALPHAARG1:		return mAlphaArg1;
		case TST_ALPHAARG2:		return mAlphaArg2;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "纹理状态类型错误，无法获取纹理操作参数状态。" );
		}

		return (TextureArgument)0;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
