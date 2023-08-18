#include "OMS.h"
#include "ui_OMS.h"
#include "yolo.h"
#ifndef LIST_H
#include "list.h"
#endif

using namespace std;
using namespace cv;
using namespace dnn;
using namespace cuda;
using namespace Qt;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), yolo()
{
    ui.setupUi(this);
    index = 0;
}

MainWindow::~MainWindow()
{
}

void MainWindow::updateSelectImg()
{
    // If files were selected
    if (!filePaths.isEmpty())
    {
        QPixmap pic(filePaths[index]);
        QFileInfo info(filePaths[index]);

        ui.fileName->setText(QString::fromStdString("Source Image: ") + info.filePath());

        ui.imageView->setPixmap(pic.scaled(ui.imageView->width(), ui.imageView->height(), KeepAspectRatio));
    }
}

void MainWindow::updateProcessedImg()
{
    // If files were selected
    if (!filePaths.isEmpty())
    {
        QPixmap pic(yolo.ProcessImage(filePaths[index].toStdString(), ui.fileName_2));

        ui.imageView_2->setPixmap(pic.scaled(ui.imageView->width(), ui.imageView->height(), KeepAspectRatio));
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
        index--;
        updateSelectImg();
        updateProcessedImg();
    }
}

void MainWindow::on_next_Button_clicked()
{
    if (index < filePaths.size())
    {
        index++;
        updateSelectImg();
        updateProcessedImg();
    }
}