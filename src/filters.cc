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

	Mat floatingInput = im::matUcharToFloat(input);
	float max = im::maxFloatValue(floatingInput);
	max += 0.000001;
	float min = im::minFloatValue(floatingInput);

	//calculate space to divide
	float space = max - min;

	//determine segment size
	float segment = space / levels;

	//determine segment value offset
	float segment_value_offset = segment / 2;

	// make variable float array
	float *segment_borders;
	segment_borders = new float[levels];

	//calculate every border of each segment
	int count = 0;
	for (float q = min; q <= max; q+=segment){
		segment_borders[count] = q;
		count++;
	}

	if(config::DEBUG){
		cout << "Quantization debug info: " << endl
				<< "\tmin float value: " << min << endl
				<< "\tmax float value: " << max << endl
				<< "\tspace to divide: " << space << endl
				<< "\tsegment size: " << segment << endl
				<< "\tsegment count: " << count << endl
				<< "\tsegment borders and values below:" << endl << "\t\t";
				for (int ii = 0; ii < count; ii++){
					cout << segment_borders[ii] << "\t\t\t";
				}
				cout << endl << "\t\t\t\t";
				for (int ii = 0; ii < (count - 1); ii++){
					cout << segment_borders[ii] + segment_value_offset << "\t\t\t";
				}
				cout << endl<< endl;
	}

	// loop through every pixel in floating input matrix
	for ( int i = 0; i < (input.rows); i++) {
		for (int j = 0; j < (input.cols); j++) {
			// get floating point pixel
			float pixel = floatingInput.at<float>(i,j);
			// loop through all segment borders -1
			for (int k = 1; k <= count; k++){
				/* if pixel value is higher or eq then the previous border level and
				 * lower then the current border level then write corresponding
				 * middle value of previous segment to this pixel.
				 * Especially written this way to prevent overflow.
				 */
				if (pixel >= segment_borders[k - 1] && pixel < segment_borders[k]) {
					pixel = segment_borders[k - 1] + segment_value_offset;
				}
			}
			// write new quantized pixel value to matrix
			floatingInput.at<float>(i,j) = pixel;
	}	}

	// converts floating point matrix to uchar output matrix
	Mat output(input.rows, input.cols, CV_8UC1);
	output = im::matFloatToUchar(floatingInput);

	im::displayPixels(output, false,false,im::DISPLAY_MATRIX);
	return output;
}
