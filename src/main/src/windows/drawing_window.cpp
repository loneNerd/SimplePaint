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
      m_currentWidth = other.m_currentWidth;
      m_currentColor = other.m_currentColor;
      m_currentFigureType = other.m_currentFigureType;

      m_currentFigure = nullptr;
      setNewCurrentFigure();

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

      m_currentWidth = other.m_currentWidth;
      m_currentColor = other.m_currentColor;
      m_currentFigureType = other.m_currentFigureType;

      other.m_currentWidth = 1;
      other.m_currentColor = 0;
      other.m_currentFigureType = EFigureType::eFT_Line; 

      m_currentFigure = std::move( other.m_currentFigure );
      other.m_currentFigure = nullptr;
      
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
         
         if ( hdc == NULL )
         {
            SetLastError( 0 );
         }

         m_figureManager.update( hdc );

         if ( m_currentFigure != nullptr )
         {
            m_currentFigure->draw( hdc );
         }

         EndPaint( hWnd, &ps );
         break;
      }
      case WM_LBUTTONDOWN:
      {
         m_isDrawing = true;
         setNewCurrentFigure();
         m_currentFigure->setStartPosition( static_cast< long >( LOWORD( lParam ) ),
                                            static_cast< long >( HIWORD( lParam ) ) );
         break;
      }
      case WM_MOUSEMOVE:
      {
         if ( m_isDrawing )
         {
            short divX = 1;
            short divY = 1;

            Coordinates coord      = m_currentFigure->getCoordinates();
            unsigned int LineWidth = m_currentFigure->getLineWidth();

            if ( coord.StartX > coord.EndX )
            {
               divX = -divX;
            }

            if ( coord.StartY > coord.EndY )
            {
               divY = -divY;
            }

            InvalidateRgn( hWnd,
                           CreateRectRgn( coord.StartX - divX * LineWidth,
                                          coord.StartY - divY * LineWidth,
                                          coord.EndX   + divX * LineWidth,
                                          coord.EndY   + divY * LineWidth ),
                           true );

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

void CDrawingWindow::setNewCurrentFigure()
{
   switch ( m_currentFigureType )
   {
      case EFigureType::eFT_Circle:
      {
         m_currentFigure = std::shared_ptr< CCircleFigure >( new CCircleFigure( 0, 0, 0, 0, m_currentWidth, m_currentColor ) );
         break;
      }
      case EFigureType::eFT_Line:
      default:
      {
         m_currentFigure = std::shared_ptr< CLineFigure >( new CLineFigure( 0, 0, 0, 0, m_currentWidth, m_currentColor ) );
         break;
      }
   }
}
