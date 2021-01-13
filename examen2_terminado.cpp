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

//int num = 0;
//queue<Point2d> q;
queue<Point2d> q;
int umbralValue = 100;
int minValue;
int minArea = 400;
int maxArea = 50000;
int largoHeader = 200;
int anchoHeader = 200;
int factor = 3;
float pi = 3.141592;
int infoColor = 100;

void vecino4(int r, int c, Mat binary) {
	Point2d p;

	if (binary.at<uchar>(r - 1, c - 1) == 255) {
		p.x = r - 1;
		p.y = c - 1;
		q.push(p);
	}
	if (binary.at<uchar>(r - 1, c) == 255) {
		p.x = r - 1;
		p.y = c;
		q.push(p);
	}
	if (binary.at<uchar>(r - 1, c + 1) == 255) {
		p.x = r - 1;
		p.y = c + 1;
		q.push(p);
	}
	if (binary.at<uchar>(r, c + 1) == 255) {
		p.x = r;
		p.y = c + 1;
		q.push(p);
	}
	if (binary.at<uchar>(r + 1, c + 1) == 255) {
		p.x = r + 1;
		p.y = c + 1;
		q.push(p);
	}
	if (binary.at<uchar>(r + 1, c) == 255) {
		p.x = r + 1;
		p.y = c;
		q.push(p);
	}
	if (binary.at<uchar>(r + 1, c - 1) == 255) {
		p.x = r + 1;
		p.y = c - 1;
		q.push(p);
	}
	if (binary.at<uchar>(r, c - 1) == 255) {
		p.x = r;
		p.y = c - 1;
		q.push(p);
	}

}

int momento(int p, int q, Mat binary) {
	int m = 0;
	for (int i = 0; i < binary.rows; i++) {
		for (int j = 0; j < binary.cols; j++) {
			// i = rows, y  &  j = cols, x
			if (binary.at<uchar>(i, j) == 255) {
				m += (pow(j, p))*(pow(i, q))*(binary.at<uchar>(i, j) / 255);
			}
		}
	}
	return m;
}

void paintVisited(queue<Point2d> region, Mat v) {

	while (!region.empty()) {
		Point2d coord;
		coord = region.front();
		region.pop();
		v.at<uchar>(coord.x, coord.y) = 100;
	}

}

void paintMassCenter(int r, int c, Mat binary) {
	int value = infoColor;
	/*
	binary.at<uchar>(r - 1, c) = value;
	binary.at<uchar>(r, c + 1) = value;
	binary.at<uchar>(r + 1, c) = value;
	binary.at<uchar>(r, c - 1) = value;

	binary.at<uchar>(r, c) = value;*/
	for (int i = r - 1; i < r + 2; i++) {
		for (int j = c - 1; j < c + 2; j++) {
			binary.at<uchar>(i, j) = value;
		}
	}
}

void eraseRegion(queue<Point2d> temp, Mat v, Mat binary) {
	while (!temp.empty()) {
		Point2d item = temp.front();
		temp.pop();
		v.at<uchar>(item.x, item.y) = 0;
		binary.at<uchar>(item.x, item.y) = 0;
	}
}

void voidFunction(int, void*) {
	if (minValue < 400) minValue= 400;
}

int median(int r, int c, Mat penguin) {
	int value = 0;
	vector<int> v;
	int res = 0;
	int index = 0;
	for (int i = r - int(factor / 2); i < (r + int(factor / 2) + 1); i++) {
		for (int j = c - int(factor / 2); j < (c + int(factor / 2) + 1); j++) {
			if (i < 0 or i >= penguin.rows or j < 0 or j >= penguin.cols) {
				continue;
			}
			else {
				v.push_back(penguin.at<uchar>(i, j));
			}
		}
	}
	sort(v.begin(), v.end());
	index = (int)(v.size() / 2);
	res = v[index];
	return res;
}

void sendMedian(Mat binary, Mat thinned, Mat original) {
	for (int i = 0; i < largoHeader; i++) {
		for (int j = 0; j < binary.cols; j++) {
			//binary.at<uchar>(i, j) = 255;
			binary.at<uchar>(i, j) = median(i, j, original);
			thinned.at<uchar>(i, j) = median(i, j, original);
			if (i == largoHeader - 1) {
				binary.at<uchar>(i, j) = 255;
			}
		}
	}

}

void defineFigure(Mat binary, int regions, vector<vector<Point>> contornos, vector<Point2d> x0_y0) {
	for (int i = 0; i < regions; i++) {
		string shape = "";
		double area = contourArea(contornos.at(i));
		float r;
		r = (4 * pi*area) / (arcLength(contornos.at(i), true)*arcLength(contornos.at(i), true));
		if (r >= 0.85) shape = "circle";
		else shape = "square";
		cv::putText(binary, shape, Point(x0_y0[i].x, x0_y0[i].y), 2, 0.50f, Scalar(infoColor, infoColor, infoColor), 1);
	}
}

void setCornersBlack(Mat binary2) {
	for (int i = 0; i < binary2.rows; i++) {
		for (int j = 0; j < binary2.cols; j++) {
			if (i == 0 || i == binary2.rows - 1) {
				binary2.at<uchar>(i, j) = 0;
			}
			if (j == 0 || j == binary2.cols - 1) {
				binary2.at<uchar>(i, j) = 0;
			}
		}
	}
}

int B(int r, int c, Mat binary) {
	int sum = 0;
	sum = (binary.at<uchar>(r - 1, c) / 255) + (binary.at<uchar>(r - 1, c + 1) / 255) + (binary.at<uchar>(r, c + 1) / 255) +
		(binary.at<uchar>(r + 1, c + 1) / 255) + (binary.at<uchar>(r + 1, c) / 255) + (binary.at<uchar>(r + 1, c - 1) / 255)
		+ (binary.at<uchar>(r, c - 1) / 255) +
		(binary.at<uchar>(r - 1, c - 1) / 255);
	return sum;
}

int A(int r, int c, Mat binary) {
	int times = 0;
	string secuencia = "";
	(binary.at<uchar>(r - 1, c) == 0) ? secuencia += "0" : secuencia += "1";
	(binary.at<uchar>(r - 1, c) == 0) ? secuencia += "0" : secuencia += "1";
	(binary.at<uchar>(r - 1, c + 1) == 0) ? secuencia += "0" : secuencia += "1";
	(binary.at<uchar>(r, c + 1) == 0) ? secuencia += "0" : secuencia += "1";
	(binary.at<uchar>(r + 1, c + 1) == 0) ? secuencia += "0" : secuencia += "1";
	(binary.at<uchar>(r + 1, c) == 0) ? secuencia += "0" : secuencia += "1";
	(binary.at<uchar>(r + 1, c - 1) == 0) ? secuencia += "0" : secuencia += "1";
	(binary.at<uchar>(r, c - 1) == 0) ? secuencia += "0" : secuencia += "1";
	(binary.at<uchar>(r - 1, c - 1) == 0) ? secuencia += "0" : secuencia += "1";
	for (int i = 0; i < 9; i++) {
		if (i == 8) {
			continue;
		}
		if (secuencia[i] == '0' && secuencia[i + 1] == '1') {
			times++;
		}
	}
	return times;
}

void thinningAlgorithm(Mat thinned) {
	while (true) {
		queue <Point2d> qt;
		for (int i = 1; i < largoHeader; i++) {
			for (int j = 1; j < thinned.cols - 1; j++) {
				if (thinned.at<uchar>(i, j) == 255) {
					int b = B(i, j, thinned);
					int a = A(i, j, thinned);
					int p3_1 = (thinned.at<uchar>(i - 1, j) / 255) * (thinned.at<uchar>(i, j + 1) / 255) * (thinned.at<uchar>(i + 1, j) / 255);
					int p4_1 = (thinned.at<uchar>(i, j + 1) / 255) * (thinned.at<uchar>(i + 1, j) / 255) * (thinned.at<uchar>(i, j - 1) / 255);
					int p3_2 = (thinned.at<uchar>(i - 1, j) / 255) * (thinned.at<uchar>(i, j + 1) / 255) * (thinned.at<uchar>(i, j - 1) / 255);
					int p4_2 = (thinned.at<uchar>(i - 1, j) / 255) * (thinned.at<uchar>(i + 1, j) / 255) * (thinned.at<uchar>(i, j - 1) / 255);
					if (((b >= 2 && b <= 6) && (a == 1) && (p3_1 == 0) && (p4_1 == 0)) ||
						((b >= 2 && b <= 6) && (a == 1) && (p3_2 == 0) && (p4_2 == 0))) {
						Point2d coord;
						coord.x = i;
						coord.y = j;
						qt.push(coord);
					}
				}
			}
		}
		if (qt.empty()) {
			break;
		}
		while (!qt.empty()) {
			Point2d item;
			item = qt.front();
			qt.pop();
			thinned.at<uchar>(item.x, item.y) = 0;
		}
	}
}

int main() {
	
	Mat binary;
	Mat binary2;
	Mat binaryCopy;
	Mat thinned;

	namedWindow("valores", WINDOW_NORMAL);
	createTrackbar("Largo del header:", "valores", &largoHeader, 300, voidFunction);
	createTrackbar("umbral:", "valores", &umbralValue, 255, voidFunction);
	createTrackbar("minima area detectada:", "valores", &minArea, 1000, voidFunction);
	createTrackbar("maxima area detectada:", "valores", &maxArea, 50000, voidFunction);
	createTrackbar("color:", "valores", &infoColor, 255, voidFunction);
	
	VideoCapture cap(0);
	if (!cap.isOpened())
		return -1;
	bool showEc = false;
	bool showThin = false;
	for (;;) {

		if (GetKeyState('S') & 0x8000) {
			showEc = !showEc;
		}

		Mat frame;

		cap >> frame;
		cvtColor(frame, binary, COLOR_BGR2GRAY);
		cvtColor(frame, binary2, COLOR_BGR2GRAY);
		cvtColor(frame, binaryCopy, COLOR_BGR2GRAY);
		cvtColor(frame, thinned, COLOR_BGR2GRAY);
		Mat v(binary.rows, binary.cols, CV_8UC1, Scalar(0));

		threshold(binary2, binary2, umbralValue, 255, CV_THRESH_BINARY);
		//threshold(thinned, thinned, umbralValue, 255, CV_THRESH_BINARY);
		setCornersBlack(thinned);
		setCornersBlack(binary2);
		
		if (showEc) {
			int num = 0;
			vector<Point2d> x0_y0;
			vector<queue<Point2d>> regs;
			vector<int> areas;
			vector<int> perimeters;

			for (int i = largoHeader; i < binary2.rows - 1; i++) {
				for (int j = 1; j < binary2.cols - 1; j++) {
					int s = int(binary2.at<uchar>(i, j));
					if (s == 255 && (v.at<uchar>(i, j) == 0)) {
						queue<Point2d> temp;
						Point2d first;
						first.x = i;
						first.y = j;
						temp.push(first);
						v.at<uchar>(i, j) = 255;
						vecino4(i, j, binary2);
						while (!q.empty()) {
							Point2d item;
							item = q.front();
							q.pop();
							if ((v.at<uchar>(item.x, item.y) == 0) && binary2.at<uchar>(item.x, item.y) == 255) {
								v.at<uchar>(item.x, item.y) = 255;
								temp.push(item);
								vecino4(item.x, item.y, binary2);
							}
						}
						if (temp.size() > minArea && temp.size() < maxArea) {
							regs.push_back(temp);
							Point2d currPoint;
							currPoint.x = int(momento(1, 0, v) / momento(0, 0, v));
							currPoint.y = int(momento(0, 1, v) / momento(0, 0, v));
							x0_y0.push_back(currPoint);
							paintVisited(regs[num], v);
							areas.push_back(momento(0, 0, v));
							num++;
						}
						else {
							//pintar en v de negro la cola temp
							eraseRegion(temp, v, binary2);
						}
					}
				}
			}

			//encuentra contornos de la imagen analizada
			vector<vector<Point>> contornos;
			findContours(binary2, contornos, 0, CV_CHAIN_APPROX_SIMPLE);

			//pero los pinta en la original
			Scalar color(infoColor, infoColor, infoColor);
			drawContours(binary, contornos, -1, color, 2, 8);

			for (int i = 0; i < num; i++) {
				paintMassCenter(x0_y0[i].y, x0_y0[i].x, binary);
			}

			sendMedian(binary, thinned, binaryCopy);

			// thinning begin
			threshold(thinned, thinned, umbralValue, 255, CV_THRESH_BINARY);
			thinningAlgorithm(thinned);
			for (int i = 0; i < largoHeader; i++) {
				for (int j = 0; j < binary.cols; j++) {
					if (thinned.at<uchar>(i, j) == 255) {
						binary.at<uchar>(i, j) = 0;
					}
				}
			}
			//thinning end
			
			//computeAreas(areas);
			//computePerimeter(contornos);
			defineFigure(binary, num, contornos, x0_y0);

			imshow("binary 2", binary2);
			imshow("binary", binary);

		}
		else {
			sendMedian(binary, thinned, binaryCopy);

			imshow("binary 2", binary2);
			imshow("binary", binary);
		}

		if (GetKeyState(VK_RETURN) & 0x8000) break;
		if (waitKey(100) >= 0) continue;
	}
	return 0;
}