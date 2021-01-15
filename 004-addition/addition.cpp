#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <cmath>

using namespace std;
using namespace cv;

int main() {
	Mat robot = imread("robot.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat house = imread("house.jpg", CV_LOAD_IMAGE_GRAYSCALE);


	int rows = 0;
	if (robot.rows > house.rows) {
		rows = house.rows;
	}
	else {
		rows = robot.rows;
	}

	int cols = 0;
	if (robot.cols > house.cols) {
		cols = house.cols;
	}
	else {
		cols = robot.cols;
	}

	Mat result(rows, cols, CV_8UC1, Scalar(0));
	Mat cte(rows, cols, CV_8UC1, Scalar(0));
	Mat grad(rows, cols, CV_8UC1, Scalar(0));
	Mat result2(rows, cols, CV_8UC1, Scalar(0));
	Mat result3(rows, cols, CV_8UC1, Scalar(0));
	Mat result4(rows, cols, CV_8UC1, Scalar(0));

	

	int k = 30;
	// combinar imagenes
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			result.at<uchar>(i, j) = (uchar)((robot.at<uchar>(i, j) + house.at<uchar>(i, j)) / 2);
		}
	}
	//sumar constante
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			
			if (robot.at<uchar>(i, j) + k > 255) {
				cte.at<uchar>(i, j) = uchar(255);
			}
			else {
				cte.at<uchar>(i, j) = (robot.at<uchar>(i, j) + k);
			}
		}
	}
	// degradado
	float dx = 255.0f / grad.cols;
	float r = 0;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			r = j * dx;
			grad.at<uchar>(i, j) = r;

		}
	}
	//suma del degradado (promedio)
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			result2.at<uchar>(i, j) = (robot.at<uchar>(i, j) + grad.at<uchar>(i, j))/2;
		}
	}
	//suma directa
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			result3.at<uchar>(i, j) = (robot.at<uchar>(i, j) + grad.at<uchar>(i, j));
			if (robot.at<uchar>(i, j) + grad.at<uchar>(i,j) > 255) {
				result3.at<uchar>(i, j) = uchar(255);
			}
			else {
				result3.at<uchar>(i, j) = robot.at<uchar>(i, j) + grad.at<uchar>(i, j);
			}
		}
	}
	//suma promedio degradado
	float a = 0;
	float b = 1;
	float inc = 1.0 / result4.rows;
	for (int i = 0; i < rows; i++) {
		if (a + inc > 1) {
			a = 1;
		}
		else {
			a += inc;
		}

		if (b - inc < 0) {
			b = 0;
		}
		else {
			b -= inc;
		}
		for (int j = 0; j < cols; j++) {
			
			result4.at<uchar>(i, j) = a*robot.at<uchar>(i, j) + b*house.at<uchar>(i, j);
		}
	}

	namedWindow("addition", WINDOW_AUTOSIZE);
	imshow("addition", result);
	namedWindow("constant", WINDOW_AUTOSIZE);
	imshow("constant", cte);
	namedWindow("degrad", WINDOW_AUTOSIZE);
	imshow("degrad", grad);
	namedWindow("suma promedio", WINDOW_AUTOSIZE);
	imshow("suma promedio", result2);
	namedWindow("robot", WINDOW_AUTOSIZE);
	imshow("robot", robot);
	namedWindow("suma directa", WINDOW_AUTOSIZE);
	imshow("suma directa", result3);
	namedWindow("suma promedio degradado", WINDOW_AUTOSIZE);
	imshow("suma promedio degradado", result4);
	waitKey(0);

}