#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <cmath>
#include "opencv2/video.hpp"
#include<windows.h>
#include<algorithm>
#include<vector>

using namespace std;
using namespace cv;

int factor = 5;
Mat flowers = imread("buho.jpg", CV_LOAD_IMAGE_GRAYSCALE);
Mat result(flowers.rows, flowers.cols, CV_8UC1, Scalar(0));
Mat bordes(flowers.rows, flowers.cols, CV_8UC1, Scalar(0));
Mat penguin = imread("penguin.png", CV_LOAD_IMAGE_GRAYSCALE);
Mat limpia(penguin.rows, penguin.cols, CV_8UC1, Scalar(0));

int average(int r, int c) {
	int value = 0;
	for (int i = r - int(factor / 2); i < (r + int(factor / 2) + 1); i++) {
		for (int j = c - int(factor / 2); j < (c + int(factor / 2)+1); j++) {
			if (i < 0 or i >= flowers.rows or j < 0 or j >= flowers.cols) {
				continue;
			}
			else {
				value += int(flowers.at<uchar>(i, j));
			}
		}
	}
	int res = 0;
	res = int(value / (factor*factor));
	return res;
}

int median(int r, int c) {
	int value = 0;
	vector<int> v;
	int res = 0;
	int index = 0;
	for (int i = r - int(factor / 2); i < (r + int(factor / 2) + 1); i++) {
		for (int j = c - int(factor / 2); j < (c + int(factor / 2) + 1); j++) {
			if (i < 0 or i >= penguin.rows or j < 0 or j >= penguin.cols) {
				continue;
			}
			else {
				v.push_back(penguin.at<uchar>(i, j));
			}
		}
	}
	sort(v.begin(), v.end());
	index = (int)(v.size() / 2);
	res = v[index];
	return res;
}

int main() {
	//----------------------------------------------------- FILTRO PROMEDIO

	for (int i = 0; i < flowers.rows; i++) {
		for (int j = 0; j < flowers.cols; j++) {
			result.at<uchar>(i, j) = (uchar)(average(i, j));
		}
	}
	//----------------------------------------------------- BORDES
	for (int i = 0; i < flowers.rows; i++) {
		for (int j = 0; j < flowers.cols; j++) {
			bordes.at<uchar>(i, j) = abs(flowers.at<uchar>(i, j) - result.at<uchar>(i, j));
		}
	}

	for (int i = 0; i < flowers.rows; i++) {
		for (int j = 0; j < flowers.cols; j++) {
			if (bordes.at<uchar>(i, j) >= 25) {
				bordes.at<uchar>(i, j) = 255;
			}
			else {
				bordes.at<uchar>(i, j) = 0;
			}
		}
	}
	//----------------------------------------------------- MEDIANA
	for (int i = 0; i < penguin.rows; i++) {
		for (int j = 0; j < penguin.cols; j++) {
			limpia.at<uchar>(i, j) = (uchar)(median(i, j));
		}
	}
	namedWindow("Original", WINDOW_AUTOSIZE);
	imshow("Original", flowers);

	namedWindow("Resultado", WINDOW_AUTOSIZE);
	imshow("Resultado", result);

	namedWindow("Bordes", WINDOW_AUTOSIZE);
	imshow("Bordes", bordes);

	namedWindow("pimienta", WINDOW_AUTOSIZE);
	imshow("pimienta", penguin);

	namedWindow("limpia", WINDOW_AUTOSIZE);
	imshow("limpia", limpia);
	waitKey(0);
}
