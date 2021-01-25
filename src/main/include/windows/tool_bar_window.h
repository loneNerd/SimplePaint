#pragma once
#ifndef WINDOWS_TOOL_BAR_WINDOW_H_
#define WINDOWS_TOOL_BAR_WINDOW_H_

#include <iostream>
#include <windows.h>
// commctrl.h shoud be compiled after windows.h 
// because in other way it cause a lot of error
#include <commctrl.h>

namespace Windows
{ 
   class CToolBarWindow
   {
   public:
      explicit CToolBarWindow( HINSTANCE hInstance, HWND parent, int nCmdShow );

      CToolBarWindow( const CToolBarWindow& other );
      CToolBarWindow( CToolBarWindow&& other );
      CToolBarWindow& operator=( const CToolBarWindow& other );
      CToolBarWindow& operator=( CToolBarWindow&& other) noexcept;
      ~CToolBarWindow();

      LRESULT CALLBACK processes( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

      HWND getHandler() { return m_toolBar; }
      
   private:
      static LRESULT CALLBACK s_processes( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

      WNDCLASS  m_toolBarWindow;
      HWND      m_toolBar;
   };
};

#endif // WINDOWS_TOOL_BAR_WINDOW_H_