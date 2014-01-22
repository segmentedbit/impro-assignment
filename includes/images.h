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
 *  - smoothing
 *  - opening
 *  - labeling
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

<<<<<<< HEAD
/*
 * Solve the xray.png search for steel objects in the image.
 * it does:
 */
void xray(const cv::Mat &input);
=======
void boltsnuts(const cv::Mat &input);
>>>>>>> 09c205ef97466ed6b3f418d6130d53c3c6c3ee7d
}

#endif /* IMAGES_H_ */
