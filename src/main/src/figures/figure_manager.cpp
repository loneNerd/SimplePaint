#include "../../include/figures/figure_manager.h"

using namespace Figures;

void CFigureManager::addFigure( shared_ptr< CFigure > figure )
{
   m_figures.push_back( figure );
}

void CFigureManager::removeFigure( shared_ptr< CFigure > figure )
{
   std::remove( m_figures.begin(), m_figures.end(), figure );
}

void CFigureManager::update( const HDC& hdc )
{
   for ( const auto& figure : m_figures )
   {
      figure->draw( hdc );
   }
}
