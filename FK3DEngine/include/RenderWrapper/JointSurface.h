/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	JointSurface
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	拼接图像类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//拼接图像类（该类实现大型图像的多张纹理拼接显示方法）
	class FK_DLL JointSurface : public OverlaySurface
	{
	public:
		float				mfWidth;		//渲染宽度
		float				mfHeight;		//渲染高度

		UINT				mPicWidth;		//图像实际宽度
		UINT				mPicHeight;		//图像实际高度

		UINT				mNumTextures;	//用来拼接的纹理数量
		TextureList			mTextureList;	//用来拼接的纹理指针

		bool				mbDynamic;		//是否为需要动态更新的拼接图像
		OverlayVertex*		mpVertexData;	//拼接图像顶点数据

	protected:
		UINT				mNumCols;		//横向纹理数量
		UINT				mNumRows;		//纵向纹理数量
		UINT				mPerWidth;		//每张纹理的宽度
		UINT				mPerHeight;		//每张纹理的高度
		UINT				mRemWidth;		//最后一张纹理的使用宽度
		UINT				mRemHeight;		//最后一张纹理的使用高度
		UINT				mActWidth;		//最后一张纹理的实际宽度
		UINT				mActHeight;		//最后一张纹理的实际高度

	public:
		JointSurface( ZOrderType eZType, OverlaySurface* pZRefOverlay, TextureFilterType eFilterType );
		~JointSurface();

		//创建拼接图像
		void CreateJointSurface( float fLeft, float fTop, float fWidth, float fHeight, UINT nTexWidth, UINT nTexHeight,
			PixelFormat ePixelFormat, bool bDynamic = false, DWORD color = 0xFFFFFFFF );

		//从图像文件中载入拼接图像（纹理尺寸为图片尺寸）
		void LoadJointSurfaceFromFile( float fLeft, float fTop, float fWidth, float fHeight, FileObject* pFile,
			const char* szName, PixelFormat ePixelFormat, bool bDynamic = false, DWORD color = 0xFFFFFFFF );

		//设置拼接图像位置
		void FKFastcall SetLocation( float fLeft, float fTop );

		//设置拼接图像颜色
		void FKFastcall SetColor( DWORD color );

		//获取该拼接图像的纹理
		Texture* GetTexture( UINT nNumTexture = 0 );

	protected:
		//处理平面图像顶点
		void FKFastcall _ProcessVertex();

		//渲染平面图像
		void FKFastcall _RenderOverlay();
	};
	//--------------------------------------------------------------------------------------------------------------
	typedef Stack< JointSurface* >	JointList;
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
