/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	PRTDataChunk
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	保护数据块类
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

namespace FK3DEngine
{
	//--------------------------------------------------------------------------------------------------------------
	//保护数据块类
	class FK_DLL PRTDataChunk : public DataChunk
	{
	protected:
		BYTE		mpPassword[32];		//保护密码

	public:
		PRTDataChunk();
		PRTDataChunk( PRTDataChunk& refDataChunk );
		PRTDataChunk( size_t nSize );
		~PRTDataChunk();

		//设置读写保护密码（输入密码字符串）
		void SetPassword( const char* szPassword );

		//设置读写保护密码
		void SetPassword( const BYTE pPassword[16] );

		//清除读写保护密码
		void ClearPassword();

		//写入数据到保护数据块
		void Code( const void* pBuf, size_t nLen );

		//从保护数据块读出数据
		void Decode( void* pBuf, size_t nLen );

		//赋值操作符
		PRTDataChunk& operator = ( const PRTDataChunk& refDataChunk );
	};
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
