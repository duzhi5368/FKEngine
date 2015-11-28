/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	SpriteEmitter
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	精灵粒子发射器
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//精灵粒子发射器
	class FK_DLL SpriteEmitter : public Emitter
	{
	public:
		friend class ParticleSystem;

		//精灵粒子顶点数据
		struct Vertex
		{
			float		x, y, z, w;		//顶点坐标
			DWORD		dwColor;		//顶点颜色
			float		fU;				//纹理坐标 U
			float		fV;				//纹理坐标 V
		};
		typedef	Stack< Vertex >		VertexStack;

	protected:
		Vertex*				mpVertexData;		//精灵粒子顶点数据
		WORD*				mpIndexData;		//精灵粒子索引数据
		UINT				mNumRenderParticle;	//渲染精灵粒子数量

	public:
		SpriteEmitter( Particle* pParticle );
		virtual ~SpriteEmitter() = 0;

	protected:
		//更新精灵粒子方向
		virtual UINT _UpdateSprite( const Camera* pCamera ) = 0;

		//根据精灵粒子屏幕空间深度重新排序顶点数据
		void _ReorderVertexData();

		//更新精灵粒子发射器顶点数据
		void _UpdateVertexData( Vertex* pVerBuf, WORD* pIndexBuf, UINT nVertexOffset, UINT nIndexOffset );
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
