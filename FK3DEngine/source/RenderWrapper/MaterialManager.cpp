/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	MaterialManager
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
	MaterialManager::MaterialManager()
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	MaterialManager::~MaterialManager()
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	//初始化材质管理器
	void MaterialManager::Initialize( UINT nNumMaterial )
	{
		mMaterialMap.Initialize( nNumMaterial );
		mMaterialList.Initialize( nNumMaterial, 100 );
	}
	//--------------------------------------------------------------------------------------------------------------
	//释放材质管理器
	void MaterialManager::Release()
	{
		DeleteAllMaterial();

		mMaterialMap.Release();
		mMaterialList.Release();
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建新的材质
	Material* MaterialManager::CreateMaterial( const char* szMaterialName )
	{
		//创建新材质
		Material* pMaterial = new Material;
		pMaterial->SetName( szMaterialName );

		//将材质加入列表
		if( !mMaterialMap.Add( pMaterial, szMaterialName ) )
		{
			SafeDelete( pMaterial );
			Except( Exception::ERR_INVALIDPARAMS, (String)"指定名称为 '" + szMaterialName +
				"' 的材质已经存在。" );
		}

		*mMaterialList.Push() = pMaterial;

		return pMaterial;
	}
	//--------------------------------------------------------------------------------------------------------------
	//根据纹理创建纹理材质
	Material* MaterialManager::CreateMaterial( const char* szTexFileName, MaterialType eType )
	{
		//创建新材质
		Material* pMaterial = new Material;
		
		switch (eType)
		{
		case Opacity:			pMaterial->mRenderState.SetOpacity();		break;
		case Transparent:		pMaterial->mRenderState.SetTransparent();	break;
		case AlphaMask:			pMaterial->mRenderState.SetAlphaMask();		break;
		}

		//复制材质数据
		pMaterial->mAmbient		= ColorValue::White;
		pMaterial->mDiffuse		= ColorValue::White;
		pMaterial->mSpecular	= ColorValue::Black;
		pMaterial->mEmissive	= ColorValue::Black;
		pMaterial->mPower		= 0.0f;

		if( szTexFileName != NULL )
		{
			//根据纹理名称设置材质名
			size_t nLen = strlen( szTexFileName );
			memcpy( pMaterial->mName, szTexFileName, nLen );
			pMaterial->mName[nLen] = '\0';

			//根据当前的显示颜色格式和材质类型选择最适当的纹理格式
			PixelFormat eTexFormat;
			if( eType == Opacity )
				eTexFormat = ( RenderSystem::mDisplayColorDepth > 2 ) ? RenderSystem::mDisplayFormat : PF_R5G6B5;
			else
				eTexFormat = PF_A8R8G8B8;

			//创建材质纹理
			Texture* pTexture = TextureManager::mpSingleton->LoadFromFile( szTexFileName, 0, eTexFormat );
			TextureState* pTextureState = pMaterial->CreateTextureState();
			pTextureState->SetTexture( pTexture );
		}

		//将材质加入列表
		if( !mMaterialMap.Add( pMaterial, pMaterial->mName ) )
		{
			SafeDelete( pMaterial );
			Except( Exception::ERR_INVALIDPARAMS, (String)"指定名称为 '" + pMaterial->mName +
				"' 的材质已经存在。" );
		}

		*mMaterialList.Push() = pMaterial;

		return pMaterial;
	}
	//--------------------------------------------------------------------------------------------------------------
	//根据材质数据创建新的材质
	Material* MaterialManager::CreateMaterial( const MaterialData* pMaterialData )
	{
		//创建新材质
		Material* pMaterial = new Material;

		switch (pMaterialData->eType)
		{
		case Opacity:			pMaterial->mRenderState.SetOpacity();		break;
		case Transparent:		pMaterial->mRenderState.SetTransparent();	break;
		case AlphaMask:			pMaterial->mRenderState.SetAlphaMask();		break;
		}

		//复制材质数据
		pMaterial->mAmbient		= pMaterialData->Ambient;
		pMaterial->mDiffuse		= pMaterialData->Diffuse;
		pMaterial->mSpecular	= pMaterialData->Specular;
		pMaterial->mEmissive	= pMaterialData->Emissive;
		pMaterial->mPower		= pMaterialData->Power;

		memcpy( pMaterial->mName, pMaterialData->szMaterialName, 32 );

		//创建材质纹理
		if( pMaterialData->bHaveTexture )
		{
			//根据当前的显示颜色格式和材质类型选择最适当的纹理格式
			PixelFormat eTexFormat;
			if( pMaterialData->eType == Opacity )
				eTexFormat = ( RenderSystem::mDisplayColorDepth > 2 ) ? RenderSystem::mDisplayFormat : PF_R5G6B5;
			else
				eTexFormat = PF_A8R8G8B8;

			//创建材质纹理
			Texture* pTexture = TextureManager::mpSingleton->LoadFromFile( pMaterialData->szTexFileName, 0, eTexFormat );
			TextureState* pTextureState = pMaterial->CreateTextureState();
			pTextureState->SetTexture( pTexture );
		}

		//将材质加入列表
		if( !mMaterialMap.Add( pMaterial, pMaterialData->szMaterialName ) )
		{
			SafeDelete( pMaterial );
			Except( Exception::ERR_INVALIDPARAMS, (String)"指定名称为 '" + pMaterialData->szMaterialName +
				"' 的材质已经存在。" );
		}

		*mMaterialList.Push() = pMaterial;

		return pMaterial;
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除原有材质
	void MaterialManager::DeleteMaterial( Material* pMaterial )
	{
		if( pMaterial == NULL )
			return;

		//从列表中删除数据
		mMaterialMap.Del( pMaterial->mName );

		MaterialList::Iterator it = mMaterialList.Begin();
		MaterialList::Iterator end = mMaterialList.End();
		for(; it!=end; ++it )
		{
			if( *it == pMaterial )
			{
				mMaterialList.Erase( it );
				delete pMaterial;
				return;
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除所有材质
	void MaterialManager::DeleteAllMaterial()
	{
		MaterialList::Iterator it = mMaterialList.Begin();
		MaterialList::Iterator end = mMaterialList.End();
		for(; it!=end; ++it )
			SafeDelete( *it );

		mMaterialList.Clear();
		mMaterialMap.Clear();
	}
	//--------------------------------------------------------------------------------------------------------------
	//根据名称获取材质
	FKInline Material* MaterialManager::GetMaterial( const char* szMaterialName )
	{
		return *mMaterialMap.Find( szMaterialName );
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取材质数量
	FKInline UINT MaterialManager::GetMaterialNum()
	{
		return mMaterialList.Size();
	}
	//--------------------------------------------------------------------------------------------------------------
	//根据材质类型选择最适当的纹理格式
	PixelFormat MaterialManager::GetBestTextureFormat( MaterialType eMatType, PixelFormat eOrgPixelFormat )
	{
		//获取颜色格式字节数
		UINT nColorDepth = Misc::GetColorDepth( eOrgPixelFormat );

		switch (eMatType)
		{
		case Opacity:
			return ( nColorDepth > 2 ) ? eOrgPixelFormat : PF_R5G6B5;
		case AlphaMask:
		case Transparent:
			return PF_A8R8G8B8;
		default:
			Except( Exception::ERR_INVALIDPARAMS, "在根据材质类型选择纹理格式时，传入了错误的材质类型。" );
		}

		return PF_UNKNOWN;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
