#include <QApplication>
#include "mainwindow.h"

int main (int argc, char *argv [])
{ QApplication a (argc, argv);
  a.setApplicationName ("InductionGraph");
  a.setApplicationVersion ("1.1.0 (2023-06-29)");

  MainWindow w;
  w.show ();

  return a.exec ();
}
