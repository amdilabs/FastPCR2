/**********************************************************************************
 * Version Numbering Xnn.xx.yyy:
 *      X - Design/develop,
 *      nn - starts 01, major
 *      xx - starts 00, minor
 *      yy - starts 001, internal
 *
 * * Revision History
 * Date      Version     Auther Assay   Note
 *                              Release
 * 08/30/23  X02.01.001  MP     Yes     Laser valving positioning fix.
 * 10/16/23  X02.01.002  MP     No      Added Selectable Cuvette Sealer.
 * 10/18/23  X02.02.001  MP     Yes     LED On/Off on Engineering tab
 * 11/08/23  X02.03.001  MP     Yes     Selectable sealer - original & Neck sealing
 * 11/28/23  X02.04.001  MP     Yes     Added Pre-HBH spin-mixing and pause steps
 * 12/07/23  X02.04.002  MP     Yes     Fixed Abort button issue
 * 12/13/23  X02.05.001  MP     Yes     Added check box to run pre-HBH or not
 * 12/20/23  X02.06.001  MP     Yes     Thermo Cycling time reduction
 *
 * *******************************************************************************/

#include <QApplication>
#include "mainwindow.h"

int main (int argc, char *argv [])
{ QApplication a (argc, argv);
  MainWindow w;
  w.show ();
  return a.exec ();
}
