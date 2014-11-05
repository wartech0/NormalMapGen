#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QMessageBox>
#include <QVector3D>
#include <iostream>

#include "fragmentpane.h"

enum IMAGE
{
    TOP,
    RIGHT,
    LEFT,
    BOTTOM
};


namespace Ui {

class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_topFileButton_clicked();

    void on_rightFileButton_clicked();

    void on_bottomFileButton_clicked();

    void on_leftFileButton_clicked();

    void on_generateButton_clicked();

    void on_diffuseFileButton_clicked();

private:
    QFileInfo OpenFile(QPushButton* button, QImage& image, QGraphicsScene* scene, QGraphicsView* view);
    void EnableGenerateButton(bool topPresent, bool rightPresent, bool bottomPresent, bool leftPresent);
    void ProcessNormals(QImage image, int imagetype);
    bool SameSize(QImage first, QImage second, QImage third, QImage fourth);

private:
    Ui::MainWindow *ui;
    FragmentPane* fragPane;


    //Scenes
    QGraphicsScene* topScene;
    QGraphicsScene* rightScene;
    QGraphicsScene* bottomScene;
    QGraphicsScene* leftScene;
    QGraphicsScene* diffuseScene;
    QGraphicsScene* resultScene;

    //Images
    QImage topImage;
    QImage rightImage;
    QImage bottomImage;
    QImage leftImage;
    QImage diffuseImage;
    QImage resultImage;

    QVector3D* normals;

    QTimer* timer;
};

#endif // MAINWINDOW_H
