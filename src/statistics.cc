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
