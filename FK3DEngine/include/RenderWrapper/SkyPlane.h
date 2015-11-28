/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	SkyPlane
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	天空面
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//天空面
	class FK_DLL SkyPlane : public SkyEntity
	{
	protected:
		Texture*			mpBackgroundTexture;	//背景层纹理
		Texture*			mpSurfaceTexture;		//表层纹理

		VertexDeclaration*	mpVD;					//顶点声明
		VertexBuffer*		mpPositionVB;			//顶点坐标缓存
		VertexBuffer*		mpTexcoordVB;			//纹理坐标缓存

		float				mCloudUFactor;			//表层纹理 U 坐标变化系数
		float				mCloudVFactor;			//表层纹理 V 坐标变化系数
		float				mCloudU;				//表层纹理 U 值
		float				mCloudV;				//表层纹理 V 值

		float				mpUV[16];				//顶点纹理坐标

	public:
		SkyPlane();
		virtual ~SkyPlane();

		//设置天空纹理
		void SetSkyTexture( Texture* pBackground, Texture* pSurface );

		//设置移动方向
		void SetMoveDirection( float fMoveFront, float fMoveRight );

		//更新天空体
		void _UpdateSky( float fTimeSinceLastFrame );

		//渲染天空体
		void _RenderSky();
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
