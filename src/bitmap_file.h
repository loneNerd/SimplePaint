#pragma once
#ifndef BITMAP_H_
#define BITMAP_H_

#include <iostream>
#include <windows.h>
#include "memory/file_handle.h"
#include "memory/byte_handle.h"
#include "memory/bitmapinfo_handle.h"

namespace SimplePaint
{
   class CBitmapFile
   {
   public:
      explicit CBitmapFile( HWND hwnd, HDC hdc );

      CBitmapFile( const CBitmapFile& other )            = delete;
      CBitmapFile( CBitmapFile&& other )                 = delete;
      CBitmapFile& operator=( const CBitmapFile& other ) = delete;
      CBitmapFile& operator=( CBitmapFile&& other )      = delete;

      ~CBitmapFile();
      void createBMPFile( LPCWSTR name );
      PBITMAPINFO createBitmapInfoStruct();

   private:
      HBITMAP m_hBitmap;
      HWND    m_window;
      HDC     m_descriptor;
      HDC     m_hTargetDC;
   };
};

#endif // BITMAP_H_
