#pragma once

#include <QtWidgets/QMainWindow>
#include <QFileDialog>
#include <QPixmap>
#include "ui_ODS.h"
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <filesystem>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/cuda.hpp>

using namespace std;
using namespace cv;
using namespace dnn;
using namespace cuda;

class Yolo
{
public:
	Yolo();

	void LoadCategories();
	void ReadModel();
	void DrawLabel(Mat& input_image, string label, Scalar color, int left, int top);
	vector<Mat> PreProcess(Mat& input_image);
	Mat PostProcess(Mat& input_image, vector<Mat>& detections);

	QPixmap ProcessImage(const string& imagePath, string& processedTime);
private:
	// Categories with Colors
	vector<pair<string, Scalar>> colors;

	// Image Paths
	vector<string> imagePaths;

	// Model
	Net net;

	// Constants
	float INPUT_WIDTH;
	float INPUT_HEIGHT;
	float SCORE_THRESHOLD;
	float NMS_THRESHOLD;
	float CONFIDENCE_THRESHOLD;

	// Text parameters
	float FONT_SCALE;
	int FONT_FACE;
	int THICKNESS;

	// Directories
	string COCO_DIR;
	string IMG_DIR;
	string MODEL_DIR;
};