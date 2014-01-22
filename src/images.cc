/*
 * images.c
 *
 *  Created on: Jan 20, 2014
 *      Author: ardillo
 */

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "includes/stockpile.h"
#include "includes/images.h"
#include "includes/statistics.h"
#include "includes/morphology.h"
#include "includes/filters.h"
#include "config.h"

using namespace std;
using namespace cv;

void solve::balloons(const Mat &input){
	Mat gray = im::grayscale(input);

	namedWindow("ballloons - gray", CV_WINDOW_NORMAL);
	imshow("ballloons - gray", gray);


	Mat kernel = ( Mat_<float>(1,5) <<
		1, -8, 0, 8, -1);
		float divide_fact = 12;

	/*
	Mat kernel = ( Mat_<float>(1,3) <<
		-1, 0, 1);
		float divide_fact = 2;
		*/

	Mat output = im::filter(gray, kernel, divide_fact);
	namedWindow("balloons", CV_WINDOW_NORMAL);
	imshow("balloons", output);
}

void solve::balls(const Mat &input){
	Mat gray = im::grayscale(input);

	namedWindow("balls - gray", CV_WINDOW_NORMAL);
	imshow("balls - gray", gray);

	////////////// Gaussian kernel /////////////////
	Mat kernel = ( Mat_<float>(5,5) <<
		1, 4, 7, 4, 1,
		4, 16, 26, 16, 4,
		7, 26, 41, 26, 7,
		4, 16, 26, 16, 4,
		1, 4, 7, 4, 1);
		float divide_fact = 273;

	// smoothing and equalizing afterwards
	Mat smooth = im::filter(gray, kernel, divide_fact);

	smooth = im::equalize(smooth);
	namedWindow("balls - smooth-eq", CV_WINDOW_NORMAL);
	imshow("balls - smooth-eq", smooth);

	//histogram
	Mat histogram = im::showHist(smooth);
	namedWindow("balls - histogram", CV_WINDOW_NORMAL);
	imshow("balls - histogram", histogram);

	//thresholding
	Mat threshold = im::threshold(smooth, 140);
	namedWindow("balls - thresholded 1", CV_WINDOW_NORMAL);
	imshow("balls - thresholded 1", threshold);

	//erode
	Mat erode;
	threshold.copyTo(erode);
	for (int i =0; i<25;i++){
		erode = im::morphErode(erode);
	}
	namedWindow("balls - eroded", CV_WINDOW_NORMAL);
	imshow("balls - eroded", erode);

	//dilate a coule times

}

void solve::cheese(const Mat &input){

	// to grayscale
	Mat gray = im::grayscale(input);
	namedWindow("cheese - grayscale", CV_WINDOW_NORMAL);
	imshow("cheese - grayscale", gray);

	//gaussian filter

	////////////// Gaussian kernel /////////////////
		Mat kernel = ( Mat_<float>(5,5) <<
			1, 4, 7, 4, 1,
			4, 16, 26, 16, 4,
			7, 26, 41, 26, 7,
			4, 16, 26, 16, 4,
			1, 4, 7, 4, 1);
			float divide_fact = 273;

	Mat filtered = im::filter(gray, kernel, divide_fact);
	namedWindow("cheese - gauss filtered", CV_WINDOW_NORMAL);
	imshow("cheese - gauss filtered", filtered);

	//histogram
	Mat histogram = im::showHist(filtered);
	namedWindow("cheese - histogram", CV_WINDOW_NORMAL);
	imshow("cheese - histogram", histogram);

	//thresholding
	Mat threshold1 = im::threshold(filtered, 190);
	namedWindow("cheese - thresholded 1", CV_WINDOW_NORMAL);
	imshow("cheese - thresholded 1", threshold1);

	//closing
	Mat element = Mat::ones(5, 5, CV_8UC1);
	Mat closed = im::morphErode(threshold1, element);
	namedWindow("cheese - closed", CV_WINDOW_NORMAL);
	imshow("cheese - closed", closed);

	//inverse
	Mat inverse = im::invertGray(closed);
	namedWindow("cheese - inverse", CV_WINDOW_NORMAL);
	imshow("cheese - inverse", inverse);

	//labeling
	Mat labels = im::binaryLabel(inverse);
	namedWindow("cheese - label", CV_WINDOW_NORMAL);
	imshow("cheese - label", labels);

	//delete border objects
	Mat label_cleaned = im::deleteBorderObjects(labels);
	namedWindow("cheese - label without border objects", CV_WINDOW_NORMAL);
	imshow("cheese - label without border objects", label_cleaned);

}

void solve::xray(const cv::Mat &input){
//	// to grayscale
//	Mat gray = im::grayscale(input);
//	namedWindow("xray - grayscale", CV_WINDOW_NORMAL);
//	imshow("xray - grayscale", gray);
//
//	//gaussian filter
//
//		////////////// Gaussian kernel /////////////////
//			Mat kernel = ( Mat_<float>(5,5) <<
//				1, 4, 7, 4, 1,
//				4, 16, 26, 16, 4,
//				7, 26, 41, 26, 7,
//				4, 16, 26, 16, 4,
//				1, 4, 7, 4, 1);
//				float divide_fact = 273;
//
//		Mat filtered = im::filter(gray, kernel, divide_fact);
//		namedWindow("xray - gauss filtered", CV_WINDOW_NORMAL);
//		imshow("xray - gauss filtered", filtered);
//
////		Mat histogram = im::showHist(filtered);
////		namedWindow("histogram: Own grayImage", CV_WINDOW_NORMAL);
////		imshow("histogram: Own grayImage", histogram);
//
//		Mat threshold = im::threshold(gray, 100);
//		namedWindow("xray - threshold", CV_WINDOW_NORMAL);
//		imshow("xray - threshold", threshold);
//
//		Mat elementD = Mat::ones(5,5, CV_8UC1);
//		Mat elementE = Mat::ones(3,3, CV_8UC1);
//
//		Mat closing;
//		threshold.copyTo(closing);
//
//		for (int q=0;q<4;q++){
//			closing = im::morphClose(closing, elementD, elementE);
//		}
//		closing = im::morphErode(closing);
//		closing = im::morphErode(closing);
//
//
//		namedWindow("xray - closing", CV_WINDOW_NORMAL);
//		imshow("xray - closing", closing);
//
//		Mat inverse = im::invertGray(gray);
//		Mat mask = im::invertGray(threshold); // edited
//
//
////		for (int q=0;q<8;q++){
////			mask = im::morphDilate(mask);
////		}
//		namedWindow("mask", CV_WINDOW_NORMAL);
//		imshow("mask", mask);
//
//		Mat object = im::subtractMatrix(inverse, mask);
//		namedWindow("xray - objects", CV_WINDOW_NORMAL);
//		imshow("xray - objects", object);
//
//		Mat test = im::morphDilate(gray);
//		Mat edge = im::subtractMatrix(test, gray);
//		namedWindow("test", CV_WINDOW_NORMAL);
//		imshow("test", edge);
//		Mat thresholdTest = im::threshold(edge, 18);
//		namedWindow("xray - thresholdTest", CV_WINDOW_NORMAL);
//		imshow("xray - thresholdTest", thresholdTest);
//
//		Mat end = im::subtractMatrix(object, thresholdTest);
//		end = im::morphDilate(end);
//		end = im::invertGray(end);
//		closing = im::invertGray(closing);
//		end = im::subtractMatrix(end, closing);
//		namedWindow("end", CV_WINDOW_NORMAL);
//		imshow("end", end);
//
////		Mat sub = im::subtractMatrix(closing, filtered);
////		sub = im::morphErode(sub);
////		sub = im::morphErode(sub);
////
////		Mat test = im::morphDilate(gray);
////		Mat edge = im::subtractMatrix(test, gray);
////		namedWindow("test", CV_WINDOW_NORMAL);
////		imshow("test", edge);
////		Mat thresholdTest = im::threshold(edge, 18);
////		namedWindow("xray - thresholdTest", CV_WINDOW_NORMAL);
////		imshow("xray - thresholdTest", thresholdTest);
////
////		namedWindow("xray - sub", CV_WINDOW_NORMAL);
////		imshow("xray - sub", sub);
//
////		///////////// x-Derivative ////////////////////
////		Mat kernelX1 = ( Mat_<float>(1,5) <<
////			1, -8, 0, 8, -1);
////
////		///////////// x-Derivative ////////////////////
////		Mat kernelX2 = ( Mat_<float>(1,5) <<
////			-1, 8, 0, -8, 1);
////
////		///////////// y-Derivative ////////////////////
////		Mat kernelY1 = ( Mat_<float>(5,1) <<
////			1, -8, 0, 8, -1);
////
////		///////////// y-Derivative ////////////////////
////		Mat kernelY2 = ( Mat_<float>(5,1) <<
////			-1, 8, 0, -8, 1);
////
////		divide_fact = 12;
////
////		Mat x_deriv1 = im::filter(filtered, kernelX1, divide_fact);
////		namedWindow("xray - x derivative2", CV_WINDOW_NORMAL);
////		imshow("xray - x derivative2", x_deriv1);
////
////		Mat x_deriv2= im::filter(filtered, kernelX2, divide_fact);
////		namedWindow("xray - x derivative2", CV_WINDOW_NORMAL);
////		imshow("xray - x derivative2", x_deriv2);
////
////		Mat y_deriv1= im::filter(filtered, kernelY1, divide_fact);
////		namedWindow("xray - y derivative1", CV_WINDOW_NORMAL);
////		imshow("xray - y derivative1", y_deriv1);
////
////		Mat y_deriv2= im::filter(filtered, kernelY2, divide_fact);
////		namedWindow("xray - y derivative2", CV_WINDOW_NORMAL);
////		imshow("xray - y derivative2", y_deriv2);
////
////		Mat threshold1 = im::threshold(x_deriv1, 10);
////		namedWindow("xray - threshold1", CV_WINDOW_NORMAL);
////		imshow("xray - threshold1", threshold1);
////
////		Mat threshold2 = im::threshold(x_deriv2, 10);
////		namedWindow("xray - threshold2", CV_WINDOW_NORMAL);
////		imshow("xray - threshold2", threshold2);
////
////		Mat threshold3 = im::threshold(y_deriv1, 10);
////		namedWindow("xray - threshold3", CV_WINDOW_NORMAL);
////		imshow("xray - threshold3", threshold3);
////
////		Mat threshold4 = im::threshold(y_deriv2, 10);
////		namedWindow("xray - threshold4", CV_WINDOW_NORMAL);
////		imshow("xray - threshold4", threshold4);
////
////		Mat only_x = im::addMatrix(threshold1, threshold2);
////		namedWindow("xray - x derivative", CV_WINDOW_NORMAL);
////		imshow("xray - x derivative", only_x);
////
////		Mat only_y = im::addMatrix(threshold3, threshold4);
////		namedWindow("xray - y derivative", CV_WINDOW_NORMAL);
////		imshow("xray - y derivative", only_y);
////
////		Mat derivative_tot = im::addMatrix(only_x, only_y);
////		namedWindow("xray - derivative", CV_WINDOW_NORMAL);
////		imshow("xray - derivative", derivative_tot);
}
void solve::boltsnuts(const Mat &input){

}

