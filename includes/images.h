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
 *  TODO not working yet
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
 * Solve the xray.png search for steel objects within the person centered in the image.
 * it does:
 * - to gray scale
 * - inverse of thresholding
 * - inverse of gray scale image
 * - geoDesicDilate from background
 * - subtract geoDesicDilate output from inverse thresholded image
 * - threshold of above, for silhoutte determination and detection
 * - labelling
 */
void xray(const cv::Mat &input);

/*
 * Solve the boltsnuts.png search for the nuts in the image
 * it does:
 * TODO
 */
void boltsnuts(const cv::Mat &input);

/*
 * Solve road.png, detect highway in picture
 * it does:
 * - to gray scale
 * - gaussian filter
 * - quantization with 8 levels
 * - histogram
 * - histogram equalize
 * - histogram to determine differences
 * - thresholding
 * - dilation 3x
 * - geoDesicDilate
 */
void road(const cv::Mat &input);
}




#endif /* IMAGES_H_ */
