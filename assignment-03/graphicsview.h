#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QtGui>
#include <QGraphicsView>
#include "model.h"

using namespace std;

class GraphicsView : public QGraphicsView {
    Q_OBJECT

public:
    GraphicsView(Model * model, QWidget *parent = 0, int idx = -1);
    ~GraphicsView();

    static void colorDrawing(QColor);
    static bool straightLine;

    void reset();

    void enableDrawing(bool);

    void cleanAll(void);
    void cleanLines(void);
    void undoLastLine(void);

    //vector< vector<QPoint>* > *listPoint;
    vector<QPoint>* listPoint[100];
    int totalItems;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);

private slots:
    void resizeImages();

signals:
    void totalChanged(int);

private:
	Model * model;

    bool drawing;
    bool able2Drawing;
    static QPen *pen;

    QTimer *timer;

    int x1, x2, y1, y2;

	int idx; /* index of this graphic in model */
};

#endif // GRAPHICSVIEW_H
