//headers, librerias
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include<math.h>
using namespace std;
using namespace cv;

int main()
{
	//Imagenes originales
	const char* filenameD = "monalisa.jpg";
	Mat image = imread(filenameD, CV_LOAD_IMAGE_GRAYSCALE);
	const char* filenameA = "peppernoise.png";
	Mat pepperNoise = imread(filenameA, CV_LOAD_IMAGE_GRAYSCALE);
	//Mats resutado
	Mat blur(image.rows, image.cols, CV_8UC1);
	Mat diferencia(image.rows, image.cols, CV_8UC1);
	Mat filtroMediana(pepperNoise.rows, pepperNoise.cols, CV_8UC1);
	Mat gaussiano(image.rows, image.cols, CV_8UC1);
	Mat gaussiano2(image.rows, image.cols, CV_8UC1);
	Mat gaussiano3(image.rows, image.cols, CV_8UC1);
	int innerMatrixIndex = 3;
	int tamCampana = 5;
	int gaussMatrix[3][3] = { { 1,2,1 },{ 2,4,2 },{ 1,2,1 } };
	int gaussMatrix2[5][5] = { { 1,4,7,4,1 },{ 4,16,26,16,4 },{ 7,26,41,26,7 },{ 4,16,26,16,4 },{ 1,4,7,4,1 } };
	float suma, promedio;
	float gauss[25][25] = { 0 };

	//average blur
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			suma = 0;
			promedio = 0;
			for (int a = -(innerMatrixIndex / 2); a <= innerMatrixIndex / 2; a++)
			{
				for (int b = -(innerMatrixIndex / 2); b <= innerMatrixIndex / 2; b++)
				{
					////// suma
					if ((i + a) >= 0 &&
						(i + a) < image.rows &&
						(j + b) >= 0 &&
						(j + b) < image.cols)
					{
						suma += image.at<uchar>(i + a, j + b);
					}
				}
			}
			/////promedio
			promedio = float(suma / float(innerMatrixIndex*innerMatrixIndex));
			///// asignacion
			blur.at<uchar>(i, j) = promedio;
		}
	}
	//Resta de imagenes
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			diferencia.at<uchar>(i, j) = abs(image.at<uchar>(i, j) - blur.at<uchar>(i, j));
		}
	}
	int limite = 40;
	//umbral
	for (int i = 0; i < diferencia.rows; i++)
	{
		for (int j = 0; j < diferencia.cols; j++)
		{
			if (diferencia.at<uchar>(i, j) <= limite)
			{
				diferencia.at<uchar>(i, j) = 0;
			}
			else {
				diferencia.at<uchar>(i, j) = 255;
			}
		}
		cout << endl;
	}
	//Filtro de mediana
	float mediana = 0;
	vector<float> myVector;
	for (int i = 0; i < pepperNoise.rows; i++)
	{
		for (int j = 0; j < pepperNoise.cols; j++)
		{
			mediana = 0;
			myVector.clear();
			for (int a = -(innerMatrixIndex / 2); a <= innerMatrixIndex / 2; a++)
			{
				for (int b = -(innerMatrixIndex / 2); b <= innerMatrixIndex / 2; b++)
				{
					//agregarlos al vector
					if ((i + a) >= 0 &&
						(i + a) < pepperNoise.rows &&
						(j + b) >= 0 &&
						(j + b) < pepperNoise.cols)
					{
						myVector.push_back(pepperNoise.at<uchar>(i + a, j + b));
					}
				}
			}
			sort(myVector.begin(), myVector.end());
			mediana = myVector.at(myVector.size() / 2);
			///// asignacion
			filtroMediana.at<uchar>(i, j) = mediana;
		}
	}
	//gaussian blur
	int suma2 = 0;
	int promedio2 = 0;
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			suma2 = 0;
			promedio2 = 0;

			int y = 0;
			for (int a = -(innerMatrixIndex / 2); a <= innerMatrixIndex / 2; a++)
			{
				int x = 0;
				for (int b = -(innerMatrixIndex / 2); b <= innerMatrixIndex / 2; b++)
				{
					////// suma
					if ((i + a) >= 0 &&
						(i + a) < image.rows &&
						(j + b) >= 0 &&
						(j + b) < image.cols)
					{
						suma2 += int(image.at<uchar>(i + a, j + b)*gaussMatrix[y][x]);
					}
					x++;
				}
				y++;
			}

			/////promedio
			promedio2 = int(suma2 / 16);
			///// asignacion
			gaussiano.at<uchar>(i, j) = promedio2;
		}
	}
	//gaussian blur 5x5
	int suma3 = 0;
	int promedio3 = 0;
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			suma3 = 0;
			promedio3 = 0;
			int y1 = 0;
			for (int a = -(tamCampana / 2); a <= tamCampana / 2; a++)
			{
				int x1 = 0;
				for (int b = -(tamCampana / 2); b <= tamCampana / 2; b++)
				{
					////// suma
					if ((i + a) >= 0 &&
						(i + a) < image.rows &&
						(j + b) >= 0 &&
						(j + b) < image.cols)
					{
						suma3 += int(image.at<uchar>(i + a, j + b)*gaussMatrix2[y1][x1]);
					}
					x1++;
				}
				y1++;
			}
			/////promedio
			promedio3 = int(suma3 / 273);
			///// asignacion
			gaussiano2.at<uchar>(i, j) = promedio3;
		}
	}
	int x0 = 25 / 2;
	int y0 = 25 / 2;
	int sigma = 3;
	float pi = 3.1416;
	float sumaFiltro = 0;
	for (int i = 0; i < 25; i++) {
		for (int j = 0; j < 25; j++) {
			int cX = i - x0;
			int cY = y0 - j;
			float up = (cX*cX) + (cY*cY);
			float down = 2 * (sigma * sigma);
			float exp1 = exp(-(up) / (down));
			float constant = 1.0 / (sigma*sigma * 2 * pi);
			gauss[i][j] = constant* exp1;
			sumaFiltro += constant * exp1;
		}
	}
	
	float suma4 = 0;
	int promedio4 = 0;
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			suma4 = 0;
			promedio4 = 0;
			int y4 = 0;
			for (int a = -(25 / 2); a <= 25 / 2; a++)
			{
				int x4 = 0;
				for (int b = -(25 / 2); b <= 25 / 2; b++)
				{
					////// suma
					if ((i + a) >= 0 &&
						(i + a) < image.rows &&
						(j + b) >= 0 &&
						(j + b) < image.cols)
					{
						suma4 += int(image.at<uchar>(i + a, j + b))*gauss[y4][x4];
					}
					x4++;
				}
				y4++;
			}
			/////promedio
			promedio4 = int(suma4 / sumaFiltro);
			///// asignacion
			gaussiano3.at<uchar>(i, j) = promedio4;
		}
	}
	namedWindow("Display window", WINDOW_AUTOSIZE);
	imshow("Display window", image);
	namedWindow("Display window1", WINDOW_AUTOSIZE);
	imshow("Display window1", blur);
	namedWindow("Display window2", WINDOW_AUTOSIZE);
	imshow("Display window2", diferencia);
	namedWindow("Display window3", WINDOW_AUTOSIZE);
	imshow("Display window3", filtroMediana);
	namedWindow("Display window4", WINDOW_AUTOSIZE);
	imshow("Display window4", pepperNoise);

	namedWindow("gaussiano", WINDOW_AUTOSIZE);
	imshow("gaussiano", gaussiano);
	namedWindow("gaussiano2", WINDOW_AUTOSIZE);
	imshow("gaussiano2", gaussiano2);
	namedWindow("gaussiano3", WINDOW_AUTOSIZE);
	imshow("gaussiano3", gaussiano3);
	waitKey(0);
}
