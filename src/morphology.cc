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
			int highestNeighbour = 0;

			// Check for the highest neighbour, where 'neighbour' is any pixel
			// that is contained in both the structuring element and the original
			// image.
			for (int x=0; x<eWidth; x++) {
				for (int y=0; y<eHeight; y++) {
					int neighbour = element.at<uchar>(x, y) * temp.at<uchar>(i+x, j+y);

					if (neighbour > highestNeighbour) {
						highestNeighbour = neighbour;
					}
				}
			}
			//  calculate the final result
			output.at<uchar>(i,j) = highestNeighbour;
		}
	}

	return output;
}

Mat im::morphErode(const Mat &input, const Mat &kernel) { //TODO implement
	Mat output(input.size(), CV_8UC1);
	return output;
}

Mat im::morphOpen(const Mat &input, const Mat &erosionElement, const Mat &dilationElement) { //TODO implement
	Mat output(input.size(), CV_8UC1);
	return output;
}

Mat im::morpClose(const Mat &input, const Mat &dilationElement, const Mat &erosionElement) { //TODO implement
	Mat output(input.size(), CV_8UC1);
	return output;
}

Mat im::morphSkeleton(const Mat &input) {
	Mat output(input.size(), CV_8UC1);
	return output;
}
