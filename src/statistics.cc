/*
 * statistics.cc
 *
 *  Created on: Dec 11, 2013
 *      Author: segmentedbit & Ardillo
 */

#include <iostream>
#include <string>
#include <sstream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "includes/statistics.h"
#include "includes/morphology.h"
#include "includes/config.h"
#include "includes/extra.h"

using namespace cv;
using namespace std;

Mat im::showHist(const cv::Mat &input) {
	Mat histogram = Mat::zeros(256, 256, CV_8UC3);
	int bins[256] = {};
	int max = 0;
	/* loop every value and increment the associating bin & keep count of the maximum. */
	for (int i = 0; i < input.rows; i++){
		for (int j = 0; j < input.cols; j++){
			int value = input.at<uchar>(i,j);
			bins[value]++;
			if (bins[value] > max ){
				max = bins[value];
			}
	}	}
	// print every bin value in the console followed by the found maximum
	if (config::DEBUG) {
		cout << "Bins" << endl;
		for (int ii = 0; ii < 256; ii++){
			cout <<bins[ii] << ", ";
		}
		cout << endl << "Max: " << max << endl << endl;
	}

	/* divide every bin-value so it will fit in the hist. matrix */
	for (int i = 0; i < 256; i++){
		int bin_value = im::round((bins[i]*255.0) / max);
		for (int j = (histogram.rows-1); j >=(histogram.rows-1) - bin_value; j--) {
			histogram.at<Vec3b>(j, i)[1] = 255;
	}	}
	return histogram;
}

Mat im::equalize(const cv::Mat &input) {
	Mat output(input.rows, input.cols, CV_8UC1);

	// check for used matrix data type, only CV_8U is currently supported.
	if (input.depth() != CV_8U){
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
		remapper[x] = im::round(static_cast<float>(cummulative) / alpha) - 1;
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
			cout << endl << endl << "Matrix grey-scale =\n " << input << endl << endl;
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

Mat im::binaryLabel(const cv::Mat &input){

	Mat temp;
	input.copyTo(temp);
	int count_value = 1;
	int objCount = 0;
	for (int i = 0; i < input.rows ; i++){
		for (int j = 0; j < input.cols ; j++){
			if (temp.at<uchar>(i,j) == 255){
				Mat marker = Mat::zeros(input.rows, input.cols, CV_8UC1);
				marker.at<uchar>(i,j) = 255;
				Mat object = im::morphGeodesicDilate(marker, input);

				for ( int x =0; x < object.rows; x++){
					for (int y = 0; y < object.rows; y++){
						if (object.at<uchar>(x,y) == 255 ){
							temp.at<uchar>(x, y) = count_value;
						}
				}	}

				count_value += 1;
				objCount++;

				if (config::DEBUG){
					std::ostringstream count;
					count << "object: " << objCount;
					namedWindow(count.str(), CV_WINDOW_NORMAL);
					imshow(count.str(), object);
				}
			}
	} }

	for (int i = 0; i < input.rows ; i++){
		for (int j = 0; j < input.cols ; j++){
			int pxl = temp.at<uchar>(i,j);
			temp.at<uchar>(i,j) = (pxl * 255) / objCount;
	}	}

	return temp;
}

float im::maxFloatValue(const cv::Mat &input){
	float max = 0;
	float value = 0;

	for (int i = 0; i < input.rows; i++){
		for (int j = 0; j < input.cols; j++){
			value = input.at<float>(i,j);
			if (value > max) {
				max = value;
			}
	}	}
	return max;
}

float im::minFloatValue(const cv::Mat &input) {
	float min = 0;
	float value = 0;

	for (int i = 0; i < input.rows; i++){
		for (int j = 0; j < input.cols; j++){
			value = input.at<float>(i,j);
			if (value < min) {
				min = value;
			}
	}	}
	return min;
}


Mat im::binaryLabelCircle(const cv::Mat &input){

	Mat temp;
	input.copyTo(temp);
	int count_value = 1;
	int objCount = 0;
	for (int i = 0; i < input.rows ; i++){
		for (int j = 0; j < input.cols ; j++){
			if (temp.at<uchar>(i,j) == 255){
				Mat marker = Mat::zeros(input.rows, input.cols, CV_8UC1);
				marker.at<uchar>(i,j) = 255;
				Mat object = im::morphGeodesicDilate(marker, input);

				Mat unused;
				struct im::boundaryStruct s = im::boundary(object, unused);


				float compactness = float(s.objectPixels) / float(s.perimiterLength);

				if (config::DEBUG) {
					cout << "element " << count_value << " has values:" << endl <<
							"perimeter length: " << s.perimiterLength << endl <<
							"object pixels: " << s.objectPixels << endl <<
							"total: " << (compactness) << endl << endl;
				}

				if (compactness < 4.0L) {
					if (config::DEBUG) {
						cout << "\tthrowing out "  << compactness << endl;
					}

					for ( int x =0; x < object.rows; x++){
						for (int y = 0; y < object.rows; y++){
							if (object.at<uchar>(x,y) == 255 ){
								temp.at<uchar>(x, y) = 0;
							}
						}
					}
				} else {
					if (config::DEBUG) {
						cerr << "\tkeeping " << compactness << endl;
					}

					for ( int x =0; x < object.rows; x++){
						for (int y = 0; y < object.rows; y++){
							if (object.at<uchar>(x,y) == 255 ){
								temp.at<uchar>(x, y) = count_value;
							}
						}
					}
				}
				count_value += 1;
				objCount++;

				if (config::DEBUG){
					std::ostringstream count;
					count << "object: " << objCount;
				}
			}
		}
	}

	for (int i = 0; i < input.rows ; i++){
		for (int j = 0; j < input.cols ; j++){
			int pxl = temp.at<uchar>(i,j);
			temp.at<uchar>(i,j) = (pxl * 255) / objCount;
	}	}

	return temp;
}

