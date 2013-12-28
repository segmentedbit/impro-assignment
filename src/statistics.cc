/*
 * statistics.cc
 *
 *  Created on: Dec 11, 2013
 *      Author: segmented bit / Ardillo
 */

#include "opencv2/imgproc/imgproc.hpp"
#include "includes/statistics.h"
#include <cv.h>

using namespace cv;
using namespace im;

Mat im::showHist(const cv::Mat &input) {
	Mat histogram(256, 256, CV_8UC1);
	histogram = 255;
	int amount_of_pixels = input.rows * input.cols;
	std::cout << amount_of_pixels << std::endl;
	int bins[256] = {};
	int max;
	//count every value and increment the associating bin
	for (int i = 0; i < input.rows; i++){
		for (int j = 0; j < input.cols; j++){
			int value = input.at<uchar>(i,j);
			bins[value]++;
			if (bins[value] > max ){
				max = bins[value];
			}
	}	}
	//TODO stopped here... some vague auto zoom system
	/*
	 * it checks by modulus calculating the biggest round number
	 * 10, 100 , 100 .... etc etc
	 * This variable named 'iterate' is used to scale the bin-values
	 * in the next for loop.
	 */
	std::cout << "max value " << max << std::endl;
	int mystic;
	int iterate = 10;
	while (mystic != 0) {
		mystic = (max/iterate)%iterate;
		std::cout << "mystic " << mystic << std::endl;
		iterate *= 10;
	}
	iterate /= 10;
	std::cout << "iterate" << iterate << std::endl;


	//divide every bin-value so it will fit in the hist. matrix
	for (int i = 0; i < 256; i++){
		int bin_value;
		if (bins[i] == 0){
			bin_value = 0;
		}
		else {
			bin_value = abs(round( iterate * 1.0 / bins[i]));
		}
//		std::cout << i << " ) " << bin_value << std::endl;
		// draw a black pixel on every bin-location from bottom-up
		int x;
		if (((histogram.rows-1) - bin_value) < 0 ) {
			x = 0;
		} else {
			x = (histogram.rows-1) - bin_value;
		}

		for (int j = (histogram.rows-1); j >=x; j--) {
			histogram.at<uchar>(j, i) = 0;
	}	}
	return histogram;
}

void im::displayPixels(const cv::Mat &input, bool Color, bool debug ){
	std::cout << " in function ptr : " << &input << std::endl;

	int count = 0;
	// print all pixels in console for comparison
	if(Color){
		std::cout << std::endl << std::endl << "Matrix color (BGR notation !!!) = " << std::endl << input << std::endl << std::endl;
	}
	else{
		std::cout << std::endl << std::endl << "Matrix grey-scale = " << std::endl << input << std::endl << std::endl;
	}
	std::cout << std::endl << " Pixel iteration -->" << std::endl <<std::endl;

	// loop through all pixels, row by row.
	for (int i = 0; i < input.rows; i++){
		for (int j = 0; j < input.cols; j++){
			if (Color){
				int B = input.at<Vec3b>(i,j)[0];
				int G = input.at<Vec3b>(i,j)[1];
				int R = input.at<Vec3b>(i,j)[2];
				std::cout << count << ")  pixel at (" << i << "," << j << ") =" << " R:" << R << " G:" << G << " B:" << B ;
			}
			else{
				int grayValue = input.at<uchar>(i,j);
				std::cout << count << ")  pixel at (" << i << "," << j << ") =" << " value:" << grayValue ;
			}
			if (debug){
				//no idea what this does, Point2f? unsignedChar?
				Point2f point = input.at<Point2f>(i,j);
				uchar value = input.at<uchar>(i,j);
				std::cout << count << " --> DEBUG INFO = point:"<< point << "  value:" << value << std::endl;
			}
			else{
				std::cout << std::endl;
			}
		    count++;
		}
	}
}
