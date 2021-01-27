#ifndef UNICODE
#define UNICODE
#endif 

#include <memory>
#include <windows.h>
// commctrl.h shoud be compiled after windows.h 
// because in other way it cause a lot of error
#include <commctrl.h>

#include "windows/main_window.h"

using namespace Windows;

int APIENTRY wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
   CMainWindow mainWindow( hInstance, nCmdShow );

   MSG msg {};

   while ( GetMessage( &msg, nullptr, 0, 0 ) )
   {
      TranslateMessage( &msg );
      DispatchMessage( &msg );
   }

   return 0;
}
