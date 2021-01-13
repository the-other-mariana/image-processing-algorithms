#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <cmath>

using namespace std;
using namespace cv;

int main() {





	//-----------------------------------------------------------------Ejercicio 1

	Mat myMat1(512, 512, CV_32F, Scalar(0));

	

	//-----------------------------------------------------------------Ej 2


	Mat myMat2(512, 512, CV_32F, Scalar(0));


	for (int i = 0; i < myMat2.rows; i++) {

		for (int j = 0; j < myMat2.cols; j++) {


			if (i < (myMat2.rows / 2) && j < (myMat2.cols / 2) || (i > (myMat2.rows / 2) && j > (myMat2.cols / 2))) {
				myMat2.at<float>(i, j) = (float)(0);
			}
			else {
				myMat2.at<float>(i, j) = (float)(255);
			}
		}
	}
	


	//-----------------------------------------------------------------ej 3


	Mat myMat3(512, 512, CV_32F, Scalar(0));
	bool hola = true;

	for (int a = 0; a < myMat3.rows; a++) {
		for (int j = 0; j < myMat3.cols; j++) {
			if (j % 10 == 0) {
				hola = !hola;
			}
			if (hola) {
				myMat3.at<float>(a, j) = (float)(255);
			}
			else {
				myMat3.at<float>(a, j) = (float)(0);
			}

		}


	}
	

	//-----------------------------------------------------------------ej 4

	Mat myMat4(512, 512, CV_32F, Scalar(0));
	hola = true;

	for (int a = 0; a < myMat4.rows; a++) {
		if (a % 10 == 0) {
			hola = !hola;
		}
		for (int j = 0; j < myMat4.cols; j++) {

			if (hola) {
				myMat4.at<float>(a, j) = (float)(255);
			}
			else {
				myMat4.at<float>(a, j) = (float)(0);
			}

		}


	}
	

	//-----------------------------------------------------------------ej 5

	Mat myMat5(512, 512, CV_32F, Scalar(0));
	hola = true;

	for (int a = 0; a < myMat5.rows; a++) {
		if (a % 10 == 0) {
			hola = !hola;
		}
		for (int j = 0; j < myMat5.cols; j++) {
			if (j % 10 == 0) {
				hola = !hola;
			}
			if (hola) {
				myMat5.at<float>(a, j) = (float)(255);
			}
			else {
				myMat5.at<float>(a, j) = (float)(0);
			}

		}


	}
	


	//-----------------------------------------------------------------ej 6

	Mat myMat6(512, 512, CV_32F, Scalar(255));

	for (int i = 100; i < myMat6.rows - 100; i++) {

		for (int j = 100; j < myMat6.cols - 100; j++) {

			myMat6.at<float>(i, j) = (float)(0);


		}
	}

	for (int i = 200; i < myMat6.rows - 200; i++) {

		for (int j = 200; j < myMat6.cols - 200; j++) {

			myMat6.at<float>(i, j) = (float)(255);

		}
	}
	


	//-----------------------------------------------------------------ej 7
	Mat myMat7(512, 512, CV_32FC1, Scalar(255));
	int a = 0;



	float dx = 255.0f / 512.0f;
	cout << dx << endl;

	for (int i = 0; i < myMat7.rows; i++) {

		for (int j = 0; j < myMat7.cols; j++) {
			float r;
			r = j * dx;
			myMat7.at<float>(i, j) = float(r / 255.0f);
			a++;
		}
	}
	



	//-----------------------------------------------------------------ej 8
	Mat myMat8(512, 512, CV_32FC1, Scalar(255));
	a = 0;



	dx = 255.0f / 512.0f;
	float dy = 255.0f / 512.0f;

	for (int i = 0; i < myMat8.rows; i++) {
		for (int j = 0; j < myMat8.cols; j++) {
			float r;
			//r = j * dx;
			float inc1 = j * dx;
			float inc2 = i * dy;
			r = sqrt((inc1*inc1) + (inc2*inc2));
			myMat8.at<float>(i, j) = float(r / 255.0f);
			a++;
		}
	}

	Mat myMat9(512, 512, CV_32FC1, Scalar(255));

	dy = (float)(255) / (float)(myMat9.rows);

	for (int i = myMat9.rows-1; i >= 0; i--) {
		for (int j = 0; j < myMat9.cols; j++) {
			myMat9.at<float>(i, j) = (float)(i*dy) / 255.0f;
			//float los recibe [0,1] entonces /255 
			//para poner el opuesto (voltearlos) 1 - num
			//al ser fraccion /255 siempre < 1
			//si era 0.3, el opuesto es 0.7
			//myMat9.at<float>(i, j) = 1 - ((float)(i*dy) / 255.0f); 
		}
	}

	Mat myMat10(512, 512, CV_8UC1, Scalar(255));
	dx = float(255.0 / myMat10.rows);

	for (int i = 0; i < myMat10.rows; i++) {
		for (int j = 0; j < myMat10.cols; j++) {
			myMat10.at<uchar>(i, j) = uchar(j*dx); //degradado horizontal, uchar no necesita normalizacion
		}
	}

	//255 es blanco
	
	namedWindow("Display window", WINDOW_AUTOSIZE);
	imshow("Display window", myMat1);
	
	namedWindow("Display window2", WINDOW_AUTOSIZE);
	imshow("Display window2", myMat2);
	
	namedWindow("Display window3", WINDOW_AUTOSIZE);
	imshow("Display window3", myMat3);
	
	namedWindow("Display window4", WINDOW_AUTOSIZE);
	imshow("Display window4", myMat4);
	
	namedWindow("Display window5", WINDOW_AUTOSIZE);
	imshow("Display window5", myMat5);
	
	namedWindow("Display window6", WINDOW_AUTOSIZE);
	imshow("Display window6", myMat6);
	
	namedWindow("Display window10", WINDOW_AUTOSIZE);
	imshow("Display window10", myMat10);

	namedWindow("Display window9", WINDOW_AUTOSIZE);
	imshow("Display window9", myMat9);

	namedWindow("Display window7", WINDOW_AUTOSIZE);
	imshow("Display window7", myMat7);

	namedWindow("Display window8", WINDOW_AUTOSIZE);
	imshow("Display window8", myMat8);
	waitKey(0);




}
