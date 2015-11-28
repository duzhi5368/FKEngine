/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	StandardInclude
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	标准库声明导入
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//==================================================================================================================

// C++ 基本库
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cstdarg>
#include <cmath>

// STL 容器类
#include <vector>
#include <map>
#include <string>
#include <set>
#include <list>
#include <deque>

// STL 算法及功能
#include <algorithm>
#include <functional>
#include <limits>

// C++ 数据流管理 源文件
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

// Windows 源文件
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <windowsx.h>

#undef CreateDialog


namespace FK3DEngine
{
    using namespace std;
}

extern "C"
{
	#include <sys/types.h>
	#include <sys/stat.h>
}
