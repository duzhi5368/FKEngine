/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	IO
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "../../depend/boost/utility.hpp"
#include "../../depend/boost/scoped_ptr.hpp"
#include <cstddef>
#include <algorithm>
#include <string>
#include <vector>
//--------------------------------------------------------------------
namespace FK2DEngine
{
	class CResource;

	enum ENUM_ByteOrder
	{
		EBO_Little,
		EBO_Big,
		EBO_NoCare,
	};

#ifdef __BIG_ENDIAN__
	const ENUM_ByteOrder eNativeByteOrder = EBO_Big;
	const ENUM_ByteOrder eOtherByteOrder = EBO_Little;
#else
	const ENUM_ByteOrder eNativeByteOrder = EBO_Little;
	const ENUM_ByteOrder eOtherByteOrder = EBO_Big;
#endif
	//--------------------------------------------------------------------
	//--------------------------------------------------------------------
	class CReader
	{
	private:
		const CResource*	m_pRes;
		std::size_t			m_unPos;
	public:
		CReader( const CResource& p_Res, std::size_t p_unPos )
			: m_pRes( &p_Res )
			, m_unPos( p_unPos )
		{

		}

		const CResource& Resource() const
		{
			return *m_pRes;
		}

		std::size_t Position() const
		{
			return m_unPos;
		}

		void SetPosition( std::size_t p_unValue )
		{
			m_unPos = p_unValue;
		}

		void Seek( std::ptrdiff_t p_Offset )
		{
			m_unPos += p_Offset;
		}

		void Read( void* p_pDestBuffer, std::size_t p_unLength );

		template< typename T >
		void ReadPod( T& p_T, ENUM_ByteOrder p_eBO = EBO_NoCare )
		{
			Read( &p_T, sizeof( p_T ) );
			if( p_eBO == eOtherByteOrder )
			{
				char* pBegin = reinterpret_cast< char* >( &p_T );
				std::reverse( pBegin, pBegin + sizeof( p_T ) );
			}
		}

		template< typename T >
		T GetPod( ENUM_ByteOrder p_eBO = EBO_NoCare )
		{
			T t;
			ReadPod< T >( t, p_eBO );
			return t;
		}
	};
	//--------------------------------------------------------------------
	//--------------------------------------------------------------------
	class CWriter
	{
	private:
		CResource*		m_pRes;
		std::size_t		m_unPos;
	public:
		CWriter( CResource& p_Res, std::size_t p_unPos )
			: m_pRes( &p_Res )
			, m_unPos( p_unPos )
		{

		}

		CResource& Resource() const
		{
			return *m_pRes;
		}

		std::size_t Position() const
		{
			return m_unPos;
		}

		void SetPosition( std::size_t p_unPos )
		{
			m_unPos = p_unPos;
		}

		void Seek( std::ptrdiff_t p_Offset )
		{
			m_unPos += p_Offset;
		}

		void Write( const void* p_pSourceBuffer, std::size_t p_unLength );

		template< typename T >
		void WritePod( const T& p_T, ENUM_ByteOrder p_eBO = EBO_NoCare )
		{
			if( p_eBO == eOtherByteOrder )
			{
				char cBuffer[ sizeof( p_T ) ];
				const char* pBegin = reinterpret_cast< const char* >( &p_T );
				std::reverse_copy( pBegin, pBegin + sizeof( p_T ), cBuffer );
				Write( cBuffer, sizeof( cBuffer ) );
			}
			else
			{
				Write( &p_T, sizeof( p_T ) );
			}
		}
	};
	//--------------------------------------------------------------------
	//--------------------------------------------------------------------
	class CResource : boost::noncopyable
	{
	public:
		virtual ~CResource()
		{

		}

		CReader FrontReader() const
		{
			return CReader( *this, 0 );
		}

		CWriter BackWrite()
		{
			return CWriter( *this, Size() );
		}

		virtual std::size_t Size() const  = 0;
		virtual void Resize( std::size_t p_unNewSize ) = 0;
		virtual void Read( std::size_t p_unOffset, std::size_t p_unLength,
			void* p_pDestBuffer ) const = 0;
		virtual void Write( std::size_t p_unOffset, std::size_t p_unLength,
			const void* p_pSourceBuffer ) = 0;
	};
	//--------------------------------------------------------------------
	//--------------------------------------------------------------------
	class CBuffer : public CResource
	{
	private:
		std::vector< char >			m_vecBuf;
	public:
		CBuffer()
		{

		}

		CBuffer( char* p_pBuffer, unsigned int p_unSize )
		{
			m_vecBuf.resize( p_unSize );
			copy( p_pBuffer, p_pBuffer + p_unSize, m_vecBuf.begin() );
		}

		CBuffer( const CBuffer& p_Other )
			: m_vecBuf( p_Other.m_vecBuf )
		{

		}

		CBuffer& operator=( const CBuffer& p_Other )
		{
			m_vecBuf = p_Other.m_vecBuf;
			return *this;
		}

		std::size_t Size() const;
		void Resize( std::size_t p_unNewSize );
		void Read( std::size_t p_unOffset, std::size_t p_unLength,
			void* p_pDestBuffer ) const;
		void Write( std::size_t p_unOffset, std::size_t p_unLength,
			const void* p_pSourceBuffer );

		const void* Data() const
		{
			return &m_vecBuf[0];
		}

		void* Data()
		{
			return &m_vecBuf[0];
		}
	};
	//--------------------------------------------------------------------
	//--------------------------------------------------------------------
	enum ENUM_FileMode
	{
		EFM_Read,			
		EFM_Replace,
		EFM_Alter,
	};
	//--------------------------------------------------------------------
	class CFile : public CResource
	{
	private:
		struct						SImpl;
		boost::scoped_ptr< SImpl >	m_pImpl;
	public:
		explicit CFile( const std::wstring& p_szFileName, ENUM_FileMode p_eMode = EFM_Read );
		~CFile();

		std::size_t Size() const;
		void Resize( std::size_t p_unNewSize );
		void Read( std::size_t p_unOffset, std::size_t p_unLength,
			void* p_pDestBuffer ) const;
		void Write( std::size_t p_unOffset, std::size_t p_unLength,
			const void* p_pSourceBuffer );
	};
	//--------------------------------------------------------------------
	//! 将一个文件读取到Buffer中
	void LoadFile( CBuffer& p_Buffer, const std::wstring& p_szFileName );
	//! 根据一段Buffer创建一个文件
	void SaveFile( const CBuffer& p_Buffer, const std::wstring& p_szFileName );
}