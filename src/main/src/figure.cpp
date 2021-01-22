#include "../include/figure.h"

using namespace SimplePaint;

CFigure& CFigure::operator=( const CFigure& other ) noexcept
{
   if ( this != &other )
   {
      CFigure f{ other };
      std::swap( f, *this );
   }
   return *this;
}

CFigure& CFigure::operator=( CFigure&& other ) noexcept
{
   if ( this != &other )
   {
      std::swap( m_type,      other.m_type );
      std::swap( m_position,  other.m_position );
      std::swap( m_lineWidth, other.m_lineWidth );
      std::swap( m_color,     other.m_color );
      std::swap( m_pen,       other.m_pen );
   }
   return *this;
}

void CFigure::draw( const HDC& hdc ) const
{
   SelectObject( hdc, m_pen );
   switch ( m_type )
   {
      case EFigureType::eFT_Line:
      {

         MoveToEx( hdc, m_position.StartX, m_position.StartY, nullptr );
         LineTo( hdc, m_position.EndX, m_position.EndY );
         break;

      }
      case EFigureType::eFT_Circle:
      {

         SelectObject( hdc, m_pen );
         SelectObject( hdc, GetStockObject( NULL_BRUSH ) );
         Ellipse( hdc, m_position.StartX, m_position.StartY, m_position.EndX, m_position.EndY );
         break;

      }
   }
}
