/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	PackObject
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	打包文件对象类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//打包文件对象类 （纯虚类）
	class FK_DLL PackObject : public FileObject
	{
	protected:
		BaseFile		mPackFile;		//打包文件对象

	public:
		//创建打包文件
		virtual void CreatePack( LPCSTR szPackName, LPCSTR szBasePath, StringArray* pFileNames, int nSubLevel ) = 0;

		//打开打包文件
		virtual void OpenPack( LPCSTR szPackName ) = 0;

		//在打包文件内增加文件
		virtual void AddToPack( StringArray* pFileNames, int nSubLevel ) = 0;

		//从打包文件内删除文件
		virtual void DelFromPack( StringArray* pFileNames ) = 0;

		//获取包内文件列表
		virtual void GetFileListInPack( FileSelectList* pFileList ) = 0;

		//关闭打包文件
		virtual bool ClosePack() = 0;
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
