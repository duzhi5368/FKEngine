/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	OverlayManager
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	平面图像管理器
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

#include "UVASprite.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//平面图像渲染数据结构
	struct OverlayRenderOp
	{
		UINT				Offset;		//渲染顶点偏移量
		UINT				NumPrim;	//渲染图元数量
		PrimitiveType		DrawType;	//渲染图元类型
		TextureFilterType	FilterType;	//纹理过滤类型
		Texture*			pTexture;	//渲染纹理指针
	};
	//--------------------------------------------------------------------------------------------------------------
	//平面图像管理器
	class FK_DLL OverlayManager : public Singleton< OverlayManager >
	{
	public:
		FileObject*			mpOverlayPack;	//平面图像打包文件指针
		ImageDecoder		mImgDecoder;	//图像解码器
		UINT				mNumRenderVer;	//每帧渲染的顶点数量

		UINT				mRenderWidth;	//平面图像渲染面宽度
		UINT				mRenderHeight;	//平面图像渲染面高度

		RenderState			mRenderState;	//平面图像渲染状态

	protected:
		OverlayList			mOverlayList;	//平面图像列表

		JointList			mJointList;		//拼接图像列表
		SpriteList			mSpriteList;	//精灵列表

		SpriteSetList		mSpriteSetList;	//精灵集列表
		UVASpriteList		mUVASprite;		//精灵 UV 动画处理器列表

		Matrix4				mWorldMatrix;	//平面图像渲染世界矩阵
		Matrix4				mProjMatrix;	//平面图像渲染投影矩阵
		Matrix4				mViewMatrix;	//平面图像渲染观察矩阵

		VertexBuffer*		mpVertexBuf;	//顶点缓存
		VertexDeclaration*	mpVertexDecl;	//顶点声明
		UINT				mMaxNumVer;		//顶点缓存最大顶点数量
		UINT				mOffsetVer;		//当前帧偏移顶点数

	public:
		OverlayManager();
		~OverlayManager();

		//设置面板打包文件
		void SetOverlayPack( FileObject* pFile );

		//初始化平面图像管理器
		void Initialize( UINT nMaxNumVer = 25000 );

		//更新顶点数据
		void _UpdateVertexData();

		//渲染所有平面图像
		void _RenderAllOverlays();

		//设置渲染面尺寸
		void SetRenderPlaneSize( UINT nWidth, UINT nHeight );

		//更新平面图像动画帧延时
		void _UpdateFrameTime( float fFrameTime );

		//创建空拼接图像
		JointSurface* CreateJointSurface( float fLeft, float fTop, float fWidth, float fHeight,
			ZOrderType eZType, OverlaySurface* pZRefOverlay, UINT nTexWidth, UINT nTexHeight, PixelFormat ePixelFormat,
			TextureFilterType eFilter = TFT_POINT, bool bDynamic = false, DWORD dwColor = 0xFFFFFFFF );

		//创建空精灵集
		SpriteSet* CreateSpriteSet( UINT nTexWidth, UINT nTexHeight, PixelFormat ePixelFormat,
			UINT InitSpriteNum, UINT AddSpriteNum, TextureFilterType eFilter = TFT_LINEAR );

		//从图像文件中载入拼接图像
		JointSurface* LoadJointSurfaceFromFile( float fLeft, float fTop, float fWidth, float fHeight,
			ZOrderType eZType, OverlaySurface* pZRefOverlay, const char* szName, PixelFormat ePixelFormat,
			TextureFilterType eFilter = TFT_POINT, bool bDynamic = false, DWORD dwColor = 0xFFFFFFFF );

		//从图像文件中载入精灵集
		SpriteSet* LoadSpriteSetFromFile( const char* szName, PixelFormat ePixelFormat,
			UINT InitSpriteNum, UINT AddSpriteNum, TextureFilterType eFilter = TFT_POINT );

		//删除拼接图像
		void DeleteJointSurface( JointSurface* pJointSurface );

		//删除精灵集
		void DeleteSpriteSet( SpriteSet* pSpriteSet );

		//删除所有拼接图像
		void DeleteAllJointSurfaces();

		//删除所有精灵集
		void DeleteAllSpriteSets();

		//获取最顶层平面图像指针
		OverlaySurface* FKFastcall GetTopmostOverlay();

		//将平面图像加入到管理列表中
		void FKFastcall _AddOverlayToList( OverlaySurface* pOverlay, ZOrderType eZType, OverlaySurface* pZRefOverlay );

		//将平面图像从管理列表中删除
		void FKFastcall _DelOverlayFromList( OverlaySurface* pOverlay );

		//设置平面图像 Z 顺序
		void FKFastcall _SetOverlayZOrder( OverlaySurface* pOverlay, ZOrderType eZType, OverlaySurface* pZRefOverlay );

		//将精灵 UV 动画处理器加入到管理列表中
		void FKFastcall _AddUVASpriteToList( UVASprite* pUVASprite );

		//将精灵 UV 动画处理器从管理列表中删除
		void FKFastcall _DelUVASpriteFromList( UVASprite* pUVASprite );
	};
	//--------------------------------------------------------------------------------------------------------------
};

//==================================================================================================================
