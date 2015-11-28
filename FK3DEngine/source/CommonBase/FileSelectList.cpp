/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	FileSelectList
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../../include/CommonBase/CommonBaseInsideHead.h"

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	FileSelectList::FileSelectList( LPCSTR szRelPath )
		: mRelPath		(szRelPath)
		, mpFatherList	(NULL)
	{
		mSubSelectLists.Initialize( 8, 16 );
	}
	//--------------------------------------------------------------------------------------------------------------
	FileSelectList::~FileSelectList()
	{
		DelAllDirFromList();
		DelAllFileFromList();

		mSubSelectLists.Release();
	}
	//--------------------------------------------------------------------------------------------------------------
	//加入指定目录到选择列表
	FileSelectList* FileSelectList::AddDirToList( LPCSTR szDirName )
	{
		//将名称加入列表
		mDirList.Push( szDirName );

		//设置该子目录的相对路径字符串
		String TmpString( mRelPath );
		TmpString.AppendString( "\\" );
		TmpString.AppendString( szDirName );

		//创建该子目录的子级文件选择列表
		FileSelectList* pFileSelectList = new FileSelectList( TmpString.GetCharPtr() );
		pFileSelectList->mpFatherList = this;
		*mSubSelectLists.Push() = pFileSelectList;

		return pFileSelectList;
	}
	//--------------------------------------------------------------------------------------------------------------
	//加入指定文件到选择列表
	void FileSelectList::AddFileToList( LPCSTR szFileName )
	{
		//将名称加入列表
		mFileList.Push( szFileName );
	}
	//--------------------------------------------------------------------------------------------------------------
	//从列表中删除指定目录
	void FileSelectList::DelDirFromList( LPCSTR szDirName )
	{
		FileSelectLists::Iterator fit = mSubSelectLists.Begin();

		StringArray::Iterator it = mDirList.Begin();
		StringArray::Iterator end = mDirList.End();
		for(; it!=end; ++it, ++fit )
		{
			if( it != NULL && 0 == strcmp( it->mpCharString, szDirName ) )
			{
				mDirList.Erase( it );
				mSubSelectLists.Erase( fit );

				SafeDelete( fit );

				return;
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//从列表中删除指定文件
	void FileSelectList::DelFileFromList( LPCSTR szFileName )
	{
		StringArray::Iterator it = mFileList.Begin();
		StringArray::Iterator end = mFileList.End();
		for(; it!=end; ++it )
		{
			if( it != NULL && 0 == strcmp( it->mpCharString, szFileName ) )
			{
				mFileList.Erase( it );
				return;
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------
	//从列表中删除所有目录
	void FileSelectList::DelAllDirFromList()
	{
		FileSelectLists::Iterator fit = mSubSelectLists.Begin();
		FileSelectLists::Iterator fend = mSubSelectLists.End();
		for(; fit!=fend; ++fit )
		{
			if( fit != NULL )
				delete *fit;
		}

		mDirList.Clear();
		mSubSelectLists.Clear();
	}
	//--------------------------------------------------------------------------------------------------------------
	//从列表中删除所有文件
	void FileSelectList::DelAllFileFromList()
	{
		mFileList.Clear();
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取该列表相对路径
	String& FileSelectList::GetRelPath()
	{
		return mRelPath;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取该列表父级文件选择列表
	FileSelectList* FileSelectList::GetFatherSelectList()
	{
		return mpFatherList;
	}
	//--------------------------------------------------------------------------------------------------------------
	//获取子级目录的文件选择列表
	FileSelectList* FileSelectList::GetSubDirSelectList( LPCSTR szSubDirName )
	{
		FileSelectLists::Iterator fit = mSubSelectLists.Begin();

		StringArray::Iterator it = mDirList.Begin();
		StringArray::Iterator end = mDirList.End();
		for(; it!=end; ++it, ++fit )
		{
			if( it != NULL && 0 == strcmp( it->mpCharString, szSubDirName ) )
				return *fit;
		}

		return NULL;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
