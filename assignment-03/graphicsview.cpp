#include "graphicsview.h"

QPen *GraphicsView::pen = NULL;

bool GraphicsView::straightLine = true;

GraphicsView::GraphicsView(Model * model, QWidget *parent, int idx) : QGraphicsView(parent), model(model), idx(idx)  {
    pen = new QPen(Qt::red);
    pen->setStyle(Qt::SolidLine);
    pen->setCapStyle(Qt::RoundCap);
    pen->setWidth(2);
    pen->setColor(Qt::red);

    reset();

    timer = new QTimer();
    timer->setInterval(100);
    timer->setSingleShot(true);

	if (idx >= 0) {
		model->listLines[idx] = new vector<pair<QPoint, QPoint> >();
		model->listAux[idx] = new vector<pair<QPoint, QPoint> >();
	}
}

void GraphicsView::reset() {
    enableDrawing(false);

    drawing = false;
    totalItems = 0;

	if (idx >= 0) {
		model->listLines[idx] = new vector<pair <QPoint, QPoint> >();
		model->listAux[idx] = new vector<pair <QPoint, QPoint> >();
	}

    listPoint[100] = new vector<QPoint>[100]();
    for(int i=0; i<100; ++i) {
        listPoint[i] = new vector<QPoint>();
    }
}

GraphicsView::~GraphicsView() {}

void GraphicsView::mouseReleaseEvent(QMouseEvent * /*event*/) {
    drawing = false;
}

void GraphicsView::mousePressEvent(QMouseEvent *event) {
    if(!able2Drawing) return;

    if(straightLine) {
        if(!drawing && (event->buttons() & Qt::LeftButton)) {
            x1 = event->x();
            y1 = event->y();
        }

        drawing = (event->buttons() & Qt::LeftButton);
    } else {
        x1 = event->x();
        y1 = event->y();
        listPoint[totalItems-1]->push_back( QPoint(x1, y1) );

        this->totalItems++;
        emit this->totalChanged(totalItems);
    }
}

void GraphicsView::mouseMoveEvent(QMouseEvent * event) {
    if(!able2Drawing) return;

    x2 = event->x();
    y2 = event->y();

    if(straightLine) {
        if(!drawing) {
            this->totalItems++;
            this->undoLastLine();
        } else {
            if(event->buttons() & Qt::LeftButton) {
                drawing = false;

                x1 = event->x();
                y1 = event->y();

                this->totalItems++;
                emit this->totalChanged(totalItems);
            }
        }

		model->listLines[idx]->push_back( make_pair(QPoint(x1, y1), QPoint(x2, y2) ) );
    } else {
        QPoint a (x1, y1);
        QPoint b (x2, y2);
        listPoint[totalItems-1]->push_back(a);
        listPoint[totalItems-1]->push_back(b);

        this->scene()->addLine(x1, y1, x2, y2, *pen);
        x1 = x2; event->x();
        y1 = y2; event->y();
    }

    this->scene()->addLine(x1, y1, x2, y2, *pen);
}

void GraphicsView::resizeEvent(QResizeEvent * /*event*/) {
    timer->start();
}

void GraphicsView::resizeImages(void) {
    QList<QGraphicsItem *>::Iterator it = this->scene()->items().begin();
    QList<QGraphicsItem *>::Iterator end = this->scene()->items().end();

    while(it != end) {
        this->fitInView(*it, Qt::KeepAspectRatio);
        it++;
    }
}

void GraphicsView::cleanAll() {
	if (idx >= 0) {
		if(model->listLines[idx]->size() == 0) return;

		QList<QGraphicsItem *>::Iterator it = this->scene()->items().begin();
		while(it != this->scene()->items().end()) {
			this->scene()->removeItem(*(it));
			it = this->scene()->items().begin();
		}
		
		if(straightLine) {
			model->listLines[idx]->clear();
		} else {
			for(int i=0; i<100; ++i) {
				if(listPoint[i] != NULL)
					listPoint[i]->clear();
			}
		}
	}

    enableDrawing(false);
    totalItems = 0;
    emit totalChanged(totalItems);
}

void GraphicsView::cleanLines() {
	if (idx >= 0) {
		if(model->listLines[idx]->begin() == model->listLines[idx]->end() - 1) return;
		if(model->listLines[idx]->size() == 1) return;

		QList<QGraphicsItem *>::Iterator it = this->scene()->items().begin();
		while(it != this->scene()->items().end() - 1) {
			this->scene()->removeItem(*(it));
			it = this->scene()->items().begin();
		}
		
		if(straightLine) {
			model->listLines[idx]->erase(model->listLines[idx]->begin(), model->listLines[idx]->end() - 1);
		} else {
			model->listLines[idx]->erase(model->listLines[idx]->begin(), model->listLines[idx]->end() - 1);
		}
	}

    totalItems = 1;
    emit totalChanged(totalItems);
}

void GraphicsView::undoLastLine() {
    QList<QGraphicsItem *>::Iterator it = this->scene()->items().begin();

    if(straightLine) {
        if(it != this->scene()->items().end()) {
            this->scene()->removeItem(*(it));
            model->listLines[idx]->pop_back();

            this->totalItems--;
            emit this->totalChanged(totalItems);
        }
    } else {
        for(int i=0, j = listPoint[totalItems-1]->size();
            i<j; ++i) {
            if(it != this->scene()->items().end()) {
                this->scene()->removeItem(*(it));
                it = this->scene()->items().begin();
            } else {
                break;
            }
        }

        this->totalItems--;
        emit this->totalChanged(totalItems);

        listPoint[totalItems]->clear();
    }

    if(this->scene()->items().count() == 0) enableDrawing(false);
}

void GraphicsView::enableDrawing(bool v) {
    able2Drawing = v;
    setCursor(able2Drawing ? Qt::CrossCursor : Qt::ForbiddenCursor);

    totalItems = v;
    emit totalChanged(totalItems);
}

void GraphicsView::colorDrawing(QColor c) {
    GraphicsView::pen->setColor(c);
}
