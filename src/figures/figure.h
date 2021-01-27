#pragma once
#ifndef FIGURES_FIGURE_H_
#define FIGURES_FIGURE_H_

#include <iostream>
#include <windows.h>
#include "CommCtrl.h"

namespace Figures
{
   enum class EFigureType
   {
      eFT_Line,
      eFT_Circle
   };

   struct CCoordinates
   {
      int startX;
      int startY;
      int endX;
      int endY;
   };

   class IFigure
   {
   public:
      IFigure() :
         m_position{ 0, 0, 0, 0 },
         m_lineWidth{ 1 },
         m_color{ RGB( 0, 0, 0 ) } {}

      IFigure( int x1, int y1, int x2, int y2, unsigned width, COLORREF color ) :
         m_position{ x1, y1, x2, y2 },
         m_lineWidth{ width },
         m_color{ color } {}

      IFigure( const IFigure& other ) noexcept = default;
      IFigure( IFigure&& other )     noexcept = default;

      IFigure& operator=( const IFigure& other ) noexcept = default;
      IFigure& operator=( IFigure&& other )     noexcept = default;

      virtual ~IFigure() noexcept {}

      virtual void draw( const HDC& hdc ) const = 0;
      virtual EFigureType getFigureType() const = 0;

      virtual void setStartPosition( int x, int y ) { m_position.startX = x; m_position.startY = y; }
      virtual void setEndPosition( int x, int y )   { m_position.endX   = x; m_position.endY   = y; }

      virtual void setLineWidth( unsigned int width ) { m_lineWidth = width; }
      virtual void setColor( COLORREF color )         { m_color     = color; }

      virtual CCoordinates getCCoordinates() const { return m_position; }
      virtual COLORREF getColor()          const { return m_color; }
      virtual unsigned getLineWidth()      const { return m_lineWidth; }

   private:
      CCoordinates m_position;
      unsigned    m_lineWidth;
      COLORREF    m_color;
   };
};

#endif // FIGURES_FIGURE_H_
