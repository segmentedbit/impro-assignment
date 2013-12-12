/*
 * stockpile.cc
 *
 *  Created on: Dec 11, 2013
 *      Author: fenrir
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



/* Thresholds an image. The pixels of value <threshold> and higher are converted
 * to 1, values below are converted to zero, as by convention (is this true?)

cv::Mat threshold(const cv::Mat &input, int threshold);
*/
