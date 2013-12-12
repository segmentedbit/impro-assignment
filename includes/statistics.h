/*
 * statistics.h
 *
 *  Created on: Dec 11, 2013
 *      Author: segmented bit
 */

#ifndef STATISTICS_H_
#define STATISTICS_H_

#include "opencv2/imgproc/imgproc.hpp"

// Our own namespace to be sure that our names don't cause conflicts.
namespace im {

const int INCREASE = 1, DECREASE = 0;

/* Shows histogram of given input image. Uses a plain Mat image to display the
 * height of all the bins.
 */
cv::Mat showHist(const cv::Mat &input);

/* Equalizes the given input image
 *
 */
cv::Mat equalize(const cv::Mat &input);

/* Enhances contrast on given interval. Intervals are interpreted as
 * [left, right). Mode can be either INCREASE or DECREASE, which are const ints
 * defined to be 1 or 0, respectively.
 */
cv::Mat enhanceConstrast(const cv::Mat &input, int left, int right, int mode);

}

int maxValue(const cv::Mat &input);

int minValue(const cv::Mat &input);

#endif /* STATISTICS_H_ */
