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

void solve::balls2(const Mat &input){

	//gray value
	namedWindow("balls - gray", CV_WINDOW_NORMAL);
	Mat gray = im::grayscale(input);
	imshow("balls - gray", gray);

	namedWindow("stretch", CV_WINDOW_NORMAL);
	Mat stretch = im::equalize(gray);
	imshow("stretch", stretch);

	///////////// x-Derivative ////////////////////
	Mat kernelX1 = ( Mat_<float>(1,5) <<
					1, -8, 0, 8, -1);

	///////////// x-Derivative ////////////////////
	Mat kernelX2 = ( Mat_<float>(1,5) <<
					-1, 8, 0, -8, 1);

	///////////// y-Derivative ////////////////////
	Mat kernelY1 = ( Mat_<float>(5,1) <<
					1, -8, 0, 8, -1);

	///////////// y-Derivative ////////////////////
	Mat kernelY2 = ( Mat_<float>(5,1) <<
					-1, 8, 0, -8, 1);

	int divide_fact = 1;

	//x-derivative
	namedWindow("xDerivative", CV_WINDOW_NORMAL);
	Mat xDerivative = im::filter(gray, kernelX1, divide_fact) +
			im::filter(gray, kernelX2, divide_fact) +
			im::filter(gray, kernelY1, divide_fact) +
			im::filter(gray, kernelY2, divide_fact);
	imshow("xDerivative", xDerivative);

	namedWindow("average", CV_WINDOW_NORMAL);
	Mat average = im::gaussianFilter(xDerivative, 15, 3);
	imshow("average", average*255);

	namedWindow("diffGauss", CV_WINDOW_NORMAL);
	Mat diffGauss = im::divideMatrix(im::gaussianFilter(gray, 15, 2), im::gaussianFilter(gray, 15, 4)*3);
	imshow("diffGauss", diffGauss);

	namedWindow("diff", CV_WINDOW_NORMAL);
	Mat diff = im::subtractMatrix(im::gaussianFilter(gray, 15, 4), gray);
	imshow("diff", diff);

	namedWindow("quantization", CV_WINDOW_NORMAL);
	Mat quantization = im::quantization(gray, 10);
	imshow("quantization", quantization);

	namedWindow("median", CV_WINDOW_NORMAL);
	Mat median = im::medianFilter(quantization, 11, 11);
	imshow("median", median);

	namedWindow("inverted", CV_WINDOW_NORMAL);
	Mat inverted = im::invertGray(average*255);
	imshow("inverted", inverted); //TODO



//	Mat element = Mat::ones(5,5, CV_8UC1);
	Mat erode;
	inverted.copyTo(erode);
	for (int i =0; i< 15; i++){
		 erode = im::morphErode(erode);
	}
	namedWindow("close", CV_WINDOW_NORMAL);
	imshow("close", erode);

	//thresholding
	Mat threshold = im::threshold(erode, 240 );
	namedWindow("balls - thresholded", CV_WINDOW_NORMAL);
	imshow("balls - thresholded", threshold);

	Mat dilate;
	threshold.copyTo(dilate);
	for (int i =0; i< 22; i++){
		 dilate = im::morphDilate(dilate);
	}
	namedWindow("dilate", CV_WINDOW_NORMAL);
	imshow("dilate", dilate);


	//label
	Mat label = im::binaryLabel(dilate);
	namedWindow("balls - label", CV_WINDOW_NORMAL);
	imshow("balls - label", label);

//	Mat erode = im::morphOpen(close);
//	namedWindow("erode", CV_WINDOW_NORMAL);
//	imshow("erode", erode);

//	namedWindow("highpass", CV_WINDOW_NORMAL);
//	Mat highpass = im::divideMatrix(gray, average);
//	imshow("highpass", highpass);
//
//	namedWindow("lowContrast", CV_WINDOW_NORMAL);
//	Mat lowContrast = im::divideMatrix(gray, stretch);
//	imshow("lowContrast", lowContrast);
//
//	namedWindow("minMax", CV_WINDOW_NORMAL);
//	Mat minMax = im::localMinimumOfMaximum(gray, 5, 5);
//	imshow("minMax", minMax);
//
//	namedWindow("maxMin", CV_WINDOW_NORMAL);
//	Mat maxMin = im::localMaximumOfMinimum(gray, 5, 5);
//	imshow("maxMin", maxMin);
}


void solve::balls(const Mat &input){

	Mat gray = im::grayscale(input);
	namedWindow("balls - gray", CV_WINDOW_NORMAL);
	imshow("balls - gray", gray);

	// smoothing
	Mat smooth = im::medianFilter(gray, 3, 9);
	namedWindow("balls - smooth", CV_WINDOW_NORMAL);
	imshow("balls - smooth", smooth);

	//histogram
	Mat histogram = im::showHist(smooth);
	namedWindow("balls - histogram", CV_WINDOW_NORMAL);
	imshow("balls - histogram", histogram);

	//thresholding
	Mat threshold = im::threshold(smooth, 90);
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

	//dilate a couple times
	Mat dilate;
	erode.copyTo(dilate);
	for (int i =0; i<25;i++){
		dilate = im::morphDilate(dilate);
	}
	namedWindow("balls - dilated", CV_WINDOW_NORMAL);
	imshow("balls - dilated", dilate);

	//label
	Mat label = im::binaryLabel(dilate);
	namedWindow("balls - label", CV_WINDOW_NORMAL);
	imshow("balls - label", label);
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

	Mat gray = im::grayscale(input);
	gray = im::averageFilter(gray, 5, 5, im::PWHITE);
	namedWindow("boltsnuts - gray", CV_WINDOW_NORMAL);
	imshow("boltsnuts - gray", gray);

	Mat segmented = im::threshold(gray, 245 );
	namedWindow("segmented", CV_WINDOW_NORMAL);
	imshow("segmented", segmented);

	Mat expandedBorder = Mat::zeros(segmented.size(), CV_8UC1);
	expandedBorder.row(0) = 255;
	expandedBorder.col(0) = 255;
	expandedBorder.row(segmented.rows-1) = 255;
	expandedBorder.col(segmented.cols-1) = 255;
	expandedBorder = im::morphGeodesicDilate(expandedBorder, segmented);
	namedWindow("expandedBorder", CV_WINDOW_NORMAL);
	imshow("expandedBorder", expandedBorder);

	Mat circles = im::subtractMatrix(segmented, expandedBorder);
	namedWindow("circles", CV_WINDOW_NORMAL);
	imshow("circles", circles);

	Mat openedCircles = im::morphErode(circles);
	openedCircles = im::morphErode(openedCircles);
	openedCircles = im::morphErode(openedCircles);
	openedCircles = im::morphErode(openedCircles);
	namedWindow("openedCircles", CV_WINDOW_NORMAL);
	imshow("openedCircles", openedCircles);

	Mat geocircles = im::morphGeodesicDilate(openedCircles, circles);
	namedWindow("geocircles", CV_WINDOW_NORMAL);
	imshow("geocircles", geocircles);

	Mat labeledCircles = im::binaryLabelCircle(geocircles);
	namedWindow("labeledCircles", CV_WINDOW_NORMAL);
	imshow("labeledCircles", labeledCircles);

}

void solve::road(const Mat &input){

	Mat gray = im::grayscale(input);
	namedWindow("road - gray", CV_WINDOW_NORMAL);
	imshow("road - gray", gray);

	// smoothing
//	Mat smooth = im::medianFilter(eq, 3, 9);
//	namedWindow("road - smooth", CV_WINDOW_NORMAL);
//	imshow("road - smooth", smooth);

	Mat smooth = im::gaussianFilter(gray, 15, 1);
	namedWindow("road - smooth", CV_WINDOW_NORMAL);
	imshow("road - smooth", smooth);

	Mat quant = im::quantization(smooth, 8);
	namedWindow("road - quantization", CV_WINDOW_NORMAL);
	imshow("road - quantization", quant);

	//histogram
	Mat histogram = im::showHist(quant);
	namedWindow("road - histogram quant", CV_WINDOW_NORMAL);
	imshow("road - histogram quant", histogram);

	Mat eq = im::equalize(quant);
	namedWindow("road - equalized", CV_WINDOW_NORMAL);
	imshow("road - equalized", eq);

	//histogram
	Mat histogram2 = im::showHist(eq);
	namedWindow("road - histogram eq-quant", CV_WINDOW_NORMAL);
	imshow("road - histogram eq-quant", histogram2);

	//thresholding
	Mat threshold = im::threshold(eq, 225);
	namedWindow("road - thresholded", CV_WINDOW_NORMAL);
	imshow("road - thresholded", threshold);

	Mat dilate;
	threshold.copyTo(dilate);
	for(int i =0; i < 3; i++){
		dilate = im::morphDilate(dilate);
	}
	namedWindow("road - dilate", CV_WINDOW_NORMAL);
	imshow("road - dilate", dilate);

	Mat labeled = im::binaryLabel(dilate);
	namedWindow("labeledCircles", CV_WINDOW_NORMAL);
	imshow("labeledCircles", labeled);

//	Mat skel;
//	im::normalizeLetter(threshold, skel);
//	namedWindow("skeleton", CV_WINDOW_NORMAL);
//	imshow("skeleton", skel);

	//erode
//	Mat element = Mat::ones(5,5, CV_8UC1);
//	Mat close;
//	threshold.copyTo(close);
//	for (int i =0; i< 50; i++){
//		 close = im::morphClose(close, element, element);
//	}
//	namedWindow("road - close", CV_WINDOW_NORMAL);
//	imshow("road - erode", close);
}

void solve::xray(const Mat &input){

}
/*
////////////////// Segmented bit try/////////////////////////
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


////////////////// Ardillo bit try/////////////////////////
void solve::xray(const cv::Mat &input){
	// to grayscale
	Mat gray = im::grayscale(input);
	namedWindow("xray - grayscale", CV_WINDOW_NORMAL);
	imshow("xray - grayscale", gray);

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
		namedWindow("xray - gauss filtered", CV_WINDOW_NORMAL);
		imshow("xray - gauss filtered", filtered);

		Mat histogram = im::showHist(filtered);
		namedWindow("histogram: Own grayImage", CV_WINDOW_NORMAL);
		imshow("histogram: Own grayImage", histogram);

		Mat threshold = im::threshold(gray, 100);
		namedWindow("xray - threshold", CV_WINDOW_NORMAL);
		imshow("xray - threshold", threshold);

		Mat elementD = Mat::ones(5,5, CV_8UC1);
		Mat elementE = Mat::ones(3,3, CV_8UC1);

		Mat closing;
		threshold.copyTo(closing);

		for (int q=0;q<4;q++){
			closing = im::morphClose(closing, elementD, elementE);
		}
		closing = im::morphErode(closing);
		closing = im::morphErode(closing);


		namedWindow("xray - closing", CV_WINDOW_NORMAL);
		imshow("xray - closing", closing);

		Mat inverse = im::invertGray(gray);
		Mat mask = im::invertGray(threshold); // edited


		for (int q=0;q<8;q++){
			mask = im::morphDilate(mask);
		}
		namedWindow("mask", CV_WINDOW_NORMAL);
		imshow("mask", mask);

		Mat object = im::subtractMatrix(inverse, mask);
		namedWindow("xray - objects", CV_WINDOW_NORMAL);
		imshow("xray - objects", object);

		Mat test = im::morphDilate(gray);
		Mat edge = im::subtractMatrix(test, gray);
		namedWindow("test", CV_WINDOW_NORMAL);
		imshow("test", edge);
		Mat thresholdTest = im::threshold(edge, 18);
		namedWindow("xray - thresholdTest", CV_WINDOW_NORMAL);
		imshow("xray - thresholdTest", thresholdTest);

		Mat end = im::subtractMatrix(object, thresholdTest);
		end = im::morphDilate(end);
		end = im::invertGray(end);
		closing = im::invertGray(closing);
		end = im::subtractMatrix(end, closing);
		namedWindow("end", CV_WINDOW_NORMAL);
		imshow("end", end);

		Mat sub = im::subtractMatrix(closing, filtered);
		sub = im::morphErode(sub);
		sub = im::morphErode(sub);

		Mat test = im::morphDilate(gray);
		Mat edge = im::subtractMatrix(test, gray);
		namedWindow("test", CV_WINDOW_NORMAL);
		imshow("test", edge);
		Mat thresholdTest = im::threshold(edge, 18);
		namedWindow("xray - thresholdTest", CV_WINDOW_NORMAL);
		imshow("xray - thresholdTest", thresholdTest);

		namedWindow("xray - sub", CV_WINDOW_NORMAL);
		imshow("xray - sub", sub);

		///////////// x-Derivative ////////////////////
		Mat kernelX1 = ( Mat_<float>(1,5) <<
			1, -8, 0, 8, -1);

		///////////// x-Derivative ////////////////////
		Mat kernelX2 = ( Mat_<float>(1,5) <<
			-1, 8, 0, -8, 1);

		///////////// y-Derivative ////////////////////
		Mat kernelY1 = ( Mat_<float>(5,1) <<
			1, -8, 0, 8, -1);

		///////////// y-Derivative ////////////////////
		Mat kernelY2 = ( Mat_<float>(5,1) <<
			-1, 8, 0, -8, 1);

		divide_fact = 12;

		Mat x_deriv1 = im::filter(filtered, kernelX1, divide_fact);
		namedWindow("xray - x derivative2", CV_WINDOW_NORMAL);
		imshow("xray - x derivative2", x_deriv1);

		Mat x_deriv2= im::filter(filtered, kernelX2, divide_fact);
		namedWindow("xray - x derivative2", CV_WINDOW_NORMAL);
		imshow("xray - x derivative2", x_deriv2);

		Mat y_deriv1= im::filter(filtered, kernelY1, divide_fact);
		namedWindow("xray - y derivative1", CV_WINDOW_NORMAL);
		imshow("xray - y derivative1", y_deriv1);

		Mat y_deriv2= im::filter(filtered, kernelY2, divide_fact);
		namedWindow("xray - y derivative2", CV_WINDOW_NORMAL);
		imshow("xray - y derivative2", y_deriv2);

		Mat threshold1 = im::threshold(x_deriv1, 10);
		namedWindow("xray - threshold1", CV_WINDOW_NORMAL);
		imshow("xray - threshold1", threshold1);

		Mat threshold2 = im::threshold(x_deriv2, 10);
		namedWindow("xray - threshold2", CV_WINDOW_NORMAL);
		imshow("xray - threshold2", threshold2);

		Mat threshold3 = im::threshold(y_deriv1, 10);
		namedWindow("xray - threshold3", CV_WINDOW_NORMAL);
		imshow("xray - threshold3", threshold3);

		Mat threshold4 = im::threshold(y_deriv2, 10);
		namedWindow("xray - threshold4", CV_WINDOW_NORMAL);
		imshow("xray - threshold4", threshold4);

		Mat only_x = im::addMatrix(threshold1, threshold2);
		namedWindow("xray - x derivative", CV_WINDOW_NORMAL);
		imshow("xray - x derivative", only_x);

		Mat only_y = im::addMatrix(threshold3, threshold4);
		namedWindow("xray - y derivative", CV_WINDOW_NORMAL);
		imshow("xray - y derivative", only_y);

		Mat derivative_tot = im::addMatrix(only_x, only_y);
		namedWindow("xray - derivative", CV_WINDOW_NORMAL);
		imshow("xray - derivative", derivative_tot);
}
*/

