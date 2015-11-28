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

#pragma once

//==================================================================================================================

#include "../RenderWrapper/Material.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------

	//着色器参数类型
	enum ShaderParamType
	{
		SPT_FLOAT,	// float 型参数（单位长度为 4 float）
		SPT_INT,	// int   型参数（单位长度为 4 int）
		SPT_BOOL,	// bool  型参数（单位长度为 1 bool）		
	};

	//着色器参数结构
	struct ShaderParam
	{
		UINT				nRegIndex;	//寄存器编号
		ShaderParamType		eParamType;	//参数类型
		UINT				nNumUnits;	//单位个数
		void*				pData;		//参数数据指针
	};

	typedef Stack< ShaderParam >	ShaderParamStack;

	//--------------------------------------------------------------------------------------------------------------
	class ShaderManager;
	//--------------------------------------------------------------------------------------------------------------
	//着色器基类
	class FK_DLL BaseShader
	{
	public:
		friend class ShaderManager;

	protected:
		CgLanguageType		mShaderLangType;	//着色器语言类型

		UINT				mNumVaryingParam;	//可变参数数量
		UINT				mNumUniformParam;	//统一参数数量
		UINT				mNumConstantParam;	//常量参数数量

		ShaderParamStack	mShaderParams;		//着色器参数

		DWORD*				mpShaderCode;		//着色器代码
		DWORD				mShaderCodeLen;		//着色器代码长度

		ShaderParam*		mpTransMatParam;	//顶点转换矩阵着色器参数

		ShaderParam*		mpDiffuseParam;		//漫射色着色器参数
		ShaderParam*		mpAmbientParam;		//环境色着色器参数
		ShaderParam*		mpSpecularParam;	//高光色着色器参数
		ShaderParam*		mpEmissiveParam;	//放射色着色器参数

	public:
		BaseShader();
		virtual ~BaseShader();

		//设置着色器自定义参数
		void SetShaderCustomParam( int nIndex, ShaderParamType eType, UINT nNumUnits, void* pData );

		//设置顶点转换矩阵着色器参数
		void SetTransformMatrixParam( int nIndex );

		//设置漫射色着色器参数
		void SetShdaerDiffuseParam( int nIndex );

		//设置环境色着色器参数
		void SetShdaerAmbientParam( int nIndex );

		//设置高光色着色器参数
		void SetShdaerSpecularParam( int nIndex );

		//设置放射色着色器参数
		void SetShdaerEmissiveParam( int nIndex );

		//设置顶点转换矩阵
		void SetTransformMatrix( Camera* pCamera, Matrix4* pWorldMat );

		//设置材质
		void SetMaterial( Material* pMaterial );
	};
	//--------------------------------------------------------------------------------------------------------------
	typedef Stack< BaseShader* >		BaseShaderList;
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
