/*
 * morphology.cc
 *
 *  Created on: Jan 10, 2014
 *      Author: segmentedbit
 */

#include <opencv2/imgproc/imgproc.hpp>
#include "includes/morphology.h"

using namespace std;
using namespace cv;

Mat im::morphDilate(const Mat &input, const Mat &kernel) { //TODO implement
	Mat output(input.size(), CV_8UC1);
	return output;
}

Mat im::morphErode(const Mat &input, const Mat &kernel) { //TODO implement
	Mat output(input.size(), CV_8UC1);
	return output;
}

Mat im::morphOpen(const Mat &input, const Mat &erosionElement, const Mat &dilationElement) { //TODO implement
	Mat output(input.size(), CV_8UC1);
	return output;
}

Mat im::morpClose(const Mat &input, const Mat &dilationElement, const Mat &erosionElement) { //TODO implement
	Mat output(input.size(), CV_8UC1);
	return output;
}

Mat im::morphSkeleton(const Mat &input) {
	Mat output(input.size(), CV_8UC1);
	return output;
}
