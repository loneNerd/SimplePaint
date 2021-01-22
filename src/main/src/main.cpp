#ifndef UNICODE
#define UNICODE
#endif 

#include <algorithm>
#include <map>
#include <vector>
#include <string>
#include <utility>
#include <windows.h>
// commctrl.h shoud be compiled after windows.h 
// because in other way it cause a lot of error
#include <commctrl.h>

#include "../include/figure.h"
#include "../include/bitmap_file.h"

using std::vector;
using std::pair;
using std::wstring;

using namespace SimplePaint;

vector<pair<wstring, EFigureType>> g_figureAssociations =
{
   { L"Line",   EFigureType::eFT_Line   },
   { L"Circle", EFigureType::eFT_Circle }
};

enum EWindowElementID
{
   eWEID_SaveButtonID        = 1,
   eWEID_ComboBoxID          = 2,
   eWEID_LineWidthLabelID    = 3,
   eWEID_TextBoxID           = 4,
   eWEID_SelectColorButtonID = 5
};

HINSTANCE g_hInstance;

vector<CFigure> g_figures;
CFigure g_currentFigure;

bool g_isDrawing = false;

HWND g_hMainField;
HWND g_hToolBarField;
HWND g_hDrawingField;
HWND g_hSaveButton;
HWND g_hWndComboBox;
HWND g_hLineWidthLabel;
HWND g_hTextBox;
HWND g_hSelectColorButton;

CHOOSECOLOR g_cc;

LRESULT CALLBACK MainWindowProcesses   ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK ToolbarWindowProcesses( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK DrawingWindowProcesses( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
   g_hInstance = hInstance;

   WNDCLASS MainWindow = {};

   MainWindow.lpfnWndProc   = MainWindowProcesses;
   MainWindow.hInstance     = hInstance;
   MainWindow.lpszClassName = L"Main Window";
   MainWindow.hCursor       = LoadCursor( nullptr, IDC_ARROW );
   MainWindow.hbrBackground = ( HBRUSH )GetStockObject( GRAY_BRUSH );

   RegisterClass( &MainWindow );

   g_hMainField = CreateWindowEx( 0,
                                  L"Main Window",
                                  L"Simple Paint",
                                  WS_OVERLAPPEDWINDOW,
                                  CW_USEDEFAULT, 0, CW_USEDEFAULT, 0,
                                  nullptr,
                                  nullptr,
                                  hInstance,
                                  nullptr );
   
   if ( !g_hMainField )
      return 0;

   ShowWindow( g_hMainField, nCmdShow );
   
   WNDCLASS ToolBarWindow = {};

   ToolBarWindow.style			   = CS_HREDRAW | CS_VREDRAW;
	ToolBarWindow.lpfnWndProc	   = ToolbarWindowProcesses;
   ToolBarWindow.hInstance	      = hInstance;
	ToolBarWindow.hCursor		   = LoadCursor( nullptr, IDC_ARROW );
	ToolBarWindow.hbrBackground	= ( HBRUSH )( LTGRAY_BRUSH );
	ToolBarWindow.lpszClassName	= L"ToolBarWindow";

	RegisterClass( &ToolBarWindow );

   RECT rc {};
   GetWindowRect( g_hMainField, &rc );

   g_hToolBarField = CreateWindowEx( 0,
                                     L"ToolBarWindow",
                                     L"Toolbar Field",
                                     WS_CHILD | WS_DISABLED,
                                     0, 0, rc.right - rc.left, 50,
                                     g_hMainField,
                                     nullptr,
                                     hInstance,
                                     nullptr );
   
   if ( !g_hToolBarField )
      return 0;

   ShowWindow( g_hToolBarField, nCmdShow );

   WNDCLASS DrawingWindow = {};

	DrawingWindow.style			   = CS_HREDRAW | CS_VREDRAW;
	DrawingWindow.lpfnWndProc	   = DrawingWindowProcesses;
   DrawingWindow.hInstance	      = hInstance;
	DrawingWindow.hCursor		   = LoadCursor( nullptr, IDC_ARROW );
	DrawingWindow.hbrBackground	= ( HBRUSH )( COLOR_WINDOW + 1 );
	DrawingWindow.lpszClassName	= L"DrawingWindow";

	RegisterClass( &DrawingWindow );

  g_hDrawingField = CreateWindowEx( 0,
                                    L"DrawingWindow",
                                    L"Drawing Field",
                                    WS_CHILD | WS_CLIPCHILDREN,
                                    0, 50, 800, 600,
                                    g_hMainField,
                                    nullptr,
                                    hInstance,
                                    nullptr );
   
   if ( !g_hDrawingField )
      return 0;

   ShowWindow( g_hDrawingField, nCmdShow );

   DWORD dColors[3] = { 255, 255, 255 };

   g_cc.Flags        = CC_RGBINIT | CC_FULLOPEN;
   g_cc.hwndOwner    = g_hDrawingField;
   g_cc.lCustData    = 0L;
   g_cc.lpCustColors = dColors;
   g_cc.lStructSize  = sizeof( g_cc );
   g_cc.rgbResult    = RGB( 0, 0, 0 );

   MSG msg = {};

   while ( GetMessage( &msg, nullptr, 0, 0 ) )
   {
      TranslateMessage( &msg );
      DispatchMessage( &msg );
   }

   return 0;
}

LRESULT CALLBACK MainWindowProcesses( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
   switch ( uMsg )
   {
      case WM_PAINT:
      {
         PAINTSTRUCT ps;
         HDC hdc = BeginPaint( hWnd, &ps );

         EndPaint( hWnd, &ps );
         break;
      }
      case WM_COMMAND:
      {
         switch ( LOWORD( wParam ) )
         {
            case eWEID_SaveButtonID:
            {
               CBitmapFile file( g_hDrawingField, GetDC( g_hDrawingField ) );
               OPENFILENAME ofn;

               wchar_t szFileName[ MAX_PATH ] = L"";
               ZeroMemory( &ofn, sizeof( ofn ) );

               ofn.lStructSize = sizeof( ofn );
               ofn.hwndOwner   = g_hDrawingField;
               ofn.lpstrFilter = L"Image Files (*.bmp)\0*.bmp\0";
               ofn.lpstrFile   = szFileName;
               ofn.nMaxFile    = MAX_PATH;
               ofn.Flags       = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
               ofn.lpstrDefExt = L"bmp";

               if ( GetSaveFileName( &ofn ) )
               {
                  file.createBMPFile( szFileName );
               }
               
               break;
            }
            case eWEID_ComboBoxID:
            {
               if ( HIWORD( wParam ) == CBN_SELENDOK )
               {
                  int i = SendMessage( g_hWndComboBox, CB_GETCURSEL, 0, 0 );
                  g_currentFigure.setFigureType( g_figureAssociations[ i ].second );
               }
            }
            case eWEID_TextBoxID:
            {
               BOOL bWorked    = FALSE;
               unsigned nValue = 0;

               nValue = GetDlgItemInt( hWnd, eWEID_TextBoxID, &bWorked, TRUE );

               if ( bWorked == TRUE )
               {
                  if ( nValue == 0 )
                  {
                     MessageBox( hWnd, TEXT( "Size can't be zero" ), TEXT( "Warning" ), MB_OK | MB_ICONWARNING );
                     SetDlgItemInt( hWnd, eWEID_TextBoxID, 1u, TRUE );
                  }
                  else if ( nValue <= 32u )
                  {
                     g_currentFigure.setLineWidth( nValue );
                  }
                  else
                  {
                     MessageBox( hWnd, TEXT( "Too large value, maximum 32" ), TEXT( "Warning" ), MB_OK | MB_ICONWARNING );
                     SetDlgItemInt( hWnd, eWEID_TextBoxID, 32u, TRUE );
                  }
                  

               }
               break;
            }

            case eWEID_SelectColorButtonID:
            {
               if ( ChooseColor( &g_cc ) ) 
               {
                  g_currentFigure.setColor( ( COLORREF )g_cc.rgbResult );
               }

               break;
            }
         }
         break;
      }

      case WM_SIZE:
      {
         RECT rc {};
         GetWindowRect( g_hMainField, &rc );
         SetWindowPos( g_hToolBarField, nullptr, 0, 0, rc.right - rc.left, 50, 0 );
         break;
      }

      case WM_DESTROY:
      {
         PostQuitMessage( 0 );
         break;
      }

   }
   return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

LRESULT CALLBACK ToolbarWindowProcesses( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) 
{
   switch ( uMsg )
   {
      case WM_CREATE:
      {
         g_hSaveButton = CreateWindow( WC_BUTTON, 
                                      TEXT( "Save" ),
                                      WS_VISIBLE | WS_CHILD | WS_BORDER,
                                      10, 10, 50, 25,
                                      g_hMainField,
                                      ( HMENU )eWEID_SaveButtonID,
                                      nullptr,
                                      nullptr );

         g_hWndComboBox = CreateWindow( WC_COMBOBOX, 
                                       TEXT( "" ), 
                                       CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
                                       75, 10, 150, 200, 
                                       g_hMainField, 
                                       ( HMENU )eWEID_ComboBoxID, 
                                       nullptr,
                                       nullptr );

         for ( const auto& figure : g_figureAssociations )
         {
            SendMessage( g_hWndComboBox, ( UINT )CB_ADDSTRING, ( WPARAM ) 0, ( LPARAM )figure.first.c_str() ); 
         }

         SendMessage( g_hWndComboBox, CB_SETCURSEL, ( WPARAM )0, ( LPARAM )0 );

         g_hLineWidthLabel = CreateWindow( WC_STATIC,
                                          TEXT( "Line\nWidth" ),
                                          WS_CHILD | WS_VISIBLE,
                                          250, 8, 45, 30,
                                          g_hMainField,
                                          ( HMENU )eWEID_LineWidthLabelID,
                                          nullptr,
                                          nullptr );

         g_hTextBox = CreateWindow( WC_EDIT,
                                   TEXT( "1" ),
                                   WS_BORDER | WS_CHILD | WS_VISIBLE | ES_NUMBER,
                                   300, 10, 50, 25, 
                                   g_hMainField,
                                   ( HMENU )eWEID_TextBoxID,
                                   nullptr,
                                   nullptr);

         g_hSelectColorButton = CreateWindow( WC_BUTTON, 
                                              TEXT( "Select Color" ),
                                              WS_VISIBLE | WS_CHILD | WS_BORDER,
                                              375, 10, 100, 25,
                                              g_hMainField,
                                              ( HMENU )eWEID_SelectColorButtonID,
                                              nullptr,
                                              nullptr );
         
         break;
      }
      
      case WM_PAINT:
      {
         PAINTSTRUCT ps;
         HDC hdc = BeginPaint( hWnd, &ps );

         EndPaint( hWnd, &ps );
         break;
      }
      case WM_DESTROY:
      {
         PostQuitMessage( 0 );
         break;
      }
   }
   return DefWindowProc( hWnd, uMsg, wParam, lParam );
   
}

LRESULT CALLBACK DrawingWindowProcesses( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) 
{
   switch ( uMsg )
	{
	   case WM_PAINT:
      {
         PAINTSTRUCT ps;
         HDC hdc = BeginPaint( hWnd, &ps );

         for ( const CFigure& figure : g_figures )
         {
            figure.draw( hdc );
         }

         g_currentFigure.draw( hdc );

         EndPaint( hWnd, &ps );
         break;
      }
     
      case WM_LBUTTONDOWN:
      {
         g_isDrawing = true;
         g_currentFigure.setStartPosition( static_cast<long>( LOWORD( lParam ) ), 
                                           static_cast<long>( HIWORD( lParam ) ) );
         break;
      }

      case WM_MOUSEMOVE:
      {
         if ( g_isDrawing )
         {
            short divX = 1;
            short divY = 1;
            Coordinates coord = g_currentFigure.getCoordinates();
            unsigned int PenWidth = g_currentFigure.getPenWidth();

            if ( coord.StartX > coord.EndX )
            {
               divX = -divX;
            }

            if ( coord.StartY > coord.EndY )
            {
               divY = -divY;
            }
            
            InvalidateRgn( hWnd,
                           CreateRectRgn( coord.StartX - divX * PenWidth,
                                          coord.StartY - divY * PenWidth,
                                          coord.EndX   + divX * PenWidth,
                                          coord.EndY   + divY * PenWidth ),
                           true );

            g_currentFigure.setEndPosition( static_cast<long>( LOWORD( lParam ) ), 
                                            static_cast<long>( HIWORD( lParam ) ) );
         }
         break;
      }
      case WM_LBUTTONUP:
      {
         g_isDrawing = false;
         g_currentFigure.setEndPosition( static_cast<long>( LOWORD( lParam ) ), 
                                         static_cast<long>( HIWORD( lParam ) ) );
         g_figures.push_back( g_currentFigure );
         InvalidateRect( hWnd, nullptr, false );
         break;
      }
      
      case WM_DESTROY:
      {
         PostQuitMessage( 0 );
         break;
      }
   }
   return DefWindowProc( hWnd, uMsg, wParam, lParam );
}
