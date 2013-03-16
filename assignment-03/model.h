#ifndef MODEL_H
#define MODEL_H

#include <QtGui>
#include <QGraphicsView>

using namespace std;

#define NUM_IMAGES 5

class Model {
 public:
	Model();
	
	void prepStraightLine();
	void commonPrep();
	void morph(int h, double VARA, double VARB, double VARP);

	int himg, wimg;
    vector<pair<QPoint, QPoint> > **listLines;
    vector<pair<QPoint, QPoint> > **listAux;
    QImage *imgs[NUM_IMAGES];
};
#endif
