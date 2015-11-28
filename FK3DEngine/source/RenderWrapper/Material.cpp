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
	Material::Material()
		: mDiffuse			( ColorValue::White )
		, mAmbient			( ColorValue::White )
		, mSpecular			( ColorValue::Black )
		, mEmissive			( ColorValue::Black )
		, mPower			(10.0f)
		, mRefCount			(0)
	{
		memset( mName, 0, sizeof(char) * 32 );
		memset( mppTextureState, 0, sizeof(TextureState*) * 8 );
	}
	//--------------------------------------------------------------------------------------------------------------
	Material::~Material()
	{
		DeleteAllTextureState();
	}
	//--------------------------------------------------------------------------------------------------------------
	//增加/减少材质引用计数
	void Material::AddRef()
	{
		++mRefCount;
	}
	//--------------------------------------------------------------------------------------------------------------
	bool Material::DelRef()
	{
		--mRefCount;

		//如果引用为零则自动删除该材质
		if( mRefCount == 0 )
		{
			MaterialManager::mpSingleton->DeleteMaterial( this );
			return true;
		}

		return false;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置/获取材质名称
	void Material::SetName( const char* pName )
	{
		size_t nStrLen = strlen( pName );
		if( nStrLen >= 32 )
			Except( Exception::ERR_NAME_TOO_LONG, "指定的材质名称过长。" );

		memcpy( mName, pName, nStrLen + 1 );
	}
	//--------------------------------------------------------------------------------------------------------------
	const char* Material::GetName()
	{
		return mName;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置/获取漫射色
	void Material::SetDiffuseColor( const ColorValue& Diffuse )
	{
		mDiffuse = Diffuse;
	}
	//--------------------------------------------------------------------------------------------------------------
	ColorValue& Material::GetDiffuseColor()
	{
		return mDiffuse;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置/获取环境色
	void Material::SetAmbientColor( const ColorValue& Ambient )
	{
		mAmbient = Ambient;
	}
	//--------------------------------------------------------------------------------------------------------------
	ColorValue& Material::GetAmbientColor()
	{
		return mAmbient;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置/获取高光色
	void Material::SetSpecularColor( const ColorValue& Specular )
	{
		mSpecular = Specular;
	}
	//--------------------------------------------------------------------------------------------------------------
	ColorValue& Material::GetSpecularColor()
	{
		return mSpecular;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置/获取放射色
	void Material::SetEmissiveColor( const ColorValue& Emissive )
	{
		mEmissive = Emissive;
	}
	//--------------------------------------------------------------------------------------------------------------
	ColorValue& Material::GetEmissiveColor()
	{
		return mEmissive;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置/获取高光能量值
	void Material::SetPower( const float Power )
	{
		mPower = Power;
	}
	//--------------------------------------------------------------------------------------------------------------
	float Material::GetPower()
	{
		return mPower;
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建纹理状态
	TextureState* Material::CreateTextureState( UINT nStage )
	{
		if( mppTextureState[nStage] != NULL )
			Except( Exception::ERR_INVALIDPARAMS, "指定的纹理状态已经存在，无法重复创建。" );

		if( nStage >= RenderSystem::mpSingleton->mMaxTextureNum )
			Except( Exception::ERR_INTERNAL_ERROR, "无法超过支持的纹理状态最大限，创建纹理状态失败。" );

		mppTextureState[nStage] = new TextureState;

		return mppTextureState[nStage];
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除纹理状态
	void Material::DeleteTextureState( UINT nStage )
	{
		SafeDelete( mppTextureState[nStage] );
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除所有纹理状态
	void Material::DeleteAllTextureState()
	{
		for( UINT i=0; i<8; ++i )
			SafeDelete( mppTextureState[i] );
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取纹理状态
	TextureState* Material::GetTextureState( UINT nState )
	{
		return mppTextureState[nState];
	}
	//--------------------------------------------------------------------------------------------------------------
	//应用所有纹理状态
	void Material::ApplyTextureState()
	{
		for( UINT i=0; i<RenderSystem::mpSingleton->mMaxTextureNum; ++i )
		{
			if( mppTextureState[i] != NULL )
			{
				RenderSystem::mpSingleton->_SetTextureState( mppTextureState[i], i );
				RenderSystem::mpSingleton->_SetTexture( mppTextureState[i]->mpTexture, i );
			}
			else
			{
				RenderSystem::mpSingleton->_DisableTextureState( i );
				RenderSystem::mpSingleton->_SetTexture( NULL, i );
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
