#include "OMS.h"
#include "ui_OMS.h"
#include "yolo.h"
#include <QTimer>

using namespace std;
using namespace cv;
using namespace dnn;
using namespace cuda;
using namespace Qt;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    index = 0;
    //connect(ui.selectImg_Button, SIGNAL(clicked()), this, SLOT(DisplayImage()))
}

MainWindow::~MainWindow()
{
    delete sourceImg;
	delete processedImg;
}

void MainWindow::updateSelectImg()
{
    // If files were selected
    if (!filePaths.isEmpty())
    {
        sourceImg = new QGraphicsScene;

        QPixmap pic(filePaths[index]);
        QFileInfo info(filePaths[index]);

        ui.fileName->setText(QString::fromStdString("Source Image: ") + info.fileName());

        sourceImg->addPixmap(filePaths[index]);
        ui.imageView->setScene(sourceImg);
        ui.imageView->fitInView(sourceImg->itemsBoundingRect(), KeepAspectRatio);
    }
}

void MainWindow::updateProcessedImg()
{
    // If files were selected
    if (!filePaths.isEmpty())
    {
        processedImg = new QGraphicsScene;

        processedImg->addPixmap(yolo.ProcessImage(filePaths[index].toStdString(), ui.fileName_2));
        ui.imageView_2->setScene(processedImg);
        ui.imageView_2->fitInView(processedImg->itemsBoundingRect(), KeepAspectRatio);
    }
}

void MainWindow::on_selectImg_Button_clicked()
{
    filePaths.append(QFileDialog::getOpenFileNames(this, "Select Image(s) - OMS", "", tr("Images (*.png *.xpm *.jpg)")));

    updateSelectImg();
}

void MainWindow::on_processImg_Button_clicked()
{
    updateProcessedImg();
}

void MainWindow::on_back_Button_clicked()
{
    if (index > 0)
    {
        delete sourceImg;
        delete processedImg;

        index--;
        updateSelectImg();
        updateProcessedImg();
    }
}

void MainWindow::on_next_Button_clicked()
{
    if (index < filePaths.size())
    {
        delete sourceImg;
        delete processedImg;

        index++;
        updateSelectImg();
        updateProcessedImg();
    }
}