/*
 * filters.cc
 *
 *  Created on: Dec 29, 2013
 *      Author: ardillo
 */

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"	// TODO only for debugging
#include "includes/filters.h"
#include <vector>
#include <iostream>
using namespace cv;

Mat im::averageFilter(const cv::Mat &input, int width, int height, bool debug) {
	Mat output;
	if (width & 0x1 && height & 0x1){ 						//binaire check of iets oneven is
		int pixels = width * height;

		Mat kernel(height, width, CV_32FC1);				//size of kernel made with input,
		kernel = 1.0/pixels;											//value of each pixel, for making 1 together.

		int horizontal_padding = round(width/2);		//make padding var
		int vertical_padding = round(height/2);
																				// temp matrix with extra padding
		Mat temp( input.rows+(vertical_padding*2), input.cols+(horizontal_padding*2), CV_32FC1);

		for (int i = 0; i < temp.rows; i++){
			if (i < vertical_padding || i > (input.rows+vertical_padding-1)){				//adding padding in vertical way
				temp.row(i) = 0.0f;
			}
			else {
				for (int j = 0; j < temp.cols; j++){
					if ( j < horizontal_padding || j > (input.cols+horizontal_padding-1) ) {		//adding padding in horizontal way
						temp.col(j) = 0.0f;
					}
					else {													//write image, in floating point notation, on the other pixels.
						temp.at<float>(i,j) = float(input.at<uchar>(i - vertical_padding , j - horizontal_padding)/255.0);
					}
				}
			}
		}
		if (debug) {
			std::cout << kernel << "\nrounded pixel value: " << float(1.0/pixels) << "\n\n" <<
					"half padding width: " << round(width/2) <<
					" half padding length: " << round(height/2) << "\n" <<
					"input matrix width / height: " << input.cols << " " << input.rows << "\n" <<
					"temp matrix width / height: " << temp.cols << " " << temp.rows << "\n\n" <<
					temp <<
					std::endl;
					namedWindow("debug temp", CV_WINDOW_NORMAL);
					imshow("debug temp" , temp);
		}
	}
	else {
		std::cout << "width and/or height have to be odd, in order to make a correct matrix" << std::endl;
	}
	return output;
}
