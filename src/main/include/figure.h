#pragma once
#ifndef FIGURE_H_
#define FIGURE_H_

#include <iostream>
#include <windows.h>
#include "CommCtrl.h"

namespace SimplePaint
{
   enum class EFigureType
   {
      eFT_Line,
      eFT_Circle
   };

   struct Coordinates
   {
      int StartX;
      int StartY;
      int EndX;
      int EndY;
   };

   class CFigure
   {
   public:
      explicit CFigure() :
         m_type{ EFigureType::eFT_Line },
         m_position{ 0, 0, 0, 0 },
         m_pen{ CreatePen(PS_SOLID, 1, 0) },
         m_lineWidth{ 0 },
         m_color{ RGB(0, 0, 0) } {}

      explicit CFigure( EFigureType figType, int x1, int y1, int x2, int y2, HPEN pen ) :
         m_type{ figType },
         m_position{ x1, y1, x2, y2 },
         m_pen{ pen } {}

      CFigure( const CFigure& other ) noexcept = default;
      CFigure( CFigure&& other )      noexcept = default;

      CFigure& operator=( const CFigure& other ) noexcept;
      CFigure& operator=( CFigure&& other )      noexcept = default;

      ~CFigure() noexcept = default;

      void draw( const HDC& hdc ) const;

      void setStartPosition( int x, int y ) { m_position.StartX = x; m_position.StartY = y; }
      void setEndPosition( int x, int y ) { m_position.EndX = x; m_position.EndY = y; }

      void setFigureType( EFigureType type ) { m_type = type; }
      void setLineWidth( unsigned int width ) { m_lineWidth = width; m_pen = CreatePen( PS_SOLID, width, m_color ); }
      void setColor( COLORREF color ) { m_color = color; m_pen = CreatePen( PS_SOLID, m_lineWidth, color ); }

      Coordinates getCoordinates() const { return m_position; }
      HPEN getPen()                const { return m_pen; }
      unsigned getPenWidth()       const { return m_lineWidth; }

   private:
      EFigureType m_type;
      Coordinates m_position;
      unsigned    m_lineWidth;
      COLORREF    m_color;
      HPEN        m_pen;
   };
};

#endif // FIGURE_H_
