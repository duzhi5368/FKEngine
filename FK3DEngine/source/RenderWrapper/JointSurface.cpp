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
	JointSurface::JointSurface( ZOrderType eZType, OverlaySurface* pZRefOverlay, TextureFilterType eFilterType )
		: OverlaySurface	( eZType, pZRefOverlay, eFilterType )
		, mfWidth			(0.0f)
		, mfHeight			(0.0f)
		, mPicWidth			(0)
		, mPicHeight		(0)
		, mNumTextures		(0)
		, mNumCols			(0)
		, mNumRows			(0)
		, mPerWidth			(0)
		, mPerHeight		(0)
		, mRemWidth			(0)
		, mRemHeight		(0)
		, mActWidth			(0)
		, mActHeight		(0)
		, mpVertexData		(NULL)
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	JointSurface::~JointSurface()
	{
		//释放顶点数据
		SafeFree( mpVertexData );

		//删除该拼接图像所使用纹理的列表
		mNumTextures = 0;

		TextureList::Iterator it = mTextureList.Begin();
		TextureList::Iterator end = mTextureList.End();
		for(; it!=end; ++it )
			TextureManager::mpSingleton->DeleteTexture( *it );

		mTextureList.Clear();
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建拼接图像
	void JointSurface::CreateJointSurface( float fLeft, float fTop, float fWidth, float fHeight,
		UINT nTexWidth, UINT nTexHeight, PixelFormat ePixelFormat, bool bDynamic, DWORD color )
	{
		//存储渲染尺寸
		mfWidth = fWidth;
		mfHeight = fHeight;

		//存储纹理尺寸
		mPicWidth = nTexWidth;
		mPicHeight = nTexHeight;

		//获取最大纹理尺寸
		mPerWidth = TextureManager::mpSingleton->mMaxWidth;
		mPerHeight = TextureManager::mpSingleton->mMaxHeight;

		//设置是否需要动态更新纹理
		TextureUsage TexUsage;
		TexUsage = bDynamic ? TU_DYNAMIC : TU_NORMAL;

		//计算需要的纹理数量及其尺寸
		Div( mPicWidth, mPerWidth, mNumCols, mRemWidth );

		//横向最后一列纹理宽度
		if( mRemWidth == 0 )
			mRemWidth = mPerWidth;
		else
			++mNumCols;

		Div( mPicHeight, mPerHeight, mNumRows, mRemHeight );

		//纵向最后一行纹理高度
		if( mRemHeight == 0 )
			mRemHeight = mPerHeight;
		else
			++mNumRows;

		mNumTextures = mNumCols * mNumRows;

		//获得最后一张纹理的实际宽度和高度
		TextureManager::mpSingleton->GetBestSize( mRemWidth, mRemHeight, mActWidth, mActHeight );

		//分配顶点数据内存
		mNumVertex = mNumTextures * 4;
		mpVertexData = (OverlayVertex*)malloc( sizeof(OverlayVertex)*mNumVertex );
		OverlayVertex* pVer = mpVertexData;

		//初始化纹理列表空间
		mTextureList.Initialize( mNumRows * mNumCols, 2 );

		//创建纹理并计算顶点数据
		Texture* pTex = NULL;

		UINT nCurTexWidth, nCurTexHeight;
		float u2, v2;

		float fWidthScal = 1.0f / (float)mPicWidth * fWidth;
		float fHeightScal = 1.0f / (float)mPicHeight * fHeight;

		float fPosTop		= 0.0f;
		float fPosBottom	= 0.0f;
		float fPosLeft		= 0.0f;
		float fPosRight		= 0.0f;

		//行循环
		for( UINT r=0; r<mNumRows; ++r )
		{
			//计算坐标及 UV
			if( r == mNumRows - 1 )
			{
				v2 = mRemHeight / mActHeight;
				nCurTexHeight = mRemHeight;
			}
			else
			{
				v2 = 1.0;
				nCurTexHeight = mPerHeight;
			}
			float fPosHeight = nCurTexHeight * fHeightScal;
			fPosBottom += fPosHeight;

			//列循环
			for( UINT c=0; c<mNumCols; ++c )
			{
				//计算坐标及 UV
				if( c == mNumCols - 1 )
				{
					u2 = mRemWidth / mActWidth;
					nCurTexWidth = mRemWidth;
				}
				else
				{
					u2 = 1.0;
					nCurTexWidth = mPerWidth;
				}
				float fPosWidth = nCurTexWidth * fWidthScal;
				fPosRight += fPosWidth;

				//创建纹理
				pTex = TextureManager::mpSingleton->CreateTexture(
					nCurTexWidth, nCurTexHeight, ePixelFormat, TexUsage, 1 );
				*mTextureList.Push() = pTex;

				//计算顶点数据

				//左下
				pVer->x = fPosLeft + fLeft;		pVer->y = fPosBottom + fTop;	pVer->z = 1.0f;
				pVer->u = 0.0;	pVer->v = v2;	pVer->color = color;			++pVer;

				//左上
				pVer->x = fPosLeft + fLeft;		pVer->y = fPosTop + fTop;		pVer->z = 1.0f;
				pVer->u = 0.0;	pVer->v = 0.0;	pVer->color = color;			++pVer;

				//右上
				pVer->x = fPosRight + fLeft;	pVer->y = fPosTop + fTop;		pVer->z = 1.0f;
				pVer->u = u2;	pVer->v = 0.0;	pVer->color = color;			++pVer;

				//右下
				pVer->x = fPosRight + fLeft;	pVer->y = fPosBottom + fTop;	pVer->z = 1.0f;
				pVer->u = u2;	pVer->v = v2;	pVer->color = color;			++pVer;

				fPosLeft += fPosWidth;
			}

			fPosLeft = fPosRight = 0;
			fPosTop += fPosHeight;
		}

		//增加顶点计数
		OverlayManager::mpSingleton->mNumRenderVer += mNumVertex;

		mpVertexPtr = mpVertexData;
	}
	//--------------------------------------------------------------------------------------------------------------
	//从图像文件中载入拼接图像（纹理尺寸为图片尺寸）
	void JointSurface::LoadJointSurfaceFromFile( float fLeft, float fTop, float fWidth, float fHeight, FileObject* pFile,
		const char* szName, PixelFormat ePixelFormat, bool bDynamic, DWORD color )
	{
		//存储渲染尺寸
		mfWidth = fWidth;
		mfHeight = fHeight;

		//获取图像解码器
		ImageDecoder* pImgDecoder = &OverlayManager::mpSingleton->mImgDecoder;

		//载入图片
		pImgDecoder->LoadImageFromFile( pFile, szName );

		//解析图像头
		pImgDecoder->ParseHeader();

		//获取图像尺寸
		pImgDecoder->GetDimension( mPicWidth, mPicHeight );

		//获取最大纹理尺寸
		mPerWidth = TextureManager::mpSingleton->mMaxWidth;
		mPerHeight = TextureManager::mpSingleton->mMaxHeight;

		//设置是否需要动态更新纹理
		TextureUsage TexUsage;
		TexUsage = bDynamic ? TU_DYNAMIC : TU_NORMAL;

		//计算需要的纹理数量及其尺寸
		Div( mPicWidth, mPerWidth, mNumCols, mRemWidth );

		//横向最后一列纹理宽度
		if( mRemWidth == 0 )
			mRemWidth = mPerWidth;
		else
			++mNumCols;

		Div( mPicHeight, mPerHeight, mNumRows, mRemHeight );

		//纵向最后一行纹理高度
		if( mRemHeight == 0 )
			mRemHeight = mPerHeight;
		else
			++mNumRows;

		mNumTextures = mNumCols * mNumRows;

		//获得最后一张纹理的实际宽度和高度
		TextureManager::mpSingleton->GetBestSize( mRemWidth, mRemHeight, mActWidth, mActHeight );

		//分配顶点数据内存
		mNumVertex = mNumTextures * 4;
		mpVertexData = (OverlayVertex*)malloc( sizeof(OverlayVertex)* mNumVertex );
		OverlayVertex* pVer = mpVertexData;

		//初始化纹理列表空间
		mTextureList.Initialize( mNumRows * mNumCols, 2 );

		//创建纹理并计算顶点数据
		Texture* pTex = NULL;
		BYTE* pDst = NULL;
		UINT pitch = 0;

		UINT nTexWidth, nTexHeight;
		float u2, v2;

		float fWidthScal = 1.0f / (float)mPicWidth * fWidth;
		float fHeightScal = 1.0f / (float)mPicHeight * fHeight;

		float fPosTop		= 0.0f;
		float fPosBottom	= 0.0f;
		float fPosLeft		= 0.0f;
		float fPosRight		= 0.0f;

		RECT FillRect = { 0, 0, 0, 0 };

		//行循环
		for( UINT r=0; r<mNumRows; ++r )
		{
			//计算坐标及 UV
			if( r == mNumRows - 1 )
			{
				v2 = mRemHeight / mActHeight;
				nTexHeight = mRemHeight;
			}
			else
			{
				v2 = 1.0;
				nTexHeight = mPerHeight;
			}
			float fPosHeight = nTexHeight * fHeightScal;
			fPosBottom += fPosHeight;
			FillRect.bottom += nTexHeight;

			//列循环
			for( UINT c=0; c<mNumCols; ++c )
			{
				//计算坐标及 UV
				if( c == mNumCols - 1 )
				{
					u2 = mRemWidth / mActWidth;
					nTexWidth = mRemWidth;
				}
				else
				{
					u2 = 1.0;
					nTexWidth = mPerWidth;
				}
				float fPosWidth = nTexWidth * fWidthScal;
				fPosRight += fPosWidth;
				FillRect.right += nTexWidth;

				//创建纹理
				pTex = TextureManager::mpSingleton->CreateTexture( nTexWidth, nTexHeight, ePixelFormat, TexUsage, 1 );
				*mTextureList.Push() = pTex;

				//将图像数据填充到纹理中
				pTex->LockRect( (void**)&pDst, &pitch );
				pImgDecoder->Decode( pDst, &FillRect, NULL, pitch, ePixelFormat );
				pTex->UnlockRect();

				//计算顶点数据

				//左下
				pVer->x = fPosLeft + fLeft;		pVer->y = fPosBottom + fTop;	pVer->z = 1.0f;
				pVer->u = 0.0;	pVer->v = v2;	pVer->color = color;			++pVer;

				//左上
				pVer->x = fPosLeft + fLeft;		pVer->y = fPosTop + fTop;		pVer->z = 1.0f;
				pVer->u = 0.0;	pVer->v = 0.0;	pVer->color = color;			++pVer;

				//右上
				pVer->x = fPosRight + fLeft;	pVer->y = fPosTop + fTop;		pVer->z = 1.0f;
				pVer->u = u2;	pVer->v = 0.0;	pVer->color = color;			++pVer;

				//右下
				pVer->x = fPosRight + fLeft;	pVer->y = fPosBottom + fTop;	pVer->z = 1.0f;
				pVer->u = u2;	pVer->v = v2;	pVer->color = color;			++pVer;

				fPosLeft += fPosWidth;
				FillRect.left += nTexWidth;
			}

			fPosLeft = fPosRight = 0;
			fPosTop += fPosHeight;

			FillRect.left = FillRect.right = 0;
			FillRect.top += nTexHeight;
		}

		//关闭图片解码器
		if( !pImgDecoder->UnloadImage() )
			Except( Exception::ERR_INTERNAL_ERROR, "关闭拼接图像的图像数据载入解码器失败。" );

		//增加顶点计数
		OverlayManager::mpSingleton->mNumRenderVer += mNumVertex;

		mpVertexPtr = mpVertexData;
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置拼接图像位置
	void FKFastcall JointSurface::SetLocation( float fLeft, float fTop )
	{
		OverlayVertex* pVer = mpVertexData;

		//修改顶点数据
		float fWidthScal = 1.0f / (float)mPicWidth * mfWidth;
		float fHeightScal = 1.0f / (float)mPicHeight * mfHeight;

		float fPosTop		= fTop;
		float fPosBottom	= fTop;
		float fPosLeft		= fLeft;
		float fPosRight		= fLeft;

		//行循环
		for( UINT r=0; r<mNumRows; ++r )
		{
			//计算坐标
			float fHeight = ( ( r == mNumRows - 1 ) ? mRemHeight : mPerHeight ) * fHeightScal;
			fPosBottom += fHeight;

			//列循环
			for( UINT c=0; c<mNumCols; ++c )
			{
				//计算坐标
				float fWidth = ( ( c == mNumCols - 1 ) ? mRemWidth : mPerWidth ) * fWidthScal;
				fPosRight += fWidth;

				//计算顶点数据
				pVer->x = fPosLeft;		pVer->y = fPosBottom;	++pVer;	//左下
				pVer->x = fPosLeft;		pVer->y = fPosTop;		++pVer;	//左上
				pVer->x = fPosRight;	pVer->y = fPosTop;		++pVer;	//右上
				pVer->x = fPosRight;	pVer->y = fPosBottom;	++pVer;	//右下

				fPosLeft += fWidth;
			}

			fPosLeft = fPosRight = fLeft;
			fPosTop += fHeight;
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置拼接图像颜色
	void FKFastcall JointSurface::SetColor( DWORD color )
	{
		OverlayVertex* pVer = mpVertexData;

		//行循环
		for( UINT r=0; r<mNumRows; ++r )
		{
			//列循环
			for( UINT c=0; c<mNumCols; ++c )
			{
				//计算顶点数据
				pVer->color = color;	++pVer;	//左下
				pVer->color = color;	++pVer;	//左上
				pVer->color = color;	++pVer;	//右上
				pVer->color = color;	++pVer;	//右下
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取该拼接图像的纹理层
	Texture* JointSurface::GetTexture( UINT nNumTexture )
	{
#ifdef _DEBUG
		if( nNumTexture >= (UINT)mTextureList.Size() )
			return NULL;
#endif
		return mTextureList[nNumTexture];
	}
	//--------------------------------------------------------------------------------------------------------------
	//处理平面图像顶点
	void FKFastcall JointSurface::_ProcessVertex()
	{
	}
	//--------------------------------------------------------------------------------------------------------------
	//渲染平面图像
	void FKFastcall JointSurface::_RenderOverlay()
	{
		//设置纹理过滤类型
		RenderSystem::mpSingleton->_SetTextureFilter( ST_MAGFILTER, mFilterType, 0 );
		RenderSystem::mpSingleton->_SetTextureFilter( ST_MINFILTER, mFilterType, 0 );

		//循环渲染每张纹理
		TextureList::Iterator TexIterator = mTextureList.Begin();
		UINT nOffset = mVertexOffset;

		for( UINT i=0; i<mNumTextures; ++i, ++TexIterator, nOffset += 4 )
		{
			//设置纹理
			RenderSystem::mpSingleton->_SetTexture( *TexIterator, 0 );

			//绘制图元
			RenderSystem::mpSingleton->_RenderVertex( PT_TRIANGLE_FAN, nOffset, 2 );
		}
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
