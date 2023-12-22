#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "configuration.h"
#include "data.h"
#include "ramp.h"

namespace Ui
{ class MainWindow;
}

class MainWindow : public QMainWindow
{ Q_OBJECT

  public:
    MainWindow (QWidget *parent = nullptr);
    ~MainWindow (void);

  private slots:
    void actionOpenTriggered (void);
    void actionClearTriggered (void);
    void actionClearLastTriggered (void);
    void actionExportCsvTriggered (void);
    void actionExportPdfTriggered (void);
    void actionAboutTriggered (void);
    void settingsAlignmentBoxActivated (int index);
    void settingsRampDataBoxActivated (int index);
    void settingsRampFittingBoxActivated (int index);
    void settingsStepBoxEditingFinished (void);
    void settingsLengthBoxEditingFinished (void);

  private:
    Ui::MainWindow *ui;
    Configuration m_config;
    QList<Data> m_data;

    void process (void);
    void restoreSettings (void);
    void saveSettings (void);

    void plot (const Data &data);
    static double offset (const Data &data, const Configuration::Alignment alignment);
    static QList<QPointF> points (const Data &data, const Configuration::RampData rampData, const double step, const double length);
    static Ramp ramp (const Data &data, const QList<QPointF> &points, const Configuration::RampFitting rampFitting);
};

#endif // MAINWINDOW_H
