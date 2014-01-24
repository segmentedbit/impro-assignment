/*
 * filters.h
 *
 *  Created on: Dec 29, 2013
 *      Author: Ardillo & segmentedbit
 */

#ifndef FILTERS_H_
#define FILTERS_H_

#include <opencv2/imgproc/imgproc.hpp>

namespace im {

/*
 * Smooth filter with variable size. Filter width and height have to
 * be odd in order to have a middle pixel. For pType, see 'copyWithPadding'
 */
cv::Mat averageFilter(const cv::Mat &input, int width, int height, int pType);

/*
 * Returns an isotropic gaussian kernel of parametrisized size. Calling the
 * function with size=5 with result in a 5x5 gaussian kernel, etc. Make sure
 * size is uneven. Also, this function returns a Mat object of type CV_32F1.
 */
cv::Mat gaussianKernel(const int size, const int sigma=1);

/*
 * As the name says.
 */
cv::Mat medianFilter(const cv::Mat& input, const int rows, const int cols);

/*
 * Works a gaussian filter on the input image, with size and sigma as parameter
 * Parameter is 1 as a default.
 */
cv::Mat gaussianFilter(const cv::Mat& input, const int size, const int sigma=1);

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
 * Quantiziation of a image.
 * It is found usable till +/- 40 levels. Using around 40 levels gives an image
 * that looks very similar to the input image as the difference between the
 * levels is too little to notice with the naked eye.
 */
cv::Mat quantization(const cv::Mat &input, const int levels);

/*
 * Calculates the local maximum of a window by sliding the window over the whole image
 * After calculating the maximum values do the same, but take the minimum values in
 * the freshly calculated maximum matrix. This way you get the local minimum of the maximum.
 * This could be used to filter shadow or shading in a picture.
 * Experiment with different window sizes to get a good result, it depends on the gradient
 * of the shading
 */
cv::Mat localMinimumOfMaximum(const cv::Mat &input, const int window_width, const int window_height);

/*
 * Calculates the local minimum of a window by sliding the window over the whole image
 * After calculating the minimum values do the same, but take the maximum values in
 * the freshly calculated minimum matrix. This way you get the local maximum of the minimum.
 * This could be used to filter shadow or shading in a picture.
 * Experiment with different window sizes to get a good result, it depends on the gradient
 * of the shading
 */
cv::Mat localMaximumOfMinimum(const cv::Mat &input, const int window_width, const int window_height);

}
#endif /* FILTERS_H_ */
