#ifndef WINDOW_H
#define WINDOW_H

#include <QtGui>
#include <QFileDialog>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <algorithm>

#include "graphicsview.h"
#include "model.h"

#define LEFT 0
#define RIGTH 1

class MainWindow : public QWidget {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event);
    //void closeEvent(QCloseEvent *event);

private slots:
    void on_btnOpen_clicked();

    void on_btnClear_clicked();
    void on_btnClearLines_clicked();
    void on_btnUndo_clicked();

    void on_btnColor_clicked();
    void on_btnProcess_clicked();
    void on_btnSave_clicked();

    void on_radioLinea_toogled(bool);
    void on_txtValEnter();
    void on_slider_change(int);
    void UpdateCount(int);

private:
    void LoadImage(bool second);
    void CleanCanvas(bool second);
    void UndoLineCanvas(bool second);

    void ChangeColorPen(QColor);
    void CrossDisolve(int);

    void ProcessImages();

	Model model;

    double VARA, VARP, VARB;

    bool loadimg[2];

    QFileDialog *diaImage;
    QColorDialog *diaColor;

    QFrame *frame[3];
    QVBoxLayout *imageContainer[3];

    QGraphicsScene *scen[5];
    GraphicsView *view[5];

    QPushButton *btnOpen[2];

    QLineEdit *txtRuta[2];

    QLabel *lblColor;

    QRadioButton *radio[2];

    QPushButton *btnUndo[2], *btnClearLines[2], *btnClear[2];
    QPushButton *btnColor;

    QLineEdit *txtvalA, *txtvalB, *txtvalP;

    QPushButton *btnProcess;
    QPushButton *btnSave;
    QSlider *slider;
};

#endif // WINDOW_H
