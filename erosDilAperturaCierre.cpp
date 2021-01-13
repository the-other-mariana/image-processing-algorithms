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

Mat binary = imread("binary.jpg", CV_LOAD_IMAGE_GRAYSCALE);
Mat rErosion(binary.rows, binary.cols, CV_8UC1, Scalar(0));
Mat rDilatacion(binary.rows, binary.cols, CV_8UC1, Scalar(0));

int mask[3][3] = { { 0,1,0 },{ 1,1,1 },{ 0,1,0 } };
int innerMtx = 3;

// con que uno de los bracitos del elemento este fuera de la mancha, ya no se pinta el origen
bool erosion(int r, int c, Mat binary) {
	int y = 0;
	for (int i = r - int(innerMtx / 2); i <= r + int(innerMtx / 2); i++) {
		int x = 0;
		for (int j = c - int(innerMtx / 2); j <= c + int(innerMtx / 2); j++) {
			if (i < 0 or i >= binary.rows or j < 0 or j >= binary.cols) {
				continue;
			}
			else {
				if (mask[y][x] == 1) {
					if (binary.at<uchar>(i, j) == 0) {
						return false;
					}
				}
			}
			x++;
		}
		y++;
	}
	return true;
}

// con que uno de los bracitos del elemento este dentro de la mancha, se pinta ese origen
bool dilatacion(int r, int c, Mat binary) {
	int y = 0;
	for (int i = r - int(innerMtx / 2); i <= r + int(innerMtx / 2); i++) {
		int x = 0;
		for (int j = c - int(innerMtx / 2); j <= c + int(innerMtx / 2); j++) {
			if (i < 0 or i >= binary.rows or j < 0 or j >= binary.cols) {
				continue;
			}
			else {
				if (mask[y][x] == 1) {
					if (binary.at<uchar>(i, j) == 255) {
						return true;
					}
				}
			}
			x++;
		}
		y++;
	}
	return false;
}

int main() {

	Mat binary = imread("binary.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat rErosion(binary.rows, binary.cols, CV_8UC1, Scalar(0));
	Mat rDilatacion(binary.rows, binary.cols, CV_8UC1, Scalar(0));
	// umbral
	for (int i = 0; i < binary.rows; i++) {
		for (int j = 0; j < binary.cols; j++) {
			if (binary.at<uchar>(i, j) >= 127) {
				binary.at<uchar>(i, j) = 255;
			}
			else {
				binary.at<uchar>(i, j) = 0;
			}
		}
	}
	//---------------------------------------------- EROSION
	for (int i = 0; i < binary.rows; i++) {
		for (int j = 0; j < binary.cols; j++) {
			if (!erosion(i, j, binary)) {
				rErosion.at<uchar>(i, j) = 0;
			}
			else {
				rErosion.at<uchar>(i, j) = 255;
			}
			//namedWindow("Resultado", WINDOW_AUTOSIZE);
			//imshow("Resultado", result);
			//waitKey(1);
		}
	}

	//---------------------------------------------- DILATACION
	for (int i = 0; i < binary.rows; i++) {
		for (int j = 0; j < binary.cols; j++) {
			if (dilatacion(i, j, binary)) {
				rDilatacion.at<uchar>(i, j) = 255;
			}
			else {
				rDilatacion.at<uchar>(i, j) = 0;
			}
		}
	}
	
	//---------------------------------------------- CIERRE
	//primero dilatas y luego erosionas
	Mat binary2 = imread("binary.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat dCierre(binary.rows, binary.cols, CV_8UC1, Scalar(0));
	Mat cierre(binary.rows, binary.cols, CV_8UC1, Scalar(0));
	Mat opening(binary.rows, binary.cols, CV_8UC1, Scalar(0));
	Mat eOpening(binary.rows, binary.cols, CV_8UC1, Scalar(0));
	// umbral
	for (int i = 0; i < binary.rows; i++) {
		for (int j = 0; j < binary.cols; j++) {
			if (binary2.at<uchar>(i, j) >= 127) {
				binary2.at<uchar>(i, j) = 255;
			}
			else {
				binary2.at<uchar>(i, j) = 0;
			}
		}
	}
	//sobre binary2 dilatas, res en dCierre
	for (int i = 0; i < binary.rows; i++) {
		for (int j = 0; j < binary.cols; j++) {
			if (dilatacion(i, j, binary2)) {
				dCierre.at<uchar>(i, j) = 255;
			}
			else {
				dCierre.at<uchar>(i, j) = 0;
			}
		}
	}
	//sobre dCierre erosionas, res en cierre
	for (int i = 0; i < binary.rows; i++) {
		for (int j = 0; j < binary.cols; j++) {
			if (!erosion(i, j, dCierre)) {
				cierre.at<uchar>(i, j) = 0;
			}
			else {
				cierre.at<uchar>(i, j) = 255;
			}
		}
	}
	//---------------------------------------- APERTURA
	//primero erosionas y luego dilatas
	//sobre binary2 erosionas, res es eOpening
	for (int i = 0; i < binary.rows; i++) {
		for (int j = 0; j < binary.cols; j++) {
			if (!erosion(i, j, binary2)) {
				eOpening.at<uchar>(i, j) = 0;
			}
			else {
				eOpening.at<uchar>(i, j) = 255;
			}
		}
	}
	//sobre eOpening dilatas, res en opening
	for (int i = 0; i < binary.rows; i++) {
		for (int j = 0; j < binary.cols; j++) {
			if (dilatacion(i, j, eOpening)) {
				opening.at<uchar>(i, j) = 255;
			}
			else {
				opening.at<uchar>(i, j) = 0;
			}
		}
	}
	//mascara de lado 7 y poner los 4 resultados con el elem estructural
	namedWindow("Original", WINDOW_AUTOSIZE);
	imshow("Original", binary);

	namedWindow("erosion", WINDOW_AUTOSIZE);
	imshow("erosion", rErosion);

	namedWindow("dilatacion", WINDOW_AUTOSIZE);
	imshow("dilatacion", rDilatacion);

	namedWindow("cierre", WINDOW_AUTOSIZE);
	imshow("cierre", cierre);

	namedWindow("opening", WINDOW_AUTOSIZE);
	imshow("opening", opening);

	waitKey(0);
}