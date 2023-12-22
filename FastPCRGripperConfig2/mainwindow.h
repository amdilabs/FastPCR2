#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../cycler/pge.h"

namespace Ui
{ class MainWindow;
}

class MainWindow : public QMainWindow
{ Q_OBJECT

  public:
    MainWindow (QWidget *parent = nullptr);
    ~MainWindow (void);

  private slots:
    void logReceived (const QDateTime &timestamp, const Log::Severity severity, const QString &tag, const QString &text);
    void gripperBoxActivated (int index);
    void readButtonClicked (void);
    void writeButtonClicked (void);

  private:
    Ui::MainWindow *ui;
    Log m_log;
    Interface m_interface;
    Pge m_pge;

    void connectToPge (const QString &portName);
    void disconnectFromPge (void);

    void read (void);
    bool write (void);
};

#endif // MAINWINDOW_H
