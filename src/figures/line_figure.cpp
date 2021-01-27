#include "line_figure.h"

using namespace Figures;

void CLineFigure::draw( const HDC& hdc ) const
{
   HPEN pen = CreatePen( PS_SOLID, getLineWidth(), getColor() );

   SelectObject( hdc, pen );

   CCoordinates pos = getCCoordinates();
   MoveToEx( hdc, pos.startX, pos.startY, nullptr );
   LineTo( hdc, pos.endX, pos.endY );

   DeleteObject( pen );
}