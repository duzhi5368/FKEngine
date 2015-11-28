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

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	class Texture;
	//--------------------------------------------------------------------------------------------------------------
	//纹理状态类
	class FK_DLL TextureState
	{
	public:
		Texture*				mpTexture;		//纹理

		TextureFilterType		mFilterMag;		//放大纹理过滤类型
		TextureFilterType		mFilterMin;		//缩小纹理过滤类型
		TextureFilterType		mFilterMip;		//多级纹理过滤类型

		TextureAddressType		mAddressU;		//纹理 U 坐标寻址类型
		TextureAddressType		mAddressV;		//纹理 V 坐标寻址类型
		TextureAddressType		mAddressW;		//纹理 W 坐标寻址类型

		TextureOperation		mColorOp;		//纹理颜色操作类型
		TextureArgument			mColorArg1;		//纹理颜色操作参数 1
		TextureArgument			mColorArg2;		//纹理颜色操作参数 2

		TextureOperation		mAlphaOp;		//纹理 Alpha 操作类型
		TextureArgument			mAlphaArg1;		//纹理 Alpha 操作参数 1
		TextureArgument			mAlphaArg2;		//纹理 Alpha 操作参数 2

	public:
		TextureState();
		~TextureState();

		//设置/获取纹理
		void SetTexture( Texture* pTexture );
		Texture* GetTexture();

		//设置/获取纹理过滤类型
		void SetTextureFilter( SamplerType eSamplerType, TextureFilterType eFilterType );
		TextureFilterType GetTextureFilter( SamplerType eSamplerType );

		//设置/获取纹理寻址类型
		void SetTextureAddress( SamplerType eSamplerType, TextureAddressType eAddressType );
		TextureAddressType GetTextureAddress( SamplerType eSamplerType );

		//设置/获取纹理操作类型状态
		void SetTextureOperation( TextureStateType eStateType, TextureOperation eOperationType );
		TextureOperation GetTextureOperation( TextureStateType eStateType );

		//设置/获取纹理操作参数状态
		void SetTextureArgument( TextureStateType eStateType, TextureArgument eArgumentType );
		TextureArgument GetTextureArgument( TextureStateType eStateType );
	};
	//--------------------------------------------------------------------------------------------------------------
};

//==================================================================================================================
