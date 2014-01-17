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
#include "config.h"

using namespace std;
using namespace cv;

const int BG = 0;
const int FG = 1;

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

//TODO implement
Mat im::morphOpen(const Mat &input, const Mat &erosionElement, const Mat &dilationElement) {
	Mat output = morphErode(input, erosionElement);
	output = morphDilate(output, dilationElement);
	return output;
}

//TODO implement
Mat im::morphClose(const Mat &input, const Mat &dilationElement, const Mat &erosionElement) {
	Mat output = morphDilate(input, dilationElement);
	output = morphErode(output, erosionElement);
	return output;
}

Mat im::morphGeodesicDilate(const Mat &input, const Mat &control, int nTimes) {
	Mat output = input.clone();

	// Loop until nTimes = 0, or
	while (nTimes != 0) {
		Mat tempDilate = im::copyWithPadding(output, 1, 1, im::PZERO);
		Mat tempGeo = output.clone();
		// Iterate over all of inputs pixels
		for (int i = 0; i < output.rows; i++) {
			for (int j = 0; j < output.cols; j++) {
				int highestNeighbor = 0;

				// Check for the highest neighbor, where 'neighbor' is any pixel
				// that is contained in both the structuring element and the original
				// image.
				for (int x=0; x<=2; x++) {
					for (int y=0; y<=2; y++) {
						int neighbor = tempDilate.at<uchar>(i+x, j+y);

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

// Skeleton using thinning, with L Golay element

Mat im::morphSkeleton(const Mat &input, int nTimes) {
	Mat temp = input.clone();
	temp.col(0) = 0.0;
	temp.row(0) = 0.0;
	temp.col(input.cols - 1) = 0.0;
	temp.row(input.rows - 1) = 0.0;

	vector< vector<Mat> > golay = createGolay();

	if(config::DEBUG){
		cout << "temp matrix: " << endl
				<< temp << endl << endl;
	}

	while (nTimes != 0) {

		Mat stored_temp = temp.clone();

		//////////// Try these orders to test order fast//////////////
		//int order[8] = { 0,1,2,3,4,5,6,7 };
		int order[8] = { 7,6,5,4,3,2,1,0 };


		// Go over all 8 L Golay elements
		// ooh... and the order of the elements you use does matter
		for (int g=0; g < 7; g++) {
			Mat fg = golay[order[g]][FG];
			Mat bg = golay[order[g]][BG];

//			cout << "Golay Element L" << g + 1 << endl;

			// For every pixel in temp, figure out if Hit-or-Miss has a fix on it,
			// using the current Golay element. If so, subtract from output.
			for (int i = 0; i < temp.cols ; i++) {
				for (int j = 0; j < temp.rows; j++) {

					bool hit = true;

					// Convolute both BG and FG, checking if all pixels match
					for (int x=0; x<3; x++) {
						for (int y=0; y<3; y++) {

							uchar fg_char = fg.at<uchar>(x,y);
							uchar bg_char = bg.at<uchar>(x,y);

							uchar t_char = temp.at<uchar>(x+i, y+j);

							// The foreground check of the golay element;
							// breaks loop if pixel != foreground pixel;
							if ( fg_char &&  fg_char != t_char * fg_char) {
								hit = false;
								break;
							}
							// The background check of the golay element;
							// breaks loop if pixel != background pixel;
							if ( bg_char && bg_char != (1-t_char) * bg_char) {
								hit = false;
								break;
							}
						}
						// At least one check misses, and therefore don't continue loop
						if (hit == false) {
							break;
						}
					} // end nested golay element for-loop

					if (hit) {
						temp.at<uchar>(i + 1, j + 1) = 0;
							if (config::DEBUG){
								cout << "got a hit! at: Golay L"
										<< g + 1 << " erasing pixel (" << i + 1 << ","
										<< j + 1 << ")" << endl << endl;
							}
					}
				}
			}
		} // end 8 golay for-loop

		if (im::equal(stored_temp, temp)) {
			break;
		}
		nTimes--;
	}
	Mat output = temp.clone();
	return output;
}

////////////////////////SEGMENTED BIT THINNING //////////////////////////////////
/*
Mat im::morphSkeleton(const Mat &input, int nTimes) {
	Mat output = input.clone();
	output.col(0) = 0.0;
	output.row(0) = 0.0;
	output.col(output.cols - 1) = 0.0;
	output.row(output.rows - 1) = 0.0;
	vector< vector<Mat> > golay = createGolay();

	while (nTimes != 0) {
		// Update temp and tempPrev (which is the control image used to
		// determine if the final skeleton has been found) at every loop.
		Mat temp = output.clone(); // kan weg
		Mat tempPrev = output.clone();


		// Go over all 8 L Golay elements
		for (int g=0; g<8; g++) {
			Mat fg = golay[g][FG];
			Mat bg = golay[g][BG];

			// For every pixel in temp, figure out if Hit-or-Miss has a fix on it,
			// using the current Golay element. If so, subtract from output.
			for (int i = 1; i < output.cols - 1; i++) {
				for (int j = 1; j < output.rows - 1; j++) {

					bool hit = true;
					// Convolute both BG and FG, checking if all pixels match
					for (int x=0; x<3; x++) {
						for (int y=0; y<3; y++) {
							// Store temporary values
							uchar t_char = temp.at<uchar>(x+i, y+j);
							uchar fg_char = fg.at<uchar>(x,y);
							uchar bg_char = bg.at<uchar>(x,y);

							// The foreground of the golay element should hit
							// the foreground of the image;
							if (fg_char != t_char * fg_char) {
								hit = false;
								break;
							}

							// The foreground of the golay element should hit
							// the foreground of the image;
							if (bg_char != (1-t_char) * bg_char) {
								hit = false;
								break;
							}

						}
						if (hit == false) {
							break;
						}
					} // end nested golay element for-loop

					if (hit) {
						output.at<uchar>(i,j) = 0;
//						//cout << "got a hit!" << endl;
					}
				}
			}

		} // end 8 golay for-loop

		if (im::equal(output, tempPrev)) {
			break;
		}
		nTimes--;
	}
	cout << "finished";
	return output;
}

vector<vector<Mat>> im::createGolay() {
	vector< vector<Mat> > golay(8, vector<Mat>(2));

	// Create the 8 3x3 Mat elements
	for (int i=0; i<8; i++) {
		for (int j=0; j<2; j++) {
			golay.at(i).at(j) = Mat::zeros(3, 3, CV_8UC1);
		}
	}

	// Create golay element 0
	golay[0][BG].row(0) = 1;
	golay[0][FG].row(2) = 1;
	golay[0][FG].at<uchar>(1,1) = 1;

	// Create golay element 1
	golay[1][BG].at<uchar>(0,1) = 1;
	//golay[1][BG].at<uchar>(0,2) = 1; //changeable
	golay[1][BG].at<uchar>(1,2) = 1;
	golay[1][FG].at<uchar>(1,0) = 1;
	golay[1][FG].at<uchar>(1,1) = 1;
	golay[1][FG].at<uchar>(2,0) = 1; //changeable
	golay[1][FG].at<uchar>(2,1) = 1;

	// Create golay element 2
	golay[2][BG].col(2) = 1;
	golay[2][FG].col(0) = 1;
	golay[2][FG].at<uchar>(1,1) = 1;

	// Create golay element 3
	golay[3][BG].at<uchar>(1,2) = 1;
	golay[3][BG].at<uchar>(2,1) = 1;
	//golay[3][BG].at<uchar>(2,2) = 1; //changeable
	golay[3][FG].at<uchar>(0,0) = 1; //changeable
	golay[3][FG].at<uchar>(0,1) = 1;
	golay[3][FG].at<uchar>(1,0) = 1;
	golay[3][FG].at<uchar>(1,1) = 1;

	// Create golay element 4
	golay[4][BG].row(2) = 1;
	golay[4][FG].row(0) = 1;
	golay[4][FG].at<uchar>(1,1) = 1;

	// Create golay element 5
	golay[5][BG].at<uchar>(1,0) = 1;
	golay[5][BG].at<uchar>(2,1) = 1;
	//golay[5][BG].at<uchar>(2,2) = 1; //changeable
	golay[5][FG].at<uchar>(0,1) = 1;
	golay[5][FG].at<uchar>(0,2) = 1; //changeable
	golay[5][FG].at<uchar>(1,1) = 1;
	golay[5][FG].at<uchar>(1,2) = 1;

	// Create golay element 6
	golay[6][BG].col(0) = 1;
	golay[6][FG].col(2) = 1;
	golay[6][FG].at<uchar>(1,1) = 1;

	// Create golay element 7
	//golay[7][BG].at<uchar>(0,0) = 1; //changeable
	golay[7][BG].at<uchar>(0,1) = 1;
	golay[7][BG].at<uchar>(1,0) = 1;
	golay[7][FG].at<uchar>(1,1) = 1;
	golay[7][FG].at<uchar>(1,2) = 1;
	golay[7][FG].at<uchar>(2,1) = 1;
	golay[7][FG].at<uchar>(2,2) = 1; //changeable

	if(config::DEBUG) {
		for (int i=0; i<8; i++) {
			cout << "Golay Element L" << i + 1 << endl;
			for (int j=0; j<2; j++) {
				if (j == 0){
					cout << "Background:" << endl;
				}
				else {
					cout << "Foreground:" << endl;
				}
				cout	<< golay.at(i).at(j) << endl;
			}
			cout << endl;
		}
	}

	return golay;
}
