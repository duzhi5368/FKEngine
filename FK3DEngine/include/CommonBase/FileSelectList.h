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
*	文件说明：	文件选择列表类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================
#include "../CommonBase/Stack.h"
#include "../CommonBase/String.h"
#include "../CommonBase/StringArray.h"
//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------

	class FileSelectList;
	typedef Stack< FileSelectList* >	FileSelectLists;

	//--------------------------------------------------------------------------------------------------------------
	//文件选择列表类
	class FK_DLL FileSelectList
	{
	public:
		String				mRelPath;		//该列表（目录）相对路径

		StringArray			mDirList;		//子目录名称列表
		StringArray			mFileList;		//文件名称列表

		FileSelectList*		mpFatherList;	//该列表的父级文件选择列表

		FileSelectLists		mSubSelectLists;//子级文件选择列表

	public:
		FileSelectList( LPCSTR szRelPath );
		virtual ~FileSelectList();

		//加入指定目录到选择列表
		FileSelectList* AddDirToList( LPCSTR szDirName );

		//加入指定文件到选择列表
		void AddFileToList( LPCSTR szFileName );

		//从列表中删除指定目录
		void DelDirFromList( LPCSTR szDirName );

		//从列表中删除指定文件
		void DelFileFromList( LPCSTR szFileName );

		//从列表中删除所有目录
		void DelAllDirFromList();

		//从列表中删除所有文件
		void DelAllFileFromList();

		//获取该列表相对路径
		String& GetRelPath();

		//获取该列表父级文件选择列表
		FileSelectList* GetFatherSelectList();

		//获取子级目录的文件选择列表
		FileSelectList* GetSubDirSelectList( LPCSTR szSubDirName );
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
