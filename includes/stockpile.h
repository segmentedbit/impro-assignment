/*
 * stockpile.h
 *
 *  Created on: Dec 11, 2013
 *      Author: segmented bit / Ardillo
 */

#ifndef STOCKPILE_H_
#define STOCKPILE_H_

#include "opencv2/imgproc/imgproc.hpp"
#include <ctime> // for timing code snippets


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
 * TODO Ardillo: changed it in 0 or 255 instead of 1, this is easier to use with the 'Mat'
 * type. Mat doesn't except binary images with only two values.
 */
cv::Mat threshold(const cv::Mat &input, int threshold);

/*
 * Inverts a Gray-scale image, also known as a negative image.
 */
cv::Mat invertGray(const cv::Mat &input);

/* Converts a color image to a greyscale image.
 * This sums up the different color channels and divide it by three
 * TODO This has a deviation of exactly 1, so apparently opencv does
 * something different with their ToGrayScale operand.
 * TODO Has some other strange artifact, try to run program with checkerColor13_0_37.png?
 */
cv::Mat grayscale(const cv::Mat &input);

/*
 * Add two gray-scale matrices, a pixelwise ' + '.
 * Matrices have to be of the same size, otherwise it will output  a warning.
 */
cv::Mat addMatrix(const cv::Mat &original, const cv::Mat &input);

/*
 * Substract two gray-scale matrices of the same size, a pixelwise ' - ' .
 * Matrices have to be of the same size, otherwise it will output  a warning.
 */
cv::Mat substractMatrix(const cv::Mat &original, const cv::Mat &input);

/*
 * Get time in nano seconds
 */
long int getTime();

}
#endif /* STOCKPILE_H_ */
