/* code by Rafael Robledo, 
 * posted at http://code.google.com/p/nm-morph/,
 * licensed under Apache License 2.0. */

/* translations by Google Translate! */
/* let me know if you have better translations for me. */

/* to generate the makefile, qmake -o Makefile nm-morph.pro */
/* qmake-qt4 on ece459-1 */

#include "window.h"
#include "model.h"

#define MAX_POINTS 5
#define lambda 10

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    model.wimg = model.himg = 0;
    VARA = VARP = 0.0f;

    this->setMinimumSize(600, 440);
    this->resize(1000, 660);
    this->setStyleSheet("QAbstractButton, QLabel { padding: 6px 10px; };");

    // Layout for the entire window
    QHBoxLayout *superLayout = new QHBoxLayout();

    // Layout for main components
    // (includes images, controls)
    QVBoxLayout *mainLayout = new QVBoxLayout();

    // Layout for images
    QHBoxLayout *imgLayout = new QHBoxLayout();

    // Layout for controls
    QHBoxLayout *headLayout = new QHBoxLayout();

    // Layout for linetype selection
    QGroupBox *opcGrp1 = new QGroupBox(tr("Line type"));
    opcGrp1->setMinimumHeight(80);
    opcGrp1->setMaximumHeight(80);
    QFormLayout *opcForm1 = new QFormLayout();
    opcGrp1->setLayout(opcForm1);
    radio[0] = new QRadioButton(tr("Line segments"));
    radio[0]->setStyleSheet("padding: 1px");    
    radio[0]->setChecked(true);    
    radio[1] = new QRadioButton(tr("Freehand continuous line"));
    radio[1]->setStyleSheet("padding: 1px");
    connect(radio[0], SIGNAL(toggled(bool)), this, SLOT(on_radioLinea_toogled(bool)));
    connect(radio[1], SIGNAL(toggled(bool)), this, SLOT(on_radioLinea_toogled(bool)));
    opcForm1->addRow(radio[0]);
    opcForm1->addRow(radio[1]);

    // Layout for basic Controls
    QGroupBox *opcGrp2 = new QGroupBox(tr("Controls"));
    opcGrp2->setMinimumHeight(150);
    opcGrp2->setMaximumHeight(150);
    QFormLayout *opcForm2 = new QFormLayout();
    opcGrp2->setLayout(opcForm2);
    btnColor = new QPushButton("Color");
    btnProcess = new QPushButton("Process");
    btnSave = new QPushButton("Save resulting image");
    lblColor = new QLabel();
    lblColor->setAutoFillBackground(true);        
    connect(btnColor,   SIGNAL(clicked()), this, SLOT(on_btnColor_clicked()));
    connect(btnProcess, SIGNAL(clicked()), this, SLOT(on_btnProcess_clicked()));
    connect(btnSave,    SIGNAL(clicked()), this, SLOT(on_btnSave_clicked()));
    opcForm2->addRow(lblColor, btnColor);
    opcForm2->addRow(btnProcess);
    opcForm2->addRow(btnSave);

    // Layout for morphing Parameters
    QGroupBox *opcGrp3 = new QGroupBox(tr("Parameters"));
    opcGrp3->setStyleSheet("QLabel { padding: 1px; }");
    opcGrp3->setMaximumWidth(200);
    opcGrp3->setMaximumHeight(150);
    QFormLayout *opcForm3 = new QFormLayout();
    opcGrp3->setLayout(opcForm3);
    txtvalA = new QLineEdit();
    txtvalA->setAlignment(Qt::AlignRight);
    txtvalA->setText("0.001");
    connect(txtvalA, SIGNAL(returnPressed()), this, SLOT(on_txtValEnter()));
    txtvalB = new QLineEdit();
    txtvalB->setAlignment(Qt::AlignRight);
    txtvalB->setText("2");
    connect(txtvalB, SIGNAL(returnPressed()), this, SLOT(on_txtValEnter()));
    txtvalP = new QLineEdit();
    txtvalP->setAlignment(Qt::AlignRight);
    txtvalP->setText("0");
    connect(txtvalP, SIGNAL(returnPressed()), this, SLOT(on_txtValEnter()));
    opcForm3->addRow(tr("A:"), txtvalA);
    opcForm3->addRow(tr("B:"), txtvalB);
    opcForm3->addRow(tr("P:"), txtvalP);

    // Aggregate head layout controls
    headLayout->addWidget(opcGrp1);
    headLayout->addWidget(opcGrp3);
    headLayout->addWidget(opcGrp2);
    mainLayout->addLayout(headLayout);


    // Initialize image controls
    model.imgs[4] = new QImage();
    view[4] = new GraphicsView(&model);
    view[4]->enableDrawing(false);

    scen[4] = new QGraphicsScene();
    view[4]->setScene(scen[4]);

    for(int i=0; i<2; ++i) {
        model.imgs[i] = new QImage();

        view[i] = new GraphicsView(&model, this, i);
        view[i]->enableDrawing(false);
        view[i+2] = new GraphicsView(&model);
        view[i+2]->enableDrawing(false);

        connect(view[i], SIGNAL(totalChanged(int)), this, SLOT(UpdateCount(int)));

        scen[i] = new QGraphicsScene();
        scen[i+2] = new QGraphicsScene();

        btnUndo[i] = new QPushButton("Undo last trace");
        btnClearLines[i] = new QPushButton("Clear all traces");
        btnClear[i] = new QPushButton("Clear canvas (#items = 0)");
        btnOpen[i] = new QPushButton(tr("Select image"));

        connect(btnUndo[i], SIGNAL(clicked()), this, SLOT(on_btnUndo_clicked()));
        connect(btnClearLines[i],SIGNAL(clicked()), this, SLOT(on_btnClearLines_clicked()));
        connect(btnClear[i],SIGNAL(clicked()), this, SLOT(on_btnClear_clicked()));
        connect(btnOpen[i], SIGNAL(clicked()), this, SLOT(on_btnOpen_clicked()));

        imageContainer[i] = new QVBoxLayout();

        imageContainer[i]->addWidget(btnUndo[i]);
        imageContainer[i]->addWidget(btnClearLines[i]);

        imageContainer[i]->addWidget(btnClear[i]);
        imageContainer[i]->addWidget(view[i]);
        imageContainer[i]->addWidget(btnOpen[i]);

        frame[i] = new QFrame();
        frame[i]->setLayout(imageContainer[i]);

        imgLayout->addWidget(frame[i]);

        view[i]->setScene(scen[i]);
        view[i+2]->setScene(scen[i+2]);
    }

    // Holds images on the right
    imageContainer[2] = new QVBoxLayout();
    imageContainer[2]->addWidget(view[2]);
    imageContainer[2]->addWidget(view[3]);

    slider = new QSlider(Qt::Horizontal);
    slider->setMaximum(100);
    slider->setMinimum(0);
    slider->setValue(50);
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(on_slider_change(int)));

    imageContainer[2]->addWidget(slider);

    imageContainer[2]->addWidget(view[4]);
    frame[2] = new QFrame();
    frame[2]->setLayout(imageContainer[2]);
    frame[2]->setMaximumWidth(300);

    // Window layout
    mainLayout->addLayout(imgLayout);
    superLayout->addLayout(mainLayout);
    superLayout->addWidget(frame[2]);
    this->setLayout(superLayout);

    // Configure load image dialog
    diaImage = new QFileDialog(this);
    diaImage->setFileMode(QFileDialog::ExistingFile);
    diaImage->setNameFilter(tr("Images (*.png *.jpg *.bmp)"));
    diaImage->setViewMode(QFileDialog::Detail);

    diaColor = new QColorDialog(this);
    //diaColor->setCurrentColor(colorPen->rgb());
    this->ChangeColorPen(QColor(Qt::red));
}

MainWindow::~MainWindow() {

}

void MainWindow::keyPressEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_Escape) {
        close();
    } else {
        QWidget::keyPressEvent(e);
    }
}    

void MainWindow::on_txtValEnter() {
    this->ProcessImages();
}

void MainWindow::on_radioLinea_toogled(bool a) {
    GraphicsView::straightLine = (sender() == radio[!a] && a);
}

void MainWindow::on_btnOpen_clicked() {
    bool pos = (sender() == btnOpen[RIGTH]);
    LoadImage(pos);
}

// Completely clear the canvas
void MainWindow::on_btnClear_clicked() {
    bool pos = (sender() == btnClear[RIGTH]);
    CleanCanvas(pos);
}

// Remove all segments
void MainWindow::on_btnClearLines_clicked() {
    bool pos = (sender() == btnClearLines[RIGTH]);
    view[pos]->cleanLines();
}

// Undo most recent segment
void MainWindow::on_btnUndo_clicked() {
    bool pos = (sender() == btnUndo[RIGTH]);
    UndoLineCanvas(pos);
}

// Change drawing color
void MainWindow::on_btnColor_clicked() {
    if(!diaColor->exec()) return;
    this->ChangeColorPen(diaColor->selectedColor());
}

void MainWindow::ChangeColorPen(QColor colorPen) {
    diaColor->setCurrentColor(colorPen);
    lblColor->setText(colorPen.name());
    lblColor->setPalette(QPalette(colorPen.rgb()));
    GraphicsView::colorDrawing(colorPen);
}

void MainWindow::on_btnProcess_clicked() {
    this->ProcessImages();
}

class outRange {
public:
    int w, h;
    outRange(int _w, int _h) { this->w = _w; this->h = _h; }

    bool operator() (QPoint a) {
        if(a.x() < 0 || a.x() > w) return true;
        if(a.y() < 0 || a.y() > h) return true;

        return false;
    }
};

// Following the line segments, carry out the morphing.
void MainWindow::ProcessImages() {
    // Get morph parameters
    VARA = this->txtvalA->text().toDouble();
    VARB = this->txtvalB->text().toDouble();
    VARP = this->txtvalP->text().toDouble();

    // Check that both images were loaded
    if(loadimg[0] && loadimg[1] == 0) {
        QMessageBox::critical(this,
            tr("Proyecto Final - ALN - Morphing - RJRJ"),
            tr("Must load images before processing."),
            QMessageBox::Ok, QMessageBox::Ok
        );

        return;
    }

    // Check that both images have the same number of line segments
    if(GraphicsView::straightLine &&
            model.listLines[0]->size() != model.listLines[1]->size()) {
        QMessageBox::critical(this,
            tr("Proyecto Final - ALN - Morphing - RJRJ"),
            tr("Images must have same number of line segments.") +
                "(" + QString::number(model.listLines[0]->size()) + " " +
                    QString::number(model.listLines[1]->size()) + " ) ",
            QMessageBox::Ok, QMessageBox::Ok
        );

        return;
    } else if(!GraphicsView::straightLine &&
              (view[0]->totalItems != view[1]->totalItems)) {

        QMessageBox::critical(this,
            tr("Proyecto Final - ALN - Morphing - RJRJ"),
            tr("Images must have same number of points drawn on them.") +
                "(" + QString::number(model.listLines[0]->size()) + " " +
                    QString::number(model.listLines[1]->size()) + " ) ",
            QMessageBox::Ok, QMessageBox::Ok
        );

        return;
    }

    model.imgs[2] = new QImage(model.wimg, model.himg, model.imgs[0]->format());
    model.imgs[3] = new QImage(model.wimg, model.himg, model.imgs[0]->format());
    model.imgs[4] = new QImage(model.wimg, model.himg, model.imgs[0]->format());

    QRgb white = qRgba(255, 255, 255, 100);
    for(int i=0; i<model.wimg; ++i) {
        for(int j=0; j<model.himg; ++j) {
            model.imgs[4]->setPixel(i, j, white);
        }
    }

    QRgb red = qRgb(255, 0, 0);
    QRgb blue = qRgb(0, 0, 255);


    if(!GraphicsView::straightLine) {
        // Handle points (rather than lines)

        QRgb black = qRgba(0, 0, 0, 100);
        for(int iii=1; iii<view[0]->totalItems; ++iii) {
            // for each pair of lines...

            vector<QPoint> all = *view[0]->listPoint[iii];
            vector<QPoint> tempV = *view[1]->listPoint[iii];
            all.insert(all.end(), tempV.begin(), tempV.end());

            for(int m=0, n=all.size(); m<n; ++m) {
                //model.imgs[4]->setPixel(all[m], black);
            }

            // erase points which are out of range
            int w = model.imgs[0]->width();
            int h = model.imgs[0]->height();
            outRange range(w, h);
            all.erase(remove_if(all.begin(), all.end(), range), all.end());

            if(all.size() == 0) return;

            int maxx, maxy, minx, miny;
            maxx = minx = all[0].x();
            maxy = miny = all[0].y();

            // find min, max values for x, y
            for(int m=0, n=all.size(); m<n; ++m) {
                if(all[m].x() > maxx) maxx = all[m].x();
                if(all[m].x() < minx) minx = all[m].x();

                if(all[m].y() > maxy) maxy = all[m].y();
                if(all[m].y() < miny) miny = all[m].y();
            }

            int mx = maxx - minx;
            int my = maxy - miny;
            int hh;
            vector<int> points;

            int nn = mx * MAX_POINTS / w;
            hh = mx / nn;

            for(int i=0, j = minx; i<nn; ++i) {
                points.push_back(j);
                j += hh;
            }

            double x1, x2;
            double n1, n2;

            int n = all.size();
            int elems = points.size();

            double **k;
            k = new double*[elems+1];
            for(int i=0; i<=elems; ++i) {
                k[i] = new double[2];
            }
            double *b = new double[elems+1];
            double *x = new double[elems+1];

            //x1 = bigX? all[0].x() : all[0].y();
            x1 = all[0].x();
            x2 = points[0];

            hh = x2 - x1;

            for(int i=0, j=0; j<n; ++j) {
                double xx, yy;
                xx = all[j].x();
                yy = all[j].y();

                // Calculate N{i} & N{i+1}
                n1 = (x2 - xx) / hh;
                n2 = (xx - x1) / hh;

                // Add sum in k and by for each point evaluated
                k[i][0] += n1*n1;
                k[i+1][0] += n2*n2;

                k[i][1] += n1*n2;

                b[i] += n1*yy;
                b[i+1] += n2*yy;

                if(xx >= x2) {
                   double f = lambda/elems;
                   k[i][0] += f;
                   k[i][1] += -f;
                   k[i+1][0] += f;

                   i++;
                   if(i == elems) break;

                   x2 = points[i+1];
                   x1 = points[i];
                   hh = x2 -x1;
                }
            }

            // Solve tridiagonal system
            for(int i=1; i<elems; i++) {
                double m = k[i][1] / k[i-1][0];
                k[i][0] -= m * k[i-1][1];
                b[i] -= m * b[i-1];
            }

            x[elems-1] = b[elems-1] / k[elems-1][0];
            for (int i=elems-2; i>=0; i--) {
                x[i] = (b[i] - k[i][1] * x[i+1]) / k[i][0];
            }

            QPen *pen = new QPen(Qt::red);
            pen->setStyle(Qt::SolidLine);
            pen->setCapStyle(Qt::RoundCap);
            pen->setWidth(2);
            pen->setColor(Qt::red);


            for(int cc=0; cc<elems-2; ++cc) {
                int x3, x4, y3, y4;
                x3 = points[cc];    x4 = points[cc+1];
                y3 = x[cc];         y4 = x[cc+1];

                pair<QPoint, QPoint> pp = make_pair(QPoint(x3, y3), QPoint(x4, y4));
                model.listAux[0]->push_back(pp);
                //model->listLines[1]->push_back(pp);
            }
        }
    }

	if (GraphicsView::straightLine) {
		model.prepStraightLine();
	}

	model.commonPrep();

    for(int h=0; h<2; ++h) {
		model.morph(h, VARA, VARB, VARP);
    }

    //this->CrossDisolve(this->slider->value());
    //}

    for(int h=2; h<5; ++h) {
        scen[h]->clear();
        scen[h]->setSceneRect(0, 0, model.wimg, model.himg);
        scen[h]->addPixmap(QPixmap::fromImage(*model.imgs[h]));

        view[h]->fitInView(*view[h]->scene()->items().begin(), Qt::KeepAspectRatio);
    }
}

void MainWindow::on_slider_change(int a) {
    CrossDisolve(a);
}

void MainWindow::CrossDisolve(int a) {
    if(model.imgs[2] == NULL || model.imgs[3] == NULL || model.imgs[4] == NULL) return;

    float f = a / 100.0;
    float g = 1 - f;
    QRgb c, d;
    QPoint X;

    for(int i=0; i<model.wimg; ++i) {
        for(int j=0; j<model.himg; ++j) {
            X = QPoint(i, j);

            c = model.imgs[2]->pixel(X);
            d = model.imgs[3]->pixel(X);

            QRgb e = qRgb(f*qRed(c) + g*qRed(d),
                          f*qGreen(c) + g*qGreen(d),
                          f*qBlue(c) + g*qBlue(d)) ;
            model.imgs[4]->setPixel(X, qRgba(qRed(e), qGreen(e), qBlue(e), 255));
        }
    }

    scen[4]->addPixmap(QPixmap::fromImage(*model.imgs[4]));
}

void MainWindow::on_btnSave_clicked() {
}

void MainWindow::LoadImage(bool pos) {
    QStringList fileNames;

    if (!diaImage->exec()) return;

    fileNames = diaImage->selectedFiles();

    QString imagePath = fileNames.first();

    QSize sizecont = view[pos]->size();
    sizecont -= QSize(5, 5);

    model.imgs[pos]->load(imagePath);

    // load first image
    if(model.wimg == 0 && model.himg == 0) {
        model.wimg = model.imgs[pos]->width();
        model.himg = model.imgs[pos]->height();
    } else {
    // load second image
        if( (model.wimg != model.imgs[pos]->width()) ||
            (model.himg = model.imgs[pos]->height())) {
//            QMessageBox::critical(this,
//                tr("Proyecto Final - ALN - Morphing - RJRJ"),
//                tr("La imagen cargada no coincide en dimensiones con la otra imagen"),
//                QMessageBox::Ok, QMessageBox::Ok
//            );

//            model.imgs[pos] = NULL;
//            return;
        }
    }

    view[pos]->reset();
    scen[pos]->clear();
    scen[pos]->setSceneRect(0, 0, sizecont.width(), sizecont.height());
    scen[pos]->addPixmap(QPixmap::fromImage(*model.imgs[pos]));
    view[pos]->enableDrawing(true);
    view[pos]->resize(sizecont);

    loadimg[pos] = true;
}

void MainWindow::CleanCanvas(bool pos) {
    view[pos]->cleanAll();
    loadimg[pos] = false;

    if(loadimg[0] && loadimg[1] == 0) {
        model.wimg = model.himg = 0;
    }
}

void MainWindow::UndoLineCanvas(bool pos) {
    view[pos]->undoLastLine();
}

void MainWindow::UpdateCount(int value) {
    bool i = (sender() == view[RIGTH]);

    QString temp = "Clear canvas (#items = " + QString::number(value) + ")";
    btnClear[i]->setText(temp);
}
