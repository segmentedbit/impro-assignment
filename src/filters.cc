/*
 * filters.cc
 *
 *  Created on: Dec 29, 2013
 *      Author: ardillo
 */

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"	// TODO only for debugging
#include "includes/filters.h"
#include "includes/stockpile.h"
#include <vector>
#include <iostream>
#include "config.h"

using namespace std;
using namespace cv;

Mat im::averageFilter(const cv::Mat &input, int kWidth, int kHeight, const int pType) {
	// Check parameter values
	if (kWidth < 0 || kHeight < 0) {
		cerr << "averageFilter: input parameter kWidth or kHeight is negative)" << endl;
		exit(1);
	}

	// Check if and break when kernel width or height is even
	if (!(kWidth % 2 == 1 && kHeight % 2 == 1)){
		cerr << "averageFilter: width and/or height have to be odd, in order to make a correct matrix" << endl;
		exit(1);
	}

	Mat output(input.rows, input.cols, CV_8UC1);

	int kPixels = kWidth * kHeight;
	int hPadding = round(kWidth/2);
	int vPadding = round(kHeight/2);
	if (config::DEBUG) {
		cout << "averageFilter debug information:" << endl <<
				"\tkWidth: " << kWidth << endl <<
				"\tkHeight: " << kHeight << endl <<
				"\tkPixels: " << kPixels << endl <<
				"\thPadding: " << hPadding << endl <<
				"\tvPadding: " << vPadding << endl << endl;
	}

	Mat temp = im::copyWithPadding(input, hPadding, vPadding, pType);

	// temp. matrix is made, kernel is made, now calculate matrix multiply.
	for ( int i = 0; i < input.rows; i++) {
		for (int j = 0; j < input.cols; j++) {
			int middlePixel = 0;

			for (int x=0; x<kWidth; x++) {
				for (int y=0; y<kHeight; y++) {
					middlePixel += temp.at<uchar>(i+x, j+y);
				}
			}
			//  calculate the final result
			output.at<uchar>(i,j) = round(static_cast<float>(middlePixel) / kPixels);

		}
	}
	if (config::DEBUG) {
		namedWindow("Debug: averageFilter return image", CV_WINDOW_NORMAL);
		imshow("Debug: averageFilter return image",output);
		waitKey(0);
	}
	return output;
}

//TODO does not work completely
Mat im::filter(const cv::Mat &input, const cv::Mat &kernel) {
	// kernel-size check has to be done in the process of making the kernel.
	Mat output(input.rows, input.cols, CV_32S);

	int pWidth = round(kernel.cols/2);
	int pHeight = round(kernel.rows/2);
	Mat temp = im::copyWithPadding(input, 2, 2, PZERO);


	 //int horizontal_padding = round(kernel.cols/2);
	 //int vertical_padding = round(kernel.rows/2);
	 /*
	  * First make temporary matrix with extra padding
	  * TODO could be changed in other padding types.
	  */
	/*
	 Mat temp( input.rows+(vertical_padding*2), input.cols+(horizontal_padding*2), CV_32SC1);
	 for (int i = 0; i < temp.rows; i++){
	   if (i < vertical_padding || i > (input.rows+vertical_padding-1)){        //adding padding in vertical way
	     temp.row(i) = round(0);
	   }
	   else {
	     for (int j = 0; j < temp.cols; j++){
	       if ( j < horizontal_padding || j > (input.cols+horizontal_padding-1) ) {    //adding padding in horizontal way
	         temp.col(j) = round(0);
	       }
	       else {
	         temp.at<uchar>(i,j) = input.at<uchar>(i - vertical_padding , j - horizontal_padding);
	       }
	     }
	   }
	 }
	 */

	cout << temp << endl;
	/*
	 *  calculate every value overlapping temporary matrix and the kernel,
	 *  place the ending result in the middle pixel of interest.
	 */
	for ( int i = 0; i < (input.rows); i++) {
		for (int j = 0; j < (input.cols); j++) {
			int middlePixel = 0; //debug
			for ( int x = 0; x < kernel.rows; x++) {
				for (int y = 0; y < kernel.cols; y ++) {
					middlePixel += temp.at<uchar>(i+x, j+y) * kernel.at<uchar>(x,y);
				}
			}
			//cout <<"pixel: " << middlePixel <<endl;
			output.at<uchar>(i,j) = middlePixel; //  calculate the final result
		}
	}
	return output;
}
