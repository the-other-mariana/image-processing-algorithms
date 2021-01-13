#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <cmath>
#include "opencv2/video.hpp"
#include<windows.h>;

using namespace std;
using namespace cv;


int main() {
//------------------------------------------	ej 1a
	Mat oriDa = imread("dados.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat chida = imread("cielo.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat chida4 = imread("robot.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat image2;

	VideoCapture cap(0);// abre la cámara de default
	if (!cap.isOpened())//revisa si se abrió correctamente
		return -1;


	float b = 0;
	float a = 1 - b;
	float inc = 1.0f / ((oriDa.cols)*0.5);
	//-------------------------------------------------- ej 1a

	for (;;) {
		Mat frame;

		cap >> frame;//obtiene un nuevo cuadro de la cámara
		cvtColor(frame, image2, COLOR_BGR2GRAY);
		Mat img3(480, 640, CV_8U, Scalar(0));
		for (int i = 0; i < frame.rows; i++) {
			
			for (int j = 0; j < frame.cols; j++) {
				if (j < image2.cols / 2) {
					img3.at<uchar>(i,j) = (1 - b)*image2.at<uchar>(i, j) + (b)*chida.at<uchar>(i, j);
				}
				else {
					img3.at<uchar>(i, j) = chida.at<uchar>(i, j);
				}
				b = inc * j;
			}
		}

		
		imshow("frame", img3);
		if (waitKey(1) >= 0)
			break;
	}
	// --------------------------------------------------- ej 1b

	int arr[256] = { 0 };
	float arr2[256] = { 0 };
	float arr3[256] = { 0 };

	Mat oriDa2 = imread("dados.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat chida2 = imread("cielo.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat image2_2;

	VideoCapture cap2(0);// abre la cámara de default
	if (!cap2.isOpened())//revisa si se abrió correctamente
		return -1;


	float b2 = 0;
	float a2 = 1 - b;
	float inc2 = 1.0f / ((oriDa2.cols)*0.5);
	bool showec = false;

	for (;;) {
		Mat frame;

		cap2 >> frame;//obtiene un nuevo cuadro de la cámara
		cvtColor(frame, image2_2, COLOR_BGR2GRAY);
		Mat img3_2(480, 640, CV_8U, Scalar(0));
		for (int i = 0; i < frame.rows; i++) {

			for (int j = 0; j < frame.cols; j++) {
				if (j < image2.cols / 2) {
					img3_2.at<uchar>(i, j) = (1 - b)*image2_2.at<uchar>(i, j) + (b)*chida.at<uchar>(i, j);
				}
				else {
					img3_2.at<uchar>(i, j) = chida.at<uchar>(i, j);
				}
				b = inc * j;
			}
		}

		// histograma arreglo
		for (int i = 0; i < img3_2.rows; i++) {
			for (int j = 0; j < img3_2.cols; j++) {
				int index = (int)(img3_2.at<uchar>(i, j));
				arr[index]++;
			}
		}
		//histograma max
		int max = 0;
		for (int i = 0; i < 255; i++) {
			if (arr[i] > max) {
				max = arr[i];
			}
		}
		//histograma img
		int alto = 750;
		int ancho = 750;
		Mat h2(alto, ancho, CV_8U, Scalar(0));
		Mat ec(img3_2.rows, img3_2.cols, CV_8U, Scalar(0));
		int inc_h2 = ancho / 256;
		for (int i = 0; i < 255; i++) {
			rectangle(h2, Point(inc_h2 * i, h2.rows), Point((inc_h2*(i + 1) - 1), h2.rows - ((arr[i] * h2.rows) / (max))), Scalar(255, 255, 255, 0), CV_FILLED);
		}
		//ecualizacion
		if (GetKeyState('H') & 0x8000) {
			if (!showec) {
				showec = true;
				float total = img3_2.cols*img3_2.rows;
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

				for (int i = 0; i < ec.rows; i++) {
					for (int j = 0; j < ec.cols; j++) {

						ec.at<uchar>(i, j) = floor((256 - 1)*arr3[img3_2.at<uchar>(i, j)]);

					}
				}
				imshow("ecual", ec);
			}
			else {
				
			}
		}
		

		
		imshow("frame2", img3_2);
		imshow("hist", h2);
		if (GetKeyState(VK_RETURN) & 0x8000) {
			break;
		}
		if (waitKey(10) >= 0)
			continue;
		
	}
	//----------------------------ej 2a
	Mat check1(480, 640, CV_8U, Scalar(0));
	bool hola = true;
	Mat image2_3;
	
	VideoCapture cap3(0);// abre la cámara de default
	if (!cap3.isOpened())//revisa si se abrió correctamente
		return -1;
	int f = 0;
	bool change = false;
	for (;;) {
		Mat frame;
		f += 1;
		if (f % 25 == 0) {
			change = !change;
		}
		cap3 >> frame;//obtiene un nuevo cuadro de la cámara
		cvtColor(frame, image2_3, COLOR_BGR2GRAY);
		Mat img3_3(480, 640, CV_8U, Scalar(0));
		Mat res(480, 640, CV_8U, Scalar(0));

		for (int a = 0; a < check1.rows; a++) {
			if (a % 80 == 0) {
				hola = !hola;
			}
			for (int j = 0; j < check1.cols; j++) {
				if (j % 80 == 0) {
					hola = !hola;
				}
				if (hola) {
					check1.at<uchar>(a, j) = (255);
				}
				else {
					check1.at<uchar>(a, j) = (0);
				}

			}
		}
		for (int i = 0; i < img3_3.rows; i++) {
			for (int j = 0; j < img3_3.cols; j++) {
				
				if (check1.at<uchar>(i, j) == 0 && change == true) {
					image2_3.at<uchar>(i, j) = chida.at<uchar>(i, j);
				}
				else if (check1.at<uchar>(i, j) == 255 && change == false) {
					image2_3.at<uchar>(i, j) = chida.at<uchar>(i, j);
				}
			}
		}

		imshow("checkered", image2_3);
		if (waitKey(1) >= 0)
			break;
	}
	// -------------------------------- ej 2b
	Mat check4(480, 640, CV_8U, Scalar(0));
	bool hola4 = true;
	Mat image2_4;

	VideoCapture cap4(0);// abre la cámara de default
	if (!cap4.isOpened())//revisa si se abrió correctamente
		return -1;
	int f4 = 0;
	bool change4 = false;
	float inc4 = 1.0f / 25.0f;
	int b4 = 0;
	for (;;) {
		Mat frame;
		f4 += 1;
		if (f4 % 25 == 0) {
			change4 = !change4;
		}
		cap4 >> frame;//obtiene un nuevo cuadro de la cámara
		cvtColor(frame, image2_4, COLOR_BGR2GRAY);
		Mat img3_4(480, 640, CV_8U, Scalar(0));
		Mat res4(480, 640, CV_8U, Scalar(0));

		for (int a = 0; a < check4.rows; a++) {
			if (a % 80 == 0) {
				hola4 = !hola4;
			}
			for (int j = 0; j < check4.cols; j++) {
				if (j % 80 == 0) {
					hola4 = !hola4;
				}
				if (hola4) {
					check4.at<uchar>(a, j) = (255);
				}
				else {
					check4.at<uchar>(a, j) = (0);
				}
			}
		}
		int p = f % 25;

		for (int i = 0; i < img3_4.rows; i++) {
			for (int j = 0; j < img3_4.cols; j++) {
				
				if (check4.at<uchar>(i, j) == 0 && change4 == true) {
					image2_4.at<uchar>(i, j) = (1-p)*inc4*image2_4.at<uchar>(i, j) + p*inc4*chida4.at<uchar>(i, j);
				}
				else if (check4.at<uchar>(i, j) == 255 && change4 == false) {
					image2_4.at<uchar>(i, j) = (1 - p)*inc4*image2_4.at<uchar>(i,j) + p*inc4*chida4.at<uchar>(i, j);
				}
			}
		}

		imshow("checkered", image2_4);
		if (waitKey(1) >= 0)
			break;
	}
}


/*


Mat oriDa = imread("dados.jpg", CV_LOAD_IMAGE_GRAYSCALE);
Mat masc = imread("dados.jpg", CV_LOAD_IMAGE_GRAYSCALE);
Mat chida = imread("cielo.jpg", CV_LOAD_IMAGE_GRAYSCALE);

for (int i = 0; i < masc.rows; i++) {
for (int j = 0; j < masc.cols; j++) {
if (masc.at<uchar>(i, j)>200) {
masc.at<uchar>(i, j) = (uchar)(255);
}
else {
masc.at<uchar>(i, j) = (uchar)(0);
}
}
}






for (int i = 0; i < chida.rows; i++) {
for (int j = 0; j < chida.cols; j++) {

if (masc.at<uchar>(i, j) == 255) {
oriDa.at<uchar>(i, j) = chida.at<uchar>(i, j);
}
}
}


//-----
namedWindow("binary", WINDOW_AUTOSIZE);
imshow("binary", oriDa);
waitKey(0);


*/