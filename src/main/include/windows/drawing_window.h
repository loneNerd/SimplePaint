#pragma once
#ifndef WINDOWS_DRAWING_WINDOW_H_
#define WINDOWS_DRAWING_WINDOW_H_

#include <list>
#include <string>
#include <utility>
#include <windows.h>
// commctrl.h shoud be compiled after windows.h 
// because in other way it cause a lot of error
#include <commctrl.h>

#include "../bitmap_file.h"
#include "../figures/figure.h"
#include "../figures/figure_manager.h"
#include "../figures/line_figure.h"
#include "../figures/circle_figure.h"

namespace Windows
{
   using std::list;
   using std::pair;
   using std::wstring;
   using Figures::CFigureManager;
   using Figures::CFigure;
   using Figures::CLineFigure;
   using Figures::CCircleFigure;
   using Figures::EFigureType;
   using Figures::Coordinates;

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

      void setCurrentColor( COLORREF color )    { m_currentColor = color; }
      void setCurrentPenWidth( unsigned width ) { m_currentWidth = width; }
      void switchFigureType( EFigureType type ) { m_currentFigureType = type; }

      HWND getHandler() { return m_drawingField; }

   private:
      void setNewCurrentFigure();
      static LRESULT CALLBACK s_processes( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
      
      WNDCLASS m_drawingWindow;
      HWND     m_drawingField;

      CFigureManager m_figureManager;
      std::shared_ptr< CFigure > m_currentFigure = nullptr;

      unsigned m_currentWidth = 1;
      COLORREF m_currentColor = RGB( 0, 0, 0 );
      EFigureType m_currentFigureType = EFigureType::eFT_Line;

      bool m_isDrawing = false;
   };
};

#endif // WINDOWS_DRAWING_WINDOW_H_
