/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	CommonBaseInsideHead
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

//进行动态链接库导入时使用该头文件不包含模板实现
#ifdef FK_DLL_IMPORT
	#ifndef FK_NO_TEMPLATE_IMPLEMNTATION
		#define FK_NO_TEMPLATE_IMPLEMNTATION
	#endif
#endif

//--------------------------------------------------------------------------------------------------------------

#include "StandardInclude.h"
#include "Macros.h"
#include "BaseTypeDefine.h"
#include "MemoryManager.h"
#include "MemoryMarco.h"
#include "Singleton.h"
#include "Stack.h"
#include "MemPool.h"
#include "HashMap.h"
#include "Singleton.inl"
#include "Stack.inl"
#include "MemPool.inl"
#include "HashMap.inl"
#include "String.h"
#include "StringArray.h"
#include "CommandLine.h"
#include "Exception.h"
#include "MD5.h"
#include "Misc.h"
#include "NumConvert.h"
#include "ProcessorInfo.h"
#include "Timer.h"
#include "DataChunk.h"
#include "PRTDataChunk.h"
#include "ImageDecoder.h"
#include "BmpDecoder.h"
#include "TgaDecoder.h"
#include "JpgDecoder.h"
#include "DDSDecoder.h"
#include "BmpEncoder.h"
#include "TgaEncoder.h"
#include "JpgEncoder.h"
#include "MpegCommon.h"
#include "MpegInterface.h"
#include "SoundDecoder.h"
#include "WaveDecoder.h"
#include "MP3Decoder.h"
#include "FileSelectList.h"
#include "FileObject.h"
#include "BaseFile.h"
#include "PackObject.h"
#include "TextFile.h"
#include "FKPFile.h"

//--------------------------------------------------------------------------------------------------------------

using namespace FK3DEngine;

//--------------------------------------------------------------------------------------------------------------