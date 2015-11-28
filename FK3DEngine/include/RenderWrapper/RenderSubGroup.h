/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	RenderSubGroup
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	子渲染组类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//渲染单元
	struct RenderUnit
	{
		RenderParam*		pRenderParam;		//渲染参数指针
		RenderInstance*		pRenderInstance;	//渲染实例指针
	};
	//--------------------------------------------------------------------------------------------------------------
	typedef Stack< RenderUnit >		RenderUnitStack;
	//--------------------------------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------------------------------
	//交换渲染单元
	FKInline void SwapRenderUnit( RenderUnit* pUnitA, RenderUnit* pUnitB );
	//--------------------------------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------------------------------
	//子渲染组类
	class FK_DLL RenderSubGroup
	{
	public:
		UINT				mnPriority;			//子渲染组优先级

		RenderUnit*			mpHeadRenderUnit;	//起始渲染单元
		RenderUnit*			mpRearRenderUnit;	//结尾渲染单元

		RenderUnit*			mpHeadSolidUnit;	//起始不透明渲染单元
		RenderUnit*			mpRearSolidUnit;	//结尾不透明渲染单元

		RenderUnit*			mpHeadTransUnit;	//起始透明渲染单元
		RenderUnit*			mpRearTransUnit;	//结尾透明渲染单元

	public:
		//排序子渲染组
		void SortRenderSubGroup( const Camera* pCamera );

	protected:
		//查找最前透明渲染单元
		FKInline RenderUnit* _FindFirstTransUnit( RenderUnit* pStartUnit );

		//查找最末不透明渲染单元
		FKInline RenderUnit* _FindLastSolidUnit( RenderUnit* pStartUnit );

		//排序不透明渲染单元
		FKInline void _SortSolidRenderUnit();

		//排序透明渲染单元
		FKInline void _SortTransRenderUnit( const Camera* pCamera );
	};
	//--------------------------------------------------------------------------------------------------------------
	typedef Stack< RenderSubGroup >	RenderSubGroupStack;
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
