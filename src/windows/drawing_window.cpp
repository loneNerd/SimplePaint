#include "drawing_window.h"

using namespace Windows;

CDrawingWindow::CDrawingWindow( HINSTANCE hInstance, HWND parent, int nCmdShow )
{
   m_drawingWindow.style         = CS_HREDRAW | CS_VREDRAW;
   m_drawingWindow.lpfnWndProc   = CDrawingWindow::s_processes;
   m_drawingWindow.hInstance     = hInstance;
   m_drawingWindow.hCursor       = LoadCursor( nullptr, IDC_ARROW );
   m_drawingWindow.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
   m_drawingWindow.lpszClassName = L"DrawingWindow";

   RegisterClass( &m_drawingWindow );

   m_drawingField = CreateWindowEx( 0,
                                    L"DrawingWindow",
                                    L"Drawing Field",
                                    WS_CHILD | WS_CLIPCHILDREN,
                                    0, 50, 800, 600,
                                    parent,
                                    nullptr,
                                    hInstance,
                                    this );

   ShowWindow( m_drawingField, nCmdShow );
}

CDrawingWindow::~CDrawingWindow()
{
   DestroyWindow( m_drawingField );
}

LRESULT CDrawingWindow::s_processes( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
   CDrawingWindow* pThis;

   if (uMsg == WM_NCCREATE)
   {
      pThis = static_cast< CDrawingWindow* >( reinterpret_cast< CREATESTRUCT* > (lParam )->lpCreateParams );

      SetLastError( 0 );
      if ( !SetWindowLongPtr( hWnd, GWL_USERDATA, reinterpret_cast< LONG_PTR >( pThis ) ) )
      {
         if ( GetLastError() != 0 )
            return FALSE;
      }
   }
   else
   {
      pThis = reinterpret_cast< CDrawingWindow* >( GetWindowLongPtr( hWnd, GWL_USERDATA ) );
   }

   if ( pThis )
   {
      return pThis->processes( hWnd, uMsg, wParam, lParam );
   }

   return FALSE;
}

LRESULT CALLBACK CDrawingWindow::processes( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
   

   switch ( uMsg )
   {
      case WM_PAINT:
      {
         PAINTSTRUCT ps;

         HDC hdc        = BeginPaint( hWnd, &ps );
         HDC hdcMem     = CreateCompatibleDC( hdc );
         HBITMAP hbmMem = CreateCompatibleBitmap( hdc, 800, 600 );
         HANDLE hOld    = SelectObject( hdcMem, hbmMem );

         RECT rc;
         SetRect( &rc, 0, 0, 800, 600 );
         FillRect( hdcMem, &rc, ( HBRUSH )GetStockObject( WHITE_BRUSH ) );

         m_figureManager.update( hdcMem );

         if ( m_currentFigure != nullptr )
         {
            m_currentFigure->draw( hdcMem );
         }

         BitBlt( hdc, 0, 0, 800, 600, hdcMem, 0, 0, SRCCOPY );

         SelectObject( hdcMem, hOld );
         DeleteObject( hbmMem );
         DeleteDC( hdcMem );

         EndPaint( hWnd, &ps );

         break;
      }
      case WM_LBUTTONDOWN:
      {
         m_isDrawing = true;
         m_currentFigure = setNewCurrentFigure();
         m_currentFigure->setStartPosition( static_cast< long >( LOWORD( lParam ) ),
                                            static_cast< long >( HIWORD( lParam ) ) );
         break;
      }
      case WM_MOUSEMOVE:
      {
         if ( m_isDrawing )
         {
            InvalidateRect(hWnd, nullptr, false);

            m_currentFigure->setEndPosition( static_cast< long >( LOWORD( lParam ) ),
                                             static_cast< long >( HIWORD( lParam ) ) );
         }
         break;
      }
      case WM_LBUTTONUP:
      {
         m_isDrawing = false;
         m_currentFigure->setEndPosition( static_cast< long >( LOWORD( lParam ) ),
                                          static_cast< long >( HIWORD( lParam ) ) );
         m_figureManager.addFigure( m_currentFigure );
         m_currentFigure = nullptr;
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

std::shared_ptr< IFigure > CDrawingWindow::setNewCurrentFigure()
{
   switch ( m_currentFigureType )
   {
      case EFigureType::eFT_Circle:
      {
         return std::make_shared< CCircleFigure >( 0, 0, 0, 0, m_currentWidth, m_currentColor );
      }
      case EFigureType::eFT_Line:
      default:
      {
         return std::make_shared< CLineFigure >( 0, 0, 0, 0, m_currentWidth, m_currentColor );
      }
   }
}
