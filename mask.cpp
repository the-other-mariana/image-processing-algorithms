#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <cmath>

using namespace std;
using namespace cv;

int main() {
	Mat original = imread("dados.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat mask = imread("dados.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat result = imread("dados.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat pattern = imread("pattern.png", CV_LOAD_IMAGE_GRAYSCALE);

	//crear mascara: filtro binary
	for (int i = 0; i < mask.rows; i++) {
		for (int j = 0; j < mask.cols; j++) {
			if (mask.at<uchar>(i, j) > 200) {
				mask.at<uchar>(i, j) = (uchar)(255);
			}
			else {
				mask.at<uchar>(i, j) = (uchar)(0);
			}
		}
	}

	for (int i = 0; i < result.rows; i++) {
		for (int j = 0; j < result.cols; j++) {
			if (mask.at<uchar>(i, j) == 255) {
				result.at<uchar>(i, j) = pattern.at<uchar>(i, j);
			}
		}
	}
	namedWindow("mask", WINDOW_AUTOSIZE);
	imshow("mask", mask);
	namedWindow("result", WINDOW_AUTOSIZE);
	imshow("result", result);
	waitKey(0);
}