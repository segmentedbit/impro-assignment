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

Mat solve::balloons(const Mat &input){
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
	return output;
}

Mat solve::balls(const Mat &input){
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

	return edges;
}
