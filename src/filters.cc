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

Mat im::averageFilter(const cv::Mat &input, int kernel_width, int kernel_height, bool debug) {
	Mat output(input.rows, input.cols, CV_8UC1);
	if (kernel_width & 0x1 && kernel_height & 0x1){ 						//binaire check of iets oneven is
		int pixels = kernel_width * kernel_height;

		Mat kernel(kernel_height, kernel_width, CV_32FC1);				//size of kernel made with input,
		kernel = 1.0/pixels;											//value of each pixel, for making 1 together. TODO easily changed in different shapes of filters.

		int horizontal_padding = round(kernel_width/2);		//make padding var TODO could be changed in other padding types.
		int vertical_padding = round(kernel_height/2);
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
					"half padding width: " << round(kernel_width/2) <<
					" half padding length: " << round(kernel_height/2) << "\n" <<
					"input matrix width / height: " << input.cols << " " << input.rows << "\n" <<
					"temp matrix width / height: " << temp.cols << " " << temp.rows << "\n\n" <<
					temp <<
					std::endl;
					namedWindow("debug temp", CV_WINDOW_NORMAL);
					imshow("debug temp" , temp);
		}

		// temp. matrix is made, kernel is made, now calculate matrix mulltiply.
		for ( int i = 0; i < (input.rows); i++) {
			for (int j = 0; j < (input.cols); j++) {
				/*
				 * 	(temp.rows - (kernel_height - 1))
				 * (temp.cols - (kernel_width - 1))
				 *
				 *  calculate every value overlapping temporary matrix and the kernel,
				 *  place the ending result in the middle pixel of interest.
				 */
				Mat result(kernel.rows, kernel.cols, CV_32FC1); //debug
				float middlePixel = 0.0; //debug

				for ( int x = 0; x < kernel.rows; x++) {
					for (int y = 0; y < kernel.cols; y ++) {
						if (debug) {
							result.at<float>(x,y)= temp.at<float>(i+x, j+y) * kernel.at<float>(x,y);
						}
						middlePixel += temp.at<float>(i+x, j+y) * kernel.at<float>(x,y);
					}
				}
				middlePixel *= 255; //from floating point notation to uchar.
				if (debug) {
					std::cout << "loc rows / cols" << i << " " << j <<  " Pointing Pixel: " << round(middlePixel) <<
							"\nresult Matrix:\n" << result << std::endl << std::endl;
				}
				//  calculate the final result
				output.at<uchar>(i,j) = round(middlePixel);
			}
		}
	}
	else {
		std::cout << "width and/or height have to be odd, in order to make a correct matrix" << std::endl;
	}
	return output;
}

//TODO does not work completely
Mat im::filter(const cv::Mat &input, const cv::Mat &kernel) {
	// kernel-size check has to be done in the process of making the kernel.
	Mat output(input.rows, input.cols, CV_8UC1);

	int horizontal_padding = round(kernel.cols/2);
	int vertical_padding = round(kernel.rows/2);
	/*
	 * First make temporary matrix with extra padding
	 * TODO could be changed in other padding types.
	 */
	Mat temp( input.rows+(vertical_padding*2), input.cols+(horizontal_padding*2), CV_8UC1);
	for (int i = 0; i < temp.rows; i++){
		if (i < vertical_padding || i > (input.rows+vertical_padding-1)){				//adding padding in vertical way
			temp.row(i) = round(0);
		}
		else {
			for (int j = 0; j < temp.cols; j++){
				if ( j < horizontal_padding || j > (input.cols+horizontal_padding-1) ) {		//adding padding in horizontal way
					temp.col(j) = round(0);
				}
				else {
					temp.at<uchar>(i,j) = input.at<uchar>(i - vertical_padding , j - horizontal_padding);
				}
			}
		}
	}
	/*
	 *  calculate every value overlapping temporary matrix and the kernel,
	 *  place the ending result in the middle pixel of interest.
	 */
	for ( int i = 0; i < (input.rows); i++) {
		for (int j = 0; j < (input.cols); j++) {
			int middlePixel = 0; //debug
			for ( int x = 0; x < kernel.rows; x++) {
				for (int y = 0; y < kernel.cols; y ++) {
					middlePixel += temp.at<uchar>(i+x, j+y) * kernel.at<uchar>(x,y);
				}
			}
			//std::cout <<"pixel: " << middlePixel <<std::endl;
			output.at<uchar>(i,j) = middlePixel; //  calculate the final result
		}
	}
	return output;
}
