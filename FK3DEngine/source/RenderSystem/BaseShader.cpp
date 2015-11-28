/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	BaseShader
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
	BaseShader::BaseShader()
		: mShaderLangType		(Unknown)
		, mNumVaryingParam		(0)
		, mNumUniformParam		(0)
		, mNumConstantParam		(0)
		, mpShaderCode			(NULL)
		, mShaderCodeLen		(0)

		, mpTransMatParam		(NULL)
		, mpDiffuseParam		(NULL)
		, mpAmbientParam		(NULL)
		, mpSpecularParam		(NULL)
		, mpEmissiveParam		(NULL)
	{
		mShaderParams.Initialize( 3, 3 );
	}
	//--------------------------------------------------------------------------------------------------------------
	BaseShader::~BaseShader()
	{
		SafeFree( mpShaderCode );

		mShaderParams.Release();
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置着色器自定义参数
	void BaseShader::SetShaderCustomParam( int nIndex, ShaderParamType eType, UINT nNumUnits, void* pData )
	{
		if( nIndex < 0 || (UINT)nIndex >= mNumUniformParam )
			Except( Exception::ERR_INVALIDPARAMS, "指定的着色器参数不存在。" );

		ShaderParam* pParam = mShaderParams.Begin() + nIndex;

		//存储来源信息
		pParam->eParamType	= eType;
		pParam->nNumUnits	= nNumUnits;
		pParam->pData		= pData;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置顶点转换矩阵着色器参数
	void BaseShader::SetTransformMatrixParam( int nIndex )
	{
		if( nIndex < 0 )
		{
			mpTransMatParam = NULL;
			return;
		}
		else if( (UINT)nIndex >= mNumUniformParam )
		{
			Except( Exception::ERR_INVALIDPARAMS, "指定的着色器参数不存在。" );
		}

		mpTransMatParam = mShaderParams.Begin() + nIndex;

		mpTransMatParam->eParamType	= SPT_FLOAT;
		mpTransMatParam->nNumUnits	= 4;
		mpTransMatParam->pData		= NULL;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置漫射色着色器参数
	void BaseShader::SetShdaerDiffuseParam( int nIndex )
	{
		if( nIndex < 0 )
		{
			mpDiffuseParam = NULL;
			return;
		}
		else if( (UINT)nIndex >= mNumUniformParam )
		{
			Except( Exception::ERR_INVALIDPARAMS, "指定的着色器参数不存在。" );
		}

		mpDiffuseParam = mShaderParams.Begin() + nIndex;

		mpDiffuseParam->eParamType	= SPT_FLOAT;
		mpDiffuseParam->nNumUnits	= 1;
		mpDiffuseParam->pData		= NULL;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置环境色着色器参数
	void BaseShader::SetShdaerAmbientParam( int nIndex )
	{
		if( nIndex < 0 )
		{
			mpAmbientParam = NULL;
			return;
		}
		else if( (UINT)nIndex >= mNumUniformParam )
		{
			Except( Exception::ERR_INVALIDPARAMS, "指定的着色器参数不存在。" );
		}

		mpAmbientParam = mShaderParams.Begin() + nIndex;

		mpAmbientParam->eParamType	= SPT_FLOAT;
		mpAmbientParam->nNumUnits	= 1;
		mpAmbientParam->pData		= NULL;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置高光色着色器参数
	void BaseShader::SetShdaerSpecularParam( int nIndex )
	{
		if( nIndex < 0 )
		{
			mpSpecularParam = NULL;
			return;
		}
		else if( (UINT)nIndex >= mNumUniformParam )
		{
			Except( Exception::ERR_INVALIDPARAMS, "指定的着色器参数不存在。" );
		}

		mpSpecularParam = mShaderParams.Begin() + nIndex;

		mpSpecularParam->eParamType	= SPT_FLOAT;
		mpSpecularParam->nNumUnits	= 1;
		mpSpecularParam->pData		= NULL;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置放射色着色器参数
	void BaseShader::SetShdaerEmissiveParam( int nIndex )
	{
		if( nIndex < 0 )
		{
			mpEmissiveParam = NULL;
			return;
		}
		else if( (UINT)nIndex >= mNumUniformParam )
		{
			Except( Exception::ERR_INVALIDPARAMS, "指定的着色器参数不存在。" );
		}

		mpEmissiveParam = mShaderParams.Begin() + nIndex;

		mpEmissiveParam->eParamType	= SPT_FLOAT;
		mpEmissiveParam->nNumUnits	= 1;
		mpEmissiveParam->pData		= NULL;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置顶点转换矩阵
	void BaseShader::SetTransformMatrix( Camera* pCamera, Matrix4* pWorldMat )
	{
		if( mpTransMatParam == NULL )
			return;

		//根据摄像机的观察投影矩阵和世界矩阵计算转换矩阵
		static Matrix4 TransformMatrix;
		Mat4TransMat4( &TransformMatrix, pWorldMat, &pCamera->mViewProjMatrix );
		TransformMatrix.SetTranspose();

		mpTransMatParam->pData = (void*)&TransformMatrix;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置材质
	void BaseShader::SetMaterial( Material* pMaterial )
	{
		if( mpDiffuseParam != NULL )
			mpDiffuseParam->pData = (void*)&pMaterial->mDiffuse;

		if( mpAmbientParam != NULL )
			mpAmbientParam->pData = (void*)&pMaterial->mAmbient;

		if( mpSpecularParam != NULL )
			mpSpecularParam->pData = (void*)&pMaterial->mSpecular;

		if( mpEmissiveParam != NULL )
			mpEmissiveParam->pData = (void*)&pMaterial->mEmissive;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
