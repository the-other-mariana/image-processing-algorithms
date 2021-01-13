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

int num = 0;
queue<Point2d> q;
int limite = 100;
int color = 255;
float pi = 3.141592;
//Mat binary = imread("binary.jpg", CV_LOAD_IMAGE_GRAYSCALE);

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
	for (int i = limite; i < binary.rows; i++) {
		for (int j = 0; j < binary.cols; j++) {
			// i = rows, y  &  j = cols, x
			if (binary.at<uchar>(i, j) == 255) {
				m += (pow(j, p))*(pow(i, q))*(binary.at<uchar>(i, j)/255);
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
	/*
	binary.at<uchar>(r - 1, c) = 100;
	binary.at<uchar>(r, c + 1) = 100;
	binary.at<uchar>(r + 1, c) = 100;
	binary.at<uchar>(r, c - 1) = 100;

	binary.at<uchar>(r, c) = 100;
	*/
	for (int i = r - 1; i < r + 2; i++) {
		for (int j = c - 1; j < c + 2; j++) {
			binary.at<uchar>(i, j) = 0;
		}
	}
}

void circularity(vector<int> areas, vector<vector<Point>> contornos, vector<Point2d> x0_y0, Mat binary) {
	float r = 0;
	string figure = "";
	for (int i = 0; i < areas.size(); i++) {
		double area = contourArea(contornos.at(i));
		cout << "-----------------------" << endl;
		cout << "area" << i <<": "<< areas[i] << endl;
		r = (4 * pi * area) / (pow(arcLength(contornos.at(i), true), 2));
		cout << "circ: " << r << endl;
		if (r >= 0.85f) figure = "circle";
		else figure = "square";
		cv::putText(binary, figure, Point(x0_y0[i].x - 5, x0_y0[i].y -5),
			FONT_HERSHEY_COMPLEX_SMALL, 1.0, Scalar(0, 0, 0), 1);
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
int factor = 3;
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

int minArea = 400;
int umbralVal = 100;
void emptyCallback(int, void*) {
	if (minArea < 400) minArea = 400;
}

// B
int B(int r, int c, Mat binary) {
	int sum = 0;
	sum = (binary.at<uchar>(r - 1, c) / 255) + (binary.at<uchar>(r - 1, c + 1) / 255) + (binary.at<uchar>(r, c + 1) / 255) +
		(binary.at<uchar>(r + 1, c + 1) / 255) + (binary.at<uchar>(r + 1, c) / 255) + (binary.at<uchar>(r + 1, c - 1) / 255)
		+ (binary.at<uchar>(r, c - 1) / 255) +
		(binary.at<uchar>(r - 1, c - 1) / 255);
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

void computeAreas(vector<int> areas) {
	cout << "////////////////areas/////////////////" << endl;
	for (int i = 0; i < areas.size(); i++) {
		cout << areas[i] << endl;
	}
}

void computePerimeters(vector<vector<Point>> contornos, int num) {
	cout << "//////////////////perimeters///////////////" << endl;
	for (int i = 0; i < num; i++) {
		cout << contornos[i].size() << endl;
	}
}



int main() {
	Mat binary = imread("coins03.png", CV_LOAD_IMAGE_GRAYSCALE); 
	Mat binaryCopy = imread("coins03.png", CV_LOAD_IMAGE_GRAYSCALE);
	Mat binary2 = imread("coins03.png", CV_LOAD_IMAGE_GRAYSCALE);
	Mat auxMedian = imread("coins03.png", CV_LOAD_IMAGE_GRAYSCALE);
	Mat thinned = imread("coins03.png", CV_LOAD_IMAGE_GRAYSCALE);
	Mat v(binary.rows, binary.cols, CV_8UC1, Scalar(0));

	//trackbacks
	namedWindow("trackbars", WINDOW_AUTOSIZE);
	createTrackbar("umbral:", "trackbars", &umbralVal, 255, emptyCallback);

	// umbral, circulos blancos sobre fondo negro
	for (int i = 0; i < binary.rows; i++) {
		for (int j = 0; j < binary.cols; j++) {
			if (i == 0 || i == binary.rows - 1) {
				binary2.at<uchar>(i, j) = 0;
				thinned.at<uchar>(i, j) = 0;
			}
			if (j == 0 || j == binary.cols - 1) {
				binary2.at<uchar>(i, j) = 0;
				thinned.at<uchar>(i, j) = 0;
			}
			if (binary2.at<uchar>(i, j) > umbralVal) {
				binary2.at<uchar>(i, j) = 255;
				thinned.at<uchar>(i, j) = 255;
			}
			else {
				binary2.at<uchar>(i, j) = 0;
				thinned.at<uchar>(i, j) = 0;
			}
		}
	}

	vector<Point2d> x0_y0;
	
	vector<queue<Point2d>> regs;
	vector<int> areas;
	vector<int> perimeters;
	
	for (int i = limite; i < binary2.rows - 1; i++) {
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
				if (temp.size() > 400) {
					regs.push_back(temp);
					Point2d currPoint;
					currPoint.x = int(momento(1, 0, v) / momento(0, 0, v));
					currPoint.y = int(momento(0, 1, v) / momento(0, 0, v));
					x0_y0.push_back(currPoint);
					areas.push_back(momento(0, 0, v));
					paintVisited(regs[num], v);
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

	//computeAreas(areas);
	//computePerimeters(contornos, num);
	

	//pero los pinta en la original
	Scalar color(100, 100, 100);
	drawContours(binary, contornos, -1, color, 2, 8);
	
	for (int i = 0; i < num; i++) {
		paintMassCenter(x0_y0[i].y, x0_y0[i].x, binary);
	}
	cout << num << endl;
	/*
	for (int i = 0; i < limite; i++) {
		for (int j = 0; j < binary.cols; j++) {
			binary.at<uchar>(i, j) = median(i, j, binaryCopy);
		}
	}
	//////////////thinning
	while (true) {
		queue <Point2d> q;
		//////////////////////////////////////////////
		for (int i = 1; i < limite; i++) {
			for (int j = 1; j < binary.cols - 1; j++) {
				if (thinned.at<uchar>(i, j) == 255) {
					int b = B(i, j, thinned);
					int a = A(i, j, thinned);

					int p3_1 = (thinned.at<uchar>(i - 1, j) / 255) * (thinned.at<uchar>(i, j + 1) / 255) * (thinned.at<uchar>(i + 1, j) / 255);
					int p4_1 = (thinned.at<uchar>(i, j + 1) / 255) * (thinned.at<uchar>(i + 1, j) / 255) * (thinned.at<uchar>(i, j - 1) / 255);

					int p3_2 = (thinned.at<uchar>(i - 1, j) / 255) * (thinned.at<uchar>(i, j + 1) / 255) * (thinned.at<uchar>(i, j - 1) / 255);
					int p4_2 = (thinned.at<uchar>(i - 1, j) / 255) * (thinned.at<uchar>(i + 1, j) / 255) * (thinned.at<uchar>(i, j - 1) / 255);
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
		//namedWindow("thinned", WINDOW_AUTOSIZE);
		//imshow("thinned", thinned);
		//waitKey(10);
	}

	for (int i = 0; i < limite; i++) {
		for (int j = 0; j < binary.cols; j++) {
			if (thinned.at<uchar>(i, j) == 255) {
				binary.at<uchar>(i, j) = 0;
			}
		}
	}
	*/
	circularity(areas, contornos, x0_y0, binary);
	namedWindow("Original", WINDOW_NORMAL);
	imshow("Original", binary);

	namedWindow("binary2", WINDOW_NORMAL);
	imshow("binary2", binary2);
	//imshow("Original", auxMedian);


	waitKey(0);
}