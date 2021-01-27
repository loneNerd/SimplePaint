#pragma once
#ifndef MEMORY_BYTE_HANDLE_H_
#define MEMORY_BYTE_HANDLE_H_

#include <iostream>
#include <windows.h>

namespace Memory
{
	class CByteHandle
	{
	public:
		LPBYTE m_h;

		CByteHandle() :
			m_h( NULL )
		{
		}

		CByteHandle( LPBYTE h ) :
			m_h( h )
		{
		}

		~CByteHandle()
		{
			GlobalFree( ( HGLOBAL ) m_h );
		}

		operator LPBYTE() const
		{
			return m_h;
		}

		CByteHandle& operator=( LPBYTE h )
		{
			if ( m_h != NULL )
				GlobalFree( ( HGLOBAL )m_h );

			m_h = h;
			return *this;
		}
	};
}
#endif // MEMORY_BYTE_HANDLE_H_
