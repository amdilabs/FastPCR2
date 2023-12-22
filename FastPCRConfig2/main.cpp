/**************************************************************************
 * Version Numbering Xnn.xx.yyy:
 *      X - Design/develop,
 *      nn - starts 01, major
 *      xx - starts 00, minor
 *      yy - starts 001, internal
 *
 * * Revision History
 * Date      Version     Auther Assay   Note
 *                              Release
 * 12/04/23  X02.01.001  MP     Yes     Added Pre-HBH Configiration.
 *
**************************************************************************/

#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
