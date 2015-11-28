/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	TextTTFWin
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include <Windows.h>
#include "../include/Text.h"
#include "../include/StringConversion.h"

//--------------------------------------------------------------------
typedef struct SFontPropertiesAnsi
{
	char 			m_szName[1024];
	char 			m_szCopyright[1024];
	char 			m_szTrademark[1024];
	char 			m_szFamily[1024];
}FONT_PROPERTIES_ANSI;
//--------------------------------------------------------------------
typedef struct STTOffsetTable
{
	unsigned short	m_usMajorVersion;
	unsigned short	m_usMinorVersion;
	unsigned short	m_usNumOfTables;
	unsigned short	m_usSearchRange;
	unsigned short	m_usEntrySeletor;
	unsigned short	m_usRangeShift;
}TT_OFFSET_TABLE;
//--------------------------------------------------------------------
typedef struct STTTableDirectory
{
	char			m_szTag[4];
	unsigned long	m_ulCheckSum;
	unsigned long	m_ulOffset;
	unsigned long	m_ulLength;
}TT_TABLE_DIRECTORY;
//--------------------------------------------------------------------
typedef struct STTNameTableHeader
{
	unsigned short	m_usFSelector;
	unsigned short	m_usNRCount;
	unsigned short	m_usStorageOffset;
}TT_NAME_TABLE_HEADER;
//--------------------------------------------------------------------
typedef struct STTNameRecord
{
	unsigned short	m_usPlatformID;
	unsigned short	m_usEncodingID;
	unsigned short	m_usLanguageID;
	unsigned short	m_usNameID;
	unsigned short	m_usStringLength;
	unsigned short	m_usStringOffset;
}TT_NAME_RECORD;
//--------------------------------------------------------------------
#define SWAPWORD(x)		MAKEWORD(HIBYTE(x),LOBYTE(x))
#define SWAPLONG(x)		MAKELONG(SWAPWORD(HIWORD(x)), SWAPWORD(LOWORD(x)))
#define _T(x) x
#define TRACE printf
//--------------------------------------------------------------------
namespace FK2DEngine
{
	std::wstring GetNameFromTTFFile( const std::wstring& p_szFileName )
	{
		FONT_PROPERTIES_ANSI	fp;
		FONT_PROPERTIES_ANSI	*lpFontProps	= &fp;
		memset( lpFontProps, 0, sizeof( FONT_PROPERTIES_ANSI ) );

		HANDLE hFile = INVALID_HANDLE_VALUE;
		hFile = ::CreateFile( p_szFileName.c_str(), GENERIC_READ, 0, NULL, 
			OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL );

		if( hFile == INVALID_HANDLE_VALUE )
		{
			TRACE( _T("错误：打开文件'%s' 失败\n"), FK2DEngine::Narrow( p_szFileName ).c_str() );
			TRACE( _T("错误： %s 失败"), _T("创建文件") );
			return p_szFileName;
		}

		DWORD dwFileSize = ::GetFileSize( hFile, NULL );

		if( dwFileSize == INVALID_FILE_SIZE )
		{
			TRACE( _T("错误： %s 失败"), _T("获取文件大小") );
			::CloseHandle( hFile );
			return p_szFileName;
		}

		HANDLE hMappedFile = NULL;
		hMappedFile = ::CreateFileMapping( hFile, NULL, PAGE_READONLY, 0, dwFileSize, NULL );

		if( hMappedFile == NULL )
		{
			TRACE( _T("错误： %s 失败"), _T("创建文件映射内核对象") );
			::CloseHandle( hFile );
			return p_szFileName;
		}

		LPBYTE lpMapAddress = (LPBYTE)::MapViewOfFile( hMappedFile, FILE_MAP_READ, 0, 0, 0 );

		if( lpMapAddress == NULL )
		{
			TRACE( _T("错误： %s 失败"), _T("创建文件视图") );
			::CloseHandle( hMappedFile );
			::CloseHandle( hFile );
			return p_szFileName;
		}

		BOOL bRetVal = FALSE;
		int nIndex = 0;

		TT_OFFSET_TABLE		ttOffsetTable;
		memcpy( &ttOffsetTable, &lpMapAddress[nIndex], sizeof( TT_OFFSET_TABLE ) );
		nIndex += sizeof( TT_OFFSET_TABLE );

		ttOffsetTable.m_usNumOfTables = SWAPWORD( ttOffsetTable.m_usNumOfTables );
		ttOffsetTable.m_usMajorVersion = SWAPWORD( ttOffsetTable.m_usMajorVersion );
		ttOffsetTable.m_usMinorVersion = SWAPWORD( ttOffsetTable.m_usMinorVersion );

		if( ttOffsetTable.m_usMajorVersion != 1 || ttOffsetTable.m_usMinorVersion != 0 )
		{
			return L"";
		}

		TT_TABLE_DIRECTORY tableDirectory;
		memset( &tableDirectory, 0, sizeof( TT_TABLE_DIRECTORY ) );
		BOOL bFound = FALSE;
		char szTemp[4096];
		memset( szTemp, 0, sizeof( szTemp ) );

		for( int i = 0; i < ttOffsetTable.m_usNumOfTables; ++i )
		{
			memcpy( &tableDirectory, &lpMapAddress[nIndex], sizeof( TT_TABLE_DIRECTORY ) );
			nIndex += sizeof( TT_TABLE_DIRECTORY );

			strncpy_s( szTemp, tableDirectory.m_szTag, 4 );
			if( _stricmp( szTemp, "name" ) == 0 )
			{
				bFound = TRUE;
				tableDirectory.m_ulLength = SWAPLONG( tableDirectory.m_ulLength );
				tableDirectory.m_ulOffset = SWAPLONG( tableDirectory.m_ulOffset );
				break;
			}
			else if( szTemp[0] == 0 )
			{
				break;
			}
		}

		if( bFound )
		{
			nIndex = tableDirectory.m_ulOffset;

			TT_NAME_TABLE_HEADER	ttNameTableHeader;
			memcpy( &ttNameTableHeader, &lpMapAddress[nIndex], sizeof( TT_NAME_TABLE_HEADER ) );
			nIndex += sizeof( TT_NAME_TABLE_HEADER );

			ttNameTableHeader.m_usNRCount		= SWAPWORD( ttNameTableHeader.m_usNRCount );
			ttNameTableHeader.m_usStorageOffset = SWAPWORD( ttNameTableHeader.m_usStorageOffset );

			TT_NAME_RECORD			ttNameRecord;
			bFound = FALSE;

			for( int i = 0; i < ttNameTableHeader.m_usNRCount && 
				( ( lpFontProps->m_szCopyright[0] == 0 ) ||
				( lpFontProps->m_szName[0] == 0 ) ||
				( lpFontProps->m_szTrademark[0] == 0 ) ||
				( lpFontProps->m_szFamily[0] == 0 ) ); ++i )
			{
				memcpy( &ttNameRecord, &lpMapAddress[nIndex], sizeof( TT_NAME_RECORD ) );
				nIndex += sizeof( TT_NAME_RECORD );

				ttNameRecord.m_usNameID			= SWAPWORD( ttNameRecord.m_usNameID );
				ttNameRecord.m_usStringLength	= SWAPWORD( ttNameRecord.m_usStringLength );
				ttNameRecord.m_usStringOffset	= SWAPWORD( ttNameRecord.m_usStringOffset );

				if( ttNameRecord.m_usNameID == 1 || ttNameRecord.m_usNameID == 0
					|| ttNameRecord.m_usNameID == 7 )
				{
					int nPos = nIndex;

					nIndex = tableDirectory.m_ulOffset + ttNameRecord.m_usStringOffset +
						ttNameTableHeader.m_usStorageOffset;

					memset( szTemp, 0, sizeof( szTemp ) );

					memcpy( szTemp, &lpMapAddress[nIndex], ttNameRecord.m_usStringLength );
					nIndex += ttNameRecord.m_usStringLength;

					if( szTemp[0] != 0 )
					{
						_ASSERTE( strlen( szTemp ) < sizeof( lpFontProps->m_szName ) );

						switch( ttNameRecord.m_usNameID )
						{
						case 0:
							{
								if( lpFontProps->m_szCopyright[0] == 0 )
								{
									strncpy_s( lpFontProps->m_szCopyright, szTemp, sizeof( lpFontProps->m_szCopyright ) - 1 );
								}
							}
							break;
						case 1:
							{
								if( lpFontProps->m_szFamily[0] == 0 )
								{
									strncpy_s( lpFontProps->m_szFamily, szTemp, sizeof( lpFontProps->m_szFamily ) - 1 );
									bRetVal = TRUE;
								}
							}
							break;
						case 4:
							{
								if( lpFontProps->m_szName[0] == 0 )
								{
									strncpy_s( lpFontProps->m_szName, szTemp, sizeof( lpFontProps->m_szName ) - 1 );
								}
							}
							break;
						case 7:
							{
								if( lpFontProps->m_szTrademark[0] == 0 )
								{
									strncpy_s( lpFontProps->m_szTrademark, szTemp, sizeof( lpFontProps->m_szTrademark ) - 1 );
								}
							}
							break;
						default:
							break;
						}
					}

					nIndex = nPos;
				}
			}
		}

		::UnmapViewOfFile( lpMapAddress );
		::CloseHandle( hMappedFile );
		::CloseHandle( hFile );

		if( lpFontProps->m_szName[0] == 0 )
		{
			strcpy_s( lpFontProps->m_szName, lpFontProps->m_szFamily );
		}

		return FK2DEngine::Widen( lpFontProps->m_szName );
	}
}