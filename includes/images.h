/*
 * images.h
 *
 *  Created on: Jan 20, 2014
 *      Author: ardillo
 */

#ifndef IMAGES_H_
#define IMAGES_H_

#include "opencv2/imgproc/imgproc.hpp"

namespace solve {

/*
 * This function will solve the picture balloons.png
 * It does:
 *  - gray level
 *  - x-derivative function   ++ - SKIZ
 */
cv::Mat balloons(const cv::Mat &input);

/*
 * Solve the picture balls.png
 *  it does:
 *  - gray level
 *  - smoothing
 *  - opening
 *  - labeling
 */
cv::Mat balls(const cv::Mat &input);

}

#endif /* IMAGES_H_ */
