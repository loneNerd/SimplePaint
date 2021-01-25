#pragma once
#ifndef WINDOWS_DRAWING_WINDOW_H_
#define WINDOWS_DRAWING_WINDOW_H_

#include <vector>
#include <string>
#include <utility>
#include <windows.h>
// commctrl.h shoud be compiled after windows.h 
// because in other way it cause a lot of error
#include <commctrl.h>

#include "../bitmap_file.h"
#include "../figure.h"

namespace Windows
{
   using std::vector;
   using std::pair;
   using std::wstring;
   using SimplePaint::CFigure;
   using SimplePaint::EFigureType;
   using SimplePaint::Coordinates;

   class CDrawingWindow
   {
   public:
      explicit CDrawingWindow( HINSTANCE hInstance, HWND parent, int nCmdShow );

      CDrawingWindow( const CDrawingWindow& other );
      CDrawingWindow( CDrawingWindow&& other );

      CDrawingWindow& operator=( const CDrawingWindow& other );
      CDrawingWindow& operator=( CDrawingWindow&& other );

      ~CDrawingWindow();

      LRESULT CALLBACK processes( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

      CFigure& getCurrentFigure() { return m_currentFigure; }
      HWND     getHandler()       { return m_drawingField; }
      
   private:
      static LRESULT CALLBACK s_processes( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
      
      WNDCLASS m_drawingWindow;
      HWND     m_drawingField;

      vector< CFigure > m_figures;
      CFigure m_currentFigure;

      bool m_isDrawing = false;
   };
};

#endif // WINDOWS_DRAWING_WINDOW_H_