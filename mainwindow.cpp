#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    topScene = new QGraphicsScene(this);
    ui->topView->setScene(topScene);
    rightScene = new QGraphicsScene(this);
    ui->rightView->setScene(rightScene);
    bottomScene = new QGraphicsScene(this);
    ui->bottomView->setScene(bottomScene);
    leftScene = new QGraphicsScene(this);
    ui->leftView->setScene(leftScene);
    diffuseScene = new QGraphicsScene(this);
    ui->diffuseView->setScene(diffuseScene);
    resultScene = new QGraphicsScene(this);
    ui->resultView->setScene(resultScene);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), ui->oglWidget, SLOT(update()));
    timer->start(40);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QFileInfo MainWindow::OpenFile(QPushButton* button, QImage& image, QGraphicsScene* scene, QGraphicsView* view)
{

    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), "/", "Image File (*.png)");
    QFileInfo fi(filename);

    if(fi.exists())
    {
        button->setText(fi.fileName());
        image.load(fi.absoluteFilePath());
        scene->clear();
        scene->addPixmap(QPixmap::fromImage(image.scaled(QSize(100, 100))));
        view->show();
    }
    return fi;
}

void MainWindow::EnableGenerateButton(bool topPresent, bool rightPresent, bool bottomPresent, bool leftPresent)
{
    if(topPresent && rightPresent && bottomPresent && leftPresent)
    {
        ui->generateButton->setEnabled(true);
    }
}

void MainWindow::on_topFileButton_clicked()
{
    OpenFile(ui->topFileButton, topImage, topScene, ui->topView);
    EnableGenerateButton(!topImage.isNull(), !rightImage.isNull(), !bottomImage.isNull(), !leftImage.isNull());
}

void MainWindow::on_rightFileButton_clicked()
{
    OpenFile(ui->rightFileButton, rightImage, rightScene, ui->rightView);
    EnableGenerateButton(!topImage.isNull(), !rightImage.isNull(), !bottomImage.isNull(), !leftImage.isNull());
}

void MainWindow::on_bottomFileButton_clicked()
{
   OpenFile(ui->bottomFileButton, bottomImage, bottomScene, ui->bottomView);
   EnableGenerateButton(!topImage.isNull(), !rightImage.isNull(), !bottomImage.isNull(), !leftImage.isNull());
}

void MainWindow::on_leftFileButton_clicked()
{
    OpenFile(ui->leftFileButton, leftImage, leftScene, ui->leftView);
    EnableGenerateButton(!topImage.isNull(), !rightImage.isNull(), !bottomImage.isNull(), !leftImage.isNull());
}

void MainWindow::on_diffuseFileButton_clicked()
{
    QFileInfo fi = OpenFile(ui->diffuseFileButton, diffuseImage, diffuseScene, ui->diffuseView);
    ui->oglWidget->SetDiffuse(diffuseImage);
}

bool MainWindow::SameSize(QImage first, QImage second, QImage third, QImage fourth)
{
    QSize k = first.size();
    return k == second.size() && k == third.size() && k == fourth.size();
}

void MainWindow::ProcessNormals(QImage image, int imagetype)
{
    for(int y = 0; y < image.height(); y++)
    {
        for(int x = 0; x < image.width(); x++)
        {
            QColor color = image.pixel(x, y);
            double normal = color.valueF();
            normal -= 0.5;
            normal *= 2.0;
            if(imagetype == IMAGE::TOP)
            {
                QVector3D vector(0.0, normal, 0.0);
                normals[image.width() * y + x] = normals[image.width() * y + x] + vector;
            }

            if(imagetype == IMAGE::RIGHT)
            {
                QVector3D vector(normal, 0.0, 0.0);
                normals[image.width() * y + x] = normals[image.width() * y + x] + vector;
            }

            if(imagetype == IMAGE::BOTTOM)
            {
                QVector3D vector(0.0, -normal, 0.0);
                normals[image.width() * y + x] = normals[image.width() * y + x] + vector;
            }

            if(imagetype == IMAGE::LEFT)
            {
                QVector3D vector(-normal, 0.0, 0.0);
                normals[image.width() * y + x] = normals[image.width() * y + x] + vector;
            }
        }
    }
}

void MainWindow::on_generateButton_clicked()
{
    //Ensure all images are the same size
    if(!SameSize(topImage, rightImage, leftImage, bottomImage))
    {
        QMessageBox msgBox;
        msgBox.setText("Images are not the same size!");
        msgBox.exec();
        return;
    }

    int normalMapSize = topImage.size().width()*topImage.size().height();
    normals = new QVector3D[normalMapSize];

    for(int i = 0; i < normalMapSize; i++)
    {
        //Point all vectors -z
        normals[i].setZ(-1.0);
        normals[i].setY(0);
        normals[i].setX(0);
    }

    ProcessNormals(topImage, IMAGE::TOP);
    ProcessNormals(rightImage, IMAGE::RIGHT);
    ProcessNormals(bottomImage, IMAGE::BOTTOM);
    ProcessNormals(leftImage, IMAGE::LEFT);

    resultImage = QImage(topImage.width(), topImage.height(), QImage::Format_RGB32);

    for(int i = 0; i < normalMapSize; i++)
    {
        normals[i].normalize();
        QColor normColor;
        normColor.setRedF((normals[i].x()+1.0)/2.0);
        normColor.setGreenF((normals[i].y()+1.0)/2.0);
        normColor.setBlueF((-normals[i].z()+1.0)/2.0);
        int yCoord = i / topImage.width();
        int xCoord = i % topImage.width();
        resultImage.setPixel(QPoint(xCoord, yCoord), normColor.rgb());
    }
    resultScene->addPixmap(QPixmap::fromImage(resultImage.scaled(QSize(100, 100))));
    ui->resultView->show();
    ui->oglWidget->SetNormalMap(resultImage);
}
