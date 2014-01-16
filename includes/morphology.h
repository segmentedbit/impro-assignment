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
cv::Mat morphClose(const cv::Mat &input,
		const cv::Mat &dilationElement=defaultElement(),
		const cv::Mat &erosionElement=defaultElement());

/*
 * Geodesic dilation. Takes two Mat images: the to-be-dilated image and the
 * control image. The function keeps dilating and comparing the target image
 * with the control image, unless an optional nTime parameter is supplied,
 * which will be the maximum amount of loops to be run. The default structuring
 * element is a 3x3 "+" shaped element.
 */
cv::Mat morphGeodesicDilate(const cv::Mat &input, const cv::Mat &control, int nTimes=-1);

/*
 * Geodesic erosion. Takes two Mat images: the to-be-eroded image and the
 * control image. The function keeps eroding and comparing the target image
 * with the control image, unless an optional nTime parameter is supplied,
 * which will be the maximum amount of loops to be run. The default structuring
 * element is a 3x3 "+" shaped element.
 */
cv::Mat morphGeodesicErode(const cv::Mat &input, const cv::Mat &control, const cv::Mat &element, int nTimes=-1);

/*
 * Finds the skeleton of a given image.
 */
cv::Mat morphSkeleton(const cv::Mat &input, int nTimes=-1);

/*
 * Creates a 2-dimensional Mat vector containing the 8 L Golay elements
 */
std::vector<std::vector<cv::Mat>> createGolay();


} // end namespace im

#endif /* MORPHOLOGY_H_ */
