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
      void addFigure( shared_ptr< IFigure > figure );
      void removeFigure( shared_ptr< IFigure > figure );
      void update( const HDC& hdc );
   private:
      list < shared_ptr < IFigure > > m_figures;
   };

}

#endif // FIGURES_FIGURE_MANAGER_H_
