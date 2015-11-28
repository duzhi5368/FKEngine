/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	ObjectFileFormat
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	物体文件格式
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

#include "../RenderSystem/TypeDefine.h"
#include "../RenderSystem/VertexDeclaration.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------

	const char FK_OBJ_FILE_HEADER[]					= "FK Object File";		//文件头字符串
	const char FK_OBJ_FILE_VERSION_STR[]			= "1.3.02";				//文件版本号字符串
	const UINT FK_OBJ_FILE_VERSION					= 1302;					//文件版本号
	const UINT FK_OBJ_FILE_MIN_COMPATIBLE_VERSION	= 1302;					//最低兼容版本号

	//--------------------------------------------------------------------------------------------------------------

	//各种文件段标识
	enum ObjectChunkID
	{
		//不可用文件段
		FK_CID_Invaild				= 0x0000,

		//结束段
		FK_CID_End					= 0x0001,

		//文件头段
		FK_CID_Header				= 0x1000,
			// char*			文件头字符串	应为 FK_OBJ_FILE_HEADER 
			// UINT				文件版本号		应为 FK_OBJ_FILE_VERSION

		//材质段
		FK_CID_Material				= 0x2000,
			// MaterialData		材质数据结构

		//三角面网格
		FK_CID_TriMesh				= 0x3100,
			// TriMeshInfo		三角面网格信息
			// UINT*			骨头索引数据 长度 = sizeof(UINT) * TriMeshInfo.nNumBone

		//顶点流网格
		FK_CID_StreamMesh			= 0x3200,
			// StreamMeshInfo		顶点流网格信息
			// UINT*				顶点流网格帧段索引 长度 = sizeof(UINT) * StreamMeshInfo.nNumFrames
			// StreamMeshSegment*	顶点流网格段信息 长度 = sizeof(StreamMeshSegment) * StreamMeshInfo.nNumSegment

			//包围盒段
			FK_CID_BoundBox				= 0x3001,
				// Vector3			包围盒最大点
				// Vector3			包围盒最小点

			//索引流数据段
			FK_CID_IndexStream			= 0x3002,
				// IndexStreamInfo	索引流信息
				// void*			索引数据 长度 = IndexStreamInfo.eIndexType * IndexStreamInfo.nNumIndex

			//顶点流数据段
			FK_CID_VertexStream			= 0x3003,
				// VertexStreamInfo	顶点流信息
				// void*			顶点数据 长度 = VertexStreamInfo.nVerSize * VertexStreamInfo.nNumVer

			//顶点元素段
			FK_CID_VertexElement		= 0x3004,
				// VertexElement	顶点元素描述

			//子网格段
			FK_CID_SubMesh				= 0x3005,
				// SubMeshInfo		子网格信息

		//骨骼
		FK_CID_Skeleton			= 0x4100,
			// SkeletonInfo		骨骼信息
			// Matrix4*			关键帧矩阵数据 长度 = sizeof(Matrix4) * SkeletonInfo.nNumKey * SkeletonInfo.nNumBone

		//公告板
		FK_CID_Billboard		= 0x5100,
			// BillboardInfo	公告板信息
			// Vector3*			公告板坐标数据 长度 = sizeof(Vector3) * BillboardInfo.nNumBillboard

		//碰撞网格
		FK_CID_BoundMesh		= 0x6100,
			// BoundInfo		碰撞体信息
			// BoundFace*		碰撞面数据 长度 = sizeof(BoundFace) * BoundInfo.nNumFace
			// Vector3*			碰撞面顶点数据 长度 = sizeof(Vector3) * BoundInfo.nNumVertex
	};

	//--------------------------------------------------------------------------------------------------------------

	//材质类型
	enum MaterialType
	{
		Opacity,		//完全不透明
		Transparent,	//半透明
		AlphaMask,		//透明遮罩
	};

	//材质数据
	struct MaterialData
	{
		ColorValue		Diffuse;				//漫射色
		ColorValue		Ambient;				//环境色
		ColorValue		Specular;				//高光色
		ColorValue		Emissive;				//放射色
		float			Power;					//高光能量值

		MaterialType	eType;					//材质类型
		bool			bHaveTexture;			//是否存在纹理

		char			szMaterialName[32];		//材质名称
		char			szTexFileName[32];		//纹理文件名称
	};
	typedef Stack< MaterialData >	MaterialDataStack;

	//--------------------------------------------------------------------------------------------------------------

	//三角面索引
	struct FaceIndex
	{
		union
		{
			struct
			{
				WORD v1;
				WORD v2;
				WORD v3;
			};
			WORD v[3];
		};
	};
	typedef Stack< FaceIndex >	FaceIndexStack;

	//--------------------------------------------------------------------------------------------------------------

	//双浮点值纹理 UV 坐标
	struct UV2
	{
		float u;
		float v;
	};
	typedef Stack< UV2 >		UV2Stack;

	//颜色数据
	typedef Stack< DWORD >		ColorStack;

	//--------------------------------------------------------------------------------------------------------------

	//三角面网格信息
	struct TriMeshInfo
	{
		UINT		nNumVertex;			//网格顶点数量
		UINT		nNumFace;			//网格三角面数量
		
		UINT		nNumVertexStream;	//顶点流数量
		UINT		nNumSubMesh;		//子网格数量

		UINT		nNumBone;			//关联骨头数量
		UINT		nNumBonePerVer;		//每顶点混合骨头数量
		bool		bHaveNormal;		//是否存在法线流
	};

	//顶点流网格段信息
	struct StreamMeshSegment
	{
		UINT		nVertexOffset;		//段顶点偏移量
		UINT		nVertexNum;			//段顶点数量

		UINT		nNumFrames;			//段包含帧数
	};
	typedef Stack< StreamMeshSegment >	StreamMeshSegmentStack;

	//顶点流网格信息
	struct StreamMeshInfo
	{
		UINT		nNumVertex;			//网格顶点数量
		UINT		nNumFace;			//网格三角面数量

		UINT		nNumVertexStream;	//顶点流数量
		UINT		nNumSubMesh;		//子网格数量

		UINT		nNumSegment;		//段数

		UINT		nNumFacePerFrame;	//三角面数量
		UINT		nNumFrames;			//动画帧数

		float		fIntervalTime;		//每帧的停顿时间（秒）
	};

	//索引流信息
	struct IndexStreamInfo
	{
		IndexType	eIndexType;			//索引流类型
		UINT		nNumIndex;			//索引数量
		bool		bDynamic;			//是否为动态索引流
	};

	//顶点流信息
	struct VertexStreamInfo
	{
		UINT		nVerSize;			//每顶点长度
		UINT		nNumVer;			//顶点数量
		bool		bDynamic;			//是否为动态顶点流
	};

	//子网格信息
	struct SubMeshInfo
	{
		UINT		nIndexOffset;		//索引数据偏移量
		UINT		nVerCount;			//绘制顶点数量
		UINT		nPrimCount;			//绘制图元数量
		int			nMaterialIndex;		//使用材质索引
	};
	typedef Stack< SubMeshInfo >	SubMeshInfoStack;

	//--------------------------------------------------------------------------------------------------------------

	//骨骼信息头
	struct SkeletonInfo
	{
		UINT	nNumBone;			//骨头数量
		UINT	nNumKey;			//帧数量
		float	fIntervalTime;		//每帧的停顿时间（秒）
	};

	//--------------------------------------------------------------------------------------------------------------

	//公告板信息头
	struct BillboardInfo
	{
		UINT	nNumBillboard;		//公告板数量
		float	fSizeX;				//公告板 X 尺寸
		float	fSizeY;				//公告板 Y 尺寸
		int		nMaterialIndex;		//公告板材质索引值
		char	szUVAFileName[32];	//公告板 UV 动画文件名
	};

	//--------------------------------------------------------------------------------------------------------------

	//碰撞三角面数据
	struct BoundFace
	{
		Vector3	vNormal;		//面法线
		UINT	nV1Index;		//顶点 1 索引值
		UINT	nV2Index;		//顶点 2 索引值
		UINT	nV3Index;		//顶点 3 索引值
	};
	typedef Stack< BoundFace >	BoundFaceStack;

	//碰撞体数据头
	struct BoundInfo
	{
		//碰撞体类型
		enum Type
		{
			General,	//普通碰撞面
			Stair,		//楼梯碰撞面
			Ladder,		//阶梯碰撞面
		};

		Type		eType;			//碰撞体类型
		UINT		nNumFace;		//碰撞三角面数
		UINT		nNumVertex;		//碰撞三角面顶点数
		Vector3		vBoundBoxMax;	//碰撞体包围盒最大点
		Vector3		vBoundBoxMin;	//碰撞体包围盒最小点
	};

	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================