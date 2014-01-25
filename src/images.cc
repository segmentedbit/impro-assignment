/*
 * images.c
 *
 *  Created on: Jan 20, 2014
 *      Author: Ardillo & segmentedbit
 */

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "includes/stockpile.h"
#include "includes/images.h"
#include "includes/statistics.h"
#include "includes/morphology.h"
#include "includes/filters.h"
#include "includes/config.h"

using namespace std;
using namespace cv;

void solve::balloons(const Mat &input){

	// obviously not completed
	Mat gray = im::grayscale(input);

	namedWindow("ballloons - gray", CV_WINDOW_NORMAL);
	imshow("ballloons - gray", gray);

	Mat kernel = ( Mat_<float>(1,5) <<
		1, -8, 0, 8, -1);
		float divide_fact = 12;

	Mat output = im::filter(gray, kernel, divide_fact);
	namedWindow("balloons", CV_WINDOW_NORMAL);
		imshow("balloons", output);

	Mat thresholded = im::threshold(gray, 50);
	namedWindow("thresholded", CV_WINDOW_NORMAL);
	imshow("thresholded", thresholded);
}

void solve::balls(const Mat &input){

	//gray value
	Mat gray = im::grayscale(input);
	namedWindow("1 - balls - gray", CV_WINDOW_NORMAL);
	imshow("1 - balls - gray", gray);

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

	//calculate derivative: x+, x-, y+, y-
	Mat derivative = im::filter(gray, kernelX1, divide_fact) +
			im::filter(gray, kernelX2, divide_fact) +
			im::filter(gray, kernelY1, divide_fact) +
			im::filter(gray, kernelY2, divide_fact);
	namedWindow("2 - balls - derivative", CV_WINDOW_NORMAL);
	imshow("2 - balls - derivative", derivative);

	//blur the image with a gaussian filter (size 15, sigma 3), multiply with 255
	Mat gauss = im::gaussianFilter(derivative, 15, 3) * 255;
	namedWindow("3 - balls - gauss * 255", CV_WINDOW_NORMAL);
	imshow("3 - balls - gauss * 255", gauss);

	//invert image for further processing
	Mat inverted = im::invertGray(gauss);
	namedWindow("4 - balls - inverted", CV_WINDOW_NORMAL);
	imshow("4 - balls - inverted", inverted);

	//eroded 15 times
	Mat eroded;
	inverted.copyTo(eroded);
	for (int i =0; i< 15; i++){
		 eroded = im::morphErode(eroded);
	}
	namedWindow("5 - balls - eroded", CV_WINDOW_NORMAL);
	imshow("5 - balls - eroded", eroded);

	//thresholded at 240
	Mat thresholded = im::threshold(eroded, 240 );
	namedWindow("6 - balls - thresholded", CV_WINDOW_NORMAL);
	imshow("6 - balls - thresholded", thresholded);

	//dilated the image 22 times
	Mat dilated;
	thresholded.copyTo(dilated);
	for (int i =0; i<22; i++){
		 dilated = im::morphDilate(dilated);
	}
	namedWindow("7 - balls - dilated", CV_WINDOW_NORMAL);
	imshow("7 - balls - dilated", dilated);

	//label
	Mat labeled = im::binaryLabel(dilated);
	namedWindow("8 - balls - labeled", CV_WINDOW_NORMAL);
	imshow("8 - balls - labeled", labeled);
}

void solve::cheese(const Mat &input){

	//to grayscale
	Mat gray = im::grayscale(input);
	namedWindow("1 - cheese - grayscale", CV_WINDOW_NORMAL);
	imshow("1 - cheese - grayscale", gray);

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
	namedWindow("2 - cheese - gauss filtered", CV_WINDOW_NORMAL);
	imshow("2 - cheese - gauss filtered", filtered);

	//histogram
	Mat histogram = im::showHist(filtered);
	namedWindow("(2) - cheese - histogram", CV_WINDOW_NORMAL);
	imshow("(2) - cheese - histogram", histogram);

	//thresholding
	Mat threshold1 = im::threshold(filtered, 190);
	namedWindow("3 - cheese - thresholded 1", CV_WINDOW_NORMAL);
	imshow("3 - cheese - thresholded 1", threshold1);

	//closing
	Mat element = Mat::ones(5, 5, CV_8UC1);
	Mat closed = im::morphErode(threshold1, element);
	namedWindow("4 - cheese - closed", CV_WINDOW_NORMAL);
	imshow("4 - cheese - closed", closed);

	//inverse
	Mat inverse = im::invertGray(closed);
	namedWindow("5 - cheese - inverse", CV_WINDOW_NORMAL);
	imshow("5 - cheese - inverse", inverse);

	//labeling
	Mat labels = im::binaryLabel(inverse);
	namedWindow("6 - cheese - label", CV_WINDOW_NORMAL);
	imshow("6 - cheese - label", labels);

	//delete border objects
	Mat label_cleaned = im::deleteBorderObjects(labels);
	namedWindow("7 - cheese - label without border objects", CV_WINDOW_NORMAL);
	imshow("7 - cheese - label without border objects", label_cleaned);

}

void solve::boltsnuts(const Mat &input){

	Mat gray = im::grayscale(input);
	gray = im::averageFilter(gray, 5, 5, im::PWHITE);
	namedWindow("1 - boltsnuts - gray", CV_WINDOW_NORMAL);
	imshow("1 - boltsnuts - gray", gray);

	//segment image at 245
	Mat segmented = im::threshold(gray, 245 );
	namedWindow("2 - boltsnuts - segmented", CV_WINDOW_NORMAL);
	imshow("2 - boltsnuts - segmented", segmented);

	//obtain the background, geodesic dilated from the border
	Mat expandedBorder = Mat::zeros(segmented.size(), CV_8UC1);
	expandedBorder.row(0) = 255;
	expandedBorder.col(0) = 255;
	expandedBorder.row(segmented.rows-1) = 255;
	expandedBorder.col(segmented.cols-1) = 255;
	expandedBorder = im::morphGeodesicDilate(expandedBorder, segmented);
	namedWindow("3a - boltsnuts - expandedBorder", CV_WINDOW_NORMAL);
	imshow("3a - boltsnuts - expandedBorder", expandedBorder);

	//remove background: keep enclosed areas
	Mat objects = im::subtractMatrix(segmented, expandedBorder);
	namedWindow("3 - boltsnuts - objects: border removed", CV_WINDOW_NORMAL);
	imshow("3 - boltsnuts - objects: border removed", objects);

	//erode the objects to keep only (markers of) the largest ones
	Mat openedCircles = im::morphErode(objects);
	openedCircles = im::morphErode(openedCircles);
	openedCircles = im::morphErode(openedCircles);
	openedCircles = im::morphErode(openedCircles);
	namedWindow("4 - boltsnuts - openedCircles", CV_WINDOW_NORMAL);
	imshow("4 - boltsnuts - openedCircles", openedCircles);

	//geodesic dilation of the probable circles
	Mat bigObjects = im::morphGeodesicDilate(openedCircles, objects);
	namedWindow("5 - boltsnuts - bigObjects", CV_WINDOW_NORMAL);
	imshow("5 - boltsnuts - bigObjects", bigObjects);

	//label the circles
	Mat labeledCircles = im::binaryLabelCircle(bigObjects);
	namedWindow("6 - boltsnuts - labeledCircles", CV_WINDOW_NORMAL);
	imshow("6 - boltsnuts - labeledCircles", labeledCircles);

}

void solve::road(const Mat &input){

	//grayscale
	Mat gray = im::grayscale(input);
	namedWindow("1 - road - gray", CV_WINDOW_NORMAL);
	imshow("1 - road - gray", gray);

	//gaussian smoothing filter
	Mat smooth = im::gaussianFilter(gray, 3, 1);
	namedWindow("2 - road - smooth", CV_WINDOW_NORMAL);
	imshow("2 - road - smooth", smooth);

	//quantization with 8 levels
	Mat quant = im::quantization(smooth, 8);
	namedWindow("3 - road - quantization", CV_WINDOW_NORMAL);
	imshow("3 - road - quantization", quant);

	//histogram of quantization
	Mat histogram = im::showHist(quant);
	namedWindow("3a - road - histogram quant", CV_WINDOW_NORMAL);
	imshow("3a - road - histogram quant", histogram);

	//histogram equalize
	Mat eq = im::equalize(quant);
	namedWindow("4 - road - equalized", CV_WINDOW_NORMAL);
	imshow("4 - road - equalized", eq);

	//histogram to check difference
	Mat histogram2 = im::showHist(eq);
	namedWindow("4a - road - histogram eq-quant", CV_WINDOW_NORMAL);
	imshow("4a - road - histogram eq-quant", histogram2);

	//thresholding
	Mat threshold = im::threshold(eq, 225);
	namedWindow("5 - road - thresholded", CV_WINDOW_NORMAL);
	imshow("5 - road - thresholded", threshold);

	//dilation 3x
	Mat dilate;
	threshold.copyTo(dilate);
	for(int i =0; i < 3; i++){
		dilate = im::morphDilate(dilate);
	}
	namedWindow("6 - road - dilate", CV_WINDOW_NORMAL);
	imshow("6 - road - dilate", dilate);

	//inverse
	Mat invert = im::invertGray(dilate);
	namedWindow("7 - road - inverse of dilation", CV_WINDOW_NORMAL);
	imshow("7 - road - inverse of dilation", invert);

	//making a mask for geodesic dilate
	Mat mask = Mat::zeros(invert.rows, invert.cols, CV_8UC1);
	mask.row(0) = 255;
	mask.col(0) = 255;
	mask.row(mask.rows - 1) = 255;
	mask.col(mask.cols - 1) = 255;

	//geodesic dilate
	Mat geoDil = im::morphGeodesicDilate(mask, dilate);
	namedWindow("8 - road - Geo dilate", CV_WINDOW_NORMAL);
	imshow("8 - road - Geo dilate", geoDil);

}

void solve::xray(const cv::Mat &input){

	//to grayscale
	Mat gray = im::grayscale(input);
	namedWindow("1 - xray - grayscale", CV_WINDOW_NORMAL);
	imshow("1 - xray - grayscale", gray);

	//threshold the image at 100 and invert
	Mat thresholded = im::threshold(gray, 100);
	namedWindow("2a - xray - thresholded original", CV_WINDOW_NORMAL);
	imshow("2a - xray - thresholded original", thresholded);

	//inverse threshold
	Mat inverse_thresholded = im::invertGray(thresholded);
	namedWindow("2b - xray - inverse thresholded", CV_WINDOW_NORMAL);
	imshow("2b - xray - inverse thresholded", inverse_thresholded);

	//take inverse from original gray image
	Mat inverse = im::invertGray(gray);
	namedWindow("3a - xray - inverse grayscale", CV_WINDOW_NORMAL);
	imshow("3a - xray - inverse grayscale", inverse);

	//create a mask of the border
	Mat borderMask = Mat::zeros(gray.rows, gray.cols, CV_8UC1);
	borderMask.row(0) = 255;
	borderMask.col(0) = 255;
	borderMask.row(borderMask.rows - 1) = 255;
	borderMask.col(borderMask.cols - 1) = 255;

	//geodesic dilate the border with the inverse of the original
	Mat expandedBorder = im::morphGeodesicDilate(borderMask, inverse);
	namedWindow("3b - xray - geodesic dilate", CV_WINDOW_NORMAL);
	imshow("3b - xray - geodesic dilate", expandedBorder);

	//subtract tresholded from the expandedborder
	Mat edge = im::subtractMatrix(inverse_thresholded, expandedBorder);
	namedWindow("4 - xray - edge", CV_WINDOW_NORMAL);
	imshow("4 - xray - edge", edge);


	//thresholded the edges
	Mat silhoutte = im::threshold(edge, 35);
	namedWindow("5 - xray - silhoutte", CV_WINDOW_NORMAL);
	imshow("5 - xray - silhoutte", silhoutte);

	//label the objects
	Mat label = im::binaryLabel(silhoutte);
	namedWindow("6 - xray - label", CV_WINDOW_NORMAL);
	imshow("6 - xray - label", label);

}
