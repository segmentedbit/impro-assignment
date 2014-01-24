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

const int E_GOLAY = 0;
const int L_GOLAY = 1;
const int E_GOLAY2 = 2;

// boundary() flags: only straight connections or also diagonal
const int ALLDIRECTIONS = 0;
const int STRAIGHT = 1;
struct boundaryStruct {
	int perimiterLength;
	int objectPixels;
};

/*
 * Returns a 3x3, "+" shaped, structuring element.
 */
cv::Mat defaultElement();

/*
 * Boundary returns a struct of type {int, int}, in which the first integer
 * denotes the amount of boundary pixels found and the second denotes the total
 * amount of pixels found in the object. Don't feed the algorithm more than one
 * object or your measurement will not be correct.
 * The second Mat object passed will contain the pixels at the boundary.
 * Pass either ALLDIRECTIONS or STRAIGHT to determine the direction of boundary
 * matching.
 */
boundaryStruct boundary(const cv::Mat& input, cv::Mat& output, const int direction=STRAIGHT);

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
 * Pruning of an image.
 */
cv::Mat prune(const cv::Mat &input, int nTimes=-1);

/*
 * Creates a 2-dimensional Mat vector containing a golay alphabet. Returns
 * the L alphabet as a default, but can also return an e golay, by
 * parametarising it with E_GOLAY.
 *
 */
std::vector< std::vector<cv::Mat> > createGolay(const int type=L_GOLAY);
void normalizeLetter(cv::Mat & inputarray, cv::Mat & outputarray);
void thinning(cv::Mat& im);
void thinningGuoHall(cv::Mat& im);


} // end namespace im

#endif /* MORPHOLOGY_H_ */
