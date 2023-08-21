#include "OMS.h"
#include "ui_OMS.h"
#include "yolo.h"

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
    updateButtonStates();
}

MainWindow::~MainWindow()
{
}

void MainWindow::updateSelectImg()
{
    QPixmap pic;
    // If the images are not in vector
    if (index >= images.size())
    {
        images.push_back({ QPixmap(), QPixmap() });
        processingTime.push_back("");
    }

    // If the selected image has not been loaded yet
    if (images[index].first.isNull())
    {
        Mat img = imread(filePaths[index].toStdString(), IMREAD_COLOR);
        int targetWidth = 640 * 1.5;
        int targetHeight = static_cast<int>((static_cast<float>(targetWidth) / img.cols) * img.rows);
        cv::resize(img, img, Size(targetWidth, targetHeight), INTER_LINEAR);
        cvtColor(img, img, COLOR_BGR2RGB);

        pic = QPixmap::fromImage(QImage(img.data, img.cols, img.rows, img.step, QImage::Format_RGB888));
        images[index].first = pic;
    }
    // If the selected image has been loaded
    else
    {
        pic = images[index].first;
    }

    // Update the image view
    QFileInfo info(filePaths[index]);
    ui.fileName->setText(QString::fromStdString("Source Image: ") + info.fileName());
    ui.imageView->setPixmap(pic.scaled(ui.imageView->width(), ui.imageView->height(), KeepAspectRatio));
}

void MainWindow::updateProcessedImg()
{
    QPixmap pic;
    // If the processed image has not been loaded yet
    if (images[index].second.isNull())
    {
        string time;
        pic = (yolo.ProcessImage(filePaths[index].toStdString(), time));
        images[index].second = pic;
        processingTime[index] = time;
    }
    // If the processed image has been loaded
    else
    {
        pic = images[index].second;
    }

    // Update the image view
    ui.imageView_2->setPixmap(pic.scaled(ui.imageView->width(), ui.imageView->height(), KeepAspectRatio));
    ui.fileName_2->setText(QString::fromStdString("Processed Image: ") + QString::fromStdString(processingTime[index]));
}

void MainWindow::updateButtonStates()
{
    ui.back_Button->setEnabled(index > 0);
    ui.next_Button->setEnabled(index < filePaths.size() - 1);
}

void MainWindow::on_selectImg_Button_clicked()
{
    filePaths.append(QFileDialog::getOpenFileNames(this, "Select Image(s) - OMS", "", tr("Images (*.png *.xpm *.jpg)")));

    updateSelectImg();
    updateProcessedImg();
    updateButtonStates();
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
        updateButtonStates();
    }
}

void MainWindow::on_next_Button_clicked()
{
    if (index < filePaths.size())
    {
        index++;
        updateSelectImg();
        updateProcessedImg();
        updateButtonStates();
    }
}