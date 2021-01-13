#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <cmath>
#include "opencv2/video.hpp"
#include<windows.h>
#include<algorithm>
#include<vector>
#include<queue>

using namespace std;
using namespace cv;




int innerMtx = 3;

// B
int B(int r, int c, Mat binary) {
	int sum = 0;
	sum = (binary.at<uchar>(r - 1, c)/255) + (binary.at<uchar>(r - 1, c + 1)/255) + (binary.at<uchar>(r, c + 1)/255) +
		(binary.at<uchar>(r + 1, c + 1)/255) + (binary.at<uchar>(r + 1, c)/255) + (binary.at<uchar>(r + 1, c - 1)/255) 
		+ (binary.at<uchar>(r, c - 1)/255) +
		(binary.at<uchar>(r - 1, c - 1)/255);
	return sum;
}
// A
int A(int r, int c, Mat binary) {
	//(condition) ? (if_true) : (if_false)
	int rep = 0;
	string str = "";

	(binary.at<uchar>(r - 1, c) == 0) ? str += "0" : str += "1";
	(binary.at<uchar>(r - 1, c + 1) == 0) ? str += "0" : str += "1";
	(binary.at<uchar>(r, c + 1) == 0) ? str += "0" : str += "1";
	(binary.at<uchar>(r + 1, c + 1) == 0) ? str += "0" : str += "1";

	(binary.at<uchar>(r + 1, c) == 0) ? str += "0" : str += "1";
	(binary.at<uchar>(r + 1, c - 1) == 0) ? str += "0" : str += "1";
	(binary.at<uchar>(r, c - 1) == 0) ? str += "0" : str += "1";
	(binary.at<uchar>(r - 1, c - 1) == 0) ? str += "0" : str += "1";

	(binary.at<uchar>(r - 1, c) == 0) ? str += "0" : str += "1";

	for (int i = 0; i < 9; i++) {
		if (i == 8) {
			continue;
		}
		if (str[i] == '0' && str[i + 1] == '1') {
			rep++;
		}
	}
	return rep;
}

int main() {
	Mat binary = imread("gate5.png", CV_LOAD_IMAGE_GRAYSCALE);
	Mat thinned = imread("gate5.png", CV_LOAD_IMAGE_GRAYSCALE);

	//umbral, donde negro = 0 y blanco = 1

	for (int i = 0; i < binary.rows; i++) {
		for (int j = 0; j < binary.cols; j++) {
			if (binary.at<uchar>(i, j) > 127) {
				binary.at<uchar>(i, j) = 0;
				thinned.at<uchar>(i, j) = 0;
			}
			else {
				binary.at<uchar>(i, j) = 255;
				thinned.at<uchar>(i, j) = 255;
			}
		}
	}


	while (true) {
		queue <Point2d> q;
		//////////////////////////////////////////////
		for (int i = 1; i < binary.rows - 1; i++) {
			for (int j = 1; j < binary.cols - 1; j++) {
				if (thinned.at<uchar>(i, j) == 255) {
					int b = B(i, j, thinned);
					int a = A(i, j, thinned);

					int p3_1 = (thinned.at<uchar>(i - 1, j)/255) * (thinned.at<uchar>(i, j + 1)/255) * (thinned.at<uchar>(i + 1, j)/255);
					int p4_1 = (thinned.at<uchar>(i, j + 1)/255) * (thinned.at<uchar>(i + 1, j)/255) * (thinned.at<uchar>(i, j - 1)/255);

					int p3_2 = (thinned.at<uchar>(i - 1, j)/255) * (thinned.at<uchar>(i, j + 1)/255) * (thinned.at<uchar>(i, j - 1)/255);
					int p4_2 = (thinned.at<uchar>(i - 1, j)/255) * (thinned.at<uchar>(i + 1, j)/255) * (thinned.at<uchar>(i, j - 1)/255);
					// primera o segunda iteracion cumplida se guarda para borrarse
					if (((b >= 2 && b <= 6) && (a == 1) && (p3_1 == 0) && (p4_1 == 0)) ||
						((b >= 2 && b <= 6) && (a == 1) && (p3_2 == 0) && (p4_2 == 0))) {
						Point2d coord;
						coord.x = i;
						coord.y = j;
						q.push(coord);
					}
				}
			}
		}
		///////////////////////////////////////////
		if (q.empty()) {
			break;
		}
		while (!q.empty()) {
			Point2d item;
			item = q.front();
			q.pop();
			thinned.at<uchar>(item.x, item.y) = 0;
		}
		//////////////////////////////////////////
		namedWindow("thinned", WINDOW_AUTOSIZE);
		imshow("thinned", thinned);
		waitKey(10);
	}

	namedWindow("Original", WINDOW_AUTOSIZE);
	imshow("Original", binary);

	waitKey(0);
}