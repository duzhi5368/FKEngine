/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	NumConvert
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
	//是否可将多字节字符串转换为整数
	bool NumConvert::CanMultiByteToInt( LPCSTR szStr, UINT nLen )
	{
		if( nLen == 0 )
			nLen = (UINT)strlen( szStr );

		//如果第一个字符为负号则略过
		UINT nRemainNum = nLen;
		char* pStr = (char*)szStr;
		if( *pStr == '-' )
		{
			++pStr;
			--nRemainNum;
		}

		for( unsigned int i=0; i<nRemainNum; ++i )
		{
			BYTE Symbol = (BYTE)*pStr++;

			//如果当前字符不是数字则无法转换
			if( Symbol < 48 && Symbol > 57 )
				return false;
		}

		return true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//是否可将多字节字符串转换为浮点数
	bool NumConvert::CanMultiByteToFloat( LPCSTR szStr, UINT nLen )
	{
		if( nLen == 0 )
			nLen = (UINT)strlen( szStr );

		//如果第一个字符为负号则略过
		UINT nRemainNum = nLen;
		char* pStr = (char*)szStr;
		if( *pStr == '-' )
		{
			++pStr;
			--nRemainNum;
		}

		bool HaveDot = false;
		for( unsigned int i=0; i<nRemainNum; ++i )
		{
			BYTE Symbol = (BYTE)*pStr++;

			//如果遇到 '.' 小数点
			if( Symbol == 46 )
			{
				//如果是第一次遇到 '.' 小数点
				if( !HaveDot )
				{
					//如果小数点不在开始或结尾
					if( i > 0 && i+1 < nRemainNum )
						HaveDot = true;
					else
						return false;
				}
				//如果第二次遇到则无法转换
				else
					return false;
			}
			//如果当前字符不是数字则无法转换
			else if( Symbol < 48 && Symbol > 57 )
				return false;
		}

		return true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//是否可将 Unicode 字符串转换为整数
	bool NumConvert::CanUnicodeToInt( LPCWSTR szStr, UINT nLen )
	{
		if( nLen == 0 )
			nLen = (UINT)wcslen( szStr );

		//如果第一个字符为负号则略过
		UINT nRemainNum = nLen;
		wchar_t* pStr = (wchar_t*)szStr;
		if( *pStr == L'-' )
		{
			++pStr;
			--nRemainNum;
		}

		for( unsigned int i=0; i<nRemainNum; ++i )
		{
			WORD Symbol = (WORD)*pStr++;

			//如果当前字符不是数字则无法转换
			if( Symbol < 48 && Symbol > 57 )
				return false;
		}

		return true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//是否可将 Unicode 字符串转换为浮点数
	bool NumConvert::CanUnicodeToFloat( LPCWSTR szStr, UINT nLen )
	{
		if( nLen == 0 )
			nLen = (UINT)wcslen( szStr );

		//如果第一个字符为负号则略过
		UINT nRemainNum = nLen;
		wchar_t* pStr = (wchar_t*)szStr;
		if( *pStr == '-' )
		{
			++pStr;
			--nRemainNum;
		}

		bool HaveDot = false;
		for( unsigned int i=0; i<nRemainNum; ++i )
		{
			WORD Symbol = (WORD)*pStr++;

			//如果遇到 '.' 小数点
			if( Symbol == 46 )
			{
				//如果是第一次遇到 '.' 小数点
				if( !HaveDot )
				{
					//如果小数点不在开始或结尾
					if( i > 0 && i+1 < nRemainNum )
						HaveDot = true;
					else
						return false;
				}
				//如果第二次遇到则无法转换
				else
					return false;
			}
			//如果当前字符不是数字则无法转换
			else if( Symbol < 48 && Symbol > 57 )
				return false;
		}

		return true;
	}
	//--------------------------------------------------------------------------------------------------------------
	//将多字节字符串转换为整数
	int NumConvert::MultiByteToInt( LPCSTR szStr )
	{
		return atoi( szStr );
	}
	//--------------------------------------------------------------------------------------------------------------
	//将多字节字符串转换为浮点数
	float NumConvert::MultiByteToFloat( LPCSTR szStr )
	{
		return (float)atof( szStr );
	}
	//--------------------------------------------------------------------------------------------------------------
	//将 Unicode 字符串转换为整数
	int NumConvert::UnicodeToInt( LPCWSTR szStr )
	{
		return _wtoi( szStr );
	}
	//--------------------------------------------------------------------------------------------------------------
	//将 Unicode 字符串转换为浮点数
	float NumConvert::UnicodeToFloat( LPCWSTR szStr )
	{
		return (float)_wtof( szStr );
	}
	//--------------------------------------------------------------------------------------------------------------
	//将整数转换为多字节字符串
	int NumConvert::IntToMultiByte( int nValue, char* pBuf )
	{
		int nNumChar = 0;

		//将数字转换为倒序字符串
		char* pChar = pBuf;
		int nParseVal = ( nValue >= 0 ) ? nValue : -nValue;
		do
		{
			int nDigit = nParseVal % 10;
			nParseVal /= 10;

			*pChar = (char)( nDigit + '0' );
			++pChar;
			++nNumChar;
		}while( nParseVal > 0 );

		//如果为负数
		if( nValue < 0 )
		{
			*pChar = '-';
			++pChar;
			++nNumChar;
		}

		*pChar = '\0';
		--pChar;

		//反转字符顺序
		while( pBuf < pChar )
		{
			char TmpChar = *pBuf;
			*pBuf = *pChar;
			*pChar = TmpChar;
			--pChar;
			++pBuf;
		}

		return nNumChar;
	}
	//--------------------------------------------------------------------------------------------------------------
	//将整数转换为 Unicode 字符串
	int NumConvert::IntToUnicode( int nValue, wchar_t* pBuf )
	{
		int nNumChar = 0;

		//将数字转换为倒序字符串
		wchar_t* pChar = pBuf;
		int nParseVal = ( nValue >= 0 ) ? nValue : -nValue;
		do
		{
			int nDigit = nParseVal % 10;
			nParseVal /= 10;

			*pChar = (wchar_t)( nDigit + L'0' );
			++pChar;
			++nNumChar;
		}while( nParseVal > 0 );

		//如果为负数
		if( nValue < 0 )
		{
			*pChar = L'-';
			++pChar;
			++nNumChar;
		}

		*pChar = L'\0';
		--pChar;

		//反转字符顺序
		while( pBuf < pChar )
		{
			wchar_t TmpChar = *pBuf;
			*pBuf = *pChar;
			*pChar = TmpChar;
			--pChar;
			++pBuf;
		}

		return nNumChar;
	}
	//--------------------------------------------------------------------------------------------------------------
	//将浮点数转换为多字节字符串
	int NumConvert::FloatToMultiByte( float fValue, char* pBuf, int nPrecision )
	{
		// 4 字节浮点数可存储 8 位精度
		// 8 字节浮点数可存储 16 位精度
		// 10 字节浮点数可存储 19 位精度

		bool bSign = false;			//是否存在负号
		int nExponent;				// 10 进制指数

		unsigned char pBCDBuf[10] = { NULL };	//两进制格式数字缓存

		//如果浮点值为零
		if( fValue == 0.0f )
		{
			char* pChar = pBuf;
			*pChar = '0';
			++pChar;

			if( nPrecision > 0 )
			{
				*pChar = '.';
				++pChar;

				for( int i=0; i<nPrecision; ++i )
				{
					*pChar = '0';
					++pChar;
				}
			}

			*pChar = '\0';

			return (int)( pChar - pBuf );
		}

		const float fOne = 1.0f;
		const float fTen = 10.0f;

		int CtrlwdHolder;			//原控制字
		int CtrlwdSetter;			//新控制字

		__asm
		{
			//四字节浮点数存储结构： 1 位符号 8 位指数 23 位尾数
			mov		eax, [fValue];
			shr		eax, 23;

			//判断是否有符号
			test	eax, 0x100;
			je		NoSign;
			mov		BYTE PTR [bSign], 1;

NoSign:
			//获取浮点数两进制指数
			and		eax, 0xFF;
			sub		eax, 127;
			mov		[nExponent], eax;

			fclex;							//清除浮点异常
			fstcw	[CtrlwdHolder];			//保存控制字
			mov		[CtrlwdSetter], 0x27F;	//设置控制字
			fldcw	CtrlwdSetter;

			//浮点数的两进制指数 = log2( fValue ) 并且 log2( fValue ) * log10( 2 ) = log10( fValue )

			fild	[nExponent];			//装入两进制指数
			fldlg2;							// st0 = log10( 2 ), st1 = 指数
			fmulp	st(1), st;				// st0 = log10( fValue )
			fistp	[nExponent];			// nExponent = log10( fValue ) 得出 10 进制指数

			//需要将浮点数的 8 个有效数字移动到小数点前，以使其全在字符转换范围之内。
			//因此，我们需要将原浮点数乘以合适的十进制指数。

			xor		esi, esi;				//清空标志寄存器
			mov		eax, 7;					//要使 8 个有效数字在小数点前，那么其指数应为 7。
			sub		eax, [nExponent];		//减去实际指数得出修正值
			mov		[nExponent], eax;		//保存修正后的指数


			//根据修正指数计算实际的修正系数
			jns		CalcPower;				//如果指数小于零则改变它的符号
			neg		eax;
			mov		esi, 1;					//标志指数小于零

CalcPower:
			fld1;							//计算临时变量 z = 1	st0 = z
			fld		[fTen];					//计算临时变量 x = 10	st0 = x, st1 = z

CalcLoop:
			test	al, 1;					//如果指数为奇数
			je		ChangeFactor;
			// z *= x
			fxch	st(1);					// st0 = z, st1 = x
			fmul	st, st(1);				// st0 = z * x, st1 = x;
			fxch	st(1);					// st0 = x, st1 = z * x;

ChangeFactor:
			shr		eax, 1;					//指数除以 2
			test	eax, eax;				//如果指数为零则退出循环
			je		ExitLoop;
			// x *= x
			fld		st(0);					// st0 = x, st1 = x, st1 = z
			fmulp	st(1), st;				// st0 = x * x, st1 = z

			jmp		CalcLoop;				//回到循环起始

ExitLoop:
			test	esi, esi;				//如果指数为负数
			fstp	st(0);					// st0 = z （即最终的修正系数）
			je		PositiveExp;
			fdivr	[fOne];					//求它的倒数 st0 = 1.0f / z

PositiveExp:


			fld		[fValue];				//装入源浮点数 st0 = fValue, st1 = 修正系数
			fmulp	st(1), st;				//变化原浮点数进位 st0 = 变化后的浮点数

			//转换 BCD 格式数字为字符串
			lea		esi, pBCDBuf;
			fbstp	[esi];					//将 st0 保存为两进制编码的十进制数字（BCD）

			add		esi, 3;					// BCD 格式每字节保存两个数字，总共保存 18 个数字，顶部一个字节保存符号。
			mov		edi, pBuf;				//我们只需要转换前 4 个字节的 8 个数字，因此我们将指针指向数字起始位置。

			//循环 4 次处理 8 个数字
			mov		ecx, 4;

ProcTwoDigit:
			movzx	ax, BYTE PTR [esi];		//将两位数字载入 ax 寄存器 xxxx xxxx AAAA BBBB
			dec		esi;					//将指针指向下两个数字存储字节
			shl		ax, 4;					//xxxx AAAA BBBB xxxx
			shr		al, 4;					//xxxx AAAA xxxx BBBB
			rol		ax, 8;					//xxxx BBBB xxxx AAAA
			add		ax, 0x3030;				//BBBB + 48 AAAA + 48 （此时已为多字节字符集数字编码）
			mov		[edi], ax;				//将字符数据复制进字符串
			add		edi, 2;					//移动两字符偏移量

			dec		ecx;					//递减计数器
			jnz		ProcTwoDigit;

			fldcw	[CtrlwdHolder];			//恢复控制字
			fwait;
		}

		//如果有效数字第一位为零
		if( *pBuf == '0' )
		{
			memcpy( pBuf, pBuf + 1, 7 );
			pBuf[7] = '0';
			++nExponent;
		}

		char* pEndPos = NULL;

		//如果小数点在有效数字左边
		if( nExponent >= 8 )
		{
			//如果需要显示小数
			if( nPrecision > 0 )
			{
				//计算小数点右边填充零位数量
				int nNumZeroPad = nExponent - 8;
				if( nNumZeroPad > nPrecision )
					nNumZeroPad = nPrecision;

				//计算保留有效数字数量
				int nRemainDigit = nPrecision - nNumZeroPad;

				//小数起始位置
				char* pDecPos = pBuf + 2;

				//移动有效数字
				memmove( pDecPos + nNumZeroPad, pBuf, nRemainDigit );

				//填充零位
				for( int i=0; i<nNumZeroPad; ++i )
					pDecPos[i] = '0';

				pBuf[0] = '0';
				pBuf[1] = '.';

				pEndPos = pDecPos + nPrecision;
			}
			//如果不需要显示小数
			else
			{
				*pBuf = '1';
				pEndPos = pBuf + 1;
			}
		}
		//如果小数点在有效数字右边
		else if( nExponent <= 0 )
		{
			//计算小数点左边填充零位数量
			int nNumZeroPad = -nExponent;
			char* pPadPos = pBuf + 8;
			for( int i=0; i<nNumZeroPad; ++i )
				pPadPos[i] = '0';

			//如果需要显示小数
			if( nPrecision > 0 )
			{
				pPadPos[ nNumZeroPad ] = '.';

				//小数起始位置
				char* pDecPos = pPadPos + nNumZeroPad + 1;
				for( int i=0; i<nPrecision; ++i )
					pDecPos[i] = '0';

				pEndPos = pDecPos + nPrecision;
			}
			//如果不需要显示小数
			else
			{
				pEndPos = pPadPos + nNumZeroPad;
			}
		}
		//如果小数点在有效数字中间
		else
		{
			//如果需要显示小数
			if( nPrecision > 0 )
			{
				//小数点右边有效数字位置
				char* pDecDigit = pBuf + 8 - nExponent;

				//计算小数点右边保留有效数字数量
				int nRemainDigit = nExponent;
				if( nRemainDigit > nPrecision )
				{
					nRemainDigit = nPrecision;
				}
				else
				{
					//计算小数点右边填充零位数量
					int nNumZeroPad = nPrecision - nRemainDigit;
					char* pPadPos = pDecDigit + 1 + nRemainDigit;

					//填充零位
					for( int i=0; i<nNumZeroPad; ++i )
						pPadPos[i] = '0';
				}

				//移动小数点右边有效数字到正确位置
				memmove( pDecDigit + 1, pDecDigit, nRemainDigit );

				*pDecDigit = '.';

				pEndPos = pDecDigit + nPrecision + 1;
			}
			//如果不需要显示小数
			else
			{
				pEndPos = pBuf + 8 - nExponent;
			}
		}

		*pEndPos = '\0';
		int nStrLen = (int)( pEndPos - pBuf ) + 1;

		//如果存在负号
		if( bSign )
		{
			//检查是否存在不为零的有效数字
			char* pChar = pBuf;
			while( *pChar != '\0' )
			{
				if( *pChar != '0' && *pChar != '.' )
				{
					//在字符串起始位置增加负号
					memmove( pBuf + 1, pBuf, nStrLen );
					pBuf[0] = '-';
					++nStrLen;
					return nStrLen;
				}

				++pChar;
			}
		}

		return nStrLen;
	}
	//--------------------------------------------------------------------------------------------------------------
	//将浮点数转换为 Unicode 字符串
	int NumConvert::FloatToUnicode( float fValue, wchar_t* pBuf, int nPrecision )
	{		
		bool bSign = false;			//是否存在负号
		int nExponent;				// 10 进制指数

		unsigned char pBCDBuf[10] = { NULL };	//两进制格式数字缓存

		//如果浮点值为零
		if( fValue == 0.0f )
		{
			wchar_t* pChar = pBuf;
			*pChar = L'0';
			++pChar;

			if( nPrecision > 0 )
			{
				*pChar = L'.';
				++pChar;

				for( int i=0; i<nPrecision; ++i )
				{
					*pChar = L'0';
					++pChar;
				}
			}

			*pChar = L'\0';

			return (int)( pChar - pBuf );
		}

		const float fOne = 1.0f;
		const float fTen = 10.0f;

		int CtrlwdHolder;			//原控制字
		int CtrlwdSetter;			//新控制字

		__asm
		{
			//四字节浮点数存储结构： 1 位符号 8 位指数 23 位尾数
			mov		eax, [fValue];
			shr		eax, 23;

			//判断是否有符号
			test	eax, 0x100;
			je		NoSign;
			mov		BYTE PTR [bSign], 1;

NoSign:
			//获取浮点数两进制指数
			and		eax, 0xFF;
			sub		eax, 127;
			mov		[nExponent], eax;

			fclex;							//清除浮点异常
			fstcw	[CtrlwdHolder];			//保存控制字
			mov		[CtrlwdSetter], 0x27F;	//设置控制字
			fldcw	CtrlwdSetter;

			//浮点数的两进制指数 = log2( fValue ) 并且 log2( fValue ) * log10( 2 ) = log10( fValue )

			fild	[nExponent];			//装入两进制指数
			fldlg2;							// st0 = log10( 2 ), st1 = 指数
			fmulp	st(1), st;				// st0 = log10( fValue )
			fistp	[nExponent];			// nExponent = log10( fValue ) 得出 10 进制指数

			//需要将浮点数的 8 个有效数字移动到小数点前，以使其全在字符转换范围之内。
			//因此，我们需要将原浮点数乘以合适的十进制指数。

			xor		esi, esi;				//清空标志寄存器
			mov		eax, 7;					//要使 8 个有效数字在小数点前，那么其指数应为 7。
			sub		eax, [nExponent];		//减去实际指数得出修正值
			mov		[nExponent], eax;		//保存修正后的指数


			//根据修正指数计算实际的修正系数
			jns		CalcPower;				//如果指数小于零则改变它的符号
			neg		eax;
			mov		esi, 1;					//标志指数小于零

CalcPower:
			fld1;							//计算临时变量 z = 1	st0 = z
			fld		[fTen];					//计算临时变量 x = 10	st0 = x, st1 = z

CalcLoop:
			test	al, 1;					//如果指数为奇数
			je		ChangeFactor;
			// z *= x
			fxch	st(1);					// st0 = z, st1 = x
			fmul	st, st(1);				// st0 = z * x, st1 = x;
			fxch	st(1);					// st0 = x, st1 = z * x;

ChangeFactor:
			shr		eax, 1;					//指数除以 2
			test	eax, eax;				//如果指数为零则退出循环
			je		ExitLoop;
			// x *= x
			fld		st(0);					// st0 = x, st1 = x, st1 = z
			fmulp	st(1), st;				// st0 = x * x, st1 = z

			jmp		CalcLoop;				//回到循环起始

ExitLoop:
			test	esi, esi;				//如果指数为负数
			fstp	st(0);					// st0 = z （即最终的修正系数）
			je		PositiveExp;
			fdivr	[fOne];					//求它的倒数 st0 = 1.0f / z

PositiveExp:


			fld		[fValue];				//装入源浮点数 st0 = fValue, st1 = 修正系数
			fmulp	st(1), st;				//变化原浮点数进位 st0 = 变化后的浮点数

			//转换 BCD 格式数字为字符串
			lea		esi, pBCDBuf;
			fbstp	[esi];					//将 st0 保存为两进制编码的十进制数字（BCD）

			add		esi, 3;					// BCD 格式每字节保存两个数字，总共保存 18 个数字，顶部一个字节保存符号。
			mov		edi, pBuf;				//我们只需要转换前 4 个字节的 8 个数字，因此我们将指针指向数字起始位置。

			//循环 4 次处理 8 个数字
			mov		ecx, 4;

ProcTwoDigit:
			movzx	eax, BYTE PTR [esi];	//将两位数字载入 eax 寄存器 xxxx xxxx xxxx xxxx xxxx xxxx AAAA BBBB
			dec		esi;					//将指针指向下两个数字存储字节
			shl		eax, 12;				//xxxx xxxx xxxx AAAA BBBB xxxx xxxx xxxx
			shr		ax, 12;					//xxxx xxxx xxxx AAAA xxxx xxxx xxxx BBBB
			rol		eax, 16;				//xxxx xxxx xxxx BBBB xxxx xxxx xxxx AAAA
			add		eax, 0x300030;			//xxxx xxxx BBBB + 48 xxxx xxxx AAAA + 48 （此时已为双字节字符集数字编码）
			mov		[edi], eax;				//将字符数据复制进字符串
			add		edi, 4;					//移动两字符偏移量

			dec		ecx;					//递减计数器
			jnz		ProcTwoDigit;

			fldcw	[CtrlwdHolder];			//恢复控制字
			fwait;
		}

		//如果有效数字第一位为零
		if( *pBuf == L'0' )
		{
			wmemcpy( pBuf, pBuf + 1, 7 );
			pBuf[7] = L'0';
			++nExponent;
		}

		wchar_t* pEndPos = NULL;

		//如果小数点在有效数字左边
		if( nExponent >= 8 )
		{
			//如果需要显示小数
			if( nPrecision > 0 )
			{
				//计算小数点右边填充零位数量
				int nNumZeroPad = nExponent - 8;
				if( nNumZeroPad > nPrecision )
					nNumZeroPad = nPrecision;

				//计算保留有效数字数量
				int nRemainDigit = nPrecision - nNumZeroPad;

				//小数起始位置
				wchar_t* pDecPos = pBuf + 2;

				//移动有效数字
				wmemmove( pDecPos + nNumZeroPad, pBuf, nRemainDigit );

				//填充零位
				for( int i=0; i<nNumZeroPad; ++i )
					pDecPos[i] = L'0';

				pBuf[0] = L'0';
				pBuf[1] = L'.';

				pEndPos = pDecPos + nPrecision;
			}
			//如果不需要显示小数
			else
			{
				*pBuf = L'1';
				pEndPos = pBuf + 1;
			}
		}
		//如果小数点在有效数字右边
		else if( nExponent <= 0 )
		{
			//计算小数点左边填充零位数量
			int nNumZeroPad = -nExponent;
			wchar_t* pPadPos = pBuf + 8;
			for( int i=0; i<nNumZeroPad; ++i )
				pPadPos[i] = L'0';

			//如果需要显示小数
			if( nPrecision > 0 )
			{
				pPadPos[ nNumZeroPad ] = L'.';

				//小数起始位置
				wchar_t* pDecPos = pPadPos + nNumZeroPad + 1;
				for( int i=0; i<nPrecision; ++i )
					pDecPos[i] = L'0';

				pEndPos = pDecPos + nPrecision;
			}
			//如果不需要显示小数
			else
			{
				pEndPos = pPadPos + nNumZeroPad;
			}
		}
		//如果小数点在有效数字中间
		else
		{
			//如果需要显示小数
			if( nPrecision > 0 )
			{
				//小数点右边有效数字位置
				wchar_t* pDecDigit = pBuf + 8 - nExponent;

				//计算小数点右边保留有效数字数量
				int nRemainDigit = nExponent;
				if( nRemainDigit > nPrecision )
				{
					nRemainDigit = nPrecision;
				}
				else
				{
					//计算小数点右边填充零位数量
					int nNumZeroPad = nPrecision - nRemainDigit;
					wchar_t* pPadPos = pDecDigit + 1 + nRemainDigit;

					//填充零位
					for( int i=0; i<nNumZeroPad; ++i )
						pPadPos[i] = L'0';
				}

				//移动小数点右边有效数字到正确位置
				wmemmove( pDecDigit + 1, pDecDigit, nRemainDigit );

				*pDecDigit = L'.';

				pEndPos = pDecDigit + nPrecision + 1;
			}
			//如果不需要显示小数
			else
			{
				pEndPos = pBuf + 8 - nExponent;
			}
		}

		*pEndPos = '\0';
		int nStrLen = (int)( pEndPos - pBuf ) + 1;

		//如果存在负号
		if( bSign )
		{
			//检查是否存在不为零的有效数字
			wchar_t* pChar = pBuf;
			while( *pChar != L'\0' )
			{
				if( *pChar != L'0' && *pChar != L'.' )
				{
					//在字符串起始位置增加负号
					wmemmove( pBuf + 1, pBuf, nStrLen );
					pBuf[0] = L'-';
					++nStrLen;
					return nStrLen;
				}

				++pChar;
			}
		}

		return nStrLen;
	}
	//--------------------------------------------------------------------------------------------------------------
}

//==================================================================================================================
