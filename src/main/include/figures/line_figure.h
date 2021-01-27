#pragma once
#ifndef FIGURES_LINE_FIGURE_H_
#define FIGURES_LINE_FIGURE_H_

#include "figure.h"

namespace Figures
{
   class CLineFigure : public CFigure
   {
   public:
      CLineFigure() : CFigure() 
      {
      }

      CLineFigure( int x1, int y1, int x2, int y2, unsigned width, COLORREF color ) :
         CFigure( x1, y1, x2, y2, width, color ) 
      {
      }

      void draw( const HDC& hdc ) const override;
      EFigureType getFigureType() const override { return EFigureType::eFT_Line;  }
   };
};

#endif // FIGURES_LINE_FIGURE_H_
