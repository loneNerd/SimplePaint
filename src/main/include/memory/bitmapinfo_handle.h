#pragma once
#ifndef MEMORY_BITMAPINFO_HANDLE_H_
#define MEMORY_BITMAPINFO_HANDLE_H_

#include <iostream>
#include <windows.h>

namespace Memory
{
   class CBitmapInfoHandle
   {
	public:
		PBITMAPINFO m_h;

		CBitmapInfoHandle() :
			m_h(NULL)
		{
		}

		CBitmapInfoHandle( PBITMAPINFO h ) :
			m_h(h)
		{
		}

		~CBitmapInfoHandle()
		{
			if ( m_h != NULL )
				LocalFree( ( HLOCAL )m_h );
		}

		operator PBITMAPINFO() const
		{
			return m_h;
		}

		operator PBITMAPINFOHEADER() const
		{
			return ( PBITMAPINFOHEADER )m_h;
		}

		CBitmapInfoHandle& operator=( PBITMAPINFO h )
		{
			if ( m_h != NULL )
				LocalFree( ( HLOCAL ) m_h );

			m_h = h;
			return *this;
		}
   };
}

#endif // MEMORY_BITMAPINFO_HANDLE_H_