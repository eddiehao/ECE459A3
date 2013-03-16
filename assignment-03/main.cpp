/* code by Rafael Robledo, 
 * posted at http://code.google.com/p/nm-morph/,
 * licensed under Apache License 2.0. */

/* translations by Google Translate! */
/* let me know if you have better translations for me. */

/* to generate the makefile, qmake -o Makefile nm-morph.pro */
/* qmake-qt4 on ece459-1 */

#include <QtGui/QApplication>
#include "window.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
