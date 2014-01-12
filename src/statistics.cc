/*
 * statistics.cc
 *
 *  Created on: Dec 11, 2013
 *      Author: segmented bit / Ardillo
 */

#include "opencv2/imgproc/imgproc.hpp"
#include "includes/statistics.h"
#include <iostream>

using namespace cv;
using namespace std;

Mat im::showHist(const cv::Mat &input) {
	Mat histogram = Mat::zeros(256, 256, CV_8UC3);
	int bins[256] = {};
	int max;
	/* loop every value and increment the associating bin & keep count of the maximum. */
	for (int i = 0; i < input.rows; i++){
		for (int j = 0; j < input.cols; j++){
			int value = input.at<uchar>(i,j);
			bins[value]++;
			if (bins[value] > max ){
				max = bins[value];
			}
	}	}
	/* divide every bin-value so it will fit in the hist. matrix */
	for (int i = 0; i < 256; i++){
		int bin_value = round( (bins[i]*255 / max));
		for (int j = (histogram.rows-1); j >=(histogram.rows-1) - bin_value; j--) {
			histogram.at<Vec3b>(j, i)[1] = 255;
	}	}
	return histogram;
}

Mat im::equalize(const cv::Mat &input) {
	Mat output(input.rows, input.cols, CV_8UC1);

	// check for used matrix data type, only CV_8U is currently supported.
	if (!(input.depth() == 0)){
		cout << "At the moment only images with values 0...255 are supported\n"
				"Your image does not have the correct matrix type." << endl;
		exit(1);
	}

	// Copied from histogram function
	// Calculate every bin value
	int bins[256] = {};
	for (int i = 0; i < input.rows; i++){
		for (int j = 0; j < input.cols; j++){
			int value = input.at<uchar>(i,j);
			bins[value]++;
	}	}

	// remapping algorithm, see page 60 of reader
	// make a remapping array to refactor every found graylevel to
	float alpha = (input.rows * input.cols) / 256;
	int remapper[256];
	int cummulative = 0;
	for (int x = 0; x < 256; x++){
		cummulative += bins[x];
		remapper[x] = round(cummulative / alpha) - 1;
		if (remapper[x] < 0){
			remapper[x] = 0;
		}
	}

	// loop again through the image and remaps every value
	for (int i = 0; i < input.rows; i++){
			for (int j = 0; j < input.cols; j++){
				int value = input.at<uchar>(i,j);
				output.at<uchar>(i,j) = remapper[value];
		}	}

	return output;
}

void im::displayPixels(const cv::Mat &input, bool Color, bool debug, const int dType){
	cout << " in function ptr : " << &input << endl;

	// print all pixels in console, as a matrix
	if (dType == im::DISPLAY_BOTH || dType == im::DISPLAY_MATRIX) {
		if(Color){
			cout << endl << endl << "Matrix color (BGR notation !!!) = " << endl << input << endl << endl;
		}
		else{
			cout << endl << endl << "Matrix grey-scale = " << endl << input << endl << endl;
		}
	}

	// Print the pixel in the console, list-wise
	if (dType == im::DISPLAY_BOTH || dType == im::DISPLAY_LIST) {
		int count = 0;

		cout << endl << " Pixel iteration -->" << endl <<endl;

		// loop through all pixels, row by row.
		for (int i = 0; i < input.rows; i++){
			for (int j = 0; j < input.cols; j++){
				if (Color){
					int B = input.at<Vec3b>(i,j)[0];
					int G = input.at<Vec3b>(i,j)[1];
					int R = input.at<Vec3b>(i,j)[2];
					cout << count << ")  pixel at (" << i << "," << j << ") =" << " R:" << R << " G:" << G << " B:" << B ;
				}
				else{
					int grayValue = input.at<uchar>(i,j);
					cout << count << ")  pixel at (" << i << "," << j << ") =" << " value:" << grayValue ;
				}
				if (debug){
					//no idea what this does, Point2f? unsignedChar?
					Point2f point = input.at<Point2f>(i,j);
					uchar value = input.at<uchar>(i,j);
					cout << count << " --> DEBUG INFO = point:"<< point << "  value:" << value << endl;
				}
				else{
					cout << endl;
				}
				count++;
			}
		}
	}
}
