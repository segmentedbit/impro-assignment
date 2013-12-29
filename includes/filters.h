/*
 * filters.h
 *
 *  Created on: Dec 29, 2013
 *      Author: ardillo
 */

#ifndef FILTERS_H_
#define FILTERS_H_

#include "opencv2/imgproc/imgproc.hpp"

namespace im{

/*
 * Smooth filter with variable size. Filter width and height have to
 * be odd in order to have a middle pixel.
 * Calculation is done in an temporary matrix with floating point
 *  values varying from 0...1. This is done for higher accuracy.
 *  the temporary matrix has padding around the original image.
 */
cv::Mat averageFilter(const cv::Mat &input, int width, int height, bool debug);

}
#endif /* FILTERS_H_ */
