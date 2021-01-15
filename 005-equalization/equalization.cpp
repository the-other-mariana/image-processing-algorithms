#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <cmath>

using namespace std;
using namespace cv;

int main() {

	int arr[256] = { 0 };
	float arr2[256] = { 0 };
	float arr3[256] = { 0 };


	Mat image = imread("normal.jpg", CV_LOAD_IMAGE_GRAYSCALE);


	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			int index = (int)(image.at<uchar>(i, j));
			arr[index]++;
		}
	}

	int max = 0;

	for (int i = 0; i < 255; i++) {
		if (arr[i] > max) {
			max = arr[i];
		}
	}


	int alto = image.rows;
	int ancho = image.cols;
	Mat myMat1(alto, ancho, CV_8U, Scalar(0));

	Mat histog(750, 750, CV_8U, Scalar(0));

	int inc = 750 / 256;


	// histograma original
	for (int i = 0; i < 255; i++) {
		rectangle(histog, Point(inc * i, histog.rows), Point((inc*(i + 1) - 1), histog.rows - ((arr[i] * histog.rows) / (max))), Scalar(255, 255, 255, 0), CV_FILLED);
	}

	//-- PMF
	float total = image.cols*image.rows;
	for (int i = 0; i < 255; i++)
	{
		arr2[i] = float(arr[i]) / total;
	}
	arr3[0] = arr2[0];

	//-- CDF
	for (int i = 1; i < 255; i++)
	{
		arr3[i] = arr2[i] + arr3[i - 1];
	}

	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {

			myMat1.at<uchar>(i, j) = floor((256 - 1)*arr3[image.at<uchar>(i, j)]);

		}
	}

	// histograma 2
	int h2[256] = { 0 };


	for (int i = 0; i < myMat1.rows; i++) {
		for (int j = 0; j < myMat1.cols; j++) {
			int index = (int)(myMat1.at<uchar>(i, j));
			h2[index]++;
		}
	}

	int maxH2 = 0;

	for (int i = 0; i < 255; i++) {
		if (h2[i] > maxH2) {
			maxH2 = h2[i];
		}
	}


	Mat histog2(750, 750, CV_8U, Scalar(0));

	inc = 750 / 256;


	// histograma original
	for (int i = 0; i < 255; i++) {
		rectangle(histog2, Point(inc * i, histog2.rows), Point((inc*(i + 1) - 1), histog2.rows - ((h2[i] * histog2.rows) / (maxH2))), Scalar(255, 255, 255, 0), CV_FILLED);
	}

	namedWindow("original", WINDOW_AUTOSIZE);
	imshow("original", image);

	namedWindow("Histograma", WINDOW_AUTOSIZE);
	imshow("Histograma", histog);

	namedWindow("Histograma 2 Ecualizado", WINDOW_AUTOSIZE);
	imshow("Histograma 2 Ecualizado", histog2);

	namedWindow("Ecualizada", WINDOW_AUTOSIZE);
	imshow("Ecualizada", myMat1);
	waitKey(0);


}