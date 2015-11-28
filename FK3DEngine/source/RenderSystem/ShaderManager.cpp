/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	ShaderManager
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
	ShaderManager::ShaderManager()
		: mpShaderPack		(NULL)
	{
		mVSList.Initialize( 10, 10 );
		mPSList.Initialize( 10, 10 );
	}
	//--------------------------------------------------------------------------------------------------------------
	ShaderManager::~ShaderManager()
	{
		DeleteAllVertexShader();
		DeleteAllPixelShader();

		mVSList.Release();
		mPSList.Release();
	}
	//--------------------------------------------------------------------------------------------------------------
	//关联着色器代码打包文件
	void ShaderManager::SetProgramPackFile( FileObject* pPackFile )
	{
		mpShaderPack = pPackFile;
	}
	//--------------------------------------------------------------------------------------------------------------
	//从指定的文件中载入着色器
	void ShaderManager::CreateShaderFromFile( LPCSTR szFileName )
	{
		//读取文件数据
		mpShaderPack->Open( szFileName, OT_Read );
		DWORD dwFileLen = mpShaderPack->GetLength();

		BYTE* pFileData = (BYTE*)malloc( dwFileLen );
		if( pFileData == NULL )
			Except( Exception::ERR_NOT_ENOUGH_MEM, "在加载 SCG 文件时无法分配数据缓存失败。" );

		mpShaderPack->Read( pFileData, dwFileLen );
		mpShaderPack->Close();
		BYTE* pDataPtr = pFileData;


		//验证文件头
		if( 0 != memcmp( pDataPtr, szScgFileHeader, sizeof(szScgFileHeader) ) )
		{
			free( pFileData );
			Except( Exception::ERR_INTERNAL_ERROR, (String)"非法的 SCG 着色器程序文件 '" + szFileName + "'。" );
		}
		pDataPtr += sizeof(szScgFileHeader);

		//读取着色器数量
		UINT nNumShaders = *(UINT*)pDataPtr;
		pDataPtr += sizeof(UINT);


		//循环载入所有着色器
		for( UINT i=0; i<nNumShaders; ++i )
		{
			//读取着色器信息头
			CgShaderData* pShaderData = (CgShaderData*)pDataPtr;
			pDataPtr += sizeof(CgShaderData);

			//读取统一参数寄存器索引值数据
			DWORD* pUniformParamIndex = (DWORD*)pDataPtr;
			size_t nUniformParamIndexLen = sizeof(DWORD) * pShaderData->nNumUniformParam;
			pDataPtr += nUniformParamIndexLen;

			//读取着色器代码
			DWORD* pShaderCode = (DWORD*)pDataPtr;
			pDataPtr += pShaderData->nShaderCodeLen;


			BaseShader* pShader = NULL;

			//根据着色器类型判断是顶点着色器还是像素着色器
			bool bIsVS;
			switch ( pShaderData->eShaderLangType )
			{
			case DX_VS_11:		case DX_VS_20:		case DX_VS_2X:
			case OGL_VP_20:		case OGL_VP_30:		case OGL_ARB_VP1:
				{
					pShader = static_cast< BaseShader* >( CreateVertexShader() );
					bIsVS = true;
					break;
				}
			case DX_PS_11:		case DX_PS_12:		case DX_PS_13:		case DX_PS_20:
			case DX_PS_2X:		case OGL_FP_20:		case OGL_FP_30:		case OGL_ARB_FP1:
				{
					pShader = static_cast< BaseShader* >( CreatePixelShader() );
					bIsVS = false;
					break;
				}
			default:
				{
					free( pFileData );
					Except( Exception::ERR_INTERNAL_ERROR, "错误的着色器类型。" );
				}
			}


			//设置着色器参数
			pShader->mNumVaryingParam	= pShaderData->nNumVaryingParam;
			pShader->mNumUniformParam	= pShaderData->nNumUniformParam;
			pShader->mNumConstantParam	= pShaderData->nNumConstantParam;

			//存储着色器统一参数寄存器索引值
			for( UINT i=0; i<pShader->mNumUniformParam; ++i )
			{
				ShaderParam* pParam = pShader->mShaderParams.Push();
				pParam->nRegIndex = (UINT)*pUniformParamIndex++;
			}

			//存储着色器代码
			pShader->mShaderCodeLen = pShaderData->nShaderCodeLen;
			pShader->mpShaderCode = (DWORD*)malloc( pShader->mShaderCodeLen );
			if( pShader->mpShaderCode == NULL )
			{
				free( pFileData );
				Except( Exception::ERR_INTERNAL_ERROR, "分配着色器代码缓存失败。" );
			}

			memcpy( pShader->mpShaderCode, pShaderCode, pShader->mShaderCodeLen );


			//构建着色器
			if( bIsVS )
			{
				VertexShader* pVS = static_cast< VertexShader* >( pShader );
				pVS->BuildVertexShader();
			}
			else
			{
				PixelShader* pPS = static_cast< PixelShader* >( pShader );
				pPS->BuildPixelShader();
			}
		}


		free( pFileData );
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取指定编号的顶点着色器
	VertexShader* ShaderManager::GetVertexShader( UINT nIndex )
	{
		if( nIndex >= mVSList.Size() )
			Except( Exception::ERR_INVALIDPARAMS, "指定编号的顶点着色器不存在。" );

		return *( mVSList.Begin() + nIndex );
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取指定编号的像素着色器
	PixelShader* ShaderManager::GetPixelShader( UINT nIndex )
	{
		if( nIndex >= mPSList.Size() )
			Except( Exception::ERR_INVALIDPARAMS, "指定编号的像素着色器不存在。" );

		return *( mPSList.Begin() + nIndex );
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除指定的顶点着色器
	void ShaderManager::DeleteVertexShader( VertexShader* pVertexShader )
	{
		if( pVertexShader == NULL )
			return;

		VertexShaderList::Iterator it = mVSList.Begin();
		VertexShaderList::Iterator end = mVSList.End();
		for(; it!=end; ++it )
		{
			if( *it == pVertexShader )
			{
				mVSList.Erase( it );
				delete pVertexShader;
				return;
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除指定的像素着色器
	void ShaderManager::DeletePixelShader( PixelShader* pPixelShader )
	{
		if( pPixelShader == NULL )
			return;

		PixelShaderList::Iterator it = mPSList.Begin();
		PixelShaderList::Iterator end = mPSList.End();
		for(; it!=end; ++it )
		{
			if( *it == pPixelShader )
			{
				mPSList.Erase( it );
				delete pPixelShader;
				return;
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除所有的顶点着色器
	void ShaderManager::DeleteAllVertexShader()
	{
		VertexShaderList::Iterator it = mVSList.Begin();
		VertexShaderList::Iterator end = mVSList.End();
		for(; it!=end; ++it )
		{
			SafeDelete( *it );
		}

		mVSList.Clear();
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除所有的像素着色器
	void ShaderManager::DeleteAllPixelShader()
	{
		PixelShaderList::Iterator it = mPSList.Begin();
		PixelShaderList::Iterator end = mPSList.End();
		for(; it!=end; ++it )
		{
			SafeDelete( *it );
		}

		mPSList.Clear();
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
