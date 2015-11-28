/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	StreamMesh
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	顶点流网格类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//顶点流网格类
	class FK_DLL StreamMesh : public Geometry
	{
	public:
		UINT				mNumVertex;			//顶点数量
		UINT				mNumIndex;			//索引数量
		UINT				mNumIndexPerFrame;	//每帧索引数量

		UINT				mNumKey;			//帧数量
		UINT				mNumSegment;		//段数

		float				mIntervalTime;		//每帧的停顿时间

		UINT*				mpSegmentsIndex;	//每帧段索引数据
		StreamMeshSegment*	mpSegments;			//段信息

	public:
		StreamMesh();
		~StreamMesh();

		//几何体创建函数
		static Geometry* Creator();

		//分配段数据
		void AllocSegmentBuf();

		//释放段数据
		void FreeSegmentBuf();

	protected:
		//渲染物体
		void _RenderObj( const RenderParam* pParam, const Matrix4* pNodeMatrix );

		//渲染动画物体
		void _RenderAnimObj( const RenderParam* pParam, const AnimControl* pAnimControl, const Matrix4* pNodeMatrix );
	};
	//--------------------------------------------------------------------------------------------------------------
	typedef Stack< StreamMesh* >		StreamMeshList;
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
