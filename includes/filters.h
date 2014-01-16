/*
 * filters.h
 *
 *  Created on: Dec 29, 2013
 *      Author: ardillo
 */

#ifndef FILTERS_H_
#define FILTERS_H_

#include "opencv2/imgproc/imgproc.hpp"

namespace im {

/*
 * Smooth filter with variable size. Filter width and height have to
 * be odd in order to have a middle pixel. For pType, see 'copyWithPadding'
 */
cv::Mat averageFilter(const cv::Mat &input, int width, int height, int pType);

/*
 * Smooth filter with variable size. Filter width and height have to
 * be odd in order to have a middle pixel.
 * Calculation is done in an temporary matrix with floating point
 *  values varying from 0...1. This is done for higher accuracy.
 *  the temporary matrix has padding around the original image.
 */
cv::Mat averageFilterDebug(const cv::Mat &input, int width, int height, bool debug);

/*
 * Basic filter which excepts an input matrix and a custom made kernel.
 * It automatically transposes the input to a temporary image with padding.
 */
cv::Mat filter(const cv::Mat &input, const cv::Mat &kernel, const float divide_factor);

/*
 * Quantiziation of a image. Converts an image to floating point notation.
 * Then searches the max and min value of the image, using this info it
 * will calculate the pixel delta and divide them into equal spaces.
 * It then loops over the floating point matrix and gives a pixel the corresponding
 * middle space value of the space it belongs to.
 * After that it converts the floating point matrix back to uchar matrix, which acts as
 * the output image.
 * This is tested and found stable till +/- 40 levels. Using around 40 levels gives an image
 * very corresponding to the input image with to little difference to see with naked eye.
 */
cv::Mat quantization(const cv::Mat &input, const int levels);

}
#endif /* FILTERS_H_ */
