#include "../../include/windows/drawing_window.h"

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

CDrawingWindow::CDrawingWindow( const CDrawingWindow& other )
{
   if ( this != &other )
   {
      m_drawingWindow = other.m_drawingWindow;
      DestroyWindow( m_drawingField );
      m_drawingField = other.m_drawingField;

      m_figures.clear();
      m_figures.resize( other.m_figures.size() );
      m_figures.assign( other.m_figures.begin(), other.m_figures.end() );
      m_currentFigure = other.m_currentFigure;

      m_isDrawing = other.m_isDrawing;
   }
}

CDrawingWindow::CDrawingWindow( CDrawingWindow&& other )
{
   if ( this != &other )
   {
      m_drawingWindow = other.m_drawingWindow;
      other.m_drawingField = nullptr;

      DestroyWindow( m_drawingField );
      m_drawingField = other.m_drawingField;
      other.m_drawingField = nullptr;

      m_figures.clear();
      m_figures.resize( other.m_figures.size() );
      m_figures.assign( other.m_figures.begin(), other.m_figures.end() );
      other.m_figures.clear();

      m_currentFigure = other.m_currentFigure;

      m_isDrawing = other.m_isDrawing;
      other.m_isDrawing = false;
   }
}

CDrawingWindow& CDrawingWindow::operator=( const CDrawingWindow& other )
{
   if ( this != &other )
   {
      CDrawingWindow temp{ other };
      std::swap( temp, *this );
   }

   return *this;
}

CDrawingWindow& CDrawingWindow::operator=( CDrawingWindow&& other )
{
   if ( this != &other )
   {
      CDrawingWindow temp{ other };
      std::swap( temp, *this );
   }

   return *this;
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
         HDC hdc = BeginPaint( hWnd, &ps );

         for ( const CFigure& figure : m_figures )
         {
            figure.draw( hdc );
         }

         m_currentFigure.draw( hdc );

         EndPaint( hWnd, &ps );
         break;
      }

      case WM_LBUTTONDOWN:
      {
         m_isDrawing = true;
         m_currentFigure.setStartPosition( static_cast< long >( LOWORD( lParam ) ),
                                           static_cast< long >( HIWORD( lParam ) ) );
         break;
      }

      case WM_MOUSEMOVE:
      {
         if ( m_isDrawing )
         {
            short divX = 1;
            short divY = 1;

            Coordinates coord     = m_currentFigure.getCoordinates();
            unsigned int PenWidth = m_currentFigure.getPenWidth();

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

            m_currentFigure.setEndPosition( static_cast< long >( LOWORD( lParam ) ),
                                            static_cast< long >( HIWORD( lParam ) ) );
         }
         break;
      }
      case WM_LBUTTONUP:
      {
         m_isDrawing = false;
         m_currentFigure.setEndPosition( static_cast< long >( LOWORD( lParam ) ),
                                         static_cast< long >( HIWORD( lParam ) ) );
         m_figures.push_back( m_currentFigure );
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
