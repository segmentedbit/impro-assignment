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
	imshow("ballls - gray", gray);

	////////////// Gaussian kernel /////////////////
	Mat kernel = ( Mat_<float>(5,5) <<
		1, 4, 7, 4, 1,
		4, 16, 26, 16, 4,
		7, 26, 41, 26, 7,
		4, 16, 26, 16, 4,
		1, 4, 7, 4, 1);
		float divide_fact = 273;
	Mat smooth = im::filter(gray, kernel, divide_fact);

	namedWindow("balls - smooth", CV_WINDOW_NORMAL);
	imshow("ballls - smooth", smooth);

	Mat quant = im::quantization(smooth, 20);

	namedWindow("balls - quantization", CV_WINDOW_NORMAL);
	imshow("ballls - quantization", quant);

	///////////// x-Derivative ////////////////////
	kernel = ( Mat_<float>(1,5) <<
		1, -8, 0, 8, -1);
		divide_fact = 12;
	Mat xDer = im::filter(smooth, kernel, divide_fact);

	///////////// y-Derivative ////////////////////
	kernel = ( Mat_<float>(5,1) <<
		1, -8, 0, 8, -1);
		divide_fact = 12;
	Mat yDer = im::filter(smooth, kernel, divide_fact);

	Mat edges = im::addMatrix(xDer, yDer);
	namedWindow("ballsEdges", CV_WINDOW_NORMAL);
	imshow("ballsEdges", edges);
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

void solve::boltsnuts(const Mat &input){
	namedWindow("boltsnuts - gray", CV_WINDOW_NORMAL);
	namedWindow("segmented", CV_WINDOW_NORMAL);
	namedWindow("expandedBorder", CV_WINDOW_NORMAL);
	namedWindow("openedCircles", CV_WINDOW_NORMAL);
	namedWindow("perfect", CV_WINDOW_NORMAL);

	Mat gray = im::grayscale(input);

	Mat segmented = im::threshold(gray, 245 );
	Mat expandedBorder = Mat::zeros(segmented.size(), CV_8UC1);
	expandedBorder.row(0) = 255;
	expandedBorder.col(0) = 255;
	expandedBorder.row(segmented.rows-1) = 255;
	expandedBorder.col(segmented.cols-1) = 255;
	expandedBorder = im::morphGeodesicDilate(expandedBorder, segmented);

	Mat circles = im::subtractMatrix(segmented, expandedBorder);

	Mat openedCircles = im::morphErode(circles);
	openedCircles = im::morphErode(openedCircles);
	openedCircles = im::morphErode(openedCircles);
	openedCircles = im::morphErode(openedCircles);


	Mat perfect = im::morphGeodesicDilate(openedCircles, circles);
	imshow("boltsnuts - gray", gray);
	imshow("segmented", segmented);
	imshow("expandedBorder", expandedBorder);
	imshow("circles", circles);
	imshow("openedCircles", openedCircles);
	imshow("perfect", perfect);

	waitKey(0);
}

void solve::xray(const Mat &input){
	Mat gray = im::grayscale(input);

	namedWindow("xray - gray", CV_WINDOW_NORMAL);
	namedWindow("segmented", CV_WINDOW_NORMAL);
	namedWindow("minMax", CV_WINDOW_NORMAL);
	namedWindow("maxMin", CV_WINDOW_NORMAL);
	namedWindow("powImg", CV_WINDOW_NORMAL);
	namedWindow("stretch", CV_WINDOW_NORMAL);
	namedWindow("gauss1", CV_WINDOW_NORMAL);

	Mat segmented = im::threshold(gray, 70);
	Mat gauss1 = im::gaussianFilter(gray, 9, 2);

	Mat minMax = im::localMinimumOfMaximum(gauss1, 5, 5);
	Mat maxMin = im::localMaximumOfMinimum(gauss1, 5, 5);
	Mat powImg = maxMin.clone();

	for (int y=0; y<powImg.rows; y++) {
		for (int x=0; x<powImg.cols; x++) {
			powImg.at<uchar>(y,x) = static_cast<int>(powImg.at<uchar>(y,x)/2) * static_cast<int>(powImg.at<uchar>(y,x)/2);
		}
	}

	Mat stretch = im::equalize(gray);

	imshow("xray - gray", gray);
	imshow("segmented", segmented);
	imshow("minMax", minMax);
	imshow("maxMin", maxMin);
	imshow("powImg", powImg);
	imshow("stretch", stretch);
	imshow("gauss1", gauss1);
	waitKey(0);
}

