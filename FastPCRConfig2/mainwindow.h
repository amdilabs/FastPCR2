#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../cycler/pcrprotocol.h"

namespace Ui
{ class MainWindow;
}

class MainWindow : public QMainWindow
{ Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  private slots:
    void actionOpenTriggered (void);
    void actionSaveTriggered (void);
    void actionSaveAsTriggered (void);

  private:
    static const double decelerationInterval;

    Ui::MainWindow *ui;
    QString m_fileName;

    void populate (const PcrProtocol &protocol);
    PcrProtocol protocol (void) const;
    bool save (const QString &fileName);
};

#endif // MAINWINDOW_H
