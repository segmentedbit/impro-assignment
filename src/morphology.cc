/*
 * morphology.cc
 *
 *  Created on: Jan 10, 2014
 *      Author: segmentedbit
 */

#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "includes/morphology.h"
#include "includes/stockpile.h"
#include "includes/statistics.h"

using namespace std;
using namespace cv;

Mat im::defaultElement() {
	Mat output = Mat::zeros(3, 3, CV_8UC1);
	output.row(1) = 1;
	output.col(1) = 1;
	return output;

}

Mat im::morphDilate(const Mat &input, const Mat &element) {
	int status = validateKernel(element, im::UNEVEN);
	if (status != 0) {
		cerr << "morphDilate: Structuring element does not validate" << endl;
		exit(1);
	}

	Mat output(input.size(), CV_8UC1);

	int eWidth = element.rows;
	int eHeight = element.cols;

	Mat temp = im::copyWithPadding(input, eWidth/2, eHeight/2, im::PZERO);

	// Iterate over all of inputs pixels
	for (int i = 0; i < input.rows; i++) {
		for (int j = 0; j < input.cols; j++) {
			int highestNeighbor = 0;

			/*


			// Check for the highest neighbor, where 'neighbor' is any pixel
			// that is contained in both the structuring element and the original
			// image.
			for (int x=0; x<eWidth; x++) {
				for (int y=0; y<eHeight; y++) {
					int neighbor = element.at<uchar>(x, y) * temp.at<uchar>(i+x, j+y);

					if (neighbor > highestNeighbor) {
						highestNeighbor = neighbor;
					}
				}
			}
			*/
			/*
			 * Ardillo... speed check
			 */
			for (int q = 0; q < 3; q++){
				int neighbor = element.at<uchar>(q,1) * temp.at<uchar>(i+q, j+1);
				if (neighbor > highestNeighbor) {
					highestNeighbor = neighbor;
				}
			}for (int q = 2; q >= 0; q -= 2){
				int neighbor = element.at<uchar>(1,q) * temp.at<uchar>(i+1, j+q);
				if (neighbor > highestNeighbor) {
					highestNeighbor = neighbor;
				}
			}
			//  calculate the final result
			output.at<uchar>(i,j) = highestNeighbor;
		}
	}

	return output;
}

Mat im::morphErode(const Mat &input, const Mat &element) {
	int status = validateKernel(element, im::UNEVEN);
	if (status != 0) {
		cerr << "morphErode: Structuring element does not validate" << endl;
		exit(1);
	}

	Mat output(input.size(), CV_8UC1);

	int eWidth = element.rows;
	int eHeight = element.cols;

	Mat temp = im::copyWithPadding(input, eWidth/2, eHeight/2, im::PWHITE);

	// Iterate over all of inputs pixels
	for (int i = 0; i < input.rows; i++) {
		for (int j = 0; j < input.cols; j++) {
			int lowestNeighbor = 255;

			// Check for the lowest neighbor, where 'neighbor' is any pixel
			// that is contained in both the structuring element and the original
			// image.
 			for (int x=0; x<eWidth; x++) {
				for (int y=0; y<eHeight; y++) {
					if (element.at<uchar>(x,y) == 1) {
						int neighbor = temp.at<uchar>(i+x, j+y);
						if (neighbor < lowestNeighbor) {
							lowestNeighbor = neighbor;
						}

					}
				}
			}			//  calculate the final result
			output.at<uchar>(i,j) = lowestNeighbor;
		}
	}

	return output;
}

Mat im::morphOpen(const Mat &input, const Mat &erosionElement, const Mat &dilationElement) { //TODO implement
	Mat output = morphErode(input, erosionElement);
	output = morphDilate(output, dilationElement);
	return output;
}

Mat im::morphClose(const Mat &input, const Mat &dilationElement, const Mat &erosionElement) { //TODO implement
	Mat output = morphDilate(input, dilationElement);
	output = morphErode(output, erosionElement);
	return output;
}

// To a great extend, the code is the same as for morphDilate, except for
// the while loop and some small other changes. This is done deliberately though,
// as calling morphDilate would force us to create a second loop over the image
// to calculate the minimum of the output image and control image.
Mat im::morphGeodesicDilate(const Mat &input, const Mat &control, const Mat &element, int nTimes) {
	int status = validateKernel(element, im::UNEVEN);
	if (status != 0) {
		cerr << "morphDilate: Structuring element does not validate" << endl;
		exit(1);
	}
	im::displayPixels(input, false, false, im::DISPLAY_MATRIX);

	Mat output = input.clone();
	int eWidth = element.rows;
	int eHeight = element.cols;

	int loops = 0;
	// Loop until nTimes = 0, or
	while (nTimes != 0) {
		Mat tempDilate = im::copyWithPadding(output, eWidth/2, eHeight/2, im::PZERO);
		Mat tempGeo = output.clone();
		// Iterate over all of inputs pixels
		for (int i = 0; i < output.rows; i++) {
			for (int j = 0; j < output.cols; j++) {
				int highestNeighbor = 0;

				// Check for the highest neighbor, where 'neighbor' is any pixel
				// that is contained in both the structuring element and the original
				// image.
				for (int x=0; x<eWidth; x++) {
					for (int y=0; y<eHeight; y++) {
						int neighbor = element.at<uchar>(x, y) * tempDilate.at<uchar>(i+x, j+y);

						if (neighbor > highestNeighbor) {
							highestNeighbor = neighbor;
						}
					}
				}
				//  calculate the final result
				output.at<uchar>(i,j) = min(highestNeighbor, static_cast<int>(control.at<uchar>(i,j)));

			}
		}
		if (im::equal(output, tempGeo)) {
			break;
		}
		nTimes--;
	}
	return output;
}

Mat im::morphGeodesicErode(const Mat &input, const Mat &control, const Mat &element, const int nTimes) {
	return input;
}


Mat im::morphSkeleton(const Mat &input) {
	Mat output(input.size(), CV_8UC1);
	return output;
}
