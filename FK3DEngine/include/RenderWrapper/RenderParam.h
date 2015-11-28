/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	RenderParam
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	渲染参数类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	class Material;
	class RenderObject;
	class AnimControl;
	//--------------------------------------------------------------------------------------------------------------
	//渲染参数类
	class FK_DLL RenderParam
	{
	public:
		//绘制类型
		enum DrawType
		{
			DT_Basic,	//基本图元
			DT_TPatch,	//三角形插值面片
			DT_RPatch,	//矩形插值面片
		};

		//基本图元绘制参数
		struct BasicParam
		{
			PrimitiveType	ePrimType;		//绘制图元类型
			DWORD			nVerOffset;		//顶点数据偏移量
			DWORD			nMinIndex;		//使用最小索引值
			DWORD			nIndexOffset;	//索引数据偏移量
			DWORD			nVerCount;		//绘制顶点数量
			DWORD			nPrimCount;		//绘制图元数量
		};

		//三角形插值面片绘制参数
		struct TPatchParam
		{
			DWORD			nPrimCount;		//绘制图元数量
		};

		//矩形插值面片绘制参数
		struct RPatchParam
		{
			DWORD			nPrimCount;		//绘制图元数量
		};

	public:
		DrawType			eDrawType;		//绘制方式
		Material*			pMaterial;		//渲染材质
		UINT				nPriority;		//子渲染组优先级

		union
		{
			BasicParam	sBasicParam;		//基本图元绘制参数
			TPatchParam	sTPatchParam;		//三角形插值面片绘制参数
			RPatchParam	sRPatchParam;		//矩形插值面片绘制参数
		};

	public:
		//根据参数进行渲染
		void FKFastcall RenderByParam() const;

		//是否需要进行进行渲染
		bool FKFastcall NeedRender() const;
	};
	//--------------------------------------------------------------------------------------------------------------
	typedef Stack< RenderParam* >	RenderParamList;
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
