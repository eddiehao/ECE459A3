/* to generate the makefile, qmake -o Makefile test-harness.pro */
/* qmake-qt4 on ece459-1. */

#include <iostream>

#include <QtGui/QApplication>
#include "model.h"
#include "window.h"

int main(int argc, char * argv[]) {
    QCoreApplication a(argc, argv);

    Model m;

    m.imgs[0]->load((argc > 2) ? argv[1] : "yosemite.jpg");
    m.imgs[1]->load((argc > 2) ? argv[2] : "montmorency.jpg");

    m.wimg = m.imgs[0]->width();
    m.himg = m.imgs[0]->height();

    m.listLines[0] = new vector<pair<QPoint, QPoint> >();
    m.listLines[1] = new vector<pair<QPoint, QPoint> >();

    m.listAux[0] = new vector<pair<QPoint, QPoint> >();
    m.listAux[1] = new vector<pair<QPoint, QPoint> >();

    m.imgs[2] = new QImage(m.wimg, m.himg, QImage::Format_RGB32);
    m.imgs[3] = new QImage(m.wimg, m.himg, QImage::Format_RGB32);
    m.imgs[4] = new QImage(m.wimg, m.himg, QImage::Format_RGB32);

    QPoint pt1A(105,363), pt1B(1311,858);
    pair<QPoint, QPoint> pp1(pt1A, pt1B);
    QPoint pt2A(108,2164), pt2B(2440,1128);
    pair<QPoint, QPoint> pp2(pt2A, pt2B);

    m.listLines[0]->push_back(pp1);
    m.listLines[1]->push_back(pp2);

    QPoint pt3A(1539,900), pt3B(1647,2574);
    pair<QPoint, QPoint> pp3(pt3A, pt3B);
    QPoint pt4A(2572,1170), pt4B(2312,2952);
    pair<QPoint, QPoint> pp4(pt4A, pt4B);

    m.listLines[0]->push_back(pp3);
    m.listLines[1]->push_back(pp4);

    QPoint pt5A(2781,900), pt5B(2430,2442);
    pair<QPoint, QPoint> pp5(pt5A, pt5B);
    QPoint pt6A(2616,1156), pt6B(2532,2972);
    pair<QPoint, QPoint> pp6(pt6A, pt6B);

    m.listLines[0]->push_back(pp5);
    m.listLines[1]->push_back(pp6);

    QPoint pt7A(3050,750), pt7B(3950,500);
    pair<QPoint, QPoint> pp7(pt3A, pt3B);
    QPoint pt8A(2740,1032), pt8B(3932,68);
    pair<QPoint, QPoint> pp8(pt8A, pt8B);

    m.listLines[0]->push_back(pp7);
    m.listLines[1]->push_back(pp8);

    m.prepStraightLine();
    m.commonPrep();
    m.morph(0, 0.5, 1.25, 0.2);
    m.morph(1, 0.5, 1.25, 0.2);

    m.imgs[2]->save("yosemite-warped.png");
    m.imgs[3]->save("montmorency-warped.png");
    a.exit(0);
}
