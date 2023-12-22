#include "colourmap.h"

const QColor ColourMap::colours [] = {
  QColor (0x66, 0xb3, 0x3e),
  QColor (0x8a, 0x2f, 0xe4),
  QColor (0xd4, 0xbc, 0x00),
  QColor (0xc3, 0x26, 0x1d),
  QColor (0x00, 0x6f, 0x49),
  QColor (0xc3, 0x5c, 0xa5),
  QColor (0x01, 0xa9, 0xb5),
  QColor (0x3c, 0x46, 0xbd)
};

const int ColourMap::numOfColours = sizeof (colours) / sizeof (QColor);

const QCPScatterStyle::ScatterShape ColourMap::shapes [] = {
  QCPScatterStyle::ssDisc,
  QCPScatterStyle::ssCross,
  QCPScatterStyle::ssTriangle,
  QCPScatterStyle::ssSquare
};

const int ColourMap::numOfShapes = sizeof (shapes) / sizeof (QCPScatterStyle::ScatterShape);

ColourMap::ColourMap (void)
{ reset ();
}

void ColourMap::reset (void)
{ m_colourIndex = 0;
  m_shapeIndex = 0;
}

void ColourMap::next (void)
{ if (++m_colourIndex == numOfColours)
  { m_colourIndex = 0;

    if (++m_shapeIndex == numOfShapes)
    { m_shapeIndex = 0;
    }
  }
}

QColor ColourMap::colour (void) const
{ return colours [m_colourIndex];
}

QCPScatterStyle::ScatterShape ColourMap::shape (void) const
{ return shapes [m_shapeIndex];
}
