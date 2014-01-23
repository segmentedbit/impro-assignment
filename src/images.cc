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
	imshow("inverted", inverted);

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

	Mat smooth = im::gaussianFilter(gray, 3, 1);
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

	Mat invert = im::invertGray(dilate);

	Mat mask = Mat::zeros(invert.rows, invert.cols, CV_8UC1);
	mask.row(0) = 255;
	mask.col(0) = 255;
	mask.row(mask.rows - 1) = 255;
	mask.col(mask.cols - 1) = 255;

	Mat geoDil = im::morphGeodesicDilate(mask, dilate);
	namedWindow("road - Geo dilate", CV_WINDOW_NORMAL);
	imshow("road - Geo dilate", geoDil);

}

/*
TODO has to be removed
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

*/

void solve::xray(const cv::Mat &input){
	// to grayscale
	Mat gray = im::grayscale(input);
	namedWindow("xray - grayscale", CV_WINDOW_NORMAL);
	imshow("xray - grayscale", gray);

	Mat threshold = im::threshold(gray, 100);
	threshold = im::invertGray(threshold);
	namedWindow("xray - threshold", CV_WINDOW_NORMAL);
	imshow("xray - threshold", threshold);

	Mat inverse = im::invertGray(gray);
	namedWindow("xray - inverse", CV_WINDOW_NORMAL);
	imshow("xray - inverse", inverse);

	Mat dilMask = Mat::zeros(gray.rows, gray.cols, CV_8UC1);
	dilMask.row(0) = 255;
	dilMask.col(0) = 255;
	dilMask.row(dilMask.rows - 1) = 255;
	dilMask.col(dilMask.cols - 1) = 255;

	Mat geoDil = im::morphGeodesicDilate(dilMask, inverse);
	namedWindow("xray - Geo dilate", CV_WINDOW_NORMAL);
	imshow("xray - Geo dilate", geoDil);

	Mat edge = im::subtractMatrix(threshold, geoDil);
	namedWindow("xray - end", CV_WINDOW_NORMAL);
	imshow("xray - end", edge);

	Mat silhoutte = im::threshold(edge, 35);
	namedWindow("xray - silhoutte", CV_WINDOW_NORMAL);
	imshow("xray - silhoutte", silhoutte);

	Mat label = im::binaryLabel(silhoutte);
	namedWindow("xray - label", CV_WINDOW_NORMAL);
	imshow("xray - label", label);

}
