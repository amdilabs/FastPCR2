#ifndef PLOTCONFIGURATION_H
#define PLOTCONFIGURATION_H

class PlotConfiguration
{ public:
    PlotConfiguration (void);

    bool visible (const int index) const;
    void setVisible (const int index, const bool visible);

  private:
    int m_visible;
};

#endif // PLOTCONFIGURATION_H
