#ifndef COLOURMAP_H
#define COLOURMAP_H

#include "../../lib/ui/qcustomplot.h"

class ColourMap
{ public:
    ColourMap (void);

    void reset (void);
    void next (void);

    QColor colour (void) const;
    QCPScatterStyle::ScatterShape shape (void) const;

  private:
    static const QColor colours [];
    static const int numOfColours;
    static const QCPScatterStyle::ScatterShape shapes [];
    static const int numOfShapes;

    int m_colourIndex;
    int m_shapeIndex;
};

#endif // COLOURMAP_H
