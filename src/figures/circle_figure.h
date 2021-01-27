#pragma once
#ifndef FIGURES_CIRCLE_FIGURE_H_
#define FIGURES_CIRCLE_FIGURE_H_

#include "figure.h"

namespace Figures
{
   class CCircleFigure : public IFigure
   {
   public:
      explicit CCircleFigure() : IFigure() 
      {
      }

      explicit CCircleFigure( int x1, int y1, int x2, int y2, unsigned width, COLORREF color ) :
         IFigure( x1, y1, x2, y2, width, color )
      {
      }

      virtual ~CCircleFigure() noexcept = default;

      void draw( const HDC & hdc )  const override;
      EFigureType getFigureType() const override { return EFigureType::eFT_Circle; }
   };
};

#endif // FIGURES_CIRCLE_FIGURE_H_
