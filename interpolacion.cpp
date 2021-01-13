#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <cmath>

using namespace std;
using namespace cv;

int main() {

	float cx = 3;
	float cy = 3;
	Mat original = imread("ghost.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat escalada(int(original.rows*cy), int(original.cols*cx), CV_8UC1, Scalar(0));

	//------------------------------- ESCALAMIENTO 
	for (int i = 0; i < escalada.rows; i++) {
		for (int j = 0; j < escalada.cols; j++) {
			int	u = int(j / cx);
			int v = int(i / cy);
			if (u > original.cols || u < 0 || v > original.rows || v < 0) continue;
			escalada.at<uchar>(i, j) = original.at<uchar>(v, u);
		}
	}

	//------------------------------- ESCALAMIENTO CON INTERPOLACION PARA NO PIXELEAR
	float ix = 3;
	float iy = 3;
	Mat interp(int(original.rows*iy), int(original.cols*ix), CV_8UC1, Scalar(0));

	for (int i = 0; i < interp.rows; i++) {
		for (int j = 0; j < interp.cols; j++) {
			int q11 = 0, q22 = 0, q21 = 0;
			float px = (j / ix);
			float py = (i / iy);
			
			int q12 = original.at<uchar>(int(py), int(px));
			if (int(py + 1) < original.rows) q11 = original.at<uchar>(int(py + 1), int(px));
			if (int(px + 1) < original.cols) q22 = original.at<uchar>(int(py), int(px + 1));
			if (int(px + 1) < original.cols && int(py + 1) < original.rows) q21 = original.at<uchar>(int(py + 1), int(px + 1));
			
			int x1 = int(px);
			int x2 = int(px + 1);
			int y1 = int(py + 1);
			int y2 = int(py);

			int r1 = ((q21 - q11) / (x2 - x1)) * (px - x1) + q11;
			int r2 = ((q22 - q12) / (x2 - x1)) * (px - x1) + q12;

			int P = ((r2 - r1) / (y2 - y1)) * (py - y1) + r1;
			interp.at<uchar>(i, j) = P;
		}
	}

	namedWindow("original", WINDOW_AUTOSIZE);
	imshow("original", original);

	namedWindow("escalado simple", WINDOW_AUTOSIZE);
	imshow("escalado simple", escalada);

	namedWindow("interpolacion", WINDOW_AUTOSIZE);
	imshow("interpolacion", interp);
	waitKey(0);
}