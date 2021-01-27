#include "../../include/figures/line_figure.h"

using namespace Figures;

void CLineFigure::draw( const HDC& hdc ) const
{
   HPEN pen = CreatePen( PS_SOLID, this->getLineWidth(), this->getColor() );

   SelectObject( hdc, pen );

   Coordinates pos = this->getCoordinates();
   MoveToEx( hdc, pos.StartX, pos.StartY, nullptr );
   LineTo( hdc, pos.EndX, pos.EndY );

   DeleteObject( pen );
}