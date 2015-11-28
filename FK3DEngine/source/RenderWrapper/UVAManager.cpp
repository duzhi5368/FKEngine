/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	UVAManager
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
	UVAManager::UVAManager()
		: mpPackFile	(NULL)
	{
		mUVAnimationList.Initialize( 20, 20 );
	}
	//--------------------------------------------------------------------------------------------------------------
	UVAManager::~UVAManager()
	{
		DeleteAllUVAnimation();
	}
	//--------------------------------------------------------------------------------------------------------------
	//设置 UVA 打包文件
	void UVAManager::SetUVAPackFile( FileObject* pPackFile )
	{
		mpPackFile = pPackFile;
	}
	//--------------------------------------------------------------------------------------------------------------
	//创建 UV 动画数据对象
	UVAnimation* UVAManager::CreateUVAnimation( LPCSTR szFileName )
	{
		//打开 UVA 文件
		mpPackFile->Open( szFileName );

		//读取 UVA 文件头信息
		UVAHeader sHeader;
		mpPackFile->Read( &sHeader, sizeof(UVAHeader) );

		if( sHeader.nVersion != 0x0100 )
			Except( Exception::ERR_INTERNAL_ERROR, (String)"指定载入的 UVA 文件 '" + szFileName + "' 版本不正确。" );

		//分配内存
		DWORD nDateLen = sizeof(UVASegment) * sHeader.nSegment + sizeof(UVAFrame) * sHeader.nFrames + sizeof(UVAnimation);
		BYTE* pMemBuf = (BYTE*)malloc( nDateLen );

		//创建 UV 动画数据对象
		UVAnimation* pUVAnimation = (UVAnimation*)pMemBuf;

		*mUVAnimationList.Push() = pUVAnimation;

		//读取动画段数据和帧数据
		pUVAnimation->nSegment = sHeader.nSegment;
		pUVAnimation->nFrames = sHeader.nFrames;
		pUVAnimation->pSegments = (UVASegment*)( pUVAnimation + 1 );
		pUVAnimation->pFrames = (UVAFrame*)( pUVAnimation->pSegments + pUVAnimation->nSegment );

		mpPackFile->Read( pUVAnimation->pSegments, nDateLen - sizeof(UVAnimation) );

		//关闭文件
		mpPackFile->Close();

		return pUVAnimation;
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除 UV 动画数据对象
	void UVAManager::DeleteUVAnimation( UVAnimation* pUVAnimation )
	{
		if( pUVAnimation == NULL )
			return;

		UVAnimationList::Iterator it = mUVAnimationList.Begin();
		UVAnimationList::Iterator end = mUVAnimationList.End();
		for(; it!=end; ++it )
		{
			if( *it == pUVAnimation )
			{
				UVAnimation* pUVA = *it;
				free( pUVA );

				mUVAnimationList.Erase( it );
				return;
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//删除所有 UV 动画数据对象
	void UVAManager::DeleteAllUVAnimation()
	{
		UVAnimationList::Iterator it = mUVAnimationList.Begin();
		UVAnimationList::Iterator end = mUVAnimationList.End();
		for(; it!=end; ++it )
		{
			if( *it != NULL )
			{
				UVAnimation* pUVA = *it;
				free( pUVA );
			}
		}

		mUVAnimationList.Clear();
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
