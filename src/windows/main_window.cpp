#include "main_window.h"

using namespace Windows;

CMainWindow::CMainWindow( HINSTANCE hInstance, int nCmdShow )
{
   m_mainWindow.lpfnWndProc   = CMainWindow::s_processes;
   m_mainWindow.hInstance     = hInstance;
   m_mainWindow.lpszClassName = L"Main Window";
   m_mainWindow.hCursor       = LoadCursor( nullptr, IDC_ARROW );
   m_mainWindow.hbrBackground = ( HBRUSH )GetStockObject( GRAY_BRUSH );

   RegisterClass( &m_mainWindow );

   m_mainField = CreateWindowEx( 0,
                                 L"Main Window",
                                 L"Simple Paint",
                                 WS_OVERLAPPEDWINDOW,
                                 CW_USEDEFAULT, 0, CW_USEDEFAULT, 0,
                                 nullptr,
                                 nullptr,
                                 hInstance,
                                 this );

   toolBarWindow = std::unique_ptr< CToolBarWindow >( new CToolBarWindow( hInstance, m_mainField, nCmdShow ) );
   drawingWindow = std::unique_ptr< CDrawingWindow >( new CDrawingWindow( hInstance, m_mainField, nCmdShow ) );

   ShowWindow( m_mainField, nCmdShow );

   m_saveButton = CreateWindow( WC_BUTTON,
                                TEXT( "Save" ),
                                WS_VISIBLE | WS_CHILD | WS_BORDER,
                                10, 10, 50, 25,
                                m_mainField,
                                ( HMENU )eWEID_SaveButtonID,
                                nullptr,
                                this );

   m_wndComboBox = CreateWindow( WC_COMBOBOX,
                                 TEXT( "" ),
                                 CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
                                 75, 10, 150, 200,
                                 m_mainField,
                                 ( HMENU )eWEID_ComboBoxID,
                                 nullptr,
                                 this );

   for ( const auto& figure : m_figureAssociations )
   {
      SendMessage( m_wndComboBox, ( UINT )CB_ADDSTRING, ( WPARAM )0, ( LPARAM )figure.first.c_str() );
   }

   SendMessage( m_wndComboBox, CB_SETCURSEL, ( WPARAM )0, ( LPARAM )0 );

   m_lineWidthLabel = CreateWindow( WC_STATIC,
                                    TEXT( "Line\nWidth" ),
                                    WS_CHILD | WS_VISIBLE,
                                    250, 8, 45, 30,
                                    m_mainField,
                                    ( HMENU )eWEID_LineWidthLabelID,
                                    nullptr,
                                    this );

   m_textBox = CreateWindow( WC_EDIT,
                             TEXT( "1" ),
                             WS_BORDER | WS_CHILD | WS_VISIBLE | ES_NUMBER,
                             300, 10, 50, 25,
                             m_mainField,
                             ( HMENU )eWEID_TextBoxID,
                             nullptr,
                             this );

   m_selectColorButton = CreateWindow( WC_BUTTON,
                                       TEXT( "Select Color" ),
                                       WS_VISIBLE | WS_CHILD | WS_BORDER,
                                       375, 10, 100, 25,
                                       m_mainField,
                                       ( HMENU )eWEID_SelectColorButtonID,
                                       nullptr,
                                       this );

   DWORD dColors[ 3 ] = { 255, 222, 222 };

   m_cc.Flags        = CC_RGBINIT | CC_FULLOPEN;
   m_cc.hwndOwner    = nullptr;
   m_cc.lCustData    = reinterpret_cast< LPARAM >( this );
   m_cc.lpCustColors = dColors;
   m_cc.lStructSize  = sizeof( m_cc );
   m_cc.rgbResult    = RGB( 255, 222, 222 );
}

CMainWindow::~CMainWindow()
{
   DestroyWindow( m_mainField );
   DestroyWindow( m_lineWidthLabel );
   DestroyWindow( m_saveButton );
   DestroyWindow( m_selectColorButton );
   DestroyWindow( m_textBox );
   DestroyWindow( m_wndComboBox );
}

LRESULT CMainWindow::s_processes( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
   CMainWindow* pThis;

   if ( uMsg == WM_NCCREATE )
   {
      pThis = static_cast< CMainWindow* >( reinterpret_cast< CREATESTRUCT* >( lParam )->lpCreateParams );

      SetLastError( 0 );
      if ( !SetWindowLongPtr( hWnd, GWL_USERDATA, reinterpret_cast< LONG_PTR >( pThis ) ) )
      {
         if ( GetLastError() != 0 )
            return FALSE;
      }
   }
   else
   {
      pThis = reinterpret_cast< CMainWindow* >( GetWindowLongPtr( hWnd, GWL_USERDATA ) );
   }

   if ( pThis )
   {
      return pThis->processes( hWnd, uMsg, wParam, lParam );
   }

   return FALSE;
}

LRESULT CALLBACK CMainWindow::processes( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
   switch ( uMsg )
   {
      case WM_COMMAND:
      {
         switch ( LOWORD( wParam ) )
         {
            case eWEID_SaveButtonID:
            {
               HWND drawingHandler = drawingWindow->getHandler();
               SimplePaint::CBitmapFile file( drawingHandler, GetDC( drawingHandler ) );
               OPENFILENAME ofn;

               wchar_t szFileName[ MAX_PATH ] = L"";
               ZeroMemory( &ofn, sizeof( ofn ) );

               ofn.lStructSize = sizeof( ofn );
               ofn.hwndOwner   = drawingHandler;
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
                  int i = SendMessage( m_wndComboBox, CB_GETCURSEL, 0, 0 );
                  drawingWindow->switchFigureType( m_figureAssociations[ i ].second );
               }
            }
            case eWEID_TextBoxID:
            {
               BOOL bWorked    = FALSE;
               unsigned nValue = GetDlgItemInt( hWnd, eWEID_TextBoxID, &bWorked, TRUE );

               if ( bWorked == TRUE )
               {
                  if ( nValue == 0 )
                  {
                     MessageBox( hWnd, TEXT( "Size can't be zero" ), TEXT( "Warning" ), MB_OK | MB_ICONWARNING );
                     SetDlgItemInt( hWnd, eWEID_TextBoxID, 1u, TRUE );
                  }
                  else if ( nValue <= 32u )
                  {
                     drawingWindow->setCurrentPenWidth( nValue );
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
               if ( ChooseColor( &m_cc ) )
               {
                  drawingWindow->setCurrentColor( ( COLORREF )m_cc.rgbResult );
               }

               break;
            }
         }
         break;
      }
      case WM_SIZE:
      {
         RECT rc {};
         GetWindowRect( m_mainField, &rc );
         SetWindowPos( toolBarWindow->getHandler(), nullptr, 0, 0, rc.right - rc.left, 50, 0 );
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
