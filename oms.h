#pragma once

#include <QtWidgets/QMainWindow>
#include <QFileDialog>
#include <QPixmap>
#include "ui_OMS.h"
#include "yolo.h"
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/cuda.hpp>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>

using namespace std;
using namespace cv;
using namespace dnn;
using namespace cuda;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateSelectImg();
    void updateProcessedImg();
    void on_selectImg_Button_clicked();
    void on_processImg_Button_clicked();
    void on_back_Button_clicked();
    void on_next_Button_clicked();

private:
    Ui::OMSClass ui;

    int index;

    Yolo yolo;

    QGraphicsScene* sourceImg;
    QGraphicsScene* processedImg;

    QStringList filePaths;
    vector<QGraphicsScene*> processedImgs;
};
