#include "yolo.h"

using namespace std;
using namespace cv;
using namespace dnn;
namespace fs = filesystem;

Yolo::Yolo()
{
	INPUT_WIDTH = 640;
	INPUT_HEIGHT = 640;
	SCORE_THRESHOLD = 0.5;
	NMS_THRESHOLD = 0.45;
	CONFIDENCE_THRESHOLD = 0.45;

	FONT_SCALE = 0.6;
	FONT_FACE = FONT_HERSHEY_TRIPLEX;
	THICKNESS = 1;

	COCO_DIR = "models/coco.txt";
	IMG_DIR = "images/";
	MODEL_DIR = "models/yolov5l6.onnx";

	LoadCategories();
	ReadModel();
}

void Yolo::LoadCategories()
{
	try
	{
		ifstream ifs(COCO_DIR);
		string category;

		srand(time(0));
		while (getline(ifs, category))
		{
			int b = rand() % 256;
			int g = rand() % 256;
			int r = rand() % 256;
			colors.push_back({ category, Scalar(b, g, r) });
		}
		cout << "[SYSTEM] Loaded " + to_string(colors.size()) + " COCO categories.\n";
	}
	catch (const exception& ex)
	{
		cout << "[LOAD_CATEGORY_ERROR] " << ex.what() << endl;
	}
}

void Yolo::ReadModel()
{
	try
	{
		net = readNet(MODEL_DIR);
		net.setPreferableBackend(DNN_BACKEND_CUDA);
		net.setPreferableTarget(DNN_TARGET_CUDA_FP16);
		cout << "[SYSTEM] Loaded " + MODEL_DIR.substr(MODEL_DIR.find_last_of('/') + 1) + " model.\n";
	}
	catch (const exception& ex)
	{
		cout << "[READ_MODEL_ERROR] " << ex.what() << endl;
	}
}

void Yolo::DrawLabel(Mat& input_image, string label, Scalar color, int left, int top)
{
	try
	{
		// Display the label at the top of the bounding box.
		int baseLine;
		Size label_size = getTextSize(label, FONT_FACE, FONT_SCALE, THICKNESS, &baseLine);
		top = max(top, label_size.height);
		
		// Offset is used to move the label up
		int offset = 20;

		// Top left corner.
		Point tlc = Point(left, top - offset);

		// Bottom right corner.
		Point brc = Point(left + label_size.width, top + label_size.height + baseLine - offset);

		// Draw rectangle for label
		rectangle(input_image, tlc, brc, color, FILLED);
		rectangle(input_image, tlc, brc, color, 3 * THICKNESS);

		// Put the label on the rectangle
		putText(input_image, label, Point(left, top + label_size.height - offset), FONT_FACE, FONT_SCALE, Scalar(0, 0, 0), THICKNESS, LINE_AA);
	}
	catch (const exception& ex)
	{
		cout << "[DRAW_PREDICTION_ERROR] " << ex.what() << endl;
	}
}

vector<Mat> Yolo::pre_process(Mat& input_image)
{
	try
	{
		// Convert to blob
		Mat blob;
		blobFromImage(input_image, blob, 1 / 255.0, Size(INPUT_WIDTH, INPUT_HEIGHT), Scalar(0, 0, 0), true, false);

		// Sets the input to the network
		vector<Mat> detections;
		net.setInput(blob);

		// Runs the forward pass to get output from the output layers
		net.forward(detections, net.getUnconnectedOutLayersNames());

		return detections;
	}
	catch (const exception& ex)
	{
		cout << "[PRE_PROCESS_ERROR] " << ex.what() << endl;
	}

	return vector<Mat>();
}

Mat Yolo::post_process(Mat& input_image, vector<Mat>& detections)
{
	try
	{
		// Initialize vectors to hold respective outputs while unwrapping detections
		vector<int> category_ids;
		vector<float> confidences;
		vector<Rect> boxes;

		// Resizing factor.
		float x_factor = input_image.cols / INPUT_WIDTH;
		float y_factor = input_image.rows / INPUT_HEIGHT;
		float* data = (float*)detections[0].data;
		const int dimensions = 85;

		// Get the number of detections
		int num_detections = detections[0].size[1];

		// Loop through all detections
		for (int i = 0; i < num_detections; i++)
		{
			// Get the confidence
			float confidence = data[4];

			// Check if confidence is equal to or above threshold
			if (confidence >= CONFIDENCE_THRESHOLD)
			{
				float* categories = data + 5;

				// Create a 1x85 Mat and store class scores of 80 categories
				Mat scores(1, colors.size(), CV_32FC1, categories);

				// Perform minMaxLoc and acquire the index of best class score
				Point category_id;
				double max_category_score;
				minMaxLoc(scores, 0, &max_category_score, 0, &category_id);

				// Continue if the class score is above the threshold
				if (max_category_score > SCORE_THRESHOLD)
				{
					// Store class ID and confidence in the pre-defined respective vectors
					confidences.push_back(confidence);
					category_ids.push_back(category_id.x);

					// Center
					float cx = data[0];
					float cy = data[1];

					// Box dimension
					float w = data[2];
					float h = data[3];

					// Bounding box coordinates
					int left = int((cx - 0.5 * w) * x_factor);
					int top = int((cy - 0.5 * h) * y_factor);
					int width = int(w * x_factor);
					int height = int(h * y_factor);

					// Store good detections in the boxes vector
					boxes.push_back(Rect(left, top, width, height));
				}
			}
			// Jump to the next row
			data += 85;
		}

		// Perform Non-Maximum Suppression and draw predictions
		vector<int> indices;
		NMSBoxes(boxes, confidences, SCORE_THRESHOLD, NMS_THRESHOLD, indices);
		for (int i = 0; i < indices.size(); i++)
		{
			int idx = indices[i];
			Rect box = boxes[idx];
			int left = box.x;
			int top = box.y;
			int width = box.width;
			int height = box.height;

			// Get the label for the class name and its confidence
			string label = format("%.1f", confidences[idx] * 100);
			label = colors[category_ids[idx]].first + ": " + label + "%";


			// Finds the color of the label
			size_t colonPos = label.find(':');
			// Extract the substring before the colon
			string category = label.substr(0, colonPos);

			bool isColor = false;
			Scalar color;
			int id = 0;
			while (!isColor && i < colors.size())
			{
				if (colors[id].first == category)
				{
					color = colors[id].second;
					isColor = true;
				}
				id++;
			}

			// Draw bounding box
			rectangle(input_image, Point(left, top), Point(left + width, top + height), color, 3 * THICKNESS);

			// Draw class labels
			DrawLabel(input_image, label, color, left, top);
		}
	}
	catch (const exception& ex)
	{
		cout << "[POST_PROCESS_ERROR] " << ex.what() << endl;
	}

	return input_image;
}

QPixmap Yolo::ProcessImage(const string& path, QLabel* processedTime)
{
	QPixmap pix;
	try
	{
		string filename = path.substr(path.find_last_of('/') + 1);
		Mat rawImg = imread(path, IMREAD_COLOR);

		// Resize image
		int targetWidth = INPUT_WIDTH * 1.5;
		int targetHeight = static_cast<int>((static_cast<float>(targetWidth) / rawImg.cols) * rawImg.rows);
		Mat resizedImg;
		resize(rawImg, resizedImg, Size(targetWidth, targetHeight), INTER_LINEAR);

		// Process the image
		vector<Mat> detections;
		detections = pre_process(resizedImg);
		Mat img = post_process(resizedImg, detections);

		// Label image with processing time
		vector<double> layersTimes;
		double freq = getTickFrequency() / 1000;
		double ms = net.getPerfProfile(layersTimes) / freq;
		string label = format("Processing time: %.2f ms", ms);
		processedTime->setText("Processed Image: " + QString::fromStdString(format("%.2f ms", ms)));
		//putText(resizedImg, label, Point(10, 30), FONT_FACE, FONT_SCALE, Scalar(0, 0, 0), THICKNESS + 1, LINE_AA);
		//putText(resizedImg, label, Point(10, 30), FONT_FACE, FONT_SCALE, Scalar(255, 255, 255), THICKNESS, LINE_AA);
		cvtColor(resizedImg, resizedImg, COLOR_BGR2RGB);
		pix = QPixmap::fromImage(QImage(resizedImg.data, resizedImg.cols, resizedImg.rows, resizedImg.step, QImage::Format_RGB888));
	}
	catch (const exception& ex)
	{
		cout << "[DISPLAY_IMAGE_ERROR] " << ex.what() << endl;
	}

	return pix;
}