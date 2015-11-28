/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	OverlaySurface
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	平面图像类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------

	// Z 顺序设置类型
	enum ZOrderType
	{
		ZOT_Top,		//最上层（最后渲染）
		ZOT_Bottom,		//最下层（最先渲染）
		ZOT_Upon,		//指定平面图像之上（后渲染）
		ZOT_Below,		//指定平面图像之下（先渲染）
	};

	//平面图像顶点结构
	struct OverlayVertex
	{
		float	x, y, z;	//坐标
		DWORD	color;		//颜色
		float	u, v;		//纹理坐标
	};

	//--------------------------------------------------------------------------------------------------------------
	//平面图像类（该类实现大型图像的多张纹理拼接显示方法）
	class FK_DLL OverlaySurface
	{
	public:
		friend class OverlayManager;

	public:
		bool				mbVisable;		//是否可见
		TextureFilterType	mFilterType;	//纹理过滤类型

	protected:
		OverlayVertex*		mpVertexPtr;	//顶点数据指针
		UINT				mNumVertex;		//顶点数量
		UINT				mVertexOffset;	//渲染顶点偏移量

	public:
		OverlaySurface( ZOrderType eZType, OverlaySurface* pZRefOverlay, TextureFilterType eFilterType );
		virtual ~OverlaySurface();

		//设置平面图像可见状态
		void FKFastcall SetVisable( bool bVisable );

		//设置平面图像 Z 顺序
		void FKFastcall SetZOrder( ZOrderType eZType, OverlaySurface* pZRefOverlay = NULL );

		//设置平面图像纹理过滤类型
		void FKFastcall SetFilterType( TextureFilterType eFilterType );

	protected:
		//更新平面图像顶点
		UINT FKFastcall _UpdateVertex( OverlayVertex* pVertexBuf, UINT nOffset );

		//处理平面图像顶点
		virtual void FKFastcall _ProcessVertex() = 0;

		//渲染平面图像
		virtual void FKFastcall _RenderOverlay() = 0;
	};
	//--------------------------------------------------------------------------------------------------------------
	typedef Stack< OverlaySurface* >	OverlayList;
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
