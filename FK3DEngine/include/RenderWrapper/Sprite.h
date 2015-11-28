/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Sprite
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	精灵类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	class SpriteSet;
	class UVASprite;
	struct UVAnimation;
	//--------------------------------------------------------------------------------------------------------------
	//精灵类
	class FK_DLL Sprite : public OverlaySurface
	{
	public:
		float				mfWidth;		//渲染宽度
		float				mfHeight;		//渲染高度

		SpriteSet*			mpSpriteSet;	//该精灵所从属的精灵集指针
		OverlayVertex		mpVertexData[4];//平面图像顶点数据（三角形列）

		UVASprite*			mpUVASprite;	//精灵 UV 动画处理器

	public:
		Sprite( SpriteSet* pSpriteSet, float fLeft, float fTop, float fWidth, float fHeight,
			ZOrderType eZType, OverlaySurface* pZRefOverlay, TextureFilterType eFilterType,
			UV4* pUV, DWORD color );
		Sprite( SpriteSet* pSpriteSet, float fLeft, float fTop, float fWidth, float fHeight,
			ZOrderType eZType, OverlaySurface* pZRefOverlay, TextureFilterType eFilterType,
			UVAnimation* pUVAnimation, DWORD color );
		~Sprite();

		//将该精灵从它的精灵集内删除
		void DeleteFromSpriteSet();

		//设置精灵 UV 坐标
		void FKFastcall SetSpriteUV( const UV4* pUV );

		//设置精灵位置
		void FKFastcall SetLocation( float fLeft, float fTop );

		//移动精灵位置
		void FKFastcall MoveLocation( float fX, float fY );

		//设置精灵尺寸
		void FKFastcall SetSize( float fWidth, float fHeight );

		//设置精灵颜色
		void FKFastcall SetColor( DWORD color );

	protected:
		//处理平面图像顶点
		void FKFastcall _ProcessVertex();

		//渲染平面图像
		void FKFastcall _RenderOverlay();
	};
	//--------------------------------------------------------------------------------------------------------------
	typedef Stack< Sprite* >	SpriteList;
	//--------------------------------------------------------------------------------------------------------------
};

//==================================================================================================================
