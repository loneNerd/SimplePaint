#include "circle_figure.h"

using namespace Figures;

void CCircleFigure::draw( const HDC& hdc ) const
{
   HPEN pen = CreatePen( PS_SOLID, getLineWidth(), getColor() );

   SelectObject( hdc,  pen );
   SelectObject( hdc, GetStockObject( NULL_BRUSH ) );

   CCoordinates pos = getCCoordinates();
   Ellipse( hdc, pos.startX, pos.startY, pos.endX, pos.endY );

   DeleteObject( pen );
}
