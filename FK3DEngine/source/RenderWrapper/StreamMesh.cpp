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
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../../include/RenderWrapper/RenderWrapperInsideHeader.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	StreamMesh::StreamMesh()
		: mNumSegment		(0)
		, mNumVertex		(0)
		, mNumIndex			(0)
		, mNumIndexPerFrame	(0)
		, mNumKey			(0)
		, mIntervalTime		(0.0f)
		, mpSegmentsIndex	(NULL)
		, mpSegments		(NULL)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	StreamMesh::~StreamMesh()
	{
		FreeSegmentBuf();
	}
	//--------------------------------------------------------------------------------------------------------------
	//几何体创建函数
	Geometry* StreamMesh::Creator()
	{
		return new StreamMesh;
	}
	//--------------------------------------------------------------------------------------------------------------
	//分配段数据
	void StreamMesh::AllocSegmentBuf()
	{
		mpSegmentsIndex = (UINT*)malloc( sizeof(UINT) * mNumKey );
		mpSegments = (StreamMeshSegment*)malloc( sizeof(StreamMeshSegment) * mNumSegment );
	}
	//--------------------------------------------------------------------------------------------------------------
	//释放段数据
	void StreamMesh::FreeSegmentBuf()
	{
		SafeFree( mpSegmentsIndex );
		SafeFree( mpSegments );
	}
	//--------------------------------------------------------------------------------------------------------------
	//渲染物体
	void StreamMesh::_RenderObj( const RenderParam* pParam, const Matrix4* pNodeMatrix )
	{
		//设置世界矩阵
		RenderSystem::mpSingleton->_SetWorldMatrix( pNodeMatrix );

		//根据渲染参数渲染
		pParam->RenderByParam();
	}
	//--------------------------------------------------------------------------------------------------------------
	//渲染动画物体
	void StreamMesh::_RenderAnimObj( const RenderParam* pParam, const AnimControl* pAnimControl, const Matrix4* pNodeMatrix )
	{
		//计算渲染顶点偏移位置
		static RenderParam sParam;
		memcpy( &sParam, pParam, sizeof(RenderParam) );

		StreamMeshSegment* pSegment = mpSegments + mpSegmentsIndex[ pAnimControl->mCurKey ];
		sParam.sBasicParam.nVerOffset = pSegment->nVertexOffset;
		sParam.sBasicParam.nIndexOffset = pAnimControl->mCurKey * mNumIndexPerFrame;
		sParam.sBasicParam.nVerCount = pSegment->nVertexNum;

		//设置世界矩阵
		RenderSystem::mpSingleton->_SetWorldMatrix( pNodeMatrix );

		//根据渲染参数渲染
		sParam.RenderByParam();
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
