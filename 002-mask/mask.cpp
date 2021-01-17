#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <cmath>

using namespace std;
using namespace cv;

int main() {
	Mat original = imread("dice.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat mask = imread("dice.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat result = imread("dice.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat pattern = imread("pattern.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	int thresh_value = 200;

	//create mask: binary filter
	for (int i = 0; i < mask.rows; i++) {
		for (int j = 0; j < mask.cols; j++) {
			if (mask.at<uchar>(i, j) > thresh_value) {
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
	namedWindow("Mask", WINDOW_AUTOSIZE);
	imshow("Mask", mask);
	namedWindow("Result", WINDOW_AUTOSIZE);
	imshow("Result", result);
	namedWindow("Pattern", WINDOW_AUTOSIZE);
	imshow("Pattern", pattern);
	waitKey(0);
}