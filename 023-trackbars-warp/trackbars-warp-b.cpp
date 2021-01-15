#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <cmath>
#include<windows.h>

using namespace std;
using namespace cv;

//MATRICES: PRIMERO CENTRAS, DESPUES MATRICES TRANSFORMACION, DESPUES DESCENTRAS
//WARPING 1A: 20 Y 128
//WARPING 1B: 20 Y 30
//WARPING 2A: Y0 Y EL 1 DESPUES DEL *
//WARPING 2B: X0 Y0 Y 512

int interpolacionLineal(float px, float py, Mat original) {
	int q11 = 0, q22 = 0, q21 = 0;
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
	return P;
}

int sign(int value) {
	if (value > 0) value = 1;
	if (value == 0) value = 0;
	if (value < 0) value = -1;
	return value;
}

void escalamiento(float Cx, float Cy, Mat interp, Mat original) {
	for (int i = 0; i < interp.rows; i++) {
		for (int j = 0; j < interp.cols; j++) {
			int q11 = 0, q22 = 0, q21 = 0;
			float px = (j / Cx);
			float py = (i / Cy);
			interp.at<uchar>(i, j) = interpolacionLineal(px, py, original);
		}
	}
}
void traslacion(float tx, float ty, Mat trasladada, Mat original) {
	for (int i = 0; i < trasladada.rows; i++) {
		for (int j = 0; j < trasladada.cols; j++) {
			float u = int(j - tx);
			float v = int(i - ty);
			if (u >= original.cols || u < 0 || v >= original.rows || v < 0) continue;
			trasladada.at<uchar>(i, j) = interpolacionLineal(u, v, original);
		}
	}
}
void rotacion(int angulo, Mat rotada, Mat original) {
	float rads = (angulo * 3.1416) / 180.0f;
	for (int i = 0; i < rotada.rows; i++) {
		for (int j = 0; j < rotada.cols; j++) {
			int x0 = j - (rotada.cols / 2);
			int y0 = (rotada.rows / 2) - i;

			float u0 = x0 * cos(rads) + y0 * sin(rads);
			float v0 = -1 * x0 * sin(rads) + y0 * cos(rads);

			int u = u0 + (rotada.cols / 2);
			int v = (rotada.rows / 2) - v0;

			if (u >= original.cols || u < 0 || v >= original.rows || v < 0) continue;
			rotada.at<uchar>(i, j) = interpolacionLineal(u, v, original);
		}
	}
}
void sesgadoV(float factor, Mat sesV, Mat original) {
	for (int i = 0; i < sesV.rows; i++) {
		for (int j = 0; j < sesV.cols; j++) {
			int x0 = j - (sesV.cols / 2);
			int y0 = (sesV.rows / 2) - i;

			int u = j;
			int v = i - (factor*u);

			if (u >= original.cols || u < 0 || v >= original.rows || v < 0) continue;
			sesV.at<uchar>(i, j) = interpolacionLineal(u, v, original);
		}
	}
}
void sesgadoH(float factor, Mat sesH, Mat original) {
	for (int i = 0; i < sesH.rows; i++) {
		for (int j = 0; j < sesH.cols; j++) {
			int x0 = j - (sesH.cols / 2);
			int y0 = (sesH.rows / 2) - i;
			int v = i;
			int u = j - (factor*v);

			if (u >= original.cols || u < 0 || v >= original.rows || v < 0) continue;
			//sesH.at<uchar>(i, j) = original.at<uchar>(v, u);
			sesH.at<uchar>(i, j) = interpolacionLineal(u, v, original);
		}
	}
}
void warping1a(Mat warp1a, Mat original) {
	for (int i = 0; i < warp1a.rows; i++) {
		for (int j = 0; j < warp1a.cols; j++) {
			//centrado:
			int x0 = j - (warp1a.cols / 2);
			int y0 = (warp1a.rows / 2) - i;

			float u0 = x0;
			float v0 = y0 + 20 * sin((2 * 3.1416 * x0) / 128);

			//descentrado:
			float u = u0 + (warp1a.cols / 2);
			float v = (warp1a.rows / 2) - v0;

			if (u >= original.cols || u < 0 || v >= original.rows || v < 0) continue;
			warp1a.at<uchar>(i, j) = interpolacionLineal(u, v, original);
		}
	}
}
void warping1b(float amplitud, float frecuencia, Mat warp1b, Mat original) {
	for (int i = 0; i < warp1b.rows; i++) {
		for (int j = 0; j < warp1b.cols; j++) {
			//centrado:
			int x0 = j - (warp1b.cols / 2);
			int y0 = (warp1b.rows / 2) - i;

			float u0 = x0;
			float v0 = y0 + amplitud * sin((2 * 3.1416 * y0) / frecuencia);

			//descentrado:
			float u = u0 + (warp1b.cols / 2);
			float v = (warp1b.rows / 2) - v0;

			if (u >= original.cols || u < 0 || v >= original.rows || v < 0) continue;
			warp1b.at<uchar>(i, j) = interpolacionLineal(u, v, original);
		}
	}
}
void warping2a(Mat warp2a, Mat original) {
	for (int i = 1; i < warp2a.rows; i++) {
		for (int j = 1; j < warp2a.cols; j++) {
			//centrado:
			int x0 = j - (warp2a.cols / 2);
			int y0 = (warp2a.rows / 2) - i;

			float v0 = sign(y0 - i)*(((y0 - i)*(y0 - i)) / i) + i;
			float u0 = x0;

			//descentrado:
			float u = u0 + (warp2a.cols / 2);
			float v = (warp2a.rows / 2) - v0;

			if (u >= original.cols || u < 0 || v >= original.rows || v < 0) continue;
			warp2a.at<uchar>(i, j) = interpolacionLineal(u, v, original);
		}
	}
}
void warping2b(float x, float y, Mat warp2b, Mat original) {
	for (int i = 0; i < warp2b.rows; i++) {
		for (int j = 0; j < warp2b.cols; j++) {
			//centrado:
			int x0 = j - (warp2b.cols / 2);
			int y0 = (warp2b.rows / 2) - i;

			float r = sqrt((x - x0)*(x - x0) + (y - y0)*(y - y0));
			float theta = (3.1416 * r) / 512;
			float u0 = (x - x0)*cos(theta) + (y - y0)*sin(theta) + x0;
			float v0 = -1 * (x - x0)*sin(theta) + (y - y0)*cos(theta) + y0;

			//descentrado:
			float u = u0 + (warp2b.cols / 2);
			float v = (warp2b.rows / 2) - v0;

			if (u >= original.cols || u < 0 || v >= original.rows || v < 0) continue;
			warp2b.at<uchar>(i, j) = interpolacionLineal(u, v, original);
		}
	}
}

int maxAmplitud = 80;
float amplitud = 1;
int slider_A = 1;

int maxFrecuencia = 120;
float frecuencia = 1;
int slider_F = 1;

void on_trackbar(int, void*) {

	amplitud = (float)slider_A / maxAmplitud;
	amplitud = amplitud * 100;
	frecuencia = (float)slider_F / maxFrecuencia;
	frecuencia = frecuencia * 100;
}

int main() {
	Mat original = imread("wasp.png", CV_LOAD_IMAGE_GRAYSCALE);
	Mat original2 = imread("wasp.png", CV_LOAD_IMAGE_GRAYSCALE);
	namedWindow("trackbars", WINDOW_AUTOSIZE);
	namedWindow("view", WINDOW_AUTOSIZE);

	createTrackbar("Amplitud", "trackbars", &slider_A, maxAmplitud, on_trackbar);
	createTrackbar("Frecuencia", "trackbars", &slider_F, maxFrecuencia, on_trackbar);

	while (1) {
		if (GetKeyState('S') & 0x8000) {
			Mat warp1b(int(original.rows), int(original.cols), CV_8UC1, Scalar(0));

			warping1b(amplitud, frecuencia, warp1b, original);

			imshow("view", warp1b);
		}
		if (GetKeyState(VK_RETURN) & 0x8000) break;
		if (waitKey(100) >= 0) continue;
	}

}