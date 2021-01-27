#include "../../include/figures/line_figure.h"

using namespace Figures;

void CLineFigure::draw( const HDC& hdc ) const
{
   HPEN pen = CreatePen( PS_SOLID, this->getLineWidth(), this->getColor() );

   SelectObject( hdc, pen );

   CCoordinates pos = this->getCCoordinates();
   MoveToEx( hdc, pos.m_startX, pos.m_startY, nullptr );
   LineTo( hdc, pos.m_endX, pos.m_endY );

   DeleteObject( pen );
}