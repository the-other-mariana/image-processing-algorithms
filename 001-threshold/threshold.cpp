#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <cmath>

using namespace std;
using namespace cv;

int main() {

	string imgname = "lena.png";
	int arr[256] = { 0 };

	Mat original = imread(imgname, CV_LOAD_IMAGE_GRAYSCALE);
	Mat image = imread(imgname, CV_LOAD_IMAGE_GRAYSCALE);

	//binary
	Mat image2 = imread(imgname, CV_LOAD_IMAGE_GRAYSCALE);
	for (int i = 0; i < image2.rows; i++) {
		for (int j = 0; j < image2.cols; j++) {
			if (image2.at<uchar>(i, j) > 80) {
				image2.at<uchar>(i, j) = 255;
			}
			else {
				image2.at<uchar>(i, j) = 0;
			}
		}
	}
	//binary inverse
	Mat image3 = imread(imgname, CV_LOAD_IMAGE_GRAYSCALE);
	for (int i = 0; i < image2.rows; i++) {
		for (int j = 0; j < image2.cols; j++) {
			if (image3.at<uchar>(i, j) > 80) {
				image3.at<uchar>(i, j) = 0;
			}
			else {
				image3.at<uchar>(i, j) = 255;
			}
		}
	}
	//trunc
	Mat image4 = imread(imgname, CV_LOAD_IMAGE_GRAYSCALE);
	for (int i = 0; i < image4.rows; i++) {
		for (int j = 0; j < image4.cols; j++) {
			if (image4.at<uchar>(i, j) > 80) {
				image4.at<uchar>(i, j) = 255;
			}
			else {

			}
		}
	}
	//tozero
	Mat image5 = imread(imgname, CV_LOAD_IMAGE_GRAYSCALE);
	for (int i = 0; i < image5.rows; i++) {
		for (int j = 0; j < image5.cols; j++) {
			if (image5.at<uchar>(i, j) > 80) {

			}
			else {
				image5.at<uchar>(i, j) = 0;
			}
		}
	}
	//tozero inv
	Mat image6 = imread(imgname, CV_LOAD_IMAGE_GRAYSCALE);
	for (int i = 0; i < image6.rows; i++) {
		for (int j = 0; j < image6.cols; j++) {
			if (image6.at<uchar>(i, j) > 80) {
				image6.at<uchar>(i, j) = 0;
			}
			else {

			}
		}
	}

	//histogram array: each color value, how many times it repeats itself

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

	// ------------------------------	incremento 1	
	int alto = 750;
	int ancho = 750;
	Mat myMat1(alto, ancho, CV_32F, Scalar(0));

	int inc = ancho / 256;
	for (int i = 0; i < 255; i++) {
		rectangle(myMat1, Point(inc * i, myMat1.rows), Point((inc*(i + 1) - 1), myMat1.rows - ((arr[i] * myMat1.rows) / (max))), Scalar(255, 255, 255, 0), CV_FILLED);
	}

	//-------------------------------     incremento 2
	Mat myMat2(alto, ancho, CV_32F, Scalar(0));

	int inc2 = ancho / 256;
	for (int i = 0; i < 255; i += 2) {
		rectangle(myMat2, Point(inc * i, myMat2.rows), Point((inc*(i + 2) - 1), myMat2.rows - (((arr[i] + arr[i + 1]) * myMat2.rows) / (2 * max))), Scalar(255, 255, 255, 0), CV_FILLED);
	}


	//-------------------------------     incremento 4
	Mat myMat4(alto, ancho, CV_32F, Scalar(0));

	int inc4 = ancho / 256;
	for (int i = 0; i < 255; i += 4) {
		rectangle(myMat4, Point(inc * i, myMat4.rows), Point((inc*(i + 4) - 1), myMat4.rows - (((arr[i] + arr[i + 1] + arr[i + 2] + arr[i + 3]) * myMat4.rows) / (4 * max))), Scalar(255, 255, 255, 0), CV_FILLED);
	}

	//--------------square
	Mat square(500, 500, CV_8UC1, Scalar(0));
	int border = 50;
	for (int i = 0; i < square.rows; i++) {
		for (int j = 0; j < square.cols; j++) {
			if ((i > border && i < square.rows - border) && (j > border && j < square.cols - border)) {
				square.at<uchar>(i, j) = (uchar)(255);
			}
		}
	}
	//filters

	namedWindow("original", WINDOW_AUTOSIZE);
	imshow("original", original);
	namedWindow("binary inv", WINDOW_AUTOSIZE);
	imshow("binary inv", image3);
	namedWindow("threshold", WINDOW_AUTOSIZE);
	imshow("threshold", image4);
	namedWindow("tozero", WINDOW_AUTOSIZE);
	imshow("tozero", image5);
	namedWindow("tozero inv", WINDOW_AUTOSIZE);
	imshow("tozero inv", image6);
	//histograms
	namedWindow("Display window", WINDOW_AUTOSIZE);
	imshow("Display window", myMat1);
	namedWindow("Display window2", WINDOW_AUTOSIZE);
	imshow("Display window2", myMat2);
	namedWindow("Display window4", WINDOW_AUTOSIZE);
	imshow("Display window4", myMat4);
	namedWindow("binary", WINDOW_AUTOSIZE);
	imshow("binary", image2);

	namedWindow("square", WINDOW_AUTOSIZE);
	imshow("square", square);
	waitKey(0);

}