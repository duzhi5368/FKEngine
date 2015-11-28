/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	SkyBox
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	天空盒
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//天空盒纹理数据
	struct SkyBoxTexture
	{
		Texture*	pTopTexture;		//顶面纹理
		Texture*	pFrontTexture;		//前面纹理
		Texture*	pBackTexture;		//后面纹理
		Texture*	pLeftTexture;		//左面纹理
		Texture*	pRightTexture;		//右面纹理
	};
	//--------------------------------------------------------------------------------------------------------------
	//天空盒
	class FK_DLL SkyBox : public SkyEntity
	{
	public:
		Texture*			mpTopTexture;		//顶面纹理
		Texture*			mpFrontTexture;		//前面纹理
		Texture*			mpBackTexture;		//后面纹理
		Texture*			mpLeftTexture;		//左面纹理
		Texture*			mpRightTexture;		//右面纹理

	protected:
		VertexDeclaration*	mpVD;				//顶点声明
		VertexBuffer*		mpPositionVB;		//顶点坐标缓存
		VertexBuffer*		mpTexcoordVB;		//纹理坐标缓存

	public:
		SkyBox();
		~SkyBox();

		//设置天空盒纹理
		void SetSkyBoxTexture( SkyBoxTexture* pSkyBoxTexture );

		//更新天空体
		void _UpdateSky( float fTimeSinceLastFrame );

		//渲染天空体
		void _RenderSky();
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
