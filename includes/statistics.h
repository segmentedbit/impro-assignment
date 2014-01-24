/*
 * statistics.h
 *
 *  Created on: Dec 11, 2013
 *      Author: segmentedbit & Ardillo
 */

#ifndef STATISTICS_H_
#define STATISTICS_H_

#include <opencv2/imgproc/imgproc.hpp>

// Our own namespace to be sure that our names don't cause conflicts.
namespace im {

const int DISPLAY_MATRIX = 0;
const int DISPLAY_LIST = 1;
const int DISPLAY_BOTH = 2;

const int INCREASE = 1, DECREASE = 0;

/*
 * Shows histogram of given input image. Uses a plain Mat image to display the
 * height of all the bins.
 */
cv::Mat showHist(const cv::Mat &input );

/*
 * Equalizes the given input image
 */
cv::Mat equalize(const cv::Mat &input);

/*
 * Enhances contrast on given interval. Intervals are interpreted as
 * [left, right). Mode can be either INCREASE or DECREASE.
 */
cv::Mat enhanceConstrast(const cv::Mat &input, int left, int right, int mode);

/*
 *  Displays every pixel with its value in the console, This can be used for a grey-scale image
 *  or a color image. dType defaults to DISPLAY_BOTH. Other constants are
 *  DISPLAY_MATRIX and DISPLAY_LIST; which only print the matrix form or list
 *  form, respectively.
 */
void displayPixels(const cv::Mat &input, bool Color, bool debug, const int dType=DISPLAY_BOTH);

/*
 *  Labels a binary image, returns an Matrix with a different
 *  gray value for each object.
 */
cv::Mat binaryLabel(const cv::Mat &input);

/*
 *  Does the same as binaryLabel; except it adds the requirement that objects
 *  are circles.
 */
cv::Mat binaryLabelCircle(const cv::Mat &input);

/*
 * Returns a maximum found pixel value in a floating point matrix
 */
float maxFloatValue(const cv::Mat &input);

/*
 * Returns a minimal found pixel value in a floating point matrix
 */
float minFloatValue(const cv::Mat &input);


}

#endif /* STATISTICS_H_ */
