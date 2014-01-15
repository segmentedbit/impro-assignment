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

const int PZERO = 0;
const int PWHITE = 1;
const int PREPLICATE = 2;

const int UNEVEN = 1;

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
cv::Mat subtractMatrix(const cv::Mat &original, const cv::Mat &input);

/*
 * Copies an image into a new image, automatically creating the right padding
 * which is often needed for spation operations.
 * pType (padding type) can be one of the following constants:
 *    - PZERO		(fill padding with zeroes)
 *    - PWHITE		(fill padding with the max value)
 *    - PREPLICATE	(expand the border into the padding)
 */
cv::Mat copyWithPadding(const cv::Mat &original, const int pWidth, const int pHeight, const int pType);

/*
 * Equal takes two images an compares them, pixelwise, for equality
 */
bool equal(const cv::Mat &first, const cv::Mat &second);

/*
 * converts CV_8U matrix to CV_32F matrix
 * from chars(0..255) to float
 */
cv::Mat matUcharToFloat(const cv::Mat &input);

/*
 * converts CV_32F matrix to CV_8U
 * from float to chars(0..255)
 */
cv::Mat matFloatToUchar(const cv::Mat &input);

/*
 * Get time in nano seconds
 * TODO DO NOT USE THIS
 */
long int getTime();

int validateKernel(const cv::Mat &kernel, int flags);

cv::Mat addMatrixOld(const cv::Mat &original, const cv::Mat &input);
cv::Mat subtractMatrixOld(const cv::Mat &original, const cv::Mat &input);



}
#endif /* STOCKPILE_H_ */
