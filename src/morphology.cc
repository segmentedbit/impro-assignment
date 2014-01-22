/*
 * morphology.cc
 *
 *  Created on: Jan 10, 2014
 *      Author: segmentedbit
 */

#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/highgui/highgui.hpp"

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

vector<Mat> createUElements() {
	vector<Mat> element;

	// Create the 8 3x3 Mat elements
	for (int i=0; i<4; i++) {
		element.push_back(Mat::zeros(3, 3, CV_8UC1));
	}

	element[0].row(0) = 1;
	element[0].at<uchar>(1,1) = 1;

	element[1].col(2) = 1;
	element[1].at<uchar>(1,1) = 1;

	element[2].row(2) = 1;
	element[2].at<uchar>(1,1) = 1;

	element[3].col(0) = 1;
	element[3].at<uchar>(1,1) = 1;

	if(config::DEBUG) {
		for (int i=0; i<4; i++) {
			cout << "'U'" << i << endl;
			cout << element.at(i) << endl;
		}
		cout << endl;
	}
	return element;
}

struct im::boundaryStruct im::boundary(const Mat& input, Mat& output, const int direction) {
	//if (input.size() != output.size()) {
	//	cerr << "im::boundary(): input and output Mat objects are not of the same size. Exiting" << endl;
	//	exit(1);
	//}
	output = Mat::zeros(input.size(), CV_8UC1);

	uint perimeterLength = 0;
	uint objectPixels = 0;
	vector<Mat> elements = createUElements();
	Mat temp = im::copyWithPadding(input, 1, 1, im::PWHITE);
	bool isBoundary = false;

	for (int y=0; y<input.rows; y++) {
		for (int x=0; x<input.cols; x++) {
			uchar pixValue = temp.at<uchar>(y+1, x+1);
			if (pixValue == 0) {
				// Not an object pixel, ignore.
				continue;
			}
			objectPixels++;

			// It's an object pixel. Is it part of the boundary?
			switch (direction) {
			case im::ALLDIRECTIONS:
				cerr << "Not yet implemented" << endl;
				exit(1);
				break; // end ALLDIRECTIONS
			case im::STRAIGHT:
				// Is this pixel part of a straight-connected boundary?
				isBoundary = false;
				for (int yy=-1; yy<=1; yy++) {
					if (isBoundary)
						break;
					for (int xx=-1; xx<=1; xx++) {
						uchar neighbor = temp.at<uchar>(y+yy+1, x+xx+1);
						if (neighbor == 0) { // this pixel is a background pixel
							// Found a background pixel!
							perimeterLength++;
							output.at<uchar>(y, x) = 1;
							// Stop looking further
							isBoundary = true;
							break;
						}
					}

				}
				// Done checking for boundaryness. Bit if it is, isn't it
				// also a double connection (check with 'U' shaped kernel)?
				if (!isBoundary) {
					break;
				}
				for (int i=0; i<4; i++) {
					// Per 'U' element loop
					bool elementIsSuccess = true;
					for (int yy=-1; yy<=1; yy++) {
						if (!elementIsSuccess)
							break;
						for (int xx=-1; xx<=1; xx++) {
							if (elements[i].at<uchar>(yy, xx) == 0 && temp.at<uchar>(y+yy+1, x+xx+1) != 0) {
								elementIsSuccess = false;
								break;
							}
							// Else we don't care
						}
					}
					if (elementIsSuccess) {
						perimeterLength++;
						break; // Don't bother trying the other elements, if any left
					}
				} // Done checking for double boundaryness
				break; // end STRAIGHT
			default:
				cerr << "im::boundary(): Direction parameter invalid, is " << direction << endl;
				exit(1);
			}
		}
	}
	im::boundaryStruct s;
	s.perimiterLength = perimeterLength;
	s.objectPixels = objectPixels;
	cout << "in " << &input << endl;
	return s;
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

Mat im::morphOpen(const Mat &input, const Mat &erosionElement, const Mat &dilationElement) {
	Mat output = morphErode(input, erosionElement);
	output = morphDilate(output, dilationElement);
	return output;
}


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


Mat im::prune(const Mat &input, int nTimes) {
	Mat output = input.clone();

	vector< vector<Mat> > golay = im::createGolay(im::E_GOLAY2);

	if(config::DEBUG){
		cout << "temp matrix: " << endl
				<< output << endl << endl;
	}

	while (nTimes != 0) {
		Mat control = output.clone();

		//////////// Try these orders to test order fast//////////////
		//int order[8] = { 0,1,2,3,4,5,6,7 };
		int order[8] = { 7,6,5,4,3,2,1,0 };

		// keeps track of what pixels are to removed

		// Go over all 8 L Golay elements
		for (int g=0; g < 7; g++) {
			Mat fg = golay[order[g]][FG];
			Mat bg = golay[order[g]][BG];

			Mat padded = im::copyWithPadding(output, 1, 1, im::PZERO);

			// For every pixel in temp, figure out if Hit-or-Miss has a fix on it,
			// using the current Golay element. If so, subtract from output.
			for (int i = 0; i < input.rows; i++) {
				for (int j = 0; j < input.cols; j++) {

					bool mark = true;

					// Convolute both BG and FG, checking if all pixels match
					for (int x=0; x<3; x++) {
						for (int y=0; y<3; y++) {

							uchar fgp = fg.at<uchar>(x,y);
							uchar bgp = bg.at<uchar>(x,y);
							uchar p  = padded.at<uchar>(i+x,j+y);

							// Determine if fg and bg match
							if ((fgp != p*fgp) || 		// false if both p and fgp are 1, or if fgp equals zero
								(bgp != (1-p) * bgp)) {	// false if both (1-p) and bgp are 1, ie bgp is 1 and p is zero=background.
														// 		also always false when bgp equals zero, which means don't care.
								mark = false;
								break;
							} else {

							}
						}
						// At least one check misses, and therefore don't continue loop
						if (mark == false) {
							break;
						}
					} // end nested golay element for-loop

					if (mark) {
						output.at<uchar>(i, j) = 0;
						if (config::DEBUG){
							cout << "got a hit! at: Golay L"
									<< g + 1 << " erasing pixel (" << i + 1 << ","
									<< j + 1 << ")" << endl << endl;
						}
					}
				}
			}
		} // end 8 golay for-loop

		if (im::equal(control, output)) {
			break;
		}
		nTimes--;
	}

	return output;
}


Mat im::morphSkeleton(const Mat &input, int nTimes) {
	Mat output = input.clone();

	vector< vector<Mat> > golay = createGolay(im::L_GOLAY);

	if(config::DEBUG){
		cout << "temp matrix: " << endl
				<< output << endl << endl;
	}

	while (nTimes != 0) {
		Mat control = output.clone();

		//////////// Try these orders to test order fast//////////////
		//int order[8] = { 0,1,2,3,4,5,6,7 };
		int order[8] = { 7,6,5,4,3,2,1,0 };

		// keeps track of what pixels are to removed

		// Go over all 8 L Golay elements
		for (int g=0; g < 7; g++) {
			Mat fg = golay[order[g]][FG];
			Mat bg = golay[order[g]][BG];

			Mat padded = im::copyWithPadding(output, 1, 1, im::PZERO);

			// For every pixel in temp, figure out if Hit-or-Miss has a fix on it,
			// using the current Golay element. If so, subtract from output.
			for (int i = 0; i < input.rows; i++) {
				for (int j = 0; j < input.cols; j++) {

					bool mark = true;

					// Convolute both BG and FG, checking if all pixels match
					for (int x=0; x<3; x++) {
						for (int y=0; y<3; y++) {

							uchar fgp = fg.at<uchar>(x,y);
							uchar bgp = bg.at<uchar>(x,y);
							uchar p  = padded.at<uchar>(i+x,j+y);

							// Determine if fg and bg match
							if ((fgp != p*fgp) || 		// false if both p and fgp are 1, or if fgp equals zero
								(bgp != (1-p) * bgp)) {	// false if both (1-p) and bgp are 1, ie bgp is 1 and p is zero=background.
														// 		also always false when bgp equals zero, which means don't care.
								mark = false;
								break;
							} else {

							}
						}
						// At least one check misses, and therefore don't continue loop
						if (mark == false) {
							break;
						}
					} // end nested golay element for-loop

					if (mark) {
						output.at<uchar>(i, j) = 0;
						if (config::DEBUG){
							cout << "got a hit! at: Golay L"
									<< g + 1 << " erasing pixel (" << i + 1 << ","
									<< j + 1 << ")" << endl << endl;
						}
					}
				}
			}
		} // end 8 golay for-loop

		if (im::equal(control, output)) {
			break;
		}
		/*
		// Run some pruning cycles
		if (nTimes % 5 == 0) {
			output = morphSkeleton2(output, 2);
		}
		*/
		nTimes--;
	}

	return output;
}




vector<vector<Mat>> im::createGolay(const int type) {
	vector< vector<Mat> > golay(8, vector<Mat>(2));

	// Create the 8 3x3 Mat elements
	for (int i=0; i<8; i++) {
		for (int j=0; j<2; j++) {
			golay.at(i).at(j) = Mat::zeros(3, 3, CV_8UC1);
		}
	}

	switch (type) {
	case (im::L_GOLAY):
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
		break;

	// Doesn't quite work, look into if necessary
	case im::E_GOLAY:
		golay[0][FG].at<uchar>(1,0) = 1;
		golay[0][FG].at<uchar>(1,1) = 1;
		golay[0][BG].at<uchar>(0,1) = 1;
		golay[0][BG].at<uchar>(2,1) = 1;
		golay[0][BG].row(2) = 1;

		golay[1][FG].at<uchar>(1,1) = 1;
		golay[1][BG].at<uchar>(2,1) = 1;
		golay[1][BG].col(0) = 1;
		golay[1][BG].row(2) = 1;

		golay[2][FG].at<uchar>(1,1) = 1;
		golay[2][FG].at<uchar>(2,1) = 1;
		golay[2][BG].at<uchar>(1,0) = 1;
		golay[2][BG].at<uchar>(1,2) = 1;
		golay[2][BG].col(0) = 1;

		golay[3][FG].at<uchar>(1,1) = 1;
		golay[3][BG].at<uchar>(1,2) = 1;
		golay[3][BG].col(0) = 1;
		golay[3][BG].row(0) = 1;

		golay[4][FG].at<uchar>(1,2) = 1;
		golay[4][FG].at<uchar>(1,1) = 1;
		golay[4][BG].at<uchar>(0,1) = 1;
		golay[4][BG].at<uchar>(2,1) = 1;
		golay[4][BG].row(0) = 1;

		golay[5][FG].at<uchar>(1,1) = 1;
		golay[5][BG].at<uchar>(0,1) = 1;
		golay[5][BG].col(2) = 1;
		golay[5][BG].row(0) = 1;

		golay[6][FG].at<uchar>(0,1) = 1;
		golay[6][FG].at<uchar>(1,1) = 1;
		golay[6][BG].at<uchar>(1,0) = 1;
		golay[6][BG].at<uchar>(1,2) = 1;
		golay[6][BG].col(2) = 1;

		golay[7][FG].at<uchar>(1,1) = 1;
		golay[7][BG].at<uchar>(1,0) = 1;
		golay[7][BG].col(2) = 1;
		golay[7][BG].row(2) = 1;
		break;

	// Useful for pruning
	case im::E_GOLAY2:
		golay[0][FG].at<uchar>(0,1) = 1;
		golay[0][FG].at<uchar>(1,1) = 1;
		golay[0][BG].at<uchar>(1,0) = 1;
		golay[0][BG].at<uchar>(1,2) = 1;
		golay[0][BG].row(2) = 1;

		golay[1][FG].at<uchar>(1,1) = 1;
		golay[1][BG].at<uchar>(1,2) = 1;
		golay[1][BG].col(0) = 1;
		golay[1][BG].row(2) = 1;

		golay[2][FG].at<uchar>(1,1) = 1;
		golay[2][FG].at<uchar>(1,2) = 1;
		golay[2][BG].at<uchar>(0,1) = 1;
		golay[2][BG].at<uchar>(2,1) = 1;
		golay[2][BG].col(0) = 1;

		golay[3][FG].at<uchar>(1,1) = 1;
		golay[3][BG].at<uchar>(2,1) = 1;
		golay[3][BG].col(0) = 1;
		golay[3][BG].row(0) = 1;

		golay[4][FG].at<uchar>(2,1) = 1;
		golay[4][FG].at<uchar>(1,1) = 1;
		golay[4][BG].at<uchar>(1,0) = 1;
		golay[4][BG].at<uchar>(1,2) = 1;
		golay[4][BG].row(0) = 1;

		golay[5][FG].at<uchar>(1,1) = 1;
		golay[5][BG].at<uchar>(1,0) = 1;
		golay[5][BG].col(2) = 1;
		golay[5][BG].row(0) = 1;

		golay[6][FG].at<uchar>(1,0) = 1;
		golay[6][FG].at<uchar>(1,1) = 1;
		golay[6][BG].at<uchar>(0,1) = 1;
		golay[6][BG].at<uchar>(2,1) = 1;
		golay[6][BG].col(2) = 1;

		golay[7][FG].at<uchar>(1,1) = 1;
		golay[7][BG].at<uchar>(0,1) = 1;
		golay[7][BG].col(2) = 1;
		golay[7][BG].row(2) = 1;
		break;
	}

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


/*
 * http://answers.opencv.org/question/3207/what-is-a-good-thinning-algorithm-for-getting-the/
 */
void ThinSubiteration1(Mat & pSrc, Mat & pDst) {
	int rows = pSrc.rows;
	int cols = pSrc.cols;
	pSrc.copyTo(pDst);
	for(int i = 0; i < rows; i++) {
		for(int j = 0; j < cols; j++) {
			if(pSrc.at<float>(i, j) == 1.0f) {
				/// get 8 neighbors
				/// calculate C(p)
				int neighbor0 = (int) pSrc.at<float>( i-1, j-1);
				int neighbor1 = (int) pSrc.at<float>( i-1, j);
				int neighbor2 = (int) pSrc.at<float>( i-1, j+1);
				int neighbor3 = (int) pSrc.at<float>( i, j+1);
				int neighbor4 = (int) pSrc.at<float>( i+1, j+1);
				int neighbor5 = (int) pSrc.at<float>( i+1, j);
				int neighbor6 = (int) pSrc.at<float>( i+1, j-1);
				int neighbor7 = (int) pSrc.at<float>( i, j-1);
				int C = int(~neighbor1 & ( neighbor2 | neighbor3)) +
						int(~neighbor3 & ( neighbor4 | neighbor5)) +
						int(~neighbor5 & ( neighbor6 | neighbor7)) +
						int(~neighbor7 & ( neighbor0 | neighbor1));
				if(C == 1) {
					/// calculate N
					int N1 = int(neighbor0 | neighbor1) +
							 int(neighbor2 | neighbor3) +
							 int(neighbor4 | neighbor5) +
							 int(neighbor6 | neighbor7);
					int N2 = int(neighbor1 | neighbor2) +
							 int(neighbor3 | neighbor4) +
							 int(neighbor5 | neighbor6) +
							 int(neighbor7 | neighbor0);
					int N = min(N1,N2);
					if ((N == 2) || (N == 3)) {
						/// calculate criteria 3
						int c3 = ( neighbor1 | neighbor2 | ~neighbor4) & neighbor3;
						if(c3 == 0) {
							pDst.at<float>( i, j) = 0.0f;
						}
					}
				}
			}
		}
	}
}


void ThinSubiteration2(Mat & pSrc, Mat & pDst) {
	int rows = pSrc.rows;
	int cols = pSrc.cols;
	pSrc.copyTo( pDst);
	for(int i = 0; i < rows; i++) {
		for(int j = 0; j < cols; j++) {
			if (pSrc.at<float>( i, j) == 1.0f) {
				/// get 8 neighbors
				/// calculate C(p)
				int neighbor0 = (int) pSrc.at<float>( i-1, j-1);
				int neighbor1 = (int) pSrc.at<float>( i-1, j);
				int neighbor2 = (int) pSrc.at<float>( i-1, j+1);
				int neighbor3 = (int) pSrc.at<float>( i, j+1);
				int neighbor4 = (int) pSrc.at<float>( i+1, j+1);
				int neighbor5 = (int) pSrc.at<float>( i+1, j);
				int neighbor6 = (int) pSrc.at<float>( i+1, j-1);
				int neighbor7 = (int) pSrc.at<float>( i, j-1);
				int C = int(~neighbor1 & ( neighbor2 | neighbor3)) +
						int(~neighbor3 & ( neighbor4 | neighbor5)) +
						int(~neighbor5 & ( neighbor6 | neighbor7)) +
						int(~neighbor7 & ( neighbor0 | neighbor1));
				if(C == 1) {
					/// calculate N
					int N1 = int(neighbor0 | neighbor1) +
							 int(neighbor2 | neighbor3) +
							 int(neighbor4 | neighbor5) +
							 int(neighbor6 | neighbor7);
					int N2 = int(neighbor1 | neighbor2) +
							 int(neighbor3 | neighbor4) +
							 int(neighbor5 | neighbor6) +
							 int(neighbor7 | neighbor0);
					int N = min(N1,N2);
					if((N == 2) || (N == 3)) {
						int E = (neighbor5 | neighbor6 | ~neighbor0) & neighbor7;
						if(E == 0) {
							pDst.at<float>(i, j) = 0.0f;
						}
					}
				}
			}
		}
	}
}


void im::normalizeLetter(Mat & inputarray, Mat & outputarray) {
	bool bDone = false;
	int rows = inputarray.rows;
	int cols = inputarray.cols;

	inputarray.convertTo(inputarray,CV_32FC1);

	inputarray.copyTo(outputarray);

	outputarray.convertTo(outputarray,CV_32FC1);

	/// pad source
	Mat p_enlarged_src = Mat(rows + 2, cols + 2, CV_32FC1);
	for(int i = 0; i < (rows+2); i++) {
		p_enlarged_src.at<float>(i, 0) = 0.0f;
		p_enlarged_src.at<float>( i, cols+1) = 0.0f;
	}
	for(int j = 0; j < (cols+2); j++) {
		p_enlarged_src.at<float>(0, j) = 0.0f;
		p_enlarged_src.at<float>(rows+1, j) = 0.0f;
	}
	for(int i = 0; i < rows; i++) {
		for(int j = 0; j < cols; j++) {
			if (inputarray.at<float>(i, j) >= 20.0f) {
				p_enlarged_src.at<float>( i+1, j+1) = 1.0f;
			}
			else
				p_enlarged_src.at<float>( i+1, j+1) = 0.0f;
		}
	}

	/// start to thin
	Mat p_thinMat1 = Mat::zeros(rows + 2, cols + 2, CV_32FC1);
	Mat p_thinMat2 = Mat::zeros(rows + 2, cols + 2, CV_32FC1);
	Mat p_cmp = Mat::zeros(rows + 2, cols + 2, CV_8UC1);

	while (bDone != true) {
		/// sub-iteration 1
		ThinSubiteration1(p_enlarged_src, p_thinMat1);
		/// sub-iteration 2
		ThinSubiteration2(p_thinMat1, p_thinMat2);
		/// compare
		compare(p_enlarged_src, p_thinMat2, p_cmp, CV_CMP_EQ);
		/// check
		int num_non_zero = countNonZero(p_cmp);
		if(num_non_zero == (rows + 2) * (cols + 2)) {
			bDone = true;
		}
		/// copy
		p_thinMat2.copyTo(p_enlarged_src);
	}
	// copy result
	for(int i = 0; i < rows; i++) {
		for(int j = 0; j < cols; j++) {
			outputarray.at<float>( i, j) = p_enlarged_src.at<float>( i+1, j+1);
		}
	}
}



/**
 * Code for thinning a binary image using Zhang-Suen algorithm.
 * https://opencv-code.com/quick-tips/implementation-of-thinning-algorithm-in-opencv/
 */

/**
 * Perform one thinning iteration.
 * Normally you wouldn't call this function directly from your code.
 *
 * @param  im    Binary image with range = 0-1
 * @param  iter  0=even, 1=odd
 */
void thinningIteration(cv::Mat& im, int iter)
{
    cv::Mat marker = cv::Mat::zeros(im.size(), CV_8UC1);

    for (int i = 1; i < im.rows-1; i++)
    {
        for (int j = 1; j < im.cols-1; j++)
        {
            uchar p2 = im.at<uchar>(i-1, j);
            uchar p3 = im.at<uchar>(i-1, j+1);
            uchar p4 = im.at<uchar>(i, j+1);
            uchar p5 = im.at<uchar>(i+1, j+1);
            uchar p6 = im.at<uchar>(i+1, j);
            uchar p7 = im.at<uchar>(i+1, j-1);
            uchar p8 = im.at<uchar>(i, j-1);
            uchar p9 = im.at<uchar>(i-1, j-1);

            int A  = (p2 == 0 && p3 == 1) + (p3 == 0 && p4 == 1) +
                     (p4 == 0 && p5 == 1) + (p5 == 0 && p6 == 1) +
                     (p6 == 0 && p7 == 1) + (p7 == 0 && p8 == 1) +
                     (p8 == 0 && p9 == 1) + (p9 == 0 && p2 == 1);
            int B  = p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;
            int m1 = iter == 0 ? (p2 * p4 * p6) : (p2 * p4 * p8);
            int m2 = iter == 0 ? (p4 * p6 * p8) : (p2 * p6 * p8);

            if (A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0)
                marker.at<uchar>(i,j) = 1;
        }
    }

    im &= ~marker;
}

/**
 * Function for thinning the given binary image
 *
 * @param  im  Binary image with range = 0-255
 */
void im::thinning(cv::Mat& im)
{
    im /= 255;

    cv::Mat prev = cv::Mat::zeros(im.size(), CV_8UC1);
    cv::Mat diff;

    do {
        thinningIteration(im, 0);
        thinningIteration(im, 1);
        cv::absdiff(im, prev, diff);
        im.copyTo(prev);
    }
    while (cv::countNonZero(diff) > 0);

    im *= 255;
}


/**
 * Code for thinning a binary image using Guo-Hall algorithm.
 * https://opencv-code.com/quick-tips/implementation-of-guo-hall-thinning-algorithm/
 */

/**
 * Perform one thinning iteration.
 * Normally you wouldn't call this function directly from your code.
 *
 * @param  im    Binary image with range = 0-1
 * @param  iter  0=even, 1=odd
 */
void thinningGuoHallIteration(cv::Mat& im, int iter)
{
    cv::Mat marker = cv::Mat::zeros(im.size(), CV_8UC1);

    for (int i = 1; i < im.rows; i++)
    {
        for (int j = 1; j < im.cols; j++)
        {
            uchar p2 = im.at<uchar>(i-1, j);
            uchar p3 = im.at<uchar>(i-1, j+1);
            uchar p4 = im.at<uchar>(i, j+1);
            uchar p5 = im.at<uchar>(i+1, j+1);
            uchar p6 = im.at<uchar>(i+1, j);
            uchar p7 = im.at<uchar>(i+1, j-1);
            uchar p8 = im.at<uchar>(i, j-1);
            uchar p9 = im.at<uchar>(i-1, j-1);

            int C  = (!p2 & (p3 | p4)) + (!p4 & (p5 | p6)) +
                     (!p6 & (p7 | p8)) + (!p8 & (p9 | p2));
            int N1 = (p9 | p2) + (p3 | p4) + (p5 | p6) + (p7 | p8);
            int N2 = (p2 | p3) + (p4 | p5) + (p6 | p7) + (p8 | p9);
            int N  = N1 < N2 ? N1 : N2;
            int m  = iter == 0 ? ((p6 | p7 | !p9) & p8) : ((p2 | p3 | !p5) & p4);

            if (C == 1 && (N >= 2 && N <= 3) & m == 0)
                marker.at<uchar>(i,j) = 1;
        }
    }

    im &= ~marker;
}

/**
 * Function for thinning the given binary image
 *
 * @param  im  Binary image with range = 0-255
 */
void im::thinningGuoHall(cv::Mat& im)
{
    im /= 255;

    cv::Mat prev = cv::Mat::zeros(im.size(), CV_8UC1);
    cv::Mat diff;

    do {
        thinningGuoHallIteration(im, 0);
        thinningGuoHallIteration(im, 1);
        cv::absdiff(im, prev, diff);
        im.copyTo(prev);
    }
    while (cv::countNonZero(diff) > 0);

    im *= 255;
}
