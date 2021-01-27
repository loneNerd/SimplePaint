#include "../../include/figures/circle_figure.h"

using namespace Figures;

void CCircleFigure::draw( const HDC& hdc ) const
{
   HPEN pen = CreatePen( PS_SOLID, this->getLineWidth(), this->getColor() );

   SelectObject( hdc,  pen );
   SelectObject( hdc, GetStockObject( NULL_BRUSH ) );

   CCoordinates pos = this->getCCoordinates();
   Ellipse( hdc, pos.m_startX, pos.m_startY, pos.m_endX, pos.m_endY );

   DeleteObject( pen );
}
