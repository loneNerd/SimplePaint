#include "../../include/figures/circle_figure.h"

using namespace Figures;

void CCircleFigure::draw( const HDC& hdc ) const
{
   HPEN pen = CreatePen( PS_SOLID, this->getLineWidth(), this->getColor() );

   SelectObject( hdc,  pen );
   SelectObject( hdc, GetStockObject( NULL_BRUSH ) );

   Coordinates pos = this->getCoordinates();
   Ellipse( hdc, pos.StartX, pos.StartY, pos.EndX, pos.EndY );

   DeleteObject( pen );
}
