/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	WinFile
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#include "../include/IO.h"
#include "../include/StringConversion.h"
#include "../include/WinUtility.h"
#include <Windows.h>

//--------------------------------------------------------------------
struct FK2DEngine::CFile::SImpl
{
public:
	HANDLE			m_hHandle;
public:
	SImpl()
		: m_hHandle( INVALID_HANDLE_VALUE )
	{

	}

	~SImpl()
	{
		if( m_hHandle != INVALID_HANDLE_VALUE )
		{
			::CloseHandle( m_hHandle );
		}
	}
};
//--------------------------------------------------------------------
FK2DEngine::CFile::CFile( const std::wstring& p_szFileName, ENUM_FileMode p_eMode )
	: m_pImpl( new SImpl )
{
	DWORD dwAccess;
	switch( p_eMode )
	{
	case EFM_Read:
		dwAccess = GENERIC_READ;
		break;
	case EFM_Replace:
		dwAccess = GENERIC_WRITE;
		break;
	case EFM_Alter:
		dwAccess = GENERIC_READ | GENERIC_WRITE;
		break;
	}

	DWORD dwShareMode = FILE_SHARE_READ;
	DWORD dwCreationDisp = ( p_eMode == EFM_Read ) ? OPEN_EXISTING : OPEN_ALWAYS;

	m_pImpl->m_hHandle = ::CreateFile( p_szFileName.c_str(), dwAccess, dwShareMode,
		0, dwCreationDisp, FILE_ATTRIBUTE_NORMAL, 0 );
	if( m_pImpl->m_hHandle == INVALID_HANDLE_VALUE )
	{
		Win::ThrowLastError( "开启" + FK2DEngine::Narrow( p_szFileName ) );
	}
	if( p_eMode == EFM_Replace )
	{
		Resize( 0 );
	}
}
//--------------------------------------------------------------------
FK2DEngine::CFile::~CFile()
{
	
}
//--------------------------------------------------------------------
std::size_t FK2DEngine::CFile::Size() const
{
	return ::GetFileSize( m_pImpl->m_hHandle, 0 );
}
//--------------------------------------------------------------------
void FK2DEngine::CFile::Resize( std::size_t p_unNewSize )
{
	if( ::SetFilePointer( m_pImpl->m_hHandle, p_unNewSize, 0, FILE_BEGIN ) == INVALID_SET_FILE_POINTER )
	{
		Win::ThrowLastError( "设置文件指针" );
	}
	Win::Check( ::SetEndOfFile( m_pImpl->m_hHandle ), "恢复一个文件" );
}
//--------------------------------------------------------------------
void FK2DEngine::CFile::Read( std::size_t p_unOffset, std::size_t p_unLength,
		  void* p_pDestBuffer ) const
{
	if( ::SetFilePointer( m_pImpl->m_hHandle, p_unOffset, 0, FILE_BEGIN ) == INVALID_SET_FILE_POINTER )
	{
		Win::ThrowLastError( "设置文件指针" );
	}
	DWORD dwDummy = 0;
	Win::Check( ::ReadFile( m_pImpl->m_hHandle, p_pDestBuffer, p_unLength, &dwDummy, 0 ) );
}
//--------------------------------------------------------------------
void FK2DEngine::CFile::Write( std::size_t p_unOffset, std::size_t p_unLength,
		   const void* p_pSourceBuffer )
{
	if( ::SetFilePointer( m_pImpl->m_hHandle, p_unOffset, 0, FILE_BEGIN ) == INVALID_SET_FILE_POINTER )
	{
		Win::ThrowLastError( "设置文件指针" );
	}
	DWORD dwDummy = 0;
	Win::Check( ::WriteFile( m_pImpl->m_hHandle, p_pSourceBuffer, p_unLength, &dwDummy, 0 ) );
}
//--------------------------------------------------------------------