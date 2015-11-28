/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	ObjectFileLoader
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
	ObjectFileLoader::ObjectFileLoader()
		: mpObjectPack		(NULL)
	{
		mLoadedMaterialList.Initialize( 10, 10 );
		mLoadedTriMeshList.Initialize( 20, 100 );
		mLoadedStreamMeshList.Initialize( 20, 100 );
		mLoadedSkeletonList.Initialize( 10, 10 );
		mLoadedParticleList.Initialize( 10, 10 );
		mLoadedEmitterList.Initialize( 10, 10 );
		mLoadedBoundMeshList.Initialize( 20, 50 );
	}
	//--------------------------------------------------------------------------------------------------------------
	ObjectFileLoader::~ObjectFileLoader()
	{
		if( mpObjectPack != NULL )
		{
			mpObjectPack->Close();
			mpObjectPack = NULL;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置物体打包文件
	void ObjectFileLoader::SetObjectPackFile( FileObject* pPackFile )
	{
		mpObjectPack = pPackFile;
	}
	//--------------------------------------------------------------------------------------------------------------
	//载入物体文件
	void ObjectFileLoader::LoadObjectFile( const char* szFileName )
	{
		//清理上次的加载列表
		mLoadedMaterialList.Clear();
		mLoadedTriMeshList.Clear();
		mLoadedStreamMeshList.Clear();
		mLoadedSkeletonList.Clear();
		mLoadedParticleList.Clear();
		mLoadedEmitterList.Clear();
		mLoadedBoundMeshList.Clear();

		FKTry
		{
			//打开文件
			mpObjectPack->Open( szFileName, OT_Read );

			//获取文件长度
			mFileLength = mpObjectPack->GetLength();

			//读取文件头
			ObjectChunkID eChunkID = FK_CID_Invaild;

			if( !_ReadChunkID( eChunkID ) )
				Except( Exception::ERR_CANNOT_READ_FILE, "无法读取文件头段。" );

			if( eChunkID != FK_CID_Header )
				Except( Exception::ERR_INTERNAL_ERROR, "非法的文件头段标识。" );

			_ReadChunkHeader();

			//循环读取所有剩余文件段
			while( _ReadChunkID( eChunkID ) )
			{
				switch (eChunkID)
				{
				case FK_CID_Material:		_ReadChunkMaterial();		break;
				case FK_CID_TriMesh:		_ReadChunkTriMesh();		break;
				case FK_CID_StreamMesh:		_ReadChunkStreamMesh();		break;
				case FK_CID_Skeleton:		_ReadChunkSkeleton();		break;
				case FK_CID_Billboard:		_ReadChunkBillboard();		break;
				case FK_CID_BoundMesh:		_ReadChunkBoundMesh();		break;
				case FK_CID_End:			goto ChunkEnd;
				default:
					Except( Exception::ERR_INVALIDPARAMS, "无法识别的文件段标识。" );
				}
			}

ChunkEnd:
			//关闭文件
			mpObjectPack->Close();
		}
		FKCatch
		{
			mpObjectPack->Close();

			String sOrgDesc = sException.mDesc;
			sException.mDesc.Format( "载入自由骑士3D游戏引擎物体文件 '%s' 失败。\n失败原因： %s",
				szFileName, sOrgDesc.GetCharPtr() );

			ExceptToUpLevel;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//读取文件段标识
	bool ObjectFileLoader::_ReadChunkID( ObjectChunkID& eChunkID )
	{
		//如果到达文件结尾则返回
		DWORD dwCurPos = mpObjectPack->GetPosition();
		if( dwCurPos == mFileLength )
			return false;

		//读取文件段标识
		mpObjectPack->Read( &eChunkID, sizeof(ObjectChunkID) );

		return true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//读取文件头段
	void ObjectFileLoader::_ReadChunkHeader()
	{
		//读取文件头字符串
		static char pFileHeader [sizeof(FK_OBJ_FILE_HEADER)] = { NULL };
		mpObjectPack->Read( pFileHeader, sizeof(FK_OBJ_FILE_HEADER) );
		if( 0 != memcmp( pFileHeader, FK_OBJ_FILE_HEADER, sizeof(FK_OBJ_FILE_HEADER) ) )
			Except( Exception::ERR_INTERNAL_ERROR, "非法的文件头字符串。" );

		//读取文件版本号
		UINT nVersion = 0;
		mpObjectPack->Read( &nVersion, sizeof(UINT) );

		//如果文件版本号比当前支持版本号要高
		if( nVersion > FK_OBJ_FILE_VERSION )
			Except( Exception::ERR_INTERNAL_ERROR, "文件版本号高于当前支持版本号。" );

		//如果文件版本号低于最低兼容版本号
		if( nVersion < FK_OBJ_FILE_MIN_COMPATIBLE_VERSION )
			Except( Exception::ERR_INTERNAL_ERROR, "文件版本号低于最低兼容版本号。" );
	}
	//--------------------------------------------------------------------------------------------------------------
	//读取材质段
	void ObjectFileLoader::_ReadChunkMaterial()
	{
		//读取材质数据
		static MaterialData sMaterialData;
		mpObjectPack->Read( &sMaterialData, sizeof(MaterialData) );

		//创建材质
		*mLoadedMaterialList.Push() = MaterialManager::mpSingleton->CreateMaterial( &sMaterialData );
	}
	//--------------------------------------------------------------------------------------------------------------
	//读取三角面网格段
	void ObjectFileLoader::_ReadChunkTriMesh()
	{
		//创建网格
		TriMesh* pTriMesh = (TriMesh*)GeometryManager::mpSingleton->CreateGeometry( TriMesh::Creator );


		//读取三角面网格信息
		static TriMeshInfo sMeshInfo;

		mpObjectPack->Read( &sMeshInfo, sizeof(TriMeshInfo) );

		if( sMeshInfo.nNumBone > 0 )
		{
			//读取骨头索引数据
			size_t nDataLen = sizeof(UINT) * sMeshInfo.nNumBone;
			UINT* pBoneIndexBuf = (UINT*)malloc( nDataLen );
			mpObjectPack->Read( pBoneIndexBuf, (DWORD)nDataLen );

			pTriMesh->SetBoneIndex( pBoneIndexBuf, sMeshInfo.nNumBone );

			free( pBoneIndexBuf );

			//判断是否需要使用软件顶点处理
			pTriMesh->mNumBlendMatrix	= sMeshInfo.nNumBonePerVer;
			pTriMesh->mMaxBlendIndex	= sMeshInfo.nNumBone - 1;

			DWORD dwHardMaxBlendIndex = sMeshInfo.bHaveNormal ?
				( RenderSystem::mdwMaxBlendIndex / 2 ) : RenderSystem::mdwMaxBlendIndex;

			pTriMesh->mbSoftVertexProc = ( dwHardMaxBlendIndex < pTriMesh->mMaxBlendIndex
				|| RenderSystem::mdwMaxBlendNum < pTriMesh->mNumBlendMatrix );
		}

		//设置网格顶点流数量
		pTriMesh->AllocStreamBuf( sMeshInfo.nNumVertexStream );
		pTriMesh->AllocMaterialBuf( sMeshInfo.nNumSubMesh );
		pTriMesh->AllocRenderParamBuf( sMeshInfo.nNumSubMesh );

		UINT nVertexStreamIndex = 0;
		UINT nSubMeshIndex = 0;


		//创建顶点声明
		pTriMesh->mpVertexDeclaration = BufferManager::mpSingleton->AddVertexDeclaration();

		//循环读取所有三角面网格段
		ObjectChunkID eChunkID = FK_CID_Invaild;
		while( _ReadChunkID( eChunkID ) )
		{
			switch (eChunkID)
			{
			case FK_CID_BoundBox:
				{
					_ReadChunkBoundBox( pTriMesh );
					break;
				}
			case FK_CID_IndexStream:
				{
					_ReadChunkIndexStream( pTriMesh, pTriMesh->mbSoftVertexProc );
					break;
				}
			case FK_CID_VertexStream:
				{
					_ReadChunkVertexStream( pTriMesh, nVertexStreamIndex, pTriMesh->mbSoftVertexProc );
					++nVertexStreamIndex;
					break;
				}
			case FK_CID_VertexElement:
				{
					_ReadChunkVertexElement( pTriMesh );
					break;
				}
			case FK_CID_SubMesh:
				{
					_ReadChunkSubMesh( pTriMesh, nSubMeshIndex );
					++nSubMeshIndex;
					break;
				}
			case FK_CID_End:
				{
					goto ChunkEnd;
				}
			default:
				Except( Exception::ERR_INVALIDPARAMS, "无法识别的三角面网格段标识。" );
			}
		}

ChunkEnd:
		//加载顶点声明
		pTriMesh->mpVertexDeclaration->CreateVertexDeclaration();

		*mLoadedTriMeshList.Push() = pTriMesh;
	}
	//--------------------------------------------------------------------------------------------------------------
	//读取顶点流网格段
	void ObjectFileLoader::_ReadChunkStreamMesh()
	{
		//创建网格
		StreamMesh* pStreamMesh = (StreamMesh*)GeometryManager::mpSingleton->CreateGeometry( StreamMesh::Creator );


		//读取顶点流网格信息
		static StreamMeshInfo sMeshInfo;

		mpObjectPack->Read( &sMeshInfo, sizeof(StreamMeshInfo) );

		pStreamMesh->mNumSegment		= sMeshInfo.nNumSegment;
		pStreamMesh->mNumVertex			= sMeshInfo.nNumVertex;
		pStreamMesh->mNumIndex			= sMeshInfo.nNumFace * 3;
		pStreamMesh->mNumIndexPerFrame	= sMeshInfo.nNumFacePerFrame * 3;
		pStreamMesh->mNumKey			= sMeshInfo.nNumFrames;
		pStreamMesh->mIntervalTime		= sMeshInfo.fIntervalTime;

		//设置网格顶点流数量
		pStreamMesh->AllocStreamBuf( sMeshInfo.nNumVertexStream );
		pStreamMesh->AllocMaterialBuf( sMeshInfo.nNumSubMesh );
		pStreamMesh->AllocRenderParamBuf( sMeshInfo.nNumSubMesh );

		UINT nVertexStreamIndex = 0;
		UINT nSubMeshIndex = 0;


		//读取顶点流网格段数据
		pStreamMesh->AllocSegmentBuf();

		mpObjectPack->Read( pStreamMesh->mpSegmentsIndex, sizeof(UINT) * pStreamMesh->mNumKey );
		mpObjectPack->Read( pStreamMesh->mpSegments, sizeof(StreamMeshSegment) * pStreamMesh->mNumSegment );


		//创建顶点声明
		pStreamMesh->mpVertexDeclaration = BufferManager::mpSingleton->AddVertexDeclaration();

		//循环读取所有三角面网格段
		ObjectChunkID eChunkID = FK_CID_Invaild;
		while( _ReadChunkID( eChunkID ) )
		{
			switch (eChunkID)
			{
			case FK_CID_BoundBox:
				{
					_ReadChunkBoundBox( pStreamMesh );
					break;
				}
			case FK_CID_IndexStream:
				{
					_ReadChunkIndexStream( pStreamMesh, pStreamMesh->mbSoftVertexProc );
					break;
				}
			case FK_CID_VertexStream:
				{
					_ReadChunkVertexStream( pStreamMesh, nVertexStreamIndex, pStreamMesh->mbSoftVertexProc );
					++nVertexStreamIndex;
					break;
				}
			case FK_CID_VertexElement:
				{
					_ReadChunkVertexElement( pStreamMesh );
					break;
				}
			case FK_CID_SubMesh:
				{
					_ReadChunkSubMesh( pStreamMesh, nSubMeshIndex );
					++nSubMeshIndex;
					break;
				}
			case FK_CID_End:
				{
					goto ChunkEnd;
				}
			default:
				Except( Exception::ERR_INVALIDPARAMS, "无法识别的顶点流网格段标识。" );
			}
		}

ChunkEnd:
		//加载顶点声明
		pStreamMesh->mpVertexDeclaration->CreateVertexDeclaration();

		*mLoadedStreamMeshList.Push() = pStreamMesh;
	}
	//--------------------------------------------------------------------------------------------------------------
	//读取包围盒段
	void ObjectFileLoader::_ReadChunkBoundBox( RenderObject* pRenderObject )
	{
		//读取包围盒最大点和最小点
		static Vector3 pBoundVer[2];
		mpObjectPack->Read( pBoundVer, sizeof(Vector3) * 2 );

		//设置包围盒
		pRenderObject->mBoundBox.SetMaxMin( pBoundVer[0], pBoundVer[1] );
	}
	//--------------------------------------------------------------------------------------------------------------
	//读取索引流数据段
	void ObjectFileLoader::_ReadChunkIndexStream( RenderObject* pRenderObject, bool bSoftProc )
	{
		//读取索引流信息
		static IndexStreamInfo sIndexStreamInfo;
		mpObjectPack->Read( &sIndexStreamInfo, sizeof(IndexStreamInfo) );

		//创建索引流
		IndexBuffer* pIndexBuffer = BufferManager::mpSingleton->AddIndexBuffer();
		pIndexBuffer->CreateIndexBuffer(
			sIndexStreamInfo.nNumIndex,
			sIndexStreamInfo.eIndexType,
			sIndexStreamInfo.bDynamic,
			bSoftProc );

		//加载索引数据
		void* pDstBuf = pIndexBuffer->Lock( 0, 0 );
		mpObjectPack->Read( pDstBuf, pIndexBuffer->mnBufLen );
		pIndexBuffer->Unlock();

		pRenderObject->mpIndexBuffer = pIndexBuffer;
	}
	//--------------------------------------------------------------------------------------------------------------
	//读取顶点流数据段
	void ObjectFileLoader::_ReadChunkVertexStream( RenderObject* pRenderObject, UINT nIndex, bool bSoftProc )
	{
		//读取顶点流信息
		static VertexStreamInfo sVertexStreamInfo;
		mpObjectPack->Read( &sVertexStreamInfo, sizeof(VertexStreamInfo) );

		//创建顶点流
		VertexBuffer* pVertexBuffer = BufferManager::mpSingleton->AddVertexBuffer();
		pVertexBuffer->CreateVertexBuffer(
			sVertexStreamInfo.nVerSize,
			sVertexStreamInfo.nNumVer,
			sVertexStreamInfo.bDynamic,
			bSoftProc );

		//加载顶点流数据
		void* pDstBuf = pVertexBuffer->Lock( 0, 0 );
		mpObjectPack->Read( pDstBuf, pVertexBuffer->mnBufLen );
		pVertexBuffer->Unlock();

		pRenderObject->mppVertexBuffer[nIndex] = pVertexBuffer;
	}
	//--------------------------------------------------------------------------------------------------------------
	//顶点声明段
	void ObjectFileLoader::_ReadChunkVertexElement( RenderObject* pRenderObject )
	{
		//读取顶点元素描述
		static VertexElement sVertexElement;
		mpObjectPack->Read( &sVertexElement, sizeof(VertexElement) );

		//加入顶点元素描述
		pRenderObject->mpVertexDeclaration->AddElement(
			sVertexElement.nStream,
			sVertexElement.nOffset,
			sVertexElement.Type,
			sVertexElement.Method,
			sVertexElement.Usage,
			sVertexElement.UsageIndex );
	}
	//--------------------------------------------------------------------------------------------------------------
	//读取子网格段
	void ObjectFileLoader::_ReadChunkSubMesh( RenderObject* pRenderObject, UINT nIndex )
	{
		//读取子网格信息
		static SubMeshInfo sSubMeshInfo;
		mpObjectPack->Read( &sSubMeshInfo, sizeof(SubMeshInfo) );

		//根据子网格信息设置渲染参数
		UINT nMaterialIndex = (UINT)sSubMeshInfo.nMaterialIndex;
		if( nMaterialIndex >= mLoadedMaterialList.Size() )
			Except( Exception::ERR_INVALIDPARAMS, "子网格指定使用的材质索引值错误。" );

		Material* pMaterial = mLoadedMaterialList[ nMaterialIndex ];
		pMaterial->AddRef();

		pRenderObject->mppMaterial[nIndex] = pMaterial;

		RenderParam* pRenderParam = pRenderObject->mpRenderParam + nIndex;

		pRenderParam->pMaterial	= pMaterial;
		pRenderParam->eDrawType	= RenderParam::DT_Basic;
		pRenderParam->nPriority	= RSGP_Default;

		pRenderParam->sBasicParam.ePrimType		= PT_TRIANGLE_LIST;
		pRenderParam->sBasicParam.nVerOffset	= 0;
		pRenderParam->sBasicParam.nMinIndex		= 0;
		pRenderParam->sBasicParam.nIndexOffset	= sSubMeshInfo.nIndexOffset;
		pRenderParam->sBasicParam.nVerCount		= sSubMeshInfo.nVerCount;
		pRenderParam->sBasicParam.nPrimCount	= sSubMeshInfo.nPrimCount;
	}
	//--------------------------------------------------------------------------------------------------------------
	//读取骨骼段
	void ObjectFileLoader::_ReadChunkSkeleton()
	{
		//读取骨骼信息
		static SkeletonInfo sSkeletonInfo;

		mpObjectPack->Read( &sSkeletonInfo, sizeof(SkeletonInfo) );

		//创建骨骼
		Skeleton* pSkeleton = SkeletonManager::mpSingleton->CreateSkeleton(
			sSkeletonInfo.nNumKey, sSkeletonInfo.fIntervalTime );

		//创建所有骨头
		DWORD dwSizePerBone = sizeof(Matrix4) * sSkeletonInfo.nNumKey;
		Matrix4* pMatrixBuf = (Matrix4*)malloc( dwSizePerBone );

		for( UINT i=0; i<sSkeletonInfo.nNumBone; ++i )
		{
			//读取骨头矩阵数据
			mpObjectPack->Read( pMatrixBuf, dwSizePerBone );
			pSkeleton->CreateBone( pMatrixBuf );
		}

		free( pMatrixBuf );

		*mLoadedSkeletonList.Push() = pSkeleton;
	}
	//--------------------------------------------------------------------------------------------------------------
	//读取公告板段
	void ObjectFileLoader::_ReadChunkBillboard()
	{
		//读取公告板信息
		static BillboardInfo sBillboardInfo;
		mpObjectPack->Read( &sBillboardInfo, sizeof(BillboardInfo) );

		//获取公告板材质
		if( (UINT)sBillboardInfo.nMaterialIndex >= mLoadedMaterialList.Size() )
			Except( Exception::ERR_INVALIDPARAMS, "公告板使用材质索引值错误。" );

		Material* pMaterial = mLoadedMaterialList[ sBillboardInfo.nMaterialIndex ];

		//读取公告板坐标数据
		DWORD dwPosSize = sizeof(Vector3) * sBillboardInfo.nNumBillboard;
		Vector3* pPositionBuf = (Vector3*)malloc( dwPosSize );
		mpObjectPack->Read( pPositionBuf, dwPosSize );


		Particle* pParticle = NULL;
		Emitter* pEmitter = NULL;

		//如果为静态公告板
		if( sBillboardInfo.szUVAFileName[0] == '\0' )
		{
			BillboardParticle::Param sParamP;

			sParamP.pMaterial	= pMaterial;
			sParamP.fSizeX		= sBillboardInfo.fSizeX;
			sParamP.fSizeY		= sBillboardInfo.fSizeY;

			pParticle = ParticleSystem::mpSingleton->CreateParticle( BillboardParticle::Creator, &sParamP );

			BillboardEmitter::Param sParamE;

			sParamE.pParticle		= pParticle;
			sParamE.nNumBillboard	= sBillboardInfo.nNumBillboard;
			sParamE.pPosition		= pPositionBuf;

			pEmitter = ParticleSystem::mpSingleton->CreateEmitter( BillboardEmitter::Creator, &sParamE );
		}
		//如果为 UV 动画公告板
		else
		{
			UVABillboardParticle::Param sParamP;

			sParamP.pMaterial		= pMaterial;
			sParamP.pUVAFileName	= sBillboardInfo.szUVAFileName;
			sParamP.fSizeX			= sBillboardInfo.fSizeX;
			sParamP.fSizeY			= sBillboardInfo.fSizeY;

			pParticle = ParticleSystem::mpSingleton->CreateParticle( UVABillboardParticle::Creator, &sParamP );

			UVABillboardEmitter::Param sParamE;

			sParamE.pParticle		= pParticle;
			sParamE.nNumBillboard	= sBillboardInfo.nNumBillboard;
			sParamE.pPosition		= pPositionBuf;

			pEmitter = ParticleSystem::mpSingleton->CreateEmitter( UVABillboardEmitter::Creator, &sParamE );
		}

		free( pPositionBuf );

		*mLoadedParticleList.Push() = pParticle;
		*mLoadedEmitterList.Push() = pEmitter;
	}
	//--------------------------------------------------------------------------------------------------------------
	//读取碰撞网格段
	void ObjectFileLoader::_ReadChunkBoundMesh()
	{
		//读取碰撞体信息
		static BoundInfo sBoundInfo;

		mpObjectPack->Read( &sBoundInfo, sizeof(BoundInfo) );

		//加载碰撞网格数据
		DWORD dwBoundFaceSize = sizeof(BoundFace) * sBoundInfo.nNumFace;
		DWORD dwBoundVertexSize = sizeof(Vector3) * sBoundInfo.nNumVertex;

		BoundFace* pBoundFace = (BoundFace*)malloc( dwBoundFaceSize + dwBoundVertexSize );
		Vector3* pBoundVertex = (Vector3*)( pBoundFace + sBoundInfo.nNumFace );

		mpObjectPack->Read( pBoundFace, dwBoundFaceSize );
		mpObjectPack->Read( pBoundVertex, dwBoundVertexSize );

		//创建碰撞网格
		BoundMesh* pBoundMesh = (BoundMesh*)PhysicsSystem::mpSingleton->CreateBoundVolume( BoundMesh::Creator );
		pBoundMesh->LoadBoundMesh( &sBoundInfo, pBoundFace, pBoundVertex );

		free( pBoundFace );

		*mLoadedBoundMeshList.Push() = pBoundMesh;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
