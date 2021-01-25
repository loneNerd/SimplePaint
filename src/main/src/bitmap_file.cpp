#include "../include/bitmap_file.h"

using namespace SimplePaint;

CBitmapFile::CBitmapFile( HWND hwnd, HDC hdc )
{
   m_window     = hwnd;
   m_descriptor = hdc;
   m_hTargetDC  = CreateCompatibleDC( hdc );
   RECT rect    = { 0 };

   GetClientRect( hwnd, &rect );

   m_hBitmap = CreateCompatibleBitmap( hdc, rect.right - rect.left, rect.bottom - rect.top );
   SelectObject( m_hTargetDC, m_hBitmap );
   PrintWindow( hwnd, m_hTargetDC, PW_CLIENTONLY );
}

CBitmapFile::~CBitmapFile()
{
   DeleteObject( m_hBitmap );
   ReleaseDC( m_window, m_descriptor );
   DeleteDC( m_hTargetDC );
}

PBITMAPINFO CBitmapFile::createBitmapInfoStruct()
{ 
   BITMAP      bmp; 
   PBITMAPINFO pbmi; 
   WORD        cClrBits; 
 
   if ( !GetObject( m_hBitmap, sizeof( BITMAP ), &bmp ) ) 
      return 0; 

   cClrBits = static_cast< WORD >( bmp.bmPlanes * bmp.bmBitsPixel ); 

   if ( cClrBits == 1 ) 
   {
      cClrBits = 1;
   }
   else if ( cClrBits <= 4 )
   {
      cClrBits = 4; 
   }
   else if ( cClrBits <= 8 )
   {
      cClrBits = 8;
   }
   else if ( cClrBits <= 16 )
   {
      cClrBits = 16;
   }
   else if ( cClrBits <= 24 )
   {
      cClrBits = 24;
   }
   else 
   {
      cClrBits = 32;
   }

   if ( cClrBits < 24 )
   {
      pbmi = ( PBITMAPINFO ) LocalAlloc( LPTR, 
                                         sizeof( BITMAPINFOHEADER ) + 
                                         sizeof( RGBQUAD ) * ( 1 << cClrBits ) );
   }
   else
   {
      pbmi = ( PBITMAPINFO ) LocalAlloc( LPTR, sizeof( BITMAPINFOHEADER ) );
   }

   pbmi->bmiHeader.biSize     = sizeof( BITMAPINFOHEADER );
   pbmi->bmiHeader.biWidth    = bmp.bmWidth;
   pbmi->bmiHeader.biHeight   = bmp.bmHeight;
   pbmi->bmiHeader.biPlanes   = bmp.bmPlanes;
   pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;

   if ( cClrBits < 24 )
   {
      pbmi->bmiHeader.biClrUsed = ( 1 << cClrBits );
   }

   pbmi->bmiHeader.biCompression = BI_RGB;
   pbmi->bmiHeader.biSizeImage = ( ( pbmi->bmiHeader.biWidth * cClrBits + 31 ) & ~31 ) / 8 * pbmi->bmiHeader.biHeight;
   pbmi->bmiHeader.biClrImportant = 0;

   return pbmi;
} 

void CBitmapFile::createBMPFile( LPCWSTR name ) 
{ 
   HANDLE            hf;
   BITMAPFILEHEADER  hdr;
   PBITMAPINFOHEADER pbih;
   LPBYTE            lpBits;
   DWORD             dwTotal;
   DWORD             cb;
   BYTE              *hp;
   DWORD             dwTmp;

   PBITMAPINFO pbi = createBitmapInfoStruct();

   pbih   = ( PBITMAPINFOHEADER )pbi;
   lpBits = ( LPBYTE ) GlobalAlloc( GMEM_FIXED, pbih->biSizeImage );

   if ( !lpBits ) 
      return;
 
   if ( !GetDIBits( m_descriptor, m_hBitmap, 0, ( WORD )pbih->biHeight, lpBits, pbi, DIB_RGB_COLORS ) ) 
   {
      return;
   }

   hf = CreateFileW( name, 
                     GENERIC_READ | GENERIC_WRITE, 
                     ( DWORD ) 0, 
                     nullptr, 
                     CREATE_ALWAYS, 
                     FILE_ATTRIBUTE_NORMAL, 
                     ( HANDLE ) nullptr );

   if ( hf == INVALID_HANDLE_VALUE ) 
      return;
   
   hdr.bfType = 0x4d42;
   hdr.bfSize = ( DWORD )( sizeof( BITMAPFILEHEADER ) + pbih->biSize + pbih->biClrUsed *
                           sizeof( RGBQUAD )          + pbih->biSizeImage ); 
   hdr.bfReserved1 = 0; 
   hdr.bfReserved2 = 0; 

   hdr.bfOffBits = ( DWORD ) sizeof( BITMAPFILEHEADER ) + 
                    pbih->biSize + pbih->biClrUsed *
                    sizeof ( RGBQUAD ); 

   if ( !WriteFile( hf, ( LPVOID ) &hdr, sizeof( BITMAPFILEHEADER ), ( LPDWORD ) &dwTmp,  nullptr ) ) 
   {
      return;
   }

   if ( !WriteFile( hf, 
                    ( LPVOID ) pbih, 
                    sizeof( BITMAPINFOHEADER ) + pbih->biClrUsed * sizeof ( RGBQUAD ),
                    ( LPDWORD ) &dwTmp, 
                    ( nullptr ) ) )
      return; 

   dwTotal = cb = pbih->biSizeImage;
   hp = lpBits;

   if ( !WriteFile( hf, ( LPSTR ) hp, ( int ) cb, ( LPDWORD )&dwTmp, nullptr ) ) 
      return; 

   if ( !CloseHandle( hf ) )
      return; 

   GlobalFree( ( HGLOBAL )lpBits );
}
