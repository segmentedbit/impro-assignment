/*
 * statistics.cc
 *
 *  Created on: Dec 11, 2013
 *      Author: segmented bit
 */

#include "opencv2/imgproc/imgproc.hpp"
#include "includes/statistics.h"
#include <cv.h>

using namespace cv;
using namespace im;


void im::displayPixels(const cv::Mat &input, bool Color, bool debug ){
	std::cout << " in function ptr : " << &input << std::endl;

	int count = 0;
	// print all pixels in console for comparison
	if(Color){
		std::cout << std::endl << std::endl << "Matrix color (BGR notation !!!) = " << std::endl << input << std::endl << std::endl;
	}
	else{
		std::cout << std::endl << std::endl << "Matrix grey-scale = " << std::endl << input << std::endl << std::endl;

	}
	std::cout << std::endl << " Pixel iteration -->" << std::endl <<std::endl;


	// loop through all pixels, row by row.
	for (int i = 0; i < (&input)->rows; i++){
		for (int j = 0; j < input.cols; j++){

			if (Color){
				int B = input.at<Vec3b>(i,j)[0];
				int G = input.at<Vec3b>(i,j)[1];
				int R = input.at<Vec3b>(i,j)[2];
				std::cout << count << ")  pixel at (" << i << "," << j << ") =" << " R:" << R << " G:" << G << " B:" << B ;
			}
			else{
				int grayValue = input.at<uchar>(i,j);
				std::cout << count << ")  pixel at (" << i << "," << j << ") =" << " value:" << grayValue ;
			}

			if (debug){
				//no idea what this does, Point2f? unsignedChar?
				Point2f point = input.at<Point2f>(i,j);
				uchar value = input.at<uchar>(i,j);
				std::cout << count << " --> DEBUG INFO = point:"<< point << "  value:" << value << std::endl;
			}
			else{
				std::cout << std::endl;
			}

		    count++;
		}
	}
}
