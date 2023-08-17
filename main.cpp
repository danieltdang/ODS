#include "OMS.h"
#include <QtWidgets/QApplication>
#include <QApplication>
#include <QLabel>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}
