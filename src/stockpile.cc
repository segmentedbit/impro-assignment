/*
 * stockpile.cc
 *
 *  Created on: Dec 11, 2013
 *      Author: fenrir / Ardillo
 */

#include "opencv2/imgproc/imgproc.hpp"
#include "includes/stockpile.h"
#include <vector>
#include <iostream>
using namespace cv;



Mat im::invert(const cv::Mat &input) {
	Mat inverted;
	input.copyTo(inverted);
	MatIterator_<u_char> it = inverted.begin<u_char>();
	MatIterator_<u_char> it_end = inverted.end<u_char>();
	int i = 0;
	for(; it != it_end; ++it) {

		*it = (u_char)140;
	}
	std::cout << i;

	return inverted;
}

<<<<<<< HEAD
=======
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
	for (int i = 0; i < input.rows; i++){
			for (int j = 0; j < input.cols; j++){
				float B = 0.1140 * input.at<Vec3b>(i,j)[0];
				float G = 0.5870 * input.at<Vec3b>(i,j)[1];
				float R = 0.2989 * input.at<Vec3b>(i,j)[2];
				int grey = round((R + G + B));
				grayImage.at<uchar>(i,j) = grey;
			}
	}
	return grayImage;
}

Mat im::addMatrix(const cv::Mat &original, const cv::Mat &input) {
	Mat outcome(original.rows, original.cols, CV_8UC1);
	if(original.rows == input.rows && original.cols == input.cols){        // Size check
		for (int i = 0; i < original.rows; i++){
						for (int j = 0; j < original.cols; j++){
							int originalPixel = original.at<uchar>(i,j);
							int inputPixel = input.at<uchar>(i,j);
							int value;
								if (originalPixel + inputPixel > 255){     // Prevent spin through count
									value = 255;
								}
								else {
									 value = originalPixel + inputPixel;
								}
							outcome.at<uchar>(i,j) = value;
						}
			}
	} else {
		std::cout << "Matrices do not have the same size" << std::endl;
	}
		return outcome;
}

Mat im::substractMatrix(const cv::Mat &original, const cv::Mat &input) {
	Mat outcome(original.rows, original.cols, CV_8UC1);
	if(original.rows == input.rows && original.cols == input.cols){        // Size check
		for (int i = 0; i < original.rows; i++){
						for (int j = 0; j < original.cols; j++){
							int originalPixel = original.at<uchar>(i,j);
							int inputPixel = input.at<uchar>(i,j);
							int value;
								if (originalPixel - inputPixel < 0){     // Prevent spin through count
									value = 0;
								}
								else {
									 value = originalPixel - inputPixel;
								}
							outcome.at<uchar>(i,j) = value;
						}
			}
	} else {
		std::cout << "Matrices do not have the same size" << std::endl;
	}
		return outcome;
}

long int im::getTime() {
	struct timespec get_time;
	clock_gettime(CLOCK_REALTIME, &get_time);
	long int time = get_time.tv_nsec;
	return time;
}

>>>>>>> e702741f8a1a1f2f6846dcf8b639bae4e0c99c22
