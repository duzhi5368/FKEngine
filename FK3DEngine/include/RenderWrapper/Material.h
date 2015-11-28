/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Material
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	材质类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================
#include "RenderState.h"
//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	class TextureState;
	//--------------------------------------------------------------------------------------------------------------
	//材质类
	class FK_DLL Material
	{
	public:
		char				mName[32];			//材质名称

		ColorValue			mDiffuse;			//漫射色
		ColorValue			mAmbient;			//环境色
		ColorValue			mSpecular;			//高光色
		ColorValue			mEmissive;			//放射色
		float				mPower;				//高光能量值

		RenderState			mRenderState;		//材质渲染状态
		TextureState*		mppTextureState[8];	//材质纹理状态

		UINT				mRefCount;			//引用计数

	public:
		Material();
		~Material();

		//增加/减少材质引用计数
		void AddRef();
		bool DelRef();

		//设置/获取材质名称
		void SetName( const char* pName );
		const char* GetName();

		//设置/获取漫射色
		void SetDiffuseColor( const ColorValue& Diffuse );
		ColorValue& GetDiffuseColor();

		//设置/获取环境色
		void SetAmbientColor( const ColorValue& Ambient );
		ColorValue& GetAmbientColor();

		//设置/获取高光色
		void SetSpecularColor( const ColorValue& Specular );
		ColorValue& GetSpecularColor();

		//设置/获取放射色
		void SetEmissiveColor( const ColorValue& Emissive );
		ColorValue& GetEmissiveColor();

		//设置/获取高光能量值
		void SetPower( const float Power );
		float GetPower();

		//创建纹理状态
		TextureState* CreateTextureState( UINT nStage = 0 );

		//删除纹理状态
		void DeleteTextureState( UINT nStage = 0 );

		//删除所有纹理状态
		void DeleteAllTextureState();

		//获取纹理状态
		TextureState* GetTextureState( UINT nState = 0 );

		//应用所有纹理状态
		void ApplyTextureState();
	};
	//--------------------------------------------------------------------------------------------------------------
	typedef Stack< Material* >		MaterialList;
	typedef HashMap< Material* >	MaterialMap;
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
