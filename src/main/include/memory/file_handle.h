#pragma once
#ifndef MEMORY_FILE_HANDLE_H_
#define MEMORY_FILE_HANDLE_H_

#include <iostream>
#include <windows.h>

namespace Memory
{
	class CFileHandle
	{
	public:
		HANDLE m_h;

		CFileHandle() :
			m_h( NULL )
		{
		}

		CFileHandle( HANDLE h ) :
			m_h( h )
		{
		}

		~CFileHandle()
		{
			if ( IsValid() )
			{
				CloseHandle( m_h );
				m_h = NULL;
			}
		}

		bool IsValid() const
		{
			return m_h != NULL && m_h != INVALID_HANDLE_VALUE;
		}

		operator HANDLE() const
		{
			return m_h;
		}

		CFileHandle& operator=( HANDLE h )
		{
			m_h = h;
			return *this;
		}
	};
}
#endif // MEMORY_FILE_HANDLE_H_
