/*
 * stockpile.cc
 *
 *  Created on: Dec 11, 2013
 *      Author: fenrir / Ardillo
 */

#include "opencv2/imgproc/imgproc.hpp"
#include <vector>
#include <iostream>
#include "includes/stockpile.h"
#include "config.h"


using namespace std;
using namespace cv;

Mat im::segmentize(const Mat &input, int threshold) {
	Mat output = input.clone();
	for (int i = 0; i < input.rows; i++){
		for (int j = 0; j < input.cols; j++){
			int value = input.at<uchar>(i,j);
			if (value >= threshold) {
				output.at<uchar>(i,j) = 1;
			}
			else {
				output.at<uchar>(i,j) = 0;
			}
		}
	}
	return output;
}


Mat im::threshold(const cv::Mat &input, int threshold) {
	Mat binary;
	input.copyTo(binary);
	for (int i = 0; i < input.rows; i++){
		for (int j = 0; j < input.cols; j++){
			int value = input.at<uchar>(i,j);
			if (value >= threshold) {
				binary.at<uchar>(i,j) = 255;
			}
			else {
				binary.at<uchar>(i,j) = 0;
			}
		}
	}
	return binary;
}

Mat im::invertGray(const cv::Mat &input) {
	Mat inverted;
	input.copyTo(inverted);
	for (int i = 0; i < input.rows; i++){
		for (int j = 0; j < input.cols; j++){
			inverted.at<uchar>(i,j)= 255 - input.at<uchar>(i,j);
		}
	}
	return inverted;
}

Mat im::grayscale(const cv::Mat &input) {
	Mat grayImage(input.rows, input.cols, CV_8UC1);
	for (int i = 0; i < input.rows; i++) {
		for (int j = 0; j < input.cols; j++) {
			float B = 0.1140 * input.at<Vec3b>(i,j)[0];
			float G = 0.5870 * input.at<Vec3b>(i,j)[1];
			float R = 0.2989 * input.at<Vec3b>(i,j)[2];
			int grey = round((R + G + B));
			grayImage.at<uchar>(i,j) = grey;
		}
	}
	return grayImage;
}

Mat im::addMatrix(const cv::Mat &first, const cv::Mat &second) {
	// Size check; fail when first and second are not equally sized.
	if (first.rows != second.rows || first.cols != second.cols) {
		cerr << "Matrices do not have the same size" << endl;
		exit(1);
	}
	Mat outcome(first.rows, first.cols, CV_8UC1);
	for (int i = 0; i < first.rows; i++){
		for (int j = 0; j < first.cols; j++){
			int firstPixel = first.at<uchar>(i,j);
			int secondPixel = second.at<uchar>(i,j);
			int value;
				if (firstPixel + secondPixel > 255){     // Prevent spin through count
					value = 255;
				}
				else {
					 value = firstPixel + secondPixel;
				}
			outcome.at<uchar>(i,j) = value;
		}
	}
	return outcome;
}

Mat im::subtractMatrix(const cv::Mat &first, const cv::Mat &second) {
	// Size check; fail when first and second are not equally sized.
	if (first.rows != second.rows || first.cols != second.cols) {
		cerr << "Matrices do not have the same size" << endl;
		exit(1);
	}
	Mat outcome(first.rows, first.cols, CV_8UC1);
	for (int i = 0; i < first.rows; i++) {
		for (int j = 0; j < first.cols; j++) {
			int firstPixel = first.at<uchar>(i,j);
			int secondPixel = second.at<uchar>(i,j);
			int value;

			// Prevent spin through count
			if (firstPixel - secondPixel < 0){
				value = 0;
			}
			else {
				 value = firstPixel - secondPixel;
			}
			outcome.at<uchar>(i,j) = value;
		}
	}
	return outcome;
}

// Needs work on the PHREPLICATE option; corners are not yet handled.
Mat im::copyWithPadding(const Mat &original, const int hPadding, const int vPadding, const int pType) {
	int oWidth = original.size().width;
	int oHeight = original.size().height;

	try {
		Mat output(original.rows + 2*vPadding, original.cols + 2*hPadding, CV_8UC1);

		if (config::DEBUG) {
			cout << "copyWithPadding debug information:" << endl <<
					"\toWidth: " << oWidth << endl <<
					"\toHeight: " << oHeight << endl <<
					"\thPadding: " << hPadding << endl <<
					"\tvPadding: " << vPadding << endl << endl;
		}

		original.copyTo(output(Rect(cv::Point(hPadding, vPadding), original.size())));
		switch (pType) {
			case PZERO:
				for (int i=0; i<hPadding; i++) {
					output.col(i) = 0.0;
				}
				for (int i=0; i<vPadding; i++) {
					output.row(i) = 0.0;
				}
				for (int i=oWidth+hPadding; i<oWidth+2*hPadding; i++) {
					output.col(i) = 0.0;
				}
				for (int i=oHeight+vPadding; i<oHeight+2*vPadding; i++) {
					output.row(i) = 0.0;
				}
				break;
			case PWHITE:
				for (int i=0; i<hPadding; i++) {
					output.col(i) = 255;
				}
				for (int i=0; i<vPadding; i++) {
					output.row(i) = 255;
				}
				for (int i=oWidth+hPadding; i<oWidth+2*hPadding; i++) {
					output.col(i) = 255;
				}
				for (int i=oHeight+vPadding; i<oHeight+2*vPadding; i++) {
					output.row(i) = 255;
				}
				break;
			case PREPLICATE:
				for (int i=0; i<hPadding; i++) {
					output.col(i) = original.col(0);
				}
				for (int i=0; i<vPadding; i++) {
					output.row(i) = original.row(0);
				}
				for (int i=oWidth+hPadding; i<oWidth+2*hPadding; i++) {
					output.col(i) = original.col(oWidth-1);
				}
				for (int i=oWidth+vPadding; i<oHeight+2*vPadding; i++) {
					output.row(i) = original.row(oHeight-1);
				}
				//TODO corners
				break;
			default: ;

		}
		return output;
	} catch (Exception& e) {
		const char* err_msg = e.what();
		cerr << "Exception in copyWithPadding:" << endl <<
				"\toWidth: " << oWidth << endl <<
				"\toHeight: " << oHeight << endl <<
				"\thPadding: " << hPadding << endl <<
				"\tvPadding: " << vPadding << endl <<
				err_msg << "exit" << endl;
		exit(1);
	}
}

int im::validateKernel(const Mat &kernel, const int flags) {
	int status = 0;

	// Check if kernel width and height are uneven

	if(flags & im::UNEVEN) {
		if (!(kernel.rows % 2 == 1 && kernel.cols % 2 == 1)){
			if (config::DEBUG)
				cerr << "averageFilter: width and/or height have to be odd, in order to make a correct matrix" << endl;
			status += im::UNEVEN;
		}
	}
	return status;
}

bool im::equal(const Mat &first, const Mat &second) {
	// Size check; fail when first and second are not equally sized.
	if (first.rows != second.rows || first.cols != second.cols) {
		cerr << "Matrices do not have the same size" << endl;
		exit(1);
	}
	for (int i=0; i<first.rows; i++) {
		for (int j=0; j<first.cols; j++) {
			if (first.at<uchar>(i,j) != second.at<uchar>(i,j)) {
				return false;
			}
		}
	}
	return true;
}

Mat im::matUcharToFloat(const cv::Mat &input) {
	// code below is necessary,  converting does not have influence
	// pixel values
	Mat output = input;
	output.convertTo(output, CV_32F);
	for ( int i = 0; i < (output.rows); i++) {
		for (int j = 0; j < (output.cols); j++) {
			output.at<float>(i,j) = output.at<float>(i,j) / 255;
	}	}
	return output;
}

Mat im::matFloatToUchar(const cv::Mat &input) {
	// code below is necessary,  converting does not have influence
	// pixel values
	// this is done with 2 if checks, to prevent strange white spaces.
	Mat temp = input;
	Mat output(input.rows, input.cols, CV_8U);
	for ( int i = 0; i < (temp.rows); i++) {
		for (int j = 0; j < (temp.cols); j++) {
			int value = round(temp.at<float>(i,j) * 255);
			if (value > 255) { value = 255; }
			if (value < 0 ) { value = 0; }
			output.at<uchar>(i,j) = value;
	}	}
	return output;
}

long int im::getTime() {
	struct timespec get_time;
	clock_gettime(CLOCK_REALTIME, &get_time);
	long int time = get_time.tv_nsec;
	return time;
}
