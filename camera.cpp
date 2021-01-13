#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <cmath>
#include "opencv2/video.hpp"

using namespace std;
using namespace cv;


int main(){

	Mat oriDa = imread("dados.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat masc = imread("dados.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat chida = imread("cielo.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat image2;
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



	VideoCapture cap(0);// abre la cámara de default
	if (!cap.isOpened())//revisa si se abrió correctamente
		return -1;

	for(;;){
		Mat frame;

		cap >> frame;//obtiene un nuevo cuadro de la cámara
		cvtColor(frame, image2, COLOR_BGR2GRAY);

		for (int i = 0; i < frame.rows; i++) {
			for (int j = 0; j < frame.cols; j++) {

				if (masc.at<uchar>(i, j) == 255) {
					//****
					oriDa.at<uchar>(i, j) = image2.at<uchar>(i, j);
					//oriDa.at<uchar>(i, j) = chida.at<uchar>(i, j);
				}
			}
		}


		imshow("frame", oriDa);
		if (waitKey(1) >= 0)
			break;
	}


	return 0;
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

