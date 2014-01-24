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
void balloons(const cv::Mat &input);

/*
 * Solve the picture balls.png
 *  it does:
 *  - gray level
 *  - median filter
 *  - histogram
 *  - thresholding
 *  - erode 25x
 *  - dilate 25x
 *  - label image
 */
void balls(const cv::Mat &input);

/*
 * Solve the cheese.png, count the holes in the cheese.
 * it does:
 * - convert to gray level
 * - gaussian filter
 * - threshold
 * - closing
 * - inverse of closing result
 * - labeling
 * - delete border objects
 */
void cheese(const cv::Mat &input);

/*
 * Solve the xray.png search for steel objects in the image.
 * it does:
 * TODO
 */
void xray(const cv::Mat &input);

/*
 * Solve the boltsnuts.png search for the nuts in the image
 * it does:
 * TODO
 */
void boltsnuts(const cv::Mat &input);

/*
 *
 */
void road(const cv::Mat &input);
}




#endif /* IMAGES_H_ */
