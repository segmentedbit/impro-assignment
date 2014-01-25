/*
 * filters.cc
 *
 *  Created on: Dec 29, 2013
 *      Author: Ardillo & segmentedbit
 */

#include <vector>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "includes/filters.h"
#include "includes/stockpile.h"
#include "includes/statistics.h"
#include "includes/extra.h"
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
	int hPadding = kWidth/2;
	int vPadding = kHeight/2;
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
			output.at<uchar>(i,j) = im::round(static_cast<float>(middlePixel) / kPixels);

		}
	}

	return output;
}

Mat im::medianFilter(const Mat& input, const int rows, const int cols) {
	Mat output = Mat::zeros(input.size(), CV_8UC1);
	Mat temp = input.clone();
	const int rows_pad = rows / 2;
	const int cols_pad = cols / 2;

	for (int y=0; y<input.rows; y++) {
		for (int x=0; x<input.cols; x++) {

			vector<uchar> values;

			for (int wy=-rows_pad; wy<rows_pad; wy++) {
				for (int wx=-cols_pad; wx<cols_pad; wx++) {
					if (y+wy >= 0 && x+wx >= 0 && (y+wy) < input.rows && (x+wx) < input.cols) {
						values.push_back(temp.at<uchar>(y+wy,x+wx));
					}
				} //end window wx
			} // end window wy

			sort(values.begin(), values.end());

			if (values.size() % 2 == 0) {
				output.at<uchar>(y,x) = (values[values.size() / 2 - 1] + values[values.size() / 2]) / 2;
			} else {
				output.at<uchar>(y,x) = values[values.size() / 2];
			}

			if(config::DEBUG){
				cout << values.size() << " value is " << int(values[values.size() / 2]) << " " << endl;
			}

		} // end image x
	} // end image y
	return output;
}

Mat im::gaussianFilter(const Mat& input, const int size, const int sigma) {
	if (size % 2 == 0) {
		cerr << "gaussianFilter: size is even, need uneven size: (" << size << "). Exiting" << endl;
		exit(1);
	}
	int padding = size / 2;
	Mat gaussian = im::gaussianKernel(size, sigma);
	Mat temp = im::copyWithPadding(input, padding, padding, im::PREPLICATE);
	Mat output = Mat::zeros(input.size(), CV_8UC1);

	for (int y=0; y<input.rows; y++) {
		for (int x=0; x<input.cols; x++) {
			float new_value = 0;
			for (int yy=0; yy<gaussian.rows; yy++) {
				for (int xx=0; xx<gaussian.cols; xx++) {
					new_value += static_cast<float>(temp.at<uchar>(y+yy, x+xx)) * gaussian.at<float>(yy, xx);
				}
			}
			//cout << new_value << endl;
			output.at<uchar>(y, x) = new_value;
		}
	}
	return output;
}

Mat im::filter(const cv::Mat &input, const cv::Mat &kernel, const float divide_factor) {
	int status = validateKernel(kernel, im::UNEVEN);
	if (status != 0) {
		cerr << "im::filter kernel element does not validate" << endl;
		exit(1);
	}
	int pWidth = kernel.cols/2;
	int pHeight = kernel.rows/2;

	Mat temp;

	// Copy matrix and convert it to Floating point notation
	temp = im::copyWithPadding(input, pWidth, pHeight, PZERO);
	Mat temp_float = im::matUcharToFloat(temp);

	if (config::DEBUG) {
		cout << "kernel: " << endl << kernel << endl << endl;
		cout << "matrix with padding: " << endl << temp_float << endl << endl;
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

/*
 * Converts an image to floating point notation.
 * Then searches the max and min value of the image, using this info it
 * will calculate the pixel delta and divide them into equal spaces.
 * It then loops over the floating point matrix and gives a pixel the corresponding
 * middle space value of the space it belongs to.
 * After that it converts the floating point matrix back to uchar matrix, which acts as
 * the output image.
 */
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
	return output;
}

Mat im::localMinimumOfMaximum(const cv::Mat &input, const int window_width, const int window_height){
	// Check parameter values
	if (window_width < 0 || window_height < 0) {
		cerr << "localMinOfMax: input parameter window_width or window_height is  negative)" << endl;
		exit(1);
	}
	// Check if and break when kernel width or height is even
	if (!(window_width % 2 == 1 && window_height % 2 == 1)){
		cerr << "localMinOfMax: width and/or height have to be odd, in order to make a correct matrix" << endl;
		exit(1);
	}

	int pWidth = window_width/2;
	int pHeight = window_height/2;

    Mat temp = im::copyWithPadding(input, pHeight, pWidth, PREPLICATE);
	Mat max(input.rows, input.cols, CV_8UC1);

	// loop through matrix
	for ( int i = 0; i < input.rows; i++) {
		for (int j = 0; j < input.cols; j++) {
			int localMax = 0;

			//loop through kernel
			for (int x=0; x<window_width; x++) {
				for (int y=0; y<window_height; y++) {

					//determine if value is higher -> Max value
					int localValue = temp.at<uchar>(i+x, j+y);
						if (localValue > localMax){
							localMax = localValue;
						}
			}	}
			max.at<uchar>(i,j) = localMax;
	}	}

	if(config::DEBUG){
		namedWindow("local max", CV_WINDOW_NORMAL);
		imshow("local max", max);
	}

	temp = im::copyWithPadding(max, pHeight, pWidth, PREPLICATE);
	Mat minOfMax(input.rows, input.cols, CV_8UC1);

	// loop through found Maximum values and select the minimum;
	for ( int i = 0; i < input.rows; i++) {
		for (int j = 0; j < input.cols; j++) {
			int localMin = 255;

			//loop through kernel
			for (int x=0; x<window_width; x++) {
				for (int y=0; y<window_height; y++) {

					//determine if value is higher -> Max value
					int localValue = temp.at<uchar>(i+x, j+y);
						if (localValue < localMin){
							localMin = localValue;
						}
			}	}
			minOfMax.at<uchar>(i,j) = localMin;
	}	}
	return minOfMax;
}

Mat im::localMaximumOfMinimum(const cv::Mat &input, const int window_width, const int window_height){
	// Check parameter values
	if (window_width < 0 || window_height < 0) {
		cerr << "localMaxOfMin: input parameter window_width or window_height is negative)" << endl;
		exit(1);
	}
	// Check if and break when kernel width or height is even
	if (!(window_width % 2 == 1 && window_height % 2 == 1)){
		cerr << "localMaxOfMin: width and/or height have to be odd, in order to make a correct matrix" << endl;
		exit(1);
	}

	int pWidth = window_width/2;
	int pHeight = window_height/2;

    Mat temp = im::copyWithPadding(input, pHeight, pWidth, PREPLICATE);
	Mat min(input.rows, input.cols, CV_8UC1);

	// loop through matrix
	for ( int i = 0; i < input.rows; i++) {
		for (int j = 0; j < input.cols; j++) {
			int localMin = 255;

			//loop through kernel
			for (int x=0; x<window_width; x++) {
				for (int y=0; y<window_height; y++) {

					//determine if value is higher -> Max value
					int localValue = temp.at<uchar>(i+x, j+y);
						if (localValue < localMin){
							localMin = localValue;
						}
			}	}
			min.at<uchar>(i,j) = localMin;
	}	}

	if(config::DEBUG){
		namedWindow("local min", CV_WINDOW_NORMAL);
		imshow("local min", min);
	}

	temp = im::copyWithPadding(min, pHeight, pWidth, PREPLICATE);
	Mat maxOfMin(input.rows, input.cols, CV_8UC1);

	// loop through found Maximum values and select the minimum;
	for ( int i = 0; i < input.rows; i++) {
		for (int j = 0; j < input.cols; j++) {
			int localMax = 0;

			//loop through kernel
			for (int x=0; x<window_width; x++) {
				for (int y=0; y<window_height; y++) {

					//determine if value is higher -> Max value
					int localValue = temp.at<uchar>(i+x, j+y);
						if (localValue > localMax){
							localMax = localValue;
						}
			}	}
			maxOfMin.at<uchar>(i,j) = localMax;
	}	}
	return maxOfMin;
}

/*
 *  Look here for explanation on the 2D isotropic gaussian formula
 *   http://homepages.inf.ed.ac.uk/rbf/HIPR2/gsmooth.htm
 */
Mat im::gaussianKernel(const int size, const int sigma) {
	if (size % 2 == 0) { // size is even
		cerr << "gaussianKernel: Structuring element does not validate" << endl;
		exit(1);
	}
	Mat output = Mat(size, size, CV_32FC1);
	const int amp = size / 2;

	for (int y=-amp; y<=amp; y++) {
		for (int x=-amp; x<=amp; x++) {
			output.at<float>(y+amp,x+amp) = 1 / (2*3.14*pow(sigma,2)) * exp(-( (pow(x,2)+pow(y,2)) / (2*pow(sigma,2)) ) );
		}
	}
	return output;
}

