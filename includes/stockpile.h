/*
 * stockpile.h
 *
 *  Created on: Dec 11, 2013
 *      Author: segmentedbit & Ardillo
 */

#ifndef STOCKPILE_H_
#define STOCKPILE_H_

#include <opencv2/imgproc/imgproc.hpp>

// Our own namespace to be sure that our names don't cause conflicts.
namespace im {

const int PZERO = 0;
const int PWHITE = 1;
const int PREPLICATE = 2;

const int UNEVEN = 1;

/* NOTE
 * ====
 * The namespace 'cv' is written explicitly instead of writing 'using namespace cv'
 * on top of the file. If we set 'using namespace cv' the setting would
 * propagate through to the calling file which could lead to unexpected results.
 * Furthermore, the input is passed by reference and defined as const, so
 * it won't be modified.
 */


/*
 * Segments an image into pixelvalues of either 1 or 0. The pixels of value
 * <threshold> and higher are convertedto 1, values below are converted to zero.
 */
cv::Mat segmentize(const cv::Mat &input, int threshold);

/*
 * Thresholds an image. The pixels of value <threshold> and higher are converted
 * to 255, values below are converted to zero.
 */
cv::Mat threshold(const cv::Mat &input, int threshold);

/*
 * Inverts a Gray-scale image, also known as a negative image.
 * Is able to handle both grey scale and binary images.
 */
cv::Mat invertGray(const cv::Mat &input);

/*
 * Converts a color image to a greyscale image.
 * This sums up the different color channels and divide it by three
 * This has a deviation of exactly 1, so apparently opencv does
 * something different with their ToGrayScale operand.
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
 * divide two gray-scale matrices of the same size, a pixelwise ' / ' .
 * Matrices have to be of the same size, otherwise it will output  a warning.
 */
cv::Mat divideMatrix(const cv::Mat &first, const cv::Mat &second);

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
 * Deletes border objects, therefore you need to give a labeled
 * binary image
 */
cv::Mat deleteBorderObjects(const cv::Mat &input);

/*
 * Checks if kernel has an odd height and width in order
 * to have a absolute middle pixel
 */
int validateKernel(const cv::Mat &kernel, int flags);

}
#endif /* STOCKPILE_H_ */
