/*
 * filters.cc
 *
 *  Created on: Dec 29, 2013
 *      Author: ardillo
 */

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"	// TODO only for debugging
#include "includes/filters.h"
#include "includes/stockpile.h"
#include "includes/statistics.h"
#include <vector>
#include <iostream>
#include "config.h"

using namespace std;
using namespace cv;

Mat im::averageFilter(const cv::Mat &input, int kWidth, int kHeight, const int pType) {
	// Check parameter values
	if (kWidth < 0 || kHeight < 0) {
		cerr << "averageFilter: input parameter kWidth or kHeight is negative)" << endl;
		exit(1);
	}

	// Check if and break when kernel width or height is even
	if (!(kWidth % 2 == 1 && kHeight % 2 == 1)){
		cerr << "averageFilter: width and/or height have to be odd, in order to make a correct matrix" << endl;
		exit(1);
	}

	Mat output(input.rows, input.cols, CV_8UC1);

	int kPixels = kWidth * kHeight;
	int hPadding = round(kWidth/2);
	int vPadding = round(kHeight/2);
	if (config::DEBUG) {
		cout << "averageFilter debug information:" << endl <<
				"\tkWidth: " << kWidth << endl <<
				"\tkHeight: " << kHeight << endl <<
				"\tkPixels: " << kPixels << endl <<
				"\thPadding: " << hPadding << endl <<
				"\tvPadding: " << vPadding << endl << endl;
	}

	Mat temp = im::copyWithPadding(input, hPadding, vPadding, pType);

	// temp. matrix is made, kernel is made, now calculate matrix multiply.
	for ( int i = 0; i < input.rows; i++) {
		for (int j = 0; j < input.cols; j++) {
			int middlePixel = 0;

			for (int x=0; x<kWidth; x++) {
				for (int y=0; y<kHeight; y++) {
					middlePixel += temp.at<uchar>(i+x, j+y);
				}
			}
			//  calculate the final result
			output.at<uchar>(i,j) = round(static_cast<float>(middlePixel) / kPixels);

		}
	}
	if (config::DEBUG) {
		namedWindow("Debug: averageFilter return image", CV_WINDOW_NORMAL);
		imshow("Debug: averageFilter return image",output);
		waitKey(0);
	}
	return output;
}

Mat im::filter(const cv::Mat &input, const cv::Mat &kernel, const float divide_factor) {

	// TODO kernel check
	int pWidth = round(kernel.cols/2);
	int pHeight = round(kernel.rows/2);

	Mat temp;

	// Copy matrix and convert it to Floating point notation
	temp = im::copyWithPadding(input, pWidth, pHeight, PZERO);
	Mat temp_float = im::matUcharToFloat(temp);

	if (config::DEBUG) {
		cout << "kernel: " << endl << kernel << endl << endl;
		cout << "matrix with padding: " << endl << temp_float << endl << endl;
		//namedWindow("DEBUG - temp", CV_WINDOW_NORMAL);
		//imshow("DEBUG - temp", temp_float);
	}

	/*
	 *  calculate every value overlapping temporary matrix and the kernel,
	 *  place the ending result in the middle pixel of interest.
	 */
	temp = Mat::zeros(input.rows, input.cols, CV_32FC1);
	for ( int i = 0; i < (input.rows); i++) {
		for (int j = 0; j < (input.cols); j++) {
			int middlePixel = 0;
			for ( int x = 0; x < kernel.rows; x++) {
				for (int y = 0; y < kernel.cols; y ++) {
					middlePixel += temp_float.at<float>(i+x, j+y) * kernel.at<float>(x,y);
			}	}
			temp.at<float>(i,j) = middlePixel/divide_factor;
	} }


	Mat output_uchar(input.rows, input.cols, CV_8UC1);
	output_uchar = im::matFloatToUchar(temp);
	if(config::DEBUG){
		cout << "temp : " << endl << temp << endl << endl;
		cout << "output_char: " << endl << output_uchar << endl << endl;
	}

	return output_uchar;
}

Mat im::quantization(const cv::Mat &input, const int levels){
	Mat output(input.rows, input.cols, CV_8UC1);

	// make variable float array
	int *slots;
	slots = new int[levels ];

	// divide the floating space 0...1 in the amount of levels given
	int slot_size = 255 / levels;
	int count = 0;
	for (int q = 0; q <= 255; q += slot_size){
		slots[count] = q;
		++count;
	}

	// loop through every pixel in input matrix
	for ( int i = 0; i < (input.rows); i++) {
		for (int j = 0; j < (input.cols); j++) {
			// get floating point pixel
			int pixel = input.at<uchar>(i,j);
			// loop through all slots, keep track of count during this
			for (int k = 1; k <= levels; k++){
				// if pixelvalue is higher then the current slot value
				// and smaller then the previous slot value,
				// then write the corresponding slot value in output
				if (pixel >= slots[k - 1] && pixel < slots[k] ){
					output.at<uchar>(i,j) = slots[k - 1];
				}
				else if (k == levels && pixel >= slots[k]){
					output.at<uchar>(i,j) = slots[k - 1];
				}
			}
	}	}
	if(config::DEBUG){
		cout << "Quantization debug info: " << endl
				<< "\tslot size: " << slot_size << endl
				<< "\tslot count: " << count << endl
				<< "\tslots:" << endl << "\t\t";
				for (int ii = 0; ii <= levels; ii++){
					cout << slots[ii] << ", ";
				}
				cout << endl << endl;
	//	cout << "Output Matrix: " << endl
	//			<< output << endl << endl;
	}
	return output;

	/*
	float amount = 1.0 / levels;
	Mat output(input.rows, input.cols, CV_8UC1);
	Mat temp = im::matUcharToFloat(input);

	int count = 0;

	// make variable float array
	float *slots;
	slots = new float[levels];

	// divide the floating space 0...1 in the amount of levels given
	for (float q = 0; q <= 1; q += amount){
		slots[count] = q;
		count++;
	}
	// loop through every pixel in input matrix
	for ( int i = 0; i < (input.rows); i++) {
		for (int j = 0; j < (input.cols); j++) {
			// get floating point pixel
			float pixel = temp.at<float>(i,j);
			// loop through all slots, keep track of count during this
			for (int k = 0; k >= levels; k++){
				// if pixelvalue is higher then slot value,
				// then write the corresponding
				if (pixel < slots[k] ){
					temp.at<float>(i,j) = slots[k - 1];
				}
			}
	}	}
	if(config::DEBUG){
		cout << "Quantization debug info: " << endl
				<< "\tslot size: " << amount << endl
				<< "\tslot count: " << count << endl
				<< "\tslots:" << endl << "\t\t";
				for (int ii = 0; ii <= levels; ii++){
					cout << slots[ii] << ", ";
				}
				cout << endl << endl;
		cout << "temp Matrix: " << endl
				<< temp << endl << endl;
	}
	output = im::matFloatToUchar(temp);
	return output;
	*/
}
