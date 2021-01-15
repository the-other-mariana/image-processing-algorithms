//adyacencia(con 4 adyacencia u 8 adyacencia) conectividad(que haya camnio) regiones(pixeles conectados)
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
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace cv;
int num = 0;
queue<Point2d> q;
Mat binary = imread("binary2.png", CV_LOAD_IMAGE_GRAYSCALE);
Mat v(binary.rows, binary.cols, CV_8UC1, Scalar(0));
//Mat v(binary.rows, binary.cols, CV_8UC3, Scalar(0));
int color = 255;

void vecino4(int r, int c) {
	Point2d p;
	if (r > 0 && r < binary.rows - 1 && c > 0 && c < binary.cols - 1) {
		if (binary.at<uchar>(r - 1, c) == 255) {
			p.x = r - 1;
			p.y = c;
			q.push(p);
		}
		if (binary.at<uchar>(r, c + 1) == 255) {
			p.x = r;
			p.y = c + 1;
			q.push(p);
		}
		if (binary.at<uchar>(r + 1, c) == 255) {
			p.x = r + 1;
			p.y = c;
			q.push(p);
		}
		if (binary.at<uchar>(r, c - 1) == 255) {
			p.x = r;
			p.y = c - 1;
			q.push(p);
		}
	}
	

}

void vecino8(int r, int c) {
	Point2d p;
	if (r > 0 && r < binary.rows - 1 && c > 0 && c < binary.cols - 1) {
		if (binary.at<uchar>(r - 1, c) == 255) {
			p.x = r - 1;
			p.y = c;
			q.push(p);
		}
		if (binary.at<uchar>(r, c + 1) == 255) {
			p.x = r;
			p.y = c + 1;
			q.push(p);
		}
		if (binary.at<uchar>(r + 1, c) == 255) {
			p.x = r + 1;
			p.y = c;
			q.push(p);
		}
		if (binary.at<uchar>(r, c - 1) == 255) {
			p.x = r;
			p.y = c - 1;
			q.push(p);
		}

		if (binary.at<uchar>(r - 1, c + 1) == 255) {
			p.x = r - 1;
			p.y = c;
			q.push(p);
		}
		if (binary.at<uchar>(r + 1, c + 1) == 255) {
			p.x = r;
			p.y = c + 1;
			q.push(p);
		}
		if (binary.at<uchar>(r + 1, c - 1) == 255) {
			p.x = r + 1;
			p.y = c;
			q.push(p);
		}
		if (binary.at<uchar>(r - 1, c - 1) == 255) {
			p.x = r;
			p.y = c - 1;
			q.push(p);
		}
	}
}

int main() {
	srand(time(NULL));
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

	for (int i = 1; i < binary.rows - 1; i++) {
		for (int j = 1; j < binary.cols - 1; j++) {
			int s = int(binary.at<uchar>(i, j));
			if (s == 255 && (v.at<uchar>(i, j) == 0)) {
				v.at<uchar>(i, j) = color;
				vecino8(i, j);
				while (!q.empty()) {
					Point2d item;
					item = q.front();
					q.pop();
					if ((v.at<uchar>(item.x, item.y) == 0) && binary.at<uchar>(item.x, item.y) == 255) {
						v.at<uchar>(item.x, item.y) = color;
						//v.at<Vec3b>(item.x, item.y) = Vec3b(color, color, color);
						vecino8(item.x, item.y);
						//namedWindow("region", WINDOW_AUTOSIZE);
						//imshow("region", v);
						//waitKey(1);
					}

				}
				num++;
				//color += 50;
				
				int rn = rand() % 255 + 20;
				color = rn;

			}
		}
	}
	cout << "Regiones: " << num << endl;
	namedWindow("image", WINDOW_AUTOSIZE);
	imshow("image", binary);

	namedWindow("region", WINDOW_AUTOSIZE);
	imshow("region", v);
	waitKey(0);
}