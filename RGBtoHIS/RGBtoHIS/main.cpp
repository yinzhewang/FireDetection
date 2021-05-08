#include <opencv2\opencv.hpp>
#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;

double Judge(double in);
void RGBtoHIS(Mat& image_input, Mat& image_output);
void DrawFire(Mat &inputImg, Mat foreImg);
void FireDetection(Mat &image_input, Mat &image_output);

int main(int argc, char** argv) {
	Mat src = imread("E:\\vs_work\\firedetection\\6.jpg");
	Mat dst;
   
	FireDetection(src, dst);
	//imshow("original picture", src);
	//imshow("convert picture", dst);
	//imwrite("E:\\vs_work\\result\\10_binary_image.jpg", dst);
	//imwrite("E:\\vs_work\\result\\10_Detect_Fire.jpg", src);

	waitKey(0);

	return 0;
}

void FireDetection(Mat &image_input, Mat &image_output)
{
	image_output = Mat::zeros(image_input.size(), CV_8UC1); //初始化output矩阵
	Mat image_middle;
	if (image_input.empty()) {
		printf("could not load image...please try again!/n ");
	}
	if (image_output.empty() || image_output.channels() != 1) {
		printf("could not initialize output...please try again!/n ");
	}
	/*初始化程序报错*/
	RGBtoHIS( image_input,  image_middle);

	for (int i = 0; i < image_middle.rows; i++) {
		for (int j = 0; j < image_middle.cols; j++) {
			double H, S, I;
			H = image_middle.at<uchar>(i, image_middle.channels()*j + 0);
			S = image_middle.at<uchar>(i, image_middle.channels()*j + 1);
			I = image_middle.at<uchar>(i, image_middle.channels()*j + 2);

			if ((H>0 && H<60) && S>20 && S<255 && I>30 && I<255)
			{
				image_output.at<uchar>(i, j) = 255;
				//printf("H = %f", H);
			}
			else
			{
				image_output.at<uchar>(i, j) = 0;
			}
		}
	}
	namedWindow("original image", WINDOW_AUTOSIZE);
	imshow("original image", image_input);
	namedWindow("binary image", WINDOW_AUTOSIZE);
	imshow("binary image", image_output);
	imshow("HIS image", image_middle);
	DrawFire(image_input, image_output);
}

void DrawFire(Mat &inputImg, Mat foreImg)
{
	vector<vector<Point>> contours_set;//保存轮廓提取后的点集及拓扑关系  

	findContours(foreImg, contours_set, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	Mat result0;
	Scalar holeColor;
	Scalar externalColor;

	vector<vector<Point>>::iterator iter = contours_set.begin();

	for (; iter != contours_set.end(); )   //迭代器循环
	{
		Rect rect = boundingRect(*iter);
		/*	float radius;
		Point2f center;
		minEnclosingCircle(*iter, center, radius);*/

		if (rect.area()> 0)
		{
			rectangle(inputImg, rect, Scalar(0, 255, 0));  //scalar表示是什么颜色去框选
			++iter;
		}
		else
		{
			iter = contours_set.erase(iter);
		}
	}
	imshow("Detect Fire", inputImg);
}

void RGBtoHIS(Mat& image_input, Mat& image_output)
{
	image_output = Mat::zeros(image_input.size(), CV_8UC3); //初始化output矩阵
	if (image_input.empty()) {
		printf("could not load RGBtoHIS image...please try again!/n ");
	}
	if (image_output.empty() || image_output.channels() != 3) {
		printf("could not initialize RGBtoHIS output...please try again!/n ");
	}
	/*初始化程序报错*/

	for (int i = 0; i < image_input.rows; i++) {
		for (int j = 0; j < image_input.cols; j++) {
			float R, G, B;
			B = image_input.at<uchar>(i, image_input.channels()*j + 0);
			G = image_input.at<uchar>(i, image_input.channels()*j + 1);
			R = image_input.at<uchar>(i, image_input.channels()*j + 2);
			

			//RGB转HIS公式编写
		/*	B = B / 255.0;
			G = G / 255.0;
			R = R / 255.0;*/
			float minRGB = min(min(R, G), B);

			double I = (R + B + G) / 3;
			double S = 1 - 3 * minRGB / (R + B + G);
			double H;
			double theta, den, num, esp = 0.0000001;
			double pi = 3.14159265;

			num = 0.5*((R - G) + (R - B));
			den = (R - G)*(R - G) + (R - B)*(G - B);
			den = sqrt(den);
			den = num / (den);   //den = num / (den+esp);
			theta = acos(den);

			if (G >= B)
			{
				H = theta;
			}
			else
			{
				H = 2 * pi - theta;
			}
			//H的单位是弧度
			//H = Judge(H * 255);
			//I = Judge(I * 255);
			//S = Judge(S * 255);
			H = H / (2 * pi);

			image_output.at<uchar>(i, image_output.channels()*j + 0) = H*360 ;
			image_output.at<uchar>(i, image_output.channels()*j + 1) = S*255 ;
			image_output.at<uchar>(i, image_output.channels()*j + 2) = I*255 ;
		}
	}
}

double Judge(double in)
{
	double out;
	if (in < 0)
	{
		out = 0;
	}
	if (in > 255)
	{
		out = 255;
	}
	else
	{
		out = in;
	}

	return out;
}