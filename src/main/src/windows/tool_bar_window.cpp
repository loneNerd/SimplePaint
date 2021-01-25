#include "../../include/windows/tool_bar_window.h"

using namespace Windows;

CToolBarWindow::CToolBarWindow( HINSTANCE hInstance, HWND parent, int nCmdShow )
{
   m_toolBarWindow.style         = CS_HREDRAW | CS_VREDRAW;
   m_toolBarWindow.lpfnWndProc   = CToolBarWindow::s_processes;
   m_toolBarWindow.hInstance     = hInstance;
   m_toolBarWindow.hCursor       = LoadCursor( nullptr, IDC_ARROW );
   m_toolBarWindow.hbrBackground = ( HBRUSH )( LTGRAY_BRUSH );
   m_toolBarWindow.lpszClassName = L"ToolBarWindow";

   RegisterClass( &m_toolBarWindow );

   RECT rc{};
   GetWindowRect( parent, &rc );

   m_toolBar = CreateWindowEx( 0,
                               L"ToolBarWindow",
                               L"Toolbar Field",
                               WS_CHILD | WS_DISABLED | WS_CLIPSIBLINGS,
                               0, 0, rc.right - rc.left, 50,
                               parent,
                               nullptr,
                               hInstance,
                               this );

   ShowWindow( m_toolBar, nCmdShow );
}

CToolBarWindow::~CToolBarWindow()
{
   DestroyWindow( m_toolBar );
}

CToolBarWindow::CToolBarWindow( const CToolBarWindow& other )
{
   if ( this != &other )
   {
      m_toolBarWindow = other.m_toolBarWindow;
      DestroyWindow( m_toolBar );
      m_toolBar = other.m_toolBar;
   }
}

CToolBarWindow::CToolBarWindow( CToolBarWindow&& other )
{
   if ( this != &other )
   {
      m_toolBarWindow = other.m_toolBarWindow;
      DestroyWindow( m_toolBar );
      m_toolBar = other.m_toolBar;
      DestroyWindow( other.m_toolBar );
   }
}

CToolBarWindow& CToolBarWindow::operator=( const CToolBarWindow& other )
{
   if ( this != &other )
   {
      CToolBarWindow temp{ other };
      std::swap( temp, *this );
   }

   return *this;
}

CToolBarWindow& CToolBarWindow::operator=( CToolBarWindow&& other ) noexcept
{
   if ( this != &other )
   {
      CToolBarWindow temp{ other };
      std::swap( temp, *this );
   }

   return *this;
}

LRESULT CToolBarWindow::s_processes( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
   CToolBarWindow* pThis;

   if ( uMsg == WM_NCCREATE )
   {
      pThis = static_cast< CToolBarWindow* >( reinterpret_cast< CREATESTRUCT* >( lParam )->lpCreateParams );

      SetLastError( 0 );
      if ( !SetWindowLongPtr( hWnd, GWL_USERDATA, reinterpret_cast< LONG_PTR >( pThis ) ) )
      {
         if ( GetLastError() != 0 )
            return FALSE;
      }
   }
   else
   {
      pThis = reinterpret_cast< CToolBarWindow* >( GetWindowLongPtr( hWnd, GWL_USERDATA ) );
   }

   if ( pThis )
   {
      return pThis->processes( hWnd, uMsg, wParam, lParam );
   }

   return FALSE;
}

LRESULT CALLBACK CToolBarWindow::processes( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
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
      case WM_DESTROY:
      {
         PostQuitMessage( 0 );
         break;
      }
   }

   return DefWindowProc( hWnd, uMsg, wParam, lParam );
}
