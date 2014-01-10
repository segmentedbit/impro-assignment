/*
 * morphology.h
 *
 *  Created on: Jan 10, 2014
 *      Author: segmentedbit
 */

#ifndef MORPHOLOGY_H_
#define MORPHOLOGY_H_

#include "opencv2/imgproc/imgproc.hpp"

namespace im {

/*
 * Returns a 3x3, "+" shaped, structuring element.
 */
cv::Mat defaultElement();

/*
 * Dilation function. Takes an optional Mat &kernel parameter, which can be of
 * any size. When not provided, the function will assume an 3x3 "+" kernel
 */
cv::Mat morphDilate(const cv::Mat &input, const cv::Mat &kernel=defaultElement());

/*
 * Erosion function. Takes an optional Mat &kernel parameter, which can be of
 * any size. When not provided, the function will assume an 3x3 "+" kernel
 */
cv::Mat morphErode(const cv::Mat &input, const cv::Mat &kernel=defaultElement());

/*
 * Opening function. Takes two optional parameters for the structuring
 * element: the erosion and dilation elements. They may differ. When not
 * supplied, both elements assume a 3x3 "+" structuring element.
 * Note that the order of element parameters differs from morphClose()
 */
cv::Mat morphOpen(const cv::Mat &input,
		const cv::Mat &erosionElement=defaultElement(),
		const cv::Mat &dilationElement=defaultElement());

/*
 * Opening function. Takes two optional parameters for the structuring
 * element: the dilation and erosion elements. They may differ. When not
 * supplied, both elements assume a 3x3 "+" structuring element.
 * Note that the order of element parameters differs from morphOpen()
 */
cv::Mat morpClose(const cv::Mat &input,
		const cv::Mat &dilationElement=defaultElement(),
		const cv::Mat &erosionElement=defaultElement());

/*
 * Finds the skeleton of a given image.
 */
cv::Mat morphSkeleton(const cv::Mat &input);

} // end namespace im

#endif /* MORPHOLOGY_H_ */
