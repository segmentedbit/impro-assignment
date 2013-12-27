/*
 * stockpile.h
 *
 *  Created on: Dec 11, 2013
 *      Author: segmented bit / Ardillo
 */

#ifndef STOCKPILE_H_
#define STOCKPILE_H_

#include "opencv2/imgproc/imgproc.hpp"

// Our own namespace to be sure that our names don't cause conflicts.
namespace im {

/* Inverses an image. Should be able to handle both grey scale and binary
 * images.
 * The namespace 'cv' is written explicitly instead of writing 'using namespace cv'
 * on top of the file. If we set 'using namespace cv' the setting would
 * propagate through to the calling file which could lead to unexpected results.
 * Furthermore, the input is passed by reference and defined as const, because
 * it won't be modified.
 */
cv::Mat invert(const cv::Mat &input);

/* Thresholds an image. The pixels of value <threshold> and higher are converted
 * to 1, values below are converted to zero, as by convention (is this true?)
 */
cv::Mat threshold(const cv::Mat &input, int threshold);

/* Converts a color image to a greyscale image.
 * This sums up the different color channels and divide it by three
 * TODO This has a deviation of exactly 1, so apparently opencv does
 * something different with their ToGrayScale operand.
 */
cv::Mat greyscale(const cv::Mat &input);
}

#endif /* STOCKPILE_H_ */
