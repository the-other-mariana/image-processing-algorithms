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
void dotProduct2D(float v[2], float matrix[2][2], int coord[2]) {
	int dim = 2;
	for (int r = 0; r < dim; r++) {
		v[r] = 0;
		for (int c = 0; c < dim; c++) {
			v[r] += matrix[r][c] * coord[c];
		}
	}

}
void dotProduct3D(float v[3], float matrix[3][3], int coord[3]) {
	int dim = 2;
	for (int r = 0; r < dim + 1; r++) {
		v[r] = 0;
		for (int c = 0; c < dim + 1; c++) {
			v[r] += matrix[r][c] * coord[c];
		}
	}
}
void transformMatrix(float result[3][3], float A[3][3], float B[3][3]) {
	int dim = 2;
	for (int r = 0; r < dim + 1; r++) {
		for (int c = 0; c < dim + 1; c++) {
			for (int k = 0; k < dim + 1; k++) {
				result[r][c] += A[r][k] * B[k][c];
			}
		}
	}
}
void escalamiento(float Cx, float Cy,Mat interp, Mat original) {
	float vector[3] = {0};
	float matrix[3][3] = { {1 / Cx, 0, 0},
							{0, 1 / Cy, 0}, 
							{0, 0, 1} };
	int coord[3] = { 0 };
	for (int i = 0; i < interp.rows; i++) {
		for (int j = 0; j < interp.cols; j++) {
			coord[0] = j;
			coord[1] = i;
			coord[2] = 1;
			dotProduct3D(vector, matrix, coord);
			interp.at<uchar>(i, j) = interpolacionLineal(vector[0], vector[1], original);
		}
	}
}
void traslacion(float tx, float ty, Mat trasladada, Mat original) {
	float vector[3] = { 0 };
	float matrix[3][3] = { {1, 0, tx},
							{0, 1, ty}, 
							{0, 0, 1} };
	int coord[3] = { 0 };
	for (int i = 0; i < trasladada.rows; i++) {
		for (int j = 0; j < trasladada.cols; j++) {
			coord[0] = j;
			coord[1] = i;
			coord[2] = 1;
			dotProduct3D(vector, matrix, coord);
			float u = vector[0] ;
			float v = vector[1] ;
			if (u >= original.cols || u < 0 || v >= original.rows || v < 0) continue;
			trasladada.at<uchar>(i, j) = interpolacionLineal(u, v, original);
		}
	}
}
void rotacion(int angulo, Mat rotada, Mat original) {

	float rads = (angulo * 3.1416) / 180.0f;
	float vector[3] = { 0 };
	float matrix[3][3] = { {cos(rads), sin(rads), 0}, 
							{-1 * sin(rads), cos(rads), 0}, 
							{0, 0, 1} };
	
	int coord[3] = { 0 };
	for (int i = 0; i < rotada.rows; i++) {
		for (int j = 0; j < rotada.cols; j++) {
			
			float x0 = j - (rotada.cols / 2);
			float y0 = (rotada.rows / 2) - i;
			coord[0] = x0;
			coord[1] = y0;
			coord[2] = 1;
			dotProduct3D(vector, matrix, coord);
			float u = vector[0] + (rotada.cols / 2);
			float v = (rotada.rows / 2) - vector[1];

			if (u >= original.cols || u < 0 || v >= original.rows || v < 0) continue;
			rotada.at<uchar>(i, j) = interpolacionLineal(u, v, original);
		}
	}
}
void sesgadoV(float factor, Mat sesV, Mat original) {
	float vector[3] = { 0 };
	float matrix[3][3] = { {1, 0 ,0},{factor, 1 , 0}, {0,0, 1} };
	int coord[3] = { 0 };
	for (int i = 0; i < sesV.rows; i++) {
		for (int j = 0; j < sesV.cols; j++) {
			int x0 = j - (sesV.cols / 2);
			int y0 = (sesV.rows / 2) - i;
			
			coord[0] = x0;
			coord[1] = y0;
			coord[2] = 1;
			dotProduct3D(vector, matrix, coord);
			float u = vector[0] + (sesV.cols / 2);
			float v = (sesV.rows / 2) - vector[1];
			
			if (u >= original.cols || u < 0 || v >= original.rows || v < 0) continue;
			sesV.at<uchar>(i, j) = interpolacionLineal(u, v, original);
		}
	}
}
void sesgadoH(float factor, Mat sesH, Mat original) {
	float vector[3] = { 0 };
	float matrix[3][3] = { {1, factor ,0},{0, 1 , 0}, {0,0, 1} };
	int coord[3] = { 0 };
	for (int i = 0; i < sesH.rows; i++) {
		for (int j = 0; j < sesH.cols; j++) {
			int x0 = j - (sesH.cols / 2);
			int y0 = (sesH.rows / 2) - i;

			coord[0] = x0;
			coord[1] = y0;
			coord[2] = 1;
			dotProduct3D(vector, matrix, coord);
			float u = vector[0] + (sesH.cols / 2);
			float v = (sesH.rows / 2) - vector[1];

			if (u >= original.cols || u < 0 || v >= original.rows || v < 0) continue;
			//sesH.at<uchar>(i, j) = original.at<uchar>(v, u);
			sesH.at<uchar>(i, j) = interpolacionLineal(u, v, original);
		}
	}
}

void applyTransformMatrix(float vector[3], Mat scaled, float transformMtx[3][3], int coord[3], Mat original) {
	for (int i = 0; i < scaled.rows; i++) {
		for (int j = 0; j < scaled.cols; j++) {
			float x0 = j - (scaled.cols / 2);
			float y0 = (scaled.rows / 2) - i;
			coord[0] = x0;
			coord[1] = y0;
			coord[2] = 1;
			dotProduct3D(vector, transformMtx, coord);
			float u = vector[0] + (scaled.cols / 2);
			float v = (scaled.rows / 2) - vector[1];

			if (u >= original.cols || u < 0 || v >= original.rows || v < 0) continue;
			scaled.at<uchar>(i, j) = interpolacionLineal(u, v, original);
		}
	}

}

int maxCx = 10;
float Cx = 1;
int slider_Cx = 1;

int maxCy = 10;
float Cy = 1;
int slider_Cy = 1;

int maxTx = 150;
int maxTy = 150;
int Tx = 50;
int Ty = 50;

int angulo = 15;
int maxAngulo = 350;

int maxSx = 500;
float factorSx;
int slider_Sx;

int maxSy = 500;
float factorSy;
int slider_Sy;

void on_trackbar(int, void*) {
	Cx = (float)slider_Cx / maxCx;
	if (Cx >= 0.5) Cx = Cx * 2;

	Cy = (float)slider_Cy / maxCy;
	if (Cy >= 0.5) Cy = Cy * 2;

	factorSx = (float)slider_Sx / maxSx;
	factorSy = (float)slider_Sy / maxSy;
}

int main() {

	Mat original = imread("wasp.png", CV_LOAD_IMAGE_GRAYSCALE);
	namedWindow("trackbars", WINDOW_AUTOSIZE);
	namedWindow("view", WINDOW_AUTOSIZE);
	createTrackbar("Cx", "trackbars", &slider_Cx, maxCx, on_trackbar);
	createTrackbar("Cy", "trackbars", &slider_Cy, maxCy, on_trackbar);
	createTrackbar("Tx", "trackbars", &Tx, maxTx, on_trackbar);
	createTrackbar("Ty", "trackbars", &Ty, maxTy, on_trackbar);
	createTrackbar("Angle", "trackbars", &angulo, maxAngulo, on_trackbar);
	createTrackbar("Sx", "trackbars", &slider_Sx, maxSx, on_trackbar);
	createTrackbar("Sy", "trackbars", &slider_Sy, maxSy, on_trackbar);

	while (1) {
		if (GetKeyState('S') & 0x8000) {
			float rads = (angulo * 3.1416) / 180.0f;
			float scale[3][3] = { {1 / Cx, 0, 0}, {0, 1 / Cy, 0}, {0, 0, 1} };
			float translate[3][3] = { {1 , 0, Tx }, {0, 1 , Ty }, {0, 0, 1} };
			float rotate[3][3] = { {cos(rads), sin(rads), 0}, {-1 * sin(rads), cos(rads), 0}, {0, 0, 1} };
			float shear[3][3] = { {1, factorSx, 0}, {factorSy, 1, 0}, {0, 0, 1} };

			//order: scale, translate, rotate, shear:
			// result1 = scale x translate
			float result1[3][3] = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0} };
			transformMatrix(result1, scale, translate);

			// result2 = result1 x rotate
			float result2[3][3] = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0} };
			transformMatrix(result2, result1, rotate);

			// result3 = result2 x shear
			float result3[3][3] = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0} };
			transformMatrix(result3, result2, shear);

			// final matrix obtained in result3, apply dotProduct3D:
			Mat scaled(int(original.rows*Cy), int(original.cols*Cx), CV_8UC1, Scalar(0));
			float vector[3] = { 0 };
			int coord[3] = { 0 };

			applyTransformMatrix(vector, scaled, result3, coord, original);

			imshow("view", scaled);
		}
		if (GetKeyState(VK_RETURN) & 0x8000) break;
		if (waitKey(100) >= 0) continue;
	}
	
}