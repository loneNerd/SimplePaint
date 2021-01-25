#pragma once
#ifndef WINDOWS_MAIN_WINDOW_H_
#define WINDOWS_MAIN_WINDOW_H_

#include <memory>
#include <vector>
#include <string>
#include <utility>
#include <windows.h>
// commctrl.h shoud be compiled after windows.h 
// because in other way it cause a lot of error
#include <commctrl.h>

#include "drawing_window.h"
#include "tool_bar_window.h"
#include "../bitmap_file.h"
#include "../figure.h"

namespace Windows
{
   enum EWindowElementID
   {
      eWEID_SaveButtonID        = 1,
      eWEID_ComboBoxID          = 2,
      eWEID_LineWidthLabelID    = 3,
      eWEID_TextBoxID           = 4,
      eWEID_SelectColorButtonID = 5
   };

   using std::vector;
   using std::pair;
   using std::wstring;
   using SimplePaint::CFigure;
   using SimplePaint::EFigureType;

   class CMainWindow
   {
   public:
      explicit CMainWindow( HINSTANCE hInstance, int nCmdShow );
      CMainWindow( const CMainWindow& other )            = delete;
      CMainWindow( CMainWindow&& other )                 = delete;
      CMainWindow& operator=( const CMainWindow& other ) = delete;
      CMainWindow& operator=( CMainWindow&& other )      = delete;
      ~CMainWindow();

      LRESULT CALLBACK processes( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

      HWND getMainWindow() const { return m_mainField; }
      
   private:
      static LRESULT CALLBACK s_processes( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
      std::unique_ptr< CDrawingWindow > drawingWindow;
      std::unique_ptr< CToolBarWindow > toolBarWindow;

      WNDCLASS    m_mainWindow;
      CHOOSECOLOR m_cc;
      HWND        m_lineWidthLabel;
      HWND        m_mainField;
      HWND        m_saveButton;
      HWND        m_selectColorButton;
      HWND        m_textBox;
      HWND        m_wndComboBox;
      
      vector< pair< wstring, EFigureType > > m_figureAssociations =
      {
         { L"Line",   EFigureType::eFT_Line   },
         { L"Circle", EFigureType::eFT_Circle }
      };
   };
};

#endif // WINDOWS_MAIN_WINDOW_H_
