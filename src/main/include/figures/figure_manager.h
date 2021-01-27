#pragma once
#ifndef FIGURES_FIGURE_MANAGER_H_
#define FIGURES_FIGURE_MANAGER_H_

#include <list>
#include <memory>

#include "figure.h"

using std::list;
using std::shared_ptr;

namespace Figures
{
   class CFigureManager
   {
   public:
      void addFigure( shared_ptr< CFigure > figure );
      void removeFigure( shared_ptr< CFigure > figure );
      void update( const HDC& hdc );
   private:
      list < shared_ptr < CFigure > > m_figures;
   };

}

#endif // FIGURES_FIGURE_MANAGER_H_
