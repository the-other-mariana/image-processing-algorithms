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
	const char* filenameA = "penguin.png";
	Mat pepperNoise = imread(filenameA, CV_LOAD_IMAGE_GRAYSCALE);
	//Mats resutado
	Mat blur(image.rows, image.cols, CV_8UC1);
	Mat diferencia(image.rows, image.cols, CV_8UC1);
	Mat filtroMediana(pepperNoise.rows, pepperNoise.cols, CV_8UC1);
	Mat sobelH(image.rows, image.cols, CV_32FC1);
	Mat sobelV(image.rows, image.cols, CV_32FC1);
	Mat raiz(image.rows, image.cols, CV_32FC1);
	Mat gaussiano2(image.rows, image.cols, CV_8UC1);
	Mat gaussiano3(image.rows, image.cols, CV_8UC1);
	int innerMatrixIndex = 3;
	int tamCampana = 5;
	int horiz[3][3] = { { -1,0,1 },{ -2,0,2 },{ -1,0,1 } };
	int vert[3][3] = { { -1,-2,-1 },{ 0,0,0 },{ 1,2,1 } };
	
	

	

	//sobel horizontal
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
						suma2 += (image.at<uchar>(i + a, j + b)*horiz[y][x]);
					}
					x++;
				}
				y++;
			}
			sobelH.at<float>(i, j) = abs(suma2);
		}
	}

	//sobel vertical
	int suma3 = 0;
	int promedio3 = 0;
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			suma3 = 0;
			promedio3 = 0;

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
						suma3 += (image.at<uchar>(i + a, j + b)*vert[y][x]);
					}
					x++;
				}
				y++;
			}
			sobelV.at<float>(i, j) =abs (suma3);
		}
	}

	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			float xValue = (sobelH.at<float>(i, j)*sobelH.at<float>(i, j));
			float yValue = (sobelV.at<float>(i, j)*sobelV.at<float>(i, j));
			raiz.at<float>(i, j) = (sqrt(xValue + yValue));
		}
	}
	//normalizar la foto
	double minH = 0;
	double maxH = 0;
	double minR = 0;
	double maxR = 0;
	double minV = 0;
	double maxV = 0;
	
	Mat sobelHR(image.rows, image.cols, CV_8UC1, Scalar(0));
	Mat sobelVR(image.rows, image.cols, CV_8UC1, Scalar(0));
	Mat raizR(image.rows, image.cols, CV_8UC1, Scalar(0));
	minMaxLoc(sobelH, &minH, &maxH);
	minMaxLoc(sobelV, &minV, &maxV);
	minMaxLoc(raiz, &minR, &maxR);

	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			float valH = 0;
			float valV = 0;
			float valR = 0;
			valH = (255 * (sobelH.at<float>(i, j) - minH)) / (maxH - minH);
			valV = (255 * (sobelV.at<float>(i, j) - minV)) / (maxV - minV);
			valR = (255 * (raiz.at<float>(i, j) - minR)) / (maxR- minR);
			sobelHR.at<uchar>(i, j) = abs(valH);
			sobelVR.at<uchar>(i, j) = abs(valV);
			raizR.at<uchar>(i, j) = abs(valR);
		}
	}

	

	//---------------------------------------------- PREWITT ---------------------------------------
	Mat prewittV(image.rows, image.cols, CV_32FC1, Scalar(0));
	Mat prewittH(image.rows, image.cols, CV_32FC1, Scalar(0));
	Mat raizP(image.rows, image.cols, CV_32FC1, Scalar(0));
	int horizP[3][3] = { { -1,0,1 },{ -1,0,1 },{ -1,0,1 } };
	int vertP[3][3] = { { -1,-1,-1 },{ 0,0,0 },{ 1,1,1 } };

	suma2 = 0;
	promedio2 = 0;
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
						suma2 += (image.at<uchar>(i + a, j + b)*horizP[y][x]);
					}
					x++;
				}
				y++;
			}
			prewittH.at<float>(i, j) = abs(suma2);
		}
	}
	suma3 = 0;
	promedio3 = 0;
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			suma3 = 0;
			promedio3 = 0;

			int y = 0;
			for (int a = -(innerMatrixIndex / 2); a <= innerMatrixIndex / 2; a++)
			{
				int x = 0;
				for (int b = -(innerMatrixIndex / 2); b <= innerMatrixIndex / 2; b++)
				{
					if ((i + a) >= 0 &&
						(i + a) < image.rows &&
						(j + b) >= 0 &&
						(j + b) < image.cols)
					{
						suma3 += (image.at<uchar>(i + a, j + b)*vertP[y][x]);
					}
					x++;
				}
				y++;
			}
			prewittV.at<float>(i, j) = abs(suma3);
		}
	}

	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			float xValue = (prewittH.at<float>(i, j)*prewittH.at<float>(i, j));
			float yValue = (prewittV.at<float>(i, j)*prewittV.at<float>(i, j));
			raizP.at<float>(i, j) = (sqrt(xValue + yValue));
		}
	}
	//normalizar la foto
	minH = 0;
	maxH = 0;
	minR = 0;
	maxR = 0;
	minV = 0;
	maxV = 0;

	Mat prewittHR(image.rows, image.cols, CV_8UC1, Scalar(0));
	Mat prewittVR(image.rows, image.cols, CV_8UC1, Scalar(0));
	Mat raizPR(image.rows, image.cols, CV_8UC1, Scalar(0));
	minMaxLoc(prewittH, &minH, &maxH);
	minMaxLoc(prewittV, &minV, &maxV);
	minMaxLoc(raizP, &minR, &maxR);

	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			float valH = 0;
			float valV = 0;
			float valR = 0;
			valH = (255 * (prewittH.at<float>(i, j) - minH)) / (maxH - minH);
			valV = (255 * (prewittV.at<float>(i, j) - minV)) / (maxV - minV);
			valR = (255 * (raizP.at<float>(i, j) - minR)) / (maxR - minR);
			prewittHR.at<uchar>(i, j) = abs(valH);
			prewittVR.at<uchar>(i, j) = abs(valV);
			raizPR.at<uchar>(i, j) = abs(valR);
		}
	}
	
	
	//------------------------------------ ROBERTS -----------------------------------

	Mat robertH(image.rows, image.cols, CV_32FC1, Scalar(0));
	Mat robertV(image.rows, image.cols, CV_32FC1, Scalar(0));
	Mat raizRob(image.rows, image.cols, CV_32FC1, Scalar(0));

	for (int i = 1; i < image.rows-1; i++)
	{
		for (int j = 1; j < image.cols-1; j++)
		{
			float resH = 0;
			float resV = 0;
			resH = -1 * (image.at<uchar>(i, j)) + (image.at<uchar>(i + 1, j + 1));
			resV = -1*(image.at<uchar>(i, j+1)) + (image.at<uchar>(i + 1, j));
			robertH.at<float>(i, j) = abs(resH);
			robertV.at<float>(i, j) = abs(resV);
		}
	}
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			float xValue = (robertH.at<float>(i, j)*robertH.at<float>(i, j));
			float yValue = (robertV.at<float>(i, j)*robertV.at<float>(i, j));
			raizRob.at<float>(i, j) = (sqrt(xValue + yValue));
		}
	}
	//normalizar la foto
	minH = 0;
	maxH = 0;
	minR = 0;
	maxR = 0;
	minV = 0;
	maxV = 0;

	Mat robertHR(image.rows, image.cols, CV_8UC1, Scalar(0));
	Mat robertVR(image.rows, image.cols, CV_8UC1, Scalar(0));
	Mat raizRobR(image.rows, image.cols, CV_8UC1, Scalar(0));
	minMaxLoc(robertH, &minH, &maxH);
	minMaxLoc(robertV, &minV, &maxV);
	minMaxLoc(raizRob, &minR, &maxR);

	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			float valH = 0;
			float valV = 0;
			float valR = 0;
			valH = (255 * (robertH.at<float>(i, j) - minH)) / (maxH - minH);
			valV = (255 * (robertV.at<float>(i, j) - minV)) / (maxV - minV);
			valR = (255 * (raizRob.at<float>(i, j) - minR)) / (maxR - minR);
			robertHR.at<uchar>(i, j) = abs(valH);
			robertVR.at<uchar>(i, j) = abs(valV);
			raizRobR.at<uchar>(i, j) = abs(valR);
		}
	}

	namedWindow("orig", WINDOW_AUTOSIZE);
	imshow("orig", image);
	
	

	/*namedWindow("gauss sigma 3", WINDOW_AUTOSIZE);
	imshow("gauss sigma 3", gaussiano3);

	namedWindow("cambios", WINDOW_AUTOSIZE);
	imshow("cambios", cambios);

	namedWindow("acentuada", WINDOW_AUTOSIZE);
	imshow("acentuada", acentuada);*/

	namedWindow("sobel h", WINDOW_AUTOSIZE);
	imshow("sobel h", sobelHR);

	namedWindow("sobel V", WINDOW_AUTOSIZE);
	imshow("sobel V", sobelVR);

	namedWindow("raiz", WINDOW_AUTOSIZE);
	imshow("raiz", raizR);
	
	



	
	/*
	namedWindow("sobel h", WINDOW_AUTOSIZE);
	imshow("sobel h", sobelHR);

	namedWindow("sobel v", WINDOW_AUTOSIZE);
	imshow("sobel v", sobelVR);

	namedWindow("raiz R", WINDOW_AUTOSIZE);
	imshow("raiz R", raizR);
	*/
	

	namedWindow("prewitt h", WINDOW_AUTOSIZE);
	imshow("prewitt h", prewittHR);

	namedWindow("prewitt V", WINDOW_AUTOSIZE);
	imshow("prewitt V", prewittVR);

	namedWindow("prewitt raiz", WINDOW_AUTOSIZE);
	imshow("prewitt raiz", raizPR);
	
	
	namedWindow("robert H", WINDOW_AUTOSIZE);
	imshow("robert H", robertHR);

	namedWindow("robert V", WINDOW_AUTOSIZE);
	imshow("robert V", robertVR);

	namedWindow("Rob raiz", WINDOW_AUTOSIZE);
	imshow("Rob raiz", raizRobR);
	
	



	waitKey(0);
}