#include <opencv2\opencv.hpp>
#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;

void FireDetection(Mat &image_input, Mat &image_output);
void DrawFire(Mat &inputImg, Mat foreImg);
int main(int argc, char** argv) {
	Mat src = imread("E:\\vs_work\\firedetection\\7.jpg");
	Mat dst;

	FireDetection(src,dst);

	imwrite("E:\\vs_work\\result\\10_binary_image.jpg",dst);
	imwrite("E:\\vs_work\\result\\10_Detect_Fire.jpg", src);

	waitKey(0);

	return 0;
}

void FireDetection(Mat &image_input, Mat &image_output)
{
	image_output = Mat::zeros(image_input.size(), CV_8UC1); //初始化output矩阵
	if (image_input.empty()) {
		printf("could not load image...please try again!/n ");
	}
	if (image_output.empty()||image_output.channels()!= 1) {
		printf("could not initialize output...please try again!/n ");
	}
	/*初始化程序报错*/

	for (int i = 0; i < image_input.rows; i++) {
		for (int j = 0; j < image_input.cols; j++) {
			float R, G, B;
			B = image_input.at<uchar>(i, image_input.channels()*j + 0);
			G = image_input.at<uchar>(i, image_input.channels()*j + 1);
			R = image_input.at<uchar>(i, image_input.channels()*j + 2);

			int minRGB = min(min(R, G), B);
			double S = 1 - 3.0*minRGB / (R + G + B);

			int RedThreshold = 115;
			if (R > RedThreshold && R > G&&G > B)
			{
				image_output.at<uchar>(i, j) = 255;
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