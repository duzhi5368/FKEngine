/********************************************************************
*
*	本文件是FKMMORPG的一部分。本文件版权归属FreeKnightDuzhi(王宏张)所有。
*	E-mail: duzhi5368@163.com
*	QQ: 	281862942
* 	
*	-----------------------------------------------------------------
*
*	文件名：	Singleton
*	作者：		FreeKnightDuzhi[ 王宏张 ]
*	文件说明：	
*
*	修改人:		
*	修改内容:
*********************************************************************/

#pragma once

//--------------------------------------------------------------------
#include "../../depend/boost/noncopyable.hpp"
#include <memory>
#include <assert.h>
#include <Windows.h>
//--------------------------------------------------------------------
namespace FK2DEngine
{
#if 0
	//--------------------------------------------------------------------
	template< typename T >
	class TSingleton : private boost::noncopyable
	{
	public:
		static T* _singleton;
	public:
		TSingleton()
		{
			assert( !_singleton );
			#if defined( _MSC_VER ) && _MSC_VER < 1200
				int offset = (int)(T*)1 - (int)(Singleton <T>*)(T*)1;
				_singleton = (T*)((int)this + offset);
			#else
				_singleton = static_cast< T* >( this );
			#endif
		}

		virtual ~TSingleton()
		{
			assert(_singleton);
			_singleton = NULL;
		}

		static T& GetSingleton()
		{
			assert(_singleton);
			return(*_singleton);
		}

		static T* GetSingletonPtr()
		{
			return (_singleton);
		}
	};
	//--------------------------------------------------------------------
	template< typename T >T* TSingleton< T >::_singleton	= NULL;
	//--------------------------------------------------------------------
#endif

	//--------------------------------------------------------------------
	class CResGuard
	{
	public:
		CResGuard()
		{
			m_lGrdCnt = 0;
			InitializeCriticalSection( &m_CS );
		}
		~CResGuard()
		{
			DeleteCriticalSection( &m_CS );
		}
		bool IsGuarded() const
		{
			return ( m_lGrdCnt > 0 );
		}
	public:
		class CGuard
		{
		public:
			CGuard( CResGuard& rg ) 
				: m_Rg( rg )
			{
				m_Rg.Guard();
			}
			~CGuard()
			{
				m_Rg.Unguard();
			}
		private:
			CResGuard& m_Rg;
		};
	private:
		friend class CResGuard::CGuard;
	private:
		void Guard()
		{
			EnterCriticalSection( &m_CS );
			m_lGrdCnt++;
		}

		void Unguard()
		{
			m_lGrdCnt--;
			LeaveCriticalSection( &m_CS );
		}
	private:
		CRITICAL_SECTION		m_CS;
		long					m_lGrdCnt;
	};
	//--------------------------------------------------------------------
	template< typename T >
	class TSingleton : private boost::noncopyable
	{
	public:
		static inline T* Instance();
	private:
		TSingleton()
		{

		}
		virtual ~TSingleton()
		{

		}
	
		static std::auto_ptr< T >	_instance;
		static CResGuard			_rs;
	};

	template< typename T >	std::auto_ptr< T >	TSingleton< T >::_instance;
	template< typename T >	CResGuard			TSingleton< T >::_rs;

	template< typename T >
	inline T* TSingleton< T >::Instance()
	{
		if( 0 == _instance.get() )
		{
			CResGuard::CGuard gd( _rs );
			if( 0 == _instance.get() )
			{
				_instance.reset( new T );
			}
		}

		return _instance.get();
	}

	#define DECLARE_SINGLETON_CLASS( type )	\
		friend class std::auto_ptr< type >;	\
		friend class TSingleton< type >;
}