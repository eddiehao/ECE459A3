/* to generate the makefile, qmake -o Makefile test-harness.pro */
/* qmake-qt4 on ece459-1. */

#include <iostream>

#include <QtGui/QApplication>
#include "model.h"
#include "window.h"

int main(int argc, char * argv[]) {
    QCoreApplication a(argc, argv);

    Model m;

    m.imgs[0]->load((argc > 2) ? argv[1] : "mountains.jpg");
    m.imgs[1]->load((argc > 2) ? argv[2] : "beaches.jpg");

	m.wimg = m.imgs[0]->width();
	m.himg = m.imgs[0]->height();

	m.listLines[0] = new vector<pair<QPoint, QPoint> >();
	m.listLines[1] = new vector<pair<QPoint, QPoint> >();

	m.listAux[0] = new vector<pair<QPoint, QPoint> >();
	m.listAux[1] = new vector<pair<QPoint, QPoint> >();

	m.imgs[2] = new QImage(m.wimg, m.himg, QImage::Format_RGB32);
	m.imgs[4] = new QImage(m.wimg, m.himg, QImage::Format_RGB32);

	QPoint pt1A(1000,1000), pt1B(1000,2000);
	pair<QPoint, QPoint> pp1(pt1A, pt1B);
	QPoint pt2A(1000,1000), pt2B(1000,1500);
	pair<QPoint, QPoint> pp2(pt2A, pt2B);

	m.listLines[0]->push_back(pp1);
	m.listLines[1]->push_back(pp2);

	QPoint pt3A(3000,1000), pt3B(3000,2000);
	pair<QPoint, QPoint> pp3(pt3A, pt3B);
	QPoint pt4A(1000,1000), pt4B(3000,3000);
	pair<QPoint, QPoint> pp4(pt4A, pt4B);

	m.listLines[0]->push_back(pp3);
	m.listLines[1]->push_back(pp4);

	m.prepStraightLine();
	m.commonPrep();
    m.morph(0, 0.0, 1.0, 0.2);

	m.imgs[2]->save("out.jpg");
	a.exit(0);
}
