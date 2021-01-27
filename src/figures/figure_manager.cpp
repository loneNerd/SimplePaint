#include "figure_manager.h"

using namespace Figures;

void CFigureManager::addFigure( shared_ptr< IFigure > figure )
{
   m_figures.push_back( figure );
}

void CFigureManager::removeFigure( shared_ptr< IFigure > figure )
{
   m_figures.remove( figure );
}

void CFigureManager::update( const HDC& hdc )
{
   for ( const auto& figure : m_figures )
   {
      figure->draw( hdc );
   }
}
