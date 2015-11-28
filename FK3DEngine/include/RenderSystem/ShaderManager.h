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

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//着色器管理器
	class FK_DLL ShaderManager : public Singleton< ShaderManager >
	{
	protected:
		FileObject*			mpShaderPack;	// 着色器代码打包文件

		VertexShaderList	mVSList;		//顶点着色器列表
		PixelShaderList		mPSList;		//像素着色器列表

	public:
		ShaderManager();
		virtual ~ShaderManager();

		//关联着色器代码打包文件
		void SetProgramPackFile( FileObject* pPackFile );

		//从指定的文件中载入着色器
		void CreateShaderFromFile( LPCSTR szFileName );

		//创建顶点着色器
		virtual VertexShader* CreateVertexShader() = 0;

		//创建像素着色器
		virtual PixelShader* CreatePixelShader() = 0;

		//获取指定编号的顶点着色器
		VertexShader* GetVertexShader( UINT nIndex );

		//获取指定编号的像素着色器
		PixelShader* GetPixelShader( UINT nIndex );

		//删除指定的顶点着色器
		void DeleteVertexShader( VertexShader* pVertexShader );

		//删除指定的像素着色器
		void DeletePixelShader( PixelShader* pPixelShader );

		//删除所有的顶点着色器
		void DeleteAllVertexShader();

		//删除所有的像素着色器
		void DeleteAllPixelShader();
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
